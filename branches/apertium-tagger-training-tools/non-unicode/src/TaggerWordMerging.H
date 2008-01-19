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

/** Class TaggerWordMerging (header file)
 */

#ifndef __TAGGERWORDMERGING_H
#define __TAGGERWORDMERGING_H

#include <apertium/TaggerWord.H>

using namespace std;

class TaggerWordMerging : public TaggerWord {
private:

  map<int, int> coarse2fine; //Given a coarse tag, it returns the fine tag it comes from
  set<TTag> coarse_tags;
public:
  TaggerWordMerging(const TaggerWord& w);

  TaggerWordMerging(const TaggerWordMerging& w);

  ~TaggerWordMerging();

  void add_tag(TTag &t, const string &lf, vector<string> const &prefer_rules);

  void add_tag(int coarsetag, int finetag);

  set<TTag>& get_tags();

  string get_string_tags();

  string get_lexical_form(TTag &t, int const TAG_kEOF); 

  bool is_unknown();
};

#endif
