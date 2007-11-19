#!/usr/bin/python2.5
# coding=utf-8
# -*- encoding: utf-8 -*-

import sys, string, codecs, xml, re;

import Ft;
from Ft.Xml.Domlette import NonvalidatingReader;
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
		print >> sys.stderr, 'add_stem(' + _stem + ', ' + _symlist + ')';
		self.stems.append((_stem, _symlist));
	#}

	def get_stems(self): #{
		print >> sys.stderr, self.stems;
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
		for _paradigm in self.paradigms[_tag].values(): #{
			if _paradigm.name == _name: #{	
				paradigm = _paradigm;	
			#}
		#}
		
		if paradigm == None: #{
			return None;
		#}

		paradigm.stems = [];
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
		print needle;
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
		print '  % ' + self.side + ' add_gloss(' + _paradigm + ', ' + _gloss + ')';
		self.glosses[_paradigm] = _gloss;
	#}

        def generate_monodix_entrada(self, _lemma, _paradigm, _comment, _author): #{
                incondicional = self.incondicional(_lemma, _paradigm);
		print >> sys.stderr, 'lemma: ' + _lemma + ', paradigm: ' + _paradigm + ', comment: ' + _comment + ', author: ' + _author;

                if _comment != '': #{
                        entrada = '<e lm="' + _lemma + '" a="' + _author + '">' + "\n" + '  <i>' + incondicional + '</i>' + "\n" + '  <par n="' + _paradigm + '"/>' + "\n" + '</e>' +  '<!-- ' + _comment + ' -.' + "\n";
                else: #{
                        entrada = '<e lm="' + _lemma + '" a="' + _author + '">' + "\n" + '  <i>' + incondicional + '</i>' + "\n" + '  <par n="' + _paradigm + '"/>' + "\n" + '</e>';
                #}

		print >> sys.stderr, entrada;

                return entrada;
        #}

        def generate_bidix_entrada(self, _lemma1, _lemma2, _tag, _restriction, _comment, _author): #{
                # <e><p><l>lemma1<s n="tag"/></l><r>lemma2<s n="tag"/></r></p></e>

                if _restriction == "none" or _restriction == '': #{
                        entrada = '<e a="' + _author + '">' + "\n" + '  <p>' + "\n" + '    <l>' + _lemma1 + '<s n="' + _tag + '"/></l>' + "\n" + '    <r>' + _lemma2 + '<s n="' + _tag + '"/></r>' + "\n" + '  </p>' + "\n" + '</e>' + "\n";

                else: #{
                        entrada = '<e r="' + _restriction +  '" a="' + _author + '"><p><l>' + _lemma1 + '<s n="' + _tag + '"/></l><r>' + _lemma2 + '<s n="' + _tag + '"/></r></p></e>' + "\n";
                #}

                if _comment != "": #{
                        entrada = entrada + '<!-- ' + _comment + ' -->' + "\n"; 
                #}


                return entrada;
        #}



        def incondicional(self, _lemma, _paradigm): #{
                if _paradigm.count('/') < 1: #{
                        return _lemma;
                #}

                bar_pos = _paradigm.find('/');
                und_pos = _paradigm.find('_');
                chr_str =  (und_pos - bar_pos) - 1;

                return _lemma[0:(len(_lemma) - chr_str)];
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
