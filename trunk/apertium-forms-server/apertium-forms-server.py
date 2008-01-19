#!/usr/bin/python2.5 -Wall
# coding=utf-8
# -*- encoding: utf-8 -*-

import web, os, codecs, sys;
from config import Config;
from pair import Pair;
from interface import Interface;

sys.stdout = codecs.getwriter('utf-8')(sys.stdout);
sys.stderr = codecs.getwriter('utf-8')(sys.stderr);

class Globals: #{
	config = Config(os.getcwd() + '/config/config.xml');	
#}

Globals.config.parse_config();
i = Interface();

urls = (
    '/add', 'add',
    '/commit/(.*)', 'commit',
    '/(.*)', 'form'
)

class commit: #{
    
    def GET(self, name): #{
        print >> sys.stderr , 'Commit called for ' + name;
        pairs = Globals.config.get_pairs();
        if name in pairs: #{
	    print '<html>';
            print 'Commit called for ' + name + '<br />';
            pair = pairs[name];
	    print 'Comitting...<br />';
            pair.commit();
	    print 'Committed.</html>';
	    return;
        #}

	print 'Error in commit.';
    #}
#}

class form: #{

    def GET(self, name): #{
	pairs = Globals.config.get_pairs();
	default_pair = pairs.keys()[0];
	tags = Globals.config.pairs[default_pair].get_tags();
	default_tag = tags.keys()[0];

	dictionary_left = Globals.config.pairs[default_pair].dictionary['left'];
	dictionary_bidix = Globals.config.pairs[default_pair].dictionary['bidix'];
	dictionary_right = Globals.config.pairs[default_pair].dictionary['right'];

	paradigms_left = Globals.config.pairs[default_pair].dictionary['left'].get_paradigms_by_tag(default_tag);
	paradigms_right = Globals.config.pairs[default_pair].dictionary['right'].get_paradigms_by_tag(default_tag);

	glosses_left = Globals.config.pairs[default_pair].dictionary['left'].get_glosses();
	glosses_right = Globals.config.pairs[default_pair].dictionary['right'].get_glosses();

	post_data = {
	    'selected_pair': default_pair, 
	    'selected_tag': default_tag, 
	    'tags': tags, 
	    'committing': 'no', 
	    'previewing': 'off', 
	    'left_lemma': '', 
	    'right_lemma': '',
	    'left_comment': '', 
	    'right_comment': '',
	    'left_paradigm': '',
	    'right_paradigm': '',
	    'left_glosses': glosses_left,
	    'right_glosses': glosses_right,
	    'left_display_mode': dictionary_left.get_display_by_tag(default_tag),
	    'right_display_mode': dictionary_right.get_display_by_tag(default_tag),
	    'left_paradigms': paradigms_left,
	    'right_paradigms': paradigms_right,
	    'restriction': '',
	    'pairs': pairs
	};

	print i.display(post_data);
    #}
#}

class add: #{

    def POST(self): #{
        post_data = web.input(name = []);
	pairs = Globals.config.get_pairs();

	current_pair = post_data['selected_pair'];
	current_tag = post_data['selected_tag'];
	left_paradigm = '';
	right_paradigm = '';

	try: #{
            left_paradigm = post_data['left_paradigm'];
        except: #{
	    print >> sys.stderr, 'Error';
        #}
	try: #{
            right_paradigm = post_data['right_paradigm'];
        except: #{
	    print >> sys.stderr, 'Error';
        #}

	dictionary_left = Globals.config.pairs[current_pair].dictionary['left'];
	dictionary_right = Globals.config.pairs[current_pair].dictionary['right'];
	dictionary_bidix = Globals.config.pairs[current_pair].dictionary['bidix'];

	tags = Globals.config.pairs[current_pair].get_tags();
	paradigms_left = dictionary_left.get_paradigms_by_tag(current_tag);
	paradigms_right = dictionary_right.get_paradigms_by_tag(current_tag);
	glosses_left = dictionary_left.get_glosses();
	glosses_right = dictionary_right.get_glosses();

	committing = 'no';
	try: #{
		print >> sys.stderr, 'commit box: ' , post_data['commit_box'];
		if post_data['commit_box'] == 'Commit': #
			committing = 'yes';
		#}
	#}
	except: #{
		print >> sys.stderr, 'commit box error';
	#}

	post_data = {
	    'commit_box': '',
	    'committing': committing,
	    'selected_pair': current_pair, 
	    'selected_tag': current_tag, 
	    'tags': tags, 
	    'previewing': 'on', 
	    'left_comment': post_data['left_comment'], 
	    'right_comment': post_data['right_comment'],
	    'left_lemma': post_data['left_lemma'], 
	    'right_lemma': post_data['right_lemma'],
	    'left_dictionary': dictionary_left,
	    'bidix_dictionary': dictionary_bidix,
	    'right_dictionary': dictionary_right,
	    'left_paradigm': left_paradigm,
	    'right_paradigm': right_paradigm,
	    'left_paradigms': paradigms_left,
	    'left_glosses': glosses_left,
	    'right_glosses': glosses_right,
	    'left_display_mode': dictionary_left.get_display_by_tag(current_tag),
	    'right_display_mode': dictionary_right.get_display_by_tag(current_tag),
	    'right_paradigms': paradigms_right,
	    'restriction': post_data['restriction'],
	    'pairs': pairs
	};

	print i.display(post_data);
    #}
#}

if __name__ == "__main__": #{
#    try: #{
#        pid = os.fork();
#        if pid > 0: #{
#            sys.exit(0);
#        #}
#    except OSError, e: #{
#        print >>sys.stderr, "fork #1 failed: %d (%s)" % (e.errno, e.strerror); 
#        sys.exit(1);
#    #}
#
#    #os.chdir("/");
#    os.setsid();
#    os.umask(0);
#
#    try: #{
#        pid = os.fork();
#        if pid > 0: #{
#            print "Daemon PID %d" % pid;
#            sys.exit(0);
#        #}
#    except OSError, e: #{
# #       print >>sys.stderr, "fork #2 failed: %d (%s)" % (e.errno, e.strerror);
#        sys.exit(1);
#    #}
#
#    sys.stderr = open(Globals.config.log_file, 'a+')
    web.run(urls, globals());
#}
