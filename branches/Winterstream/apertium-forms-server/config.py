#!/usr/bin/python2.5 -Wall
# coding=utf-8
# -*- encoding: utf-8 -*-

import sys, string, codecs, xml;
from xml.dom import minidom;
from xml import xpath;

from pair import *;

class Config: #{
	
	def __init__(self, filename): #{
		self.config = minidom.parse(filename).documentElement;
		self.pairs = {};
		self.working_directory = '';
	#}

	def parse_config(self): #{
		self.working_directory = self.config.getElementsByTagName('wd')[0].firstChild.nodeValue;
		self.cache_directory = self.config.getElementsByTagName('cache')[0].firstChild.nodeValue;

		path = '/webforms/pairs/pair';

		for node in xpath.Evaluate(path, self.config): #{
			pair_name = node.getAttribute('n');

			self.pairs[pair_name] = Pair(self.working_directory, pair_name, node);
			self.pairs[pair_name].cache = self.cache_directory + pair_name + '/';
			
			for enabled_tag in node.getElementsByTagName('tag'): #{
				tag_name = enabled_tag.getAttribute('n');
				shows = [];

				for show_tag in enabled_tag.getElementsByTagName('show'): #{
					shows.append(show_tag.getAttribute('syms'));
				#}

				for side in enabled_tag.getElementsByTagName('side'): #{
					paradigm_display_mode = '';
					current_side = side.getAttribute('n');

					for gloss_section in side.getElementsByTagName('paradigms'): #{
						paradigm_display_mode = gloss_section.getAttribute('display');
						for gloss in gloss_section.getElementsByTagName('paradigm'): #{
							name  = gloss.getAttribute('n');
							comment  = gloss.getAttribute('c');

							self.pairs[pair_name].dictionary[current_side].add_gloss(name, comment);
						#}
					#}
					
					self.pairs[pair_name].dictionary[current_side].set_display(paradigm_display_mode);
				#}

				self.pairs[pair_name].add_tag(tag_name, shows);
			#}
		#}
	#}

	def get_pairs(self): #{
		print self.pairs;
		return self.pairs;
	#}

	def get_pair(self,name): #{
		return self.pairs[name];
	#}

	def show_config(self): #{
		return self.config.toprettyxml();
	#}

#}
