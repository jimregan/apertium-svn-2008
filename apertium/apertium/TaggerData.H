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
#ifndef _TAGGERDATA_
#define _TAGGERDATA_

#include <apertium/ConstantManager.H>
#include <apertium/TTag.H>
#include <apertium/Collection.H>
#include <lttoolbox/PatternList.H>

#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

class TaggerData
{
private:
  set<TTag> open_class;
  vector<TForbidRule> forbid_rules;
  map<string, TTag> tag_index;
  vector<string> array_tags;
  vector<TEnforceAfterRule> enforce_rules;
  vector<string> prefer_rules;
  ConstantManager constants;
  Collection output;
  int N;
  int M;
  double **a;
  double **b;
  PatternList plist;

  void copy(TaggerData const &o);
  void destroy();
public:
  TaggerData();
  ~TaggerData();
  TaggerData(TaggerData const &o);
  TaggerData & operator =(TaggerData const &o);
  
  set<TTag> & getOpenClass();
  void setOpenClass(set<TTag> const &oc);

  vector<TForbidRule> & getForbidRules();
  void setForbidRules(vector<TForbidRule> &fr);
  
  map<string, TTag> & getTagIndex();
  void setTagIndex(map<string, TTag> const &ti);
  
  vector<string> & getArrayTags();
  void setArrayTags(vector<string> const &at);

  vector<TEnforceAfterRule> & getEnforceRules();
  void setEnforceRules(vector<TEnforceAfterRule> const &tear);

  vector<string> & getPreferRules();
  void setPreferRules(vector<string> const &pr);

  ConstantManager & getConstants();
  void setConstants(ConstantManager const &c);
  
  Collection & getOutput();
  void setOutput(Collection const &c);
 
  void setProbabilities(int const myN, int const myM, 
                        double **myA = NULL, double **myB = NULL);
  double ** getA();
  double ** getB();
  int getN();
  int getM();
  
  void setPatternList(PatternList const &pl);

  PatternList & getPatternList();
  
  void read(FILE *in);
  void write(FILE *out);
};

#endif
