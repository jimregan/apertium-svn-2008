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
#ifndef _TSXREADER_
#define _TSXREADER_

#include <apertium/ConstantManager.H>
#include <apertium/TaggerData.H>
#include <apertium/TTag.H>
#include <lttoolbox/PatternList.H>

#include <libxml/xmlreader.h>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

class TSXReader
{
private:
  xmlTextReaderPtr reader;  
  set<TTag> *open_class;
  vector<TForbidRule> *forbid_rules;
  map<string, TTag> *tag_index;
  vector<string> *array_tags;
  vector<TEnforceAfterRule> *enforce_rules;
  vector<string> *prefer_rules;
  PatternList *plist;
  ConstantManager *constants;
  TaggerData tdata;

  int type;
  string name;

  string attrib(string const &name);

  void parseError(string const &message);
  void newTagIndex(string const &tag);
  void newDefTag(string const &tag);
  void newConstant(string const &constant);
  void procDefLabel();
  void procDefMult();
  void procTagset();
  void procForbid();
  void procLabelSequence();
  void procEnforce();
  void procPreferences();
  void copy(TSXReader const &o);
  void destroy();
  void clearTagIndex();

  void step();
public:
  TSXReader();
  ~TSXReader();
  TSXReader(TSXReader const &o);
  TSXReader & operator =(TSXReader const &o);

  void read(string const &filename);
  void write(string const &filename);
  TaggerData & getTaggerData();
};

#endif
