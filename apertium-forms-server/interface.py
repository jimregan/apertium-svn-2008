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
                for pair in post_data['pairs'].keys():  
                        if pair == post_data['selected_pair']:
                                print '         <option value="' + pair + '" selected>' + pair + '</option>';
                        else: 
                                print '         <option value="' + pair + '">' + pair + '</option>';
                print '    </select>';

                print '</body>';
                print '</html>';
	#}
#}
