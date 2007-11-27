/*
 * Copyright (C) 2004-2006 Felipe Sánchez-Martínez
 * Copyright (C) 2006 Universitat d'Alacant
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#include "configure.H"
#include "TaggerEval.H"
#include "Utils.H"

#include <apertium/tagger_utils.h>
#include <apertium/tagger_word.h>
#include <apertium/tagger_data.h>

#include <iostream>
#include <getopt.h>
#include <clocale>

using namespace std;

void help(char *name) {
  cerr<<"USAGE:\n\n"
      <<name<<" --prob probfile --tagged taggedfile --untagged untaggedfile [--debug]\n\n"
      <<"ARGUMENTS:\n"
      <<"   --prob|-p:    Used to provide the apertium-tagger parameters \n"
      <<"                 you want to evaluate\n"
      <<"   --tagged|-t:  Used to provide the tagged corpus against the tagger\n"
      <<"                 parameters are avaluated\n"
      <<"   --untagged|-u: Used to provide the untagged corpus that will be tagged.\n"
      <<"                 This corpus is the same provided with the --tagged\n"
      <<"                 argument, but with the ambiguty unsolved. It can be generated\n"
      <<"                 with the lt-proc (from lttoolbox) program\n"
      <<"   --debug|-d:   Print debug information while tagging\n"
      <<"   --help|-h:    Prints this help message\n"
      <<"   --version|-v: Print version information and exits\n\n";

  cerr<<"LICENSE:\n\n"
      <<"Copyright (C) 2006 Felipe Sánchez-Martínez\n\n"
      <<"This program is free software; you can redistribute it and/or\n"
      <<"modify it under the terms of the GNU General Public License as\n"
      <<"published by the Free Software Foundation; either version 2 of the\n"
      <<" License, or (at your option) any later version.\n"
      <<"This program is distributed in the hope that it will be useful, but\n"
      <<"WITHOUT ANY WARRANTY; without even the implied warranty of\n"
      <<"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n"
      <<"General Public License for more details.\n"
      <<"\n"
      <<"You should have received a copy of the GNU General Public License\n"
      <<"along with this program; if not, write to the Free Software\n"
      <<"Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA\n"
      <<"02111-1307, USA.\n";
}

int main(int argc, char *argv[]) {
  cerr<<"LOCALE: "<<setlocale(LC_ALL,"")<<"\n";

  string probfile="";
  string taggedfile="";
  string untaggedfile="";

  cerr<<PACKAGE_STRING<<"\n";
  cerr<<"Command line: ";
  for(int i=0; i<argc; i++)
    cerr<<argv[i]<<" ";
  cerr<<"\n";

  Utils::debug=false;

  int c;
  int option_index=0;

  while (true) {
    static struct option long_options[] =
      {
	{"prob",     required_argument, 0, 'p'},
	{"tagged",   required_argument, 0, 't'},
	{"untagged", required_argument, 0, 'u'},
	{"debug",      no_argument,     0, 'd'},
	{"help",       no_argument,     0, 'h'},
	{"version",    no_argument,     0, 'v'},
	{0, 0, 0, 0}
      };

    c=getopt_long(argc, argv, "p:t:u:dhv",long_options, &option_index);
    if (c==-1)
      break;
      
    switch (c) {
    case 'p': {
      probfile=optarg;
      break;
    }
    case 't': {
      taggedfile=optarg;
      break;
    }
    case 'u': {
      untaggedfile=optarg;
      break;
    }
    case 'v': {
      cerr<<PACKAGE_STRING<<"\n";
      exit(EXIT_SUCCESS);
    }
    case 'd':
      Utils::debug=true;
      break;
    case 'h':
      help(argv[0]); 
      exit(EXIT_SUCCESS);
      break;     
    default:
      help(argv[0]);
      exit(EXIT_FAILURE);
      break;
    }    
  }

  if(probfile=="") {
    cerr<<"Error: No apertium-tagger parameters file was given. Use --prob argument to provide that file\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  if(taggedfile=="") {
    cerr<<"Error: No tagged file was given. Use --tagged argument to provide that file\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  if(untaggedfile=="") {
    cerr<<"Error: No untagged file was given. Use --untagged argument to provide that file\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }


  FILE *ftagged, *funtagged, *fprob;

  fprob = fopen(probfile.c_str(), "r");
  if (!fprob) tagger_utils::file_name_error(probfile);

  ftagged = fopen(taggedfile.c_str(), "r");
  if (!fprob) tagger_utils::file_name_error(taggedfile);

  funtagged = fopen(untaggedfile.c_str(), "r");
  if (!fprob) tagger_utils::file_name_error(untaggedfile);


  TaggerEval taggereval(fprob);
  fclose(fprob);

  taggereval.eval_tagger(ftagged, funtagged);

  fclose(ftagged);
  fclose(funtagged);

  taggereval.print_evaluation();

  exit(EXIT_SUCCESS);
}
