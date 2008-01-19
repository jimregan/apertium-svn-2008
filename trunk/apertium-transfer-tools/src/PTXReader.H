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

#ifndef __PTXREADER_
#define __PTXREADER_

#include <libxml/xmlreader.h>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class PTXReader {
private:
  xmlTextReaderPtr reader;  

  vector<vector<string> > mlu; //multilexical units, a vector of
			       //lexical units

  string name;
  int type;

  void copy(PTXReader const &o);

  void destroy();

  void step();

  string attrib(string const &name);

  void parseError(string const &message);

  void proc_mlu();
  void proc_lu(vector<string>& one_mlu);

public:

  PTXReader();

  ~PTXReader();

  PTXReader(PTXReader const &o);

  PTXReader & operator =(PTXReader const &o);

  void read(string const &filename);

  vector<vector<string> > get_all_mlu();
};

#endif
