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
#include <cstdlib>
#include <sys/types.h>
#include <getopt.h>

#include <apertium/hmm.h>
#include <apertium/tagger_data.h>
#include <apertium/utf_converter.h>
#include "configure.H"


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

void help(char *name) {
  cerr<<"HMM parameters are writen in text format\n\n";
  cerr<<"USAGE:\n";
  cerr<<name<<" --file file.prob [--human]\n\n";

  cerr<<"ARGUMENTS: \n"
      <<"   --file|-f: To specify the file with the HMM parameter to process\n";
}


void
print_A(bool human_readable) {
  int i,j;

  cout<<"TRANSITION MATRIX (A)\n------------------------------\n";
  for(i=0; i<tagger_data.getN(); i++)
    for(j=0; j<tagger_data.getN(); j++) {
      if (human_readable)
	cout<<"A["<<UtfConverter::toUtf8(tagger_data.getArrayTags()[i])<<"]["
	    <<UtfConverter::toUtf8(tagger_data.getArrayTags()[j])<<"] = ";
      else
	cout<<"A["<<i<<"]["<<j<<"] = ";
      cout<<tagger_data.getA()[i][j]<<"\n";
    }
}

void
print_B(bool human_readable) {
  int i,k;

  cout<<"EMISSION MATRIX (B)\n-------------------------------\n";
  for(i=0; i<tagger_data.getN(); i++)
    for(k=0; k<tagger_data.getM(); k++) {
      if(tagger_data.getOutput()[k].find(i)!=tagger_data.getOutput()[k].end()) {
	if (human_readable) {
	  set<TTag> tags = tagger_data.getOutput()[k];
	  set<TTag>::iterator it;
	  wstring str=L"";
	  for(it=tags.begin(); it!=tags.end(); it++) {
	    if (str.length()>0)
	      str+=L", ";
	    str+=tagger_data.getArrayTags()[*it];
	  }
	  cout<<"B["<<UtfConverter::toUtf8(tagger_data.getArrayTags()[i])<<"]["
	      <<UtfConverter::toUtf8(str)<<"] = ";
	} else
	  cout<<"B["<<i<<"]["<<k<<"] = ";
	cout<<tagger_data.getB()[i][k]<<"\n";
      }
    }
}

int main(int argc, char* argv[]) {
  string file="";
  int c;
  int option_index=0;
  bool human_readable=false;

  cerr<<"Command line: ";
  for(int i=0; i<argc; i++)
    cerr<<argv[i]<<" ";
  cerr<<"\n";

  while (true) {
    static struct option long_options[] =
      {
	{"file",    required_argument, 0, 'f'},
	{"human",   no_argument,       0, 'u'},
	{0, 0, 0, 0}
      };

    c=getopt_long(argc, argv, "f:uhv",long_options, &option_index);
    if (c==-1)
      break;
      
    switch (c) {
    case 'f':
      file=optarg; 
      break;
    case 'u':
      human_readable=true;
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

  //Now we check the command line arguments
  if (file=="") {
    cerr<<"Error: You did not provide a file (.prob). Use --file to do that\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  FILE *fin;

  fin=fopen(file.c_str(), "r");
  check_file(fin, file);

  cerr<<"Reading apertium-tagger data from file '"<<file<<"' ... "<<flush;
  tagger_data.read(fin);
  fclose(fin);
  cerr<<"done.\n";

  print_A(human_readable);
  print_B(human_readable);
}
