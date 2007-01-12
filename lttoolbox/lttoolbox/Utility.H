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

#ifndef _UTILITY_
#define _UTILITY_

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

using namespace std;

/**
 * Parametrized utility class to group diagnose and printing functions
 */
template<class T1 = void, class T2 = T1>
class Utility
{
private:
  static void outOfMemory()
    {
      cerr << "Memory exhausted. Program terminates." << endl;
      exit(EXIT_FAILURE);
    }
public:
  static string toString(set<T1> const &cjto)
    {
      ostringstream resultado;

      for(typename set<T1>::iterator it = cjto.begin(); it != cjto.end(); it++)
      {
        if(resultado.str() != "")
        {
          resultado << ' ';
        }

        resultado << *it;
      }

      return resultado.str();  
    }

  static string toString(map<T1,T2> const &mapa)
    {
      ostringstream resultado;

      for(typename map<T1,T2>::iterator it = mapa.begin(); it != mapa.end(); it++)
      {
        if(resultado.str() != "")
        {
          resultado << ' ';
        }

        resultado << '[' << it->first << " -> ";
        resultado << it->second << ']';
      }

      return resultado.str();
    }

  static string toString(map<T1, set<T2> > const &mapa)
    {
      ostringstream resultado;

      typename map<T1, set<T2> >::const_iterator it = mapa.begin();

      for(it = mapa.begin(); it != mapa.end(); it++)
      {
        if(resultado.str() != "")
        {
          resultado << ' ';
        }

        resultado << '[' << it->first << " -> ";
        resultado << Utility<T2>::toString(it->second) << ']';
      }

      return resultado.str();
    }

  static void freeArray(T1 *array)
    {
      if(array)
      {
        delete[] array;
      }

      array = NULL;
    }

  static void freeDoublePointer(T1 **dpointer, int const size)
    {
      if(dpointer)
      {
        for(int i = 0; i < size; i++)
        {
          if(dpointer[i])
          {
            delete[] dpointer[i];
          }
        }
        delete[] dpointer;
      }

      dpointer = NULL;
    }

  static T1* resizeArray(T1 *array, int const oldsize, int const newsize)
    {
      T1 *tmp = new T1[newsize];

      for(int i = 0; i < oldsize; i++)
      {
        tmp[i] = array[i];
      }

      if(array)
      {
        delete[] array;
      }

      return tmp;
    }

  static T1** resizeDoublePointer(T1 **dpointer, int const oldsize, 
				   int const newsize)
    {
      T1 **tmp = new T1* [newsize];

      for(int i = 0; i < oldsize; i++)
      {
        tmp[i] = dpointer[i];
      }
      
      for(int i = oldsize; i < newsize; i++)
      {
	tmp[i] = NULL;
      }

      if(dpointer)
      {
        delete[] dpointer;
      }

      return tmp;
    }

  static void swap(T1 * a, T1 *b)
    {
      T1 * tmp;
      tmp = a;
      a = b;
      b = tmp;
    }

  static void swap(T1 &a, T1 &b)
    {
      T1 tmp;
      tmp = a;
      a = b;
      b = tmp;
    }


  static void enableNewHandler()
    {
      set_new_handler(outOfMemory);
    }
};

#endif
