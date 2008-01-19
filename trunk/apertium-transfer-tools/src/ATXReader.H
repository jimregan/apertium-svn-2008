/*
 * Copyright (C) 2006-2007 Felipe Sánchez-Martínez
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

#ifndef __ATXREADER_
#define __ATXREADER_

#include <libxml/xmlreader.h>
#include <string>
#include "LexicalizedWords.H"

using namespace std;

class ATXReader {
private:
  xmlTextReaderPtr reader;  

  string source_lang;
  LexicalizedWords source_lexicalized_words;

  string target_lang;
  LexicalizedWords target_lexicalized_words;

  string name;
  int type;

  void copy(ATXReader const &o);

  void destroy();

  void step();

  string attrib(string const &name);

  void parseError(string const &message);

  void procLexicalizedWord(LexicalizedWords& cw);
  void procSource();
  void procTarget();

public:

  ATXReader();

  ~ATXReader();

  ATXReader(ATXReader const &o);

  ATXReader & operator =(ATXReader const &o);

  void read(string const &filename);

  string get_source_language();

  string get_target_language();

  LexicalizedWords& get_source_lexicalized_words();

  LexicalizedWords& get_target_lexicalized_words(); 
};

#endif
