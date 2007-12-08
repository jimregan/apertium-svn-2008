import os
import re
import cStringIO
from subprocess import Popen, PIPE

from Ft.Xml.Domlette import NonvalidatingReader;
from Ft.Xml.Domlette import Print, PrettyPrint;
from Ft.Xml.XPath import Evaluate;

class Globals:
    pass

def load_xml_file(filename):
    return NonvalidatingReader.parseUri(filename)

def usage():
    print "usage <monodix path> <bidix path> [<side>]"

def extract_entries(doc):
    for e in doc.xpath("/dictionary/section[@id='main']/e"):
        pass

def test_load():
    Globals.automorf = '/home/wynand/apertium-git/apertium-en-af/af-en.automorf.bin'

def test_load_file():
    return load_xml_file('file:///home/wynand/apertium-git/apertium-en-af/apertium-en-af.en-af.dix.xml')

def test_load_elements(doc):
    return doc.xpath("/dictionary/section[@id='main']/e/p")

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

    

    

        
