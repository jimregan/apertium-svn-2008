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
 * Utility functions. (header file)
 *
 *  @author   Felipe Sánchez-Martínez - fsanchez@dlsi.ua.es
 */

#ifndef __UTILS_H
#define __UTILS_H

#include <string>
#include <vector>

using namespace std;

class Utils {
public:
  static string translation_script;

  static string likelihood_script;

  static string translation_script2;

  static string likelihood_script2;

  static bool debug;

  static string translate(string script, const string& s);

  static double likelihood(string script, const string& s);

  static int get_usage_time();

  static vector<string> split_string(const string& input, const string& delimiter);

  static void print_debug(const string& s);
  static void print_debug(const double& d);
  static void print_debug(const int& i);

  static double log2(double v);
};

#endif

