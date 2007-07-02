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
 * Implementation of the method that trains a HMM-based part-of-speech
 * tagger using information from the target language (TL) by means of
 * the apertium MT system. (header file)
 *
 * For a deeper description on how the method works read the paper:
 *
 * Exploring the use of target-language information to train the
 * part-of-speech tagger of machine translation systems.  By Felipe
 * Sánchez-Martínez, Juan Antonio Pérez-Ortiz and Mikel L. Forcada.
 * In Lecture Notes in Computer Science 3230 (Advances in Natural
 * Language Processing, Proceedings of EsTAL - España for Natural
 * Language Processing), p. 137-148, October 20-22, 2004, Alicante,
 * Spain.  
 * © Springer-Verlag Berling Heidelberg 2004
 * http://www.dlsi.ua.es/~fsanchez/pub/pdf/sanchez04b.pdf
 *
 *  @author   Felipe Sánchez-Martínez - fsanchez@dlsi.ua.es
 */

#ifndef __HMM_TL_DRIVEN_TRAINER_H
#define __HMM_TL_DRIVEN_TRAINER_H

#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <fstream>

#include <apertium/Collection.H>
#include <apertium/ConstantManager.H>
#include <apertium/MorphoStream.H>
#include <apertium/TaggerData.H>
#include <apertium/TaggerUtils.H>
#include <apertium/TaggerWord.H>
#include <apertium/HMM.H>
#include <apertium/TTag.H>

#include "Segment.H"
#include "Translations.H"
#include "TransferRules.H"

#define MAX_PATH_PER_SEGMENT 100000

#define TL1 0
#define TL2 1

using namespace std;


class HMM_TL_driven_trainer {
private:

  TaggerData tagger_data;

  //TaggerData of an initial model, used when mixing parameters
  TaggerData tagger_data_initial_model; 

  bool use_tags_rules;

  HMM* hmm;

  map<int, map<int, bool> > allowed_bigrams; //Matrix (NxN) with all the
				  	     //bigrams, this matrix is
					     //initialized after calling
					     //int_allowed_bigrams.

  TransferRules* transfer_rules;

  void init_allowed_bigrams();

  void update_counts(Segment* seg, vector<Translations*> &trans, 
                     map<int, map<int, double> > &tags_pair,
                     map<int, map<int, double> > &emis, TTag last_tag_prev_segment,
                     map<int, double> &tags_count, 
                     map<int, double> &ambclass_count,
		     map<int, double> &tags_count_for_emis);

  bool is_feasible_path(const TTag& last_etq_ant, const vector<TTag>& etqpart);

  void calculate_parameters(map<int, map<int, double> > &tags_pairs, map<int, map<int, double> > &emis);

  void mix_parameters(double words_processed, double corpus_length, double mixing_c);

public:  

  /** Constructor
   */
  HMM_TL_driven_trainer(string tsxfile, TransferRules* tr);

  /** Destructor
   */
  ~HMM_TL_driven_trainer();

  /** Set the flag use_tags_rules to a desired value. If false, the
   *  information that comes from forbidden and enforce rules will no be
   *  used
   */
  void set_use_tags_rules(bool b);

  void read_dictionary(FILE *fdic);

  void read_parameters(FILE *in);

  void write_parameters(FILE *out);

  void train(FILE *is, int corpus_length, int save_after_nwords, string filename, ofstream& fpaths, ifstream& ftrans, ifstream& flike);

  void train_pruning(FILE *is, int corpus_length, int save_after_nwords, string filename, double mixing_c, ifstream& ftrans, ifstream& flike);

  void print_A();

  void print_B();
};

#endif
