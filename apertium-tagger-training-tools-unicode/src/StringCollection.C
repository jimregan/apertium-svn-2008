/*
 * Copyright (C) 2004-2006 Felipe Sánchez-Martínez
 * Copyright (C) 2006 Universitat d'Alacant
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
/**
 * StringCollection (source file)
 *
 * Implementation of a collection of strings.
 *
 *  @author   Felipe Sánchez-Martínez - fsanchez@dlsi.ua.es
 */

#include "StringCollection.H"

int 
StringCollection::size (void) {
  return element.size();
}

bool 
StringCollection::has_not (const wstring& s) { 
  return index.find(s)==index.end(); 
}

const wstring& 
StringCollection::operator[] (int n) { 
  return  *element[n]; 
}

int& 
StringCollection::operator[] (const wstring& s) {
  if (has_not(s)) {
    index[s] = index.size()-1;
    element.push_back(&(index.find(s)->first));
  }
  return index[s];
}

int& 
StringCollection::add(const wstring& s) {
  index[s] = index.size()-1;
  element.push_back(&(index.find(s)->first));  
  return index[s];
}
