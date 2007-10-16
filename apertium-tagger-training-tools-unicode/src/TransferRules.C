/*
 * Copyright (C) 2004-2006 Felipe Sánchez-Martínez
 * Copyright (C) 2006 Universitat d'Alacant
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
 
#include "TransferRules.H"
#include "Utils.H"

#include <fstream>

#include <apertium/tagger_utils.h>
#include <apertium/utf_converter.h>

TransferRules::TransferRules() {
}

TransferRules::~TransferRules() {
  //for(unsigned int i=0; i<compiled_regular_expressions.size(); i++) {
  //  regfree(&compiled_regular_expressions[i]);
  //}
}

void
TransferRules::read_rules_from_file(string filename) {
  fstream frules;
  frules.open(filename.c_str(), ios::in);
  if(!frules) {
    tagger_utils::file_name_error(filename);
    exit(EXIT_FAILURE);
  }

  string str="";
  string mode="";
  int offset_regex=regular_expressions.size();
  string regex;
  vector<int> rule;
  vector<string> splitted;
  while (!frules.eof()) {
    getline(frules, str);
    if (str.length()>0) {
      splitted=Utils::split_string(str, " ");
      //At least one element
      if (splitted[0]=="REGEX") {
	regex="";
	for(int i=1; i<splitted.size(); i++) {
	  if (splitted[i]=="__LEMMA__")
	    regex+=LEMMA;
	  else if (splitted[i]=="__TAGS__")
	    regex+=TAGS;
	  else
	    regex+=splitted[i];
	}
	//cerr<<"Inserting regex: "<<regex<<" --> pos: "<<regular_expressions.size()<<"\n";
	regular_expressions.push_back(regex);
      } else if (splitted[0]=="RULE") {
	rule.clear();
	for(int i=1; i<splitted.size(); i++) 
	  rule.push_back(atoi(splitted[i].c_str())+offset_regex);
	//cerr<<"Inserting rule ("<<rule.size()<<"): ";
	//for(int i=0; i<rule.size(); i++)
	//  cerr<<rule[i]<<" ";
	//cerr<<"\n";
	transfer_rules.push_back(rule);
      } else {
	wcerr<<L"Error reading transfer rules from file '"<<UtfConverter::fromUtf8(filename)<<L"', there is a line without REGEX or RULE command.\n";
	exit(EXIT_FAILURE);
      }
    }
  }

  frules.close();
}

void
TransferRules::compile_regular_expressions() {
  for(unsigned int i=0; i<regular_expressions.size(); i++) {
    //cerr<<"Compiling: "<<i<<" "<<regular_expressions[i]<<"\n";
    ApertiumRE compiled_regexp;
    compiled_regexp.compile(regular_expressions[i].c_str());
    //regex_t compiled_regexp;
    //int res=regcomp (&compiled_regexp, regular_expressions[i].c_str(), REG_EXTENDED | REG_ICASE | REG_NOSUB );
    //if(res!=0) {
    //  char errmsg[2048];
    //  regerror(res, &compiled_regexp, errmsg, 2048);
    //  cerr<<"Regex '"<<regular_expressions[i]<<"' compilation error: "<<errmsg<<"\n";
    //  exit(1);
    //}
    compiled_regular_expressions.push_back(compiled_regexp);
  }
}

bool 
TransferRules::match_regular_expression(wstring s, int pos_regex) {

  string res=compiled_regular_expressions[pos_regex].match(UtfConverter::toUtf8(s));

  if(res.length()>0)
    return true;
  else 
    return false;

  //int res=regexec(&compiled_regular_expressions[pos_regex], s.c_str(), 0, NULL, 0);
  //cerr<<"Match: "<<(res==0)<<" string: @"<<s<<"@ - regexp: @"<<i<<"@\n";

  //if (res==0)
  //  return true;
  //else
  //  return false;
}

bool 
TransferRules::is_segmentation_point(int tag_eof, vector<TaggerWord*>& vwords, int position, int &advance) {
  set<TTag> tags=vwords[position]->get_tags();
      
  advance=0;
   
  if(vwords[position]->get_plus_cut()) // This is a multiword unit
    return false;
   
  if (tags.size()>1) //This is an umbiguous word
    return false;
      
  if (tags.size()==0) { //This is an unknown word
    return true;
  }

  
  wstring forma_superficial=vwords[position]->get_superficial_form();

  for (int i=0; i<superficial_forms.size(); i++) {
    if (forma_superficial == superficial_forms[i])
      return false;
  }
      
  TTag tag=(*tags.begin()); //An unambiguous word, it has only one tag
  wstring stag=vwords[position]->get_lexical_form(tag, tag_eof);

  //Transfer rules are reviewed. If no transfer rule is
  //applicable, this word can be a segmentation point
  for(unsigned int i=0; i<transfer_rules.size(); i++) {
    int tag_position_at_rule=-1;
    for(unsigned int j=0; j<transfer_rules[i].size(); j++) {
      if (match_regular_expression(stag, transfer_rules[i][j])) { //Tag appears at rule i, pos. j
	tag_position_at_rule=j;
      }
         
      if (tag_position_at_rule>=0) {  //We test if the rule is applicacle
	int inicio_regla=position-tag_position_at_rule;
	if (inicio_regla<0) {
	  //The rule cannot be applied.
	  break;
	}

	//Now that we know that the tag appears in the rule i, we need
	//to test if rule match for at least one combination of tags
	//of the rest of the words
	bool rule_matched=true;
	for(unsigned int k=0; ((rule_matched)&&(k<transfer_rules[i].size())); k++) {
	  if ((inicio_regla+k)>=(vwords.size())) {
	    rule_matched=false;
	    break;
	  }
	  tags=vwords[inicio_regla+k]->get_tags();
	  set<TTag>::iterator it;
	  bool hay_etq=false;
	  for(it=tags.begin(); it!=tags.end(); it++) {
            wstring sit=vwords[inicio_regla+k]->get_lexical_form((TTag&)(*it), tag_eof);
	    if (match_regular_expression(sit, transfer_rules[i][k])) {
	      hay_etq=true; //At least one tag matches the position of
  	                    //the rules being considered
	      break;
	    }
	  }
	  if(!hay_etq) {
	    rule_matched=false;
	    break;
	  }
	}
	if (rule_matched) {
	  advance=transfer_rules[i].size()-tag_position_at_rule-1;
	  return false;
	}
      }
      tag_position_at_rule=-1;
    }
  }
   
  //This word is a segmentation point
  return true;
}

void
TransferRules::set_superficial_forms(wstring s) {
  superficial_forms=Utils::split_wstring(s,L"|");

  Utils::print_debug(L"\nSuperficial forms that will never be a segmentation point: ");
  for(int i=0; i<superficial_forms.size(); i++) {
    Utils::print_debug(superficial_forms[i]);
    Utils::print_debug(L" ");
  }
  Utils::print_debug(L"\n");
}

int 
TransferRules::get_number_regular_expressions() {
  return regular_expressions.size();
}
/*
  vector<int>
  TransferRules::match_regular_expressions(string s) {
  vector<int> matched;
  for(unsigned int i=0; i<compiled_regular_expressions.size(); i++) {

  int res=regexec(&compiled_regular_expressions[i], s.c_str(), 0, NULL, 0);

  //cerr<<"Match: "<<(res==0)<<" string: @"<<s<<"@ - regexp: @"<<i<<"@\n";

  if (res==0)
  matched.push_back(i);
  }
  return matched;
  }
*/

