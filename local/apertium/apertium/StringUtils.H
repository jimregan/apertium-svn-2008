/*
 * Copyright (C) 2006 Universitat d'Alacant / Universidad de Alicante
 * author: Felipe Sánchez-Martínez
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

class StringUtils {
  public:
  
  static string trim(string str);

  static vector<string> split_string(const string& input, const string& delimiter);

  static string vector2string(const vector<string>& v);

  //Replace each ocurrence of the string 'olds' by the string 'news' in string 'source'
  static string substitute(const string& source, const string& olds, const string& news);

  static string itoa(int n);
  
  static string ftoa(double f);

  static string tolower(const string& s);

  static string toupper(const string& s);
};

#endif
