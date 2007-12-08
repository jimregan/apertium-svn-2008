#!/usr/bin/python2.5 -Wall
# coding=utf-8
# -*- encoding: utf-8 -*-

import os
import re
import cStringIO
from itertools import izip
from subprocess import Popen, PIPE

from Ft.Xml.Domlette import NonvalidatingReader;
from Ft.Xml.Domlette import Print, PrettyPrint;
from Ft.Xml.XPath import Evaluate;


template = {
    '<vblex>' : '<par n="breek__vblex"/>',
    '<adj>' : '<par n="dadelik__adj"/>',
    '<n>' : '<par n="appèl__n"/>',
    '<n><unc>' : '<par n="poësie__n__unc"/>',
    '<np>' : '<par n="Engeland__np"/>',
    '<adv>' : '<par n="miskien__adv"/>',
    '<preadv>' : '<par n="taamlik__preadv"/>'
}


class Globals:
    lt_proc_path = '/usr/local/bin/lt-proc'
    morpo_path = '/usr/local/share/apertium/apertium-en-af/af-en.automorf.bin'

def load_xml_file(filename):
    return NonvalidatingReader.parseUri(filename)

def usage():
    print "usage <monodix path> <bidix path> [<side>]"

def extract_entries(doc, side='l'):
    return doc.xpath("/dictionary/section[@id='main']/e/p/" + side)

def make_input_list(lst):
    return [nodes_to_string(node.xpath('./text()')) for node in lst]

pattern = re.compile(r'<s n="([a-zA-Z0-9]*)"/>')
def transform_tags(tag):
    return pattern.sub(r'<\1>', tag)

def make_compare_list(lst):
    return [transform_tags(nodes_to_string(node.childNodes)) for node in lst]

def process_output(output):
    return [line.strip('^$').split('/') for line in output.split('\n')]

def extract_tags(s):
    i = 0
    while True:
        if s[i] == '<':
            if s[i+1:i+3] == 'g>':
                i = i + 2

            elif s[i+1:i+4] in ('/g>', 'b/>'):
                i = i + 3

            else:
                return s[0:i], s[i:]

        i += 1

def run_morpho(input_list):
    proc = Popen([Globals.lt_proc_path, Globals.morpo_path], stdin=PIPE, stdout=PIPE)
    stdout, stderr = proc.communicate("\n".join(input_list))
    return process_output(stdout)

def find_missing_entries(lst):
    i = make_input_list(lst)
    c = make_compare_list(lst)
    o = run_morpho(i)

    missing = []

    for entry, morpho_entries in izip(c, o):
        matches = [e for e in morpho_entries if e.startswith(entry)]

        if len(matches) == 0:
            missing.append(entry)

    return missing

def write_entries_to_file(f, lst):
    for lemma, tags in (extract_tags(e) for e in lst):
        if tags in template:
            f.write(u'    <e lm="%(lemma)s"><i>%(lemma)s</i>%(template)s</e>\n' % { 'lemma': lemma,
                                                                           'template': template[tags] })

def test_load():
    Globals.automorf = '/home/wynand/apertium-git/apertium-en-af/af-en.automorf.bin'

def test_load_file():
    return load_xml_file('file:///home/wynand/apertium-git/apertium-en-af/apertium-en-af.en-af.dix.xml')

def node_to_string(node):
       buf = cStringIO.StringIO();
       Print(node, stream=buf, encoding='utf-8');
       return buf.getvalue();

def nodes_to_string(lst):
    return ''.join(node_to_string(n) for n in lst)

if __name__ == '__main__' and False:
    import sys
    
    try:
        automorf, bidix, side = sys.argv[1:]
        
    except ValueError:
        try:
            automorf, bidix = sys.argv[1:]
        except ValueError:
            usage()
            exit(1)

    

    

        
