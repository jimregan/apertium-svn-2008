/*
 * Copyright (C) 2006 Universitat d'Alacant / Universidad de Alicante
 * author: Felipe S�nchez-Mart�nez
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
#ifndef __STRINGUTILS_H_
#define __STRINGUTILS_H_

#include <string>
#include <vector>

using namespace std;

namespace Apertium
{
bool operator==(string const &s1, string const &s2);
}

class StringUtils {
  public:
  
  static wstring trim(wstring const &str);

  static vector<wstring> split_wstring(wstring const &input, wstring const &delimiter);

  static wstring vector2wstring(vector<wstring> const &v);

  //Replace each ocurrence of the string 'olds' by the string 'news' in string 'source'
  static wstring substitute(const wstring &source, const wstring &olds, const wstring &news);

  static wstring itoa(int n);
  
  static wstring ftoa(double f);

  static wstring tolower(wstring const &s);

  static wstring toupper(wstring const &s);
};

#endif
