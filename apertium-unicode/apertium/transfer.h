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
#ifndef _TRANSFER_
#define _TRANSFER_

#include <apertium/transfer_instr.h>
#include <apertium/transfer_token.h>
#include <apertium/transfer_word.h>
#include <lttoolbox/alphabet.h>
#include <lttoolbox/buffer.h>
#include <lttoolbox/fst_processor.h>
#include <lttoolbox/ltstr.h>
#include <lttoolbox/match_exe.h>
#include <lttoolbox/match_state.h>

#include <cstdio>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <map>
#include <set>
#include <vector>

using namespace std;

class Transfer
{
private:
  
  Alphabet alphabet;
  MatchExe *me;
  MatchState ms;
  map<string, string, Ltstr> attr_items;
  map<string, string, Ltstr> variables;
  map<string, int, Ltstr> macros;
  map<string, set<string, Ltstr>, Ltstr> lists;
  map<string, set<string, Ltstr>, Ltstr> listslow;
  vector<xmlNode *> macro_map;
  vector<xmlNode *> rule_map;
  xmlDoc *doc;
  xmlNode *root_element;
  TransferWord **word;
  string **blank;
  int lword, lblank;
  Buffer<TransferToken> input_buffer;
  vector<wstring *> tmpword;
  vector<wstring *> tmpblank;

  FSTProcessor fstp;
  FILE *output;
  int any_char;
  int any_tag;

  xmlNode *lastrule;
  unsigned int nwords;
  
  map<xmlNode *, TransferInstr> evalStringCache;

  enum OutputType{lu,chunk};
  
  OutputType defaultAttrs;
  
  void copy(Transfer const &o);
  void destroy();
  void readData(FILE *input);
  void readBil(string const &filename);
  void readTransfer(string const &input);
  void collectMacros(xmlNode *localroot);
  void collectRules(xmlNode *localroot);
  string caseOf(string const &str);
  string copycase(string const &source_word, string const &target_word);

  void processLet(xmlNode *localroot);
  void processAppend(xmlNode *localroot);
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
  void processChunk(xmlNode *localroot);
  void processTags(xmlNode *localroot);

  bool beginsWith(string const &str1, string const &str2) const;
  bool endsWith(string const &str1, string const &str2) const;
  string tolower(string const &str) const;
  string tags(string const &str) const;
  wstring readWord(FILE *in);
  wstring readBlank(FILE *in);
  wstring readUntil(FILE *in, int const symbol) const;
  void applyWord(wstring const &word_str);
  void applyRule();
  TransferToken & readToken(FILE *in);
    bool checkIndex(xmlNode *element, int index, int limit);
public:
  Transfer();
  ~Transfer();
  Transfer(Transfer const &o);
  Transfer & operator =(Transfer const &o);
  
  void read(string const &transferfile, string const &datafile,
	    string const &fstfile);
  void transfer(FILE *in, FILE *out);
};

#endif
