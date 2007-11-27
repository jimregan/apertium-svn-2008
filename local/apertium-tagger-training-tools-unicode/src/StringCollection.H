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
 * StringCollection (header file)
 *
 * Implementation of a collection of strings.
 *
 *  @author   Felipe Sánchez-Martínez - fsanchez@dlsi.ua.es
 */

#ifndef __STRING_COLLECTION_H
#define __STRING_COLLECTION_H

#include <string>
#include <map>
#include <vector>

using namespace std;

class StringCollection {
private:
  map <wstring, int> index;
  vector <const wstring*> element;

public:

  /** Returns the collection's size. 
   */
  int size (void);

  /** Checks whether the collection has or not the string received as
   *  a parameter.  
   *  @param s string
   *  @return true if s is not in the collection
   */
  bool has_not (const wstring& s);

  /** @param n position in the collection
   *  @return the string at the n-th position
   */
  const wstring& operator[] (int n);

  /** If the string received as a parameter does not appear in the
   *  collection, it is added at the end.  
   *  @param s an string
   *   @return the position in which s appears in the collection.
   */
  int& operator[] (const wstring& s);

  /** Adds an string to the collection
   *  @param s the string to be added
   */  
  int& add(const wstring& s);
};

#endif
