#!/usr/bin/python2.5
# coding=utf-8
# -*- encoding: utf-8 -*-

import sys, string, codecs, xml;
from xml.dom import minidom;
from xml import xpath;

class Tag: #{
	name = '';

	def __init__(self, _name, _list): #{
		self.name = _name;
		self.list = _list;

		for s in self.list: #{
			print '% ' + s;
		#}
	#}
#}

class Paradigm: #{
	name = '';
	gloss = '';

	def __init__(self, _name, _gloss): #{
		self.name = _name;
		self.gloss = _gloss;
	#}
#}

class Dictionary: #{
	display = '';
	language = '';
	file = '';
	side = '';

	def __init__(self, _side, _language, _file, _doc): #{
		self.display = 'all';
		self.language = _language;
		self.file = _file;
		self.doc = _doc;
		self.side = _side;
	#}

	def set_display(self, _mode): #{
		self.display = _mode;
	#}

	def add_gloss(self, _paradigm, _gloss): #{
		print '  % ' + self.side + ' add_gloss(' + _paradigm + ', ' + _gloss + ')'
	#}
#}

class Pair: #{
	name = '';	

	def __init__(self, _working, _name, _parent): #{
		self.working = _working;
		self.name = _name;
		self.parent = _parent;
		self.dictionary = {};
		self.cache = {};
		self.tags = {};

		self.populate();
	#}

	def populate(self): #{
		print self.name + ' populate()';
		for dictionary in self.parent.getElementsByTagName('dictionary'): #{
			current_dict = dictionary.getAttribute('n');
			side = dictionary.getAttribute('side');
			filename = dictionary.getAttribute('file');
			filename = self.working + '/cache/' + self.name + '/' + filename;

			print ' % (' + current_dict + ') ' + side + ', ' + filename;
			doc = minidom.parse(filename).documentElement;
			self.dictionary[side] = Dictionary(side, current_dict, filename, doc);
		#}

	#}

	def dictionary(self, _side): #{
		return self.dictionaries[_side];
	#}

	def add_tag(self, _name, _list): #{
		print 'add_tag(' + _name + ')';
		self.tags[_name] = Tag(_name, _list);
	#}

	def get_tags(self): #{
		return self.tags;
	#}
#}
