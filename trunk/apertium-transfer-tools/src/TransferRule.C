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


#include "TransferRule.H"
#include "Utils.H"
#include <apertium/StringUtils.H>

set<pair<string, string> > TransferRule::categories;
set<string> TransferRule::attributes;
int TransferRule::rule_counter=0;

TransferRule::TransferRule() {
  source="";
  rule_id=rule_counter;
  rule_counter++;
}
  
TransferRule::TransferRule(const TransferRule& tr) {
  source=tr.source;
  ats=tr.ats;
}
    
TransferRule::~TransferRule() {
}

bool 
TransferRule::add_alignment_template(const AlignmentTemplate& at) {
  string source_at=Utils::vector2string(at.source);
  if (source.length()==0)
    source=source_at;

  if (source!=source_at)
    return false;

  ats.push_back(at);

  //Now we revise the categories and the attributes that will be used
  //by the generated rules
  for (unsigned i=0; i<at.source.size(); i++) {
    string s=at.source[i];
    if (s.length()==0) {
      cerr<<"Error in TransferRule::add_alignment_template: an empty source word was found\n";
      exit(EXIT_FAILURE);
    }
    pair<string, string> cat;

    string tags=Utils::tags2transferformat(Utils::get_tags(s));

    cat.first=Utils::get_lemma(s);
    cat.second=tags;
    categories.insert(cat);
    attributes.insert(tags);
  }

  return true;
}
  
int 
TransferRule::get_number_alignment_templates() {
  return ats.size();
}
  
string 
TransferRule::gen_apertium_transfer_rule(bool debug) {
  string rule="";
  bool include_otherwise=true;

  if (ats.size()==0) {
    cerr<<"Error in TransferRule::gen_apertium_transfer_rule: No alignment templates available\n";
    exit(EXIT_FAILURE);
  }

  //Sort the AT so as to apply always the most frequent AT that
  //satifies the restrictions
  AlignmentTemplateGreaterThanByCount atcomparer;
  sort(ats.begin(), ats.end(), atcomparer);

  rule+="<rule>\n";

  //The pattern to detect is the same for all AT within this transfer rule
  rule+="  <pattern>\n";
  for(unsigned i=0; i<ats[0].source.size(); i++) {
    string lemma=Utils::get_lemma(ats[0].source[i]);
    string tags=Utils::tags2transferformat(Utils::get_tags(ats[0].source[i]));
    rule+="    <pattern-item n=\"CAT__"+category_name(lemma,tags)+"\"/>\n";
  }
  rule+="  </pattern>\n";

  rule+="  <action>\n";
  rule+="    <choose>\n";

  //There is a set of different actions depending on the TL side of
  //each AT. Consequently, there's one <when> statement per AT 
  for(unsigned i=0; i<ats.size(); i++) {
    rule+="      <when>";
    rule+="<!--"+ats[i].to_string()+"-->\n";
    rule+="        <test>\n";

    int nconditions=0;
    string teststr="";
    //This AT can be applied if all restrictions are met
    for(unsigned j=0; j<ats[i].restrictions.size(); j++){
      if (ats[i].restrictions[j]!="__CLOSEWORD__") {
	nconditions++;
	teststr+="          <or>\n";

	teststr+="            <equal>\n";
	teststr+="              <clip pos=\""+Utils::itoa(j+1)+"\" side=\"tl\" part=\"tags\" queue=\"no\"/>\n";
	teststr+="              <lit-tag v=\""+Utils::tags2transferformat(ats[i].restrictions[j])+"\"/>\n";
	teststr+="            </equal>\n";

	teststr+="            <equal>\n";
	teststr+="              <clip pos=\""+Utils::itoa(j+1)+"\" side=\"tl\" part=\"tags\" queue=\"yes\"/>\n";
	teststr+="              <lit-tag v=\""+Utils::tags2transferformat(Utils::get_tags(ats[i].target[ats[i].get_open_target_word_pos(j)]))+"\"/>\n";
	teststr+="            </equal>\n";

	teststr+="          </or>\n";
      }
    }

    if (nconditions==0) { //All words were close words. We introduce a
			  //condition that is always true
      teststr+="          <equal>\n";
      teststr+="            <lit v=\"TRUE\"/>\n";
      teststr+="            <lit v=\"TRUE\"/>\n";
      teststr+="          </equal>\n";
      include_otherwise=false;
    }

    if (nconditions>1) // There are more than one restriction to test
      rule+="        <and>\n";

    rule+=teststr;

    if (nconditions>1)
      rule+="        </and>\n";

    rule+="        </test>\n";

    if (debug) {
      string s=StringUtils::substitute(ats[i].to_string(), "><", ".");
      s=StringUtils::substitute(s,"<","-");
      s=StringUtils::substitute(s,">","");
      rule+="        <out>\n";
      rule+="          <lu><lit v=\"(rid:"+StringUtils::itoa(rule_id)+" at:"+s+")\"/></lu>\n";
      rule+="        </out>\n";
    }

    int blank_pos=0;
    for(unsigned j=0; j<ats[i].target.size(); j++) {      
      if (ats[i].target[j][0]!='<') { //It's a lexicalized word, we copy it as is
	rule+="        <out>\n";
	rule+="          <lu>\n";
	string target_tags=Utils::tags2transferformat(Utils::get_tags(ats[i].target[j]));
	rule+="            <lit v=\""+StringUtils::substitute(Utils::get_lemma_without_queue(ats[i].target[j]),"_"," ")+"\"/>\n";
	rule+="            <lit-tag v=\""+target_tags+"\"/>\n";
	rule+="            <lit v=\""+StringUtils::substitute(Utils::get_queue(ats[i].target[j]),"_"," ")+"\"/>\n";
	rule+="          </lu>\n";
	rule+="        </out>\n";

	string genre=Utils::get_tag_value(target_tags,"m|f");
	if(genre.length()>0)
	  rule+="        <let><var n=\"genre\"/><lit-tag v=\""+genre+"\"/></let>\n";

      } else {
	int pos=ats[i].get_open_source_word_pos(j);
	rule+="        <out>\n";
	rule+="          <lu>\n";
	rule+="            <clip pos=\""+Utils::itoa(pos+1)+"\" side=\"tl\" part=\"lemh\"/>\n";
	rule+="            <lit-tag v=\""+Utils::tags2transferformat(Utils::get_tags(ats[i].target[j]))+"\"/>\n";
	rule+="            <clip pos=\""+Utils::itoa(pos+1)+"\" side=\"tl\" part=\"lemq\"/>\n";
	rule+="          </lu>\n";
	rule+="        </out>\n";

        rule+="        <call-macro n=\"f_genre_num\">\n";
        rule+="          <with-param pos=\""+Utils::itoa(pos+1)+"\"/>\n";
        rule+="        </call-macro>\n";
      }

      if (blank_pos<(int)(ats[i].source.size()-1)) {
	rule+="        <out>\n";
	rule+="          <b pos=\""+Utils::itoa(blank_pos+1)+"\"/>\n";
	rule+="        </out>\n";
	blank_pos++;
      } else if (j<(ats[i].target.size()-1)) {
	//TL output string has more words than the SL pattern detected
	rule+="        <out>\n";
	rule+="          <b/>\n";
	rule+="        </out>\n";
      }
    }

    if (debug) {
      rule+="        <out>\n";
      rule+="          <lu><lit v=\"(END)\"/></lu>\n";
      rule+="        </out>\n";
    }

    //If there are remaining blanks we print them out if they have
    //format information inside. This is caused by a SL input string
    //longer than the TL output one
    for (unsigned j=ats[i].target.size(); j<ats[i].source.size(); j++) {
      rule+="        <call-macro n=\"f_bcond\">\n";
      rule+="          <with-param pos=\""+Utils::itoa(j)+"\"/>\n";
      rule+="        </call-macro>\n";
    }

    rule+="      </when>\n";

    if(!include_otherwise) {
      //As the condition will always be met it has no sense to include
      //further ATs
      break;
    }
  }

  //Actions to perform when none of the ATs can be applied
  //word-for-word translation
  if(include_otherwise) {
    rule+="      <otherwise><!--Word-for-word translation-->\n";
    if (debug) {
      rule+="        <out>\n";
      rule+="          <lu><lit v=\"(rid:"+StringUtils::itoa(rule_id)+" at:word-for-word)\"/></lu>\n";
      rule+="        </out>\n";
    }

    for(unsigned i=0; i<ats[0].source.size(); i++) {
      rule+="        <call-macro n=\"f_genre_num\">\n";
      rule+="          <with-param pos=\""+Utils::itoa(i+1)+"\"/>\n";
      rule+="        </call-macro>\n";

      rule+="        <call-macro n=\"f_set_genre_num\">\n";
      rule+="          <with-param pos=\""+Utils::itoa(i+1)+"\"/>\n";
      rule+="        </call-macro>\n";

      rule+="        <out>\n";
      rule+="          <lu>\n";

      rule+="            <clip pos=\""+Utils::itoa(i+1)+"\" side=\"tl\" part=\"whole\"/>\n";
      //rule+="            <clip pos=\""+Utils::itoa(i+1)+"\" side=\"tl\" part=\"lemh\"/>\n";
      //rule+="            <clip pos=\""+Utils::itoa(i+1)+"\" side=\"tl\" part=\"tags\"/>\n";
      //rule+="            <clip pos=\""+Utils::itoa(i+1)+"\" side=\"tl\" part=\"lemq\"/>\n";

      rule+="          </lu>\n";
      if (i<(ats[0].source.size()-1))
	rule+="          <b pos=\""+Utils::itoa(i+1)+"\"/>\n";
      rule+="        </out>\n";
    }
    if (debug) {
      rule+="        <out>\n";
      rule+="          <lu><lit v=\"(END)\"/></lu>\n";
      rule+="        </out>\n";
    }

    rule+="      </otherwise>\n";
  }

  rule+="    </choose>\n";
  rule+="  </action>\n";
  rule+="</rule>\n";

  return rule;
}

string 
TransferRule::gen_apertium_transfer_head(bool debug) {
  string head="";

  head+="<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n";
  head+="<transfer>\n";

  head+="<section-def-cats>\n";

  set<pair<string,string> >::iterator it;
  for(it=categories.begin(); it!=categories.end(); it++) {
    head+="  <def-cat n=\"CAT__"+category_name(it->first,it->second)+"\">\n";
    if (it->first.length()>0) //There is a lemma
      head+="    <cat-item lemma=\""+StringUtils::substitute(it->first,"_"," ")+"\" tags=\""+it->second+"\"/>\n";
    else
      head+="    <cat-item tags=\""+it->second+"\"/>\n";
    head+="  </def-cat>\n";
  }

  head+="  <def-cat n=\"CAT__ND_GD\">\n";
  head+="    <cat-item tags=\"*.mf.*\"/>\n";
  head+="    <cat-item tags=\"*.sp.*\"/>\n";
  head+="    <cat-item tags=\"*.mf.sp.*\"/>\n";
  head+="    <cat-item tags=\"*.sp.mf.*\"/>\n";
  head+="    <cat-item tags=\"*.mf.*.sp.*\"/>\n";
  head+="    <cat-item tags=\"*.sp.*.mf.*\"/>\n";
  head+="  </def-cat>\n";

  head+="</section-def-cats>\n";

  head+="<section-def-attrs>\n";

  //set<string>::iterator it2;
  //for(it2=attributes.begin(); it2!=attributes.end(); it2++) {
  //  head+="  <def-attr n=\"ATTR__"+category_name("",(*it2))+"\">\n";
  //  head+="    <attr-item tags=\""+(*it2)+"\"/>\n";
  //  head+="  </def-attr>\n";
  //}

  head+="  <def-attr n=\"gen\">\n";
  head+="    <attr-item tags=\"m\"/>\n";
  head+="    <attr-item tags=\"f\"/>\n";
  head+="    <attr-item tags=\"mf\"/>\n";
  head+="    <attr-item tags=\"GD\"/>\n";
  head+="  </def-attr>\n";

  head+="  <def-attr n=\"num\">\n";
  head+="    <attr-item tags=\"sg\"/>\n";
  head+="    <attr-item tags=\"pl\"/>\n";
  head+="    <attr-item tags=\"sp\"/>\n";
  head+="    <attr-item tags=\"ND\"/>\n";
  head+="  </def-attr>\n";

  //head+="  <def-attr n=\"ATTR__notused\">\n";
  //head+="    <attr-item tags=\"this.attr.will.not.be.used\"/>\n";
  //head+="  </def-attr>\n";

  head+="</section-def-attrs>\n";

  head+="<section-def-vars>\n";
  head+="  <def-var n=\"genre\"/>\n";
  head+="  <def-var n=\"number\"/>\n";
  head+="</section-def-vars>\n";

  head+="<section-def-macros>\n";

  head+="<def-macro n=\"f_bcond\" npar=\"1\">\n";
  head+="<!--To test wether a blank contains format information.\n";
  head+="If no format information is present it is removed. -->\n";
  head+="  <choose>\n";
  head+="    <when>\n";
  head+="      <test>\n";
  head+="        <not>\n";
  head+="	 <equal>\n";
  head+="	   <b pos=\"1\"/>\n";
  head+="	   <lit v=\" \"/>\n";
  head+="	 </equal>\n";
  head+="	 </not>\n";
  head+="      </test>\n";
  head+="      <out>\n";
  head+="        <b pos=\"1\"/>\n";
  head+="      </out>\n";
  head+="    </when>\n";
  head+="  </choose>\n";
  head+="</def-macro>\n";

  head+="<def-macro n=\"f_genre_num\" npar=\"1\">\n";
  head+="<!--To set the global value storing the TL genre of the last seen word. -->\n";
  head+="  <choose>\n";
  head+="    <when>\n";
  head+="      <test>\n";
  head+="        <equal>\n";
  head+="          <clip pos=\"1\" side=\"tl\" part=\"gen\"/>\n";
  head+="          <lit-tag v=\"m\"/>\n";
  head+="        </equal>\n";
  head+="      </test>\n";
  head+="      <let><var n=\"genre\"/><lit-tag v=\"m\"/></let>\n";
  head+="    </when>\n";
  head+="    <when>\n";
  head+="      <test>\n";
  head+="        <equal>\n";
  head+="          <clip pos=\"1\" side=\"tl\" part=\"gen\"/>\n";
  head+="          <lit-tag v=\"f\"/>\n";
  head+="        </equal>\n";
  head+="      </test>\n";
  head+="      <let><var n=\"genre\"/><lit-tag v=\"f\"/></let>\n";
  head+="    </when>\n";
  head+="    <when>\n";
  head+="      <test>\n";
  head+="        <equal>\n";
  head+="          <clip pos=\"1\" side=\"tl\" part=\"num\"/>\n";
  head+="          <lit-tag v=\"sg\"/>\n";
  head+="        </equal>\n";
  head+="      </test>\n";
  head+="      <let><var n=\"number\"/><lit-tag v=\"sg\"/></let>\n";
  head+="    </when>\n";
  head+="    <when>\n";
  head+="      <test>\n";
  head+="        <equal>\n";
  head+="          <clip pos=\"1\" side=\"tl\" part=\"num\"/>\n";
  head+="          <lit-tag v=\"pl\"/>\n";
  head+="        </equal>\n";
  head+="      </test>\n";
  head+="      <let><var n=\"number\"/><lit-tag v=\"pl\"/></let>\n";
  head+="    </when>\n";
  head+="  </choose>\n";
  head+="</def-macro>\n";

  head+="<def-macro n=\"f_set_genre_num\" npar=\"1\">\n";
  head+="<!--To set the genre of those words with GD, and the number of those words with ND. -->\n";
  head+="<!--This is only used in no alignment template at all is applied. -->\n";
  head+="  <choose>\n";
  head+="    <when>\n";
  head+="      <test>\n";
  head+="        <equal>\n";
  head+="          <clip pos=\"1\" side=\"tl\" part=\"gen\"/>\n";
  head+="          <lit-tag v=\"GD\"/>\n";
  head+="        </equal>\n";
  head+="      </test>\n";
  head+="      <choose>\n";
  head+="        <when>\n";
  head+="          <test>\n";
  head+="            <equal>\n";
  head+="              <var n=\"genre\"/>\n";
  head+="              <lit-tag v=\"f\"/>\n";
  head+="            </equal>\n";
  head+="          </test>\n";
  head+="          <let><clip pos=\"1\" side=\"tl\" part=\"gen\"/><lit-tag v=\"f\"/></let>\n";
  head+="        </when>\n";
  head+="        <otherwise>\n";
  head+="          <let><clip pos=\"1\" side=\"tl\" part=\"gen\"/><lit-tag v=\"m\"/></let>\n";
  head+="        </otherwise>\n";
  head+="      </choose>\n";
  head+="    </when>\n";
  head+="  </choose>\n";
  head+="  <choose>\n";
  head+="    <when>\n";
  head+="      <test>\n";
  head+="        <equal>\n";
  head+="          <clip pos=\"1\" side=\"tl\" part=\"num\"/>\n";
  head+="          <lit-tag v=\"ND\"/>\n";
  head+="        </equal>\n";
  head+="      </test>\n";
  head+="      <choose>\n";
  head+="        <when>\n";
  head+="          <test>\n";
  head+="            <equal>\n";
  head+="              <var n=\"number\"/>\n";
  head+="              <lit-tag v=\"pl\"/>\n";
  head+="            </equal>\n";
  head+="          </test>\n";
  head+="          <let><clip pos=\"1\" side=\"tl\" part=\"num\"/><lit-tag v=\"pl\"/></let>\n";
  head+="        </when>\n";
  head+="        <otherwise>\n";
  head+="          <let><clip pos=\"1\" side=\"tl\" part=\"num\"/><lit-tag v=\"sg\"/></let>\n";
  head+="        </otherwise>\n";
  head+="      </choose>\n";
  head+="    </when>\n";
  head+="  </choose>\n";
  head+="</def-macro>\n";

  head+="</section-def-macros>\n";

  head+="<section-rules>\n";

  return head;
}

string 
TransferRule::gen_apertium_transfer_foot(bool debug) {
  string foot="";

  foot+="<rule>\n";
  foot+="  <pattern>\n";
  foot+="    <pattern-item n=\"CAT__ND_GD\"/>\n";
  foot+="  </pattern>\n";
  foot+="  <action>\n";

  if(debug) {
    foot+="  <out>\n";
    foot+="    <lu><lit v=\"(default)\"/></lu>\n";
    foot+="  </out>\n";
  }

  foot+="  <call-macro n=\"f_set_genre_num\">\n";
  foot+="    <with-param pos=\"1\"/>\n";
  foot+="  </call-macro>\n";
  foot+="  <out>\n";
  foot+="    <lu>\n";
  foot+="      <clip pos=\"1\" side=\"tl\" part=\"whole\"/>\n";
  foot+="    </lu>\n";
  foot+="  </out>\n";
  foot+="  </action>\n";
  foot+="</rule>\n";

  foot+="</section-rules>\n";
  foot+="</transfer>\n";

  return foot;
}

string 
TransferRule::category_name(const string& lemma, const string& tags) {
  string catname="";

  if (lemma.length()>0)
    catname+=Utils::substitute(lemma,"#","_")+"_";

  catname+=Utils::substitute(tags,".","");

  return catname;
}

