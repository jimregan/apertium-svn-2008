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
#include <lttoolbox/Compression.H>
#include <lttoolbox/EntryToken.H>
#include <lttoolbox/LtLocale.H>
#include <lttoolbox/XMLParseUtil.H>

#include <cstdlib>
#include <iostream>
#include <libxml/encoding.h>

using namespace std;

string const Compiler::COMPILER_DICTIONARY_ELEM    = "dictionary";
string const Compiler::COMPILER_ALPHABET_ELEM      = "alphabet";
string const Compiler::COMPILER_SDEFS_ELEM         = "sdefs";
string const Compiler::COMPILER_SDEF_ELEM          = "sdef";
string const Compiler::COMPILER_N_ATTR             = "n";
string const Compiler::COMPILER_PARDEFS_ELEM       = "pardefs";
string const Compiler::COMPILER_PARDEF_ELEM        = "pardef";
string const Compiler::COMPILER_PAR_ELEM           = "par";
string const Compiler::COMPILER_ENTRY_ELEM         = "e";
string const Compiler::COMPILER_RESTRICTION_ATTR   = "r";
string const Compiler::COMPILER_RESTRICTION_LR_VAL = "LR";
string const Compiler::COMPILER_RESTRICTION_RL_VAL = "RL";
string const Compiler::COMPILER_PAIR_ELEM          = "p";
string const Compiler::COMPILER_LEFT_ELEM          = "l";
string const Compiler::COMPILER_RIGHT_ELEM         = "r";
string const Compiler::COMPILER_S_ELEM             = "s";
string const Compiler::COMPILER_REGEXP_ELEM        = "re";
string const Compiler::COMPILER_SECTION_ELEM       = "section";
string const Compiler::COMPILER_ID_ATTR            = "id";
string const Compiler::COMPILER_TYPE_ATTR	   = "type";
string const Compiler::COMPILER_IDENTITY_ELEM      = "i";
string const Compiler::COMPILER_JOIN_ELEM	   = "j";
string const Compiler::COMPILER_BLANK_ELEM	   = "b";
string const Compiler::COMPILER_POSTGENERATOR_ELEM = "a";
string const Compiler::COMPILER_GROUP_ELEM         = "g";
string const Compiler::COMPILER_LEMMA_ATTR         = "lm";
string const Compiler::COMPILER_IGNORE_ATTR        = "i";
string const Compiler::COMPILER_IGNORE_YES_VAL     = "yes";

Compiler::Compiler()
{
  LtLocale::tryToSetLocale();
}

Compiler::~Compiler()
{
}

void
Compiler::parse(string const &fichero, string const &dir)
{
  direction = dir;
  reader = xmlReaderForFile(fichero.c_str(), NULL, 0);
  if(reader == NULL)
  {
    cerr << "Error: Cannot open '" << fichero << "'." << endl;
    exit(EXIT_FAILURE);
  }

  int ret = xmlTextReaderRead(reader);
  while(ret == 1)
  {
    procNode();
    ret = xmlTextReaderRead(reader);
  }

  if(ret != 0)
  {
    cerr << "Error: Parse error at the end of input." << endl;
  }

  xmlFreeTextReader(reader);
  xmlCleanupParser();

 
  // Minimize transducers 
  for(map<string, Transducer, Ltstr>::iterator it = sections.begin(),
                                               limit = sections.end(); 
      it != limit; it++)
  {
    (it->second).minimize();
  }
}


string
Compiler::latin1(xmlChar const * entrada)
{
  return XMLParseUtil::latin1(entrada);
}

void
Compiler::procAlphabet()
{
  int tipo=xmlTextReaderNodeType(reader);

  if(tipo != XML_READER_TYPE_END_ELEMENT)
  {
    int ret = xmlTextReaderRead(reader);
    if(ret == 1)
    {          
      xmlChar const *valor = xmlTextReaderConstValue(reader);
      letters = latin1(valor);
    }
    else
    {
      cerr << "Error (" << xmlTextReaderGetParserLineNumber(reader);
      cerr << "): Missing alphabet symbols." << endl;
      exit(EXIT_FAILURE);
    }
  }
}

void
Compiler::procSDef()
{
  alphabet.includeSymbol("<"+attrib(COMPILER_N_ATTR)+">");
}

void
Compiler::procParDef()
{
  int tipo=xmlTextReaderNodeType(reader);

  if(tipo != XML_READER_TYPE_END_ELEMENT)
  {
    current_paradigm = attrib(COMPILER_N_ATTR);
  }
  else
  {
    if(!paradigms[current_paradigm].isEmpty())
    {
      paradigms[current_paradigm].minimize();
      paradigms[current_paradigm].joinFinals();
      current_paradigm = "";
    }
  }
}

int
Compiler::matchTransduction(list<string> const &pi, 
				 list<string> const &pd, 
				 int estado, Transducer &t)
{
  list<string>::const_iterator izqda, dcha, limizqda, limdcha;

  if(direction == COMPILER_RESTRICTION_LR_VAL)
  {
    izqda = pi.begin();
    dcha = pd.begin();
    limizqda = pi.end();
    limdcha = pd.end();
  }
  else
  {
    izqda = pd.begin();
    dcha = pi.begin();
    limizqda = pd.end();
    limdcha = pi.end();
  }
 

  if(pi.size() == 0 && pd.size() == 0)
  {
    estado = t.insertNewSingleTransduction(alphabet("", ""), estado);
  }
  else
  {
    while(true)
    {
      int etiqueta;

      if(izqda == limizqda && dcha == limdcha)
      {
        break;
      }
      else if(izqda == limizqda)
      {
        etiqueta = alphabet("", *dcha);
        dcha++;
      }
      else if(dcha == limdcha)
      {
        etiqueta = alphabet(*izqda, "");
        izqda++;
      }
      else
      {
        etiqueta = alphabet(*izqda, *dcha);
        izqda++;
        dcha++;
      }

      estado = t.insertSingleTransduction(etiqueta, estado);
    }
  }

  return estado;
}


void
Compiler::requireEmptyError(string const &name)
{
  if(!xmlTextReaderIsEmptyElement(reader))
  {
    cerr << "Error (" << xmlTextReaderGetParserLineNumber(reader);
    cerr << "): Non-empty element '<" << name << ">' should be empty." << endl;
    exit(EXIT_FAILURE);
  }
}

bool 
Compiler::allBlanks()
{
  bool flag = true;
  string text = latin1(xmlTextReaderConstValue(reader));
  
  for(unsigned int i = 0, limit = text.size(); i < limit; i++)
  {
    flag = flag && isspace(text[i]);
  }
  
  return flag;
}

void 
Compiler::readString(list<string> &result, string const &name)
{
  if(name == "#text")
  {
    string value = latin1(xmlTextReaderConstValue(reader));
    for(unsigned int i = 0, limit = value.size(); i < limit; i++)
    {
      result.push_back(value.substr(i, 1));
    }
  }
  else if(name == COMPILER_BLANK_ELEM)
  {
    requireEmptyError(name);
    result.push_back(" ");
  }
  else if(name == COMPILER_JOIN_ELEM)
  {
    requireEmptyError(name);
    result.push_back("+");
  }
  else if(name == COMPILER_POSTGENERATOR_ELEM)
  {
    requireEmptyError(name);
    result.push_back("~");
  }
  else if(name == COMPILER_GROUP_ELEM)
  {
    int tipo=xmlTextReaderNodeType(reader);
    if(tipo != XML_READER_TYPE_END_ELEMENT)
    {
      result.push_back("#");
    }
  }
  else if(name == COMPILER_S_ELEM)
  {
    requireEmptyError(name);
    string symbol = "<" + attrib(COMPILER_N_ATTR) + ">";

    if(!alphabet.isSymbolDefined(symbol))
    {
      cerr << "Error (" << xmlTextReaderGetParserLineNumber(reader);
      cerr << "): Undefined symbol '" << symbol << "'." << endl;
      exit(EXIT_FAILURE);
    }
    
    result.push_back(symbol);
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
Compiler::skipBlanks(string &name)
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
    name = latin1(xmlTextReaderConstName(reader));
  }
}

void
Compiler::skip(string &name, string const &elem)
{
  xmlTextReaderRead(reader);
  name = latin1(xmlTextReaderConstName(reader));
  
  if(name == "#text")
  {
    if(!allBlanks())
    {
      cerr << "Error (" << xmlTextReaderGetParserLineNumber(reader);
      cerr << "): Invalid construction." << endl;
      exit(EXIT_FAILURE);
    }
    xmlTextReaderRead(reader);
    name = latin1(xmlTextReaderConstName(reader));
  }    
    
  if(name != elem)
  {
    cerr << "Error (" << xmlTextReaderGetParserLineNumber(reader);
    cerr << "): Expected '<" << elem << ">'." << endl;
    exit(EXIT_FAILURE);
  }  
}

EntryToken
Compiler::procIdentity()
{
  list<string> both_sides;

  if(!xmlTextReaderIsEmptyElement(reader))
  {
    string name = "";

    while(true)
    {
      xmlTextReaderRead(reader);
      name = latin1(xmlTextReaderConstName(reader));
      if(name == COMPILER_IDENTITY_ELEM)
      {
        break;
      }
      readString(both_sides, name);
    }
  }
  
  EntryToken e;
  e.setSingleTransduction(both_sides, both_sides);
  return e;
}

EntryToken
Compiler::procTransduction()
{
  list<string> lhs, rhs;
  string name;
  
  skip(name, COMPILER_LEFT_ELEM);

  if(!xmlTextReaderIsEmptyElement(reader))
  {
    name = "";
    while(true)
    {
      xmlTextReaderRead(reader);
      name = latin1(xmlTextReaderConstName(reader));
      if(name == COMPILER_LEFT_ELEM)
      {
        break;
      }
      readString(lhs, name);
    }
  }
 
  skip(name, COMPILER_RIGHT_ELEM);

  if(!xmlTextReaderIsEmptyElement(reader))
  {
    name = "";
    while(true)
    {
      xmlTextReaderRead(reader);
      name = latin1(xmlTextReaderConstName(reader));
      if(name == COMPILER_RIGHT_ELEM)
      {
        break;
      }
      readString(rhs, name);
    }    
  }

  skip(name, COMPILER_PAIR_ELEM);  
  
  EntryToken e;
  e.setSingleTransduction(lhs, rhs);
  return e;
}

string
Compiler::attrib(string const &name)
{
  return XMLParseUtil::attrib(reader, name);
} 

EntryToken
Compiler::procPar()
{
  EntryToken e;
  string nomparadigma = attrib(COMPILER_N_ATTR);

  if(paradigms.find(nomparadigma) == paradigms.end())
  {
    cerr << "Error (" << xmlTextReaderGetParserLineNumber(reader);
    cerr << "): Undefined paradigm '" << nomparadigma << "'." << endl;
    exit(EXIT_FAILURE);
  }
  e.setParadigm(nomparadigma);
  return e;
}

void
Compiler::insertEntryTokens(vector<EntryToken> const &elementos)
{
  if(current_paradigm != "")
  {
    // compilation of paradigms
    Transducer &t = paradigms[current_paradigm];
    int e = t.getInitial();
    
    for(unsigned int i = 0, limit = elementos.size(); i < limit; i++)
    {
      if(elementos[i].isParadigm())
      {
	e = t.insertTransducer(e, paradigms[elementos[i].paradigmName()]);
      }
      else if(elementos[i].isSingleTransduction())
      {
        e = matchTransduction(elementos[i].left(), 
                                  elementos[i].right(), e, t);
      }
      else if(elementos[i].isRegExp())
      {
	RegExpCompiler analizador;
	analizador.initialize(alphabet);
	analizador.compile(elementos[i].regExp());
	e = t.insertTransducer(e, analizador.getTransducer(), alphabet("",""));
      }
      else
      {
        cerr << "Error (" << xmlTextReaderGetParserLineNumber(reader);
        cerr << "): Invalid entry token." << endl;
        exit(EXIT_FAILURE);
      }
    }
    t.setFinal(e);
  }
  else
  {
    // compilación de dictionary

    Transducer &t = sections[current_section];
    int e = t.getInitial();

    for(unsigned int i = 0, limit = elementos.size(); i < limit; i++)
    {
      if(elementos[i].isParadigm())
      {
        if(i == elementos.size()-1)
	{
	  // paradigma sufijo
	  if(suffix_paradigms[current_section].find(elementos[i].paradigmName()) != suffix_paradigms[current_section].end())
	  {
	    t.linkStates(e, suffix_paradigms[current_section][elementos[i].paradigmName()], 0);
            e = postsuffix_paradigms[current_section][elementos[i].paradigmName()];
	  }
          else
          {
            e = t.insertNewSingleTransduction(alphabet("", ""), e);
            suffix_paradigms[current_section][elementos[i].paradigmName()] = e;
            e = t.insertTransducer(e, paradigms[elementos[i].paradigmName()]);
            postsuffix_paradigms[current_section][elementos[i].paradigmName()] = e;
	  }
	}
        else if(i == 0)
	{
          // paradigma prefijo
	  if(prefix_paradigms[current_section].find(elementos[i].paradigmName()) != prefix_paradigms[current_section].end())
	  {
            e = prefix_paradigms[current_section][elementos[i].paradigmName()];
	  }
	  else
	  {
            e = t.insertTransducer(e, paradigms[elementos[i].paradigmName()]);
            prefix_paradigms[current_section][elementos[i].paradigmName()] = e;
	  }
        }
	else
	{
          // paradigma intermedio
          e = t.insertTransducer(e, paradigms[elementos[i].paradigmName()]);
	}
      }
      else if(elementos[i].isRegExp())
      {
	RegExpCompiler analizador;
	analizador.initialize(alphabet);
	analizador.compile(elementos[i].regExp());
	e = t.insertTransducer(e, analizador.getTransducer(), alphabet("",""));
      }
      else
      {
        e = matchTransduction(elementos[i].left(), elementos[i].right(), e, t);
      }
    }
    t.setFinal(e);
  }
}


void
Compiler::requireAttribute(string const &value, string const &attrname,
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
Compiler::procSection()
{
  int tipo=xmlTextReaderNodeType(reader);

  if(tipo != XML_READER_TYPE_END_ELEMENT)
  {
    string const &id = attrib(COMPILER_ID_ATTR);
    string const &type = attrib(COMPILER_TYPE_ATTR);
    requireAttribute(id, COMPILER_ID_ATTR, COMPILER_SECTION_ELEM);
    requireAttribute(type, COMPILER_TYPE_ATTR, COMPILER_SECTION_ELEM);
    
    current_section = id;
    current_section += "@";
    current_section.append(type);
  }
  else
  {
    current_section = "";
  }
}

void
Compiler::procEntry()
{
  string atributo=this->attrib(COMPILER_RESTRICTION_ATTR);
  string ignore = this->attrib(COMPILER_IGNORE_ATTR);

  // if entry is masked by a restriction of direction or an ignore mark
  if((atributo != "" && atributo != direction) || ignore == COMPILER_IGNORE_YES_VAL)
  {
    // parse to the end of the entry
    string name = "";

    while(name != COMPILER_ENTRY_ELEM)
    {
      xmlTextReaderRead(reader);
      name = latin1(xmlTextReaderConstName(reader));
    }

    return;
  }

  vector<EntryToken> elementos;

  while(true)
  {
    int ret = xmlTextReaderRead(reader);
    if(ret != 1)
    {
      cerr << "Error (" << xmlTextReaderGetParserLineNumber(reader);
      cerr << "): Parse error." << endl;
      exit(EXIT_FAILURE);
    }
    string name = latin1(xmlTextReaderConstName(reader));
    skipBlanks(name);

    int tipo = xmlTextReaderNodeType(reader);
    if(name == COMPILER_PAIR_ELEM)
    {      
      elementos.push_back(procTransduction());
    }
    else if(name == COMPILER_IDENTITY_ELEM)
    {
      elementos.push_back(procIdentity());
    }
    else if(name == COMPILER_REGEXP_ELEM)
    {
      elementos.push_back(procRegExp());
    }
    else if(name == COMPILER_PAR_ELEM)
    {
      elementos.push_back(procPar());

      // detección del uso de paradigmas no definidos

      string const &p = elementos.rbegin()->paradigmName();

      if(paradigms.find(p) == paradigms.end())
      {
        cerr << "Error (" << xmlTextReaderGetParserLineNumber(reader);
        cerr << "): Undefined paradigm '" << p << "'." <<endl;
        exit(EXIT_FAILURE);
      }
      // descartar entradas con paradigms vacíos (por las direciones,
      // normalmente
      if(paradigms[p].isEmpty())
      {
        while(name != COMPILER_ENTRY_ELEM || tipo != XML_READER_TYPE_END_ELEMENT)
        {
          xmlTextReaderRead(reader);
          name = latin1(xmlTextReaderConstName(reader));
          tipo = xmlTextReaderNodeType(reader);
        }
        return;
      }
    }
    else if(name == COMPILER_ENTRY_ELEM && tipo == XML_READER_TYPE_END_ELEMENT)
    {
      // insertar los elementos de la entrada en el TL
      insertEntryTokens(elementos);
      return;
    }
    else if(name == "#text" && allBlanks())
    {
    }
    else if(name == "#comment")
    {
      // skip
    }
    else
    {
      cerr << "Error (" << xmlTextReaderGetParserLineNumber(reader);
      cerr << "): Invalid inclusion of '<" << name << ">' into '<" << COMPILER_ENTRY_ELEM;
      cerr << ">'." << endl;
      exit(EXIT_FAILURE);
    }
  }
}

void
Compiler::procNode()
{
  xmlChar const *xnombre = xmlTextReaderConstName(reader);
  string nombre = latin1(xnombre);

  // HACER: optimizar el orden de ejecución de esta ristra de "ifs"

  if(nombre == "#text")
  {
    /* ignorar */
  }
  else if(nombre == COMPILER_DICTIONARY_ELEM)
  {
    /* ignorar */
  }
  else if(nombre == COMPILER_ALPHABET_ELEM)
  {
    procAlphabet();
  }
  else if(nombre == COMPILER_SDEFS_ELEM)
  {
    /* ignorar */
  }
  else if(nombre == COMPILER_SDEF_ELEM)
  {
    procSDef();
  }
  else if(nombre == COMPILER_PARDEFS_ELEM)
  {
    /* ignorar */
  }
  else if(nombre == COMPILER_PARDEF_ELEM)
  {
    procParDef();
  }
  else if(nombre == COMPILER_ENTRY_ELEM)
  {
    procEntry();
  }
  else if(nombre == COMPILER_SECTION_ELEM)
  {
    procSection();
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

EntryToken
Compiler::procRegExp()
{
  EntryToken et;
  xmlTextReaderRead(reader);
  string re = latin1(xmlTextReaderConstValue(reader));
  et.setRegExp(re);
  xmlTextReaderRead(reader);
  return et;
}

void 
Compiler::write(FILE *output)
{
  // letters
  Compression::multibyte_write(letters.size(), output);
  fwrite(letters.data(), sizeof(char), letters.size(), output);
  
  // symbols
  alphabet.write(output);
   
  // transducers
  Compression::multibyte_write(sections.size(), output);

  int conta=0;
  for(map<string, Transducer, Ltstr>::iterator it = sections.begin(),
                                               limit = sections.end(); 
      it != limit; it++)
  {
    conta++;
    Compression::multibyte_write(it->first.size(), output);
    cout << it->first << " " << it->second.size();
    cout << " " << it->second.numberOfTransitions() << endl;
    fwrite(it->first.data(), sizeof(char), it->first.size(), output);
    it->second.write(output);
  }
}
