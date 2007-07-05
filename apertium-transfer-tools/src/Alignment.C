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


#include "Alignment.H"
#include "Utils.H"

#include <cstdlib>
#include <iostream>
#include <cmath>

Alignment::Alignment() {
}

Alignment::Alignment(string al, int nfields) {
  vector<string> v;
  vector<string> alig;

  v=Utils::split_string(al, " | ");

  if (v.size()!=(unsigned)nfields) {
    cerr<<"Error in Alignment::Alignment when reading alignment from string '"<<al<<"'\n";
    cerr<<"Unespected number of fields separated by ' | '\n";
    exit(EXIT_FAILURE); 
  }

  score=atof(v[0].c_str());
  source=Utils::split_string(v[1], " ");
  target=Utils::split_string(v[2], " ");
  alig=Utils::split_string(v[3], " ");

  for(unsigned i=0; i<alig.size(); i++) {
    vector<string> an_alig;

    an_alig=Utils::split_string(alig[i], ":");
    if (an_alig.size()!=2) {
      cerr<<"Error in Alignment::Alignment when reading alignment from string '"<<al<<"'\n";
      cerr<<"Unespected number of alignment values separated by ':'\n";
      exit(EXIT_FAILURE);
    }
    alignment[atoi(an_alig[0].c_str())][atoi(an_alig[1].c_str())]=true;
  }
}

Alignment::Alignment(const Alignment& al) {
  source=al.source;
  target=al.target;
  score=al.score;
  alignment=al.alignment;
}
    
Alignment::~Alignment() {
}

int 
Alignment::get_source_length() {
  return source.size();
}

int 
Alignment::get_target_length() {
  return target.size();
}

int
Alignment::length() {
  //return (source.size()+target.size())/2;
  return source.size();
}

string
Alignment::to_string() {
  string s;
  s=Utils::itoa((int)score)+" |";

  for(unsigned i=0; i<source.size(); i++) 
    s+=" "+source[i];
  s+=" |";

  for(unsigned i=0; i<target.size(); i++) 
    s+=" "+target[i];
  s+=" |";

  for (unsigned i=0; i<source.size(); i++) {
    for(unsigned j=0; j<target.size(); j++) {
      if (alignment[i][j])
	s+=" "+Utils::itoa(i)+":"+Utils::itoa(j);
    }
  }

  return s;
}

vector<Alignment> 
Alignment::extract_bilingual_phrases(int min_length, int max_length) {
  vector<Alignment> bil_phrases;

  for(unsigned i2=0; i2<target.size(); i2++) {
    for(unsigned i1=0; i1<=i2; i1++) {
      //cerr<<"I2 "<<i2<<"\n";
      //cerr<<"I1 "<<i1<<"\n";
      set<int> SP;
      for(unsigned j=0; j<source.size(); j++) {
	for(unsigned i=i1; i<=i2; i++) {
	  if (alignment[j][i])
	    SP.insert(j);
	}
      }    

      //cerr<<"SP: ";
      //for(set<int>::iterator it=SP.begin(); it!= SP.end(); it++)
      //  cerr<<*it<<" ";
      //cerr<<"\n";

      if (consecutive(SP)) {
	//cerr<<"SP consecutive\n";
	int j1=*(SP.begin()); //min value
	int j2=*(--SP.end()); //max value
	int phrase_length=j2-j1+1;

	//cerr<<"min:"<<j1<<"\n";
	//cerr<<"max:"<<j2<<"\n";
	//cerr<<"phrase:" <<sub_alignment(j1, j2, i1, i2).to_string()<<"\n";
	//cerr<<"phrase length:"<<phrase_length<<"\n";
	if ((phrase_length>=min_length)&&(phrase_length<=max_length)) {
	  if (consistent(j1, j2, i1, i2)) {
	    bil_phrases.push_back(sub_alignment(j1, j2, i1, i2));
	    //cerr<<"Added\n";
	  }
	}
      } //else {
	//cerr<<"SP no consecutive\n";
      //}
      //cerr<<"\n";
    }
  }

  return bil_phrases;
}

bool 
Alignment::consecutive(const set<int>& SP) {
  set<int>::iterator it;

  if (SP.size()==0)
    return false;

  it=SP.begin();
  int prev=(*it);

  for(it++; it!=SP.end(); it++) {
    if (((*it)-prev)!=1)
      return false;
    prev=(*it);
  }

  return true;
}

bool 
Alignment::consistent(int from_source, int to_source, int from_target, int to_target) {
  //A phrase is consistent if their source words are only aligned with
  //their target words and vice versa. No alignment with words outside
  //the phrase are allowed
  for (int i=from_source; i<=to_source; i++) {
    for (int j=0; j<from_target; j++)
      if (alignment[i][j])
	return false;
    for (int j=to_target+1; j<(int)target.size(); j++)
      if (alignment[i][j])
	return false;
  }

  for (int i=from_target; i<=to_target; i++) {
    for (int j=0; j<from_source; j++)
      if (alignment[j][i])
	return false;
    for (int j=to_source+1; j<(int)source.size(); j++)
      if (alignment[j][i])
	return false;
  }

  return true;
  /*
  //Moreover, the first and the last word of the phrase (on both
  //sides) must be aligned with at least one word of the other side
  bool first_src_aligned=false;
  bool last_src_aligned=false;
  bool first_tgt_aligned=false;
  bool last_tgt_aligned=false;

  for(int j=from_target; j<=to_target; j++) {
    if (alignment[from_source][j]) {
      first_src_aligned=true;
    }
    if (alignment[to_source][j]) {
      last_src_aligned=true;
    }
  }

  for(int i=from_source; i<=to_source; i++) {
    if (alignment[i][from_target]) {
      first_tgt_aligned=true;
    }
    if (alignment[i][to_target]) {
      last_tgt_aligned=true;
    }
  }

  return (first_src_aligned && last_src_aligned && first_tgt_aligned
  && last_tgt_aligned);
  */
}

Alignment 
Alignment::sub_alignment(int from_source, int to_source, int from_target, int to_target) {
  Alignment al;

  //It is supossed that words within the resulting alignment are NOT
  //aligned with words outside. This should be tested before calling
  //this function (see function consistent)

  al.score=score;

  for(int i=from_source; i<=to_source; i++)
    al.source.push_back(source[i]);

  for(int i=from_target; i<=to_target; i++)
    al.target.push_back(target[i]);

  for (int i=from_source; i<=to_source; i++) {
    for(int j=from_target; j<=to_target; j++) 
      al.alignment[i-from_source][j-from_target]=alignment[i][j];    
  }

  return al;
}

ostream& operator << (ostream& os, Alignment& al) {

  os<<al.to_string();
  return os;

  /*
  os<<al.score<<" |";

  for(int i=0; i<al.source.size(); i++) 
    os<<" "<<al.source[i];
  os<<" |";

  for(int i=0; i<al.target.size(); i++) 
    os<<" "<<al.target[i];
  os<<" |";

  for (int i=0; i<al.source.size(); i++) {
    for(int j=0; j<al.target.size(); j++) {
      if (al.alignment[i][j])
	os<<" "<<i<<":"<<j;
    }
  }

  return os;
  */
}

bool 
Alignment::allwords_aligned() {

  //Test if each source word is aligned with at least one target word
  for(unsigned i=0; i<source.size(); i++) {
    int is_aligned=false;
    for(unsigned j=0; (j<target.size()) && (!is_aligned); j++) {
      if (alignment[i][j])
	is_aligned=true;
    }
    if (!is_aligned)
      return false;
  }

  //Test if each target word is aligned with at least one source word
  for(unsigned j=0; j<target.size(); j++) {
    int is_aligned=false;
    for(unsigned i=0; (i<source.size()) && (!is_aligned); i++) {
      if (alignment[i][j])
	is_aligned=true;
    }
    if (!is_aligned)
      return false;
  }

  return true;
}

bool 
Alignment::are_the_same_alignment(const Alignment& al2) {
  bool ok=true;

  if ((source.size()!=al2.source.size())||
      (target.size()!=al2.target.size())) {
    ok=false;
  }

  for(unsigned i=0; (i<source.size()) && (ok); i++) {
    if (source[i]!=al2.source[i])
      ok=false;
  }

  for(unsigned i=0; (i<target.size()) && (ok); i++) {
    if (target[i]!=al2.target[i])
      ok=false;
  }
  return ok;
}

bool 
Alignment::intersection(Alignment& al2) {
  if (!are_the_same_alignment(al2)) {
    cerr<<"Error when intersecting the following two alignments:\n";
    cerr<<to_string()<<"\n";
    cerr<<al2.to_string()<<"\n";
    return false;
  }

  for(unsigned i=0; i<source.size(); i++) {
    for(unsigned j=0; j<target.size(); j++) {
      if ((alignment[i][j]) && (!al2.alignment[i][j])) {
	alignment[i][j]=false;
      }
    }
  }

  return true;
}

bool
Alignment::unionn(Alignment& al2) {
  if (!are_the_same_alignment(al2)) {
    cerr<<"Error when uniting the following two alignments:\n";
    cerr<<to_string()<<"\n";
    cerr<<al2.to_string()<<"\n";
    return false;
  }

  for(unsigned i=0; i<source.size(); i++) {
    for(unsigned j=0; j<target.size(); j++) {
      if (al2.alignment[i][j]) {
	alignment[i][j]=true;
      }
    }
  }

  return true;
}

bool 
Alignment::refined_intersection(Alignment& al2) {
  if (!are_the_same_alignment(al2)) {
    cerr<<"Error when performing the refined intersection of the following two alignments:\n";
    cerr<<to_string()<<"\n";
    cerr<<al2.to_string()<<"\n";
    return false;
  }

  //We save the alignment of (*this) before intersecting
  map<int, map<int, bool> > al1;
  for(unsigned i=0; i<source.size(); i++) {
    for(unsigned j=0; j<target.size(); j++) {
      al1[i][j]=alignment[i][j];
    }
  }

  //First, the intersection
  intersection(al2);
  //cerr<<"(0) "<<to_string()<<"\n";
  bool alignments_changed;
  //int nit=0;
  do {
    alignments_changed=false;
    //nit++;
    for (unsigned i=0; i<source.size(); i++) {
      for(unsigned j=0; j<target.size(); j++) {
	if (!alignment[i][j]) {
	  if ((al1[i][j]) || (al2.alignment[i][j])) {
	    //cerr<<"   considering ("<<i<<","<<j<<")\n";
	    bool add_alignment=true;
	    for(unsigned k=0; k<target.size(); k++) {
	      if (alignment[i][k])
		add_alignment=false;
	    }
	    for(unsigned k=0; k<source.size(); k++) {
	      if (alignment[k][j])
		add_alignment=false;
	    }
	    if (!add_alignment) {
	      //cerr<<"   ("<<i<<","<<"*) or (*,"<<j<<") are already in A\n";
	      //The aligment can still be added if it has an horizontal
	      //neighbor or a vertical neighbor already in 'alignment'
	      if ( ((alignment[i-1][j])||(alignment[i+1][j])) ||
		   ((alignment[i][j-1])||(alignment[i][j+1])) ) {
		//cerr<<"   ("<<i<<","<<j<<") has an horizontal or a vertical neighbor in A\n";
		alignment[i][j]=true; 
		//Now we that test there is no aligments in 'alignment' with
		//both horizontal and vertical neighbors
		bool neighbors=false;
		for(unsigned ii=0; (ii<source.size()) && (!neighbors); ii++) {
		  for(unsigned jj=0; (jj<target.size()) && (!neighbors); jj++) {
		    if (alignment[ii][jj]) {
		      if ( ((alignment[ii-1][jj])||(alignment[ii+1][jj])) &&
			   ((alignment[ii][jj-1])||(alignment[ii][jj+1])) ) {
			//cerr<<"   ("<<i<<","<<j<<") has both horizontal and vertical neighbors\n";
			neighbors=true;
		      }
		    }
		  }
		}
		alignment[i][j]=false;
		if(!neighbors)
		  add_alignment=true;
	      }
	    }
	    if (add_alignment) {
	      //cerr<<"   adding ("<<i<<","<<j<<")\n";
	      alignment[i][j]=true;
	      alignments_changed=true;
	    }
	  }
	}
      }
    }
    //cerr<<"("<<nit<<") "<<to_string()<<"\n";
  } while(alignments_changed);

  return true;
}
