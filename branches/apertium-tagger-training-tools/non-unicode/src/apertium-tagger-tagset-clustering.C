/*
 * Copyright (C) 2007 Felipe Sánchez-Martínez
 * 
 * author: Felipe Sánchez-Martínez
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
#include <getopt.h>

#include <clocale>
#include <cstdlib>

#include <deque>

#include <apertium/TaggerUtils.H>
#include <apertium/Collection.H>

#include "HMMStatesMerging.H"
//#include "HashMap.H"
#include "Utils.H"

using namespace std;

void help(char *name) {
  cerr<<"USAGE:\n";
  cerr<<name<<" --prob file.prob --threshold th [--divergences divergences.bin] [--clusters clusters.txt] [--restrictive] [--debug]\n";
  cerr<<"ARGUMENTS: \n"
      <<"   --prob|-p: file with the HMM parameters\n"
      <<"   --threshold|-t: threshold for the clustering\n"
      <<"   --divergences|-i: precalculated divergences\n"
      <<"   --clusters|-c: use previously defined clusters as a stating point\n"
      <<"   --restrictive|-r: Do not allow two clusters to be merged if they contain states\n"
      <<"                     further than the threshold distance\n"
      <<"   --debug|-d: Show debug information\n";
}

int main(int argc, char* argv[]) {
  int c;
  int option_index=0;

  double threshold=-1000.0;
  string probfile="";
  string divfile="";
  string clustersfile="";

  Utils::debug=false;
  bool restrictive=false;

  cerr<<"LOCALE: "<<setlocale(LC_ALL,"")<<"\n";  

  while (true) {
    static struct option long_options[] =
      {
	{"prob",         required_argument, 0, 'p'},
        {"threshold",    required_argument, 0, 't'},
        {"divergences",  required_argument, 0, 'i'},
        {"clusters",     required_argument, 0, 'c'},
	{"restrictive",  no_argument,       0, 'r'},
	{"debug",        no_argument,       0, 'd'},
	{"help",         no_argument,       0, 'h'},
	{"version",      no_argument,       0, 'v'},
	{0, 0, 0, 0}
      };

    c=getopt_long(argc, argv, "p:t:i:c:dhv",long_options, &option_index);
    if (c==-1)
      break;
      
    switch (c) {
    case 'p': 
      probfile=optarg;
      break;
    case 't': 
      threshold=atof(optarg);
      break;
    case 'i':
      divfile=optarg;
      break;
    case 'c':
      clustersfile=optarg;
      break;
    case 'r':
      restrictive=true;
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

  if (threshold==-1000) {
    cerr<<"Error: No threshold was given.\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  if (probfile=="") {
    cerr<<"Error: No file with the HMM parameters was given.\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  cerr<<"Threshold for the clustering: "<<threshold<<"\n";
  cerr<<"Is the calculation of the distance between clusters restrictive?: "<<restrictive<<"\n";


  FILE *fprob;
  fprob = fopen(probfile.c_str(), "r");
  if (!fprob) file_name_error(probfile);

  HMMStatesMerging states_merging(fprob, divfile, restrictive);
  fclose(fprob);

  vector<set<int> > states_to_merge;
  vector<set<int> > amb_class_to_merge;
  Collection new_output;

  states_to_merge=states_merging.bottom_up_aglomerative(threshold, clustersfile);

  amb_class_to_merge=states_merging.calculate_merged_ambiguity_classes(states_to_merge, new_output);

  cerr<<"The final tagset has "<<states_to_merge.size()<<" tags and "
      <<amb_class_to_merge.size()<<" ambiguity classes.\n";

  vector<set<int> >::iterator itv;
  set<int>::iterator its;

  cout<<states_to_merge.size()<<"\n";
  for(itv=states_to_merge.begin(); itv!=states_to_merge.end(); itv++) {
    cout<<(*itv).size()<<" ";
    for(its=(*itv).begin(); its!=(*itv).end(); its++)
      cout<<*its<<" ";
    cout<<"\n";
  }

  //cout<<"---------------------------------------------------\n";

  cout<<amb_class_to_merge.size()<<"\n";
  for(itv=amb_class_to_merge.begin(); itv!=amb_class_to_merge.end(); itv++) {
    cout<<(*itv).size()<<" ";
    for(its=(*itv).begin(); its!=(*itv).end(); its++)
      cout<<*its<<" ";
    cout<<"\n";
  }

  //cout<<"---------------------------------------------------\n";
  
  for(int i=0; i<new_output.size(); i++) {
    set<int> amb_class=new_output[i];
    cout<<amb_class.size()<<" ";
    for(its=amb_class.begin(); its!=amb_class.end(); its++)
      cout<<*its<<" ";
    cout<<"\n";
  }

  exit(EXIT_SUCCESS);
} 
