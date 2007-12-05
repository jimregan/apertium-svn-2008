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
		key = current_category + '.' + m.hexdigest();
		if key not in paradigms: #{
			paradigms[key] = [];
		#}


		print >> sys.stderr, current_category + '.' + m.hexdigest() , current_paradigm;
		paradigms[key].append(current_paradigm); 
	#}

	return paradigms;
#}

def generate_entry_list(context, paradigms): #{
	path = '/dictionary/section[@id="main"]/e';
	entries = {};
	for entry in Ft.Xml.XPath.Evaluate(path, contextNode=context): #{
		lema = entry.getAttributeNS(None, 'lm');
		pars = Ft.Xml.XPath.Evaluate('.//par', contextNode=entry);
		if len(pars) >= 1: #{
			par = pars[0].getAttributeNS(None, 'n');
			for hash in paradigms: #{
				if par in paradigms[hash]: #{
					if lema not in entries: #{
						entries[lema] = {};
					#}
					category = hash.split('.')[0];
					entries[lema][category] = hash;

					print >> sys.stderr, 'generate_entry_list:', lema + '.' + category, ';', par, ';',  hash;
				#}
			#}
		#}
	#}

	return entries;
#}

def retrieve_lemma(entry, side): #{

	full_lemma = '';
	for kid in side[0].childNodes: #{
		if type(kid) == Ft.Xml.Domlette.Text: #{
			if full_lemma == '': #{
				full_lemma = kid.nodeValue;
			else: #{
				full_lemma = full_lemma + ' ' + kid.nodeValue;
			#}
		#}
	#}

	return full_lemma;
#}

def retrieve_category(entry, side): #{
	for kid in Ft.Xml.XPath.Evaluate('.//s', contextNode=side[0]): #{
		return kid.getAttributeNS(None, 'n');
	#}
	return '';
#}

def generate_templates(context, left_entries, right_entries): #{

	path = '/dictionary/section[@id="main"]/e';
	template_matrix = {};

	for entry in Ft.Xml.XPath.Evaluate(path, contextNode=context): #{
		entry.removeAttributeNS(None, 'a');
		if len(Ft.Xml.XPath.Evaluate('.//i', contextNode=entry)) > 0: #{
			continue;
		#}
		left = Ft.Xml.XPath.Evaluate('.//l', contextNode=entry);
		right = Ft.Xml.XPath.Evaluate('.//r', contextNode=entry);

		left_lemma = retrieve_lemma(entry, left);
		right_lemma = retrieve_lemma(entry, right);

		left_symbol = retrieve_category(entry, left);
		right_symbol = retrieve_category(entry, right);

		if left_symbol == '' or right_symbol == '': #{
			print >> sys.stderr, 'No category found in the bidix for ' , left_lemma , ':' , right_lemma;
			continue;
		#}

		if left_symbol not in categories and right_symbol not in categories: #{
			continue;
		#}

		print >> sys.stderr, left_lemma , left_symbol , ':' , right_lemma, right_symbol;

		if left_lemma not in left_entries: #{
			#print >> sys.stderr, 'Key not found [l]: ' , left_lemma , '(' + str(len(left_entries)) + ')';
			continue;
		#}
		try:
			left_hash = left_entries[left_lemma][left_symbol];
		except:
			continue;

		if right_lemma not in right_entries: #{
			#print >> sys.stderr, 'Key not found [r]: ' , right_lemma , '(' + str(len(right_entries)) + ')';
			continue;
		#}
		try:
			right_hash = right_entries[right_lemma][right_symbol];
		except: 
			continue;

		if left_hash not in template_matrix: #{
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
		entrada = entrada.replace('<b/>', ' ').replace(left_lemma, 'lemma1').replace(right_lemma, 'lemma2');

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
#}



left_paradigms = generate_monodix_hash(left);
right_paradigms = generate_monodix_hash(right);

left_entries = generate_entry_list(left, left_paradigms);
right_entries = generate_entry_list(right, right_paradigms);

templates = generate_templates(bidix, left_entries, right_entries);

#eof
