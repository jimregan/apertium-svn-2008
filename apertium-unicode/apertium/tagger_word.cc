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
#include <apertium/tagger_word.h>

vector<wstring> TaggerWord::array_tags;
bool TaggerWord::show_ingnored_string=true;

TaggerWord::TaggerWord(bool prev_plus_cut){
   ignored_string = L"";
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
TaggerWord::set_superficial_form(const wstring &sf){
  superficial_form = sf;
}

wstring& 
TaggerWord::get_superficial_form() {
  return superficial_form;
}

void
TaggerWord::add_tag(TTag &t, const wstring &lf, vector<wstring> const &prefer_rules){

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

wstring
TaggerWord::get_string_tags() {
  wstring st;
  set<TTag>::iterator itag;
  
  st=L"{";  
  for(itag=tags.begin(); itag!=tags.end(); itag++) {
    if (itag!=tags.begin())
      st+=L',';
    st+=array_tags[*itag];
  }
  st += L'}';  
  
  return st;  
}

wstring
TaggerWord::get_lexical_form(TTag &t, int const TAG_kEOF) {
  wstring ret= L"";

  if (show_ingnored_string)
    ret.append(ignored_string);
   
  if(t==TAG_kEOF)
    return ret;

  if (!previous_plus_cut)
    ret+=L'^'; 

  if (lexical_forms.size()==0) { // This is an UNKNOWN WORD
    ret +=L'*';
    ret.append(superficial_form);
  } else if ((*lexical_forms.begin()).second[0]==L'*') { //This is an
							//unknown word
							//that has
							//been guessed
    ret += L'*';
    ret.append(superficial_form);
  } else if (lexical_forms.size()>1) {  //This is an ambiguous word
    ret.append(lexical_forms[t]);
  } else {
    ret.append(lexical_forms[t]);
  }
  
  if (ret != ignored_string) {
    if (plus_cut)
      ret+=L'+';
    else {
      ret += L'$';	
    }
  }


  //if ((superficial_form.length()>0)&&(superficial_form[superficial_form.length()-1]=='\''))
  //   //Si la forma superficial termina en apostrofo metemos un espacio en blanco tras la cadena '/$'
  //   //o '/'. De no hacerlo en la traducci�n aparecer�n dos palabras sin blanco alguno.
  //   ret+=" "; //Quiz� este no sea el sitio apropiado para hacer esto, lo suyo ser�a un m�dulo
  //             //antes del tagger o del anmor.
     
  return ret;
}

wstring 
TaggerWord::get_all_choosen_tag_first(TTag &t, int const TAG_kEOF) {
  wstring ret=L"";

  if (show_ingnored_string)
    ret.append(ignored_string);
   
  if(t==TAG_kEOF)
    return ret;
 
  if (!previous_plus_cut)
    ret+=L"^";
 
  ret.append(superficial_form);
 
  if (lexical_forms.size()==0) { // This is an UNKNOWN WORD
    ret+=L"/*";
    ret.append(superficial_form);
  } else {
    ret+=L"/";
    ret.append(lexical_forms[t]);
    if (lexical_forms.size()>1) {
      set<TTag>::iterator it;
      for (it=tags.begin(); it!=tags.end(); it++) {
	if (*it != t) {
	  ret+=L"/";
          ret.append(lexical_forms[*it]);
	}
      }
    }
  }
   
  if (ret != ignored_string) {
    if (plus_cut)
      ret+=L"+";
    else {
      ret+=L"$";
    }
  }
      
  return ret;
}

//OBSOLETE
wstring
TaggerWord::get_lexical_form_without_ignored_string(TTag &t, int const TAG_kEOF) {
  wstring ret;
   
  if(t==TAG_kEOF)
     return ret;
 
  if (lexical_forms.size()==0) { //This is an unknown word
      ret.append(L"*^");
      ret.append(superficial_form);
  } else if ((*lexical_forms.begin()).second[0]=='*') {  //This is an unknown word that has been guessed
    ret.append(L"*^");
    ret.append(superficial_form);
  } else {
    ret += L'^';
    ret.append(lexical_forms[t]);
  }
  
  if (ret.length() != 0) {
    if (plus_cut)
      ret+=L'+';
    else {
      ret +=L'$';	
    }
  }

  return ret;
}

void
TaggerWord::add_ignored_string(wstring const &s) {
  ignored_string.append(s);
}

void 
TaggerWord::set_plus_cut(const bool &c) {
  plus_cut=c;
}

bool
TaggerWord::get_plus_cut() {
  return plus_cut;
}

wostream&
operator<< (wostream& os, TaggerWord &w) {
  os<<w.get_string_tags()<< L" \t Word: " << w.get_superficial_form();
  return os;
}

void 
TaggerWord::setArrayTags(vector<wstring> const &at)
{
  array_tags = at;
}

void
TaggerWord::print()
{
  wcout << L"[#" << superficial_form << L"# ";
  for(set<TTag>::iterator it=tags.begin(), limit = tags.end(); it != limit; it++)
  {
    wcout << L"(" << *it << L" " << lexical_forms[*it] << L") ";
  }
  wcout << L"\b]\n";
}

void
TaggerWord::outputOriginal(FILE *output) {

  wstring s=superficial_form;

  map<TTag, wstring>::iterator it;
  for(it=lexical_forms.begin(); it!=lexical_forms.end(); it++) {
    if (it->second.length()>0)
    {
      s+=L'/';
      s.append(it->second);
    }
  }

  if (s.length()>0)
  {
    s=L"^"+s+L"$\n";
  }

  fputws_unlocked(s.c_str(), output);
}
