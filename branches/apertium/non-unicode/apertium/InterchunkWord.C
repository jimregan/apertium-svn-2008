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

#include <apertium/InterchunkWord.H>
#include <iostream>

map<char const *, regex_t>  InterchunkWord::compiled_regexes;

void
InterchunkWord::copy(InterchunkWord const &o)
{
  this->segment = o.segment;
  this->chunk = o.chunk;
}

void
InterchunkWord::destroy()
{
}

regex_t &
InterchunkWord::getRegex(const char *part)
{
  map<char const *, regex_t>::iterator it;
  it = InterchunkWord::compiled_regexes.find(part);

  if(it != compiled_regexes.end())
  {
    return it->second;
  }
  else
  {
    int errorcode = regcomp(&(InterchunkWord::compiled_regexes[part]),
			    part, REG_EXTENDED|REG_ICASE);
    regexError(part, errorcode);
    return compiled_regexes[part];
  }
}

void 
InterchunkWord::regexError(char const *message, int const errorcode)
{
  if(errorcode != 0)
  {
    cerr << "Error: Compilation for regular expression for '"
         << message << "' failed." << endl;
    exit(EXIT_FAILURE);
  } 
}

InterchunkWord::InterchunkWord()
{
}

InterchunkWord::InterchunkWord(string const &chunk)
{
  init(chunk);
}

InterchunkWord::~InterchunkWord()
{
  destroy();
}

InterchunkWord::InterchunkWord(InterchunkWord const &o)
{
  copy(o);
}

InterchunkWord &
InterchunkWord::operator =(InterchunkWord const &o)
{
  if(this != &o)
  {
    destroy();
    copy(o);
  }
  return *this;
}

void
InterchunkWord::init(string const &chunk)
{
  segment.clear();
  this->chunk = chunk;
}

string
InterchunkWord::chunkPart(char const *part)
{
  map<char const *, pair<int, int> >::iterator it = segment.find(part);
 
  if(it == segment.end())
  {
    regmatch_t result;
    int errorcode = regexec(&(getRegex(part)), chunk.c_str(), 1, &result, 0);

    if(errorcode != 0)
    {
      segment[part] = pair<int, int>(0,0);
      return "";
    }
    else
    {
      segment[part] = pair<int, int>(result.rm_so, result.rm_eo);
      return chunk.substr(result.rm_so, result.rm_eo - result.rm_so);
    }
  }
  else
  {
    return chunk.substr(it->second.first, it->second.second - it->second.first);
  }
}

void
InterchunkWord::setChunkPart(char const *part, string const &value)
{
  map<char const *, pair<int, int> >::iterator it = segment.find(part);

  if(it == segment.end())
  {
    regmatch_t result;

    int errorcode = regexec(&(getRegex(part)), chunk.c_str(), 1, &result, 0);
    
    if(errorcode != 0)
    {
      return;
    }
    segment[part] = pair<int, int>(result.rm_so, result.rm_eo);
  }

  string newstr = chunk.substr(0, segment[part].first);
  newstr.append(value);
  newstr.append(chunk.substr(segment[part].second));
  chunk = newstr;

  int dif = value.size() - (segment[part].second - segment[part].first);
  int const lowlimit = segment[part].first;
  int const uplimit = segment[part].second;
  for(it = segment.begin(); it != segment.end(); it++)
  {
    if(it->second.first >= uplimit)
    {
      it->second.first += dif;
      it->second.second += dif;
    } 
    else if(it->second.first > lowlimit)
    {
      // invalidate overlapping patterns
      segment.erase(it);
    }
    else if(it->second.second >= uplimit)
    {
      it->second.second += dif;
    }
    else if(it->second.second > lowlimit)
    {
      // invalidate overlapping patterns
      segment.erase(it);
    }
  }
}
