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


def call(cmdline, _in=''):
    from subprocess import Popen, PIPE

    cmdline = [unicode(s) for s in cmdline]
    """A convenience function to invoke a subprocess with the
    parameter list name (where the first argument is the name
    of an executable). The subprocess is fed the contents of
    input via stdin. We collect the output of both stdout and
    stderr from the subprocess. If stderr is not empty, we
    raise an exception, otherwise we return the contents of
    stdout."""
    proc = Popen(cmdline, stdin=PIPE, stdout=PIPE, stderr=PIPE)
    out, err = proc.communicate(_in)

    print "input = ", _in
    print "name = ", repr(cmdline)
    print "out = ", out
    #print "err = ", err

    if not (err == None or err == ''):
        raise Exception(err)

    return unicode(out)


class Apertium(object):
    def __init__(self, directory, pair):
        self.cmd = path.join(directory, 'bin', 'apertium')
        self.pair = pair

    def run(self, options, input):
        return call([self.cmd, self.pair], input)


def convert_to_dbus_name(name):
    import re
    
    return re.sub('[-]', '_', name)

class Translate(service.Object):
    def __init__(self, session_bus, apertium):
        service.Object.__init__(self, session_bus, '/' + convert_to_dbus_name(apertium.pair))

        self.apertium = apertium

    @service.method("org.apertium.Translate", in_signature='a{ss}s', out_signature='s')
    def translate(self, options, text):
        return self.apertium.run(options, text)



def create_translation_objects(session_bus):
    objs = []
    
    try:
        info = dbus.Interface(session_bus.get_object("org.apertium.info", "/"),
                              "org.apertium.Info")

    except:
        raise Exception("The apertium information service is not running")

    directory = info.directory()
    for mode in info.modes():
        objs.append(Translate(session_bus, Apertium(directory, mode)))

    return objs
        

def main():
    try:
        dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)

        objs = create_translation_objects(dbus.SessionBus())

        name = service.BusName("org.apertium.translate", dbus.SessionBus())
        mainloop = gobject.MainLoop()
        mainloop.run()

    except Exception, e:
        print e
        exit(1)
    
if __name__ == "__main__":
    main()
