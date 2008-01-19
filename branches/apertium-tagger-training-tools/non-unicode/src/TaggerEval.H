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

#ifndef __TAGGER_EVAL_H
#define __TAGGER_EVAL_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include <fstream>

#include <apertium/Collection.H>
#include <apertium/ConstantManager.H>
#include <apertium/MorphoStream.H>
#include <apertium/TaggerData.H>
#include <apertium/TaggerWord.H>
#include <apertium/TTag.H>

#include "TaggerMergingData.H"
#include "TaggerWordMerging.H"
#include "MorphoStreamMerging.H"

using namespace std;

class TaggerEval {
private:

  TaggerData tagger_data;

  double nwords;
  double nunknown;
  double nignored;
  double nambiguous;
  double nerrors_noamb;
  double nerrors_amb;
  double nerrors_unk;

  TTag eos;

  int evalword(TaggerWord& word, TTag& tag, MorphoStream& ms);  
public:  
 
  TaggerEval();

  TaggerEval(FILE* fprob);

  ~TaggerEval();

  void eval_tagger(FILE* ftagged, FILE *funtagged);

  int evalwordmerging(TaggerMergingData& tmd, TaggerWordMerging& word, TTag& tag, MorphoStreamMerging& ms);

  void print_evaluation();
};

#endif
