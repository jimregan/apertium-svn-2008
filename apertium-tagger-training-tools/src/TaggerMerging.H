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

/** Class TaggerMerging (header file)
 */

#ifndef __TAGGERMERGING_H
#define __TAGGERMERGING_H

#include "TaggerMergingData.H"
#include "TaggerEval.H"

using namespace std;

class TaggerMerging {
private:
  TaggerMergingData tmd;
  TaggerEval eval;

public:
  TaggerMerging(FILE *fprob, istream& fcounts, istream& ftagclus, int corpus_length);

  ~TaggerMerging();

  void tagger (FILE *in, FILE *out);

  void eval_tagger(FILE* ftagged, FILE *funtagged);

  void print_evaluation();
};

#endif
