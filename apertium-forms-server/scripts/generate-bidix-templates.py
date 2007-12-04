#!/usr/bin/python2.5 
# coding=utf-8
# -*- encoding: utf-8 -*-

import sys, string, codecs, xml, os, Ft, re, md5;
from Ft.Xml.Domlette import NonvalidatingReader;
from Ft.Xml.XPath import Evaluate;

sys.stdout = codecs.getwriter('utf-8')(sys.stdout);
sys.stderr = codecs.getwriter('utf-8')(sys.stderr);

categories = ['n', 'adj'];

if len(sys.argv) < 4: #{
	print 'Usage: python generate-bidix-templates.py <left monodix> <bidix> <right monodix>';
	sys.exit(-1);
#}

left_file  = os.getcwd() + '/' + sys.argv[1];
bidix_file = os.getcwd() + '/' + sys.argv[2];
right_file = os.getcwd() + '/' + sys.argv[3];

print left_file , bidix_file , right_file;

path = '/dictionary/pardefs/pardef';

left = NonvalidatingReader.parseUri('file://' + left_file);
right = NonvalidatingReader.parseUri('file://' + right_file);
	
def generate_monodix_hash(context): #{
	paradigms = {};
	for paradigm in Ft.Xml.XPath.Evaluate(path, contextNode=context): #{
		current_paradigm = paradigm.getAttributeNS(None, 'n');
		ignoring = 1;
		for tag in categories: #{
			needle = '.*__' + tag + '$';
			patron = re.compile(needle);
			if(patron.match(current_paradigm)): #{
				ignoring = 0;
			#}
		#}
	
		if ignoring == 1: #{
			continue;
		#}
	
		paradigm_hash = [];
		for entrada in Ft.Xml.XPath.Evaluate('.//e', contextNode=paradigm): #{
			restriction = entrada.getAttributeNS(None, 'r');
			if type(restriction) == None: #{
				restriction = '';
			#}
	
			symbols = '';
	
			for symbol in  Ft.Xml.XPath.Evaluate('.//s', contextNode=entrada): #{
				symbols = symbols + symbol.getAttributeNS(None, 'n') + '.';
			#}
	
	#		print symbols , restriction;
	
			paradigm_hash.append((restriction, symbols));
		#}
		m = md5.new();
		m.update(str(set(paradigm_hash)));
		if m.hexdigest() not in paradigms: #{
			paradigms[m.hexdigest()] = [];
		#}

		paradigms[m.hexdigest()].append(current_paradigm); 
	#}

	return paradigms;
#}

left_paradigms = generate_monodix_hash(left);
right_paradigms = generate_monodix_hash(right);

print 'Left paradigms: ';
for key in left_paradigms.keys(): #{
	print key, str(left_paradigms[key]);
#}

print 'Right paradigms: ';
for key in right_paradigms.keys(): #{
	print key, str(right_paradigms[key]);
#}
