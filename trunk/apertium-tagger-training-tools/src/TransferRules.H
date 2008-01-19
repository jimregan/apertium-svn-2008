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
/**
 * Class TransferRules. (header file)
 *
 *  @author   Felipe Sánchez-Martínez - fsanchez@dlsi.ua.es
 */

#ifndef __TRANSFERRULES_H
#define __TRANSFERRULES_H

#include <iostream>
#include <string>
#include <vector>
#include <deque>

#include <sys/types.h>
#include <regex.h>

#include <apertium/apertium_re.h>
#include <apertium/tagger_word.h>

//#define LEMMA "([-0-9a-zñçáéíóúàèìòùäëïöüâêîôûA-ZÑÇ_ÁÉÍÓÚÀÈÌÒÙÄËÏÖÜÂÊÎÔÛãõÃÕ\\._'·,]+)"
//#define LEMMA "([-[:alnum:]\\._'·,; ]+)"
//#define LEMMA   "([-[:alnum:]\\._'Â·,; ]+)"

#define LEMMA "([^<])+"
#define TAG   "(<[a-zA-Z0-9]+>)"
#define TAGS  "("TAG"*)"

using namespace std;

class TransferRules {
private:
  vector<string> regular_expressions;
  vector<ApertiumRE*> compiled_regular_expressions;
  vector<wstring> superficial_forms;
public:
  vector<vector<int> > transfer_rules;

  TransferRules();

  ~TransferRules();

  void read_rules_from_file(string s);

  void compile_regular_expressions();

  //vector<int> match_regular_expressions(string s);

  bool match_regular_expression(wstring s, int pos_regex);

  bool is_segmentation_point(int tag_eof, deque<TaggerWord>& vwords, int position, int &advance);

  void set_superficial_forms(wstring s);

  int get_number_regular_expressions();
};

#endif
