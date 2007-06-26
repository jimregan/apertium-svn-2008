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
/**
 * Main program of the TL-driven method used to train a HMM-based
 * part-of-speech tagger.  (source file)
 *
 * For a deeper description on how the method works read the paper:
 *
 * Exploring the use of target-language information to train the
 * part-of-speech tagger of machine translation systems.  By Felipe
 * Sánchez-Martínez, Juan Antonio Pérez-Ortiz and Mikel L. Forcada.
 * In Lecture Notes in Computer Science 3230 (Advances in Natural
 * Language Processing, Proceedings of EsTAL - España for Natural
 * Language Processing), p. 137-148, October 20-22, 2004, Alicante,
 * Spain.  
 * © Springer-Verlag Berling Heidelberg 2004
 * http://www.dlsi.ua.es/~fsanchez/pub/pdf/sanchez04b.pdf
 *
 *  @author   Felipe Sánchez-Martínez - fsanchez@dlsi.ua.es
 */

#include "HMM_TL_driven_trainer.H"
#include "Utils.H"
#include "PathsPruner.H"
#include "configure.H"

#include <apertium/TaggerUtils.H>
#include <apertium/TaggerWord.H>

#include <iostream>
#include <getopt.h>
#include <clocale>

#define MODE_UNKNOWN 0 
#define MODE_TRAIN   1

using namespace std;

void help(char *name) {
  cerr<<"USAGE:\n"
      <<name<<" --tsxfile tsxfile --train <n> [--prune <m> <l> <p> <c> --initprob init.prob]"
      <<" --file file --tscript tscript --lscript lscript [--trules transferrules]"
      <<" [--supforms superficialforms] [--genpaths pathsfile]"
      <<" [--translations transfile [--likelihoods likefiles]]"
      <<" [--save <n>] [--norules] [--debug]\n\n"
      <<"ARGUMENTS:\n"
      <<"   --tsxfile|-x: Specify the tagger specification file in XML format\n"
      <<"   --train|-t: Train the HMM-based part-of-speech tagger using\n"
      <<"               information from the target language.\n"
      <<"               Up to <n> words are processed from the training corpus\n"
      <<"   --file|-f:  Used in conjunction with --train to specify the files\n"
      <<"               the method will work with\n"
      <<"               Input files:\n"
      <<"                  file.dic: Full expanded dictionary\n"
      <<"                  file.crp: Training text corpus\n"
      <<"               Output files generated:\n"
      <<"                  file.prob: HMM parameters\n"
      <<"   --tscript|-r: Specify the full path to the translation script\n"
      <<"   --lscript|-l: Specify the full path to the likelihood-evaluation\n"
      <<"               script\n"
      <<"   --trules|-u: Specify the file with the transfer rules used when translating\n"
      <<"               (see xtract_transfer_rules.sh)\n"
      <<"   --save|-s:  Specify after how many words the HMM parameters must\n"
      <<"               be calculated and stored (optional)\n"
      <<"   --norules|-n: Forbidden and enforce rules will not be used to discard\n"
      <<"               disambiguation paths during training (by default those \n"
      <<"               rules are used)\n"      
      <<"   --genpaths|-g: Specify a file in which all disambiguations paths\n"
      <<"               for each segment are written This cause translations not\n"
      <<"               to be performed, for batch training, 1st stage\n"
      <<"  --translations|-a: Specify a file from which all translations of\n"
      <<"               each segment are read.  Used for batch training, 2nd stage\n"
      <<"  --likelihoods|-e: Specify a file from which the likelihood of each\n"
      <<"               translation is read. Used for batch training, 2nd/3rd stage\n"
      <<"   --supforms|-p: Specify a set of superficial forms (separated by '|') that\n"
      <<"               will be tested during the source-language text segmentation \n"
      <<"               to prevent the method from segmenting at those superficial forms\n"
      <<"   --prune|-k: Tell the algorithm that a disambiguation path pruning must be\n"
      <<"               performed. Meaning of the arguments to --prune:\n"
      <<"                 <m> mode of prunning:\n"
      <<"                    1: Consider only those disambiguation paths whose a priori\n"
      <<"                       likelihood is within the <p> mass probability of all the\n"
      <<"                       disambiguation paths\n"
      <<"                 <l> latency: after how many words should the parameters used to\n"
      <<"                    discard disambiguation paths be updated with new ones\n"
      <<"                    if -1, no updated will be performed\n"
      <<"                 <p> mass of probability: used as a threshold to discard \n"
      <<"                    disambiguation paths. It only has sense if pruning mode is 1\n"
      <<"                    Range of possible values: 0 < p <= 1.0\n"
      <<"                 <c> mixing constant function: constant to be used when mixing\n"
      <<"                     new parameters (weigh of the new model).  Range: c > 0 \n"
      <<"   --initprob|-b:  Specify the file (.prob) with the initial parameters to be used\n"
      <<"               when pruning techniques are used\n"
      <<"   --debug|-d: Print debug information while operating\n"
      <<"   --help|-h:  Prints this help message\n"
      <<"   --version|-v: Print version and license information and exits\n\n";
}

int main(int argc, char *argv[]) {
  int mode=MODE_UNKNOWN;

  Utils::debug=false;

  TaggerWord::show_ingnored_string=false;
  TransferRules transfer_rules;

  cerr<<"LOCALE: "<<setlocale(LC_ALL,"")<<"\n";

  int corpus_length=0;
  string filename="";
  string tsxfile="";

  string tscript="";
  string lscript="";
  string trules="";
  string supforms="";
  string initprob="";
  bool use_tags_rules=true;
  int prune_m=-1; //If greater than 0, path pruning techniques will
		  //be used
  int prune_l=-1;
  double prune_p=1.0;
  double mixing_c=-1.0;

  string pathsfile="";
  string transfile="";
  string likefile="";


  int save_after_nwords=0;

  int c;
  int option_index=0;

  //cerr<<PACKAGE_STRING<<"\n";
  cerr<<"Command line: ";
  for(int i=0; i<argc; i++)
    cerr<<argv[i]<<" ";
  cerr<<"\n";

  while (true) {
    static struct option long_options[] =
      {
	{"tsxfile",    required_argument, 0, 'x'},
	{"train",      required_argument, 0, 't'},
	{"file",       required_argument, 0, 'f'},
	{"tscript",    required_argument, 0, 'r'},
	{"lscript",    required_argument, 0, 'l'},
	{"trules",     required_argument, 0, 'u'},
	{"supforms",   required_argument, 0, 'p'},
	{"prune",      required_argument, 0, 'k'},
	{"initprob",   required_argument, 0, 'b'},
	{"genpaths",    required_argument, 0, 'g'},
	{"translations",required_argument, 0, 'a'},
	{"likelihoods", required_argument, 0, 'e'},
	{"save",       required_argument, 0, 's'},
	{"norules",    no_argument,       0, 'n'},
	{"debug",      no_argument,       0, 'd'},
	{"help",       no_argument,       0, 'h'},
	{"version",    no_argument,       0, 'v'},
	{0, 0, 0, 0}
      };

    c=getopt_long(argc, argv, "x:t:f:r:l:u:k:b:g:a:e:s:ndhv",long_options, &option_index);
    if (c==-1)
      break;
      
    switch (c) {
    case 'v':
      cerr<<PACKAGE_STRING<<"\n";
      cerr<<"LICENSE:\n\n"
	  <<"   Copyright (C) 2004-2006 Felipe Sánchez Martínez\n"
	  <<"                 2006 Universitat d'Alacant\n\n"
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
    case 'x':
      tsxfile=optarg;
      break;
    case 'k':
      if (mode==MODE_TRAIN) {
	prune_m=atoi(optarg);
	prune_l=atoi(argv[optind++]);
	prune_p=atof(argv[optind++]);
	mixing_c=atof(argv[optind++]);
	if (prune_m!=1) {
	  cerr<<"Error: Unknown prunnig mode "<<prune_m<<"\n";
	  help(argv[0]);
	  exit(EXIT_FAILURE);
	}
	if ((prune_p<=0) || (prune_p>1.0)) {
	  cerr<<"Error: mass of probability parameter given to --prune option must be grater than 0.0 and less or equal to 1.0\n";
	  help(argv[0]);
	  exit(EXIT_FAILURE);
	}
	if (mixing_c<=0) {
	  cerr<<"Error: Mixing constant c given to --prune must be grater than 0.0\n";
	  help(argv[0]);
	  exit(EXIT_FAILURE);
	}
      } else {
	cerr<<"Error: --prune argument can only be used in conjunction with --train\n";
	help(argv[0]);
	exit(EXIT_FAILURE);
      }
      break;
    case 'b': 
      initprob=optarg;
      break;
    case 't':  //Training
      corpus_length = atoi(optarg);
      if(corpus_length<=0) {
	cerr<<"Error: mandatory --train argument <n> must be a positive integer\n";
	help(argv[0]);
	exit(EXIT_FAILURE);
      }
      mode=MODE_TRAIN;
      break;
    case 'f':
      filename = optarg;
      break;
    case 'r':
      tscript = optarg;
      break;
    case 'l':
      lscript = optarg;
      break;
    case 'u':
      trules = optarg;
      break;
    case 's':
      save_after_nwords=atoi(optarg);
      break;
    case 'g':
      pathsfile=optarg;
      break;
    case 'a':
      transfile=optarg;
      break;
    case 'e':
      likefile=optarg;
      break;
    case 'n':
      use_tags_rules=false;
      break;
    case 'p': {
      supforms=optarg;
      break;
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

  if(tsxfile=="") {
    cerr<<"Error: No tagger specification file was given, Use --tsxfile argument to provide that file\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  if (mode==MODE_UNKNOWN) {
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  HMM_TL_driven_trainer hmm_trainer(tsxfile, &transfer_rules);
  hmm_trainer.set_use_tags_rules(use_tags_rules);

  Utils::translation_script=tscript;
  Utils::likelihood_script=lscript;

  PathsPruner::mode=prune_m;
  PathsPruner::latency=prune_l;
  PathsPruner::probmass=prune_p;

  if (mode==MODE_TRAIN) {
    if (filename=="") {
      cerr<<"Error: When using --train a file must be provided through the --file option\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if (tscript=="") {
      cerr<<"Error: When using --train a translation script must be given through the --tscript option\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if (lscript=="") {
      cerr<<"Error: When using --train a likelihood-evaluation script must be provided through the --lscript option\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
    if ((prune_m>0) && (initprob=="")) {
      cerr<<"Error: When using --train <n> --prune <k> initial parameters file must be provided through the --initprob option\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  if (trules!="") {
    cerr<<"Reading transfer rules from file '"<<trules<<"' "<<flush;
    transfer_rules.read_rules_from_file(trules);
    cerr<<"done.\n";
  }
  transfer_rules.compile_regular_expressions();

  if (supforms!="") {
    cerr<<"Reading superficial forms to take into account when segmenting source-language text ... ";
    transfer_rules.set_superficial_forms(supforms);
    cerr<<"done.";
  }

  FILE *fdic, *fcrp, *fprob;

  ofstream fpaths;
  ifstream ftrans, flike;

  if (mode==MODE_TRAIN) {
    if (prune_m>0) {
      fprob = fopen(initprob.c_str(), "r");
      if (!fprob) file_name_error(initprob);
      cerr<<"Reading apertium-tagger parameters from file '"<<initprob<<"'\n";
      hmm_trainer.read_parameters(fprob);
      fclose(fprob);
    } else {
      fdic = fopen((filename+".dic").c_str(), "r");
      if (!fdic) file_name_error(filename+".dic");
      hmm_trainer.read_dictionary(fdic);
      fclose(fdic);
    }

    fcrp = fopen((filename+".crp").c_str(), "r");
    if (!fcrp) file_name_error(filename+".crp");

    fprob = fopen((filename+".prob").c_str(), "w");
    if (!fprob) file_name_error(filename+".prob");
  }

  if (mode==MODE_TRAIN) {
    if (prune_m<=0)
      hmm_trainer.train(fcrp, corpus_length, save_after_nwords, filename, fpaths, ftrans, flike);
    else
      hmm_trainer.train_pruning(fcrp, corpus_length, save_after_nwords, filename, mixing_c, fpaths, ftrans, flike);
  } 

  if (mode==MODE_TRAIN) {
    fclose(fcrp);

    hmm_trainer.write_parameters(fprob);
    fclose(fprob);
  }

  exit(EXIT_SUCCESS);
}
