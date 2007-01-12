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

#ifndef _INTERCHUNKWORD_
#define _INTERCHUNKWORD_

#include <map>
#include <regex.h>
#include <string>

using namespace std;

/**
 * Word type for transfer modules
 */
class InterchunkWord
{
private:
  /**
   * Compiled regexes, for all InterchunkWords
  */
  static map<char const *, regex_t> compiled_regexes;
  
  /**
   * Pre-calculated segments for every part referenced as regular 
   * expression
   */ 
  map<char const *, pair<int, int> > segment;

  /**
   * Target language word
   */
  string chunk;
  
  /**
   * Copy method
   * @param o the object to be copied
   */
  void copy(InterchunkWord const &o);
  
  /**
   * Destroy method
   */
  void destroy();
  
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
  InterchunkWord();
  /**
   * Destructor
   */
  ~InterchunkWord();
  
  /**
   * Copy constructor
   * @param o the object to be copied
   */
  InterchunkWord(InterchunkWord const &o);
  
  /**
   * Parametric constructor calling init()
   * @param chunk the chunk
   */
  InterchunkWord(string const &chunk);
  
  /**
   * Assignment operator
   * @param o the object to be assigned
   * @return reference to left part of assignment
   */
  InterchunkWord & operator =(InterchunkWord const &o);

  /**
   * Sets a chunk
   * @param chunk the chunk
   */
  void init(string const &chunk);
  
  /**
   * Reference a chunk part
   * @param part regular expression to match
   * @returns reference to the part of string matched
   */ 
  string chunkPart(char const *part);

  /**
   * Sets a value for a chunk part
   * @param part regular expression to match
   * @param value the new value for the given part
   */
  void setChunkPart(char const *part, string const &value);

};

#endif
