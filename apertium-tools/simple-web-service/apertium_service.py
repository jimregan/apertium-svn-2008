#!/usr/bin/python2.5 -Wall
# coding=utf-8
# -*- encoding: utf-8 -*-

import web
import logging
from subprocess import Popen, PIPE

import sys;
import codecs;
import os
import os.path as path
sys.stdout = codecs.getwriter('utf-8')(sys.stdout);
sys.stderr = codecs.getwriter('utf-8')(sys.stderr);


logging.basicConfig(level=logging.DEBUG,
                    format='%(asctime)s %(levelname)-8s %(message)s',
                    datefmt='%a, %d %b %Y %H:%M:%S',
                    filename='apertium.log',
                    filemode='w')


urls = (
    '/translate/(.*)', 'translate',
#    '(.*)', 'static'
)

class translate:
    def POST(self, pair):
        i = web.input()
        proc = Popen(['/usr/local/bin/apertium-deshtml'], stdin = PIPE, stdout = PIPE)
        text = proc.communicate(i.text)[0]
        logging.debug('After dehtml: %s' % text)
        proc = Popen(['/usr/local/bin/apertium', '-u', pair.strip('/')], stdin = PIPE, stdout = PIPE)
        text = proc.communicate(text)[0]
        logging.debug('After translation: %s' % text)
        proc = Popen(['/usr/local/bin/apertium-rehtml'], stdin = PIPE, stdout = PIPE)
        text = proc.communicate(text)[0]
        logging.debug('After rehtml: %s' % text)
        print text

class static:
    def GET(self, filename):
        if filename == '' or filename == '/':
            filename = 'index.html'

        print open(path.join(os.getcwd(), *filename.split('/'))).read()


if __name__ == "__main__": web.run(urls, globals())
