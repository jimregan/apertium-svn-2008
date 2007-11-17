#!/usr/bin/python2.5 -Wall
# coding=utf-8
# -*- encoding: utf-8 -*-

import web;
from config import Config;
from pair import Pair;

class Globals: #{
	counter = 0;
	config = Config('config/config.xml');	
#}

Globals.config.parse_config();

render = web.template.render('templates/');

urls = (
    '/add', 'add',
    '/(.*)', 'form'
)

class form: #{

    def GET(self, name): #{
	pairs = Globals.config.get_pairs().keys();
	post_data = {'selected_pair': 'apertium-es-gl'};
	print render.index(pairs, post_data, cache=False);
    #}
#}

class add: #{

    def POST(self): #{
        post_data = web.input(name = []);
        print dir(post_data);
        print post_data
	pairs = Globals.config.get_pairs().keys();
	print render.index(pairs, post_data, cache=False);
    #}
#}

if __name__ == "__main__": #{
    web.run(urls, globals());
#}
