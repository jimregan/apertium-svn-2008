#!/usr/bin/env python

# GTK oriented packages
import gobject
import gtk, sys
import pygtk

pygtk.require('2.0')

try:
    import gtksourceview2 as sourceview
    
except:
    import SourceViewDummy as sourceview


# Logging
import logging

# Process oriented packages
from subprocess import Popen, PIPE
import threading
from Queue import Queue

from apertium.service import make_proxy
from apertium.command_line import call
from widget import *
import TextWidget 

# Custom widget with horizontal pane allowing for the easy
# vertical resizing of boxes
from VSizerPane import VSizerPane

# Global variables
class Globals:
    lang_code = ''
    mode = ''
    marcar = False
    stages = None # Linked list of stages
    pipeline_executor = None
    source_lang_manager = None
    source_style_manager = None
    handlers = {}
    wTree = None


def show(widget):
    widget.show()
    return widget


def make_source_view(text_buffer):
    text_view = sourceview.View(text_buffer)
    text_view.set_editable(True)
    text_view.set_wrap_mode(gtk.WRAP_WORD_CHAR)
    text_view.show()
    
    return text_view


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
    

class PipelineExecutor(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.queue = Queue()
        self.setDaemon(True)

    def run(self):
        while True:
            f = self.queue.get()
            
            while self.queue.qsize() > 0:
                f = self.queue.get()

            f()

    def add(self, stage):
        self.queue.put(stage)
    


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

@gtk_handler
def on_comboPair_changed(widget, data = None):
    setup_pair(widget.get_model().get_value((widget.get_active_iter()), 0))
    #setup_pair(widget.


class Cell(object):
    def __init__(self, func):
        self.func = func
        self.next = lambda x: x

    def __call__(self, val):
        out = self.func(val)
        return self.next(out)

    def set_next(self, next):
        self.next = next
        return next


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

def process_cmd_line(cmd):
    if len(cmd) > 1   and cmd[1] == '$1' and Globals.marcar:
        return cmd[0], '-g', cmd[2];
    elif len(cmd) > 1 and cmd[1] == '$1' and not Globals.marcar:
        return cmd[0], '-n', cmd[2];
    else:
        return cmd
    

def make_runner(cmd):
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
    view_box = show(gtk.VBox(homogeneous = False))
    view_box.set_resize_mode(gtk.RESIZE_PARENT)

    in_filter, out_filter = Globals.info.get_filters('txt')
    cell = make_runner([in_filter]) # Add the deformatter

    text_buffer, text_widget = make_text_widget(['input text'])
    update_handler = text_buffer.connect("changed", update, cell)
    view_box.pack_start(text_widget, expand = False, fill = True)

    for cmd in Globals.info.get_pipeline(name):
        cell = cell.set_next(make_runner([str(c) for c in cmd]))

        text_buffer, text_widget = make_text_widget(cmd)
        update_cell = Cell(lambda x: x)
        update_handler = text_buffer.connect("changed", update, update_cell)
        cell = cell.set_next(make_observer(text_buffer, update_handler))
        cell = cell.set_next(update_cell)
        
        view_box.pack_start(text_widget, expand = False, fill = True)

    replace_child(Globals.wTree.get_widget("portMain"), view_box)


def checkbox_event(widget, *args):
    if Globals.marcar == 1:
      Globals.marcar = 0
    else:
      Globals.marcar = 1

    Globals.stages.update(widget);


def configure_combo(combo):
    combo.set_model(gtk.ListStore(gobject.TYPE_STRING))
    cell = gtk.CellRendererText()
    combo.pack_start(cell, True)
    combo.add_attribute(cell, 'text', 0)
    return combo

def main_window():
    Globals.wTree = glade_load_and_connect("MainWindow.glade")

    comboPair = configure_combo(Globals.wTree.get_widget("comboPair"))
    
    for mode in Globals.info.modes():
        print str(mode)
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
    Globals.info = make_proxy("org.apertium.info/", "org.apertium.Info")

    setup_logging()
    #process_command_line()
    #Globals.stages = setup_mode(Globals.mode)
    main_window()

if __name__ == "__main__":
    gtk.gdk.threads_init()
    init()
    logging.debug('Completed init phase')
    gtk.main()
    logging.debug('Graceful shutdown')

