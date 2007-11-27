/*
 * Copyright (C) 2005 Universitat d'Alacant / Universidad de Alicante
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
 *  Word class and MorphoStream class definitions
 *
 *  @author	Felipe Sánchez-Martínez 
 */

#ifndef __MORPHOSTREAM2_H
#define __MORPHOSTREAM2_H

#include <apertium/ConstantManager.H>
#include <lttoolbox/MatchExe.H>
#include <lttoolbox/MatchState.H>
#include <apertium/TaggerData.H>
#include <apertium/TaggerWord.H>

#include <cstdio>
#include <deque>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

/** Class MorphoStream.  
 *  This class process the ouput of class  yyFlexLexer (lex.yy.cc), and 
 *  build the TaggerWord objects managed by the tagger 
 */
class MorphoStream {
private:
  bool foundEOF;
  string last_string_tag;
  bool debug;
  FILE *input;
  int ca_any_char;
  int ca_any_tag;
  int ca_kignorar;
  int ca_kbarra;
  int ca_kdollar;
  int ca_kbegin;
  int ca_kmot;
  int ca_kmas;
  int ca_kunknown;
  int ca_tag_keof;
  int ca_tag_kundef;

  vector<TaggerWord *> vwords; //Vector used to implement a buffer
                             //to treat ambiguous multiword units
  string yytext;

  MatchExe *me;
  TaggerData *td;
  Alphabet alphabet;
  MatchState ms;

  void readRestOfWord(int &ivwords);
  void lrlmClassify(string const &str, int &ivwords);
public:

   /** Constructor 
    *  @param is the input stream.
    */
   MorphoStream(FILE *ftxt, bool d, TaggerData *t);
  
   /** 
    *  Destructor 
    */
   ~MorphoStream();
  
   /** Get next word in the input strem
    *  @return  A pointer to the next word in the input stream 
    */
   TaggerWord* get_next_word();  
};

#endif
