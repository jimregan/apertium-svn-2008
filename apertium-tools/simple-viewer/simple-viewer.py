#!/usr/bin/python

import sys;
import pygtk;
import gtk;
import gtk.glade;

from apertium.service import make_proxy
from apertium.command_line import call

class Globals: #{
	marcar = False;
	pipeline_executor = None;
	handlers = {};
	wTree = None;
#}

class simple: #{

	def __init__(self): #{
		self.gladefile = "main_window.glade";
		Globals.wTree = gtk.glade.XML(self.gladefile, "app_window");
	#}
#}

def configure_combo(combo): #{
	combo.set_model(gtk.ListStore(gobject.TYPE_STRING));
	cell = gtk.CellRendererText();
	combo.pack_start(cell, True);
	combo.add_attribute(cell, 'text', 0);
	return combo;
#}

if __name__ == "__main__": #{
	s = simple();
	Globals.info = make_proxy("org.apertium.info/", "org.apertium.Info")
	comboPair = configure_combo(Globals.wTree.get_widget("comboPair"));

	for mode in Globals.info.modes(): #{
		comboPair.append_text(str(mode));
		comboPair.set_active(0);
	#}

	gtk.main();
#}
