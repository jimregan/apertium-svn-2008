#!/usr/bin/python2.5 -Wall
# coding=utf-8
# -*- encoding: utf-8 -*-

import web, os;
from config import Config;
from pair import Pair;

class Globals: #{
	config = Config(os.getcwd() + '/config/config.xml');	
#}

Globals.config.parse_config();

render = web.template.render('templates/');

urls = (
    '/add', 'add',
    '/(.*)', 'form'
)

class form: #{

    def GET(self, name): #{
	pairs = Globals.config.get_pairs();
	tags = Globals.config.pairs['apertium-es-ca'].get_tags();
	paradigms_left = Globals.config.pairs['apertium-es-ca'].dictionary['left'].get_paradigms_by_tag('n');
	paradigms_right = Globals.config.pairs['apertium-es-ca'].dictionary['right'].get_paradigms_by_tag('n');
	post_data = {
	    'selected_pair': pairs.keys()[0], 
	    'selected_tag': 'n', 
	    'left_lemma': '', 
	    'right_lemma': ''
	};
        print post_data;
	print render.index(pairs, tags, paradigms_left, paradigms_right, post_data, cache=False);
    #}
#}

class add: #{

    def POST(self): #{
        post_data = web.input(name = []);
	pairs = Globals.config.get_pairs();

        print post_data;

	tags = Globals.config.pairs[pairs.keys()[0]].get_tags();
	print render.index(pairs, tags, post_data, cache=False);
    #}
#}

if __name__ == "__main__": #{
    web.run(urls, globals());
#}
