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

#ifndef __PATHS_PRUNER_H
#define __PATHS_PRUNER_H

#include <vector>
#include <apertium/tagger_data.h>
#include <apertium/ttag.h>
#include "Segment.H"

using namespace std;

class PathsPruner {
private:
  static TaggerData tagger_data;

  Segment *seg;
  TTag last_tag_previous_segment;

  int k; //Number of paths tha will be taken into account, the k most
	 //promissing paths

  int paths_counter;

  double probmass_considered;

  vector<pair<int, double> > prob_path; 

  /** Using data from tagger_data it returns the likelihood of the
   *  given path.
   */
  double a_priori_likelihood(int path);

public:
  static int mode;
  static int latency;
  static double probmass;
  //static double epsilon;
  //static double delta;
  static int corpus_length; //Use to scale each threshold function

  PathsPruner(Segment *s, TTag last_tag);

  static void set_tagger_data(const TaggerData& td);

  static double calculate_probmass_to_take_into_account(double nwords_processed);
  
  void reset_paths_counter();

  /** Returns the path number of the next most promising paths 
   *  The paths counter is incremented.
   *  It returns -1 when no more paths are availables,
   *  i. e. paths_counter is greater or equal to k.
   */
  int get_next_path();

  int get_number_considered_paths();

  double get_probmass_considered();

  void compute_paths_ranking();
};

#endif
