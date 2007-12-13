#!/usr/bin/env python

usage = """Usage:
python example-service.py &
"""
    
from optparse import OptionParser, make_option
import os.path as path
import os

import gobject

import dbus
import dbus.service as service
import dbus.mainloop.glib 

class Info(service.Object):
    def __init__(self, session_bus, apertium_dir):
        service.Object.__init__(self, session_bus, '/')

        self._session_bus = session_bus
        self._modes = []
        self._directory = apertium_dir
        
        self.parse_modes(path.join(apertium_dir, "share", "apertium", "modes"))

    @service.method("org.apertium.Info", in_signature='', out_signature='s')
    def directory(self):
        return self._directory

    @service.method("org.apertium.Info", in_signature='', out_signature='as')
    def modes(self):
        return self._modes

    def parse_modes(self, modes_path):
        for dirent in os.listdir(modes_path):
            fname, ext = path.splitext(dirent)
            if ext == ".mode":
                self._modes.append(fname)



option_list = [
    make_option("-p", "--path", dest="filename", type="string",
                help="The prefix of the Apertium installation."),
    make_option("-v", "--verbose",
                help="Be verbose")
    ]

usage = "usage: %prog [options] arg"

def main():
    dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)

    parser = OptionParser(usage, option_list = option_list)
    options, args = parser.parse_args()

    info = Info(dbus.SessionBus(), options.filename)

    name = service.BusName("org.apertium.info", dbus.SessionBus())
    mainloop = gobject.MainLoop()
    mainloop.run()

if __name__ == "__main__":
    main()
