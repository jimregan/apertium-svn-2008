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
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>

#include <apertium/hmm.h>
#include <apertium/tagger_data.h>
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
  cerr<<"Forbid and enforce rules are applied to the given HMM paramters\n\n";
  cerr<<"USAGE:\n";
  cerr<<name<<" --filein filein.prob --fileout fileout.prob\n\n";

  cerr<<"ARGUMENTS: \n"
      <<"   --filein|-i: To specify the file with the HMM parameter to process\n\n"
      <<"   --fileout|-o: To specify the file to which the HMM will be writen\n\n"
      <<"NOTE: Parameters are read from and writen to the file provided\n";
}

int main(int argc, char* argv[]) {
  string filein="";
  string fileout="";
  int c;
  int option_index=0;

  cerr<<"Command line: ";
  for(int i=0; i<argc; i++)
    cerr<<argv[i]<<" ";
  cerr<<"\n";

  while (true) {
    static struct option long_options[] =
      {
	{"filein",    required_argument, 0, 'i'},
	{"fileout",   required_argument, 0, 'o'},
	{0, 0, 0, 0}
      };

    c=getopt_long(argc, argv, "i:o:hv",long_options, &option_index);
    if (c==-1)
      break;
      
    switch (c) {
    case 'i':
      filein=optarg; 
      break;
    case 'o':
      fileout=optarg; 
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
  if (filein=="") {
    cerr<<"Error: You did not an input file (.prob). Use --filein to do that\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  if (fileout=="") {
    cerr<<"Error: You did not an output file (.prob). Use --fileout to do that\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  FILE *fin, *fout;

  fin=fopen(filein.c_str(), "r");
  check_file(fin, filein);

  cerr<<"Reading apertium-tagger data from file '"<<filein<<"' ... "<<flush;
  tagger_data.read(fin);
  fclose(fin);
  cerr<<"done.\n";

  HMM hmm(&tagger_data);
  hmm.apply_rules();

  fout=fopen(fileout.c_str(), "w");
  check_file(fout, fileout);
  cerr<<"Writing apertium-tagger data to file '"<<fileout<<"' ... "<<flush;
  tagger_data.write(fout);
  fclose(fout);
  cerr<<"done.\n";
}
