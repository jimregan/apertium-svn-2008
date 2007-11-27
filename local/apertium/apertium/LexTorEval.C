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
#include <cmath>
#include <apertium/LexTorEval.H>
#include <apertium/LexTor.H>

LexTorEval::LexTorEval(istream* iref) {
  nwords=0;
  //nunknown=0;
  nignored=0;
  npol=0;
  //nerrors_nopol=0;
  nerrors_pol=0;
  //nerrors_unk=0;

  ndefault=0;

  refer=iref;

  //words2ignore.insert();
  words2ignore.insert("as<cnjadv>");
  words2ignore.insert("at<pr>");
  words2ignore.insert("before<pr>");
  words2ignore.insert("but<cnjcoo>");
  words2ignore.insert("by<pr>");
  words2ignore.insert("for<pr>");
  words2ignore.insert("how<adv>");
  words2ignore.insert("in<pr>");
  words2ignore.insert("just<adv>");
  words2ignore.insert("off<pr>");
  words2ignore.insert("on<pr>");
  words2ignore.insert("over<pr>");
  words2ignore.insert("right<adv>");
  words2ignore.insert("since<cnjadv>");
  words2ignore.insert("whether<cnjadv>");
}

LexTorEval::~LexTorEval() {
}

void 
LexTorEval::print_evaluation() {
  cerr<<"# of words.......................................... "<<nwords<<"\n"
      <<"# of ignored words.................................. "<<nignored<<"\n"
      <<"# of polisemous words............................... "<<npol<<"\n"
      <<"# of errors due to polisemous words................. "<<nerrors_pol<<"\n"
      <<"# of times context does not discriminate (NODIS).... "<<ndefault<<"\n"
      <<"% of polysemous words............................... "<<(npol/nwords)*100.0<<" %\n"
      <<"% of error over polisemous words ................... "<<(nerrors_pol/npol)*100.0<<" %\n"
      <<"% of error over all words .......................... "<<(nerrors_pol/nwords)*100.0<<" %\n"
      <<"% of NODIS.......................................... "<<(ndefault/npol)*100.0<<" %\n";
  cerr<<"NOTE: # ignored words ARE NOT included in the rest of counts\n";

  return;

  cerr<<"\nReport by words:\n---------------------------------------\n";

  map<string, double>::iterator it;
  cerr<<"WORD\t\tOCCURR\tERROR\tDEFAULT\t%ERROR\t%DEFAULT\n";
  cerr<<"-----------------------------------------------------------------\n";
  for(it=nwords_per_word.begin(); it!=nwords_per_word.end(); it++) {
    cerr<<it->first<<"\t"<<it->second<<"\t"<<nerrors_per_word[it->first]<<"\t"
	<<ndefault_per_word[it->first]<<"\t"<<(nerrors_per_word[it->first]/it->second)*100<<"\t"
	<<(ndefault_per_word[it->first]/it->second)*100<<"\n";
  }
}

void 
LexTorEval::evalword(LexTorWord& ltword, int winner, LexTorData* lextor_data) {
  string reduced_w=lextor_data->reduce(ltword.get_lexical_choice(winner,false));
  string word=lextor_data->reduce(ltword.get_word_string());
  string wref;
  string reduced_wref;
  bool ignore=false;

  getline(*refer,wref);

  //  if (words2ignore.find(word)!=words2ignore.end()) {
  //  return;
  //}

  if (wref.find(">__IGNORE") != string::npos) 
    ignore=true;
  
  if (!ignore) {
    nwords+=1.0;
    reduced_wref=lextor_data->reduce(wref);
    if (ltword. n_lexical_choices()>1) {
      npol+=1.0;
      nwords_per_word[word]+=1.0;
      if (winner<0) {
	ndefault+=1.0;
	ndefault_per_word[word]+=1.0;
      }
      if (reduced_w!=reduced_wref) {
	nerrors_pol+=1.0;
	nerrors_per_word[word]+=1.0;
	if (LexTor::debug) {
	  cerr<<"========= ERROR\n";
	}
      } else {
	if (LexTor::debug) {
	  cerr<<"========= OK\n";
	}
      }
    } else {
      //if (LexTor::debug)
      //  cerr<<"EVAL: reduced_w="<<reduced_w<<" reduced_wref="<<reduced_wref<<"\n";

      if(reduced_wref!=reduced_w) {
	cerr<<"Error: Input and reference corpora are not aligned\n";
	cerr<<"word="<<reduced_w<<" ref. word="<<reduced_wref<<"\n";
	cerr<<"Number of words: "<<nwords+nignored<<"\n";
	exit(EXIT_FAILURE);
      }
    }
  } else {
    //reduced_wref=wref;
    nignored+=1.0;
    if (LexTor::debug) {
      cerr<<"========= IGNORED\n";
    }

  }
}
