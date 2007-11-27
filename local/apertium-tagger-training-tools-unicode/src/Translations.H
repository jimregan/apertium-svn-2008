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
 * Class Translations (header file)
 *
 *  @author   Felipe Sánchez-Martínez - fsanchez@dlsi.ua.es
 */

#ifndef __TRANSLATIONS_H
#define __TRANSLATIONS_H

#include <map>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include "StringCollection.H"
#include "Segment.H"
#include "TransferRules.H"
#include <apertium/apertium_re.h>

#define TRANSLATION_UNIQUE_PATH L"___UNIQUE___"
#define TRANSLATION_NOT_USED L"___NOTUSED___"

using namespace std;

class Translations  {
private:
  map<int, int> path_translation; //For a given path g_i it stores the
				  //index of its translation tau(g_i,s) in the
				  //StringCollection translations

  map<int, double>  path_weigh; //For a given path g_i, it stores the
				//weigh p(g_i | tau(g_i,s)), i.e. the
				//contribution of g_i to the
				//translation tau(g_i,s)

  map<int, double> path_prob; //For a given path g_i it stores the final
			      //probability of g_i, i.e.
			      //p(g_i|s,M_tag)

  map<int, double> translation_likelihood; 

  StringCollection translations;

  Segment* segment;

  wstring get_translation_at(int pos);
  wstring get_path_translation(int path);
  int get_index_path_translation(int path);  
  void set_path_weigh(int path, double weigh);
  double get_path_weigh(int path);
  void set_translation_likelihood(int index_trans, double likelihood);
  double get_translation_likelihood(int index_trans);
  vector<int> get_paths_same_translation(int index_trans);

  bool all_translations_ok;
  //regex_t regex_wrongly_translated_word;
  ApertiumRE regex_wrongly_translated_word;
public:
  Translations(Segment* seg);

  ~Translations();

  int get_number_translations();

  void set_path_translation(wstring trans, int path);

  void evaluate_translations_likelihood(string likelihood_script);
  void set_previoulsy_evaluated_translations_likelihood(map<wstring, double>& translations_likelihoods);

  void calculate_probability_each_path();

  double get_path_probability(int path);

  bool are_translations_ok();
};

#endif
