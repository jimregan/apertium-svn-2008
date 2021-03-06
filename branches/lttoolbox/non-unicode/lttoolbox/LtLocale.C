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
#include <lttoolbox/LtLocale.H>

#include <clocale>
#include <iostream>

using namespace std;


char * const
LtLocale::supported_locales[] = { "es_ES.ISO-8859-1", "es_ES.ISO-8859-15",
                                "es_ES@euro", "ca_ES.ISO-8859-1",
                                "ca_ES.ISO-8859-15", "ca_ES@euro",
                                "gl_ES.ISO-8859-1", "gl_ES.ISO-8859-15",
                                "gl_ES@euro", NULL };

void
LtLocale::tryToSetLocale()
{
  for(unsigned int i = 0; supported_locales[i] != NULL; i++)
  {
    if(setlocale(LC_ALL, supported_locales[i]) != NULL)
    {
      return;
    }
  }
  
  cerr << "Warning: unsupported locale, fallback to \"C\"" << endl;
  setlocale(LC_ALL, "C");
}
