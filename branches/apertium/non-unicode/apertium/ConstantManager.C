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
#include <apertium/ConstantManager.H>
#include <lttoolbox/Compression.H>

void
ConstantManager::copy(ConstantManager const &o)
{
  constants = o.constants;
}

void
ConstantManager::destroy()
{
}

ConstantManager::ConstantManager()
{
}

ConstantManager::~ConstantManager()
{
  destroy();
}

ConstantManager::ConstantManager(ConstantManager const &o)
{
  copy(o);
}

ConstantManager &
ConstantManager::operator =(ConstantManager const &o)
{
  if(this != &o)
  {
    destroy();
    copy(o);
  }
  return *this;
}
void 
ConstantManager::setConstant(string const &constant, int const value)
{
  constants[constant] = value;
}

int 
ConstantManager::getConstant(string const &constant)
{
  return constants[constant];
}  

void
ConstantManager::write(FILE *output)
{
  Compression::multibyte_write(constants.size(), output);

  for(map<string, int>::const_iterator it = constants.begin(), limit = constants.end();
      it != limit; it++)
  {
    Compression::multibyte_write(it->first.size(), output);
    fwrite(it->first.data(), sizeof(char), it->first.size(), output);
    Compression::multibyte_write(it->second, output);
  }
}

void
ConstantManager::read(FILE *input)
{
  constants.clear();
  int size = Compression::multibyte_read(input);
  for(int i = 0; i != size; i++)
  {
    int length = Compression::multibyte_read(input);
    char mystr[length + 1];
    fread(mystr, sizeof(char), length, input);
    mystr[length] = 0;
    constants[mystr] = Compression::multibyte_read(input);
  }
}

#include <iostream>

void
ConstantManager::debug()
{
  for(map<string, int>::iterator it = constants.begin(), limit = constants.end();
      it != limit; it++)
  {
    cerr << it->first << ": " << it->second << endl;
  }
}
