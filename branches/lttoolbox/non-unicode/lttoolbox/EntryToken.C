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
#include <lttoolbox/EntryToken.H>


EntryToken::EntryToken()
{
}

EntryToken::~EntryToken()
{
  destroy();
}

EntryToken::EntryToken(EntryToken const &e)
{
  copy(e);
}

EntryToken &
EntryToken::operator =(EntryToken const &e)
{
  if(this != &e)
  {
    destroy();
    copy(e);
  }
 
  return *this;
}

void
EntryToken::copy(EntryToken const &e)
{
  type = e.type;
  leftSide = e.leftSide;
  rightSide = e.rightSide;
  parName = e.parName;
  myregexp = e.myregexp;
}  

void
EntryToken::destroy()
{
}

void
EntryToken::setParadigm(string const &np)
{
  parName = np;
  type = paradigm;
}

void
EntryToken::setSingleTransduction(list<string> const &pi, list<string> const &pd)
{
  leftSide = pi;
  rightSide = pd;
  type = single_transduction;
}

void
EntryToken::setRegExp(string const &r)
{
  myregexp = r;
  type = regexp;
}

bool
EntryToken::isParadigm() const
{
  return type == paradigm;
}

bool
EntryToken::isSingleTransduction() const
{
  return type == single_transduction;
}

bool
EntryToken::isRegExp() const
{
  return type == regexp;
}

string const &
EntryToken::paradigmName() const
{
  return parName;
}

list<string> const &
EntryToken::left() const
{
  return leftSide;
}

list<string> const & 
EntryToken::right() const
{
  return rightSide;
}

string const &
EntryToken::regExp() const
{
  return myregexp;
}
