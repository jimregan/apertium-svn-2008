/*
 * Copyright (C) 2007 Felipe Sánchez-Martínez
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

/** Class TaggerMerging (source file)
 */

#include "TaggerMerging.H"
#include "MorphoStreamMerging.H"
#include "TaggerWordMerging.H"
#include "Utils.H"
#include "TaggerEval.H"

#include <apertium/TaggerWord.H>
#include <apertium/TaggerUtils.H>

#include <cmath>

TaggerMerging::TaggerMerging(FILE *fprob, istream& fcounts, istream& ftagclus, int corpus_length) {
  tmd.read(fprob);

  tmd.read_tagsclusters(ftagclus);
  tmd.calculate_parameters_merging(fcounts, corpus_length);
}

TaggerMerging::~TaggerMerging() {
}

void 
TaggerMerging::tagger (FILE *in, FILE *out) {

  int i, j, k, nw;
  TaggerWordMerging *word=NULL;
  TTag tag;
  
  set <int> tags, pretags;
  set <int>::iterator itag, jtag;
  
  double prob, loli, x;
  int N = tmd.getN();  
  double alpha[2][N];
  vector<TTag> best[2][N];
  
  vector <TaggerWordMerging> wpend; 
  int nwpend;
  
  MorphoStreamMerging morpho_stream(in, Utils::debug, &tmd); 

  Collection &output = tmd.getOutput();
  
  loli = nw = 0;
  
  TTag eos=tmd.getEosTag();

  //Initialization
  tags.insert(eos);
  alpha[0][eos] = 1;
   
  word = morpho_stream.get_next_word();
 
  while (word) {
    wpend.push_back(*word);    	    
    nwpend = wpend.size();
    
    pretags = tags; // Tags from the previous word

    //cerr<<"Word: "<<word->get_superficial_form()<<"\n";
    //cerr<<"Coarse tags:\n";
    //set<int>::iterator it;
    //for(it=word->get_tags().begin(); it!=word->get_tags().end(); it++) {
    //  TTag t=*it;
    //  cerr<<"    "<<t<<" "<<word->get_lexical_form(t, tmd.getFineEofTag())<<"\n";
    //}

    tags = word->get_tags();
  
    if (tags.size()==0) { // This is an unknown word
      cerr<<"Error, there is a word without tags.\n";
      cerr<<"Unknown words should receive an specific tag for them.\n";
      exit(EXIT_FAILURE);
      //tags = tmd.getOpenClass();
    }
    if (output.has_not(tags)) {
      string errors;
      errors = "A new ambiguity class was found. \n";
      errors+= "Retraining the tagger is necessary so as to take it into account.\n";
      errors+= "Word '"+word->get_superficial_form()+"'.\n";
      cerr<<"Error: "<<errors;
      exit(EXIT_FAILURE);
    } 
         
    k = output[tags];  //Ambiguity class the word belongs to
    
    clear_array_double(alpha[nwpend%2], N);    
    clear_array_vector(best[nwpend%2], N);
    
    //Induction
    for (itag=tags.begin(); itag!=tags.end(); itag++) { //For all tag from the current word
      i=*itag;
      for (jtag=pretags.begin(); jtag!=pretags.end(); jtag++) {	//For all tags from the previous word
	j=*jtag;
	x = alpha[1-nwpend%2][j]*(tmd.getA())[j][i]*(tmd.getB())[i][k];
	if (alpha[nwpend%2][i]<=x) {
	  if (nwpend>1) 
	    best[nwpend%2][i] = best[1-nwpend%2][j];
	  best[nwpend%2][i].push_back(i);
	  alpha[nwpend%2][i] = x;
	}
      }
    }
    
    //Backtracking
    if (tags.size()==1) {       
      tag = *tags.begin();      
      
      prob = alpha[nwpend%2][tag];
      
      if (prob>0) 
	loli -= log(prob);
      else {
	cerr<<"Problem with word '"<<word->get_superficial_form()<<"' "<<word->get_string_tags()<<"\n";
      }
      for (unsigned t=0; t<best[nwpend%2][tag].size(); t++) {
	string const &micad = wpend[t].get_lexical_form(best[nwpend%2][tag][t], tmd.getFineEofTag());
	fwrite_unlocked(micad.c_str(), sizeof(char), micad.size(), out); //For eficiency
      }
      
      //Return to the initial state
      wpend.clear();   
      alpha[0][tag] = 1;
    }
    
    delete word;
    word = morpho_stream.get_next_word();    
  }
  
  if (tags.size()>1) {
    string errors;
    errors = "The text to disambiguate has finished, but there are ambiguous words that has not been disambiguated.\n";
    errors+= "This message should never appears. If you are reading this ..... these are very bad news.\n";
    cerr<<"Error: "<<errors;
  }  
}

void 
TaggerMerging::eval_tagger(FILE* ftagged, FILE *funtagged) {
  int i, j, k, nw;
  TaggerWordMerging *word=NULL;
  TTag tag;
  
  set <TTag> tags, pretags;
  set <TTag>::iterator itag, jtag;
  
  double prob, loli, x;
  
  double alpha[2][tmd.getN()];
  vector<TTag> best[2][tmd.getN()];
  
  vector <TaggerWordMerging> wpend;
  int nwpend;
  int nleer_sin_evaluar=0;

  MorphoStreamMerging morpho_stream(funtagged, true, &tmd);
  MorphoStreamMerging morpho_stream_eval(ftagged, true, &tmd);
  
  loli = nw = 0;

  TTag eos=tmd.getEosTag();

  cerr<<"EOS: "<<eos<<"\n";

  //Initialization
  tags.insert(eos);
  alpha[0][eos] = 1;
   
  word = morpho_stream.get_next_word();

  while (word) {
    wpend.push_back(*word);    	    
    nwpend = wpend.size();
    
    pretags = tags; // Tags from the previous word

    tags = word->get_tags();
  
    if (tags.size()==0) { // This is an unknown word
      cerr<<"Error, there is a word without tags.\n";
      cerr<<"Unknown words should receive an specific tag for them.\n";
      exit(EXIT_FAILURE);
      //tags = tmd.getOpenClass();
    }
                       
    if (tmd.getOutput().has_not(tags)) {  //An Unknown word was found
      cerr<<"Warning: A new ambiguity class was found. \n";
      cerr<<"Word '"<<word->get_superficial_form()<<"'.\n";
      cerr<<"New ambiguity class: "<<word->get_string_tags()<<"\n\n";
      exit(EXIT_FAILURE);
    } 
         
    k = tmd.getOutput()[tags];  //Ambiguity class the word belongs to

    clear_array_double(alpha[nwpend%2], tmd.getN());
    clear_array_vector(best[nwpend%2], tmd.getN());
    
    //Induction
    for (itag=tags.begin(); itag!=tags.end(); itag++) { //For all tag from the current word
      i=*itag;
      for (jtag=pretags.begin(); jtag!=pretags.end(); jtag++) {	//For all tags from the previous word
	j=*jtag;
	x = alpha[1-nwpend%2][j]*tmd.getA()[j][i]*tmd.getB()[i][k];
	if (alpha[nwpend%2][i]<=x) {
	  if (nwpend>1) 
	    best[nwpend%2][i] = best[1-nwpend%2][j];
	  best[nwpend%2][i].push_back(i);
	  alpha[nwpend%2][i] = x;
	}
      }
    }
    
    //Backtracking
    if (tags.size()==1) {       
      tag = *tags.begin();      
      
      prob = alpha[nwpend%2][tag];
      
      if (prob>0) 
	loli -= log(prob);
      else {
	cerr<<"Problem with word '"<<word->get_superficial_form()<<"' "<<word->get_string_tags()<<"\n";
      }
      for (unsigned t=0; t<best[nwpend%2][tag].size(); t++) {
	if (nleer_sin_evaluar==0)
	  nleer_sin_evaluar=eval.evalwordmerging(tmd, wpend[t], best[nwpend%2][tag][t], morpho_stream_eval);
	else
	  nleer_sin_evaluar--;   
      }      
      
      //Return to the initial state
      wpend.clear();   
      alpha[0][tag] = 1;
    }
    
    delete word;
    word = morpho_stream.get_next_word();    
  }
  
  if ((tags.size()>1)) {
    cerr<<"Error: Text to disambiguate has finished, but there are ambiguous words that has not been disambiguated.\n";
    cerr<<"This message should never appears, so .... these are very bad news.\n";
  }
}

void 
TaggerMerging::print_evaluation() {
  eval.print_evaluation();
}
