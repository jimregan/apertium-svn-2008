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
#include <lttoolbox/Alphabet.H>
#include <lttoolbox/Compression.H>
#include <lttoolbox/MyStdio.H>

#include <cctype>
#include <cstdlib>
#include <set>

Alphabet::Alphabet()
{
  string str = " ";

  includeSymbol("");

  for(int i = 1; i <256; i++)
  {
    str[0] = static_cast<char>(i);
    includeSymbol(str);
  }
}

Alphabet::~Alphabet()
{
  destroy();
}

Alphabet::Alphabet(Alphabet const &a)
{
  copy(a);
}

Alphabet &
Alphabet::operator =(Alphabet const &a)
{
  if(this != &a)
  { 
    destroy();
    copy(a);
  }
  return *this;
}

void
Alphabet::destroy()
{
}

void 
Alphabet::copy(Alphabet const &a)
{
  slexic = a.slexic;
  slexicinv = a.slexicinv;
}

void
Alphabet::includeSymbol(string const &s)
{
  if(slexic.find(s) == slexic.end())
  {
    int tam = slexic.size();
    slexic[s] = tam;
    slexicinv.push_back(s);
  }
}

int
Alphabet::operator()(string const &s1, string const &s2)
{
  return (int const) slexic[s1] * slexic.size() + (int const) slexic[s2];
}

int
Alphabet::operator()(string const &s1, char const c2)
{
  string s2 = " ";
  s2[0] = c2;

  return (*this)(s1, s2);
}

int
Alphabet::operator()(char const c1, string const &s2)
{
  string s1 = " ";
  s1[0] = c1;

  return (*this)(s1, s2);
}

int
Alphabet::operator()(char const c1, char const c2)
{
  string s1 = " ";
  string s2 = " ";
  
  s1[0] = c1;
  s2[0] = c2;
   
  return (*this)(s1, s2);
}

int
Alphabet::operator()(string const &s)
{
  return slexic[s];
}

string const &
Alphabet::firstSymbol(int const key) const
{
  return slexicinv[key/slexic.size()];
}

string const &
Alphabet::secondSymbol(int const key) const
{
  return slexicinv[key%slexic.size()];
}

bool
Alphabet::isSymbolDefined(string const &s)
{
  return slexic.find(s) != slexic.end();
}

int
Alphabet::size() const
{
  return slexic.size();
}

void
Alphabet::write(FILE *output)
{
  set<string> lista;

  for(unsigned int i = 0, limit = slexicinv.size(); i < limit; i++)
  {
    if(slexicinv[i].size() >= 3)
    {
      lista.insert(slexicinv[i].substr(1, slexicinv[i].size()-2));
    }
  }

  Compression::multibyte_write(lista.size(), output);
  for(set<string>::iterator it = lista.begin(), limit = lista.end(); 
      it != limit; it++)
  {
    Compression::multibyte_write(it->size(), output);
    fwrite(it->data(), sizeof(char), it->size(), output);
    Compression::multibyte_write(slexic["<" + *it + ">"], output);
  }
}

void
Alphabet::read(FILE *input)
{
  Alphabet a_new;

  int tam = Compression::multibyte_read(input);

  map<int, string> tmp;
  while(tam > 0)
  {
    tam--;
    int longitud = Compression::multibyte_read(input);
    char mystr[longitud+1];
    fread(mystr, sizeof(char), longitud, input);
    mystr[longitud] = 0;
    int val = Compression::multibyte_read(input);
    tmp[val] = mystr;
  }

  for(map<int, string>::iterator it = tmp.begin(), limit = tmp.end(); 
      it != limit; it++)
  {
    a_new.includeSymbol("<" + it->second +">");
  }

  *this = a_new;
}

void
Alphabet::writeSymbol(int const symbol, FILE *output) const
{
  for(unsigned int i = 0, limit = slexicinv[symbol].size(); 
      i < limit; i++)
  {
    fputc_unlocked(slexicinv[symbol][i], output);
  }
}

void
Alphabet::getSymbol(string &result, int const symbol, bool uppercase) const
{
  if(symbol == 0)
  {
    return;
  }
  
  if(!uppercase)
  {
    if(symbol < 256)
    {
      result += char(symbol);
    }
    else
    {
      result.append(slexicinv[symbol]);
    }
  }
  else if(symbol < 256)
  {
    result += char(toupper(symbol));
  }
  else
  {
    result.append(slexicinv[symbol]);
  }
}

string const &
Alphabet::getSymbol(int const symbol, bool uppercase) const
{
  if(uppercase)
  {
    if(slexicinv[symbol].size() == 1)
    {
      return slexicinv[toupper(symbol)];
    }
  }
  return slexicinv[symbol];
}

bool
Alphabet::isTag(int const symbol) const
{
  return symbol >= 256;
}
