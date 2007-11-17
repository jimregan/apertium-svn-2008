#!/usr/bin/python2.5 -Wall
# coding=utf-8
# -*- encoding: utf-8 -*-

import xmlrpclib;
import web;

serv = xmlrpclib.Server('http://localhost:8081');

class Globals: #{
	counter = 0;
#}

render = web.template.render('templates/');

urls = (
    '/add', 'add',
    '/(.*)', 'form'
)

class form: #{

    def GET(self, name): #{
	pairs = serv.language_pairs();
	post_data = {'selected_pair': 'apertium-es-gl'};
	print render.index(pairs, post_data, cache=False);
    #}
#}

class add: #{

    def POST(self): #{
        post_data = web.input(name = []);
        print dir(post_data);
        print post_data
	pairs = serv.language_pairs();
	print render.index(pairs, post_data, cache=False);
    #}
#}

if __name__ == "__main__": #{
    web.run(urls, globals());
#}
