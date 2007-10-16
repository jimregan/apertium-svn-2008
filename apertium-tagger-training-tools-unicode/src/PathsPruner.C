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
  Utils::print_debug(L"In PathsPruner::compute_paths_ranking\n");
  for(int i=0; i<seg->get_number_paths(); i++) {
    p.first=i;
    p.second=a_priori_likelihood(i);

    prob_path.push_back(p);
    sum_likelihood+=p.second;
  }

  //Normalize for better treatment
  for(int i=0; i<prob_path.size(); i++) 
    prob_path[i].second=prob_path[i].second/sum_likelihood;
  

  Utils::print_debug(L"Before sort\n-----------------------\n");
  for(int i=0; i<prob_path.size(); i++) {
    Utils::print_debug(prob_path[i].first);
    Utils::print_debug(L" ");
    Utils::print_debug(prob_path[i].second);
    Utils::print_debug(L"\n");
  }

  //Sort
  pair_comparer_greater<pair<int, double> > comparer;
  sort(prob_path.begin(), prob_path.end(), comparer);

  reset_paths_counter();

  Utils::print_debug(L"After sort\n-----------------------\n");
  for(int i=0; i<prob_path.size(); i++) {
    Utils::print_debug(prob_path[i].first);
    Utils::print_debug(L" ");
    Utils::print_debug(prob_path[i].second);
    Utils::print_debug(L"\n");
  }

  Utils::print_debug(L"Calculating the number of paths that will be taken into account\n");
  Utils::print_debug(L"----------------------------------------------\n");
  //Now depending on the path pruning mode we calculte how many
  //disambiguation paths must be taken into account 
  
  double sum_prob=0.0;
  k=0;
  for(int i=0; i<prob_path.size(); i++) {
    if (sum_prob<probmass) {
      k++;
      sum_prob+=prob_path[i].second;
      Utils::print_debug(L"sum_prob: ");
      Utils::print_debug(sum_prob);
      Utils::print_debug(L" path to be taken into account: ");
      Utils::print_debug(prob_path[i].first);
      Utils::print_debug(L" with prob ");
      Utils::print_debug(prob_path[i].second);
      Utils::print_debug(L"\n");
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
  
  Utils::print_debug(L"There will be taken into account ");
  Utils::print_debug(k);
  Utils::print_debug(L" disambiguation paths of ");
  Utils::print_debug(seg->get_number_paths());
  Utils::print_debug(L"\n");
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

/*
//Esta implementación no tiene en cuenta la etiqueta de la 
//ultima palabra del segmento anterior
double 
PathsPruner::a_priori_likelihood(int path) {
  static double prob_unknown=transition_prob_unknown_words();

  set<TTag> open_class = tagger_data.getOpenClass();

  vector <TTag> tagseq;
  TTag tag1, tag2;
  set<TTag> tags1, tags2;
  set<TTag>::iterator it;
  int k1, k2;

  double ret_prob=1.0;

  seg->get_path(tagseq, path);


  if (seg->vwords.size()==0) {
    cerr<<"Error in PathsPruner::a_priori_likelihood: Segment length is null\n";
  }

  tag1=tagseq[0];

  if(tag1<0) 
    tags1=open_class;
  else
    tags1=seg->vwords[0].get_tags();

  if(tagger_data.getOutput().has_not(tags1)) {
    cerr<<"Error: In PathsPruner::a_priori_likelihood:\n";
    cerr<<"A new ambiguty class was found\n";
    exit(1);
  }

  k1=tagger_data.getOutput()[tags1];

  for (int i=1; i<tagseq.size(); i++) {     
    tag2=tagseq[i];
    if (tag2<0) //This is an unknown word
      tags2=open_class;
    else   
      tags2=seg->vwords[i].get_tags();
     
    if(tagger_data.getOutput().has_not(tags2)) {
      cerr<<"Error: In PathsPruner::a_priori_likelihood:\n";
      cerr<<"A new ambiguity class was found\n";
      exit(1);
    }
      
    k2=tagger_data.getOutput()[tags2];

    double prob=0;
      
    if ((tag1>=0)&&(tag2<0)) { //2nd word is unknown
      for(it=open_class.begin(); it!=open_class.end(); it++) {
	prob+=tagger_data.getA()[tag1][*it];
      }
      prob=tagger_data.getB()[tag1][k1]*(prob/((double)open_class.size()));
    } 

    else if((tag1<0)&&(tag2>=0)) { //1st word in unknown
      for(it=open_class.begin(); it!=open_class.end(); it++) {
	prob+=tagger_data.getB()[*it][k1]*tagger_data.getA()[*it][tag2];
      }
      prob=prob/((double)open_class.size());
    } 

    else if((tag1<0)&&(tag2<0)) { //Boths words are unknown
      prob=prob_unknown;
    } 

    else  
      prob=tagger_data.getB()[tag1][k1]*tagger_data.getA()[tag1][tag2];

    ret_prob*=prob;

    tag1=tag2;
    tags1=tags2;
    k1=k2;
  }


  //We need to take into account also the emission probability of the
  //last tag (tag1). It can come from an unknown word
  double prob=0;

  if(tag1<0) { //Word is unknown
    for(it=open_class.begin(); it!=open_class.end(); it++)
      prob+=tagger_data.getB()[*it][k1];
    prob=prob/((double)open_class.size());
  } else { 
    prob=tagger_data.getB()[tag1][k1];
  }

  ret_prob*=prob;

  if (ret_prob<=0.0) {
    cerr<<"Warning: a priori likelihood of path "<<path<<" is null, prob: "<<ret_prob<<"\n";
    cerr<<"SEGMENT: ";
    for(int i=0; i<seg->vwords.size(); i++) {
      cerr<<seg->vwords[i].get_superficial_form()<<" ";
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
*/

//En esta implementación se tiene en cuenta la etiqueta de la ultima
//palabra del segmento anterior.
double 
PathsPruner::a_priori_likelihood(int path) {
  static double prob_unknown=transition_prob_unknown_words();

  set<TTag> open_class = tagger_data.getOpenClass();

  vector <TTag> tagseq;
  TTag tag1, tag2;
  set<TTag> tags1, tags2;
  set<TTag>::iterator it;
  int k1, k2;

  double ret_prob=1.0;

  seg->get_path(tagseq, path);

  tag1=last_tag_previous_segment;

  if(tag1<0) //The last word of the previous segment was unknown
    tags1=tagger_data.getOpenClass();
  else
    tags1.insert(last_tag_previous_segment); //We can do that because
  //the last word of each
  //segment is always unambiguous


  if(tagger_data.getOutput().has_not(tags1)) {
    cerr<<"Error: In PathsPruner::a_priori_likelihood:\n";
    cerr<<"A new ambiguty class was found\n";
    exit(1);
  }

  k1=tagger_data.getOutput()[tags1];
   
  for (int i=0; i<tagseq.size(); i++) {      
     
    tag2=tagseq[i];
    if (tag2<0) //This is an unknown word
      tags2=tagger_data.getOpenClass();
    else   
      tags2=seg->vwords[i].get_tags();
     
    if(tagger_data.getOutput().has_not(tags2)) {
      cerr<<"Error: In PathsPruner::a_priori_likelihood:\n";
      cerr<<"A new ambiguity class was found\n";
      exit(1);
    }
      
    k2=tagger_data.getOutput()[tags2];

    double prob=0;
      
    if ((tag1>=0)&&(tag2<0)) { //2nd word is unknown
      for(it=open_class.begin(); it!=open_class.end(); it++) {
	prob+=tagger_data.getA()[tag1][*it];
      }
      prob=tagger_data.getB()[tag1][k1]*(prob/((double)open_class.size()));
    } 

    else if((tag1<0)&&(tag2>=0)) { //1st word in unknown
      for(it=open_class.begin(); it!=open_class.end(); it++) {
	prob+=tagger_data.getB()[*it][k1]*tagger_data.getA()[*it][tag2];
      }
      prob=prob/((double)open_class.size());
    } 

    else if((tag1<0)&&(tag2<0)) { //Boths words are unknown
      prob=prob_unknown;
    } 

    else  
      prob=tagger_data.getB()[tag1][k1]*tagger_data.getA()[tag1][tag2];

    ret_prob*=prob;

    tag1=tag2;
    tags1=tags2;
    k1=k2;
  }


  //We need to take into account also the emission probability of the
  //last tag (tag1). It can come from an unknown word
  double prob=0;

  if(tag1<0) { //Word is unknown
    for(it=open_class.begin(); it!=open_class.end(); it++)
      prob+=tagger_data.getB()[*it][k1];
    prob=prob/((double)open_class.size());
  } else { 
    prob=tagger_data.getB()[tag1][k1];
  }

  ret_prob*=prob;

  if (ret_prob<=0.0) {
    wcerr<<L"Warning: a priori likelihood of path "<<path<<L" is null, prob: "<<ret_prob<<L"\n";
    wcerr<<L"SEGMENT: ";
    for(int i=0; i<seg->vwords.size(); i++) {
      wcerr<<seg->vwords[i].get_superficial_form()<<L" ";
    }
    wcerr<<L"\nPATH: ";      
    for(int i=0; i<tagseq.size(); i++) {
      cerr<<tagseq[i]<<L" ";
    }
    wcerr<<L"\n";
   
    ret_prob=DBL_MIN;
  }

  return ret_prob;
}


double
PathsPruner::transition_prob_unknown_words() {
  set<TTag> tags1, tags2;
  set<TTag>::iterator itag1, itag2;
  int k1, k2;    
   
  tags1=tags2=tagger_data.getOpenClass();

  k1=k2=tagger_data.getOutput()[tags1];
   
  double prob=0;
  for(itag1=tags1.begin(); itag1!=tags1.end(); itag1++) {
    double prob2=0;
    for(itag2=tags2.begin(); itag2!=tags2.end(); itag2++) {
      prob2+=tagger_data.getA()[*itag1][*itag2]; 
    }
    prob+=tagger_data.getB()[*itag1][k1]*(prob2/((double)tags2.size()));
  }

  return prob/((double)tags1.size());
}
