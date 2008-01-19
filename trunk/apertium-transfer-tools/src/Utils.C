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

#include "Utils.H"

#include <iostream>

//Delete white spaces from the end and the begining of the string
string 
Utils::trim(string str) { 
  string::iterator it;
  
  while ((str.length()>0)&&((*(it=str.begin()))==' ')) {
     str.erase(it);
  }
  
  while ((str.length()>0)&&((*(it=(str.end()-1)))==' ')) {
     str.erase(it);
  }

  return str;
}

vector<string>
Utils::split_string(const string& input, const string& delimiter) {
  int pos;
  int new_pos;
  vector<string> result;
  string s="";
  pos=0;

  while (pos<(int)input.size()) {
    new_pos=input.find(delimiter, pos);
    if(new_pos<0)
      new_pos=input.size();
    s=input.substr(pos, new_pos-pos);
    if (s.length()==0) {
      cerr<<"Warning in Utils::split_string: After splitting there is an empty string\n";
      cerr<<"Skipping this empty string\n";
    } else
      result.push_back(s);
    pos=new_pos+delimiter.size();
    //pos=new_pos+1;
  }
  return result;
}

string 
Utils::vector2string(const vector<string>& v) {
  string s="";
  for(unsigned i=0; i<v.size(); i++) {
    if (i>0)
      s+=" ";
    s+=v[i];
  }
  return s;
}

string 
Utils::substitute(const string& source, const string& olds, const string& news) {
  string s=source;

  int p=s.find(olds,0);
  while (p!=(int)string::npos) {
    s.replace(p, olds.length(), news);
    p+=news.length();
    p=s.find(olds,p);
  }

  return s;
}


string 
Utils::remove_begin_and_end_marks(string word) {
  return word.substr(1, word.size()-2);
}

string
Utils::get_lemma(string word) {
  string s="";

  int p=word.find("<",0);
  if (p!=(int)string::npos)
    s=word.substr(0, p);

  return s;
}

string 
Utils::get_lemma_without_queue(string word) {
  string l=get_lemma(word);
  string s=l;

  int p=l.find("#",0);
  if (p!=(int)string::npos)
    s=l.substr(0, p);

  return s;
}

string 
Utils::get_queue(string word) {
  string l=get_lemma(word);
  string s="";

  int p=l.find("#",0);
  if (p!=(int)string::npos)
    s=l.substr(p);

  return s;
}

string
Utils::get_tags(string word) {
  int p=word.find("<",0);
  if (p!=(int)string::npos)
    return word.substr(p, word.size()-p);
  else //Unknown word, no tags for it
    return "";
}

string
Utils::get_first_tag(string tags) {
  int p=tags.find(">",0);
  return tags.substr(0,p+1);
}

bool 
Utils::is_unknown_word(string word) {
  return ((word.length()>0)&&(word[0]=='*'));
}

string 
Utils::tags2transferformat(const string& tags) {
  string s;
  s=substitute(tags,"><",".");
  s=substitute(s,"<","");
  s=substitute(s,">","");

  return s;
}

string
Utils::itoa(int n) {
  char str[32];
  sprintf(str, "%d",n);
  return str;
}

string
Utils::ftoa(double f) {
  char str[32];
  sprintf(str, "%f",f);
  return str;
}

string 
Utils::get_tag_value(string tags, string values) {
  vector<string> pval=split_string(values,"|");

  for(unsigned i=0; i<pval.size(); i++) {
    if (tags.find(pval[i]) != string::npos)
      return pval[i];
  }

  return "";
}

bool 
Utils::case_insensitive_equal(const string& a, const string& b) {
  string alower="";
  string blower="";

  for(unsigned i=0; i<a.length(); i++) {
    alower+=tolower(a[i]);
  }

  for(unsigned i=0; i<b.length(); i++) {
    blower+=tolower(b[i]);
  }

  return (alower==blower);
}

string
Utils::strtolower(const string& s) {
  string l="";
  for(unsigned i=0; i<s.length(); i++)
    l+=tolower(s[i]);
  return l;
}

