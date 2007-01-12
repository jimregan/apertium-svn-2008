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

#include <apertium/StringUtils.H>

#include <iostream>

//Delete white spaces from the end and the begining of the string
string 
StringUtils::trim(string str) { 
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
StringUtils::split_string(const string& input, const string& delimiter) {
  unsigned pos;
  int new_pos;
  vector<string> result;
  string s="";
  pos=0;

  while (pos<input.size()) {
    new_pos=input.find(delimiter, pos);
    if(new_pos<0)
      new_pos=input.size();
    s=input.substr(pos, new_pos-pos);
    if (s.length()==0) {
      cerr<<"Warning in StringUtils::split_string: After splitting there is an empty string\n";
      cerr<<"Skipping this empty string\n";
    } else
      result.push_back(s);
    pos=new_pos+delimiter.size();
  }

  return result;
}

string 
StringUtils::vector2string(const vector<string>& v) {
  string s="";
  for(unsigned i=0; i<v.size(); i++) {
    if (i>0)
      s+=" ";
    s+=v[i];
  }
  return s;
}

string 
StringUtils::substitute(const string& source, const string& olds, const string& news) {
  string s=source;

  unsigned p=s.find(olds,0);
  while (p!=string::npos) {
    s.replace(p, olds.length(), news);
    p+=news.length();
    p=s.find(olds,p);
  }

  return s;
}

string
StringUtils::itoa(int n) {
  char str[256];
  sprintf(str, "%d",n);
  return str;
}

string
StringUtils::ftoa(double f) {
  char str[256];
  sprintf(str, "%f",f);
  return str;
}

string
StringUtils::tolower(const string& s) {
  string l="";
  for(unsigned i=0; i<s.length(); i++)
    l+=(char)::tolower(s[i]);
  return l;
}

string
StringUtils::toupper(const string& s) {
  string l="";
  for(unsigned i=0; i<s.length(); i++)
    l+=(char)::toupper(s[i]);
  return l;
}
