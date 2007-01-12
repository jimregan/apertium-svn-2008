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
#ifndef _COMPRESSION_
#define _COMPRESSION_

#include <cstdio>
#include <iostream>

using namespace std;

/**
 * Clase "Compression".
 * Métodos de clase de lectura/escritura para acceder a datos comprimidos con
 * el método "byte-aligned", es decir, usando los dos primeros bits del primer
 * byte leido (el más significativo) para decidir cuántos bytes más hay que 
 * leer.
 */
class Compression
{
public:
  /**
   * Codifica y escribe un entero a un flujo de salida.
   * @see multibyte_read()
   * @param value valor a escribir.
   * @param output flujo de salida.
   */
  static void multibyte_write(unsigned int value, FILE *output);
  
  /**
   * Lee y decodifica un entero del flujo de entrada suministrado.
   * @see multibyte_read()
   * @param input flujo de entrada.
   * @return el valor leído.
   */
  static unsigned int multibyte_read(FILE *input);
  
    /**
   * Codifica y escribe un entero a un flujo de salida.
   * @see multibyte_read()
   * @param value valor a escribir.
   * @param output flujo de salida.
   */
  static void multibyte_write(unsigned int value, ostream &os);
  
  /**
   * Lee y decodifica un entero del flujo de entrada suministrado.
   * @see multibyte_read()
   * @param input flujo de entrada.
   * @return el valor leído.
   */
  static unsigned int multibyte_read(istream &is);
};

#endif
