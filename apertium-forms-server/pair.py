#!/usr/bin/python2.5
# coding=utf-8
# -*- encoding: utf-8 -*-

import sys, string, codecs, xml, re, Ft;
from Ft.Xml.Domlette import NonvalidatingReader;
from Ft.Xml.Domlette import Print, PrettyPrint;
from Ft.Xml.XPath import Evaluate;

sys.stdout = codecs.getwriter('utf-8')(sys.stdout);
sys.stderr = codecs.getwriter('utf-8')(sys.stderr);

class Tag: #{
	name = None;

	def __init__(self, _name, _list): #{
		self.name = _name;
		self.list = _list;

		for s in self.list: #{
			print '% ' + s;
		#}
	#}

	def get_list(self): #{
		return self.list;
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

	def add_stem(self, _stem, _symlist): #{
		#print >> sys.stderr, 'add_stem(' + _stem + ', ' + _symlist + ')';
		self.stems.append((_stem, _symlist));
	#}

	def get_stems(self): #{
		#print >> sys.stderr, self.stems;
		return self.stems;
	#}
#}

class Dictionary: #{
	display = None;
	language = None;
	file = None;
	side = None;

	def __init__(self, _side, _language, _file, _doc, _tags): #{
		self.display = {};
		self.language = _language;
		self.file = _file;
		self.doc = _doc;
		self.side = _side;
		self.paradigms = {};
		self.glosses = {};
		self.tags = _tags;
	#}

	def get_tags(self): #{
		return self.tags;
	#}

	def get_tag_by_tag(self, _tag): #{
		return self.tags[_tag];
	#}

	def get_paradigms(self): #{
		return self.paradigms;
	#}

	def get_glosses(self): #{
		return self.glosses;
	#}

	def get_paradigms_by_tag(self, _tag): #{
		return self.paradigms[_tag];
	#}

	def get_paradigm(self, _name, _tag): #{
		paradigm = None;

		paradigm = self.paradigms[_tag].get(_name);
		
		if paradigm == None: #{
			print >> sys.stderr, 'We didn`t find the paradigm in the hash';
			for _paradigm in self.paradigms[_tag].values(): #{
				if _paradigm.name == _name: #{	
					paradigm = _paradigm;	
				#}
			#}

			if paradigm == None: #{
				return None;
			#}
		#}

		# paradigm stems already loaded
		if len(paradigm.stems) > 0: #{
			return paradigm;
		#}

		print >> sys.stderr, 'get_paradigm ' , paradigm.name , _name;
		path = ".//pardef[@n='" + _name + "']";
		res = self.doc.xpath(path)[0];
		
		for entrada in Ft.Xml.XPath.Evaluate('.//e', contextNode=res): #{
			symlist = '';

			pair = Ft.Xml.XPath.Evaluate('.//p', contextNode=entrada)[0]; 
			left = Ft.Xml.XPath.Evaluate('.//l', contextNode=pair)[0].firstChild; 

			if type(left) != type(None): #{
				left = Ft.Xml.XPath.Evaluate('.//l', contextNode=pair)[0].firstChild.nodeValue;
			#}

			if type(left) == type(None): #{
				left = '';
			#}

			right = Ft.Xml.XPath.Evaluate('.//r', contextNode=pair)[0]; 

			for symbol in Ft.Xml.XPath.Evaluate('.//s', contextNode=right): #{
				if symlist != '': #{
					symlist = symlist + '.' + symbol.getAttributeNS(None, 'n');
				#}
				if symlist == '': #{
					symlist = symlist + symbol.getAttributeNS(None, 'n');
				#}
			#}

			print >> sys.stderr, 'get_paradigm ' , left , symlist;
			paradigm.add_stem(left, symlist);
		#}

		return paradigm;
	#}

	def set_paradigms_by_tag(self, _tag): #{
		print self.side + ' set_paradigms_by_tag(' + _tag + ')';
		paradigms = self.doc.xpath('//pardef');
		self.paradigms[_tag] = {};
		needle = '.*__' + _tag + '$';
		patron = re.compile(needle);
		for paradigm in paradigms: #{
			n = paradigm.getAttributeNS(None, 'n');
			if(patron.match(n)): #{
				p = Paradigm(n);
				self.paradigms[_tag][n] = p;
			#}
		#}

		print self.side + '  set ' , len(self.paradigms[_tag]) , 'paradigms';
	#}

	def set_display(self, _tag, _mode): #{
		if _mode == None or _mode == '': #{
			self.display[_tag] = 'all';
		#}

		if _mode != None and _mode != '': #{
			self.display[_tag] = _mode;
		#}
	#}

	def get_display_by_tag(self, _tag): #{
		if _tag in self.display: #{
			return self.display[_tag];
		#}

		return 'all';
	#}

	def get_displays(self): #{
		return self.display;
	#}


	def add_gloss(self, _paradigm, _gloss): #{
		self.glosses[_paradigm] = _gloss;
	#}

        def generate_monodix_entrada(self, _lemma, _paradigm, _restriction, _comment, _author): #{
                incondicional = self.incondicional(_lemma, _paradigm);

		print >> sys.stderr, 'lemma: ' + _lemma + ', paradigm: ' + _paradigm + ', comment: ' + _comment + ', author: ' + _author;

		entrada = '';
		if _restriction == "none" or _restriction == '': #{
			entrada = entrada + '<e lm="' + _lemma + '" a="' + _author + '">' + "\n";
		else: #{
			entrada = entrada + '<e r="' + _restriction + '" lm="' + _lemma + '" a="' + _author + '">' + "\n";
		#}

		entrada = entrada + '  <i>' + incondicional + '</i>' + "\n";
		entrada = entrada + '  <par n="' + _paradigm + '"/>' + "\n";
		entrada = entrada + '</e>';

                if _comment != '': #{
			entrada = entrada +  '<!-- ' + _comment + ' -->' + "\n";
                #}

		print >> sys.stderr, entrada;

                return entrada;
        #}

        def generate_bidix_entrada(self, _lemma1, _lemma2, _paradigm1, _paradigm2, _tag, _restriction, _comment, _author): #{
		if _lemma1 == '' or _lemma2 == '' or _paradigm1 == None or _paradigm2 == None: #
			return '';
		#}

		_symbol_list_left = '';
		_symbol_list_right = '';

		if _tag == 'n': #{

			stems_left = _paradigm1.get_stems();
			stems_right = _paradigm2.get_stems();
	
			if len(stems_left) == len(stems_right): #{
				tags_left = set();
				for stem in stems_left: #{
					tags_left = tags_left | set(stem[1].split('.'));
				#}
	
				tags_right = set();
				for stem in stems_right: #{
					tags_right = tags_right | set(stem[1].split('.'));
				#}
	
				print >> sys.stderr , 'tags_left:' , tags_left;
				print >> sys.stderr , 'tags_right:' , tags_right;
				print >> sys.stderr , 'symdiff:' , tags_left ^ tags_right;
			#}

			_symbol_list_left = '<s n="n"/>';
			_symbol_list_right = '<s n="n"/>';
		#}

		if _tag == 'vblex': #{
			_symbol_list_left = '<s n="vblex"/>';
			_symbol_list_right = '<s n="vblex"/>';
		#}

		if _tag == 'adj': #{
			_symbol_list_left = '<s n="adj"/>';
			_symbol_list_right = '<s n="adj"/>';
		#}

		if _tag == 'adv': #{
			_symbol_list_left = '<s n="adv"/>';
			_symbol_list_right = '<s n="adv"/>';
		#}

		entrada = '';

                if _restriction == "none" or _restriction == '': #{
                        entrada = entrada + '<e a="' + _author + '">' + "\n";

                else: #{
                        entrada = entrada + '<e r="' + _restriction + '" a="' + _author + '">' + "\n";
		#}

		entrada = entrada + '  <p>' + "\n"; 
		entrada = entrada + '    <l>' + _lemma1 + _symbol_list_left + '</l>' + "\n";
		entrada = entrada + '    <r>' + _lemma2 + _symbol_list_right + '</r>' + "\n";
		entrada = entrada + '  </p>' + "\n";
		entrada = entrada + '</e>' + "\n";

                if _comment != "": #{
                        entrada = entrada + '<!-- ' + _comment + ' -->' + "\n"; 
                #}

		print >> sys.stderr, entrada;

                return entrada;
        #}

        def incondicional(self, _lemma, _paradigm): #{
                if _paradigm.count('/') < 1: #{
                        return _lemma;
                #}

                paradigm = _paradigm.decode('utf-8');
                bar_pos = paradigm.find('/');
                und_pos = paradigm.find('_');
                chr_str = (und_pos - bar_pos) - 1;
                l = _lemma.decode('utf-8');
                r = l[0:(len(l) - chr_str)];

                return r.encode('utf-8');
        #}

	def commit(self, _entrada): #{
		print >> sys.stderr, '> ' , self.file;
		print >> sys.stderr, self.side + ' commit(';
		print >> sys.stderr, _entrada;
		print >> sys.stderr, ')';

		for section in self.doc.xpath('.//section'): #{
			print >> sys.stderr , '+ section : ' + section.getAttributeNS(None, 'id');
			if section.getAttributeNS(None, 'id') == 'main': #{
				print >> sys.stderr , 'Writing to file....';
				insertion_point = section;
				child_doc = NonvalidatingReader.parseString(_entrada.encode('utf-8'), 'urn:bogus:dummy');
				child_node = child_doc.xpath('.//e')[0];
				insertion_point.appendChild(child_node);

				f = open(self.file, 'w');
				Print(self.doc, stream=f);
				f.close();
				print >> sys.stderr, 'Written.';
			#}
		#}

		print >> sys.stderr, 'Failed :(';
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

	def commit(self, _left, _bidix, _right): #{
		print >> sys.stderr , 'commit()';
		self.dictionary['left'].commit(_left);
		self.dictionary['bidix'].commit(_bidix);
		self.dictionary['right'].commit(_right);
	#}
#}
