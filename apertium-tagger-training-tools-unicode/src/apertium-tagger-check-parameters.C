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

#include <iostream>
#include <string>
#include <getopt.h>
#include <clocale>

#include <apertium/tagger_data.h>
#include <apertium/utf_converter.h>

#include "configure.H"

#define ZERO 1e-10

using namespace std;

//Global vars
TaggerData tagger_data;

void check_file(FILE *f, const string& path) {
  if (!f) {
    cerr<<"Error: cannot open file '"<<path<<"'\n";
    exit(EXIT_FAILURE);
  }
}

void help(char *name) {
  cerr<<"USAGE:\n";
  cerr<<name<<" --file file.prob\n\n";
  cerr<<"ARGUMENTS: \n"
      <<"   --file|-f: To specify a file containing the tagger parameters to check\n";
}

void check() {
  cout<<"Cheking A ...\n";
  for (int i=0; i<tagger_data.getN(); i++) {
    double sum_prob=0.0;
    for(int j=0; j<tagger_data.getN(); j++) {
      sum_prob+=tagger_data.getA()[i][j];
    }
    cout<<"SUM A["<<UtfConverter::toUtf8(tagger_data.getArrayTags()[i])<<"("<<i<<")][*]="<<sum_prob<<"\n";
  }

  cout<<"Checking B ...\n";
  for (int i=0; i<tagger_data.getN(); i++) {
    double sum_prob=0.0;
    for(int k=0; k<tagger_data.getM(); k++) {
      if (tagger_data.getOutput()[k].find(i)!=tagger_data.getOutput()[k].end())
	sum_prob+=tagger_data.getB()[i][k];
    }
    cout<<"SUM B["<<UtfConverter::toUtf8(tagger_data.getArrayTags()[i])<<"("<<i<<")][*]="<<sum_prob<<"\n";
  }
}

int main(int argc, char* argv[]) {
  string file="";

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
	{"file",    required_argument, 0, 'f'},
	{"help",       no_argument,    0, 'h'},
	{"version",    no_argument,    0, 'v'},
	{0, 0, 0, 0}
      };

    c=getopt_long(argc, argv, "f:hv",long_options, &option_index);
    if (c==-1)
      break;
      
    switch (c) {
    case 'f': 
      file=optarg;
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

  if (file=="") {
    cerr<<"Error: You did not provide a tagger parameters file. Use --file to do that\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  FILE *f;

  f=fopen(file.c_str(), "r");
  check_file(f, file);

  cerr<<"Reading tagger parameters from file '"<<file<<"' ... "<<flush;
  tagger_data.read(f);
  cerr<<"done.\n";
  fclose(f);

  check();
}
