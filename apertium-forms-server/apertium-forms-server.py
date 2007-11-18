#!/usr/bin/python2.5 -Wall
# coding=utf-8
# -*- encoding: utf-8 -*-

import web, os, codecs, sys;
from config import Config;
from pair import Pair;
import tenjin
from tenjin.helpers import *   # or escape, to_str
engine = tenjin.Engine();

sys.stdout = codecs.getwriter('utf-8')(sys.stdout);
sys.stderr = codecs.getwriter('utf-8')(sys.stderr);



class Globals: #{
	config = Config(os.getcwd() + '/config/config.xml');	
#}

Globals.config.parse_config();

#render = web.template.render('templates/');

urls = (
    '/add', 'add',
    '/(.*)', 'form'
)

class form: #{

    def GET(self, name): #{
	pairs = Globals.config.get_pairs();
	tags = Globals.config.pairs['apertium-sh-mk'].get_tags();

	dictionary_left = Globals.config.pairs['apertium-sh-mk'].dictionary['left'];
	dictionary_right = Globals.config.pairs['apertium-sh-mk'].dictionary['right'];
	paradigms_left = Globals.config.pairs['apertium-sh-mk'].dictionary['left'].get_paradigms_by_tag('n');
	paradigms_right = Globals.config.pairs['apertium-sh-mk'].dictionary['right'].get_paradigms_by_tag('n');
	glosses_left = Globals.config.pairs['apertium-sh-mk'].dictionary['left'].get_glosses();
	glosses_right = Globals.config.pairs['apertium-sh-mk'].dictionary['right'].get_glosses();

	post_data = {
	    'selected_pair': pairs.keys()[0], 
	    'selected_tag': 'n', 
	    'tags': tags, 
	    'left_lemma': '', 
	    'right_lemma': '',
	    'left_paradigm': '',
	    'right_paradigm': '',
	    'left_glosses': glosses_left,
	    'right_glosses': glosses_right,
	    'left_display_mode': dictionary_left.get_display(),
	    'right_display_mode': dictionary_right.get_display(),
	    'left_paradigms': paradigms_left,
	    'right_paradigms': paradigms_right,
	    'restriction': '',
	    'pairs': pairs
	};

	print engine.render('templates/index.pyhtml', post_data);
    #}
#}

class add: #{

    def POST(self): #{
        post_data = web.input(name = []);
	pairs = Globals.config.get_pairs();

	current_pair = post_data['selected_pair'];
	current_tag = post_data['selected_tag'];
	left_paradigm = post_data['left_paradigm'];
	right_paradigm = post_data['right_paradigm'];

	dictionary_left = Globals.config.pairs[current_pair].dictionary['left'];
	dictionary_right = Globals.config.pairs[current_pair].dictionary['right'];

	tags = Globals.config.pairs[current_pair].get_tags();
	paradigms_left = dictionary_left.get_paradigms_by_tag(current_tag);
	paradigms_right = dictionary_right.get_paradigms_by_tag(current_tag);
	glosses_left = dictionary_left.get_glosses();
	glosses_right = dictionary_right.get_glosses();

	post_data = {
	    'selected_pair': current_pair, 
	    'selected_tag': current_tag, 
	    'tags': tags, 
	    'left_lemma': post_data['left_lemma'], 
	    'right_lemma': post_data['right_lemma'],
	    'left_paradigm': left_paradigm,
	    'right_paradigm': right_paradigm,
	    'left_paradigms': paradigms_left,
	    'left_glosses': glosses_left,
	    'right_glosses': glosses_right,
	    'left_display_mode': dictionary_left.get_display(),
	    'right_display_mode': dictionary_right.get_display(),
	    'right_paradigms': paradigms_right,
	    'restriction': post_data['restriction'],
	    'pairs': pairs
	};

	print engine.render('templates/index.pyhtml', post_data);

    #}
#}

if __name__ == "__main__": #{
    web.run(urls, globals());
#}
