#!/usr/bin/python

import sys
import pygtk
import gtk
import gtk.gdk
import gtk.glade
import dbus
import gobject
from Queue import Queue
import thread 


class GladeXML(gtk.glade.XML):
    def dump_gtk_state(self, directory, _dict):
        for widget_name, widget in [(gtk.glade.get_widget_name(w), w) for w in  self.get_widget_prefix('')]:
            for key, val in widget.dump_state().iter_items():
                _dict[widget_name + '.' + key] = str(val)

    def load_gtk_state(self, directory, _dict):
        pass # We'll figure this out when we have selected a config format


handlers = {}

def gtk_handler(f):
    handlers[f.__name__] = f
    return f


def glade_load_and_connect(fname, context, **kwargs):
    widget_tree = GladeXML(fname, **kwargs)

    handlers = dict((name, getattr(context, name)) for name in context.__class__.__dict__)
    
    widget_tree.signal_autoconnect(handlers)
    return widget_tree
        

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
        
    
    def __init__(self, path):
        self.bus = dbus.SessionBus()
        self.info = dbus.Interface(self.bus.get_object("org.apertium.info", "/"), "org.apertium.Info")
        self.input_queue = Queue()
        self.glade = glade_load_and_connect(path, self);

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

        self.wndMain.show()

    def on_btnQuit_clicked(self, widget):
        #self.glade.dump_gtk_state()
        gtk.main_quit()

    def on_wndMain_destroy(self, widget):
        gtk.main_quit()

    def on_wndMain_delete_event(self, widget, event):
        #self.glade.dump_gtk_state()
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

