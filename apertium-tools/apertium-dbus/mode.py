#!/usr/bin/env python

usage = """Usage:
python example-service.py &
"""
    
import os.path as path
import os

import apertium.service as service
from apertium.command_line import OsCommand


class Translate(service.Service):
    def __init__(self, cmdline, mode):
        service.Service.__init__(self, '/' + mode)
        self.cmd = OsCommand(cmdline)

    @service.method("org.apertium.Mode", in_signature='a{ss}s', out_signature='s')
    def translate(self, options, text):
        #default_options = {'show_missing': False}
        
        out, err = self.cmd.communicate(text)
        return out


def convert_to_dbus_name(name):
    import re
    return re.sub('[-]', '_', name)

def create_translation_objects():
    objs = []
    
    #try:
    info = service.make_proxy("org.apertium.info/", "org.apertium.Info")

    #except:
    #    raise Exception("Could not connect to the Apertium information service")

    directory = info.directory()
    for mode in info.modes():
        cmdline = [path.join(info.directory(), 'bin', 'apertium'), mode]
        objs.append(Translate(cmdline, convert_to_dbus_name(mode)))

    return objs
        

def quit_handler(*args):
    service.quit()
    
if __name__ == "__main__":
    objs = create_translation_objects()
    service.add_signal_receiver(quit_handler, dbus_interface = "org.apertium.General", signal_name = "QuitSignal")
    service.run_as("org.apertium.mode")
