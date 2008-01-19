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

#include <apertium/MorphoStream.H>
#include <apertium/TaggerWord.H>
#include <apertium/TaggerUtils.H>
#include <cmath>
#include "TaggerEval.H"
#include "Utils.H"

TaggerEval::TaggerEval() {
  nwords=0.0;
  nunknown=0.0;
  nignored=0.0;
  nambiguous=0.0;
  nerrors_noamb=0.0;
  nerrors_amb=0.0;
  nerrors_unk=0.0;
}

TaggerEval::TaggerEval(FILE* fprob) {
  tagger_data.read(fprob);

  nwords=0.0;
  nunknown=0.0;
  nignored=0.0;
  nambiguous=0.0;
  nerrors_noamb=0.0;
  nerrors_amb=0.0;
  nerrors_unk=0.0;

  eos=tagger_data.getTagIndex()["TAG_SENT"];

  TaggerWord::setArrayTags(tagger_data.getArrayTags());
}

TaggerEval::~TaggerEval() {
}

void 
TaggerEval::eval_tagger(FILE* ftagged, FILE *funtagged){
  int i, j, k, nw;
  TaggerWord *word=NULL;
  TTag tag;
  
  set <TTag> tags, pretags;
  set <TTag>::iterator itag, jtag;
  
  double prob, loli, x;
  
  double alpha[2][tagger_data.getN()];
  vector<TTag> best[2][tagger_data.getN()];
  
  vector <TaggerWord> wpend; 
  int nwpend;
  int nleer_sin_evaluar=0;
  
  MorphoStream morpho_stream(funtagged, true, &tagger_data);
  MorphoStream morpho_stream_eval(ftagged, true, &tagger_data);
  
  loli = nw = 0;
  
  //Initialization
  tags.insert(eos);
  alpha[0][eos] = 1;
   
  word = morpho_stream.get_next_word();

  while (word) {
    wpend.push_back(*word);    	    
    nwpend = wpend.size();
    
    pretags = tags; // Tags from the previous word

    tags = word->get_tags();
  
    if (tags.size()==0) // This is an unknown word
      tags = tagger_data.getOpenClass();
                       
    if (tagger_data.getOutput().has_not(tags)) {  //An Unknown word was found

      cerr<<"Warning: A new ambiguity class was found. \n";
      cerr<<"Retraining the tagger is necessary so as to take it into account.\n";
      cerr<<"Word '"<<word->get_superficial_form()<<"'.\n";
      cerr<<"New ambiguity class: "<<word->get_string_tags()<<"\n\n";

      cerr<<"You can still continue using this tagger parameters, but unfortunately you cannot";
      cerr<<"evaluate them. This is just because the evaluation would be not correct\n";

      exit(EXIT_FAILURE);
    } 
         
    k = tagger_data.getOutput()[tags];  //Ambiguity class the word belongs to

    clear_array_double(alpha[nwpend%2], tagger_data.getN());
    clear_array_vector(best[nwpend%2], tagger_data.getN());
    
    //Induction
    for (itag=tags.begin(); itag!=tags.end(); itag++) { //For all tag from the current word
      i=*itag;
      for (jtag=pretags.begin(); jtag!=pretags.end(); jtag++) {	//For all tags from the previous word
	j=*jtag;
	x = alpha[1-nwpend%2][j]*tagger_data.getA()[j][i]*tagger_data.getB()[i][k];
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
	  nleer_sin_evaluar=evalword(wpend[t], best[nwpend%2][tag][t], morpho_stream_eval);
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
TaggerEval::print_evaluation() {
  cout<<"# of words........................................... "<<nwords<<"\n"
      <<"# of unknown words................................... "<<nunknown<<"\n"
      <<"# of ignored words................................... "<<nignored<<"\n"
      <<"# of ambiguous words................................. "<<nambiguous<<"\n"
      <<"# of errors NOT due to ambiguous words............... "<<nerrors_noamb<<"\n"
      <<"# of errors due to ambiguous words................... "<<nerrors_amb<<"\n"
      <<"# of errors due to unknown words..................... "<<nerrors_unk<<"\n"
      <<"% of unknown words................................... "<<(nunknown/nwords)*100.0<<" %\n"
      <<"% of ambiguous words................................. "<<(nambiguous/nwords)*100.0<<" %\n"
      <<"% of unknown and ambiguous words..................... "<<((nambiguous+nunknown)/nwords)*100.0<<" %\n"
      <<"% of error over ambiguous words ..................... "<<(nerrors_amb/nambiguous)*100.0<<" %\n"
      <<"% of error over ambiguous and unknown words.......... "<<((nerrors_amb+nerrors_unk)/(nambiguous+nunknown))*100.0<<" %\n"
      <<"% of error over all words ........................... "<<((nerrors_amb+nerrors_unk)/nwords)*100.0<<" %\n"
      <<"% of error over all words (including non amb. errors) "<<((nerrors_amb+nerrors_unk+nerrors_noamb)/nwords)*100.0<<" %\n";

  cerr<<"NIGNORED: "<<nignored<<"\n";
}

int
TaggerEval::evalword(TaggerWord& word, TTag& tag, MorphoStream& morpho_streameval) {
  static string fsok;
  static TTag tagok;   
  static bool read_word_ok=true;
  static TaggerWord *wordok;    
   
  string fstag; 
  set<TTag> tagsok;   
   
  fstag=word.get_superficial_form();
   
  if(read_word_ok) {   
    wordok = morpho_streameval.get_next_word();
    fsok = wordok->get_superficial_form();
    tagsok = wordok->get_tags();   

    if(tagsok.size()>1) {
      cerr<<"Error in tagged corpus (.eval) used for evaluation. A word with more than one tag was found\n"<<*wordok<<"\n";
      return 0;
    }
    else if(tagsok.size()==0) {
      cerr<<"Error in tagged corpus (.eval) used for evaluation. An unknown word was found\n"<<*wordok<<"\n";
      return 0;
    }
   
    tagok = *(tagsok.begin());
  
    nwords+=1.0;
  }
  else
    read_word_ok=true;
   
  if ((tagok!=tag)&&(tag==eos)) { 
    //An end-of-sentence that needs to be skeeped
    read_word_ok=false;
    return 0;
  }

  Utils::print_debug(word.get_string_tags()+" "+word.get_superficial_form());
  Utils::print_debug(" ("+tagger_data.getArrayTags()[tag]+") \t--\t ");
  Utils::print_debug(wordok->get_superficial_form()+" (");
  Utils::print_debug(tagger_data.getArrayTags()[tagok]+")  ===> ");

  if (fstag!=fsok) {  	    
    int ntokens_ok = ntokens_multiword(wordok->get_lexical_form(tagok, tagger_data.getTagIndex()["TAG_kEOF"]));
    int ntokens_tag = ntokens_multiword(word.get_lexical_form(tag, tagger_data.getTagIndex()["TAG_kEOF"]));
    int words_distance = abs(ntokens_ok - ntokens_tag);
    if(ntokens_ok<ntokens_tag) { 
      //We need to read more words from the tagged corpus used for evaluation
      //so as to align it witrh the corpus being tagged
      while(words_distance>0) {
	delete wordok;
	wordok = morpho_streameval.get_next_word();
	words_distance--;
      }
      words_distance=0;
    }
      
    if (words_distance>0) {
      delete wordok;
      nignored+=1.0;
      Utils::print_debug(" IGNORED (multiword): ");
      Utils::print_debug(words_distance);
      Utils::print_debug("\n");
      return words_distance;
    }
      
    int nguiones_ok = nguiones_fs(fsok); 
    int nguiones_tag = nguiones_fs(fstag);
    words_distance = abs(nguiones_ok-nguiones_tag);
    if(nguiones_ok<nguiones_tag) { 
      //We need to read more words from the tagged corpus used for evaluation
      //so as to align it witrh the corpus being tagged
      while(words_distance>0) {
	delete wordok;
	wordok = morpho_streameval.get_next_word();
	words_distance--;
      }
      words_distance=0;
    }
    delete wordok;
    nignored+=1.0;
    Utils::print_debug(" IGNORED (hyphen): ");
    Utils::print_debug(words_distance);
    Utils::print_debug("\n");
    return words_distance;      
  }
 
  //Both superficial forms are equal
  if (word.get_tags().size()>1) // Ambiguous
    nambiguous+=1.0;
  else if (word.get_tags().size()==0) { // Unknown
    nunknown+=1.0;
  }

  if (tag!=tagok) {
    if (word.get_tags().size()==0) {
      nerrors_unk+=1.0;
      Utils::print_debug("ERROR UNKNOWN");
      if (tagger_data.getOpenClass().find(tagok)==tagger_data.getOpenClass().end())
	Utils::print_debug(", TAG NOT AVAILABLE IN THE OPEN CLASS");
      Utils::print_debug("\n");
    } else  if (word.get_tags().size()==1) {
      nerrors_noamb+=1.0;
      Utils::print_debug("ERROR TAG OK NOT AVAILABLE (NO AMBIGUOUS)\n");
    } else {
      if (word.get_tags().find(tagok)==word.get_tags().end()) {
	nerrors_noamb+=1.0;
	Utils::print_debug("ERROR TAG OK NOT AVAILABLE (AMBIGUOUS)\n");
      }
      else { 
	nerrors_amb+=1.0;
	Utils::print_debug("ERROR\n");
      }
    }
  }
  else
    Utils::print_debug("OK\n");
   
  delete wordok;   
  return 0;
}

int
TaggerEval::evalwordmerging(TaggerMergingData& tmd, TaggerWordMerging& word, TTag& tag, MorphoStreamMerging& morpho_streameval) {
  static string fsok;
  static TTag tagok;   
  static bool read_word_ok=true;
  static TaggerWord *wordok;    
   
  string fstag; 
  set<TTag> tagsok;   
   
  fstag=word.get_superficial_form();
   
  if(read_word_ok) {   
    wordok = morpho_streameval.get_next_word();
    fsok = wordok->get_superficial_form();
    tagsok = wordok->get_tags();

    if(tagsok.size()>1) {
      cerr<<"Error in tagged corpus (.eval) used for evaluation. A word with more than one tag was found\n"<<*wordok<<"\n";
      return 0;
    }
    else if(tagsok.size()==0) {
      cerr<<"Error in tagged corpus (.eval) used for evaluation. An unknown word was found\n"<<*wordok<<"\n";
      return 0;
    }
   
    tagok = *(tagsok.begin());
  
    nwords+=1.0;
  }
  else
    read_word_ok=true;
   
  if (((tagok!=tag)||(fstag != fsok)) && (tag==tmd.getEosTag()) && (fstag==".")) {
    cerr<<"SKIPING EOS: "<<tag<<"\n";
    //An end-of-sentence that needs to be skeeped
    read_word_ok=false;
    return 0;
  }

  Utils::print_debug(word.get_string_tags()+" "+word.get_superficial_form());
  Utils::print_debug(" (");
  Utils::print_debug(tag);
  Utils::print_debug(") \t--\t ");
  Utils::print_debug(wordok->get_superficial_form()+" (");
  Utils::print_debug(tagok);
  Utils::print_debug(")  ===> ");

  if (fstag!=fsok) {  	    
    int ntokens_ok = ntokens_multiword(wordok->get_lexical_form(tagok, tmd.getFineEofTag()));
    int ntokens_tag = ntokens_multiword(word.get_lexical_form(tag, tmd.getFineEofTag()));
    int words_distance = abs(ntokens_ok - ntokens_tag);
    if(ntokens_ok<ntokens_tag) { 
      //We need to read more words from the tagged corpus used for evaluation
      //so as to align it witrh the corpus being tagged
      while(words_distance>0) {
	delete wordok;
	wordok = morpho_streameval.get_next_word();
	words_distance--;
      }
      words_distance=0;
    }
      
    if (words_distance>0) {
      delete wordok;
      nignored+=1.0;
      Utils::print_debug(" IGNORED (multiword): ");
      Utils::print_debug(words_distance);
      Utils::print_debug("\n");

      return words_distance;
    }
      
    int nguiones_ok = nguiones_fs(fsok); 
    int nguiones_tag = nguiones_fs(fstag);
    words_distance = abs(nguiones_ok-nguiones_tag);
    if(nguiones_ok<nguiones_tag) { 
      //We need to read more words from the tagged corpus used for evaluation
      //so as to align it witrh the corpus being tagged
      while(words_distance>0) {
	delete wordok;
	wordok = morpho_streameval.get_next_word();
	words_distance--;
      }
      words_distance=0;
    }
    delete wordok;
    nignored+=1.0;
    Utils::print_debug(" IGNORED (hyphen): ");
    Utils::print_debug(words_distance);
    Utils::print_debug("\n");

    return words_distance;      
  }
 
  //Both superficial forms are equal
  if (word.get_tags().size()>1) // Ambiguous
    nambiguous+=1.0;
  else if (word.is_unknown()) { // Unknown
    nunknown+=1.0;
  }

  if (tag!=tagok) {
    if (word.get_tags().size()==0) {
      nerrors_unk+=1.0;
      Utils::print_debug("ERROR UNKNOWN");
      if (tagger_data.getOpenClass().find(tagok)==tagger_data.getOpenClass().end())
	Utils::print_debug(", TAG NOT AVAILABLE IN THE OPEN CLASS");
      Utils::print_debug("\n");
    } else  if (word.get_tags().size()==1) {
      if (!word.is_unknown()) {
	nerrors_noamb+=1.0;
	Utils::print_debug("ERROR TAG OK NOT AVAILABLE (NO AMBIGUOUS)\n");
      }
    } else {
      if (word.get_tags().find(tagok)==word.get_tags().end()) {
	nerrors_noamb+=1.0;
	Utils::print_debug("ERROR TAG OK NOT AVAILABLE (AMBIGUOUS)\n");
      }
      else { 
	nerrors_amb+=1.0;
	Utils::print_debug("ERROR\n");
      }
    }
  }
  else
    Utils::print_debug("OK\n");
   
  delete wordok;   
  return 0;
}
