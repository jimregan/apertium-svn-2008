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


#include "AlignmentTemplate.H"
#include "Utils.H"

#include <cstdlib>
#include <iostream>

LexicalizedWords AlignmentTemplate::source_lexicalized_words;
LexicalizedWords AlignmentTemplate::target_lexicalized_words;

AlignmentTemplate::AlignmentTemplate() {
  invalid=VALID;
}

AlignmentTemplate::AlignmentTemplate(string al):Alignment(al,5) {
  invalid=VALID;
  vector<string> v;

  v=Utils::split_string(al, " | ");

  if (v.size()!=5) {
    cerr<<"Error in AlignmentTemplate::AlignmentTemplate when reading alignment template from string '"<<al<<"'\n";
    cerr<<"Unespected number of fields separated by ' | '\n";
    exit(EXIT_FAILURE); 
  }

  restrictions=Utils::split_string(v[4], " ");
}

AlignmentTemplate::AlignmentTemplate(const AlignmentTemplate& al):Alignment(al) {
  invalid=al.invalid;
  restrictions=al.restrictions;
}
    
AlignmentTemplate::~AlignmentTemplate() {
}

void 
AlignmentTemplate::set_count(int count) {
  score=(double)count;
}

int
AlignmentTemplate::get_count() {
  return (int) score;
}

bool
AlignmentTemplate::is_valid(bool equalcat, bool noword4word, FSTProcessor& fstp, Alignment& bilph) {

  if(invalid!=VALID)
    return false;

  //Now follows some additional validations

  //There cannot be INVALID restrictions
  for (unsigned i=0; i<restrictions.size(); i++) {
    if (restrictions[i]=="__INVALID__") {
      invalid=INVALID_RESTRICTIONS;
      return false;
    }
  }

  if (equalcat) {
    //Test that those open words that are aligned have the same
    //category, i. e., the same first tag
    for (unsigned i=0; i<source.size(); i++) {
      if (source[i][0]=='<') { //It's an open word
	int j=get_open_target_word_pos(i);

	string first_sl_tag=Utils::get_first_tag(source[i]);
	string first_tl_tag=Utils::get_first_tag(target[j]);

	if (first_sl_tag!=first_tl_tag) {
	  invalid=INVALID_NO_EQUALCAT;
	  return false;
	}
      }
    }
  }

  if (noword4word) {
    if (is_equivalent_to_word_for_word(bilph, fstp)) {
      invalid=INVALID_EQUIVALENT_WORD_FOR_WORD;
      return false;
    }
  }

  invalid=VALID;
  return true;
}

int 
AlignmentTemplate::invalid_reason() {
  return invalid;
}

string
AlignmentTemplate::to_string() {
  Alignment &al=(*this);
  string s=al.to_string();

  s+=" |";

  for(unsigned i=0; i<restrictions.size(); i++) 
    s+=" "+restrictions[i];

  return s;
}

ostream& operator << (ostream& os, AlignmentTemplate& at) {
  os<<at.to_string();
  return os;
  /*
  Alignment &al=at;
  os<<al<<" |";

  for(unsigned i=0; i<at.restrictions.size(); i++) 
    os<<" "<<at.restrictions[i];

  return os;
  */
}


void 
AlignmentTemplate::set_lexicalized_words(const LexicalizedWords& scw, const LexicalizedWords& tcw) {
  source_lexicalized_words=scw;
  target_lexicalized_words=tcw;

  source_lexicalized_words.insert_ends();
  target_lexicalized_words.insert_ends();
}

AlignmentTemplate 
AlignmentTemplate::xtract_alignment_template(Alignment& al, FSTProcessor& fstp) {
  AlignmentTemplate at;

  at.invalid=VALID;
  at.alignment=al.alignment;
  at.score=0;

  int nopen_source=0;
  int nopen_target=0;

  //Determine the word class for each source word
  for(unsigned i=0; i<al.source.size(); i++) {
    string w=Utils::remove_begin_and_end_marks(al.source[i]);
    string wclass;

    if (Utils::is_unknown_word(w))
      wclass="__UNKNOWN__";
    else if (source_lexicalized_words.is_lexicalized_word(w)) 
      wclass=w;
    else {
      wclass=Utils::get_tags(w);
      nopen_source++;
    }
    at.source.push_back(wclass);
  }

  //Determine the word class for each target word
  for(unsigned i=0; i<al.target.size(); i++) {
    string w=Utils::remove_begin_and_end_marks(al.target[i]);
    string wclass;

    if (Utils::is_unknown_word(w))
      wclass="__UNKNOWN__";
    else if (target_lexicalized_words.is_lexicalized_word(w))
      wclass=w;
    else {
      wclass=Utils::get_tags(w);
      nopen_target++;
    }
    at.target.push_back(wclass);
  }

  if (nopen_source!=nopen_target) {
    at.invalid=INVALID_WRONG_OPEN_WORDS;
    at.restrictions.push_back("__INVALID__");
    return at;
  }

  if (!at.are_open_word_aligments_ok()) {
    at.invalid=INVALID_WRONG_OPEN_WORDS;
    at.restrictions.push_back("__INVALID__");
    return at;
  }

  //Calculate the restricitions that need to be met
  //before applying this AT
  for(unsigned i=0; i<al.source.size(); i++) {
    string w=Utils::remove_begin_and_end_marks(al.source[i]);
    string biltrans, bclass;

    if (Utils::is_unknown_word(w)) 
      bclass="__UNKNOWN__";
    else if (source_lexicalized_words.is_lexicalized_word(w)) 
      bclass="__CLOSEWORD__";
    else {
      //We need to replace each "_" in w by a blank " " before translating
      w=Utils::substitute(w,"_"," ");

      biltrans=fstp.biltransWithoutQueue(w, false);

      if (!is_translation_ok(biltrans)) {
	at.invalid=INVALID_NO_OK_TRANSLATIONS;
	cerr<<"Error in AlignmentTemplate::xtract_alignment_template: ";
	cerr<<"There were no OK translation for source word '"<<w<<"', translation was: '"<<biltrans<<"'\n";
	at.restrictions.push_back("__INVALID__");
	return at;
      } 

      //Execution continues only if ALL since this point was OK
      bclass=Utils::get_tags(biltrans);
      string blemma=Utils::get_lemma(biltrans);

      int tlp=at.get_open_target_word_pos(i);
      if (tlp>=0) {
	string tlemma=Utils::substitute(Utils::get_lemma(Utils::remove_begin_and_end_marks(al.target[tlp])),"_"," ");

	if (Utils::strtolower(blemma) != Utils::strtolower(tlemma)) {
	  at.invalid=INVALID_DIFFERENT_TRANSLATIONS;
	  cerr<<"Warning: The AT extracted from the following alignment cannot be used.\n";
	  cerr<<al<<"\n";
	  cerr<<"Cause: translation of '"<<w<<"' is '"<<blemma<<"', but it should be '"<<tlemma<<"'\n";
	  at.restrictions.push_back("__INVALID__");
	  return at;
	}
      } else {
	cerr<<"Warning: The AT extracted from the following alignment cannot be used.\n";
	cerr<<al<<"\n";
	cerr<<"Cause: bug not solved\n";
	at.invalid=INVALID_OTHERS;
	at.restrictions.push_back("__INVALID__");
	return at;
      }      
    }
    at.restrictions.push_back(bclass);
  }

  return at;
}

bool
AlignmentTemplate::is_translation_ok(string t) {
  return ((t.length()>0)&&(t[0]!='@'));
}

/* OLD, realmente testea que la aplicación del AT proporcione el
mismo resultado - VER!!!!!!!!!!
bool 
AlignmentTemplate::is_equivalent_to_word_for_word(Alignment& al, FSTProcessor& fstp) {
  string al_target="";
  string at_translation="";
  //cerr<<"AT: "<<to_string()<<"\n";
  //cerr<<"AL: "<<al<<"\n";
  for(unsigned i=0; i<target.size(); i++) {
    if (at_translation.length()>0)
      at_translation+=" ";

    if (target[i]=="__UNKNOWN__") {
      at_translation+=target[i];
      //cerr<<"Unknown word: "<<target[i]<<"\n";
    } else if (target[i][0]!='<') { //It's a close word we copy it as is
      at_translation+="^"+target[i]+"$";
      //cerr<<"Close word: "<<target[i]<<"\n";
    }  else {
      int p=get_open_source_word_pos(i);
      string w=Utils::remove_begin_and_end_marks(al.source[p]);
      string t=fstp.biltrans(w, false);
      //cerr<<"Source: "<<al.source[p]<<" Target: "<<target[i]<<" Translation: "<<t<<"\n";
      at_translation+="^"+Utils::get_lemma(t)+target[i]+"$";
    }
  }

  for(unsigned i=0; i<al.target.size(); i++) {
    if(al_target.length()>0)
      al_target+=" ";
    if (Utils::is_unknown_word(Utils::remove_begin_and_end_marks(al.target[i])))
      al_target+="__UNKNOWN__";
    else
      al_target+=al.target[i];
  }

  at_translation=Utils::strtolower(at_translation);
  al_target=Utils::strtolower(al_target);

  //cerr<<"TRANSLATION: "<<at_translation<<"\n";
  //cerr<<"TL AL SIDE: "<<al_target<<"\n";
  //cerr<<"Equivalent to word4word: "<<(al_target==at_translation)<<"\n\n";
  return (al_target==at_translation);
}
*/

bool 
AlignmentTemplate::is_equivalent_to_word_for_word(Alignment& al, FSTProcessor& fstp) {
  string word4word="";
  string translation="";

  //cerr<<"AT: "<<to_string()<<"\n";
  //cerr<<"AL: "<<al<<"\n";

  //First compute the translation applying the AT
  for(unsigned i=0; i<target.size(); i++) {
    if (translation.length()>0)
      translation+=" ";

    if (target[i]=="__UNKNOWN__") {
      translation+=target[i];
    } else if (target[i][0]!='<') { //It's a close word we copy it as is
      translation+="^"+target[i]+"$";
    }  else {
      int p=get_open_source_word_pos(i);
      string w=Utils::remove_begin_and_end_marks(al.source[p]);
      string t=fstp.biltrans(w, false);
      translation+="^"+Utils::get_lemma(t)+target[i]+"$";
    }
  }

  //Now compute the word for word translation
  for(unsigned i=0; i<al.source.size(); i++) {
    if (word4word.length()>0)
      word4word+=" ";
    string w=Utils::remove_begin_and_end_marks(al.source[i]);
    if (Utils::is_unknown_word(w))
      word4word+="__UNKNOWN__";
    else
      word4word+="^"+fstp.biltrans(w, false)+"$";
  }


  translation=Utils::strtolower(translation);
  word4word=Utils::strtolower(word4word);

  //cerr<<"TRANSLATION: "<<translation<<"\n";
  //cerr<<"WORD4WORD: "<<word4word<<"\n";
  //cerr<<"Equivalent to word4word: "<<(translation==word4word)<<"\n\n";

  return (translation==word4word);
}

bool
AlignmentTemplate::are_open_word_aligments_ok() {
  bool open_sl[source.size()];
  bool open_tl[target.size()];

  for(unsigned i=0; i<source.size(); i++) {
    if ((source[i][0]=='<')||(source[i]=="__UNKNOWN__"))
      open_sl[i]=true;
    else
      open_sl[i]=false;
  }

  for(unsigned i=0; i<target.size(); i++) {
    if ((target[i][0]=='<')||(target[i]=="__UNKNOWN__"))
      open_tl[i]=true;
    else
      open_tl[i]=false;
  }

  //Each open SL word must be aligned with only one open TL word
  for (unsigned i=0; i<source.size(); i++) {
    if (open_sl[i]) {
      bool aligned=false;
      for (unsigned j=0; j<target.size(); j++) {
	if (open_tl[j]) {
	  if (alignment[i][j]) {
	    if (aligned)
	      return false;
	    aligned=true;
	  }
	}
      }
      if (!aligned)
	return false;
    }
  }

  //Each open TL word must be aligned with only one open SL word
  for (unsigned i=0; i<target.size(); i++) {
    if (open_tl[i]) {
      bool aligned=false;
      for (unsigned j=0; j<source.size(); j++) {
	if (open_sl[j]) {
	  if (alignment[j][i]) {
	    if (aligned)
	      return false;
	    aligned=true;
	  }
	}
      }
      if (!aligned)
	return false;
    }
  }

  return true;
}

int 
AlignmentTemplate::get_open_source_word_pos(int target_pos) {
  for (unsigned i=0; i<source.size(); i++) {
    if (((source[i][0]=='<')||(source[i]=="__UNKNOWN__")) && (alignment[i][target_pos]))
      return i;
  }

  cerr<<"Error in AlignmentTemplate::get_open_source_word_pos: No open source word aligned with target word at "<<target_pos<<" was found\n";
  cerr<<"AT: "<<to_string()<<"\n";
  exit(EXIT_FAILURE);

  return -1;
}

int 
AlignmentTemplate::get_open_target_word_pos(int source_pos) {
  for (unsigned i=0; i<target.size(); i++) {
    if (((target[i][0]=='<')||(target[i]=="__UNKNOWN__")) && (alignment[source_pos][i]))
      return i;
  }

  cerr<<"Error in AlignmentTemplate::get_open_target_word_pos: No open target word aligned with source word at "<<source_pos<<" was found\n";
  cerr<<"AT: "<<to_string()<<"\n";
  exit(EXIT_FAILURE);

  return -1;
}
