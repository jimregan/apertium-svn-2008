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
 * Class Segment. It define a text segment  (header file)
 *
 *  @author   Felipe Sánchez-Martínez - fsanchez@dlsi.ua.es
 */

#ifndef __SEGMENT_H
#define __SEGMENT_H

#include <set>
#include <map>
#include <iostream>
#include <string>
#include <vector>

#include <apertium/tagger_word.h>
#include <apertium/tagger_data.h>
#include <apertium/morpho_stream.h>
#include <apertium/tagger_utils.h>

#include "StringCollection.H"
#include "TransferRules.H"

#define TAGGER_WORD_BUFFER_SIZE 100

using namespace std;

class Segment  {
private:
  static map<wstring, TTag, Ltstr> tag_index;

  int npaths;

  vector< vector<TTag> > contador_caminos; //Vector con las etiquetas de cada nodo del segmento

  vector<int> nfijo_caminos;  //Vector con el nº veces que debe usarse
			      //un etiqueta antes de pasar a la siguiente
public:

  vector<TaggerWord> vwords;

  vector<TaggerWord> vwords_before;
  vector<TaggerWord> vwords_after;
  
  Segment();

  static Segment* new_segment(MorphoStream &ms, TransferRules* tr, TaggerData &td);

  int get_number_paths();

  static void set_tag_index(map<wstring, TTag, Ltstr> ti);
   
  wstring get_path(vector <TTag>& etqpart, int path);                
};

#endif
