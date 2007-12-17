#!/usr/bin/python

import os        
import new
import sys
from Queue import Queue
import thread 
from ConfigParser import ConfigParser, DuplicateSectionError, NoSectionError

import gobject
import gtk
import gtk.gdk
import gtk.glade
import pygtk

import dbus


class Serialize:
    # Don't judge me because of this HORRIBLE scheme.
    # I really did try to add methods to the GTK classes,
    # but Python complained loudly.
    
    @classmethod
    def Default(cls):
        def dump_state(self):
            return {}

        def load_state(self, p):
            pass

        return { "dump_state": dump_state,
                 "load_state": load_state }


    @classmethod
    def Window(cls):
        def dump_state(self):
            return { "x_size": self.get_size()[0],
                     "y_size": self.get_size()[1],
                     "x_pos":  self.get_position()[0], 
                     "y_pos":  self.get_position()[1] }

        def load_state(self, p):
            self.move(int(p["x_pos"]), int(p["y_pos"]))
            self.resize(int(p["x_size"]), int(p["y_size"]))

        return { "dump_state": dump_state,
                 "load_state": load_state }


    @classmethod
    def VPaned(cls):
        def dump_state(self):
            return { "position": self.get_position() }

        def load_state(self, p):
            self.set_position(int(p["position"]))

        return { "dump_state": dump_state,
                 "load_state": load_state }


class GladeXML(gtk.glade.XML):
    def get_codec(self, obj):
        codec = None
        try:
            codec = getattr(Serialize, obj.__class__.__name__)
        except:
            codec = Serialize.Default

        return codec()
    

    def get_widgets(self):
        return ((gtk.glade.get_widget_name(w), w) for w in  self.get_widget_prefix(''))

    
    def dump_gtk_state(self, cfg):
        for widget_name, widget in self.get_widgets():
            for key, val in self.get_codec(widget)['dump_state'](widget).iteritems():
                try:
                    cfg.add_section(widget_name)
                except DuplicateSectionError, e:
                    pass
                
                cfg.set(widget_name, key, str(val))


    def load_gtk_state(self, cfg):
        for widget_name, widget in self.get_widgets():
            try:
                self.get_codec(widget)['load_state'](widget, dict(cfg.items(widget_name)))
            except NoSectionError, e:
                pass


    def connect(self, context):
        handlers = dict((name, getattr(context, name)) for name in context.__class__.__dict__)

        self.signal_autoconnect(handlers)


class MainWindow:
    def setup_combo(self, info, combo):
        combo.set_model(gtk.ListStore(gobject.TYPE_STRING))
        cell = gtk.CellRendererText()
        combo.pack_start(cell, True)
        combo.add_attribute(cell, "text", 0)

        for mode in self.info.modes():
            combo.append_text(mode)

        combo.set_active(0)

        return combo


    def options(self):
        if self.chkMarkUnknown.get_active():
            return {"mark_unknown": "true"}
        else:
            return {}


    def load_config(self):
        self.config.readfp(open('defaults.cfg'))
        self.config.read([os.path.expanduser('~/.apertium-simple-viewer.cfg')])
        self.glade.load_gtk_state(self.config)

    def save_config(self):
        self.glade.dump_gtk_state(self.config)
        self.config.write(open(os.path.expanduser('~/.apertium-simple-viewer.cfg'), 'w'))
    
    def __init__(self, path):
        self.config = ConfigParser()
        self.bus = dbus.SessionBus()
        self.info = dbus.Interface(self.bus.get_object("org.apertium.info", "/"), "org.apertium.Info")
        self.input_queue = Queue()
        self.glade = GladeXML(path)
        self.glade.connect(self);

        self.buffer         = self.glade.get_widget("txtInput").get_buffer()
        self.output_buffer  = self.glade.get_widget("txtOutput").get_buffer()
        self.wndMain        = self.glade.get_widget("wndMain")
        self.dlgAbout       = self.glade.get_widget("dlgAbout")
        self.chkMarkUnknown = self.glade.get_widget("chkMarkUnknown")

        self.comboPair      = self.setup_combo(self.info, self.glade.get_widget("comboPair"))

        def on_changed(w):
            self.input_queue.put([self.options(), w.get_text(w.get_start_iter(), w.get_end_iter())])

        self.buffer.connect("changed", on_changed)

        def translator_loop():
            while True:
                options, _input = self.input_queue.get()
                while not self.input_queue.empty():
                    options, _input = self.input_queue.get()

                try:
                    result = self.translator.translate(options, _input)

                    def update_text():
                        self.output_buffer.set_text(result)
                        return False

                    gobject.idle_add(update_text)
                    
                except Exception, e:
                    print e

        thread.start_new_thread(translator_loop, ())

        self.load_config()
        self.wndMain.show()

    def on_btnQuit_clicked(self, widget):
        self.save_config()
        gtk.main_quit()

    def on_wndMain_destroy(self, widget):
        gtk.main_quit()

    def on_wndMain_delete_event(self, widget, event):
        self.save_config()
        return False

    def on_btnAbout_clicked(self, widget):
        self.dlgAbout.show()

    def on_dlgAbout_response(self, widget, response):
        self.dlgAbout.hide()

    def on_comboPair_changed(self, widget):
        pair_name = widget.get_model().get_value(widget.get_active_iter(), 0)
        dbus_pair_name = "/" + "_".join(pair_name.split("-"))
        self.translator = dbus.Interface(self.bus.get_object("org.apertium.mode", dbus_pair_name), "org.apertium.Mode")
        self.buffer.emit("changed")

    def on_chkMarkUnknown_toggled(self, widget):
        self.buffer.emit("changed")

if __name__ == "__main__":
    gtk.gdk.threads_init()
    wnd = MainWindow("main_window.glade")
    gtk.main();

