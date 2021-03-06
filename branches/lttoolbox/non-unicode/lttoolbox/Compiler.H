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
#ifndef _MYCOMPILER_
#define _MYCOMPILER_

#include <lttoolbox/Alphabet.H>
#include <lttoolbox/RegExpCompiler.H>
#include <lttoolbox/EntryToken.H>
#include <lttoolbox/Ltstr.H>
#include <lttoolbox/Transducer.H>

#include <map>
#include <string>
#include <set>
#include <libxml/xmlreader.h>

using namespace std;

/**
 * A compiler of dictionaries to letter transducers
 */
class Compiler
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
   * The dictionary section being compiled
   */
  string current_section;
  
  /**
   * The direction of the compilation, 'lr' (left-to-right) or 'rl'
   * (right-to-left)
   */
  string direction;
  
  /**
   * List of characters to be considered alphabetic
   */
  string letters;
  
  /**
   * Identifier of all the symbols during the compilation
   */
  Alphabet alphabet;  
  
  /**
   * List of named transducers-paradigms
   */
  map<string, Transducer, Ltstr> paradigms;
  
  /**
   * List of named dictionary sections
   */
  map<string, Transducer, Ltstr> sections;
  
  /**
   * List of named prefix copy of a paradigm
   */
  map<string, map<string, int, Ltstr>, Ltstr> prefix_paradigms;
  
  /**
   * List of named suffix copy of a paradigm
   */
  map<string, map<string, int, Ltstr>, Ltstr> suffix_paradigms;

  /**
   * List of named endings of a suffix copy of a paradgim
   */
  map<string, map<string, int, Ltstr>, Ltstr> postsuffix_paradigms;

  /*    
  static string range(char const a, char const b);
  string readAlphabet();
  */

  /**
   * Method to parse an XML Node
   */
  void procNode();

  /**
   * Parse the &lt;alphabet&gt; element
   */
  void procAlphabet();

  /**
   * Parse the &lt;sdef&lt; element
   */
  void procSDef();

  /**
   * Parse the &lt;pardef&gt; element
   */
  void procParDef();
  
  /**
   * Parse the &lt;e&gt; element
   */
  void procEntry();

  /**
   * Parse the &lt;re&gt; element
   * @return a list of tokens from the dictionary's entry
   */
  EntryToken procRegExp();

  /**
   * Parse the &lt;section&gt; element
   */
  void procSection();

  /**
   * Gets an attribute value with their name and the current context
   * @param name the name of the attribute
   * @return the value of the attribute
   */
  string attrib(string const &name);

  /**
   * Construct symbol pairs by align left side of both parts and insert
   * them into a transducer
   * @param lp left part of the transduction
   * @param rp right part of the transduction
   * @param state the state from wich insert the new transduction
   * @param t the transducer
   * @return the last state of the inserted transduction
   */
  int matchTransduction(list<string> const &lp, list<string> const &rp,
			    int state, Transducer &t);
  /**
   * Parse the &lt;p&lt; element
   * @return a list of tokens from the dictionary's entry
   */
  EntryToken procTransduction();

  /**
   * Parse the &lt;i&lt; element
   * @return a list of tokens from the dictionary's entry
   */
  EntryToken procIdentity();

  /**
   * Parse the &lt;par&gt; element
   * @return a list of tokens from the dictionary's entry
   */
  EntryToken procPar();

  /**
   * Insert a list of tokens into the paradigm / section being processed
   * @param elements the list
   */
  void insertEntryTokens(vector<EntryToken> const &elements);

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
  
  
  void readString(list<string> &result, string const &name);
  
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

public:

  /*
   * Constants to represent the element and the attributes of
   * dictionaries
   */
  static string const COMPILER_DICTIONARY_ELEM;
  static string const COMPILER_ALPHABET_ELEM;
  static string const COMPILER_SDEFS_ELEM;
  static string const COMPILER_SDEF_ELEM;
  static string const COMPILER_N_ATTR;
  static string const COMPILER_PARDEFS_ELEM;
  static string const COMPILER_PARDEF_ELEM;
  static string const COMPILER_PAR_ELEM;
  static string const COMPILER_ENTRY_ELEM;
  static string const COMPILER_RESTRICTION_ATTR;
  static string const COMPILER_RESTRICTION_LR_VAL;
  static string const COMPILER_RESTRICTION_RL_VAL;
  static string const COMPILER_PAIR_ELEM;
  static string const COMPILER_LEFT_ELEM;
  static string const COMPILER_RIGHT_ELEM;
  static string const COMPILER_S_ELEM;
  static string const COMPILER_REGEXP_ELEM;
  static string const COMPILER_SECTION_ELEM;
  static string const COMPILER_ID_ATTR;
  static string const COMPILER_TYPE_ATTR;
  static string const COMPILER_IDENTITY_ELEM;
  static string const COMPILER_JOIN_ELEM;
  static string const COMPILER_BLANK_ELEM;
  static string const COMPILER_POSTGENERATOR_ELEM;
  static string const COMPILER_GROUP_ELEM;
  static string const COMPILER_LEMMA_ATTR;
  static string const COMPILER_IGNORE_ATTR;
  static string const COMPILER_IGNORE_YES_VAL;


  /**
   * Copnstructor
   */
  Compiler();

  /**
   * Destructor
   */
  ~Compiler();

  /**
   * Compile dictionary to letter transducers
   */
  void parse(string const &fichero, string const &dir);
  
  /**
   * UTF-8 to Latin-1 conversion
   * @param input the input string, in UTF-8 encoding
   * @return the output string, in Latin-1 encoding
   */
  static string latin1(xmlChar const * input);

  /**
   * Write the result of compilation 
   * @param fd the stream where write the result
   */
  void write(FILE *fd);
};


#endif
