#!/usr/bin/env python

# GTK oriented packages
import gobject
import gtk, sys
import pygtk

pygtk.require('2.0')

# Logging
import logging

# Process oriented packages
from subprocess import Popen, PIPE
import threading
from Queue import Queue

import dbus

from widget import *
import TextWidget 

# Global variables
class Globals:
    marcar = False
    pipeline_executor = None
    source_lang_manager = None
    source_style_manager = None
    handlers = {}
    wTree = None


class PipelineExecutor(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.queue = Queue()
        self.setDaemon(True)
        self.last_f = lambda: None

    def run(self):
        while True:
            self.last_f = self.queue.get()
            
            while self.queue.qsize() > 0:
                self.last_f = self.queue.get()

            self.last_f()

    def add(self, stage):
        self.queue.put(stage)

    def reexec(self):
        self.add(self.last_f)
        

class Cell(object):
    """A really simple dataflow class. It only supports building dataflow chains.

    >>> a = lambda x: 'a' + x + 'a'
    >>> b = lambda x: 'b' + x + 'b'
    >>> c_a = Cell(a)
    >>> c_b = c_a.set_next(Cell(b))
    >>> c_a('?')
    'ba?ab'

    It just like a chain of lazy function invocations. This class supports the
    execution of the Apertium pipeline. For each Apertium module, there will be
    a cell. After every such cell, there is a cell which contains a function to
    update one of our text views. 
    """
    def __init__(self, func):
        self.func = func
        self.next = lambda x: x

    def __call__(self, val):
        out = self.func(val)
        return self.next(out)


# Widget convenience functions

def show(widget):
    widget.show()
    return widget


def configure_combo(combo):
    combo.set_model(gtk.ListStore(gobject.TYPE_STRING))
    cell = gtk.CellRendererText()
    combo.pack_start(cell, True)
    combo.add_attribute(cell, 'text', 0)
    return combo


def make_text_buffer():
    buf = sourceview.Buffer()
    buf.set_language(Globals.source_lang_manager.get_language('apertium'))
    buf.set_style_scheme(Globals.source_style_manager.get_scheme('tango'))
    buf.set_highlight_syntax(True)
    buf.set_highlight_matching_brackets(False)

    return buf


def make_text_widget(cmd):
    text_buffer = make_text_buffer()
    src_view = show(make_source_view(text_buffer))
    return text_buffer, TextWidget.make(" ".join(cmd), src_view)


def text_window(title, text_buffer):
    wTree = gtk.glade.XML("TextWindow.glade")
    
    wnd = wTree.get_widget("text_window")
    wnd.set_title(title)

    def close(widget, data = None):
        wnd.hide()
        wnd.destroy()

    wnd.connect("destroy", close)

    text_view = make_source_view(text_buffer)
    scrolled_window = wTree.get_widget("scrolled_window")
    scrolled_window.add_with_viewport(text_view)
    
    wTree.signal_autoconnect({'on_btn_close_clicked': close})
    
    wnd.show()    


# GTK Handlers which are automatically connected to the Glade
# events with the same names

@gtk_handler
def on_wndMain_destroy(widget, data = None):
    gtk.main_quit()


@gtk_handler
def on_btnQuit_clicked(widget, data = None):
    gtk.main_quit()


@gtk_handler
def on_wndMain_delete_event(widget, event, data = None):
    return False


@gtk_handler
def on_chkMarkUnknown_toggled(widget, data = None):
    Globals.marcar = not Globals.marcar
    Globals.pipeline_executor.reexec()


@gtk_handler
def on_comboPair_changed(widget, data = None):
    setup_pair(widget.get_model().get_value((widget.get_active_iter()), 0))


def call(params, _input):
    from subprocess import Popen, PIPE
    
    proc = Popen(params, stdin=PIPE, stdout=PIPE, stderr=PIPE)
    return proc.communicate(_input)
    

def make_runner(cmd):
    def process_cmd_line(cmd):
        if len(cmd) > 1   and cmd[1] == '$1' and Globals.marcar:
            return cmd[0], '-g', cmd[2];
        elif len(cmd) > 1 and cmd[1] == '$1' and not Globals.marcar:
            return cmd[0], '-n', cmd[2];
        else:
            return cmd


    def runner(val):
        out, err = call(list(process_cmd_line(cmd)), str(val))
        return out

    return Cell(runner)


def make_observer(text_buffer, update_handler):
    def observer(val):
        gtk.gdk.threads_enter()
        text_buffer.handler_block(update_handler)
        text_buffer.set_text(val)
        text_buffer.handler_unblock(update_handler)
        gtk.gdk.threads_leave()
        return val

    return Cell(observer)


def update(widget, runner):
    def get_text(buf):
        return buf.get_text(buf.get_start_iter(), buf.get_end_iter())

    Globals.pipeline_executor.add(lambda: runner(get_text(widget)))


def replace_child(container, new_child):
    child = container.get_children()[0] # we must keep a reference (child) before removing it from portMain
    container.remove(child)
    container.add(new_child)


def setup_pair(name):
    def next(cell, obj):
        cell.next = obj
        return cell.next

    pack_opts = {'expand': False, 'fill': True}

    view_box = show(gtk.VBox(homogeneous = False))

    in_filter, out_filter = Globals.info.get_filters('txt') # this will likely be apertium-destxt and apertium-retxt
    cell = make_runner([in_filter]) # Add the deformatter

    text_buffer, text_widget = make_text_widget(['input text'])
    text_buffer.connect("changed", update, cell)
    view_box.pack_start(text_widget, **pack_opts)

    pipeline = Globals.info.get_pipeline(name)
    for i, cmd in enumerate(pipeline):
        cell = next(cell, make_runner([str(c) for c in cmd]))

        text_buffer, text_widget = make_text_widget(cmd)
        
        update_cell = Cell(lambda x: x)
        update_handler = text_buffer.connect("changed", update, update_cell)

        if i == len(pipeline) - 1:
            cell = next(cell, make_runner([out_filter]))
        
        cell = next(cell, make_observer(text_buffer, update_handler))
        cell = next(cell, update_cell)
        
        view_box.pack_start(text_widget, **pack_opts)

    replace_child(Globals.wTree.get_widget("portMain"), view_box)


def main_window():
    Globals.wTree = glade_load_and_connect("MainWindow.glade")
    comboPair = configure_combo(Globals.wTree.get_widget("comboPair"))
    
    for mode in Globals.info.modes():
        comboPair.append_text(str(mode))
        
    comboPair.set_active(0)
    
    
def setup_logging():
    logging.basicConfig(level=logging.DEBUG,
                        format='%(asctime)s %(levelname)-8s %(message)s',
                        datefmt='%a, %d %b %Y %H:%M:%S',
                        filename='apertium.log',
                        filemode='w')


def init():
    Globals.pipeline_executor = PipelineExecutor()
    Globals.pipeline_executor.start()

    Globals.source_lang_manager  = sourceview.language_manager_get_default()
    Globals.source_style_manager = sourceview.style_scheme_manager_get_default()
    Globals.info = dbus.Interface(dbus.SessionBus().get_object("org.apertium.info", "/"), "org.apertium.Info")

    setup_logging()
    main_window()


if __name__ == "__main__":
    gtk.gdk.threads_init()
    init()
    logging.debug('Completed init phase')
    gtk.main()
    logging.debug('Graceful shutdown')

