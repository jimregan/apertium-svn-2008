#!/usr/bin/env python

import types

import pygtk
pygtk.require('2.0')
import gtk
import xml
import logging
from subprocess import Popen, PIPE

try:
    import xml.etree.ElementTree as ET # Python 2.5
except:
    try:
        import elementtree.ElementTree as ET
    except:
        print "Could not load the ElementTree module"
        exit(1)

env = {} # my idiosyncratic way of storing globals

class FoldBox(gtk.HBox):
    def __init__(self, text_buffer):
        gtk.HBox.__init__(self, homogeneous = False)

        text_view = gtk.TextView(text_buffer)
        text_view.set_editable(True)
        text_view.set_wrap_mode(gtk.WRAP_WORD)
        text_view.show()
        
        scrolled_window = gtk.ScrolledWindow()
        scrolled_window.show()
        scrolled_window.add_with_viewport(text_view)

        self.pack_start(scrolled_window)

class FoldBoxRow(gtk.HBox):
    def __init__(self, stages):
        gtk.HBox.__init__(self, stages)

        for stage in stages:
            fold_box = FoldBox(stage.text_buffer)
            fold_box.show()
            self.pack_start(fold_box)



class Stage:
    def __init__(self, command_line):
        logging.debug('creating stage with command_line %s' % str(command_line))
        self.text_buffer = gtk.TextBuffer()
        self.command_line = command_line

    def run(self):
        logging.debug("Running %s" % str(self.command_line))

    def update(self, widget, previous_buffer):
        logging.debug("Running %s" % str(self.command_line))
        p = Popen(self.command_line, stdin = PIPE, stdout = PIPE)
        out, err = p.communicate(previous_buffer.get_text(previous_buffer.get_start_iter(),
                                                          previous_buffer.get_end_iter()))
        self.text_buffer.set_text(out)

    
def menu_item(label, item = None):
    menu_item = gtk.MenuItem(label)
    menu_item.show()

    if type(item) == types.MethodType:
        menu_item.connect("activate", item)

    elif type(item) == gtk.Menu:
        menu_item.set_submenu(item)

    return menu_item

def menu(*items):
    menu = gtk.Menu()

    for item in items:
        menu.append(item)

    return menu

env['apertium-bin-path'] = '/usr/local/bin'
env['apertium-dict-path'] = '/usr/local/share/apertium'

def apertium_program(program):
    # TODO: Make this search for the Apertium path
    return "%s/%s" % (env['apertium-bin-path'], program)

def apertium_dictionary(dictionary):
    # TODO: Make this search for the Apertium path
    return "%s/apertium-%s/%s" % (env['apertium-dict-path'], env['lang-code'], dictionary) 



def print_usage(self):
    raise Exception("usage: apertium-view [<modes file>] [<language code>]")



def setup_mode(mode):
    stages = [Stage("")]

    for program in mode.findall('.//program'):
        command = apertium_program(program.attrib['name']).split(' ')

        for param in program.findall('.//file'):
            command.append(apertium_dictionary(param.attrib['name']))

        stages.append(Stage(tuple(command)))

    for i, stage in enumerate(stages[0:-1]):
        stage.text_buffer.connect("changed", stages[i + 1].update, stage.text_buffer)

    return stages



class Apertium:
    stages = []

    def __init__(self):
        def create_window():        
            fold_box_row = FoldBoxRow(self.stages)
            fold_box_row.show()

            vbox = gtk.VBox(homogeneous = False, spacing = 5)
            vbox.show()
            
            vbox.pack_start(self.menubar(), expand = False)
            vbox.pack_start(fold_box_row, expand = True)

            self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
            self.window.add(vbox)
        
            self.window.connect("delete_event", self.delete_event)
            self.window.connect("destroy", self.quit)

            self.window.show()

        self.setup_debugging()
        self.process_command_line()
        logging.debug('parsed command line')

        self.stages = setup_mode(env['mode'])
        create_window()

    def setup_debugging(self):
        logging.basicConfig(level=logging.DEBUG,
                            format='%(asctime)s %(levelname)-8s %(message)s',
                            datefmt='%a, %d %b %Y %H:%M:%S',
                            filename='apertium.log',
                            filemode='w')


    def process_command_line(self):
        import sys
        logging.debug('argv == %s' % sys.argv)

        env['lang-code'] = sys.argv[1]

        def process_modes(mode_file, mode_code = None):
            logging.debug("mode_file == %s" % mode_file)
            tree = ET.parse(mode_file)

            def find_mode():
                if mode_code != None:
                    for mode in tree.findall('mode'):
                        if mode.attrib['name'] == mode_code:
                            return mode

                        self.print_usage()

                else:
                    logging.debug('returning first mode')
                    return tree.find('mode')

            return find_mode()

        env['mode'] = process_modes(*sys.argv[2:4])
                

    def load_mode_file(self, filename):
        tree = ET.parse(filename)

    def quit(self, widget, data = None):
        gtk.main_quit()

    def delete_event(self, widget, event, data = None):
        return False

    def menu_file_open(self, widget, data = None):
        if "open_dialog" not in self.__dict__:
            self.open_dialog = gtk.FileSelection("Select the modes file")
            self.open_dialog.ok_button.connect("clicked",
                                               self.load_mode_file(self.open_dialog.get_filename()))
            self.open_dialog.ok_button.connect("clicked",
                                               lambda w: self.open_dialog.hide())
            self.open_dialog.cancel_button.connect("clicked",
                                                   lambda w: self.open_dialog.hide())

        self.open_dialog.show()

    def menubar(self):
        bar_items = (menu_item("File", menu(menu_item("Open", self.menu_file_open),
                                            menu_item("Exit", self.quit))),
                     menu_item("Compile"))
        
        menu_bar = gtk.MenuBar()
        for item in bar_items:
            menu_bar.append(item)
        menu_bar.show()

        return menu_bar
        
    def main(self):
        gtk.main()
        logging.debug('Graceful shutdown')

if __name__ == "__main__":
    try:
        apertium = Apertium()
        apertium.main()
        
    except Exception, e:
        print e
        logging.error(e)
        exit(1)
