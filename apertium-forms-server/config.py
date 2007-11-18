#!/usr/bin/python2.5 -Wall
# coding=utf-8
# -*- encoding: utf-8 -*-

import sys, string, codecs, xml, os;
from xml.dom import minidom;
from xml import xpath;


import Ft;
from Ft.Xml.Domlette import NonvalidatingReader;
from Ft.Xml.XPath import Evaluate;

from pair import *;
sys.stdout = codecs.getwriter('utf-8')(sys.stdout);
sys.stderr = codecs.getwriter('utf-8')(sys.stderr);


class Config: #{
	
	def __init__(self, filename): #{
		self.config = NonvalidatingReader.parseUri('file:///' + filename);
		self.pairs = {};
		self.working_directory = None;
	#}

	def parse_config(self): #{
		self.working_directory = self.config.xpath('/webforms/directories/wd')[0].firstChild.nodeValue;
		self.cache_directory = self.config.xpath('/webforms/directories/cache')[0].firstChild.nodeValue;

		path = '/webforms/pairs/pair';

		for node in Ft.Xml.XPath.Evaluate(path, contextNode=self.config): #{
			#pair_name = node.getAttribute('n');
			pair_name = node.getAttributeNS(None, 'n');
			enabled = node.getAttributeNS(None, 'enabled');

			if enabled == 'no': #{
				continue;
			#}

			self.pairs[pair_name] = Pair(self.working_directory, pair_name, node);
			self.pairs[pair_name].cache = self.cache_directory + pair_name + '/';
			
			for enabled_tag in Ft.Xml.XPath.Evaluate(".//tag",contextNode=node): #{
				tag_name = enabled_tag.getAttributeNS(None, 'n');
				shows = [];

				for show_tag in Ft.Xml.XPath.Evaluate('.//show', contextNode= enabled_tag): #{
					shows.append(show_tag.getAttributeNS(None, 'syms'));
				#}

				for side in Ft.Xml.XPath.Evaluate('.//side', contextNode=enabled_tag): #{
					paradigm_display_mode = None;
					current_side = side.getAttributeNS(None, 'n');

					for gloss_section in Ft.Xml.XPath.Evaluate('.//paradigms', contextNode=side): #{
						paradigm_display_mode = gloss_section.getAttributeNS(None, 'display');

						for gloss in Ft.Xml.XPath.Evaluate('.//paradigm', contextNode=gloss_section): #{
							name  = gloss.getAttributeNS(None, 'n');
							comment  = gloss.getAttributeNS(None, 'c');

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
		return self.pairs;
	#}

	def get_pair(self,name): #{
		return self.pairs[name];
	#}

	def show_config(self): #{
		return self.config.toprettyxml();
	#}

#}
