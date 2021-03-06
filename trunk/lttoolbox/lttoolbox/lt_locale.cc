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
#include <lttoolbox/lt_locale.h>

#include <clocale>
#include <iostream>

using namespace std;


void
LtLocale::tryToSetLocale()
{
  if(setlocale(LC_CTYPE, "") != NULL)
  {
    return;
  }
 
  wcerr << L"Warning: unsupported locale, fallback to \"C\"" << endl;
  setlocale(LC_ALL, "C");
}
