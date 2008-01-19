/*
 * Copyright (C) 2006-2007 Felipe Sánchez-Martínez
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
 
#ifndef __TRANSFER_RULE_H_
#define __TRANSFER_RULE_H_

using namespace std;

#include <vector>
#include <string>
#include <iostream>
#include <set>

#include "AlignmentTemplate.H"

class TransferRule {
private:
  vector<AlignmentTemplate> ats;
  string source;

  //To store different categories used by the automatically generated
  //rules. It is update when the add_alignment_template method is
  //called 
  //first=lemma; second=tags;
  static set<pair<string, string> > categories;

  //To store the attributes of the categories used by the
  //automatically generated rules. It is update when the
  //add_alignment_template method is called
  static set<string> attributes;

  static string category_name(const string& lemma, const string& tags);

  static int rule_counter;
  int rule_id;
public: 
       
  TransferRule();
  
  TransferRule(const TransferRule& al);
    
  ~TransferRule();

  /** Return true if the AT was added.
   */
  bool add_alignment_template(const AlignmentTemplate& at);
  
  int get_number_alignment_templates();
  
  string gen_apertium_transfer_rule(bool debug=false);

  static string gen_apertium_transfer_head(bool debug=false);

  static string gen_apertium_transfer_foot(bool debug=false);
};

#endif
