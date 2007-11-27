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


#include "ATXReader.H"
#include <lttoolbox/XMLParseUtil.H>

void
ATXReader::copy(ATXReader const &o) {
  cerr<<"Warning in ATXReader::copy: Not implemented\n";
}

void
ATXReader::destroy() {
}

ATXReader::ATXReader() {
}

ATXReader::~ATXReader() {
  destroy();
}

ATXReader::ATXReader(ATXReader const &o) {
  copy(o);
}


void
ATXReader::step() {
  int retval = xmlTextReaderRead(reader);
  if(retval != 1)
  {
    parseError("unexpected EOF");
  }
  name = XMLParseUtil::latin1(xmlTextReaderConstName(reader));
  type = xmlTextReaderNodeType(reader);

  //cerr<<"In ATXReader::step: name='"<<name<<"', type="<<type<<"\n";
}

ATXReader &
ATXReader::operator =(ATXReader const &o) {
  if(this != &o)
  {
    destroy();
    copy(o);
  }
  return *this;
}

string
ATXReader::attrib(string const &name) {
  return XMLParseUtil::attrib(reader, name);
} 

void
ATXReader::parseError(string const &message) {
  cerr << "Error: (" << xmlTextReaderGetParserLineNumber(reader);
  cerr << "): " << message << "." << endl;
  exit(EXIT_FAILURE);
}

void
ATXReader::procLexicalizedWord(LexicalizedWords& cw) {
  string tags=attrib("tags");
  if (tags.length()==0)
    parseError("mandatory attribute 'tags' cannot be empty");
  else if (tags[0]=='*')
    parseError("mandatory attribute 'tags' cannot start with '*'");

  //cerr<<attrib("lemma")<<" "<<attrib("tags")<<"\n";
  cw.insert(attrib("lemma"), attrib("tags"));
}

void 
ATXReader::procSource() {
  if(name == "lexicalized-words") {
    step();
    while (!((name=="lexicalized-words") && (type==XML_READER_TYPE_END_ELEMENT))) {
      if (name=="lexicalized-word")
	procLexicalizedWord(source_lexicalized_words);
      step();
    }
  }

  while(name == "#text" || name == "#comment") 
    step();
  
  while (!((name=="source") && (type==XML_READER_TYPE_END_ELEMENT))) 
    step();
  
  step();

  while(name == "#text" || name == "#comment") 
    step();
}

void 
ATXReader::procTarget() {
  if(name == "lexicalized-words") {
    step();
    while (!((name=="lexicalized-words") && (type==XML_READER_TYPE_END_ELEMENT))) {
      if (name=="lexicalized-word")
	procLexicalizedWord(target_lexicalized_words);
      step();
    }
  }

  while(name == "#text" || name == "#comment") 
    step();  

  while (!((name=="target") && (type==XML_READER_TYPE_END_ELEMENT))) 
    step();
  
  step();

  while(name == "#text" || name == "#comment")
    step();  
}

void
ATXReader::read(string const &filename) {
  reader = xmlReaderForFile(filename.c_str(), NULL, 0);
  if(reader == NULL) {
    cerr << "Error: Cannot open '" << filename << "'." << endl;
    exit(EXIT_FAILURE);
  }

  step();

  while(name == "#text" || name == "#comment") 
    step();
  
  if(name == "transfer-at") {
    source_lang=attrib("source");
    target_lang=attrib("target");
    step();
    while(name == "#text" || name == "#comment") 
      step();
  }

  if (name == "source") {
    step();
    while(name == "#text" || name == "#comment") 
      step();
    
    procSource();
  }

  if (name == "target") {
    step();
    while(name == "#text" || name == "#comment") 
      step();
    
    procTarget();
  }

  xmlFreeTextReader(reader);
  xmlCleanupParser();

  //cerr<<"source="<<source_lang<<"\n";
  //cerr<<"target="<<target_lang<<"\n";
}

LexicalizedWords&
ATXReader::get_source_lexicalized_words() {
  return source_lexicalized_words;
}

LexicalizedWords&
ATXReader::get_target_lexicalized_words() {
  return target_lexicalized_words;
}

string 
ATXReader::get_source_language() {
  return source_lang;
}

string 
ATXReader::get_target_language() {
  return target_lang;
}
