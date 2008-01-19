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
#ifndef _EXPANDER_
#define _EXPANDER_

#include <lttoolbox/Ltstr.H>

#include <list>
#include <map>
#include <string>
#include <libxml/xmlreader.h>

using namespace std;

typedef list<pair<string, string> > EntList;

/**
 * A compiler of dictionaries to letter transducers
 */
class Expander
{
private:
  /**
   * The libxml2's XML reader
   */
  xmlTextReaderPtr reader;
  
  /**
   * The paradigm being compiled
   */
  string current_paradigm;
  
  /**
   * The direction of the compilation, 'lr' (left-to-right) or 'rl'
   * (right-to-left)
   */
  string direction;
  
  /**
   * Paradigms
   */
  map<string, EntList, Ltstr> paradigm;

  map<string, EntList, Ltstr> paradigm_lr;

  map<string, EntList, Ltstr> paradigm_rl;

  /**
   * Method to parse an XML Node
   */
  void procNode(FILE *output);

  /**
   * Parse the &lt;pardef&gt; element
   */
  void procParDef();
  
  /**
   * Parse the &lt;e&gt; element
   */
  void procEntry(FILE *output);

  /**
   * Parse the &lt;re&gt; element
   * @return the string representing the regular expression
   */
  string procRegExp();

  /**
   * Gets an attribute value with their name and the current context
   * @param name the name of the attribute
   * @return the value of the attribute
   */
  string attrib(string const &name);

  /**
   * Parse the &lt;p&lt; element
   * @return a pair of string, left part and right part of a transduction
   */
  pair<string, string> procTransduction();

  /**
   * Parse the &lt;i&lt; element
   * @return a string from the dictionary's entry
   */
  string procIdentity();

  /**
   * Parse the &lt;par&gt; element
   * @return the name of the paradigm
   */
  string procPar();

  /**
   * Skip all document #text nodes before "elem"
   * @param name the name of the node
   * @param elem the name of the expected node
   */
  void skip(string &name, string const &elem);

  /**
   * Skip all blank #text nodes before "name"
   * @param name the name of the node
   */
  void skipBlanks(string &name);
  
  
  void readString(string &result, string const &name);
  
  /**
   * Force an element to be empty, and check for it
   * @param name the element 
   */
  void requireEmptyError(string const &name);

  /**
   * Force an attribute to be specified, amd check for it
   * @param value the value of the attribute
   * @param attrname the name of the attribute
   * @param elemname the parent of the attribute
   */
  void requireAttribute(string const &value, string const &attrname,
                        string const &elemname);

  /**
   * True if all the elements in the current node are blanks
   * @return true if all are blanks
   */
  bool allBlanks();

  /**
   * Append a list of endings to a list of current transductions.
   * @param result the current partial transductions, and after calling
   *               this method, the result of concatenations.
   * @param endings the endings to be appended.
   */
  static void append(list<pair<string, string> > &result, 
                     list<pair<string, string> > const &endings);

  /**
   * Append a list of endings to a list of current transductions.
   * @param result the current partial transductions, and after calling
   *               this method, the result of concatenations.
   * @param endings the endings to be appended.
   */
  static void append(list<pair<string, string> > &result, 
                     string const &endings);

  /**
   * Append a list of endings to a list of current transductions.
   * @param result the current partial transductions, and after calling
   *               this method, the result of concatenations.
   * @param endings the endings to be appended.
   */
  static void append(list<pair<string, string> > &result, 
	             pair<string, string> const &endings);

public:
  /**
   * Constructor
   */
  Expander();

  /**
   * Destructor
   */
  ~Expander();

  /**
   * Compile dictionary to letter transducers
   */
  void expand(string const &fichero, FILE *output);
};


#endif
