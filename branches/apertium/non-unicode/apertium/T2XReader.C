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
#include <apertium/T2XReader.H>
#include <lttoolbox/XMLParseUtil.H>
#include <lttoolbox/Compression.H>

#include <cstdlib>
#include <iostream>

string const
T2XReader::ANY_TAG = "<ANY_TAG>";

string const
T2XReader::ANY_CHAR = "<ANY_CHAR>";

void
T2XReader::copy(T2XReader const &o)
{
}

void
T2XReader::destroy()
{
}

T2XReader::T2XReader()
{
  td.getAlphabet().includeSymbol(ANY_TAG);
  td.getAlphabet().includeSymbol(ANY_CHAR);
}

T2XReader::~T2XReader()
{
  destroy();
}

T2XReader::T2XReader(T2XReader const &o)
{
  copy(o);
}

void
T2XReader::step()
{
  int retval = xmlTextReaderRead(reader);
  if(retval != 1)
  {
    parseError("unexpected EOF");
  }
  name = latin1(xmlTextReaderConstName(reader));
  type = xmlTextReaderNodeType(reader);
}

T2XReader &
T2XReader::operator =(T2XReader const &o)
{
  if(this != &o)
  {
    destroy();
    copy(o);
  }
  return *this;
}

string
T2XReader::attrib(string const &name)
{
  return XMLParseUtil::attrib(reader, name);
} 

string
T2XReader::latin1(xmlChar const * input)
{
  return XMLParseUtil::latin1(input);
}

void
T2XReader::parseError(string const &message)
{
  cerr << "Error: (" << xmlTextReaderGetParserLineNumber(reader);
  cerr << "): " << message << "." << endl;
  exit(EXIT_FAILURE);
}

int
T2XReader::insertLemma(int const base, string const &lemma)
{
  int retval = base;
  static int const any_char = td.getAlphabet()(ANY_CHAR);
  if(lemma == "")
  {
    retval = td.getTransducer().insertSingleTransduction(any_char, retval);
    td.getTransducer().linkStates(retval, retval, any_char);
    int another = td.getTransducer().insertSingleTransduction('\\', retval);
    td.getTransducer().linkStates(another, retval, any_char);
  }
  else
  {
    for(unsigned int i = 0, limit = lemma.size();  i != limit; i++)
    {
      retval = td.getTransducer().insertSingleTransduction(int((unsigned char)lemma[i]), retval);
    }
  }
  
  return retval;
}

int
T2XReader::insertTags(int const base, string const &tags)
{
  int retval = base;
  static int const any_tag = td.getAlphabet()(ANY_TAG);
  if(tags.size() != 0)
  {
    for(unsigned int i = 0, limit = tags.size(); i < limit; i++)
    {
      if(tags[i] == '*')
      {
        retval = td.getTransducer().insertSingleTransduction(any_tag, retval);
        td.getTransducer().linkStates(retval, retval, any_tag);
        i++;
      }  
      else
      {
        string symbol = "<";
        for(unsigned int j = i; j != limit; j++)
        {
          if(tags[j] == '.')
          {  
            symbol.append(tags.substr(i, j-i));
            i = j;
            break;
          }
        }
        
        if(symbol == "<")
        {
          symbol.append(tags.substr(i));
          i = limit;
        }
        symbol += '>';
        td.getAlphabet().includeSymbol(symbol);
        retval = td.getTransducer().insertSingleTransduction(td.getAlphabet()(symbol), retval);
      }
    }
  }
  else
  {
    retval = td.getTransducer().insertSingleTransduction(any_tag, retval);
    td.getTransducer().linkStates(retval, retval, any_tag);
  }
  
  return retval;
}

void
T2XReader::read(string const &filename)
{
  reader = xmlReaderForFile(filename.c_str(), NULL, 0);
  if(reader == NULL)
  {
    cerr << "Error: Cannot open '" << filename << "'." << endl;
    exit(EXIT_FAILURE);
  }

  procDefCats();
  step();
  while(name == "#text" || name == "#comment")
  {
    step();
  }
 
  procDefAttrs();
  step();
  while(name == "#text" || name == "#comment")
  {
    step();
  }
  
  procDefVars();
  step();
  while(name == "#text" || name == "#comment")
  {
    step();
  }

  if(name == "section-def-lists")
  {
    procDefLists();
    step();
    while(name == "#text" || name == "#comment")
    {
      step();
    }
  }

  if(name == "section-def-macros")
  {
    procDefMacros();
    step();
    while(name == "#text" || name == "#comment")
    {
      step();
    }
  }

  if(name == "section-rules")
  {
    procRules();
    step();
    while(name == "#text" || name == "#comment")
    {
      step();
    }
  }  
}

void
T2XReader::procRules()
{
  int count = 0;
  set<int> alive_states;
  
  while(type != XML_READER_TYPE_END_ELEMENT || 
	name != "section-rules")
  {
    step();
    if(name == "rule")
    {
      if(type != XML_READER_TYPE_END_ELEMENT)
      {
	count++;
      }
    }
    else if(name == "pattern")
    {
      if(type != XML_READER_TYPE_END_ELEMENT)
      {
        alive_states.clear();
        alive_states.insert(td.getTransducer().getInitial());
      }
      else
      {
        for(set<int>::iterator it = alive_states.begin(), limit = alive_states.end();
            it != limit; it++)
        {
          td.getTransducer().setFinal(*it);
          if(td.getFinals().find(*it) == td.getFinals().end())
          {
            td.getFinals()[*it] = count;
          }       
        }
      }
    }
    else if(name == "pattern-item")
    {
      if(type != XML_READER_TYPE_END_ELEMENT)
      {
        pair<multimap<string, LemmaTags>::iterator,
             multimap<string, LemmaTags>::iterator> range;

        range = cat_items.equal_range(attrib("n"));
      
        if(range.first == range.second)
        {
          parseError("Undefined cat-item '" + attrib("n"));
        }

// new code
      
        set<int> alive_states_new;
        
        for(; range.first != range.second; range.first++)
        {
          for(set<int>::iterator it = alive_states.begin(), limit = alive_states.end();
              it != limit; it++)
          {
            // mark of begin of word
            int tmp = td.getTransducer().insertSingleTransduction('^', *it);
            if(*it != td.getTransducer().getInitial())
            {
              // insert optional blank between two words
              int alt = td.getTransducer().insertSingleTransduction(' ', *it);
              td.getTransducer().linkStates(alt, tmp, '^');
            }
            
            // insert word
            tmp = insertLemma(tmp, range.first->second.lemma);
            tmp = insertTags(tmp, range.first->second.tags);
            
            // insert mark of end of word
            tmp = td.getTransducer().insertSingleTransduction('$', tmp);
            
            // set as alive_state
            alive_states_new.insert(tmp);
          }
        } 
        
        // copy new alive states on alive_states set
        alive_states = alive_states_new;      
      }
    }
  }
}

void
T2XReader::write(string const &filename)
{
  FILE *out = fopen(filename.c_str(), "w");
  if(!out)
  {
    cerr << "Error: cannot open '" << filename;
    cerr << "' for writing" << endl;
    exit(EXIT_FAILURE);
  }
  
  td.write(out);

  fclose(out);
}

void
T2XReader::procDefAttrs()
{
  string attrname = "";

  while(type != XML_READER_TYPE_END_ELEMENT || 
	name != "section-def-attrs")
  {
    step();
    if(name == "attr-item")
    {
      if(type != XML_READER_TYPE_END_ELEMENT)
      {
        insertAttrItem(attrname, attrib("tags"));
      }
    }
    else if(name == "def-attr")
    {
      if(type != XML_READER_TYPE_END_ELEMENT)
      {
        attrname = attrib("n");
      }
      else
      {
        attrname = "";
      }
    }
    else if(name == "#text")
    {
      // do nothing
    }
    else if(name == "#comment")
    {
      // do nothing
    }
    else if(name == "section-def-attrs")
    {
      // do nothing
    }
    else
    {
      parseError("Unexpected '<" + name + ">' tag");
    }
  }
}

void 
T2XReader::procDefCats()
{
  while(type == XML_READER_TYPE_END_ELEMENT || name != "interchunk")
  {
    step();
    if(name != "#text" && name != "interchunk" && 
       name != "section-def-cats" && name != "#comment")
    {
      parseError("'<" + name + ">' tag unexpected");
    }
  }
  
  string catname = "";

  while(type != XML_READER_TYPE_END_ELEMENT || 
	name != "section-def-cats")
  {
    step();
    if(name == "cat-item")
    {
      if(type != XML_READER_TYPE_END_ELEMENT)
      {
        insertCatItem(catname, attrib("lemma"), attrib("tags"));
      }
    }
    else if(name == "def-cat")
    {
      if(type != XML_READER_TYPE_END_ELEMENT)
      {
        catname = attrib("n");
      }
      else
      {
        catname = "";
      }
    }
    else if(name == "#text")
    {
      // do nothing
    }
    else if(name == "#comment")
    {
      // do nothing
    }
    else if(name == "section-def-cats")
    {
      // do nothing
    }
    else
    {
      parseError("Unexpected '<" + name + ">' tag");
    }
  }
}

void
T2XReader::procDefVars()
{
  while(type != XML_READER_TYPE_END_ELEMENT || 
	name != "section-def-vars")
  {
    step();
    if(name == "def-var")
    {
      if(type != XML_READER_TYPE_END_ELEMENT)
      {
        createVar(attrib("n"));
      }
    }
    else if(name == "#text")
    {
      // do nothing
    }
    else if(name == "#comment")
    {
      // do nothing
    }
    else if(name == "section-def-vars")
    {
      // do nothing
    }
    else
    {
      parseError("Unexpected '<" + name + ">' tag");
    }
  }
}

void
T2XReader::procDefLists()
{
  string listname = "";

  while(type != XML_READER_TYPE_END_ELEMENT || 
	name != "section-def-lists")
  {
    step();
    if(name == "list-item")
    {
      if(type != XML_READER_TYPE_END_ELEMENT)
      {
        insertListItem(listname, attrib("v"));
      }
    }
    else if(name == "def-list")
    {
      if(type != XML_READER_TYPE_END_ELEMENT)
      {
        listname = attrib("n");
      }
      else
      {
        listname = "";
      }
    }
    else if(name == "#text")
    {
      // do nothing
    }
    else if(name == "#comment")
    {
      // do nothing
    }
    else if(name == "section-def-lists")
    {
      // do nothing
    }
    else
    {
      parseError("Unexpected '<" + name + ">' tag");
    }
  }
}

void
T2XReader::procDefMacros()
{
  int count = 0;
  while(type != XML_READER_TYPE_END_ELEMENT || 
	name != "section-def-macros")
  {
    step();
    if(name == "def-macro")
    {
      if(type != XML_READER_TYPE_END_ELEMENT)
      {
        createMacro(attrib("n"), count++);
      }
    }
  }
}

void
T2XReader::createMacro(string const &name, int const value)
{
  if(td.getMacros().find(name) != td.getMacros().end())
  {
    parseError("Macro '" + name + "' defined at least twice");    
  }
  td.getMacros()[name] = value;
}

void
T2XReader::insertListItem(string const &name, string const &value)
{
  td.getLists()[name].insert(value);
}

void
T2XReader::createVar(string const &name)
{
  td.getVariables()[name] = "";
}

void
T2XReader::insertCatItem(string const &name, string const &lemma, 
			 string const &tags)
{
  LemmaTags lt;
  lt.lemma = lemma;
  lt.tags = tags;
  cat_items.insert(pair<string, LemmaTags>(name, lt));
}

void 
T2XReader::insertAttrItem(string const &name, string const &tags)
{
  if(td.getAttrItems()[name].size() != 0)
  {
    td.getAttrItems()[name] += '|';
  }  
  
  td.getAttrItems()[name] += '<';

  for(unsigned int i = 0, limit = tags.size(); i != limit; i++)
  {
    if(tags[i] == '.')
    {
      td.getAttrItems()[name].append("><");
    }
    else
    {
	td.getAttrItems()[name] += tags[i];
    }
  }
  td.getAttrItems()[name] += '>';
  
}
