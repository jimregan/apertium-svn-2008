#!/usr/bin/python

import sys;
import pygtk;
import gtk;
import gtk.glade;

class simple: #{

	def __init__(self): #{
		self.gladefile = "main_window.glade";
		self.window = gtk.glade.XML(self.gladefile, "app_window");
	#}
#}

if __name__ == "__main__": #{
	s = simple();
	gtk.main();
#}
