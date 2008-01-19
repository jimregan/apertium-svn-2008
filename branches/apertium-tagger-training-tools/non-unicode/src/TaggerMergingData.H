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

/** Class TaggerMergingData (header file)
 */

#ifndef __TAGGERMERGINGDATA_H
#define __TAGGERMERGINGDATA_H

#include <apertium/TaggerData.H>
#include <apertium/TTag.H>

#include <set>

using namespace std;

class TaggerMergingData : public TaggerData {
private:

  int COARSE_N; //Number of states 
  int COARSE_M; //Number of observable outputs (ambiguity clases)

  Collection coarse_output; // Collection of ambiguity classes

  double **coarse_a, **coarse_b;  // Transition (a) and emission (b) matrices

  //Counts collected during the training
  

  map<int, int> fine2coarse; //Map the fine tags into the coarse one
  map<int, int> fineamb2coarseamb; //Map the fine amb. classes into
				   //the coarse one

  vector<set<int> > coarse2fine; //Given a coarse tag returns the set
  //of fine tags that it holds

  vector<set<int> > coarseamb2fineamb; //Given a coarse amb. class it
				       //returns the set of fine amb
				       //class  that it holds

  void initialize_parameters();

  void read_counts(istream& fcounts, 
		   map<int, map<int, double> >& tags_pair,
		   map<int, map<int, double> >& emis,
		   map<int, double>& tags_count,
		   map<int, double>& ambclass_count,
		   map<int, double>& tags_count_for_emis);
public:
  TaggerMergingData();

  ~TaggerMergingData();

  Collection& getOutput(); 

  map<int, int>& getFine2Coarse();

  double ** getA(); 

  double ** getB(); 

  int getN(); 

  int getM(); 

  int getEosTag(); //end-of-sentence coarse tag

  int getFineEofTag(); //end-of-file fine-grained tag

  int getUndefTag(); //kUNDEF coarse tag, asigned to unknown words

  int getFineUndefTag(); //kUNDEF fine-grained tag, asigned to unknown words

  void read_tagsclusters(istream& ftagsclus);

  void calculate_parameters_merging(istream& fcounts, int corpus_length);
};

#endif
