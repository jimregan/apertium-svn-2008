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
#include <clocale>

#include <lttoolbox/FSTProcessor.H>

#include "configure.H"
#include "Alignment.H"
#include "ATXReader.H"
#include "LexicalizedWords.H"
#include "AlignmentTemplate.H"
#include "zfstream.H"


using namespace std;


void help(char *name) {
  cerr<<"USAGE:\n";
  cerr<<name<<" --atx file.atx --bil bildic --input phrases --output alignment_templates [--equalcat] [--noword4word] [--gzip]\n\n";
  cerr<<"ARGUMENTS: \n"
      <<"   --atx|-x: Specify a file with the alignment-template extractor's configuration\n"
      <<"   --bil|-b: Specify the bilingual dictionary (in binary format) to be used\n"
      <<"   --input|-i: Specify a file containing the bilingual phrases to work with\n"
      <<"   --output|-o: Specify the file in which the extracted alignment templates must be writen\n"
      <<"   --equalcat|-e: Discard alignment templates if aligned open words differ at the first tag\n"
      <<"   --noword4word|-w: Discard alignment templates which are equivalent to word-for-word translation\n"
      <<"   --gzip|-z: Tell the program that the input file is gziped, the output will be gziped too\n"
      <<"   --help|-h: Show this help\n"
      <<"   --version|-v: Show version information\n";
}

int main(int argc, char* argv[]) {
  int c;
  int option_index=0;

  string phrases_file="";
  string at_file="";
  string atx_file="";
  string bil_dic="";
  bool use_zlib=false;
  bool equalcat=false;
  bool noword4word=false;

  cerr<<"Command line: ";
  for(int i=0; i<argc; i++)
    cerr<<argv[i]<<" ";
  cerr<<"\n\n";

  cerr<<"LOCALE: "<<setlocale(LC_ALL,"")<<"\n";


  while (true) {
    static struct option long_options[] =
      {
	{"atx",     required_argument,  0, 'x'},
	{"bil",     required_argument,  0, 'b'},
	{"input",   required_argument,  0, 'i'},
	{"output",  required_argument,  0, 'o'},
	{"equalcat",      no_argument,  0, 'e'},
        {"noword4word",   no_argument,  0, 'w'},
	{"gzip",          no_argument,  0, 'z'},
	{"help",          no_argument,  0, 'h'},
	{"version",       no_argument,  0, 'v'},
	{0, 0, 0, 0}
      };

    c=getopt_long(argc, argv, "x:b:i:o:ewzhv",long_options, &option_index);
    if (c==-1)
      break;
      
    switch (c) {
    case 'x': 
      atx_file=optarg;
      break;
    case 'b':
      bil_dic=optarg;
      break;
    case 'i':
      phrases_file=optarg;
      break;
    case 'o':
      at_file=optarg;
      break;
    case 'e':
      equalcat=true;
      break;
    case 'w':
      noword4word=true;
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
	  <<"   Copyright (C) 2006-2007 Felipe Sánchez-Martínez\n\n"
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

  if (atx_file=="") {
    cerr<<"Error: No alignment-template extractor's configuration file was given. You need to provide it with the --atx option\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  if (bil_dic=="") {
    cerr<<"Error: No bilingual dictionary was given. You need to provide it with the --bil option\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  if (phrases_file=="") {
    cerr<<"Error: No input file was given. You need to provide it with the --input option\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  if (at_file=="") {
    cerr<<"Error: No output file was given. You need to provide it with the --output option\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }
  
  cerr<<"Alignment template extractor's configuration file: '"<<atx_file<<"'\n";
  cerr<<"Bilingual dictionary to be used to learn restrictions: '"<<bil_dic<<"'\n";
  cerr<<"Bilingual phrases will be read from file '"<<phrases_file<<"'\n";
  cerr<<"Alignment templates will be writen to file '"<<at_file<<"'\n\n";

  ATXReader atx_reader;
  atx_reader.read(atx_file);

  cerr<<"Source language: "<<atx_reader.get_source_language()<<"\n";
  cerr<<"Target language: "<<atx_reader.get_target_language()<<"\n\n";

  cerr<<"Will alignment templates be discarded if aligned open words differ at the first tag? "<<equalcat<<"\n";
  cerr<<"Will alignment templates be discarded if are equivalent to word-for-word translation? "<<noword4word<<"\n\n";

  AlignmentTemplate::set_lexicalized_words(atx_reader.get_source_lexicalized_words(),
					   atx_reader.get_target_lexicalized_words());


  FILE *fbil_dic=NULL;
  fbil_dic=fopen(bil_dic.c_str(), "r");
  if (!fbil_dic) {
    cerr<<"Error: Cannot open bilingual dictionary file '"<<bil_dic<<"\n";
    exit(EXIT_FAILURE);
  }
  FSTProcessor fstp;
  fstp.load(fbil_dic);
  fstp.initBiltrans();
  fclose(fbil_dic);

  istream *fbil;
  if (use_zlib) {
    fbil = new gzifstream(phrases_file.c_str());
  }  else {
    fbil = new ifstream(phrases_file.c_str());
  }

  if (fbil->fail()) {
    cerr<<"Error: Cannot open input file '"<<phrases_file<<"'\n";
    delete fbil;
    exit(EXIT_FAILURE);
  }

  ostream *fout;
  if(use_zlib) {
    fout = new gzofstream(at_file.c_str());
  } else {
    fout = new ofstream(at_file.c_str());
  }

  if (fout->fail()) {
    cerr<<"Error: Cannot open output file '"<<at_file<<"'\n";
    delete fbil;
    delete fout;
    exit(EXIT_FAILURE);
  }

  time_t start_time, end_time;

  start_time=time(NULL);
  cerr<<"Alignment templates extraction started at: "<<ctime(&start_time);

  string onealg;

  double nbilph=0.0;
  double ndiscarded_bilph=0.0;
  double ndiscarded_invalid_alignment=0.0;
  double ndiscarded_wrong_open_words=0.0;
  double ndiscarded_not_reproducible=0.0;
  double ndiscarded_not_equal_cat=0.0;
  double ndiscarded_equivalent_word4word=0.0;

  while (!fbil->eof()) {
    getline(*fbil,onealg);
    if(onealg.length()>0) {
      nbilph+=1.0;
      Alignment bil_phrase(onealg);

      if (bil_phrase.all_end_words_aligned()) {
	AlignmentTemplate at=AlignmentTemplate::xtract_alignment_template(bil_phrase, fstp);

	if (at.is_valid(equalcat, noword4word, fstp, bil_phrase)) {
	  (*fout)<<at<<"\n";
	} else {
	  ndiscarded_bilph+=1.0;
          if (at.invalid_reason() == AlignmentTemplate::INVALID_WRONG_OPEN_WORDS) {
	    ndiscarded_wrong_open_words+=1.0;
	    cerr<<"Warning: AT discarded due to wrong alignments: ";
	    cerr<<bil_phrase.to_string()<<"\n";
            cerr<<at<<"\n";
	  } else if (at.invalid_reason() == AlignmentTemplate::INVALID_NO_OK_TRANSLATIONS)
	    ndiscarded_not_reproducible+=1.0;
	  else if (at.invalid_reason() == AlignmentTemplate::INVALID_DIFFERENT_TRANSLATIONS)
	    ndiscarded_not_reproducible+=1.0;
	  else if (at.invalid_reason() == AlignmentTemplate::INVALID_NO_EQUALCAT)
	    ndiscarded_not_equal_cat+=1.0;
	  else if (at.invalid_reason() == AlignmentTemplate::INVALID_EQUIVALENT_WORD_FOR_WORD)
	    ndiscarded_equivalent_word4word+=1.0;
	}
      } else {
	ndiscarded_bilph+=1.0;
	ndiscarded_invalid_alignment+=1.0;
	cerr<<"Warning: Bilingual phrase discarded due to end words not aligned: ";
	cerr<<bil_phrase.to_string()<<"\n";
      }

      if ((((int)nbilph)%100000)==0)
	cerr<<nbilph<<" bilingual phrases processed\n";
    }
  }

  end_time=time(NULL);
  cerr<<"Alignment templates extraction finished at: "<<ctime(&end_time)<<"\n";
  cerr<<"Alignment templates extraction took "<<difftime(end_time, start_time)<<" seconds\n";

  cerr<<"Number of bilingual phrases read: "<<nbilph<<"\n";
  cerr<<"Number of bilingual phrases discarded: "<<ndiscarded_bilph<<" ";
  cerr<<"("<<(ndiscarded_bilph/nbilph)*100.0<<" %)\n";

  cerr<<"Number of bilingual phrases discarded because (over the number of discarded):\n";
  cerr<<"   alignments not valid: "<<ndiscarded_invalid_alignment<<" ";
  cerr<<"("<<(ndiscarded_invalid_alignment/ndiscarded_bilph)*100.0<<" %)\n";
  cerr<<"   wrong open words alignments: "<<ndiscarded_wrong_open_words<<" ";
  cerr<<"("<<(ndiscarded_wrong_open_words/ndiscarded_bilph)*100.0<<" %)\n";
  cerr<<"   bil. ph. not reproducible: "<<ndiscarded_not_reproducible<<" ";
  cerr<<"("<<(ndiscarded_not_reproducible/ndiscarded_bilph)*100.0<<" %)\n";
  cerr<<"   not equal cat: "<<ndiscarded_not_equal_cat<<" ";
  cerr<<"("<<(ndiscarded_not_equal_cat/ndiscarded_bilph)*100.0<<" %)\n";
  cerr<<"   equivalent to word for word: "<<ndiscarded_equivalent_word4word<<" ";
  cerr<<"("<<(ndiscarded_equivalent_word4word/ndiscarded_bilph)*100.0<<" %)\n";

  double ndiscarded_rest=ndiscarded_bilph-(ndiscarded_invalid_alignment +
                                           ndiscarded_wrong_open_words +
                                           ndiscarded_not_reproducible +
                                           ndiscarded_not_equal_cat +
                                           ndiscarded_equivalent_word4word);

  cerr<<"   rest:"<<ndiscarded_rest<<" ";
  cerr<<"   ("<<(ndiscarded_rest/ndiscarded_bilph)*100.0<<" %)\n";

  delete fbil;
  delete fout;
}
