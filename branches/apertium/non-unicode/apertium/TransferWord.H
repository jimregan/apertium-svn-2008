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

#ifndef _TRANSFERWORD_
#define _TRANSFERWORD_

#include <map>
#include <regex.h>
#include <string>

using namespace std;

/**
 * Word type for transfer modules
 */
class TransferWord
{
private:
  /**
   * Compiled regexes, for all TransferWords
  */
  static map<char const *, regex_t> compiled_regexes;
  
  /**
   * Source language part of word, for every part referenced as regular 
   * expression
   */ 
  map<char const *, pair<int, int> > s;

  /**
   * Target language part of word, for every part referenced as regular 
   * expression
   */ 
  map<char const *, pair<int, int> > t;

  /**
   * Source language part of word, for every part referenced as regular 
   * expression (without queue version)
   */ 
  map<char const *, pair<int, int> > snq;

  /**
   * Target language part of word, for every part referenced as regular 
   * expression (without queue version)
   */ 
  map<char const *, pair<int, int> > tnq;

  
  /**
   * Source language word
   */
  string s_str;
  
  /**
   * Target language word
   */
  string t_str;
  
  /**
   * Queue length
   */
  int queue_length;

  /**
   * Copy method
   * @param o the object to be copied
   */
  void copy(TransferWord const &o);
  
  /**
   * Destroy method
   */
  void destroy();
  
  /**
   * Accesses the source/target side of a word using the specified part
   * @param m tipically s or t
   * @param str tipically s_str or t_str
   * @param part regular expression to match/access
   * @return reference to matched/accessed string
   */
  string access(map<char const *, pair<int, int> > &m, 
                string const &str, char const *part);

  /**
   * Assings a value to the source/target side of a word using the specified part
   * @param m tipically s or t
   * @param str tipically s_str or t_str
   * @param part regular expression to match/access
   * @value the string to be assigned
   */
  void assign(map<char const *, pair<int, int> > &m, string &str, char const *part, string const &value);

  /**
   * Get a regex_t from a regular expression
   * @param part the regular expression
   * @return the regex_t of the compiled regular expression;
   */
  static regex_t & getRegex(const char *part);
  
  /**
   * Error function compiling regular expressions
   * @param message error message
   * @param errorcode return code of regcomp call
   */
  static void regexError(char const *message, int const errorcode);  
public:
  /**
   * Non-parametric constructor
   */
  TransferWord();
  /**
   * Destructor
   */
  ~TransferWord();
  
  /**
   * Copy constructor
   * @param o the object to be copied
   */
  TransferWord(TransferWord const &o);
  
  /**
   * Parametric constructor calling init()
   * @param src source word
   * @param tgt target word
   * @param queue queue lenght
   */
  TransferWord(string const &src, string const &tgt, int queue = 0);
  
  /**
   * Assignment operator
   * @param o the object to be assigned
   * @return reference to left part of assignment
   */
  TransferWord & operator =(TransferWord const &o);

  /**
   * Sets a bi-word (a source language word and its counterpart in target
   * language
   * @param src source word
   * @param tgt target word
   */
  void init(string const &src, string const &tgt);
  
  /**
   * Reference a source language word part
   * @param part regular expression to match
   * @param with_queue access taking into account the queue
   * @returns reference to the part of string matched
   */ 
  string source(char const *part, bool with_queue = true);

  /**
   * Reference a target language word part
   * @param part regular expression to match
   * @param with_queue access taking into account the queue
   * @returns reference to the part of string matched
   */ 
  string target(char const *part, bool with_queue = true);

  /**
   * Sets a value for a source language word part
   * @param part regular expression to match
   * @param value the new value for the given part
   * @param with_queue access taking or not into account the queue
   */
  void setSource(char const *part, string const &value, bool with_queue = true);

  /**
   * Sets a value for a target language word part
   * @param part regular expression to match
   * @param value the new value for the given part
   * @param with_queue access taking or not into account the queue
   */
  void setTarget(char const *part, string const &value, bool with_queue = true);

  /**
   * Resets the cache of matchings for this TransferWord
   */
  void clearCache();
};

#endif
