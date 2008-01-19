/*
 * Copyright (C) 2007 Felipe Sánchez-Martínez
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

#include <iostream>
#include <fstream>
#include <string>

#include <getopt.h>
#include <clocale>

#include<apertium/TaggerUtils.H>

#include "TaggerMerging.H"
#include "Utils.H"

using namespace std;

void help(char *name) {
  cerr<<"USAGE:\n";
  cerr<<name<<" --prob file.prob --counts file.counts --tagclusters clusters.txt --length corpuslength [--eval --tagged file.tagged --untagged file.untagged] [--debug]\n";
  cerr<<"ARGUMENTS: \n"
      <<"   --prob|-p: file with the fine-grained HMM parameters\n"
      <<"   --counts|-c: file with the counts collected during training\n"
      <<"   --tagclusters|-t: file generated with apertium-tagger-tagset-clustering\n"
      <<"   --length|-l: Number of words of the corpus used to train the faine-grained HMM parameters\n"
      <<"   --eval|-e: To evaluate the part-of-speech tagging performance\n"
      <<"   --tagged|-g: Used to provide the tagged corpus against the tagger\n"
      <<"                parameters are avaluated\n"
      <<"   --untagged|-u: Used to provide the untagged corpus that will be tagged.\n"
      <<"   --debug|-d: Show debug information\n";
}


int main(int argc, char* argv[]) {
  int c;
  int option_index=0;

  string probfile="";
  string countsfile="";
  string tagclustersfile="";
  int corpus_length=0;
  Utils::debug=false;

  bool evaltagger=false;
  string taggedfile="";
  string untaggedfile="";

  cerr<<"LOCALE: "<<setlocale(LC_ALL,"")<<"\n";  

  while (true) {
    static struct option long_options[] =
      {
	{"prob",         required_argument, 0, 'p'},
        {"counts",       required_argument, 0, 'c'},
        {"tagclusters",  required_argument, 0, 't'},
        {"length",       required_argument, 0, 'l'},
	{"tagged",       required_argument, 0, 'g'},
	{"untagged",     required_argument, 0, 'u'},
	{"eval",         no_argument,       0, 'e'},
	{"debug",        no_argument,       0, 'd'},
	{"help",         no_argument,       0, 'h'},
	{"version",      no_argument,       0, 'v'},
	{0, 0, 0, 0}
      };

    c=getopt_long(argc, argv, "p:c:t:l:g:u:edhv",long_options, &option_index);
    if (c==-1)
      break;
      
    switch (c) {
    case 'p': 
      probfile=optarg;
      break;
    case 'c': 
      countsfile=optarg;
      break;
    case 't':
      tagclustersfile=optarg;
      break;
    case 'l':
      corpus_length=atoi(optarg);
      break;
    case 'g':
      taggedfile=optarg;
      break;
    case 'u':
      untaggedfile=optarg;
      break;
    case 'e':
      evaltagger=true;
      break;
    case 'd':
      Utils::debug=true;
      break;
    case 'h': 
      help(argv[0]);
      exit(EXIT_SUCCESS);
      break;
    case 'v':
      cerr<<"LICENSE:\n\n"
	  <<"   Copyright (C) 2007 Felipe Sánchez Martínez\n\n"
	  <<"   This program is free software; you can redistribute it and/or\n"
	  <<"   modify it under the terms of the GNU General Public License as\n"
	  <<"   published by the Free Software Foundation; either version 2 of the\n"
	  <<"   License, or (at your option) any later version.\n"
	  <<"   This program is distributed in the hope that it will be useful, but\n"
	  <<"   WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	  <<"   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n"
	  <<"   General Public License for more details.\n"
	  <<"\n"
	  <<"   You should have received a copy of the GNU General Public License\n"
	  <<"   along with this program; if not, write to the Free Software\n"
	  <<"   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA\n"
	  <<"   02111-1307, USA.\n";
      exit(EXIT_SUCCESS);
      break;    
    default:
      help(argv[0]);
      exit(EXIT_FAILURE);
      break;
    }
  }

  if (probfile=="") {
    cerr<<"No file with the fine-grained HMM parameters was given";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }
  
  if (countsfile=="") {
    cerr<<"No file with the counts collected during training was given";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  if (tagclustersfile=="") {
    cerr<<"No file with tags clusters was given";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  if (corpus_length==0) {
    cerr<<"The length of the corpus used to train the fine-grained HMM parameters was not given";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  if (evaltagger) {
    if (taggedfile=="") {
      cerr<<"No file with the tagged corpus to use for the evaluation was given";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if(untaggedfile=="") {
      cerr<<"No file with the untagged corpus to use for the evaluation was given";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  FILE* fprob;
  ifstream fcounts, ftagclus;

  fprob = fopen(probfile.c_str(), "r");
  if (!fprob) file_name_error(probfile);

  fcounts.open(countsfile.c_str(), ios::in);
  if(fcounts.fail()) file_name_error(countsfile);

  ftagclus.open(tagclustersfile.c_str(), ios::in);
  if(ftagclus.fail()) file_name_error(tagclustersfile);

  TaggerMerging merged_tagger(fprob, fcounts, ftagclus, corpus_length);

  fclose(fprob);
  fcounts.close();
  ftagclus.close();

  if (evaltagger) {
    FILE *ftagged, *funtagged;
    ftagged = fopen(taggedfile.c_str(), "r");
    if (!ftagged) file_name_error(taggedfile);

    funtagged = fopen(untaggedfile.c_str(), "r");
    if (!funtagged) file_name_error(untaggedfile);

    merged_tagger.eval_tagger(ftagged, funtagged);

    fclose(ftagged);
    fclose(funtagged);

    merged_tagger.print_evaluation();
  } else 
    merged_tagger.tagger(stdin, stdout);
}
