#!/usr/bin/python2.5 
# coding=utf-8
# -*- encoding: utf-8 -*-

import sys, string, codecs, xml, os, Ft, re, md5, cStringIO;
from Ft.Xml.Domlette import NonvalidatingReader;
from Ft.Xml.XPath import Evaluate;

sys.stdout = codecs.getwriter('utf-8')(sys.stdout);
sys.stderr = codecs.getwriter('utf-8')(sys.stderr);

categories = ['n', 'adj', 'np'];

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
		current_category = '';
		ignoring = 1;
		for tag in categories: #{
			needle = '.*__' + tag + '$';
			patron = re.compile(needle);
			if(patron.match(current_paradigm)): #{
				current_category = tag;
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
	
			paradigm_hash.append((restriction, symbols));
		#}
		m = md5.new();
		m.update(str(set(paradigm_hash)));
		if m.hexdigest() not in paradigms: #{
			paradigms[current_category + '.' + m.hexdigest()] = [];
		#}

		print >> sys.stderr, current_category + '.' + m.hexdigest() , current_paradigm;
		paradigms[current_category + '.' + m.hexdigest()].append(current_paradigm); 
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
					category = hash.split('.')[0];
					entries[lema + '.' + category] = hash;

					print >> sys.stderr, 'generate_entry_list:', lema + '.' + category, ';', par, ';',  hash;
				#}
			#}
		#}
	#}

	return entries;
#}

def generate_correspondences(context, left_entries, right_entries): #{
	path = '/dictionary/section[@id="main"]/e';
	matrix = {};
	for entry in Ft.Xml.XPath.Evaluate(path, contextNode=context): #{
		left = Ft.Xml.XPath.Evaluate('.//l', contextNode=entry);	
		if len(left) >= 1: #{
			try: #{
				left_lemma = left[0].firstChild.nodeValue;
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

			#print matrix[left_hash][right_hash], '\t', left_lemma, '\t', left_entries[left_lemma], '\t', right_lemma, '\t', right_entries[right_lemma];

		except: #{
			continue;
		#}
	#}

	return matrix;
#}

def generate_templates(context, matrix, left_entries, right_entries): #{

	print '\n\n\n';

	path = '/dictionary/section[@id="main"]/e';
	template_matrix = {};

	for entry in Ft.Xml.XPath.Evaluate(path, contextNode=context): #{
		left = Ft.Xml.XPath.Evaluate('.//l', contextNode=entry);	
		if len(left) >= 1: #{
			try: #{
				left_lemma = left[0].firstChild.nodeValue;
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

		if len(Ft.Xml.XPath.Evaluate('.//b', contextNode=entry)) > 0: #{
			continue;
		#}

		if left_lemma not in left_entries: #{
			#print 'Key not found [l]: ' , left_lemma , '(' + str(len(left_entries)) + ')';
			continue;
		#}
		left_hash = left_entries[left_lemma];


		if right_lemma not in right_entries: #{
			#print 'Key not found [r]: ' , right_lemma , '(' + str(len(right_entries)) + ')';
			continue;
		#}
		right_hash = right_entries[right_lemma];

		if left_entries[left_lemma] not in template_matrix: #{
			template_matrix[left_hash] = {};
		#}

		if right_hash not in template_matrix[left_hash]: #{
			template_matrix[left_hash][right_hash] = {};
		#}

		bidix_hash = left_lemma + '.' + left_hash + ':' + right_lemma + '.' + right_hash;
		#bidix_hash = left_hash + ':' + right_hash;

		buf = cStringIO.StringIO();
		Ft.Xml.Domlette.Print(entry, stream=buf, encoding='utf-8');
		entrada = buf.getvalue();
		buf.close();
		entrada = entrada.replace(left_lemma, 'lemma1').replace(right_lemma, 'lemma2');

		if bidix_hash not in template_matrix[left_hash][right_hash]: #{
			template_matrix[left_hash][right_hash][bidix_hash] = '';
			template_matrix[left_hash][right_hash][bidix_hash] = template_matrix[left_hash][right_hash][bidix_hash] + '\n' + entrada;
		else: #{
			template_matrix[left_hash][right_hash][bidix_hash] = template_matrix[left_hash][right_hash][bidix_hash] + '\n' +  entrada;
		#}

		#print template_matrix[left_hash][right_hash][bidix_hash];
	#}


	for left in template_matrix: #{
		for right in template_matrix[left]: #{
			for bidix in template_matrix[left][right]: #{
				print '*** ' , bidix , ' ***';
				print template_matrix[left][right][bidix];
			#}
		#}
	#}

#
#	if left_lemma == current_left_lemma: #{
#		print 'Appending for: ' , left_hash , ':' , right_hash;
#		entradas = entradas + '\n' + new_entrada.replace(left_lemma, 'lemma1').replace(right_lemma, 'lemma2');
#	else: #{
#		print u'→', entradas;
#		entradas = '';
#		entradas = entradas + '\n' + new_entrada.replace(left_lemma, 'lemma1').replace(right_lemma, 'lemma2');
#		print u'ø', entradas;
#	#}
#	
#}



left_paradigms = generate_monodix_hash(left);
right_paradigms = generate_monodix_hash(right);

left_entries = generate_entry_list(left, left_paradigms);
right_entries = generate_entry_list(right, right_paradigms);

matrix = generate_correspondences(bidix, left_entries, right_entries);

templates = generate_templates(bidix, matrix, left_entries, right_entries);

#eof
