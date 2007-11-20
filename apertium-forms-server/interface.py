#!/usr/bin/python2.5
# coding=utf-8
# -*- encoding: utf-8 -*-

import sys, string, codecs, xml, re, Ft;
from Ft.Xml.XPath import Evaluate;

sys.stdout = codecs.getwriter('utf-8')(sys.stdout);
sys.stderr = codecs.getwriter('utf-8')(sys.stderr);

#
#	This class implements the HTML interface for the 
#	forms. If you want to change how they look, make
#	the change here. 
#



class Interface: #{
	
	def display(self, post_data): #{

		if post_data['committing'] == 'yes': #{
			left_entrada = self.show_entrada(post_data, 'left');
			bidix_entrada = self.show_entrada(post_data, 'bidix');
			right_entrada = self.show_entrada(post_data, 'right');

			print '<pre>';
			print left_entrada.replace('<', '&lt;').replace('>', '&gt;');
			print "\n";
			print bidix_entrada.replace('<', '&lt;').replace('>', '&gt;');
			print "\n";
			print right_entrada.replace('<', '&lt;').replace('>', '&gt;');
			print '</pre>';

			selected_pair = post_data['selected_pair'];
			pairs = post_data['pairs'];
			post_data['left_dictionary'].commit(left_entrada);
			post_data['bidix_dictionary'].commit(bidix_entrada);
			post_data['right_dictionary'].commit(right_entrada);

#			pairs[selected_pair].commit(left_entrada, bidix_entrada, right_entrada);

                	print '<a href="http://xixona.dlsi.ua.es:8080/">again!</a>';

			return;
		#}

                print '<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">';
                print '<head>';
                print '  <title>Apertium dictionary management</title>';
                print '  <meta http-equiv="Content-Type" content="text/html; charset=utf-8"> ';
                print '  <link rel="stylesheet" type="text/css" href="/static/styles/default.css"/>';
                print '  <script src="/static/js/boxover.js"></script> ';
                print '</head>';
                print '<body>';
                print '<form action="add" method="POST" name="dixform">';
                print '<a href="http://xixona.dlsi.ua.es:8080/">home</a>';
                print '<div id="language_bar">';
                print 'ca · en · es';
                print '</div>';
                print '<div>';
                print '  <div width="100%">                                                            <!-- Header -->';
                print '    Language pair: <select name="selected_pair" onChange="dixform.submit();">';
                for pair in post_data['pairs'].keys(): #{
                        if pair == post_data['selected_pair']: #{
                                print '         <option value="' + pair + '" selected>' + pair + '</option>';
                        else: #{
                                print '         <option value="' + pair + '">' + pair + '</option>';
			#}
		#}
                print '    </select>';
                print '    Part-of-speech: <select name="selected_tag" onChange="dixform.submit();">';
                for tag in post_data['tags'].keys(): #{
                        if tag == post_data['selected_tag']: #{
                                print '         <option value="' + tag + '" selected>' + tag + '</option>';
                        else: #{
                                print '         <option value="' + tag + '">' + tag + '</option>';
			#}
		#}
                print '    </select>';
                print '  </div>';
		print '</div>';
		print '<div>';
                print '  <hr />';
                print '  <div id="left"> <!-- Left -->';
		print '    <p>';
                print '      Lemma:<sup><span class="tooltip" title="header=[Lemma] body=[Type in the lemma, or citation form of the word you wish to add.]">?</span></sup>';
                print '      <input type="text" name="left_lemma" value="' + post_data['left_lemma'] + '">';
		print '    </p>';
		print '    <p>';
                print '      Paradigm:<sup><span class="tooltip" title="header=[Paradigm] body=[Paradigms define how a word inflects, select the one that fits the lemma you added.]">?</span></sup>';
                print '      <select name="left_paradigm">';
                for left_p in post_data['left_paradigms']: #{
                        if post_data['left_display_mode'] == 'list' and left_p not in post_data['left_glosses']: #{
                                continue;
			#}
                        if left_p == post_data['left_paradigm']: #{
                                if left_p in post_data['left_glosses']: #{
                                        print '                <option value="' + left_p + '" selected>' + post_data['left_glosses'][left_p] + '</option>';
                                else: #{
                                        print '                <option value="' + left_p + '" selected>' + left_p + '</option>';
                        else: #{
                                if left_p in post_data['left_glosses']: #{
                                        print '                <option value="' + left_p + '">' + post_data['left_glosses'][left_p] + '</option>';
                                else: #{
                                        print '                <option value="' + left_p + '">' + left_p + '</option>';
				#}
			#}
		#}
                print '      </select>';
		print '    </p>';

		self.show_preview(post_data, 'left');

		left_entrada = self.show_entrada(post_data, 'left');
		print '  <pre>';
		print left_entrada.replace('<', '&lt;').replace('>', '&gt;');
		print '  </pre>';

                print '  </div>';

                print '    <!-- Bidix side -->';
                print '  <div id="centre">';
                if post_data['restriction'] != 'LR' and post_data['restriction'] != 'RL': #{
                        print '    <input type="radio" name="restriction" value="none" checked>←→<br />';
                else: #{
                        print '    <input type="radio" name="restriction" value="none">←→<br />';
                #}
                print '';
                if post_data['restriction'] == 'LR': #{
                        print '    <input type="radio" name="restriction" value="LR" checked>→→<br />';
                else: #{
                        print '    <input type="radio" name="restriction" value="LR">→→<br />';
                #}
                print '';
                if post_data['restriction'] == 'RL': #{
                        print '    <input type="radio" name="restriction" value="RL" checked>←←<br />';
                else: #{
                        print '    <input type="radio" name="restriction" value="RL">←←<br />';
                #}
                print '';
                print '    <input type="submit" name="clear_box" value="Clear">';
                print '    <input type="submit" name="preview_box" value="Preview">';
                print '    <input type="submit" name="commit_box" value="Commit">';
                print '';

		print '<pre>';
		bidix_entrada = self.show_entrada(post_data, 'bidix');
		print bidix_entrada.replace('<', '&lt;').replace('>', '&gt;');
		print '</pre>';
                print '  </div>';


                print '  <div> <!-- Right -->';
		print '    <p>';
                print '      Lemma:<sup><span class="tooltip" title="header=[Lemma] body=[Type in the lemma, or citation form of the word you wish to add.]">?</span></sup>';
                print '      <input type="text" name="right_lemma" value="' + post_data['right_lemma'] + '">';
		print '    </p>';
		print '    <p>';
                print '      Paradigm:<sup><span class="tooltip" title="header=[Paradigm] body=[Paradigms define how a word inflects, select the one that fits the lemma you added.]">?</span></sup>';
                print '      <select name="right_paradigm">';
                for right_p in post_data['right_paradigms']: #{
                        if post_data['right_display_mode'] == 'list' and right_p not in post_data['right_glosses']: #{
                                continue;
                        #}
                        if right_p == post_data['right_paradigm']: #{
                                if right_p in post_data['right_glosses']: #{
                                        print '                <option value="' + right_p + '" selected>' + post_data['right_glosses'][right_p] + '</option>';
                                else: #{
                                        print '                <option value="' + right_p + '" selected>' + right_p + '</option>';
                        else: #{
                                if right_p in post_data['right_glosses']: #{
                                        print '                <option value="' + right_p + '">' + post_data['right_glosses'][right_p] + '</option>';
                                else: #{
                                        print '                <option value="' + right_p + '">' + right_p + '</option>';
                                #}
                        #}
                #}
                print '      </select>';
		print '    </p>';

		self.show_preview(post_data, 'right');

		right_entrada = self.show_entrada(post_data, 'right');
		print '  <pre>';
		print right_entrada.replace('<', '&lt;').replace('>', '&gt;');
		print '  </pre>';

                print '  </div>';
                print '</div>';

                print '</body>';
                print '</html>';
	#}

	def show_preview(self, post_data, _side): #
		print >> sys.stderr, 'right paradigm: ' , post_data['right_paradigm'];
		p = _side + '_paradigm';
		if post_data['previewing'] == 'on': #{
			paradigm = post_data[_side + '_dictionary'].get_paradigm(post_data[p], post_data['selected_tag']);
			if type(paradigm) == type(None): #{
				return;
			#}
	                if type(paradigm.get_stems()) != type(None): #{
	                        for s in post_data[_side + '_paradigms'][paradigm.name].get_stems(): #{
					shows = post_data[_side + '_dictionary'].get_tag_by_tag(post_data['selected_tag']).get_list();

					if len(shows) > 0: #{
						for show in post_data[_side + '_dictionary'].get_tag_by_tag(post_data['selected_tag']).get_list(): #{
							if show == s[1]: #{
				                                print post_data[_side + '_lemma'] + s[0] + '<br />';
								print '<span id="symbol_list">' + s[1] + '</span>';
								print '<p />';
							#}
						#}
					else: #{
		                                print self.incondicional(post_data[_side + '_lemma'], post_data[_side + '_paradigm']) + s[0] + '<br />';
						print '<span id="symbol_list">' + s[1] + '</span>';
						print '<p />';
					#}
				#}
	                else: #{
	                        print '         No stems';
	                #}
		#}
	#}

	def show_entrada(self, post_data, _side): #{
		if post_data['previewing'] == 'on': #{
			if _side == 'bidix': #{
				dictionary = post_data[_side + '_dictionary'];
	
				if type(dictionary) != None: #{
					_lemma1 = post_data['left_lemma'];
					_lemma2 = post_data['right_lemma'];
					_tag = post_data['selected_tag'];
					_comment = '';
					_restriction = post_data['restriction'];
					_author = 'webform';
		
		        		entrada = dictionary.generate_bidix_entrada(_lemma1, _lemma2, _tag, _restriction, _comment, _author);
	
					return entrada;
				#}
			#}

			if _side == 'right' or _side == 'left' and post_data[_side + '_paradigm']: #{
				dictionary = post_data[_side + '_dictionary'];
				paradigm = post_data[_side + '_dictionary'].get_paradigm(post_data[_side + '_paradigm'], post_data['selected_tag']);
	
				if type(paradigm) != type(None): #{
					_lemma = post_data[_side + '_lemma'];
					_paradigm = paradigm.name;
					_comment = '';
					_author = 'webform';
		
					entrada = dictionary.generate_monodix_entrada(_lemma, _paradigm, _comment, _author);
	
					return entrada;
				#}
			#}
		#}
		return '';
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
		r = l[0:(len(_lemma) - chr_str)];

		return r.encode('utf-8');
	#}
#}
