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
 * Utility functions. (header file)
 *
 *  @author   Felipe Sánchez-Martínez - fsanchez@dlsi.ua.es
 */

#ifndef __SMOOTHUTILS_H
#define __SMOOTHUTILS_H

#include <map>
#include <apertium/TaggerData.H>

using namespace std;

class SmoothUtils {
private:
  double static lambda(double count);
  map<int, double> static prob_amb_class_from_tag(TaggerData& tagger_data, 
						  int i, 
						  map<int, double> &prob_ambclass);

public:
  void static calculate_smoothed_parameters(TaggerData& tagger_data,
					    map<int, double> &tags_count, 
					    map<int, map<int, double> > &tags_pairs, 
					    map<int, double> &ambclass_count, 
					    map<int, map<int, double> > &emis, 
					    map<int, double> &tags_count_for_emis,
					    double corpus_length);

};

#endif

