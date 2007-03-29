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
#ifndef _TRANSFER_MULT_
#define _TRANSFER_MULT_

#include <apertium/TransferInstr.H>
#include <apertium/TransferToken.H>
#include <apertium/TransferWord.H>
#include <lttoolbox/Alphabet.H>
#include <lttoolbox/Buffer.H>
#include <lttoolbox/FSTProcessor.H>
#include <lttoolbox/MatchExe.H>
#include <lttoolbox/MatchState.H>

#include <cstdio>
#include <map>
#include <set>
#include <vector>

using namespace std;

class TransferMult
{
private:
  
  Alphabet alphabet;
  MatchExe *me;
  MatchState ms;
  map<string, string> attr_items;
  map<string, string> variables;
  map<string, int> macros;
  map<string, set<string> > lists;
  map<string, set<string> > listslow;
  TransferWord **word;
  string **blank;
  Buffer<TransferToken> input_buffer;
  vector<string *> tmpword;
  vector<string *> tmpblank;
  

  FSTProcessor fstp;
  FILE *output;
  int any_char;
  int any_tag;
  bool isRule;
  unsigned int numwords;
  
  unsigned int nwords;
  
  enum OutputType{lu,chunk};
  
  OutputType defaultAttrs;
  
  void copy(TransferMult const &o);
  void destroy();
  void readData(FILE *input);
  void readBil(string const &filename);
  string caseOf(string const &str);
  string copycase(string const &source_word, string const &target_word);

  bool beginsWith(string const &str1, string const &str2) const;
  bool endsWith(string const &str1, string const &str2) const;
  string tolower(string const &str) const;
  string tags(string const &str) const;
  string readWord(FILE *in);
  string readBlank(FILE *in);
  string readUntil(FILE *in, int const symbol) const;
  void applyWord(string const &word_str);
  void applyRule();
  TransferToken & readToken(FILE *in);
  void writeMultiple(list<vector<string> >::iterator itwords,
                     list<string>::iterator itblanks, 
                     list<vector<string> >::const_iterator limitwords, 
                     string acum = "", bool multiple = false);
  vector<string> acceptions(string const &str);
  bool isDefaultWord(string const &str);
public:
  TransferMult();
  ~TransferMult();
  TransferMult(TransferMult const &o);
  TransferMult & operator =(TransferMult const &o);
  
  void read(string const &datafile, string const &fstfile);
  void transfer(FILE *in, FILE *out);
};

#endif
