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
#ifndef _T2XREADER_
#define _T2XREADER_

#include <apertium/TransferData.H>

#include <libxml/xmlreader.h>
#include <map>
#include <string>

using namespace std;

class T2XReader
{
private:
  struct LemmaTags
  {
    string lemma;
    string tags;
  };

  xmlTextReaderPtr reader;  

  int type;
  string name;

  multimap<string, LemmaTags> cat_items;
  TransferData td;

  string attrib(string const &name);
  string latin1(xmlChar const *input);

  void parseError(string const &message);
  void copy(T2XReader const &o);
  void destroy();
  void clearTagIndex();
  
  void step();
  void procTransfer();
  void procDefCats();
  void procDefAttrs();
  void procDefVars();
  void procDefLists();
  void procDefMacros();
  void procRules();

  void insertCatItem(string const &name, string const &lemma, 
		     string const &tags);
  void insertAttrItem(string const &name, string const &tags);
  void createVar(string const &name);
  void insertListItem(string const &name, string const &value);
  void createMacro(string const &name, int const val);

  int insertLemma(int const base, string const &lemma);
  int insertTags(int const base, string const &tags);
  
public:
  static string const ANY_TAG;
  static string const ANY_CHAR;


  T2XReader();
  ~T2XReader();
  T2XReader(T2XReader const &o);
  T2XReader & operator =(T2XReader const &o);

  void read(string const &filename);
  void write(string const &filename);
};

#endif
