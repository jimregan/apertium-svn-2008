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

#include <apertium/HMM.H>
#include <apertium/MorphoStream.H>
#include <apertium/TTag.H>
#include <apertium/TaggerWord.H>
#include <apertium/Collection.H>
#include <apertium/TaggerData.H>
#include <apertium/TSXReader.H>
#include "configure.H"
#include "SmoothUtils.H"

#define ZERO 1e-10

#define MODE_UNKNOWN 0
#define MODE_TRAIN   1
#define MODE_RETRAIN 2

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

void apply_rules() {
  bool found;

  for(size_t i=0; i<tagger_data.getForbidRules().size(); i++) {
    tagger_data.getA()[tagger_data.getForbidRules()[i].tagi][tagger_data.getForbidRules()[i].tagj] = ZERO;
  }

  for(size_t i=0; i<tagger_data.getEnforceRules().size(); i++) {
    for(int j=0; j<tagger_data.getN(); j++) {
      found = false;
      for (size_t j2=0; j2<tagger_data.getEnforceRules()[i].tagsj.size(); j2++) {
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

void kupiec (FILE *is, int corpus_length, string savecountsfile) {
  int i, j, k, k1, k2, nw=0;
  map<int, double> classes_ocurrences; // M
  map<int, map<int, double> > classes_pair_ocurrences; // MxM

  map<int, double> tags_count; // N
  map<int, double> tags_count_for_emis; // N
  map<int, map<int, double> > tags_pairs; //NxN
  map<int, map<int, double> > emis; //NxM

  MorphoStream lexmorfo(is, true, &tagger_data);

  TaggerWord *word=NULL;

  for(k1=0; k1<tagger_data.getM(); k1++) {
    classes_ocurrences[k1]=0; 
    for (k2=0; k2<tagger_data.getM(); k2++)
      classes_pair_ocurrences[k1][k2]=0;
  }

  set<TTag> tags;
  tags.clear();
  tags.insert(eos);  
  k1=tagger_data.getOutput()[tags]; //The first tag (ambiguity class) seen is the end-of-sentence
  classes_ocurrences[k1]++;
  
  cerr<<"Kupiec initialization of transition and emission probabilities ..."<<flush;

  //We count for each ambiguity class the number of ocurrences
  word = lexmorfo.get_next_word();
  while((word)) {
    if (++nw%10000==0) cerr<<'.'<<flush; 
    
    tags=word->get_tags();

    if (tags.size()==0) { //This is an unknown word
      tags = tagger_data.getOpenClass();
    }
    else if (tagger_data.getOutput().has_not(tags)) { 
      string errors;
      errors = "A new ambiguity class was found. I cannot continue.\n";
      errors+= "Word '"+word->get_superficial_form()+"' not found in the dictionary.\n";
      errors+= "New ambiguity class: "+word->get_string_tags()+"\n";
      errors+= "Take a look at the dictionary and at the training corpus. Then, retrain.";      
      cerr<<"Error: "<<errors<<"\n";
      exit(EXIT_FAILURE);
    }    

    k2=tagger_data.getOutput()[tags];

    classes_ocurrences[k2]++;
    classes_pair_ocurrences[k1][k2]++;  //k1 followed by k2
    delete word;
    word=lexmorfo.get_next_word();

    k1=k2;

    if((corpus_length>0)&&(nw>=corpus_length)&&(tags.size()==1)&&((*(tags.begin()))==eos)) {
      //En of training
      cerr<<nw<<" ";
      break;
    }
  }  
  
  //Estimation of the number of time each tags occurs in the training text
  for(i=0; i<tagger_data.getN(); i++) {  
    tags_count_for_emis[i]=0;
    for(k=0; k<tagger_data.getM();  k++) { 
      if(tagger_data.getOutput()[k].find(i)!=tagger_data.getOutput()[k].end()) {
        tags_count_for_emis[i] += classes_ocurrences[k]/((double)tagger_data.getOutput()[k].size());
      }
    }
  }
      
  //Estimation of the number of times each tag pair occurs
  for(i=0; i<tagger_data.getN(); i++)
    for(j=0; j<tagger_data.getN(); j++)
      tags_pairs[i][j]=0;

  set<TTag> tags1, tags2;
  set<TTag>::iterator itag1, itag2;
  for(k1=0; k1<tagger_data.getM(); k1++) {
    tags1=tagger_data.getOutput()[k1];
    for(k2=0; k2<tagger_data.getM(); k2++) {
      tags2=tagger_data.getOutput()[k2];
      double nocurrences=classes_pair_ocurrences[k1][k2]/((double)(tags1.size()*tags2.size()));
      for (itag1=tags1.begin(); itag1!=tags1.end(); itag1++) {
	if (((*itag1)<0)||((*itag1)>=tagger_data.getN())) 
	  cerr<<"Error: Tag "<<*itag1<<" out of range\n";
        for (itag2=tags2.begin(); itag2!=tags2.end(); itag2++) {
          if (((*itag2)<0)||((*itag2)>=tagger_data.getN())) 
	    cerr<<"Error: Tag "<<*itag2<<" out of range\n";
          tags_pairs[*itag1][*itag2]+=nocurrences;
	  tags_count[*itag1]+=nocurrences;
	}
      }
    }
  }

  //Estimation of the number of times each ambiguity class is emitted
  //from each tag
  for(i=0; i<tagger_data.getN(); i++) {
    for(k=0; k<tagger_data.getM(); k++)  {
      if (tagger_data.getOutput()[k].find(i)!=tagger_data.getOutput()[k].end()) {
	emis[i][k]=classes_ocurrences[k]/((double)tagger_data.getOutput()[k].size());
      }
    }
  }

  if (savecountsfile!="") {
    cerr<<"Saving counts to file '"<<savecountsfile<<"' ... "<<flush;
    SmoothUtils::save_counts(tagger_data, savecountsfile, tags_pairs, emis, tags_count, classes_ocurrences, tags_count_for_emis);
    cerr<<"done.\n";
  }

  SmoothUtils::calculate_smoothed_parameters(tagger_data, tags_count, tags_pairs, classes_ocurrences, emis, tags_count_for_emis, nw);
  cerr<<" done\n";
}

void baum_welch(FILE *ftxt, int corpus_length, string savecountsfile) {
  int i, j, k, t, len, nw = 0;
  TaggerWord *word=NULL;
  TTag tag; 
  set<TTag> tags, pretags;
  set<TTag>::iterator itag, jtag;
  map <int, double> gamma;
  map <int, double>::iterator jt, kt;
  map < int, map <int, double> > alpha, beta, xsi, phi;
  map < int, map <int, double> >::iterator it;
  double prob, loli;              
  vector < set<TTag> > pending;
  
  map<int, double> ambclass_count;

  // alpha => forward probabilities
  // beta  => backward probabilities

  MorphoStream morpho_stream(ftxt, true, &tagger_data);

  loli = 0;
  tag = eos;
  tags.clear();
  tags.insert(tag);
  pending.push_back(tags);

  alpha[0].clear();      
  alpha[0][tag] = 1;

  cerr<<"Performing Baum-Welch training ..."<<flush;

  word = morpho_stream.get_next_word();

  while (word) {   
    if (++nw%10000==0) cerr<<'.'<<flush;
           
    pretags = pending.back();

    tags = word->get_tags();    
    
    if (tags.size()==0) { // This is an unknown word
      tags = tagger_data.getOpenClass();
    }

    if (tagger_data.getOutput().has_not(tags)) {
      string errors;
      errors = "A new ambiguity class was found. I cannot continue.\n";
      errors+= "Word '"+word->get_superficial_form()+"' not found in the dictionary.\n";
      errors+= "New ambiguity class: "+word->get_string_tags()+"\n";
      errors+= "Take a look at the dictionary, then retrain.";
      cerr<<"Error: "<<errors<<"\n";
      exit(EXIT_FAILURE);
    }
    
    k = tagger_data.getOutput()[tags];
    ambclass_count[k]++;

    len = pending.size();
    alpha[len].clear();     
      
    //Forward probabilities
    for (itag=tags.begin(); itag!=tags.end(); itag++) {
      i=*itag;
      for (jtag=pretags.begin(); jtag!=pretags.end(); jtag++) {
	j=*jtag;
	alpha[len][i] += alpha[len-1][j]*tagger_data.getA()[j][i]*tagger_data.getB()[i][k];
      }
    }

    if (tags.size()>1) {
      pending.push_back(tags);
    } else {  // word is unambiguous
      tag = *tags.begin(); 
      beta[0].clear();
      beta[0][tag] = 1;   
      prob = alpha[len][tag];
      loli -= log(prob);  
      
      for (t=0; t<len; t++) {  // loop from T-1 to 0	
	pretags = pending.back();
	pending.pop_back();
	k = tagger_data.getOutput()[tags];
	beta[1-t%2].clear();
	for (itag=tags.begin(); itag!=tags.end(); itag++) {
	  i=*itag;
	  for (jtag=pretags.begin(); jtag!=pretags.end(); jtag++) {
	    j = *jtag;	      
	    beta[1-t%2][j] += tagger_data.getA()[j][i]*tagger_data.getB()[i][k]*beta[t%2][i];
	    xsi[j][i] += alpha[len-t-1][j]*tagger_data.getA()[j][i]*tagger_data.getB()[i][k]*beta[t%2][i]/prob;
	  }
	  double previous_value = gamma[i];
       
	  gamma[i] +=  alpha[len-t][i]*beta[t%2][i]/prob;		       
	  if (isnan(gamma[i])) {
	    cerr<<"NAN(3) gamma["<<i<<"] = "<<gamma[i]<<" alpha["<<len-t<<"]["<<i<<"]= "<<alpha[len-t][i]<<" beta["<<t%2<<"]["<<i<<"] = "<<beta[t%2][i]<<" prob = "<<prob<<" previous gamma: "<<previous_value<<"\n";
	    exit(EXIT_FAILURE);
	  }
	  if (isinf(gamma[i])) {
	    cerr<<"INF(3) gamma["<<i<<"] = "<<gamma[i]<<" alpha["<<len-t<<"]["<<i<<"]= "<<alpha[len-t][i]<<" beta["<<t%2<<"]["<<i<<"] = "<<beta[t%2][i]<<" prob = "<<prob<<" previous gamma: "<<previous_value<<"\n";
	    exit(EXIT_FAILURE);
	  }
	  if (gamma[i]==0) {
	    //cerr<<"ZERO(3) gamma["<<i<<"] = "<<gamma[i]<<" alpha["<<len-t<<"]["<<i<<"]= "<<alpha[len-t][i]<<" beta["<<t%2<<"]["<<i<<"] = "<<beta[t%2][i]<<" prob = "<<prob<<" previous gamma: "<<previous_value<<"\n";
            gamma[i]=DBL_MIN;
	  }
	  phi[i][k] += alpha[len-t][i]*beta[t%2][i]/prob;
	}
	tags=pretags;
      }
	
      tags.clear();
      tags.insert(tag);
      pending.push_back(tags);
      alpha[0].clear();
      alpha[0][tag] = 1;
      

      if((corpus_length>0)&&(nw>=corpus_length)&&(tag==eos)) {
        cerr<<nw<<" ";
	//En of training
	break;
      }
    }
    
    delete word; 
    word = morpho_stream.get_next_word();
  }  
  if ((pending.size()>1) || ((tag!=eos))) 
    cerr<<"Warning: Thee las tag is not the end-of-sentence-tag\n";
  
  /*
  //Clean previous values   
  for(i=0; i<tagger_data.getN(); i++) {
  for(j=0; j<tagger_data.getN(); j++)
  tagger_data.getA()[i][j]=0;
  for(k=0; k<tagger_data.getM(); k++)
  tagger_data.getB()[i][k]=0;
  }
 
  // new parameters
  for (it=xsi.begin(); it!=xsi.end(); it++) {
  i = it->first;
  for (jt=xsi[i].begin(); jt!=xsi[i].end(); jt++) {
  j = jt->first;
  if (xsi[i][j]>0) {        
  tagger_data.getA()[i][j] = xsi[i][j]/gamma[i];
	
  if (isnan(tagger_data.getA()[i][j])) {
  cerr <<"Error: BW - NAN(1) a["<<i<<"]["<<j<<"]="<<tagger_data.getA()[i][j]<<"\txsi["<<i<<"]["<<j<<"]="<<xsi[i][j]<<"\tgamma["<<i<<"]="<<gamma[i]<<"\n";
  exit(EXIT_FAILURE);
  }
  if (isinf(tagger_data.getA()[i][j])) {
  cerr <<"Error: BW - INF(1) a["<<i<<"]["<<j<<"]="<<tagger_data.getA()[i][j]<<"\txsi["<<i<<"]["<<j<<"]="<<xsi[i][j]<<"\tgamma["<<i<<"]="<<gamma[i]<<"\n";
  exit(EXIT_FAILURE);
  }
  if (tagger_data.getA()[i][j]==0) {
  //cerr <<"Error: BW - ZERO(1) a["<<i<<"]["<<j<<"]="<<a[i][j]<<"\txsi["<<i<<"]["<<j<<"]="<<xsi[i][j]<<"\tgamma["<<i<<"]="<<gamma[i]<<"\n";
  //exit(1);
  }
  }
  }
  }
  

  for (it=phi.begin(); it!=phi.end(); it++) {
  i = it->first;
  for (kt=phi[i].begin(); kt!=phi[i].end(); kt++) {
  k = kt->first;
  if (phi[i][k]>0) {
  tagger_data.getB()[i][k] = phi[i][k]/gamma[i];	
        
  if (isnan(tagger_data.getB()[i][k])) {
  cerr <<"Error: BW - NAN(2) b["<<i<<"]["<<k<<"]="<<tagger_data.getB()[i][k]<<"\tphi["<<i<<"]["<<k<<"]="<<phi[i][k]<<"\tgamma["<<i<<"]="<<gamma[i]<<"\n";
  exit(EXIT_FAILURE);
  }
  if (isinf(tagger_data.getB()[i][k])) {
  cerr <<"Error: BW - INF(2) b["<<i<<"]["<<k<<"]="<<tagger_data.getB()[i][k]<<"\tphi["<<i<<"]["<<k<<"]="<<phi[i][k]<<"\tgamma["<<i<<"]="<<gamma[i]<<"\n";
  exit(EXIT_FAILURE);
  }
  if (tagger_data.getB()[i][k]==0) {
  //cerr <<"Error: BW - ZERO(2) b["<<i<<"]["<<k<<"]="<<b[i][k]<<"\tphi["<<i<<"]["<<k<<"]="<<phi[i][k]<<"\tgamma["<<i<<"]="<<gamma[i]<<"\n";
  //exit(1);
  }
  }
  }
  }
  */

  if (savecountsfile!="") {
    cerr<<"Saving counts to file '"<<savecountsfile<<"' ... "<<flush;
    SmoothUtils::save_counts(tagger_data, savecountsfile, xsi, phi, gamma, ambclass_count, gamma);
    cerr<<"done.\n";
  }

  SmoothUtils::calculate_smoothed_parameters(tagger_data, gamma, xsi, ambclass_count, phi, gamma, nw);
  cerr<<" done. Log="<<loli<<"\n";
}

void help(char *name) {
  cerr<<"USAGE:\n";
  cerr<<name<<" --train <n> [--clength <corpus_length>] --tsxfile tsxfile --dicfile file.dic --crpfile file.crp --outfile fileout.prob [--norules] [--savecounts fileout.counts]\nOR\n"
      <<name<<" --retrain <n> [--clength <corpus_length>] --infile filein.prob --crpfile file.crp --outfile fileout.prob [--savecounts fileout.counts]\n\n";

  cerr<<"ARGUMENTS: \n"
      <<"   --train|-t: To train the HMM-based apertium-tagger for <n> Baum-Welch interations\n"
      <<"            Parameters are initialized using the Kupiec method on the same corpus\n"
      <<"   --retrain|-r: Retrain the apertium-tagger for <n> additional Baum-Welch iterations\n"
      <<"   --clength|-l: To specify the length of the corpus to process\n"
      <<"   --tsxfile|-x: To provide the tagger specification file in XML\n"
      <<"   --dicfile|-d: To specify the expanded dictionary that will be used to extract the\n"
      <<"            ambiguity classes\n"
      <<"   --crpfile|-c: To specify the corpus to be used for training\n"
      <<"   --infile|-i: To specify a file containing initial tagger parameters\n"
      <<"   --outfile|-o: To specify the file in which the new parameters will be stored\n"
      <<"   --savecounts|-s: To specify the file in which the collected counts will be stored\n"
      <<"   --norules|-n: Do not use forbid and enforce rules\n";
}


int main(int argc, char* argv[]) {
  string tsxfile="";
  string filedic="";
  string filecrp="";
  string filein="";
  string fileout="";
  string filecounts="";
  int nit=-1;
  int corpus_length=-1;
  int mode=MODE_UNKNOWN;
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
	{"train",      required_argument, 0, 't'},
	{"retrain",    required_argument, 0, 'r'},
	{"clength",    required_argument, 0, 'l'},
	{"tsxfile",    required_argument, 0, 'x'},
	{"dicfile",    required_argument, 0, 'd'},
	{"crpfile",    required_argument, 0, 'c'},
	{"infile",     required_argument, 0, 'i'},
	{"outfile",    required_argument, 0, 'o'},
	{"savecounts", required_argument, 0, 's'},
	{"norules",      no_argument,     0, 'n'},
	{"help",         no_argument,     0, 'h'},
	{"version",      no_argument,     0, 'v'},
	{0, 0, 0, 0}
      };

    c=getopt_long(argc, argv, "t:r:l:x:d:c:i:o:s:nhv",long_options, &option_index);
    if (c==-1)
      break;
      
    switch (c) {
    case 't': 
      mode=MODE_TRAIN;
      nit=atoi(optarg);
      break;
    case 'r': 
      mode=MODE_RETRAIN;
      nit=atoi(optarg);
      break;
    case 'l': 
      corpus_length=atoi(optarg);
      if(corpus_length<=0) {
	cerr<<"Error: corpus length provided with --clength must be a positive integer\n";
	help(argv[0]);
	exit(EXIT_FAILURE);
      }
      break;
    case 'x': 
      tsxfile=optarg;
      break;
    case 'd': 
      filedic=optarg;
      break;
    case 'c': 
      filecrp=optarg;
      break;
    case 'i': 
      filein=optarg;
      break;
    case 'o': 
      fileout=optarg;
      break;
    case 's':
      filecounts=optarg;
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

  //Now we check the command line arguments
  if (mode==MODE_TRAIN) {
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
  } else if (mode==MODE_RETRAIN) {
    if(filein=="") {
      cerr<<"Error: You did not provide a tagger parameter input file. Use --infile to do that\n";
      help(argv[0]);
      exit(EXIT_FAILURE);
    }
  } else {
    cerr<<"Error: you did not provide --train option, neither --retrain. Please provide one of them\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }

  if (filecrp=="") {
    cerr<<"Error: You did not provide a training corpus. Use --crpfile to do that\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }
  if(fileout=="") {
    cerr<<"Error: You did not provide an output file for the tagger parameters. Use --outfile to do that\n";
    help(argv[0]);
    exit(EXIT_FAILURE);
  }


  if (mode==MODE_TRAIN) {
    TSXReader treader;
    treader.read(tsxfile);
    tagger_data=treader.getTaggerData();

    FILE *fdic, *fcrp, *fout;

    HMM hmm(&tagger_data);
    hmm.set_debug(true);
    hmm.set_eos((tagger_data.getTagIndex())["TAG_SENT"]);

    TaggerWord::setArrayTags(tagger_data.getArrayTags());
    eos=(tagger_data.getTagIndex())["TAG_SENT"];

    //NEW
    if (tagger_data.getOpenClass().size()==0) {
      cerr<<"Inserting TAG_kUNDEF in open_class because it was empty\n";
      tagger_data.getOpenClass().insert(tagger_data.getTagIndex()["TAG_kUNDEF"]);
    }
    //

    fdic=fopen(filedic.c_str(), "r");
    check_file(fdic, filedic);
    fcrp=fopen(filecrp.c_str(), "r");
    check_file(fcrp, filecrp);

    cerr<<"Calculating ambiguity classes ... "<<flush;
    hmm.read_dictionary(fdic);
    cerr<<"done.\n";
    fclose(fdic);

    kupiec(fcrp, corpus_length, filecounts);

    if (use_forbid_enforce_rules) {
      cerr<<"Applying forbid and enforce rules ... "<<flush;
      apply_rules();
      cerr<<"done.\n";
    }

    while(nit>0) {
      rewind(fcrp);
      baum_welch(fcrp, corpus_length, filecounts);
      nit--;
    }
    fclose(fcrp);

    fout=fopen(fileout.c_str(), "w");
    check_file(fout, fileout);
    cerr<<"Writing apertium-tagger parameters to file '"<<fileout<<"' ... "<<flush;
    tagger_data.write(fout);
    cerr<<"done.\n";
    fclose(fout);

  } else {
    FILE *fin, *fcrp, *fout;

    fin=fopen(filein.c_str(), "r");
    check_file(fin, filein);
    fcrp=fopen(filecrp.c_str(), "r");
    check_file(fcrp, filecrp);

    cerr<<"Reading initial parameters from file '"<<filein<<"' ... "<<flush;
    tagger_data.read(fin);
    cerr<<"done.\n";

    HMM hmm(&tagger_data);
    hmm.set_debug(true);
    hmm.set_eos((tagger_data.getTagIndex())["TAG_SENT"]);

    TaggerWord::setArrayTags(tagger_data.getArrayTags());
    eos=(tagger_data.getTagIndex())["TAG_SENT"];

    while(nit>0) {
      baum_welch(fcrp, corpus_length, filecounts);
      rewind(fcrp);
      nit--;
    }
    fclose(fcrp);

    fout=fopen(fileout.c_str(), "w");
    check_file(fout, fileout);
    cerr<<"Writing apertium-tagger parameters to file '"<<fileout<<"' ... "<<flush;
    tagger_data.write(fout);
    cerr<<"done.\n";
    fclose(fout);
  }
}
