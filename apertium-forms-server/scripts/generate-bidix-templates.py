#!/usr/bin/python2.5 
# coding=utf-8
# -*- encoding: utf-8 -*-

import sys, string, codecs, xml, os, Ft, re, md5, cStringIO;
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


left = NonvalidatingReader.parseUri('file://' + left_file);
bidix = NonvalidatingReader.parseUri('file://' + bidix_file);
right = NonvalidatingReader.parseUri('file://' + right_file);
	
def generate_monodix_hash(context): #{
	path = '/dictionary/pardefs/pardef';
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

def generate_entry_list(context, paradigms): #{
	path = '/dictionary/section[@id="main"]/e';
	entries = {};
	for entry in Ft.Xml.XPath.Evaluate(path, contextNode=context): #{
		lema = entry.getAttributeNS(None, 'lm');
		pars = Ft.Xml.XPath.Evaluate('.//par', contextNode=entry);
		if len(pars) >= 1 and lema.count(' ') < 1: #{
			par = pars[0].getAttributeNS(None, 'n');
			for hash in paradigms: #{
				if par in paradigms[hash]: #{
					entries[lema] = hash;
					#print lema, hash, par;	
				#}
			#}
		#}
	#}

	return entries;
#}

def generate_correspondences(context, left_entries, right_entries): #{
	path = '/dictionary/section[@id="main"]/e';
	matrix = {};
	current_left_lemma = '';
	entradas = '';
	for entry in Ft.Xml.XPath.Evaluate(path, contextNode=context): #{
		left = Ft.Xml.XPath.Evaluate('.//l', contextNode=entry);	
		if len(left) >= 1: #{
			try: #{
				left_lemma = left[0].firstChild.nodeValue;

				if left_lemma != current_left_lemma and left_lemma != '': #{
					current_left_lemma = left_lemma;
				elif left_lemma != current_left_lemma and left_lemma == '': #{
					current_left_lemma = left_lemma;
				else:
					current_left_lemma = '';
				#}

#				print current_left_lemma , left_lemma;

			except: #{
				continue;
			#}
		else: #{
			continue;
		#}

		right = Ft.Xml.XPath.Evaluate('.//r', contextNode=entry);	
		if len(right) >= 1: #{
			try: #{
				right_lemma = right[0].firstChild.nodeValue;
			except: #{
				continue;
			#}
		else: #{
			continue;
		#}

		left_symbol = Ft.Xml.XPath.Evaluate('.//s', contextNode=left[0]);
		if len(left_symbol) >= 1: #{
			left_symbol = left_symbol[0].getAttributeNS(None, 'n');
                	
			ignoring = 1;

	                for tag in categories: #{
	                        if tag == left_symbol: #{
	                                ignoring = 0;
	                        #}
	                #}

	                if ignoring == 1: #{
				continue;
                	#}
		#}

		if len(Ft.Xml.XPath.Evaluate('.//b', contextNode=entry)) > 0: #{
			continue;
		#}

		#print left_lemma, '\t', right_lemma;

		try: #{
			left_hash = left_entries[left_lemma];
			right_hash = right_entries[right_lemma];

			if left_entries[left_lemma] not in matrix: #{
				matrix[left_hash] = {};
			#}
	
			if right_hash not in matrix[left_hash]: #{
				matrix[left_hash][right_hash] = 0;
			#}
	
			matrix[left_hash][right_hash] =  matrix[left_hash][right_hash] + 1;

			buf = cStringIO.StringIO();
			Ft.Xml.Domlette.Print(entry, stream=buf, encoding='utf-8')
			new_entrada = buf.getvalue();
			buf.close();
	
			if left_lemma == current_left_lemma: #{
				print 'Appending for: ' , left_hash , ':' , right_hash;
				entradas = entradas + '\n' + new_entrada.replace(left_lemma, 'lemma1').replace(right_lemma, 'lemma2');
			else: #{
				print u'→', entradas;
				entradas = '';
				entradas = entradas + '\n' + new_entrada.replace(left_lemma, 'lemma1').replace(right_lemma, 'lemma2');
				print u'ø', entradas;
			#}
	
			#print matrix[left_hash][right_hash], '\t', left_lemma, '\t', left_entries[left_lemma], '\t', right_lemma, '\t', right_entries[right_lemma];

		except: #{
			continue;
		#}
	#}

	return matrix;
#}

left_paradigms = generate_monodix_hash(left);
right_paradigms = generate_monodix_hash(right);

left_entries = generate_entry_list(left, left_paradigms);
right_entries = generate_entry_list(right, right_paradigms);

matrix = generate_correspondences(bidix, left_entries, right_entries);

for left in matrix: #{
	for right in matrix[left]: #{
		print matrix[left][right], '\t', left, '\t', right;
	#}
#}
