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
#ifndef __LEXTORDATA_H
#define __LEXTORDATA_H

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <cmath>

#include <lttoolbox/FSTProcessor.H>

#define WORD_DATA_TYPE unsigned short
#define MAX_WORD_INDEX (pow(2.0,(double)(sizeof(WORD_DATA_TYPE)*8))-1)

#define COUNT_DATA_TYPE float

#define NULLWORD "NULLWORD"

using namespace std;

/** Class LexTorData. (Lexical Selector Data class)
 */  

class LexTorData{
private:

  WORD_DATA_TYPE n_stopwords;
  WORD_DATA_TYPE n_words;
  WORD_DATA_TYPE n_words_per_set;
  WORD_DATA_TYPE n_set;

  //For a give word (or lexical choice) its index is returned and vice versa
  map<string, WORD_DATA_TYPE> word2index;
  vector<string> index2word;

  map<WORD_DATA_TYPE, COUNT_DATA_TYPE> wordcount;

  //For a given lexical choice it contains the set of words it appears
  //with, and for each co-appearing word, the number of times they
  //co-appear
  map<WORD_DATA_TYPE, map<WORD_DATA_TYPE, COUNT_DATA_TYPE> > lexchoice_set;

  //For a given lexical choice it contains the sum of all co-appearing words
  map<WORD_DATA_TYPE, COUNT_DATA_TYPE> lexchoice_sum;

  //For a given lexical choice it contains its probability  
  //map<WORD_DATA_TYPE, double> lexchoice_prob;

  //Set of stopwords
  set<string> stopwords;

  //Set of words to work with
  set<string> words;

  //For a given word it contains its set of lexical-choices (when available)
  map<string, set<string> > lexical_choices;
  
  set<string> reduced_lexical_choices;

  void new_word_register(const string& w);
public:

  LexTorData();
  
  LexTorData(const LexTorData& ltd);
  
  ~LexTorData();

  COUNT_DATA_TYPE vote_from_word(const string& lexical_choice, const string& word);

  //double get_lexchoice_prob(const string& lexical_choice);

  COUNT_DATA_TYPE get_lexchoice_sum(const string& lexical_choice);

  void set_wordcount(const string& word, COUNT_DATA_TYPE c);
  COUNT_DATA_TYPE get_wordcount(const string& word);

  void set_lexchoice_sum(const string& lexical_choice, COUNT_DATA_TYPE sum);

  bool is_stopword(const string& word);

  void read(istream& is);

  void write(ostream& os);

  void read_stopwords(istream& is);

  void read_words(istream& is);

  void read_lexical_choices(FSTProcessor& fstp);

  void set_nwords_per_set(int i);

  void set_cooccurrence_context(const string& lexical_choice, 
                                const vector<pair<string, COUNT_DATA_TYPE> >& context);

  //vector<pair<WORD_DATA_TYPE, double> >
  //get_cooccurrence_vector(const string& lexical_choice);
  double get_module_lexchoice_vector(const string& lexical_choice);

  double cosine(const string& reduced_lexch1, const string& reduced_lexch2);

  set<string> get_words();

  set<string> get_lexical_choices(const string& word);

  //Used to ensure that none of the stopwords are in the set 
  //of words from which co-occurrence models are being estimated
  void ensure_stopwords_ok();

  //Given a word in the apertium format  the lemma and the fisrt tag
  //are returned (both in lower case) if possible
  string reduce(const string& s);

  string reduce_lexical_choice(const string& s);
};

#endif
