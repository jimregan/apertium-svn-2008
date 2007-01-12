/*
 * Copyright (C) 2006 Universitat d'Alacant / Universidad de Alicante
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

#include <apertium/LexTorWord.H>
#include <apertium/StringUtils.H>

LexTorWord::LexTorWord() {
  ignored_string="";
  word="";
  default_choice=0;
}
  
LexTorWord::LexTorWord(const LexTorWord& ltw) {
  word=ltw.word;
  ignored_string=ltw.ignored_string;
  lexical_choices=ltw.lexical_choices;
  default_choice=ltw.default_choice;
}

LexTorWord::LexTorWord(const string& str, FSTProcessor *fstp) {
  word=str;
  ignored_string="";
  extract_lexical_choices(fstp);
}
  
LexTorWord::~LexTorWord() {
}

string 
LexTorWord::get_word_string() {
  return word;
}

int 
LexTorWord::n_lexical_choices() {
  return lexical_choices.size();
}

string 
LexTorWord::get_lexical_choice(int choice, bool include_ignored) {
  if (word=="") {
    if (include_ignored)
      return ignored_string;
    else 
      return "";
  }

  if (choice<0)
    choice=default_choice;

  if (choice>=(int)lexical_choices.size()) {
    cerr<<"Error in LexTorWord::get_lexical_choice, choice position given is "
	<<"greater than the number of choices available\n";
    cerr<<"position requested: "<<choice<<"\n";
    cerr<<"number of lexical choices: "<<lexical_choices.size()<<"\n";
    cerr<<"Returning default choice\n";
    choice=default_choice;
  }

  if (include_ignored)
    return ignored_string+"^"+lexical_choices[choice]+"$";
  else
    return lexical_choices[choice];
}

string 
LexTorWord::translate(FSTProcessor& bildic, int lexchoice) {
  string t;

  //cerr<<"In LexTorWord::translate, called with: "<<word<<", lexchoice: "<<lexchoice<<"\n";

  if (lexchoice<0)
    lexchoice=default_choice;
  else if(lexchoice>=(int)lexical_choices.size()) {
    cerr<<"Error in LexTorWord::translate, choice position given is "
	<<"greater than the number of choices available\n";
    cerr<<"position requested: "<<lexchoice<<"\n";
    cerr<<"number of lexical choices: "<<lexical_choices.size()<<"\n";
    cerr<<"Returning default choice\n";
    lexchoice=default_choice;
  }

  t=bildic.biltrans(lexical_choices[lexchoice],false);

  //cerr<<"Translation: "<<t<<"\n";

  return t;
}

void 
LexTorWord::extract_lexical_choices(FSTProcessor *fstp) {

  lexical_choices=StringUtils::split_string(fstp->biltrans(word,false),"/");
  default_choice=0;

  if (lexical_choices.size()>1) { //lexically ambiguous word
    for(unsigned i=0; i<lexical_choices.size(); i++) {

      unsigned p=lexical_choices[i].find(" D<");
      if (p!=string::npos) {
	if (!((lexical_choices[i].length()>p+2) && (lexical_choices[i][p+2]=='<'))) {
	  cerr<<"Error in LexTorWord::next_word when analyzing lexical options\n";
	  cerr<<"Word: "<<word<<"; lexical choices: "<<fstp->biltrans(word,false)<<"\n";
	  exit(EXIT_FAILURE);
	}
	default_choice=i;
      }
    }
  }
}

LexTorWord* 
LexTorWord::next_word(istream& is, FSTProcessor *fstp) {
  LexTorWord w;
  char c, prev_c=' ';
  bool finish=false;
  bool reading_word=false;

  while (!finish) {
    is>>c;

    if (is.fail()) {
      if (reading_word) {
	cerr<<"Error in LexTorWord::next_word while reading input word\n";
	cerr<<"Malformed input string, at '"<<c<<"'\n";
	exit(EXIT_FAILURE);
      } else {
	if ((w.word.length()>0)||(w.ignored_string.length()>0)) {
	  if(fstp!=NULL)
	    w.extract_lexical_choices(fstp);
	  return new LexTorWord(w);
	} else 
	  return NULL;
      }
    }

    if ((c=='^') && (prev_c!='\\') && (!reading_word)) {
      reading_word=true;
    } else if ((c=='$') && (prev_c!='\\') && (reading_word)) {
      finish=true;
    } else {
      if (reading_word)
	w.word+=c;
      else
	w.ignored_string+=c;
    }
    prev_c=c;
  }

  if ((w.word.length()==0) && (w.ignored_string.length()==0))
    return NULL;

  if(fstp!=NULL)
    w.extract_lexical_choices(fstp);

  /*
    cerr<<"word: "<<w.word<<"\n";
    for (unsigned i=0; i<w.lexical_choices.size(); i++) {
    cerr<<"Lex choice at "<<i<<": "<<w.lexical_choices[i]<<"\n";
    }
    cerr<<"Default: "<<w.default_choice<<"\n\n";
  */

  return new LexTorWord(w);
}

