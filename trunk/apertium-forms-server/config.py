#!/usr/bin/python2.5 -Wall
# coding=utf-8
# -*- encoding: utf-8 -*-

import sys, string, codecs, xml, os, Ft, re, md5, cStringIO;
from Ft.Xml.Domlette import NonvalidatingReader;
from Ft.Xml.XPath import Evaluate;
from pair import *;

sys.stdout = codecs.getwriter('utf-8')(sys.stdout);
sys.stderr = codecs.getwriter('utf-8')(sys.stderr);

class Config: #{
	
	def __init__(self, filename): #{
		self.config = NonvalidatingReader.parseUri('file:///' + filename);
		self.log_file = '';
		self.pairs = {};
		self.working_directory = None;
		self.templates_directory = None;
	#}

	def load_templates(self, _file): #{
		print 'templates: ' + 'file:///' + self.working_directory + '/' + self.templates_directory + '/' + _file;
		tmpldoc = NonvalidatingReader.parseUri('file:///' + self.working_directory + '/' + self.templates_directory + '/' + _file);

		matrix = {};

		for left in Ft.Xml.XPath.Evaluate('.//left', contextNode=tmpldoc): #{
			left_hash = left.getAttributeNS(None, 'id');
			if left_hash not in matrix: #{
				matrix[left_hash] = {};
			#}
			for right in Ft.Xml.XPath.Evaluate('.//right', contextNode=left): #{
				right_hash = right.getAttributeNS(None, 'id');
				if right_hash not in matrix[left_hash]: #{
					matrix[left_hash][right_hash] = '';
				#}
				for template in  Ft.Xml.XPath.Evaluate('.//template', contextNode=right): #{
				        buf = cStringIO.StringIO();
				        Ft.Xml.Domlette.Print(template, stream=buf, encoding='utf-8');
				        text = buf.getvalue();
				        buf.close();

					matrix[left_hash][right_hash] = text.replace('<template>', '').replace('</template>', '');

					print 'Added template (' + left_hash + ':' + right_hash + '); length: ' + str(len(text));
				#}
			#}
		#}

		return matrix;
	#}

	def parse_config(self): #{
		self.working_directory = self.config.xpath('/webforms/directories/wd')[0].firstChild.nodeValue;
		self.cache_directory = self.config.xpath('/webforms/directories/cache')[0].firstChild.nodeValue;
		self.templates_directory = self.config.xpath('/webforms/directories/templates')[0].firstChild.nodeValue;
		self.log_file = self.config.xpath('/webforms/log-file')[0].firstChild.nodeValue;

		path = '/webforms/pairs/pair';

		for node in Ft.Xml.XPath.Evaluate(path, contextNode=self.config): #{
			pair_name = node.getAttributeNS(None, 'n');
			enabled = node.getAttributeNS(None, 'enabled');

			if enabled == 'no': #{
				continue;
			#}

			self.pairs[pair_name] = Pair(self.working_directory, pair_name, node);
			self.pairs[pair_name].cache = self.cache_directory + pair_name + '/';
				
			templates = {};
			for tmpls in Ft.Xml.XPath.Evaluate(".//templates",contextNode=node): #{
				templates = self.load_templates(tmpls.getAttributeNS(None, 'file'));
			#}

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

							self.pairs[pair_name].dictionary[current_side].add_gloss(tag_name, name, comment);
						#}
					#}

					self.pairs[pair_name].dictionary[current_side].set_display(tag_name, paradigm_display_mode);
				#}

				self.pairs[pair_name].add_tag(tag_name, shows);
			#}
			self.pairs[pair_name].set_templates(templates);
		#}


#
#	Speedup idea: Load all glossed paradigm entries at the start.
#

#		for pair in self.pairs: #{
#			for side in self.pairs[pair].dictionary.keys(): #{
#				for tag in self.pairs[pair].dictionary[side].paradigms.keys(): #{
#					for paradigm in self.pairs[pair].dictionary[side].paradigms[tag].keys(): #{
#						print self.pairs[pair].dictionary[side].paradigms[tag].get(paradigm).name;
#						self.pairs[pair].dictionary[side].get_paradigm(tag.encode('utf-8'), paradigm.encode('utf-8'));
#					#}
#				#}
#			#}
#		#}


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
