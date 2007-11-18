#!/usr/bin/python2.5
# coding=utf-8
# -*- encoding: utf-8 -*-

import sys, string, codecs, xml, re;

import Ft;
from Ft.Xml.Domlette import NonvalidatingReader;
from Ft.Xml.XPath import Evaluate;


class Interface: #{
	
	def display(self, post_data): #{
                print '<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">';
                print '<head>';
                print '  <title>Apertium dictionary management</title>';
                print '  <meta http-equiv="Content-Type" content="text/html; charset=utf-8"> ';
                print '  <link rel="stylesheet" type="text/css" href="styles/default.css"/>';
                print '  <script src="js/boxover.js"></script> ';
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
                        else: #}
                                print '         <option value="' + tag + '">' + tag + '</option>';
			#}
		#}
                print '    </select>';
                print '  </div>';
                print '  <hr />';
                print '  <div> <!-- Left -->';
                print '      Lemma:<sup><span class="tooltip" title="header=[Lemma] body=[Type in the lemma, or citation form of the word you wish to add.]">?</span></sup>';
                print '      <input type="text" name="left_lemma" value="' + post_data['left_lemma'] + '">';
                print '      Paradigm:<sup><span class="tooltip" title="header=[Paradigm] body=[Paradigms define how a word inflects, select the one that fits the lemma you added.]">?</span></sup>';
                print '      <select name="left_paradigm">';
                for left_p in post_data['left_paradigms']: #{
                        if post_data['left_display_mode'] == 'list' and left_p not in post_data['left_glosses']: #{
                                continue;
			#}
                        if left_p == post_data['left_paradigm']: #{
                                if left_p in post_data['left_glosses']: #{
                                        print '                <option value="' + left_p + '" selected>' + post_data['left_glosses'][left_p] + '</option>';
                                else: #}
                                        print '                <option value="' + left_p + '" selected>' + left_p + '</option>';
                        else: #}
                                if left_p in post_data['left_glosses']: #{
                                        print '                <option value="' + left_p + '">' + post_data['left_glosses'][left_p] + '</option>';
                                else: #}
                                        print '                <option value="' + left_p + '">' + left_p + '</option>';
				#}
			#}
		#}
                print '      </select>';

                if type(post_data['left_paradigms'][left_p].get_stems()) != type(None): #{
                        for left_s in post_data['left_paradigms'][left_p].get_stems(): #{
                                print '             ' + left_s;
                else: #{
                        print '         No stems'; 
		#}

                print '  </div>';


                print '  <div> <!-- Right -->';
                print '      Lemma:<sup><span class="tooltip" title="header=[Lemma] body=[Type in the lemma, or citation form of the word you wish to add.]">?</span></sup>';
                print '      <input type="text" name="right_lemma" value="' + post_data['right_lemma'] + '">';
                print '      Paradigm:<sup><span class="tooltip" title="header=[Paradigm] body=[Paradigms define how a word inflects, select the one that fits the lemma you added.]">?</span></sup>';
                print '      <select name="right_paradigm">';
                for right_p in post_data['right_paradigms']: #{
                        if post_data['right_display_mode'] == 'list' and right_p not in post_data['right_glosses']: #{
                                continue;
                        #}
                        if right_p == post_data['right_paradigm']: #{
                                if right_p in post_data['right_glosses']: #{
                                        print '                <option value="' + right_p + '" selected>' + post_data['right_glosses'][right_p] + '</option>';
                                else: #}
                                        print '                <option value="' + right_p + '" selected>' + right_p + '</option>';
                        else: #}
                                if right_p in post_data['right_glosses']: #{
                                        print '                <option value="' + right_p + '">' + post_data['right_glosses'][right_p] + '</option>';
                                else: #}
                                        print '                <option value="' + right_p + '">' + right_p + '</option>';
                                #}
                        #}
                #}
                print '      </select>';

                if type(post_data['right_paradigms'][right_p].get_stems()) != type(None): #{
                        for right_s in post_data['right_paradigms'][right_p].get_stems(): #{
                                print '             ' + right_s;
                else: #{
                        print '         No stems';
                #}

                print '  </div>';


                print '</body>';
                print '</html>';
	#}
#}
