/*
 * Copyright (C) 2006 Universitat d'Alacant / Universidad de Alicante
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

#include <lttoolbox/FSTProcessor.H>

#include <apertium/LexTor.H>
#include <apertium/LexTorWord.H>
#include <apertium/LexTorData.H>
#include <clocale>
#include <cstdlib>

#define MODE_TRAINWRD 0
#define MODE_TRAINLCH 1
#define MODE_LEXTOR 2
#define MODE_LEXTORTL 3

using namespace std;


void help(char *name) {
  cerr<<"USAGE:\n";
  cerr<<name<<" --trainwrd stopwords words n left right corpus model [--weightexp w]\nOR\n";
  cerr<<name<<" --trainlch stopwords lexchoices n left right corpus wordmodel dic bildic model [--weightexp w]\nOR\n";
  cerr<<name<<" --lextor model dic left right [--debug] [--weightexp w]\n\n";
  //cerr<<name<<" --lextortl stopwords words tlmodel dic bildic left right [--debug] [--weightexp w]\n\n"; 
  cerr<<"ARGUMENTS: \n"
      <<"   --trainwrd|-t: Train word co-occurrences model.\n"
      <<"   Required parameters:\n"
      <<"      stopwords: file containing a list of stop words. Stop words are ignored\n"
      <<"      words: file containing a list of words. For each word a co-occurrence model is built\n"
      <<"      n: number of words per co-occurrence model (for each model, the n most frequent words)\n"
      <<"      left: left-side context to take into account (number of words)\n"
      <<"      right: right-side context to take into account (number of words)\n"
      <<"      corpus: file containing the training corpus\n"
      <<"      model: output file on which the co-occurrence models are saved\n\n"

      <<"   --trainlch|-r: Train lexical choices co-occurrences model using a target-language co-occurrence model.\n"
      <<"   Required parameters:\n"
      <<"      stopwords: file containing a list of stop words. Stop words are ignored\n"
      <<"      lexchoices: file containing a list of lexical choices. For each lexical choice a co-occurrence model is built\n"
      <<"      n: number of words per co-occurrence model (for each model, the n most frequent words)\n"
      <<"      left: left-side context to take into account (number of words)\n"
      <<"      right: right-side context to take into account (number of words)\n"
      <<"      corpus: file containing the training corpus\n"
      <<"      wordmodel: target-language word co-occurrence model (previously trained by means of the --trainwrd option)\n"
      <<"      dic: lexical-selection dictionary (binary format)\n"
      <<"      bildic: bilingual dictionary (binary format)\n"
      <<"      model: output file on which the co-occurrence models are saved\n\n"

      <<"   --lextor|-l: Perform the lexical selection on the input stream.\n"
      <<"   Required parameters:\n"
      <<"      model: file containing the model to be used for the lexical selection\n"
      <<"      dic: lexical-selection dictionary (binary format)\n"
      <<"      left: left-side context to take into account (number of words)\n"
      <<"      right: right-side context to take into account (number of words)\n\n"

    //      <<"   --lextortl|-e: Perform the lexical selection on the input stream by using a tl model.\n"
    //      <<"   Required parameters:\n"
    //      <<"      stopwords: file containing a list of stop words in the source language. Stop words are ignored\n"
    //      <<"      words: file containing the list of polysemous words in the source language\n"
    //      <<"      tlmodel: file containing the target-language model to be used for the lexical selection\n"
    //      <<"      dic: lexical-selection dictionary (binary format)\n"
    //      <<"      bildic: bilingual dictionary (binary format)\n"
    //      <<"      left: left-side context to take into account (number of words)\n"
    //      <<"      right: right-side context to take into account (number of words)\n\n"

      <<"   --weightexp|-w: Specify a weight value to change the influence of surrounding words while training or\n"
      <<"     performing the lexica selection. It must be positive.\n\n"

      <<"   --debug|-d: Show debug information while operating\n"
      <<"   --help|-h: Show this help\n"
      <<"   --version|-v: Show version information\n\n";
  cerr<<"Reads from standard input and writes to standard output\n";
}

int main(int argc, char* argv[]) {
  int c;
  int option_index=0;
  int mode=-1;

  //Parameters for the "trainwrd" or the "trainlch" mode option
  string stopwords_file="";
  string words_file="";
  string corpus_file="";
  int nwords_model=0;
  int nwords_left=-1;
  int nwords_right=-1;

  string model_file="";

  string lexchoices_file="";
  string wordmodel_file="";
  string bildic_file="";

  //Parameters for the "lextor" option
  string dic_file="";

  double weight_exponent=0.0;

  LexTor::debug=false;

  //cerr<<"LOCALE: "<<setlocale(LC_ALL,"")<<"\n";

  while (true) {
    static struct option long_options[] =
      {
	{"trainwrd",  required_argument, 0, 't'},
	{"trainlch",  required_argument, 0, 'r'},
	{"lextor",    required_argument, 0, 'l'},
	//	{"lextortl",  required_argument, 0, 'e'},
        {"weightexp", required_argument, 0, 'w'},
	{"debug",        no_argument,    0, 'd'},
	{"help",         no_argument,    0, 'h'},
	{"version",      no_argument,    0, 'v'},
	{0, 0, 0, 0}
      };

    c=getopt_long(argc, argv, "t:r:l:e:w:dhv",long_options, &option_index);
    if (c==-1)
      break;
      
    switch (c) {
    case 't':
      mode=MODE_TRAINWRD;
      stopwords_file=optarg;
      words_file=argv[optind++];
      nwords_model=atoi(argv[optind++]);
      nwords_left=atoi(argv[optind++]);
      nwords_right=atoi(argv[optind++]);
      corpus_file=argv[optind++];
      model_file=argv[optind++];
      break;
    case 'r':
      //--trainlch stopwords lexchoices n left right corpus wordmodel dic bildic model
      mode=MODE_TRAINLCH;
      stopwords_file=optarg;
      lexchoices_file=argv[optind++];
      nwords_model=atoi(argv[optind++]);
      nwords_left=atoi(argv[optind++]);
      nwords_right=atoi(argv[optind++]);
      corpus_file=argv[optind++];
      wordmodel_file=argv[optind++];
      dic_file=argv[optind++];
      bildic_file=argv[optind++];
      model_file=argv[optind++];
      break;
    case 'l':
      mode=MODE_LEXTOR;
      model_file=optarg;
      dic_file=argv[optind++];
      nwords_left=atoi(argv[optind++]);
      nwords_right=atoi(argv[optind++]);
      break;
    case 'e':
      mode=MODE_LEXTORTL;
      stopwords_file=optarg;
      words_file=argv[optind++];
      model_file=argv[optind++];
      dic_file=argv[optind++];
      bildic_file=argv[optind++];
      nwords_left=atoi(argv[optind++]);
      nwords_right=atoi(argv[optind++]);
      break;
    case 'w':
      weight_exponent=atof(optarg);
      break;
    case 'd':
      LexTor::debug=true;
      break;
    case 'h': 
      help(argv[0]);
      exit(EXIT_SUCCESS);
      break;
    case 'v':
      cerr<<"APERTIUM"<<"\n"; //"APERTIUM" era PACKAGE_STRING
      cerr<<"LICENSE:\n\n"
	  <<"   Copyright (C) 2006 Universitat d'Alacant / Universidad de Alicante\n\n"
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

  if (weight_exponent<0) {
    cerr<<"Error: the weight exponent provided is less than zero. It must be positive\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  //When reading from the input stream '*all* characters must be
  //processed, including ' ','\n', .....
  cin.unsetf(ios::skipws);

  if (mode==MODE_TRAINWRD) {
    if(stopwords_file=="") {
      cerr<<"Error: no stopwords file was given\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if (words_file=="") {
      cerr<<"Error: no words file was given\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if (nwords_model==0) {
      cerr<<"Error: the number of word per co-occurrence model must be grater than 0\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if (nwords_left<0) {
      cerr<<"Error: no left-side context number of words was provided\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if (nwords_right<0) {
      cerr<<"Error: no rigth-side context number of words was provided\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if (corpus_file=="") {
      cerr<<"Error: No training corpus file was given\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if (model_file=="") {
      cerr<<"Error: No output file to save the co-occurrence models was given\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }

    ifstream fstopwords, fwords, fcorpus;

    fstopwords.open(stopwords_file.c_str(), ios::in);
    if (fstopwords.fail()) {
      cerr<<"Error: Cannot open file '"<<stopwords_file<<"'\n";
      exit(EXIT_FAILURE);
    }

    fwords.open(words_file.c_str(), ios::in);
    if (fwords.fail()) {
      cerr<<"Error: Cannot open file '"<<words_file<<"'\n";
      exit(EXIT_FAILURE);
    }

    fcorpus.open(corpus_file.c_str(), ios::in);
    if(fcorpus.fail()) {
      cerr<<"Error: Cannot open file '"<<corpus_file<<"'\n";
      exit(EXIT_FAILURE);
    }

    ofstream fmodel;
    fmodel.open(model_file.c_str(), ios::out | ios::trunc);
    if(fmodel.fail()) {
      cerr<<"Error: Cannot open file '"<<model_file<<"'\n";
      exit(EXIT_FAILURE);
    }

    LexTorData lextor_data;

    lextor_data.read_stopwords(fstopwords);
    lextor_data.read_words(fwords);
    lextor_data.set_nwords_per_set(nwords_model);

    fstopwords.close();
    fwords.close();

    LexTor lexical_selector;
    lexical_selector.set_lextor_data(&lextor_data);

    //Whe reading from the input corpus '*all* characters must be
    //processed, including ' ','\n', .....
    fcorpus.unsetf(ios::skipws);

    //Train
    lexical_selector.trainwrd(fcorpus, nwords_left, nwords_right, weight_exponent);
    fcorpus.close();

    //Write parameters
    lextor_data.write(fmodel);
    fmodel.close();
  } 

  else if (mode==MODE_TRAINLCH) {
    if(stopwords_file=="") {
      cerr<<"Error: no stopwords file was given\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if (lexchoices_file=="") {
      cerr<<"Error: no lexical choices file was given\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if (nwords_model==0) {
      cerr<<"Error: the number of word per co-occurrence model must be greater than 0\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if (nwords_left<0) {
      cerr<<"Error: no left-side context number of words was provided\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if (nwords_right<0) {
      cerr<<"Error: no rigth-side context number of words was provided\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if (corpus_file=="") {
      cerr<<"Error: No training corpus file was given\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if(wordmodel_file=="") {
      cerr<<"Error: No target-language word co-occurrence model was provided\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if (dic_file=="") {
      cerr<<"Error: No lexical-selection dictionary was provided\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if (bildic_file=="") {
      cerr<<"Error: No bilingual dictionary was provided\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if (model_file=="") {
      cerr<<"Error: No output file to save the co-occurrence models was given\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }

    ifstream fstopwords, flexchoices, fcorpus, fwordmodel;
    FILE *fdic=NULL, *fbildic=NULL;

    fstopwords.open(stopwords_file.c_str(), ios::in);
    if (fstopwords.fail()) {
      cerr<<"Error: Cannot open file '"<<stopwords_file<<"'\n";
      exit(EXIT_FAILURE);
    }

    flexchoices.open(lexchoices_file.c_str(), ios::in);
    if (flexchoices.fail()) {
      cerr<<"Error: Cannot open file '"<<lexchoices_file<<"'\n";
      exit(EXIT_FAILURE);
    }

    fcorpus.open(corpus_file.c_str(), ios::in);
    if(fcorpus.fail()) {
      cerr<<"Error: Cannot open file '"<<corpus_file<<"'\n";
      exit(EXIT_FAILURE);
    }

    fwordmodel.open(wordmodel_file.c_str(), ios::in);
    if(fwordmodel.fail()) {
      cerr<<"Error: Cannot open file '"<<wordmodel_file<<"'\n";
      exit(EXIT_FAILURE);
    }

    fdic=fopen(dic_file.c_str(), "r");
    if(!fdic) {
      cerr<<"Error: Cannot open file '"<<dic_file<<"'\n";
      exit(EXIT_FAILURE);
    }

    fbildic=fopen(bildic_file.c_str(), "r");
    if(!fbildic) {
      cerr<<"Error: Cannot open file '"<<bildic_file<<"'\n";
      exit(EXIT_FAILURE);
    }

    ofstream fmodel;
    fmodel.open(model_file.c_str(), ios::out | ios::trunc);
    if(fmodel.fail()) {
      cerr<<"Error: Cannot open file '"<<model_file<<"'\n";
      exit(EXIT_FAILURE);
    }

    LexTorData lextor_data;

    lextor_data.read_stopwords(fstopwords);
    lextor_data.read_words(flexchoices);
    lextor_data.set_nwords_per_set(nwords_model);

    fstopwords.close();
    flexchoices.close();

    LexTor lexical_selector;
    lexical_selector.set_lextor_data(&lextor_data);

    LexTorData wordmodel;
    wordmodel.read(fwordmodel);
    fwordmodel.close();

    FSTProcessor fstpdic;
    fstpdic.load(fdic);
    fstpdic.initBiltrans();
    fclose(fdic);

    lextor_data.read_lexical_choices(fstpdic);

    FSTProcessor fstpbildic;
    fstpbildic.load(fbildic);
    fstpbildic.initBiltrans();
    fclose(fbildic);


    //Whe reading from the input corpus '*all* characters must be
    //processed, including ' ','\n', .....
    fcorpus.unsetf(ios::skipws);

    //Train
    lexical_selector.trainlch(fcorpus, nwords_left, nwords_right, wordmodel, fstpdic, fstpbildic, weight_exponent);

    fcorpus.close();

    //Write parameters
    lextor_data.write(fmodel);
    fmodel.close();
  }

  else if (mode==MODE_LEXTOR) {
    if(model_file=="") {
      cerr<<"Error: no model file was given\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if (dic_file=="") {
      cerr<<"Error: no dic file was given\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if (nwords_left<0) {
      cerr<<"Error: no left-side context number of words was provided\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if (nwords_right<0) {
      cerr<<"Error: no rigth-side context number of words was provided\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }

    FILE *fdic=NULL;
    fdic=fopen(dic_file.c_str(), "r");
    if (!fdic) {
      cerr<<"Error: Cannot open dictionary file '"<<dic_file<<"' for lexical selection\n";
      exit(EXIT_FAILURE);
    }
    FSTProcessor fstp;
    fstp.load(fdic);
    fstp.initBiltrans();
    fclose(fdic);

    ifstream fmodel;
    fmodel.open(model_file.c_str(), ios::in);
    if(fmodel.fail()) {
      cerr<<"Error: Cannot open file '"<<model_file<<"'\n";
      exit(EXIT_FAILURE);
    }

    //Whe reading from the input stream '*all* characters must be
    //processed, including ' ','\n', .....
    cin.unsetf(ios::skipws);

    LexTorData lextor_model;
    lextor_model.read(fmodel);
    fmodel.close();

    LexTor lexical_selector;
    lexical_selector.set_lextor_data(&lextor_model);

    lexical_selector.lexical_selector(cin, fstp, nwords_left, nwords_right, weight_exponent);
  } 

  else if (mode==MODE_LEXTORTL) {
    if(stopwords_file=="") {
      cerr<<"Error: no stopwords file was given\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if(words_file=="") {
      cerr<<"Error: no words file was given\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if(model_file=="") {
      cerr<<"Error: no target-language model file was given\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if (dic_file=="") {
      cerr<<"Error: No lexical-selection dictionary was provided\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if (bildic_file=="") {
      cerr<<"Error: No bilingual dictionary was provided\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if (nwords_left<0) {
      cerr<<"Error: no left-side context number of words was provided\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if (nwords_right<0) {
      cerr<<"Error: no rigth-side context number of words was provided\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }

    ifstream fstopwords, fwords, fmodel;
    FILE *fdic=NULL, *fbildic=NULL;

    fstopwords.open(stopwords_file.c_str(), ios::in);
    if (fstopwords.fail()) {
      cerr<<"Error: Cannot open file '"<<stopwords_file<<"'\n";
      exit(EXIT_FAILURE);
    }

    fwords.open(words_file.c_str(), ios::in);
    if (fwords.fail()) {
      cerr<<"Error: Cannot open file '"<<words_file<<"'\n";
      exit(EXIT_FAILURE);
    }

    fmodel.open(model_file.c_str(), ios::in);
    if(fmodel.fail()) {
      cerr<<"Error: Cannot open file '"<<model_file<<"'\n";
      exit(EXIT_FAILURE);
    }

    fdic=fopen(dic_file.c_str(), "r");
    if(!fdic) {
      cerr<<"Error: Cannot open file '"<<dic_file<<"'\n";
      exit(EXIT_FAILURE);
    }

    fbildic=fopen(bildic_file.c_str(), "r");
    if(!fbildic) {
      cerr<<"Error: Cannot open file '"<<bildic_file<<"'\n";
      exit(EXIT_FAILURE);
    }

    LexTorData lextor_data;

    lextor_data.read_stopwords(fstopwords);
    fstopwords.close();

    lextor_data.read_words(fwords);
    fwords.close();

    LexTor lexical_selector;
    lexical_selector.set_lextor_data(&lextor_data);

    LexTorData tlmodel;
    tlmodel.read(fmodel);
    fmodel.close();

    FSTProcessor fstpdic;
    fstpdic.load(fdic);
    fstpdic.initBiltrans();
    fclose(fdic);

    FSTProcessor fstpbildic;
    fstpbildic.load(fbildic);
    fstpbildic.initBiltrans();
    fclose(fbildic);


    lextor_data.read_lexical_choices(fstpdic);

    //Whe reading from the input stream '*all* characters must be
    //processed, including ' ','\n', .....
    cin.unsetf(ios::skipws);


    lexical_selector.set_tlmodel(&tlmodel);
    lexical_selector.set_bildic(&fstpbildic);

    lexical_selector.lexical_selector(cin, fstpdic, nwords_left, nwords_right, weight_exponent);
  } 

  else {
    cerr<<"Error: No operation mode was provided\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }
}
