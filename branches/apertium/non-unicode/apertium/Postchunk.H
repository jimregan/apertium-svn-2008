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
#ifndef _POSTCHUNK_
#define _POSTCHUNK_

#include <apertium/TransferInstr.H>
#include <apertium/TransferToken.H>
#include <apertium/InterchunkWord.H>
#include <lttoolbox/Alphabet.H>
#include <lttoolbox/Buffer.H>
#include <lttoolbox/MatchExe.H>
#include <lttoolbox/MatchState.H>

#include <cstdio>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <map>
#include <set>
#include <vector>

using namespace std;

class Postchunk
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
  vector<xmlNode *> macro_map;
  vector<xmlNode *> rule_map;
  xmlDoc *doc;
  xmlNode *root_element;
  InterchunkWord **word;
  string **blank;
  int lword, lblank;
  Buffer<TransferToken> input_buffer;
  vector<string *> tmpword;
  vector<string *> tmpblank;

  FILE *output;
  int any_char;
  int any_tag;

  xmlNode *lastrule;
  unsigned int nwords;
  
  map<xmlNode *, TransferInstr> evalStringCache;

  void copy(Postchunk const &o);
  void destroy();
  void readData(FILE *input);
  void readPostchunk(string const &input);
  void collectMacros(xmlNode *localroot);
  void collectRules(xmlNode *localroot);
  static string caseOf(string const &str);
  string copycase(string const &source_word, string const &target_word);

  void processLet(xmlNode *localroot);
  void processOut(xmlNode *localroot);
  void processCallMacro(xmlNode *localroot);
  void processModifyCase(xmlNode *localroot);
  bool processLogical(xmlNode *localroot);
  bool processTest(xmlNode *localroot);
  bool processAnd(xmlNode *localroot);
  bool processOr(xmlNode *localroot);
  bool processEqual(xmlNode *localroot);
  bool processBeginsWith(xmlNode *localroot);
  bool processEndsWith(xmlNode *localroot);
  bool processContainsSubstring(xmlNode *localroot);
  bool processNot(xmlNode *localroot);
  bool processIn(xmlNode *localroot);
  void processRule(xmlNode *localroot);
  string evalString(xmlNode *localroot);
  void processInstruction(xmlNode *localroot);
  void processChoose(xmlNode *localroot);
  void processTags(xmlNode *localroot);
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
  static void unchunk(string const &chunk, FILE *output);
  static vector<string> getVecTags(string const &chunk);
  static int beginChunk(string const &chunk);
  static int endChunk(string const &chunk);
  static void splitWordsAndBlanks(string const &chunk, 
				  vector<string *> &words,
				  vector<string *> &blanks);
  static string pseudolemma(string const &chunk);
  static string wordzero(string const &chunk);

  bool checkIndex(xmlNode *element, int index, int limit);
public:
  Postchunk();
  ~Postchunk();
  Postchunk(Postchunk const &o);
  Postchunk & operator =(Postchunk const &o);
  
  void read(string const &transferfile, string const &datafile);
  void postchunk(FILE *in, FILE *out);
};

#endif
