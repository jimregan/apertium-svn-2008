#!/usr/bin/python2.5
# coding=utf-8
# -*- encoding: utf-8 -*-

import sys, string, codecs, xml, re;

import Ft;
from Ft.Xml.Domlette import NonvalidatingReader;
from Ft.Xml.XPath import Evaluate;

class Tag: #{
	name = None;

	def __init__(self, _name, _list): #{
		self.name = _name;
		self.list = _list;

		for s in self.list: #{
			print '% ' + s;
		#}
	#}
#}

class Paradigm: #{
	name = None;

	def __init__(self, _name): #{
		self.name = _name;
		self.stems = [];
	#}

	def add_gloss(self, _gloss): #{
		self.gloss = _gloss;
	#}

	def add_stem(self, _stem): #{
		self.stems.append(_stem);
	#}
#}

class Dictionary: #{
	display = None;
	language = None;
	file = None;
	side = None;

	def __init__(self, _side, _language, _file, _doc, _tags): #{
		self.display = 'all';
		self.language = _language;
		self.file = _file;
		self.doc = _doc;
		self.side = _side;
		self.paradigms = {};
		self.glosses = {};
	#}

	def get_paradigms(self): #{
		return self.paradigms;
	#}

	def get_paradigms_by_tag(self, _tag): #{
		return self.paradigms[_tag];
	#}

	def set_paradigms_by_tag(self, _tag): #{
		print self.side + ' set_paradigms_by_tag(' + _tag + ')';
		paradigms = self.doc.xpath('//pardef');
		self.paradigms[_tag] = [];
		needle = '.*__' + _tag + '$';
		print needle;
		patron = re.compile(needle);
		for paradigm in paradigms: #{
			n = paradigm.getAttributeNS(None, 'n');
			if(patron.match(n)): #{
				p = Paradigm(n);
				self.paradigms[_tag].append(p);
			#}
		#}

		print self.side + '  set ' , len(self.paradigms[_tag]) , 'paradigms';
	#}

	def set_display(self, _mode): #{
		self.display = _mode;
	#}

	def add_gloss(self, _paradigm, _gloss): #{
		print '  % ' + self.side + ' add_gloss(' + _paradigm + ', ' + _gloss + ')';
		self.glosses[_paradigm] = _gloss;
	#}
#}

class Pair: #{
	name = None;	

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
		for dictionary in self.parent.xpath('.//dictionary'): #{
			current_dict = dictionary.getAttributeNS(None, 'n');
			side = dictionary.getAttributeNS(None, 'side');
			filename = dictionary.getAttributeNS(None, 'file');
			filename = self.working + '/cache/' + self.name + '/' + filename;

			print ' % (' + current_dict + ') ' + side + ', ' + filename;
			doc = NonvalidatingReader.parseUri('file:///' + filename);
			self.dictionary[side] = Dictionary(side, current_dict, filename, doc, self.tags);
		#}

	#}

	def dictionary(self, _side): #{
		return self.dictionaries[_side];
	#}

	def add_tag(self, _name, _list): #{
		print 'add_tag(' + _name + ')';
		self.tags[_name] = Tag(_name, _list);
		self.dictionary['left'].set_paradigms_by_tag(_name);
		self.dictionary['right'].set_paradigms_by_tag(_name);
	#}

	def get_tags(self): #{
		return self.tags;
	#}
#}
