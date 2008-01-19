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
#include <cstdlib>
#include <iostream>
#include <libgen.h>
#include <string>

#include <apertium/ApertiumConfig.H>


#if !HAVE_DECL_FPUTS_UNLOCKED
#define fputs_unlocked fputs
#endif

#if !HAVE_DECL_FGETC_UNLOCKED 
#define fgetc_unlocked fgetc
#endif

#if !HAVE_DECL_PUTC_UNLOCKED
#define fputc_unlocked fputc
#endif

#if !HAVE_DECL_FWRITE_UNLOCKED
#define fwrite_unlocked fwrite
#endif

#if !HAVE_DECL_FREAD_UNLOCKED
#define fread_unlocked fread
#endif


using namespace std;

void readAndWriteUntil(FILE *input, FILE *output, int const charcode)
{
  int mychar;

  while((mychar = fgetc_unlocked(input)) != charcode)
  {
    if(feof(input))
    {
      cerr << "ERROR: Unexpected EOF" << endl;
      exit(EXIT_FAILURE);
    }
    fputc_unlocked(mychar, output);
    if(mychar == '\\')
    {
      mychar = fgetc(input);
      fputc(mychar, output);
    }
  }
}

void procWord(FILE *input, FILE *output)
{
  int mychar;
  string buffer = "";

  bool buffer_mode = false;
  while((mychar = fgetc_unlocked(input)) != '$')
  {
    if(feof(input))
    {
      cerr << "ERROR: Unexpected EOF" << endl;
      exit(EXIT_FAILURE);
    }
  
    switch(mychar)
    {
    case '<':
      if(!buffer_mode)
      {
	buffer_mode = true;
      }
      break;
    case '#':
      if(buffer_mode)
      {
	buffer_mode = false;
      }
      break;
    }

    if(buffer_mode)
    {
      if(mychar != '+')
      {
	buffer += static_cast<char>(mychar);
      }
      else
      {
        buffer.append("$ ^");
      }
    }
    else
    {
      fputc_unlocked(mychar, output);
    }
  }
  fputs_unlocked(buffer.c_str(), output);
}

void processStream(FILE *input, FILE *output)
{
  while(true)
  {
    int mychar = fgetc_unlocked(input);
    if(feof(input))
    {
      break;
    }
    switch(mychar)
    {
    case '[':
      fputc_unlocked('[', output);
      readAndWriteUntil(input, output, ']');
      fputc_unlocked(']', output);
      break;
    case '\\':
      fputc_unlocked(mychar, output);
      fputc_unlocked(fgetc_unlocked(input), output);
      break;
    case '^':
      fputc_unlocked(mychar, output);
      procWord(input, output);
      fputc_unlocked('$', output);
      break;
    default:
      fputc_unlocked(mychar, output);
      break;
    }
  }
}

void usage(char *progname)
{
  cerr << "USAGE: " << basename(progname) << " [input_file [output_file]]" << endl;
  exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
  if(argc > 3)
  {
    usage(argv[0]);
  }

  FILE *input, *output;
  
  if(argc == 1)
  {
    input = stdin;
    output = stdout;
  }
  else if (argc == 2)
  {
    input = fopen(argv[1], "r");
    if(!input)
    {
      usage(argv[0]);
    }
    output = stdout;
  }
  else
  {
    input = fopen(argv[1], "r");
    output = fopen(argv[2], "w");
    if(!input || !output)
    {
      usage(argv[0]);
    }
  }

  if(feof(input))
  {
    cerr << "ERROR: Can't read file '" << argv[1] << "'" << endl;
    exit(EXIT_FAILURE);
  }

  processStream(input, output);
}
