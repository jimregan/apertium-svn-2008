#!/usr/bin/env python

# GTK oriented packages
import gtk, sys
import pygtk
pygtk.require('2.0')

# Logging
import logging

# Process oriented packages
from subprocess import Popen, PIPE
import threading
from Queue import Queue

# XML packages
import xml
try:
    import xml.etree.ElementTree as ET # Python 2.5
except:
    import elementtree.ElementTree as ET

# Our config file
import config 

# Custom widget with horizontal pane allowing for the easy
# vertical resizing of boxes
from VSizerPane import VSizerPane



# Global variables
class Globals:
    lang_code = ''
    mode = ''
    stages = None # Linked list of stages
    pipeline_executor = None



class View(gtk.Expander):
    """
    A GTK expander containing a scrollable text window and a
    VSizerPane at the bottom
    """
    window = None # if the text box is detached into a window

    def __init__(self, label, text_buffer):
        gtk.Expander.__init__(self, label)

        text_view = gtk.TextView(text_buffer)
        text_view.set_editable(True)
        text_view.set_wrap_mode(gtk.WRAP_WORD)
        text_view.show()

        scrolled_window = gtk.ScrolledWindow()
        scrolled_window.show()
        scrolled_window.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)
        scrolled_window.add_with_viewport(text_view)
        scrolled_window.set_size_request(-1, 100)

        sizer_pane = VSizerPane('handle.xpm', scrolled_window)
        sizer_pane.show()
        
        vbox = gtk.VBox(homogeneous = False)
        vbox.show()
        vbox.pack_start(scrolled_window, expand = True, fill = True)
        vbox.pack_start(sizer_pane, expand = False, fill = True)

        self.add(vbox)
        self.set_expanded(True)
    


class PipelineExecutor(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.queue = Queue()
        self.setDaemon(True)

    def run(self):
        while True:
            stage = self.queue.get()
            
            while self.queue.qsize() > 0:
                stage = self.queue.get()

            stage.run()

    def add(self, stage):
        self.queue.put(stage)
    


class Stage:
    def __init__(self, command_line):
        logging.debug('creating stage with command_line %s' % str(command_line))
        self.command_line = command_line
        self.next = None
        
        self.text_buffer = gtk.TextBuffer()
        self.update_handler = self.text_buffer.connect("changed", self.update)

    def run(self):
        if self.next == None:
            return
        
        proc = Popen(self.next.command_line, stdin = PIPE, stdout = PIPE)

        try:
            out, err = proc.communicate(self.text_buffer.get_text(self.text_buffer.get_start_iter(),
                                                                  self.text_buffer.get_end_iter()))

            gtk.gdk.threads_enter()
            self.next.text_buffer.handler_block(self.next.update_handler)
            self.next.text_buffer.set_text(out.strip())
            self.next.text_buffer.handler_unblock(self.next.update_handler)
            gtk.gdk.threads_leave()

            self.next.run()

        except Exception, e:
            logging.error("Cripes! %s" % str(e))

    def update(self, widget, *args):
        Globals.pipeline_executor.add(self)

    def __iter__(self):
        def itr(stage):
            while stage != None:
                yield stage
                stage = stage.next

        return itr(self)


def menu_item(label, item = None):
    menu_item = gtk.MenuItem(label)
    menu_item.show()

    if type(item) == type(lambda: 2):
        menu_item.connect("activate", item)

    elif type(item) == gtk.Menu: 
        menu_item.set_submenu(item)

    return menu_item

def menu(*items):
    menu = gtk.Menu()

    for item in items:
        menu.append(item)

    return menu

def menubar():
    bar_items = (menu_item("File", menu(menu_item("Open", menu_file_open),
                                        menu_item("Exit", quit))),
                 menu_item("Compile"))

    menu_bar = gtk.MenuBar()
    for item in bar_items:
        menu_bar.append(item)
    menu_bar.show()

    return menu_bar



def load_mode_file(filename):
    pass

def menu_file_open(widget, data = None):
    if "open_dialog" not in self.__dict__:
        self.open_dialog = gtk.FileSelection("Select the modes file")
        self.open_dialog.ok_button.connect("clicked",
                                           load_mode_file(self.open_dialog.get_filename()))
        self.open_dialog.ok_button.connect("clicked",
                                           lambda w: self.open_dialog.hide())
        self.open_dialog.cancel_button.connect("clicked",
                                               lambda w: self.open_dialog.hide())

    self.open_dialog.show()



def quit(widget, data = None):
    gtk.main_quit()

def delete_event(widget, event, data = None):
    return False

def main_window():
    def make_stage_name(command_line):
        s = " ".join(command_line)
        
        if len(s) > 64:
            return s[0:64] + "..."
        else:
            return s
    
    def make_handle_box(stages):
        view_box = gtk.VBox(homogeneous = False)

        view_list = []

        for stage in stages:
            view = View(make_stage_name(stage.command_line), stage.text_buffer)
            view_list.append(view)
            view.show()
            view_box.pack_start(view, expand = False, fill = True)

        for view in view_list[1:-1]:
            view.set_expanded(False)

        return view_box
    
    view_box = make_handle_box(Globals.stages)
    view_box.show()

    vbox = gtk.VBox(homogeneous = False, spacing = 5)
    vbox.show()

    vbox.pack_start(menubar(), expand = False)
    vbox.pack_start(view_box, expand = True)

    scrolled_window = gtk.ScrolledWindow()
    scrolled_window.set_policy(gtk.POLICY_NEVER, gtk.POLICY_AUTOMATIC)
    scrolled_window.show()
    scrolled_window.add_with_viewport(vbox)

    window = gtk.Window(gtk.WINDOW_TOPLEVEL)
    window.add(scrolled_window)

    window.connect("delete_event", delete_event)
    window.connect("destroy", quit)

    window.resize(500, 400)

    window.show()
    


def apertium_program(program):
    # TODO: Make this search for the Apertium path
    return "%s/%s" % (config.apertium_bin_path, program)

def apertium_dictionary(dictionary):
    # TODO: Make this search for the Apertium path
    return "%s/apertium-%s/%s" % (config.apertium_dict_path, Globals.lang_code, dictionary) 

def setup_mode(mode):
    stages = Stage("")

    def add(stages):
        for program in mode.findall('.//program'):
            command = apertium_program(program.attrib['name']).split(' ')
            if len(command) > 1 and command[1] == '$1':
                command[1] = '-g'

            for param in program.findall('.//file'):
                command.append(apertium_dictionary(param.attrib['name']))

            stages.next = Stage(tuple(command))
            stages = stages.next

    add(stages)

    return stages



def setup_logging():
    logging.basicConfig(level=logging.DEBUG,
                        format='%(asctime)s %(levelname)-8s %(message)s',
                        datefmt='%a, %d %b %Y %H:%M:%S',
                        filename='apertium.log',
                        filemode='w')



def print_usage():
    print "usage: apertium-view [<modes file>] [<language code>]"

def process_command_line():
    import sys

    logging.debug('Command line is %s' % sys.argv)
    Globals.lang_code = sys.argv[1]

    def process_modes(mode_file, mode_code = None):
        logging.debug("Using mode file %s" % mode_file)
        tree = ET.parse(mode_file)

        def find_mode():
            if mode_code != None:
                logging.debug('Looking for mode code %s' % mode_code)
                for mode in tree.findall('mode'):
                    if mode.attrib['name'] == mode_code:
                        return mode

                    print_usage()

            else:
                logging.debug('No mode code specified, using first one in file')
                return tree.find('mode')

        return find_mode()

    Globals.mode = process_modes(*sys.argv[2:4])
    logging.debug('Parsed command line')



def init():
    setup_logging()
    process_command_line()
    Globals.stages = setup_mode(Globals.mode)
    Globals.pipeline_executor = PipelineExecutor()
    Globals.pipeline_executor.start()
    main_window()

if __name__ == "__main__":
    if len(sys.argv) < 3: #{
        print 'Usage: apertium-view.py <pair name> <modes file> [direction]';
        sys.exit(-1);
    #}

    gtk.gdk.threads_init()
    init()
    logging.debug('Completed init phase')
    gtk.main()
    logging.debug('Graceful shutdown')

