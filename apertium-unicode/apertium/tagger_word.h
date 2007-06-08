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
#ifndef __TAGGERWORD_H
#define __TAGGERWORD_H

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include <apertium/ttag.h>

using namespace std;

/** Class TaggerWord.
 *  It stores the superficial form and all possible tags that it can receive.
 *  It has the fine tags delivered by the morphological analyzer and the coarse
 *  ones used by the PoS tagger.
 */  
class TaggerWord{
private:
  wstring superficial_form; 
  
  set<TTag> tags;  //Set of al possible tags
  map<TTag, wstring> lexical_forms;  //For a give corase tag it stores the fine tag 
                                    //delevered by the morphological analyzer
  wstring ignored_string;
  
  bool plus_cut; //Flag to distinguish the way in which the word was ended.
                  //If it was done by '$' its value should be false
                  //If it was done by '+' its value should be true
  bool previous_plus_cut; //Flag to distinguish the way in which thre
			  //previous word was ended. It has the same
			  //plus_cut meaning

public:
  static vector<wstring> array_tags;

  static bool show_ingnored_string;

   /** 
    * Constructor 
    */
   TaggerWord(bool prev_plus_cut=false);
  
   /** 
    * Copy constructor
    */
   TaggerWord(const TaggerWord &w);
  
   /** 
    * Destructor 
    */
   ~TaggerWord();
  
   /** Set the superficial form of the word.
    *  @param s the superfial form
    */
   void set_superficial_form(const wstring &s);
  
   /** Get the superficla form of the word
    *
    */
   wstring& get_superficial_form();
  
   /** Add a new tag to the set of all possible tags of the word.
    *  @param t the coarse tag
    *  @param lf the lexical form (fine tag)
    */
   void add_tag(TTag &t, const wstring &lf, vector<wstring> const &prefer_rules);

   /** Get the set of tags of this word.
    *  @return  set of tags.
    */  
   set<TTag>& get_tags();
  
   /** Get a wstring with the set of tags
    */
   wstring get_string_tags();
   
  /** Get the lexical form (fine tag) for a given tag (coarse one)
   *  @param  t the tag
   *  @return the lexical form of tag t
   */
  wstring get_lexical_form(TTag &t, int const TAG_kEOF); 
  
  /** Get the lexical form (fine tag) for a given tag (coarse one)
   *  @param  t the tag
   *  @return the lexical form of tag t without other text that
   *          is ignored.
   */  
  wstring get_lexical_form_without_ignored_string(TTag &t, int const TAG_kEOF); 
  
  /** Add text to the ignored string
   *
   */   
  void add_ignored_string(wstring const &s);
  
  /** Set the flag plus_cut to a certain value. If this flag is set to true means
   *  that there were a '+' between this word and the next one 
   */
   void set_plus_cut(const bool &c);

  /** Get the value of the plus_cut flag */
  bool get_plus_cut();
   
  /** Output operator
   */
  friend wostream& operator<< (wostream& os, TaggerWord &w);
  
  static void setArrayTags(vector<wstring> const &at);

  void print();
  
  void outputOriginal(FILE *output);
};

#endif
