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

#include <apertium/TransferWord.H>
#include <iostream>

map<char const *, regex_t>  TransferWord::compiled_regexes;

void
TransferWord::copy(TransferWord const &o)
{
  s = o.s;
  t = o.t;
  snq = o.snq;
  tnq = o.tnq;
  s_str = o.s_str;
  t_str = o.t_str;
  queue_length = o.queue_length;
}

void
TransferWord::destroy()
{
}

string
TransferWord::access(map<char const *, pair<int, int> > &m, string const &str, char const *part)
{
  map<char const *, pair<int, int> >::iterator it = m.find(part);
 
  if(it == m.end())
  {
    regmatch_t result;
    int errorcode = regexec(&(getRegex(part)), str.c_str(), 1, &result, 0);

    if(errorcode != 0)
    {
      m[part] = pair<int, int>(0,0);
      return "";
    }
    else
    {
      m[part] = pair<int, int>(result.rm_so, result.rm_eo);
      return str.substr(result.rm_so, result.rm_eo - result.rm_so);
    }
  }
  else
  {
    return str.substr(it->second.first, it->second.second - it->second.first);
  }
}

regex_t &
TransferWord::getRegex(const char *part)
{
  map<char const *, regex_t>::iterator it;
  it = TransferWord::compiled_regexes.find(part);

  if(it != compiled_regexes.end())
  {
    return it->second;
  }
  else
  {
    int errorcode = regcomp(&(TransferWord::compiled_regexes[part]),
			    part, REG_EXTENDED|REG_ICASE);
    regexError(part, errorcode);
    return compiled_regexes[part];
  }
}

void 
TransferWord::regexError(char const *message, int const errorcode)
{
  if(errorcode != 0)
  {
    cerr << "Error: Compilation for regular expression for '"
         << message << "' failed." << endl;
    exit(EXIT_FAILURE);
  } 
}

TransferWord::TransferWord()
{
}

TransferWord::TransferWord(string const &src, string const &tgt, int queue)
{
  init(src, tgt);
  queue_length = queue;
}

TransferWord::~TransferWord()
{
  destroy();
}

TransferWord::TransferWord(TransferWord const &o)
{
  copy(o);
}

TransferWord &
TransferWord::operator =(TransferWord const &o)
{
  if(this != &o)
  {
    destroy();
    copy(o);
  }
  return *this;
}

void
TransferWord::init(string const &src, string const &tgt)
{
  s.clear();
  t.clear();
  s_str = src;
  t_str = tgt;
}

string
TransferWord::source(char const *part, bool with_queue)
{
  if(with_queue)
  {
    return access(s, s_str, part);
  }
  else
  {
    return access(snq, s_str.substr(0, s_str.size() - queue_length), part);
  }
}

string
TransferWord::target(char const *part, bool with_queue)
{
  if(with_queue)
  {
    return access(t, t_str, part);
  }
  else
  {
    return access(tnq, t_str.substr(0, t_str.size() - queue_length), part);
  }
}

void
TransferWord::assign(map<char const *, pair<int, int> > &m, string &str, char const *part,
	      string const &value)
{
  map<char const *, pair<int, int> >::iterator it = m.find(part);

  if(it == m.end())
  {
    regmatch_t result;

    int errorcode = regexec(&(getRegex(part)), str.c_str(), 1, &result, 0);
    
    if(errorcode != 0)
    {
      return;
    }
    m[part] = pair<int, int>(result.rm_so, result.rm_eo);
  }

  string newstr = str.substr(0, m[part].first);
  newstr.append(value);
  newstr.append(str.substr(m[part].second));
  str = newstr;

  int dif = value.size() - (m[part].second - m[part].first);
  int const lowlimit = m[part].first;
  int const uplimit = m[part].second;
  
  for(it = m.begin(); it != m.end(); it++)
  {
    if(it->second.first == it->second.second)
    { 
      m.erase(it);
    }
    else if(it->second.first >= uplimit)
    {
      it->second.first += dif;
      it->second.second += dif;
    } 
    else if(it->second.first > lowlimit)
    {
      // invalidate overlapping patterns
      m.erase(it);
    }
    else if(it->second.second >= uplimit)
    {
      it->second.second += dif;
    }
    else if(it->second.second > lowlimit)
    {
      // invalidate overlapping patterns
      m.erase(it);
    }
  } 
  
  //m.clear();
}

void
TransferWord::setSource(char const *part, string const &value, bool with_queue)
{
  if(with_queue)
  {
    assign(s, s_str, part, value);
  }
  else
  {
    string mystring = s_str.substr(0, s_str.size() - queue_length);
    assign(snq, mystring, part, value);
    s_str = mystring + s_str.substr(s_str.size() - queue_length);
  }
}

void
TransferWord::setTarget(char const *part, string const &value, bool with_queue)
{
  if(with_queue)
  {
    assign(t, t_str, part, value);
  }
  else
  {
    string mystring = t_str.substr(0, t_str.size() - queue_length);
    assign(tnq, mystring, part, value);
    t_str = mystring + t_str.substr(t_str.size() - queue_length);
  }
}

