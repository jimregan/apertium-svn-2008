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
#ifndef _ALPHABET_
#define _ALPHABET_

#include <cstdio>
#include <list>
#include <map>
#include <vector>
#include <string>

#include <lttoolbox/Ltstr.H>

using namespace std;

/**
 * Alphabet class.
 * Encodes pairs of symbols into an integer.
 */
class Alphabet
{
private:
  /**
   * Symbol-identifier relationship.
   * @see slexicinv
   */
  map<string, int, Ltstr> slexic;  

  /**
   * Identifier-symbol relationship.
   * @see slexic
   */
  vector<string> slexicinv;
  
  void copy(Alphabet const &a);
  void destroy();
public:

  /**
   * Constructor.
   */
  Alphabet();
  
  /**
   * Destructor.
   */
  ~Alphabet();
  
  /**
   * Copy constructor.
   */
  Alphabet(Alphabet const &a);
  
  /**
   * Assign operator.
   */
  Alphabet & operator = (Alphabet const &a);

  /**
   * Include a symbol into the alphabet.
   */
  void includeSymbol(string const &s);  
  
  /**
   * Get an unique code for every symbol pair.  This flavour is for string
   * pairs.
   * @param s1 left symbol
   * @param s2 right symbol
   * @return code for (s1,s2)
   */
  int operator()(string const &s1, string const &s2);
 
  /**
   * Get an unique code for every symbol pair.  This flavour is for 
   * (string, character) pairs.
   * @param s1 left symbol.
   * @param c2 right symbol.
   * @return code for (s1, c2).
   */
  int operator()(string const &s1, char const c2);

  /**
   * Get an unique code for every symbol pair.  This flavour is for
   * (character, string) pairs.
   * @param c1 left symbol.
   * @param s2 right symbol.
   * @return code for (c1, s2).
   */
  int operator()(char const c1, string const &s2);

  /**
   * Get an unique code for every symbol pair.  This flavour is for
   * character pairs.
   * @param c1 left symbol.
   * @param c2 right symbol.
   * @return code for (c1, c2).
   */
  int operator()(char const c1, char const c2);
  
  /**
   * Gets the individual symbol identifier.
   * @param s symbol to be identified.
   * @return symbol identifier.
   */
  int operator()(string const &s);

  /**
   * Decodes the first symbol of a given pair key.
   * @param key the symbol pair key.
   * @return first symbol encoded in the key.
   */
  string const & firstSymbol(int const key) const;


  /**
   * Decodes the second symbol of a given pair key.
   * @param key the symbol pair key.
   * @return second symbol encoded in the key.
   */
  string const & secondSymbol(int const clave) const;

  /**
   * Check wether the symbol is defined in the alphabet.
   * @param s symbol
   * @return true if defined
   */
  bool isSymbolDefined(string const &s);

  /**
   * Returns the size of the alphabet (number of symbols).
   * @return number of symbols.
   */
  int size() const;

  /**
   * Write method.
   * @param output output stream.
   */
  void write(FILE *output);
  
  /**
   * Read method.
   * @param input input stream.
   */
  void read(FILE *input);

  /**
   * Write a symbol enclosed by angle brackets in the output stream.
   * @param symbol symbol code.
   * @param output output stream.
   */
  void writeSymbol(int const symbol, FILE *output) const;
  
  /**
   * Concat a symbol in the string that is passed by reference.
   * @param result string where the symbol should be concatenated
   * @param symbol code of the symbol
   * @param uppercase true if we want an uppercase symbol
   */  
  void getSymbol(string &result, int const symbol, 
		 bool uppercase = false) const;
		 
  /**
   * Gets a symbol from their code.
   * @param symbol code of the symbol
   * @param uppercase true if we want an uppercase symbol
   * @return the symbol
   */ 
  string const & getSymbol(int const symbol, 
			   bool uppercase = false) const;

  /**
   * Checks whether a symbol is a tag or not
   * @param symbol the code of the symbol
   * @return true if the symbol is a tag
   */
  bool isTag(int const symbol) const;
};

#endif
