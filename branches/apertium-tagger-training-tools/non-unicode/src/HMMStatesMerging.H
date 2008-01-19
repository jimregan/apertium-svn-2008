/*
 * Copyright (C) 2007 Felipe Sánchez-Martínez
 * 
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
/**
 * Class HMMStatesMerging  (header file)
 *
 *  @author   Felipe Sánchez-Martínez - fsanchez@dlsi.ua.es
 */

#ifndef __HMMSTATESMERGING_H
#define __HMMSTATESMERGING_H

#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <string>
#include <vector>
#include <cmath> 
#include <cfloat>
#include <cstdio>

//#include <apertium/TTag.H>
#include <apertium/TaggerData.H>
#include <apertium/Collection.H>

#include "HashMap.H"

using namespace std;

#define BIG_NUMBER 1000000.0
#define INCOMPATIBLE -100.0

/** Class HMMStatesMerging
 */
class HMMStatesMerging {
private:
  TaggerData tagger_data;

  string divergences_file;

  double threshold;

  bool restrictive;

  //Matrix to store the calculated divergences
  HashIntMap<HashIntMap<double> > divergences; 

  void calculate_all_divergences();

  double clusters_distance(set<int> cl1, set<int> cl2);

  double& divergence(int i, int j);

  //void divergence(int i, int j, double d);

  /** Calculate the divergence between the two probabilistic
   *  dritibutions.
   *  @params state_a and state_b are number of states
   */  
  double calculate_divergence(int state_a, int state_b);

  /** Calculate the Kullback-Libler distance between the two probabilistic
   *  dritibutions.
   *  @params state_a and state_b are number of states
   */  
  double kullback_leibler(int state_a, int state_b);

  /** Returns the set of ambiguity classes the states belongs to
   */
  set<int> ambiguity_classes_state_appears(int state);

  /** Returns true if both states can be merged because all
   *  constraints are met
   */
  bool sastify_merge_constraints(int state_a, int state_b);

public:
  HMMStatesMerging(FILE* fprob, const string& divfile, bool restrict);

  /** Returns a vector of sets. Each state belonging to the same set
   *  are supposed to be merged. To find each set of states to be
   *  merged the bottom-up aglomerative clustering algorithm is used
   */
  vector<set<int> > bottom_up_aglomerative(double threshold, string clustersfile="");

  /** Given the states to be merged, the method calculate which
   *  ambiguity classes are also merged as a consequence of the states
   *  being merged. In new output the new ambiguity classes (of coarse
   *  tags) are returned
   */
  vector<set<int> > calculate_merged_ambiguity_classes(vector<set<int> > merged_states,  
                                                       Collection& new_output);
};

#endif
