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

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <float.h>
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>
#include <clocale>

#include <apertium/hmm.h>
#include <apertium/morpho_stream.h>
#include <apertium/ttag.h>
#include <apertium/tagger_word.h>
#include <apertium/collection.h>
#include <apertium/tagger_data.h>
#include <apertium/tsx_reader.h>
#include "configure.H"
#include "SmoothUtils.H"

#define ZERO 1e-10


using namespace std;


//Global vars
TaggerData tagger_data;
TTag eos; //End-of-sentence tag

void check_file(FILE *f, const string& path) {
  if (!f) {
    cerr<<"Error: cannot open file '"<<path<<"'\n";
    exit(EXIT_FAILURE);
  }
}

void 
supervised(FILE *ftagged, FILE *funtagged) {
  int i, j, k, nw=0;

  map<int, map<int, double> > tags_pair; //NxN
  map<int, map<int, double> > emis;     //NxM
  map<int, double> ambclass_count;      //M
  map<int, double> tags_count;          //N
  map<int, double> tags_count_for_emis; //N
  
  MorphoStream stream_tagged(ftagged, true, &tagger_data);
  MorphoStream stream_untagged(funtagged, true, &tagger_data);
  
  TaggerWord *word_tagged=NULL, *word_untagged=NULL;
  
  set<TTag> tags;
 
  // Init counters
  for(i=0; i<tagger_data.getN(); i++) {
    tags_count[i]=0;
    tags_count_for_emis[i]=0;
    for(j=0; j<tagger_data.getN(); j++)
      tags_pair[i][j]=0;
  }
  for(k=0; k<tagger_data.getM(); k++) {
    ambclass_count[k]=0;
    for(i=0; i<tagger_data.getN(); i++) {
      if (tagger_data.getOutput()[k].find(i)!=tagger_data.getOutput()[k].end())
        emis[i][k] = 0;
    }  
  }
 
  TTag tag1, tag2;  
  tag1 = eos; // The first seen tag is the end-of-sentence tag
  
  word_tagged = stream_tagged.get_next_word();
  word_untagged = stream_untagged.get_next_word();
  while(word_tagged) {
    wcerr<<*word_tagged<<L" -- "<<*word_untagged<<L"\n"; 

    if (word_tagged->get_superficial_form()!=word_untagged->get_superficial_form()) {              
      wcerr<<L"\nTagged text (.tagged) and analyzed text (.untagged) streams are not aligned.\n";
      wcerr<<L"Take a look at tagged text (.tagged).\n";
      wcerr<<L"Perhaps this is caused by a multiword unit that is not a multiword unit in one of the two files.\n";
      wcerr<<*word_tagged<<L" -- "<<*word_untagged<<L"\n"; 
      exit(1);
    }

    if (++nw%100==0) cerr<<'.'<<flush; 
    
    tag2 = tag1;
   
    if (word_untagged==NULL) {
      wcerr<<L"word_untagged==NULL\n";
      exit(1);
    }

    if (word_tagged->get_tags().size()==0) // Unknown word
      tag1 = -1;
    else if (word_tagged->get_tags().size()>1) // Ambiguous word
      wcerr<<L"Error in tagged text. An ambiguous word was found: "<<word_tagged->get_superficial_form()<<L"\n";
    else
      tag1 = *(word_tagged->get_tags()).begin();


    if ((tag1>=0) && (tag2>=0)) {
      tags_pair[tag2][tag1]++;
      tags_count[tag2]++;
    }

    if (word_untagged->get_tags().size()==0) { // Unknown word
      tags = tagger_data.getOpenClass();
    } else if (tagger_data.getOutput().has_not(word_untagged->get_tags())) { //We are training, there is no problem
      wstring errors;
      errors = L"A new ambiguity class was found. I cannot continue.\n";
      errors+= L"Word '"+word_untagged->get_superficial_form()+L"' not found in the dictionary.\n";
      errors+= L"New ambiguity class: "+word_untagged->get_string_tags()+L"\n";
      errors+= L"Take a look at the dictionary, then retrain.";
      tagger_utils::fatal_error(errors);      
    } else {
      tags = word_untagged->get_tags();
    }

    k=tagger_data.getOutput()[tags];
    if(tag1>=0) {
      if (tagger_data.getOutput()[k].find(tag1)!=tagger_data.getOutput()[k].end()) {     
	emis[tag1][k]++;
	tags_count_for_emis[tag1]++;
	ambclass_count[k]++;
      } else {
	cerr<<"Warning: Ambiguity class "<<k<<" is emmited from tag "<<tag1<<" but it should not\n";
      }
    }
  
    delete word_tagged;
    word_tagged=stream_tagged.get_next_word();
    delete word_untagged;
    word_untagged=stream_untagged.get_next_word();       
  }
  
  SmoothUtils::calculate_smoothed_parameters(tagger_data, tags_count, tags_pair, ambclass_count, emis, tags_count_for_emis, nw);  
  
  cerr<<"Number of words processed: "<<nw<<"\n";  
}

void apply_rules() {
  bool found;

  for(int i=0; i<tagger_data.getForbidRules().size(); i++) {
    tagger_data.getA()[tagger_data.getForbidRules()[i].tagi][tagger_data.getForbidRules()[i].tagj] = ZERO;
  }

  for(int i=0; i<tagger_data.getEnforceRules().size(); i++) {
    for(int j=0; j<tagger_data.getN(); j++) {
      found = false;
      for (int j2=0; j2<tagger_data.getEnforceRules()[i].tagsj.size(); j2++) {
        if (tagger_data.getEnforceRules()[i].tagsj[j2]==j) {
          found = true;
          break;
        }
      }
      if (!found) {
        tagger_data.getA()[tagger_data.getEnforceRules()[i].tagi][j] = ZERO;
      }
    }
  }

  // Normalize probabilities
  for(int i=0; i<tagger_data.getN(); i++) {
    double sum=0;
    for(int j=0; j<tagger_data.getN(); j++)
      sum += tagger_data.getA()[i][j];
    for(int j=0; j<tagger_data.getN(); j++) {
      if(sum>0)
        tagger_data.getA()[i][j] = tagger_data.getA()[i][j]/sum;
      else
        tagger_data.getA()[i][j] = 0;
    }
  }
}

void help(char *name) {
  cerr<<"USAGE:\n";
  cerr<<name<<" --tsxfile tsxfile --dicfile file.dic --tagged file.tagger --untagged file.untagged --outfile fileout.prob [--norules]\n\n";

  cerr<<"ARGUMENTS: \n"
      <<"   --tsxfile|-x: To provide the tagger specification file in XML\n"
      <<"   --dicfile|-d: To specify the expanded dictionary that will be used to extract the\n"
      <<"            ambiguity classes\n"
      <<"   --tagged|-t: To specify the file with the tagged corpus to be used for training\n"
      <<"   --untagged|-u: To specify the file with the untagged corpus to be used for training\n"
      <<"   --outfile|-o: To specify the file in which the new parameters will be stored\n"
      <<"   --norules|-n: Do not use forbid and enforce rules\n";
}


int main(int argc, char* argv[]) {
  string tsxfile="";
  string filedic="";
  string filetagged="";
  string fileuntagged="";
  string fileout="";

  bool use_forbid_enforce_rules=true;

  int c;
  int option_index=0;

  cerr<<"LOCALE: "<<setlocale(LC_ALL,"")<<"\n";

  cerr<<"Command line: ";
  for(int i=0; i<argc; i++)
    cerr<<argv[i]<<" ";
  cerr<<"\n";

  while (true) {
    static struct option long_options[] =
      {
	{"tsxfile",  required_argument, 0, 'x'},
	{"dicfile",  required_argument, 0, 'd'},
	{"tagged",   required_argument, 0, 't'},
	{"untagged", required_argument, 0, 'u'},
	{"outfile",  required_argument, 0, 'o'},
	{"norules",    no_argument,     0, 'n'},
	{"help",       no_argument,     0, 'h'},
	{"version",    no_argument,     0, 'v'},
	{0, 0, 0, 0}
      };

    c=getopt_long(argc, argv, "x:d:t:u:o:nhv",long_options, &option_index);
    if (c==-1)
      break;
      
    switch (c) {
    case 'x': 
      tsxfile=optarg;
      break;
    case 'd': 
      filedic=optarg;
      break;
    case 't': 
      filetagged=optarg;
      break;
    case 'u': 
      fileuntagged=optarg;
      break;
    case 'o': 
      fileout=optarg;
      break;
    case 'n': 
      use_forbid_enforce_rules=false;
      break;
    case 'h': 
      help(argv[0]);
      exit(EXIT_SUCCESS);
      break;
    case 'v':
      cerr<<PACKAGE_STRING<<"\n";
      cerr<<"LICENSE:\n\n"
	  <<"   Copyright (C) 2006 Felipe Sánchez Martínez\n\n"
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

  if (tsxfile=="") {
    cerr<<"Error: You did not provide a tagger specification file (.tsx). Use --tsxfile to do that\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  if (filedic=="") {
    cerr<<"Error: You did not provide an expanded dictionary file. Use --dicfile to do that\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }
 
  if (filetagged=="") {
    cerr<<"Error: You did not provide a tagged corpus. Use --tagged to do that\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  if (fileuntagged=="") {
    cerr<<"Error: You did not provide a untagged corpus. Use --untagged to do that\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  if(fileout=="") {
    cerr<<"Error: You did not provide an output file for the tagger parameters. Use --outfile to do that\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }



  TSXReader treader;
  treader.read(tsxfile);
  tagger_data=treader.getTaggerData();

  FILE *fdic, *ftagged, *funtagged, *fout;

  HMM hmm(&tagger_data);
  hmm.set_debug(true);
  hmm.set_eos((tagger_data.getTagIndex())[L"TAG_SENT"]);

  TaggerWord::setArrayTags(tagger_data.getArrayTags());
  eos=(tagger_data.getTagIndex())[L"TAG_SENT"];

  fdic=fopen(filedic.c_str(), "r");
  check_file(fdic, filedic);
  ftagged=fopen(filetagged.c_str(), "r");
  check_file(ftagged, filetagged);
  funtagged=fopen(fileuntagged.c_str(), "r");
  check_file(funtagged, fileuntagged);

  cerr<<"Calculating ambiguity classes ... "<<flush;
  hmm.read_dictionary(fdic);
  cerr<<"done.\n";
  fclose(fdic);

  supervised(ftagged, funtagged);
  fclose(ftagged);
  fclose(funtagged);

  if (use_forbid_enforce_rules) {
    cerr<<"Applying forbid and enforce rules ... "<<flush;
    apply_rules();
    cerr<<"done.\n";
  }

  fout=fopen(fileout.c_str(), "w");
  check_file(fout, fileout);
  cerr<<"Writing apertium-tagger parameters to file '"<<fileout<<"' ... "<<flush;
  tagger_data.write(fout);
  cerr<<"done.\n";
  fclose(fout);
}
