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

#include "PathsPruner.H"

#include <algorithm>
#include <cfloat>
#include <cmath>

#include <apertium/utf_converter.h>

#include "Utils.H"

TaggerData PathsPruner::tagger_data;
int PathsPruner::mode;
int PathsPruner::latency;
double PathsPruner::probmass;
//double PathsPruner::epsilon;
//double PathsPruner::delta;
int PathsPruner::corpus_length;

//Definition of the comparer function to be used to sort the
//prob_path vector. Sort by the second field, the probability
template<class T> class pair_comparer_greater {
public:
  bool operator()(const T &e1, const T &e2)  const {
    return (e1.second > e2.second);
  }
};

PathsPruner::PathsPruner(Segment *s, TTag last_tag) {
  seg=s;
  last_tag_previous_segment=last_tag;
  paths_counter=0;
  k=0;
}

void
PathsPruner::reset_paths_counter() {
  paths_counter=0;
}

void
PathsPruner::set_tagger_data(const TaggerData& td) {
  tagger_data=td;
}

double 
PathsPruner::calculate_probmass_to_take_into_account(double nwords_processed) {
  double aux;
  switch (mode) {
  case 1:
    //Nothing to do, probmass was specified at the begining
    break;
  case 2:
    probmass=1.0-(nwords_processed/((double)corpus_length));
    break;
  case 3:
    aux=nwords_processed/(((double)corpus_length)*0.1)-5.0;
    probmass=1.0-(1.0/(1.0+exp((-1.0)*aux)));
    break;
  case 4:
    aux=nwords_processed/(((double)corpus_length)*0.25);
    probmass=exp(-aux);
    break;
  case 5:
    return pow(nwords_processed,2.0)/pow(corpus_length,2.0);
    break;
  default:
    cerr<<"Error in PathsPruner::calculate_probmass_to_take_into_account: ";
    cerr<<"Unknown pruning mode "<<mode<<"\n";
    exit(EXIT_FAILURE);
  }
  return probmass;
}

void 
PathsPruner::compute_paths_ranking() {
  pair<int, double> p;

  double sum_likelihood=0.0;
  Utils::print_debug("In PathsPruner::compute_paths_ranking\n");
  for(int i=0; i<seg->get_number_paths(); i++) {
    p.first=i;
    p.second=a_priori_likelihood(i);

    //Utils::print_debug("PATH ");
    //Utils::print_debug(i);
    //Utils::print_debug(", a_priori_likelihood: ");
    //Utils::print_debug(p.second);
    //Utils::print_debug("\n");

    prob_path.push_back(p);
    sum_likelihood+=p.second;
  }

  //Normalize for better treatment
  for(int i=0; i<prob_path.size(); i++) 
    prob_path[i].second=prob_path[i].second/sum_likelihood;
  

  //Utils::print_debug("Before sort\n-----------------------\n");
  //for(int i=0; i<prob_path.size(); i++) {
  //  Utils::print_debug(prob_path[i].first);
  //  Utils::print_debug(" ");
  //  Utils::print_debug(prob_path[i].second);
  //  Utils::print_debug("\n");
  //}

  //Sort
  pair_comparer_greater<pair<int, double> > comparer;
  sort(prob_path.begin(), prob_path.end(), comparer);

  reset_paths_counter();

  //Utils::print_debug("After sort\n-----------------------\n");
  //for(int i=0; i<prob_path.size(); i++) {
  //  Utils::print_debug(prob_path[i].first);
  //  Utils::print_debug(" ");
  //  Utils::print_debug(prob_path[i].second);
  //  Utils::print_debug("\n");
  //}

  Utils::print_debug("Calculating the number of paths that will be taken into account\n");
  Utils::print_debug("----------------------------------------------\n");
  //Now depending on the path pruning mode we calculte how many
  //disambiguation paths must be taken into account 
  
  double sum_prob=0.0;
  k=0;
  for(int i=0; i<prob_path.size(); i++) {
    if (sum_prob<probmass) {
      k++;
      sum_prob+=prob_path[i].second;
      Utils::print_debug("sum_prob: ");
      Utils::print_debug(sum_prob);
      Utils::print_debug(" path to be taken into account: ");
      Utils::print_debug(prob_path[i].first);
      Utils::print_debug(" with prob ");
      Utils::print_debug(prob_path[i].second);
      Utils::print_debug("\n");
    } else {
      //At least one path to take into account
      if (k==0) {
	k++;
	sum_prob+=prob_path[i].second;
      } 
      /*else {	
	fstream f;
	f.open("cuentas_japo.txt", fstream::out | fstream::app);
	if(i>0) {
	f<<prob_path[i].second/prob_path[i-1].second<<"\n";
	} else {
	f<<"-0.0";
	}
	f.close();	
	}*/
      break;
    }
    /*
      else {
      double diff=abs(prob_path[i-1].second - prob_path[i].second);
      double relevance;
      //We calculate the amount of probability added if we consider
      //this path in respect to the probability added since this
      //point
      double sumaux=0;
      for(int j=0; j<=i; j++)
      sumaux+=prob_path[j].second;
      relevance=prob_path[i].second/sumaux;

      Utils::print_debug("Testing if path ");
      Utils::print_debug(prob_path[i].first);
      Utils::print_debug(" with prob ");
      Utils::print_debug(prob_path[i].second);
      Utils::print_debug(" must be taken into account\n");
      Utils::print_debug("diff = ");
      Utils::print_debug(diff);
      Utils::print_debug(" relevance = ");
      Utils::print_debug(relevance);
      Utils::print_debug("\n");
      if ((diff<=epsilon) && (relevance>=delta)) {
      k++;
      sum_prob+=prob_path[i].second;
      Utils::print_debug("The path WILL BE TAKEN INTO ACCOUNT\n");
      } else
      break;
      }
    */
  }
  probmass_considered=sum_prob;
  
  Utils::print_debug("There will be taken into account ");
  Utils::print_debug(k);
  Utils::print_debug(" disambiguation paths of ");
  Utils::print_debug(seg->get_number_paths());
  Utils::print_debug("\n");
}

int
PathsPruner::get_next_path() {
  int ret_path=-1;
  if ((paths_counter<prob_path.size()) && (paths_counter<k))
    ret_path=prob_path[paths_counter].first;   
  
  paths_counter++;
  return ret_path;
}

int 
PathsPruner::get_number_considered_paths() {
  return k;
}


double 
PathsPruner::get_probmass_considered() {
  return probmass_considered;
}

double 
PathsPruner::a_priori_likelihood(int path) {
  double ret_prob;
  vector <TTag> tagseq;

  TTag tag;
  set<TTag> tags, pretags;
  set<TTag>::iterator itag, jtag;

  map <int, map <int, double> > alpha, beta;

  seg->get_path(tagseq, path);

  //Utils::print_debug("Caculating a priori likelihood for path ");
  //Utils::print_debug(path);
  //Utils::print_debug(":\n");
  //if (Utils::debug) {
  //  for(int i=0; i<tagseq.size(); i++) {
  //    cerr<<UtfConverter::toUtf8(tagger_data.getArrayTags()[tagseq[i]])<<" ";
  //  }
  //  cerr<<"\n";
  //}

  if ((seg->vwords_before.size()==0) || (seg->vwords_after.size()==0)) {
    cerr<<"Error: This segment has no context\n";
    cerr<<"SEGMENT: ";
    for(int i=0; i<seg->vwords.size(); i++) {
      cerr<<UtfConverter::toUtf8(seg->vwords[i].get_superficial_form())<<" ";
    }
    exit(EXIT_FAILURE);
  }

  if (seg->vwords_before.front().get_tags().size()!=1) {
    cerr<<"Error in PathsPruner::a_priori_likelihood: First word of seg->vwords_before is ambiguous\n";
    cerr<<"SEGMENT: ";
    for(int i=0; i<seg->vwords.size(); i++) {
      cerr<<UtfConverter::toUtf8(seg->vwords[i].get_superficial_form())<<" ";
    }
    cerr<<"WORDS BEFORE: ";
    for(int i=0; i<seg->vwords_before.size(); i++) {
      cerr<<UtfConverter::toUtf8(seg->vwords_before[i].get_superficial_form())<<" ";
    }
    exit(EXIT_FAILURE);
  } 

  if (seg->vwords_after.back().get_tags().size()!=1) {
    cerr<<"Error in PathsPruner::a_priori_likelihood: Last word of seg->vwords_after is ambiguous\n";
    cerr<<"SEGMENT: ";
    for(int i=0; i<seg->vwords.size(); i++) {
      cerr<<UtfConverter::toUtf8(seg->vwords[i].get_superficial_form())<<" ";
    }
    cerr<<"WORDS AFTER: ";
    for(int i=0; i<seg->vwords_after.size(); i++) {
      cerr<<UtfConverter::toUtf8(seg->vwords_after[i].get_superficial_form())<<" ";
    }
    exit(EXIT_FAILURE);
  } 

  if (seg->vwords.size() != tagseq.size()) {
    cerr<<"Error: Number of tags in tagseq differs from number of words in seg->vwords\n";
    cerr<<"SEGMENT: ";
    for(int i=0; i<seg->vwords.size(); i++) {
      cerr<<UtfConverter::toUtf8(seg->vwords[i].get_superficial_form())<<" ";
    }
    exit(EXIT_FAILURE);
  }

  //Forward probabilities
  tag = *(seg->vwords_before.front().get_tags().begin()); //Not ambiguous
  alpha[0][tag] = 1;
  pretags=seg->vwords_before.front().get_tags();

  //Utils::print_debug("alpha[0][");
  //Utils::print_debug(tagger_data.getArrayTags()[tag]);
  //Utils::print_debug("]=");
  //Utils::print_debug(alpha[0][tag]);
  //Utils::print_debug("\n");

  for (int i=1; i<seg->vwords_before.size(); i++) {
    tags=seg->vwords_before[i].get_tags();
    if (tags.size()==0)
      tags=tagger_data.getOpenClass();
    if(tagger_data.getOutput().has_not(tags)) {
      cerr<<"Error: In PathsPruner::a_priori_likelihood:\n";
      cerr<<"A new ambiguity class was found\n";
      exit(EXIT_FAILURE);
    }

    k = tagger_data.getOutput()[tags];

    for (itag=tags.begin(); itag!=tags.end(); itag++)
      for (jtag=pretags.begin(); jtag!=pretags.end(); jtag++)
	alpha[i][*itag]+= alpha[i-1][*jtag]*tagger_data.getA()[*jtag][*itag]*tagger_data.getB()[*itag][k];

    //if(Utils::debug) {
    //  for (itag=tags.begin(); itag!=tags.end(); itag++)
    //	  cerr<<"alpha["<<i<<"]["<<UtfConverter::toUtf8(tagger_data.getArrayTags()[*itag])<<"]="<<alpha[i][*itag]<<"\n";
    //}

    pretags=tags;
  }

  tags=seg->vwords.front().get_tags();
  if (tags.size()==0)
    tags=tagger_data.getOpenClass();
  if(tagger_data.getOutput().has_not(tags)) {
    cerr<<"Error: In PathsPruner::a_priori_likelihood:\n";
    cerr<<"A new ambiguity class was found\n";
    exit(EXIT_FAILURE);
  }

  k = tagger_data.getOutput()[tags];

  ret_prob=0.0;
  for (jtag=pretags.begin(); jtag!=pretags.end(); jtag++)
    ret_prob+=alpha[seg->vwords_before.size()-1][*jtag]*tagger_data.getA()[*jtag][tagseq.front()]*tagger_data.getB()[tagseq.front()][k];

  //Sequence of tags of the path being evaluated
  for(int i=1; i<seg->vwords.size(); i++) {
    tags=seg->vwords[i].get_tags();
    if(tags.size()==0)
      tags=tagger_data.getOpenClass();
    if(tagger_data.getOutput().has_not(tags)) {
      cerr<<"Error: In PathsPruner::a_priori_likelihood:\n";
      cerr<<"A new ambiguity class was found\n";
      exit(EXIT_FAILURE);
    }

    k = tagger_data.getOutput()[tags];

    ret_prob*=tagger_data.getA()[tagseq[i-1]][tagseq[i]]*tagger_data.getB()[tagseq[i]][k];

    pretags=tags;
  }

  //Backward probabilities
  tags=seg->vwords_after.front().get_tags();
  if(tags.size()==0)
    tags=tagger_data.getOpenClass();
  if(tagger_data.getOutput().has_not(tags)) {
    cerr<<"Error: In PathsPruner::a_priori_likelihood:\n";
    cerr<<"A new ambiguity class was found\n";
    exit(EXIT_FAILURE);
  }

  k = tagger_data.getOutput()[tags];

  for (jtag=tags.begin(); jtag!=tags.end(); jtag++) {
    beta[0][*jtag]=ret_prob*tagger_data.getA()[tagseq.back()][*jtag]*tagger_data.getB()[*jtag][k];
  }

  //if(Utils::debug) {
  //  for (jtag=tags.begin(); jtag!=tags.end(); jtag++)
  //    cerr<<"beta[0]["<<UtfConverter::toUtf8(tagger_data.getArrayTags()[*jtag])<<"]="<<beta[0][*jtag]<<"\n";
  //}

  pretags=tags;

  for (int i=1; i<seg->vwords_after.size(); i++) {
    tags=seg->vwords_after[i].get_tags();
    if (tags.size()==0)
      tags=tagger_data.getOpenClass();
    if(tagger_data.getOutput().has_not(tags)) {
      cerr<<"Error: In PathsPruner::a_priori_likelihood:\n";
      cerr<<"A new ambiguity class was found\n";
      exit(EXIT_FAILURE);
    }

    k = tagger_data.getOutput()[tags];

    for (itag=tags.begin(); itag!=tags.end(); itag++)
      for (jtag=pretags.begin(); jtag!=pretags.end(); jtag++) 
	beta[i][*itag]+= beta[i-1][*jtag]*tagger_data.getA()[*jtag][*itag]*tagger_data.getB()[*itag][k];

    //if(Utils::debug) {
    //  for (itag=tags.begin(); itag!=tags.end(); itag++)
    //    cerr<<"beta["<<i<<"]["<<UtfConverter::toUtf8(tagger_data.getArrayTags()[*itag])<<"]="<<beta[i][*itag]<<"\n";
    //}

    pretags=tags;
  }

  tag = *(seg->vwords_after.back().get_tags().begin());
  ret_prob = beta[seg->vwords_after.size()-1][tag];

  //Utils::print_debug("Return value: ");
  //Utils::print_debug(ret_prob);
  //Utils::print_debug("\n");

  if (ret_prob<=0.0) {
    cerr<<"Warning: a priori likelihood of path "<<path<<" is null, prob: "<<ret_prob<<"\n";
    cerr<<"SEGMENT: ";
    for(int i=0; i<seg->vwords.size(); i++) {
      cerr<<UtfConverter::toUtf8(seg->vwords[i].get_superficial_form())<<" ";
    }
    cerr<<"\nPATH: ";      
    for(int i=0; i<tagseq.size(); i++) {
      cerr<<tagseq[i]<<" ";
    }
    cerr<<"\n";
   
    ret_prob=DBL_MIN;
  }

  return ret_prob;
}

