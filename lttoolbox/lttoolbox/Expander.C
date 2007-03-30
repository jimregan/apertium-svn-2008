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

#include <lttoolbox/Compiler.H>
#include <lttoolbox/Expander.H>
#include <lttoolbox/LtLocale.H>
#include <lttoolbox/XMLParseUtil.H>
#include <lttoolbox/MyStdio.H>

#include <cstdlib>
#include <iostream>
#include <libxml/encoding.h>

using namespace std;

Expander::Expander()
{
  LtLocale::tryToSetLocale();
}

Expander::~Expander()
{
}

void
Expander::expand(string const &fichero, FILE *output)
{
  reader = xmlReaderForFile(fichero.c_str(), NULL, 0);
  if(reader == NULL)
  {
    cerr << "Error: Cannot open '" << fichero << "'." << endl;
    exit(EXIT_FAILURE);
  }

  int ret = xmlTextReaderRead(reader);
  while(ret == 1)
  {
    procNode(output);
    ret = xmlTextReaderRead(reader);
  }

  if(ret != 0)
  {
    cerr << "Error: Parse error at the end of input." << endl;
  }

  xmlFreeTextReader(reader);
  xmlCleanupParser();
}

void
Expander::procParDef()
{
  int tipo=xmlTextReaderNodeType(reader);

  if(tipo != XML_READER_TYPE_END_ELEMENT)
  {
    current_paradigm = attrib(Compiler::COMPILER_N_ATTR);
  }
  else
  {
    current_paradigm = "";
  }
}

void
Expander::requireEmptyError(string const &name)
{
  if(!xmlTextReaderIsEmptyElement(reader))
  {
    cerr << "Error (" << xmlTextReaderGetParserLineNumber(reader);
    cerr << "): Non-empty element '<" << name << ">' should be empty." << endl;
    exit(EXIT_FAILURE);
  }
}

bool 
Expander::allBlanks()
{
  bool flag = true;
  string text = XMLParseUtil::latin1(xmlTextReaderConstValue(reader));
  
  for(unsigned int i = 0, limit = text.size(); i < limit; i++)
  {
    flag = flag && isspace(text[i]);
  }
  
  return flag;
}

void 
Expander::readString(string &result, string const &name)
{
  if(name == "#text")
  {
    string value = XMLParseUtil::latin1(xmlTextReaderConstValue(reader));
    result.append(value);
  }
  else if(name == Compiler::COMPILER_BLANK_ELEM)
  {
    requireEmptyError(name);
    result += ' ';
  }
  else if(name == Compiler::COMPILER_JOIN_ELEM)
  {
    requireEmptyError(name);
    result += '+';
  }
  else if(name == Compiler::COMPILER_POSTGENERATOR_ELEM)
  {
    requireEmptyError(name);
    result += '~';
  }
  else if(name == Compiler::COMPILER_GROUP_ELEM)
  {
    int tipo=xmlTextReaderNodeType(reader);
    if(tipo != XML_READER_TYPE_END_ELEMENT)
    {
      result += '#';
    }
  }
  else if(name == Compiler::COMPILER_S_ELEM)
  {
    requireEmptyError(name);
    result += '<';
    result.append(attrib(Compiler::COMPILER_N_ATTR));
    result += '>';
  }
  else
  {
    cerr << "Error (" << xmlTextReaderGetParserLineNumber(reader);
    cerr << "): Invalid specification of element '<" << name;
    cerr << ">' in this context." << endl;
    exit(EXIT_FAILURE);
  }
}

void
Expander::skipBlanks(string &name)
{
  if(name == "#text")
  {
    if(!allBlanks())
    {
      cerr << "Error (" << xmlTextReaderGetParserLineNumber(reader); 
      cerr << "): Invalid construction." << endl;
      exit(EXIT_FAILURE);
    }
    xmlTextReaderRead(reader);
    name = XMLParseUtil::latin1(xmlTextReaderConstName(reader));
  }
}

void
Expander::skip(string &name, string const &elem)
{
  xmlTextReaderRead(reader);
  name = XMLParseUtil::latin1(xmlTextReaderConstName(reader));
  
  if(name == "#text")
  {
    if(!allBlanks())
    {
      cerr << "Error (" << xmlTextReaderGetParserLineNumber(reader);
      cerr << "): Invalid construction." << endl;
      exit(EXIT_FAILURE);
    }
    xmlTextReaderRead(reader);
    name = XMLParseUtil::latin1(xmlTextReaderConstName(reader));
  }    
    
  if(name != elem)
  {
    cerr << "Error (" << xmlTextReaderGetParserLineNumber(reader);
    cerr << "): Expected '<" << elem << ">'." << endl;
    exit(EXIT_FAILURE);
  }  
}

string
Expander::procIdentity()
{
  string both_sides = "";

  if(!xmlTextReaderIsEmptyElement(reader))
  {
    string name = "";

    while(true)
    {
      xmlTextReaderRead(reader);
      name = XMLParseUtil::latin1(xmlTextReaderConstName(reader));
      if(name == Compiler::COMPILER_IDENTITY_ELEM)
      {
        break;
      }
      readString(both_sides, name);
    }
  }
  return both_sides;  
}

pair<string, string>
Expander::procTransduction()
{
  string lhs = "", rhs = ""; 
  string name = "";
  
  skip(name, Compiler::COMPILER_LEFT_ELEM);

  if(!xmlTextReaderIsEmptyElement(reader))
  {
    name = "";
    while(true)
    {
      xmlTextReaderRead(reader);
      name = XMLParseUtil::latin1(xmlTextReaderConstName(reader));
      if(name == Compiler::COMPILER_LEFT_ELEM)
      {
        break;
      }
      readString(lhs, name);
    }
  }
 
  skip(name, Compiler::COMPILER_RIGHT_ELEM);

  if(!xmlTextReaderIsEmptyElement(reader))
  {
    name = "";
    while(true)
    {
      xmlTextReaderRead(reader);
      name = XMLParseUtil::latin1(xmlTextReaderConstName(reader));
      if(name == Compiler::COMPILER_RIGHT_ELEM)
      {
        break;
      }
      readString(rhs, name);
    }    
  }

  skip(name, Compiler::COMPILER_PAIR_ELEM);  
  
  pair<string, string> e(lhs, rhs);
  return e;
}

string
Expander::attrib(string const &nombre)
{
  return XMLParseUtil::attrib(reader, nombre);
} 

string
Expander::procPar()
{
  EntryToken e;
  string nomparadigma = attrib(Compiler::COMPILER_N_ATTR);
  return nomparadigma;
}

void
Expander::requireAttribute(string const &value, string const &attrname,
                           string const &elemname)
{
  if(value == "")
  {
    cerr << "Error (" << xmlTextReaderGetParserLineNumber(reader);  
    cerr << "): '<" << elemname;
    cerr << "' element must specify non-void '";
    cerr<< attrname << "' attribute." << endl;
    exit(EXIT_FAILURE);
  }  
}

void
Expander::procEntry(FILE *output)
{
  string atributo=this->attrib(Compiler::COMPILER_RESTRICTION_ATTR);
  string entrname=this->attrib(Compiler::COMPILER_LEMMA_ATTR);
  
  EntList items, items_lr, items_rl;
  if(atributo == Compiler::COMPILER_RESTRICTION_LR_VAL)
  {
    items_lr.push_back(pair<string, string>("", ""));
  }
  else if(atributo == Compiler::COMPILER_RESTRICTION_RL_VAL)
  {
    items_rl.push_back(pair<string, string>("", ""));
  }
  else
  {
    items.push_back(pair<string, string>("",""));
  }

  while(true)
  {
    int ret = xmlTextReaderRead(reader);
    if(ret != 1)
    {
      cerr << "Error (" << xmlTextReaderGetParserLineNumber(reader);
      cerr << "): Parse error." << endl;
      exit(EXIT_FAILURE);
    }
    string name = XMLParseUtil::latin1(xmlTextReaderConstName(reader));
    skipBlanks(name);

    int tipo = xmlTextReaderNodeType(reader);
    if(name == Compiler::COMPILER_PAIR_ELEM)
    {      
      pair<string, string> p = procTransduction();
      append(items, p);
      append(items_lr, p);
      append(items_rl, p);
    }
    else if(name == Compiler::COMPILER_IDENTITY_ELEM)
    {
      string val = procIdentity();
      append(items, val);
      append(items_lr, val);
      append(items_rl, val);
    }
    else if(name == Compiler::COMPILER_REGEXP_ELEM)
    {
      string val = "__REGEXP__" + procRegExp();
      append(items, val);
      append(items_lr, val);
      append(items_rl, val);
    }
    else if(name == Compiler::COMPILER_PAR_ELEM)
    {
      string p = procPar();
      // detección del uso de paradigmas no definidos

      if(paradigm.find(p) == paradigm.end() &&
         paradigm_lr.find(p) == paradigm.end() &&
         paradigm_rl.find(p) == paradigm.end())
      {
        cerr << "Error (" << xmlTextReaderGetParserLineNumber(reader);
        cerr << "): Undefined paradigm '" << p << "'." <<endl;
        exit(EXIT_FAILURE);
      }
      
      if(atributo == Compiler::COMPILER_RESTRICTION_LR_VAL)
      {
        if(paradigm[p].size() == 0 && paradigm_lr[p].size() == 0)
        {
          skip(name, Compiler::COMPILER_ENTRY_ELEM);
          return;
        }
        EntList first = items_lr;
        append(first, paradigm[p]);
        append(items_lr, paradigm_lr[p]);
        items_lr.insert(items_lr.end(), first.begin(), first.end());
      }
      else if(atributo == Compiler::COMPILER_RESTRICTION_RL_VAL)
      {
        if(paradigm[p].size() == 0 && paradigm_rl[p].size() == 0)
        {
          skip(name, Compiler::COMPILER_ENTRY_ELEM);
          return;
        }
        EntList first = items_rl;
        append(first, paradigm[p]);
        append(items_rl, paradigm_rl[p]);
        items_rl.insert(items_rl.end(), first.begin(), first.end());
      }
      else
      {
        if(paradigm_lr[p].size() > 0)
        {
          items_lr.insert(items_lr.end(), items.begin(), items.end());
        }
        if(paradigm_rl[p].size() > 0)
        {
          items_rl.insert(items_rl.end(), items.begin(), items.end());
        }
        
        append(items_lr, paradigm_lr[p]);
        append(items_rl, paradigm_rl[p]);
        append(items, paradigm[p]);
      }
    }
    else if(name == Compiler::COMPILER_ENTRY_ELEM && tipo == XML_READER_TYPE_END_ELEMENT)
    {
      if(current_paradigm == "")
      {
        for(EntList::iterator it = items.begin(),
                                                 limit = items.end();
            it != limit; it++)
        {
          fputs_unlocked(it->first.c_str(), output);
          fputc_unlocked(':', output);
          fputs_unlocked(it->second.c_str(), output);
          fputc_unlocked('\n', output);
        }
        for(EntList::iterator it = items_lr.begin(),
                                                 limit = items_lr.end();
            it != limit; it++)
        {
          fputs_unlocked(it->first.c_str(), output);
          fputc_unlocked(':', output);
          fputc_unlocked('>', output);
          fputc_unlocked(':', output);
          fputs_unlocked(it->second.c_str(), output);
          fputc_unlocked('\n', output);
        }
        for(EntList::iterator it = items_rl.begin(),
                                                 limit = items_rl.end();
            it != limit; it++)
        {
          fputs_unlocked(it->first.c_str(), output);
          fputc_unlocked(':', output);
          fputc_unlocked('<', output);
          fputc_unlocked(':', output);
          fputs_unlocked(it->second.c_str(), output);
          fputc_unlocked('\n', output);
        }
      }
      else
      {
        paradigm_lr[current_paradigm].insert(paradigm_lr[current_paradigm].end(),
                                             items_lr.begin(), items_lr.end());
        paradigm_rl[current_paradigm].insert(paradigm_rl[current_paradigm].end(),
                                             items_rl.begin(), items_rl.end());
        paradigm[current_paradigm].insert(paradigm[current_paradigm].end(),
                                          items.begin(), items.end());
      }

      return;
    }
    else if(name == "#text" && allBlanks())
    {
    }
    else
    {
      cerr << "Error (" << xmlTextReaderGetParserLineNumber(reader);
      cerr << "): Invalid inclusion of '<" << name << ">' into '<" << Compiler::COMPILER_ENTRY_ELEM;
      cerr << ">'." << endl;
      exit(EXIT_FAILURE);
    }
  }
}

void
Expander::procNode(FILE *output)
{
  xmlChar const *xnombre = xmlTextReaderConstName(reader);
  string nombre = XMLParseUtil::latin1(xnombre);

  // HACER: optimizar el orden de ejecución de esta ristra de "ifs"

  if(nombre == "#text")
  {
    /* ignorar */
  }
  else if(nombre == Compiler::COMPILER_DICTIONARY_ELEM)
  {
    /* ignorar */
  }
  else if(nombre == Compiler::COMPILER_ALPHABET_ELEM)
  {
    /* ignorar */
  }
  else if(nombre == Compiler::COMPILER_SDEFS_ELEM)
  {
    /* ignorar */
  }
  else if(nombre == Compiler::COMPILER_SDEF_ELEM)
  {
    /* ignorar */
  }
  else if(nombre == Compiler::COMPILER_PARDEFS_ELEM)
  {
    /* ignorar */
  }
  else if(nombre == Compiler::COMPILER_PARDEF_ELEM)
  {
    procParDef();
  }
  else if(nombre == Compiler::COMPILER_ENTRY_ELEM)
  {
    procEntry(output);
  }
  else if(nombre == Compiler::COMPILER_SECTION_ELEM)
  {
    /* ignorar */
  }
  else if(nombre== "#comment")
  {
    /* ignorar */
  }
  else
  {
    cerr << "Error (" << xmlTextReaderGetParserLineNumber(reader);
    cerr << "): Invalid node '<" << nombre << ">'." << endl;
    exit(EXIT_FAILURE);
  }
}

string
Expander::procRegExp()
{
  xmlTextReaderRead(reader);
  string re = XMLParseUtil::latin1(xmlTextReaderConstValue(reader));
  xmlTextReaderRead(reader);
  return re;
}

void
Expander::append(EntList &result,
		 EntList const &endings)
{
  EntList temp;
  EntList::iterator it, limit;
  EntList::const_iterator it2, limit2;

  for(it = result.begin(), limit = result.end(); it != limit; it++)
  {
    for(it2 = endings.begin(), limit2 = endings.end(); it2 != limit2; it2++)
    {
      temp.push_back(pair<string, string>(it->first + it2->first, 
		   		          it->second + it2->second));
    }
  }

  result = temp;
}

void
Expander::append(EntList &result, string const &endings)
{  
  EntList::iterator it, limit;
  for(it = result.begin(), limit = result.end(); it != limit; it++)
  {
    it->first.append(endings);
    it->second.append(endings);
  }
}

void
Expander::append(EntList &result, 
		 pair<string, string> const &endings)
{
  EntList::iterator it, limit;
  for(it = result.begin(), limit = result.end(); it != limit; it++)
  {
    it->first.append(endings.first);
    it->second.append(endings.second);
  }
}
