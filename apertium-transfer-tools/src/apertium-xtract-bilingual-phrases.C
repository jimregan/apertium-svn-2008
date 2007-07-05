/*
 * Copyright (C) 2006-2007 Felipe Sánchez-Martínez
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
#include <ctime>

#include "configure.H"
#include "Alignment.H"
#include "zfstream.H"

using namespace std;


void help(char *name) {
  cerr<<"USAGE:\n";
  cerr<<name<<" --input alignments --output phrases [--min <n>] [-max <n>] [--zlib]\n\n";
  cerr<<"ARGUMENTS: \n"
      <<"   --input|-i: Specify a file containing the alignments to work with\n"
      <<"   --output|-o: Specify the file in which the extracted phrases must be writen\n"
      <<"   --min|-n: Set the minimum length of the bilingual phrases extracted\n"
      <<"   --max|-m: Set the maximum length of the bilingual phrases extracted\n"
      <<"   --gzip|-z: Tell the program that the input file is gziped, the output will be gziped too\n"
      <<"   --help|-h: Show this help\n"
      <<"   --version|-v: Show version information\n\n";
  cerr<<"Note: To calculate the length of a bilingual phrase, only the source side is taken into account\n";
}

int main(int argc, char* argv[]) {
  int c;
  int option_index=0;

  string alignments_file="";
  string phrases_file="";
  bool use_zlib=false;

  int min=-1;
  int max=10000;

  cerr<<"Command line: ";
  for(int i=0; i<argc; i++)
    cerr<<argv[i]<<" ";
  cerr<<"\n\n";


  while (true) {
    static struct option long_options[] =
      {
	{"input",  required_argument,   0, 'i'},
	{"output", required_argument,   0, 'o'},
	{"gzip",        no_argument,    0, 'z'},
	{"min",    required_argument,   0, 'n'},
	{"max",    required_argument,   0, 'm'},
	{"help",        no_argument,    0, 'h'},
	{"version",     no_argument,    0, 'v'},
	{0, 0, 0, 0}
      };

    c=getopt_long(argc, argv, "i:o:n:m:zhv",long_options, &option_index);
    if (c==-1)
      break;
      
    switch (c) {
    case 'i':
      alignments_file=optarg;
      break;
    case 'o':
      phrases_file=optarg;
      break;
    case 'n':
      min=atoi(optarg);
      break;
    case 'm': 
      max=atoi(optarg);
      break;
    case 'z':
      use_zlib=true;
      break;
    case 'h': 
      help(argv[0]);
      exit(EXIT_SUCCESS);
      break;
    case 'v':
      cerr<<PACKAGE_STRING<<"\n";
      cerr<<"LICENSE:\n\n"
	  <<"   Copyright (C)  2006-2007 Felipe Sánchez-Martínez\n\n"
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

  if (alignments_file=="") {
    cerr<<"Error: No input file was given. You need to provide it with the --input option\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  if (phrases_file=="") {
    cerr<<"Error: No output file was given. You need to provide it with the --output option\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  if (min>max) {
    cerr<<"Error: The minimum length of the bilingual phrases to extract is greater than the maximum\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  cerr<<"Alignments will be read from file '"<<alignments_file<<"'\n";
  cerr<<"Bilingual phrases will be writen to file '"<<phrases_file<<"'\n";
  cerr<<"Bilingual phrases length will be between "<<min<<" and "<<max<<" words.\n\n";
  
  istream *falg;
  if (use_zlib) {
    falg = new gzifstream(alignments_file.c_str());
  }  else {
    falg = new ifstream(alignments_file.c_str());
  }

  if (falg->fail()) {
    cerr<<"Error: Cannot open input file '"<<alignments_file<<"'\n";
    delete falg;
    exit(EXIT_FAILURE);
  }

  ostream *fout;
  if(use_zlib) {
    fout = new gzofstream(phrases_file.c_str());
  } else {
    fout = new ofstream(phrases_file.c_str());
  }

  if (fout->fail()) {
    cerr<<"Error: Cannot open output file '"<<phrases_file<<"'\n";
    delete falg;
    delete fout;
    exit(EXIT_FAILURE);
  }

  time_t start_time, end_time;

  start_time=time(NULL);
  cerr<<"Bilingual phrases extraction started at: "<<ctime(&start_time);
  vector<Alignment> bilingual_phrases;
  string onealg;
  while (!falg->eof()) {
    getline(*falg,onealg);
    if(onealg.length()>0) {
      Alignment al(onealg);

      if(al.allwords_aligned()) {
	bilingual_phrases=al.extract_bilingual_phrases(min, max);

	for (unsigned i=0; i<bilingual_phrases.size(); i++)
	  (*fout)<<bilingual_phrases[i]<<"\n";
      } else {
	cerr<<"Warning: Alignment discarded due to words not aligned: ";
	cerr<<al.to_string()<<"\n";
      }
    }
  }

  end_time=time(NULL);
  cerr<<"Bilingual phrases extraction finished at: "<<ctime(&end_time)<<"\n";
  cerr<<"Bilingual phrases extraction took "<<difftime(end_time, start_time)<<" seconds\n";

  delete falg;
  delete fout;
}
