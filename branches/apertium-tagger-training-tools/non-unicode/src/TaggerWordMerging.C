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

/** Class TaggerWordMerging (source file)
 */

#include "TaggerWordMerging.H"

#include <apertium/TaggerUtils.H>

TaggerWordMerging::TaggerWordMerging(const TaggerWord& w):TaggerWord(w) {
}

TaggerWordMerging::TaggerWordMerging(const TaggerWordMerging& w):TaggerWord(w) {
  coarse2fine=w.coarse2fine;
  coarse_tags=w.coarse_tags;
}

TaggerWordMerging::~TaggerWordMerging() {
}

void 
TaggerWordMerging::add_tag(TTag &t, const string &lf, vector<string> const &prefer_rules) {
  cerr<<"Error in TaggerWordMerging::add_tag(TTag &t, const string &lf, vector<string> const &prefer_rules)\n";
  cerr<<"This method is not implemented and cannot be used\n";
  exit(EXIT_FAILURE);
}

void 
TaggerWordMerging::add_tag(int coarsetag, int finetag) {
  coarse2fine[coarsetag]=finetag;
  coarse_tags.insert(coarsetag);
}

set<TTag>& 
TaggerWordMerging::get_tags() {
  return coarse_tags;
}

string
TaggerWordMerging::get_string_tags() {
  string st;
  set<TTag>::iterator itag;
  
  st="{";  
  for(itag=coarse_tags.begin(); itag!=coarse_tags.end(); itag++) {
    if (itag!=coarse_tags.begin())
      st+=",";
    st+=itoa(*itag);
  }
  st += "}";  
  
  return st;  
}

string 
TaggerWordMerging::get_lexical_form(TTag &t, int const TAG_kEOF) {
  return TaggerWord::get_lexical_form(coarse2fine[t], TAG_kEOF);
}

bool 
TaggerWordMerging::is_unknown() {
  return (TaggerWord::get_tags().size()==0);
}
