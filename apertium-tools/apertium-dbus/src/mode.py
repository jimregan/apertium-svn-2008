#!/usr/bin/env python

usage = """Usage:
python example-service.py &
"""
    
import os.path as path
import os
import logging

import apertium.service as service
from apertium.command_line import call

class Translate(service.Service):
    def __init__(self, cmdline, mode):
        service.Service.__init__(self, '/' + mode)
        self.cmdline = cmdline

    @service.method("org.apertium.Mode", in_signature='a{ss}s', out_signature='s')
    def translate(self, options, text):
        def parse_bool(string):
            if string == 'true':
                return True
            elif string == 'false':
                return False
            else:
                raise Exception("Illegal boolean value")
        
        default_options = {'mark_unknown': 'false'}
        default_options.update(options)

        logging.debug("Seeing options %s" % repr(options))

        cmdline = list(self.cmdline)
        if not parse_bool(default_options['mark_unknown']):
            cmdline.append('-u')
        
        logging.debug("Cmdline == %s" % repr(cmdline))

        out, err = call(cmdline, text)
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

def setup_logging():
    logging.basicConfig(level=logging.DEBUG,
                        format='%(asctime)s %(levelname)-8s %(message)s',
                        datefmt='%a, %d %b %Y %H:%M:%S',
                        filename='/tmp/mode.log',
                        filemode='w')
    
if __name__ == "__main__":
    setup_logging()
    objs = create_translation_objects()
    service.add_signal_receiver(quit_handler, dbus_interface = "org.apertium.General", signal_name = "QuitSignal")
    service.run_as("org.apertium.mode")
