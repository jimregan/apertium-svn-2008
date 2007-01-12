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

#include <apertium/TransferData.H>
#include <lttoolbox/Compression.H>

void
TransferData::copy(TransferData const &o)
{
  alphabet = o.alphabet;
  transducer = o.transducer;
  finals = o.finals;
  attr_items = o.attr_items;
  macros = o.macros;
  lists = o.lists;
  variables = o.variables;
}

void
TransferData::destroy()
{
}

TransferData::TransferData()
{
}

TransferData::~TransferData()
{
  destroy();
}

TransferData::TransferData(TransferData const &o)
{
  copy(o);
}

TransferData &
TransferData::operator =(TransferData const &o)
{
  if(this != &o)
  {
    destroy();
    copy(o);
  }
  return *this;
}

Alphabet &
TransferData::getAlphabet()
{
  return alphabet;
}

Transducer & 
TransferData::getTransducer()
{
  return transducer;
}

map<int, int> & 
TransferData::getFinals()
{
  return finals;
}

map<string, string> &
TransferData::getAttrItems()
{
  return attr_items;
}

map<string, int> &
TransferData::getMacros()
{
  return macros;
}

map<string, set<string> > &
TransferData::getLists()
{
  return lists;
}

map<string, string> &
TransferData::getVariables()
{
  return variables;
}
  
void 
TransferData::write(FILE *output)
{
  alphabet.write(output);
  transducer.write(output);  

  // finals

  Compression::multibyte_write(finals.size(), output);  
  for(map<int, int>::const_iterator it = finals.begin(), limit = finals.end();
      it != limit; it++)
  {
    Compression::multibyte_write(it->first, output);
    Compression::multibyte_write(it->second, output);
  }

  // attr_items
  Compression::multibyte_write(attr_items.size(), output);
  for(map<string, string>::const_iterator it = attr_items.begin(), limit = attr_items.end();
      it != limit; it++)
  {
    Compression::multibyte_write(it->first.size(), output);
    fwrite(it->first.data(), sizeof(char), it->first.size(), output);
    Compression::multibyte_write(it->second.size(), output);
    fwrite(it->second.data(), sizeof(char), it->second.size(), output);
  }
  
  // variables
  Compression::multibyte_write(variables.size(), output);
  for(map<string, string>::const_iterator it = variables.begin(), limit = variables.end();
      it != limit; it++)
  {
    Compression::multibyte_write(it->first.size(), output);
    fwrite(it->first.data(), sizeof(char), it->first.size(), output);
    Compression::multibyte_write(it->second.size(), output);
    fwrite(it->second.data(), sizeof(char), it->second.size(), output);
  }

  // macros
  Compression::multibyte_write(macros.size(), output);
  for(map<string, int>::const_iterator it = macros.begin(), limit = macros.end();
      it != limit; it++)
  {
    Compression::multibyte_write(it->first.size(), output);
    fwrite(it->first.data(), sizeof(char), it->first.size(), output);
    Compression::multibyte_write(it->second, output);
  }

  // lists
  Compression::multibyte_write(lists.size(), output);
  for(map<string, set<string> >::const_iterator it = lists.begin(), limit = lists.end();
      it != limit; it++)
  {
    Compression::multibyte_write(it->first.size(), output);
    fwrite(it->first.data(), sizeof(char), it->first.size(), output);
    Compression::multibyte_write(it->second.size(), output);
   
    for(set<string>::const_iterator it2 = it->second.begin(), limit2 = it->second.end();
	it2 != limit2; it2++)
    {
      Compression::multibyte_write(it2->size(), output);
      fwrite(it2->data(), sizeof(char), it2->size(), output);
    }
  }
}

