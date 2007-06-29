/*
 * Copyright (C) 2005 Universitat d'Alacant / Universidad de Alicante
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
#include <apertium/TaggerWord.H>

vector<string> TaggerWord::array_tags;
bool TaggerWord::show_ingnored_string=true;

TaggerWord::TaggerWord(bool prev_plus_cut){
   ignored_string="";
   plus_cut=false;
   previous_plus_cut=prev_plus_cut;
}

TaggerWord::TaggerWord(const TaggerWord &w){
  superficial_form = w.superficial_form;
  tags = w.tags;
  lexical_forms = w.lexical_forms;
  ignored_string = w.ignored_string;
  plus_cut = w.plus_cut;
  previous_plus_cut=w.previous_plus_cut;
}

TaggerWord::~TaggerWord(){
}

void
TaggerWord::set_superficial_form(const string &sf){
  superficial_form = sf;
}

string& 
TaggerWord::get_superficial_form() {
  return superficial_form;
}

void
TaggerWord::add_tag(TTag &t, const string &lf, vector<string> const &prefer_rules){

  //Tag is added only is it is not present yet
  //Sometime one word can have more than one lexical form assigned to the same tag
  if (tags.find(t)==tags.end()) {
    tags.insert(t);
    lexical_forms[t]=lf;
  } else {
    //Take a look at the prefer rules
    for(int i=0; i < (int) prefer_rules.size(); i++) {
      if (lf.find(prefer_rules[i])<lf.size()) {
	lexical_forms[t]=lf;
	break;
      }
    }
  }
}

set<TTag>&
TaggerWord::get_tags() {
  return tags;
}

string
TaggerWord::get_string_tags() {
//  static string array_tags[] = TAGS_ARRAY;
  string st;
  set<TTag>::iterator itag;
  
  st="{";  
  for(itag=tags.begin(); itag!=tags.end(); itag++) {
    if (itag!=tags.begin())
      st+=",";
    st+=array_tags[*itag];
  }
  st += "}";  
  
  return st;  
}

string
TaggerWord::get_lexical_form(TTag &t, int const TAG_kEOF) {
  string ret="";

  if (show_ingnored_string)
    ret+=ignored_string;
   
  if(t==TAG_kEOF)
    return ret;

  if (!previous_plus_cut)
    ret+="^"; 

  if (lexical_forms.size()==0) { // This is an UNKNOWN WORD
    ret +="*"+superficial_form;
  } else if ((*lexical_forms.begin()).second[0]=='*') { //This is an
							//unknown word
							//that has
							//been guessed
    ret += "*"+superficial_form;
  } else if (lexical_forms.size()>1) {  //This is an ambiguous word
    ret +=lexical_forms[t];
  } else {
    ret +=lexical_forms[t];
  }
  
  if (ret != ignored_string) {
    if (plus_cut)
      ret+="+";
    else {
      ret +="$";	
    }
  }


  //if ((superficial_form.length()>0)&&(superficial_form[superficial_form.length()-1]=='\''))
  //   //Si la forma superficial termina en apostrofo metemos un espacio en blanco tras la cadena '/$'
  //   //o '/'. De no hacerlo en la traducción aparecerán dos palabras sin blanco alguno.
  //   ret+=" "; //Quizá este no sea el sitio apropiado para hacer esto, lo suyo sería un módulo
  //             //antes del tagger o del anmor.
     
  return ret;
}

//OBSOLETE
string
TaggerWord::get_lexical_form_without_ignored_string(TTag &t, int const TAG_kEOF) {
  string ret;
   
  if(t==TAG_kEOF)
     return ret;
 
  if (lexical_forms.size()==0) { //This is an unknown word
      ret +="*^"+superficial_form;
  } else if ((*lexical_forms.begin()).second[0]=='*') {  //This is an unknown word that has been guessed
    ret += "*^"+superficial_form;
  } else {
    ret +="^"+lexical_forms[t];
  }
  
  if (ret.length() != 0) {
    if (plus_cut)
      ret+="+";
    else {
      ret +="$";	
    }
  }

  return ret;
}

void
TaggerWord::add_ignored_string(string s) {
  ignored_string += s;
}

void 
TaggerWord::set_plus_cut(const bool &c) {
  plus_cut=c;
}

bool
TaggerWord::get_plus_cut() {
  return plus_cut;
}

ostream&
operator<< (ostream& os, TaggerWord &w) {
  os<<w.get_string_tags()<<" \t Word: "<<w.get_superficial_form();
  return os;
}

void 
TaggerWord::setArrayTags(vector<string> const &at)
{
  array_tags = at;
}

void
TaggerWord::print()
{
  cout << "[#" << superficial_form << "# ";
  for(set<TTag>::iterator it=tags.begin(), limit = tags.end(); it != limit; it++)
  {
    cout << "(" << *it << " " << lexical_forms[*it] << ") ";
  }
  cout << "\b]\n";
}

void
TaggerWord::outputOriginal(FILE *output) {

  string s=superficial_form;

  map<TTag, string>::iterator it;
  for(it=lexical_forms.begin(); it!=lexical_forms.end(); it++) {
    if (it->second.length()>0)
      s+="/"+it->second;
  }

  if (s.length()>0)
    s="^"+s+"$\n";

  fprintf(output, "%s",s.c_str());
}


string 
TaggerWord::get_all_choosen_tag_first(TTag &t, int const TAG_kEOF) {
  string ret="";

  if (show_ingnored_string)
    ret+=ignored_string;
   
  if(t==TAG_kEOF)
    return ret;

  if (!previous_plus_cut)
    ret+="^";

  ret+=superficial_form;

  if (lexical_forms.size()==0) { // This is an UNKNOWN WORD
    ret +="/*"+superficial_form;
  } else {
    ret+="/"+lexical_forms[t];
    if (lexical_forms.size()>1) {
      set<TTag>::iterator it;
      for (it=tags.begin(); it!=tags.end(); it++) {
	if (*it != t) {
	  ret+="/"+lexical_forms[*it];
	}
      }
    }
  }
  
  if (ret != ignored_string) {
    if (plus_cut)
      ret+="+";
    else {
      ret +="$";	
    }
  }
     
  return ret;
}
