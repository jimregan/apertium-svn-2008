#!/usr/bin/python2.5 -Wall
# coding=utf-8
# -*- encoding: utf-8 -*-

from config import Config;
from SimpleXMLRPCServer import SimpleXMLRPCServer;

import sys;
import codecs;
sys.stdout = codecs.getwriter('utf-8')(sys.stdout);
sys.stderr = codecs.getwriter('utf-8')(sys.stderr);

class Globals: #{
	counter = 0;
	config = Config('config/config.xml');	
#}

Globals.config.parse_config();

# Create server
server = SimpleXMLRPCServer(("localhost", 8081), allow_none = True);
server.register_introspection_functions();

def rpc_export(server, name = None): #{
    def decorator(f): #{
        if name != None:
            server.register_function(f, name)
        else:
            server.register_function(f)
        return f
    #}

    return decorator
#}

@rpc_export(server)
def language_pairs(): #{
    return Globals.config.get_pairs().keys();
#}

if __name__ == '__main__': #{
    server.serve_forever()
#}
