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

#include "PTXReader.H"
#include <lttoolbox/XMLParseUtil.H>
#include <apertium/StringUtils.H>

void
PTXReader::copy(PTXReader const &o) {
  cerr<<"Warning in PTXReader::copy: Not implemented\n";
}

void
PTXReader::destroy() {
}

PTXReader::PTXReader() {
}

PTXReader::~PTXReader() {
  destroy();
}

PTXReader::PTXReader(PTXReader const &o) {
  copy(o);
}


void
PTXReader::step() {
  int retval = xmlTextReaderRead(reader);
  if(retval != 1)
  {
    parseError("unexpected EOF");
  }
  name = XMLParseUtil::latin1(xmlTextReaderConstName(reader));
  type = xmlTextReaderNodeType(reader);

  //cerr<<"In PTXReader::step: name='"<<name<<"', type="<<type<<"\n";
}

PTXReader &
PTXReader::operator =(PTXReader const &o) {
  if(this != &o)
  {
    destroy();
    copy(o);
  }
  return *this;
}

string
PTXReader::attrib(string const &name) {
  return XMLParseUtil::attrib(reader, name);
} 

void
PTXReader::parseError(string const &message) {
  cerr << "Error: (" << xmlTextReaderGetParserLineNumber(reader);
  cerr << "): " << message << "." << endl;
  exit(EXIT_FAILURE);
}


void 
PTXReader::proc_mlu() {
  vector<string> one_mlu;
  if(name == "mlu") {
    step();
    while (!((name=="mlu") && (type==XML_READER_TYPE_END_ELEMENT))) {
      if (name=="lu")
	proc_lu(one_mlu);
      step();
    }
  }

  mlu.push_back(one_mlu);

  while(name == "#text" || name == "#comment") 
    step();
}

void
PTXReader::proc_lu(vector<string>& one_mlu) {
  string tags=attrib("tags");
  if (tags.length()==0)
    parseError("mandatory attribute 'tags' cannot be empty");
  else if (tags[0]=='*')
    parseError("mandatory attribute 'tags' cannot start with '*'");
  
  unsigned p=tags.find("*",0);
  if ((p!=string::npos) && (p!=(tags.size()-1)))
     parseError("mandatory attribute 'tags' cannot cannot have a '*' in the middle");

  tags=StringUtils::substitute(tags, ".*","");
  tags=StringUtils::substitute(tags, ".", "><");
  tags="<"+tags+">";

  one_mlu.push_back(tags);
}

void
PTXReader::read(string const &filename) {
  reader = xmlReaderForFile(filename.c_str(), NULL, 0);
  if(reader == NULL) {
    cerr << "Error: Cannot open '" << filename << "'." << endl;
    exit(EXIT_FAILURE);
  }

  step();

  while(name == "#text" || name == "#comment") 
    step();
  
  if(name == "posttransfer") {
    step();
    while (!((name=="posttransfer") && (type==XML_READER_TYPE_END_ELEMENT))) {
      if (name=="mlu")
	proc_mlu();
      step();
    }
  }

  xmlFreeTextReader(reader);
  xmlCleanupParser();
}


vector<vector<string> > 
PTXReader::get_all_mlu() {
  return mlu;
}
