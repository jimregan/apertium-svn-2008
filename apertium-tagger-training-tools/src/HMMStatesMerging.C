/*
 * Copyright (C) 2007 Felipe Sánchez-Martínez
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
/**
 * Class HMMStatesMerging  (source file)
 *
 *  @author   Felipe Sánchez-Martínez - fsanchez@dlsi.ua.es
 */

#include "HMMStatesMerging.H"

#include <lttoolbox/Compression.H>
#include <lttoolbox/EndianUtil.H>

#include <apertium/TaggerUtils.H>

#include <algorithm>

#include "Utils.H"

HMMStatesMerging::HMMStatesMerging(FILE *fprob, const string& divfile, bool restrict) {
  cerr<<"Reading apertium-tagger parameters ... "<<flush;
  tagger_data.read(fprob);
  cerr<<"done.\n";

  cerr<<tagger_data.getN()<<" states and "<<tagger_data.getM()<<" ambiguity classes\n";

  divergences_file=divfile;

  if (divergences_file!="") {
    cerr<<"File to store/read divergences: "<<divergences_file<<"\n";
  }

  threshold=0;

  restrictive=restrict;
}

double&
HMMStatesMerging::divergence(int i, int j) {
  if (i>j) {
    int aux=i;
    i=j;
    j=aux;
  }

  return divergences[i][j];
}

double 
HMMStatesMerging::kullback_leibler(int state_a, int state_b) {
  double distance=0.0;

  for (int i=0; i<tagger_data.getN(); i++) {
    //if (tagger_data.getA()[state_a][i]==0)
    //  tagger_data.getA()[state_a][i]=SMALL_NUMBER;

    //if (tagger_data.getA()[state_b][i]==0)
    //  tagger_data.getA()[state_b][i]=SMALL_NUMBER;

    if (tagger_data.getA()[state_a][i]<=0) {
      cerr<<"Warning: a["<<state_a<<"]["<<i<<"]="<<tagger_data.getA()[state_a][i]<<", kullback-leibler returning INF\n";
      return BIG_NUMBER; 
    }
    if (tagger_data.getA()[state_b][i]<=0) {
      cerr<<"Warning: a["<<state_b<<"]["<<i<<"]="<<tagger_data.getA()[state_b][i]<<", kullback-leibler returning INF\n";
      return BIG_NUMBER;
    }

    distance += tagger_data.getA()[state_a][i] * Utils::log2(tagger_data.getA()[state_a][i]/tagger_data.getA()[state_b][i]);
  }

  return distance;
}

double
HMMStatesMerging::calculate_divergence(int state_a, int state_b) {
  if ((state_a>=tagger_data.getN()) || (state_b>=tagger_data.getN())) {
    cerr<<"Error in divergence: state_a or state_b out of range\n";
    cerr<<"state_a="<<state_a<<"; state_b="<<state_b<<"; N="<<tagger_data.getN()<<"\n";
    exit(EXIT_FAILURE);
  }

  //return (kullback_leibler(state_a, state_b)+kullback_leibler(state_b, state_a));

  double kl_ab, kl_ba;
  kl_ab=kullback_leibler(state_a, state_b);
  kl_ba=kullback_leibler(state_b, state_a);

  if (kl_ab<kl_ba)
    return kl_ab;
  else
    return kl_ba;
}

set<int> 
HMMStatesMerging::ambiguity_classes_state_appears(int state) {
  set<int> set_amb_classes;

  for (int k=0; k<tagger_data.getM(); k++) {
    if(tagger_data.getOutput()[k].find(state)!=tagger_data.getOutput()[k].end())
      set_amb_classes.insert(k);
  }

  return set_amb_classes;
}

bool 
HMMStatesMerging::sastify_merge_constraints(int state_a, int state_b) {
  set<int> set_amb_classes_a = ambiguity_classes_state_appears(state_a);
  set<int> set_amb_classes_b = ambiguity_classes_state_appears(state_b);
  set<int> amb_class_intersection;

  set_intersection(set_amb_classes_a.begin(), set_amb_classes_a.end(),
                   set_amb_classes_b.begin(), set_amb_classes_b.end(),
                   inserter(amb_class_intersection, amb_class_intersection.begin()));

  //Two states cannot be merged if the share one or more observable
  //outputs, i.e. ambiguity classes

  return (amb_class_intersection.size()==0);
}

void
HMMStatesMerging::calculate_all_divergences() {
  int nno_satisfy_merging_const=0;
  fstream fdiv;
  bool do_calculation=true;

  if (divergences_file!="") {
    fdiv.open(divergences_file.c_str(), ios::in);
    if (fdiv.good()) {
      do_calculation=false;
    } 
  }

  if (do_calculation) {
    cerr<<"Calculating divergences between each pair of states ...\n";
    for(int i=0; i<tagger_data.getN(); i++) {
      cerr<<i<<" "<<flush;
      for(int j=i+1; j<tagger_data.getN(); j++) {
	if (i==j) {
	  divergence(i, j) = 0;
	  continue;
	}
	if (sastify_merge_constraints(i, j)) {
	  divergence(i, j) = calculate_divergence(i, j);
	} else { 
	  nno_satisfy_merging_const++;
	  divergence(i, j) = INCOMPATIBLE;
	}
      }
    }
    cerr<<"\n";
    cerr<<nno_satisfy_merging_const<<" states pairs cannot be merged because "
	<<"they share one or more observable output\n";

  } else { //Read all the divergences from a file
    cerr<<"Reading divergences from file '"<<divergences_file<<"'\n";
    int i, j;
    double d;
    while(!fdiv.eof()) {
      i=(int)Compression::multibyte_read(fdiv);
      j=(int)Compression::multibyte_read(fdiv);
      d=EndianUtil<double>::read(fdiv);
      if (fdiv.good()) {
	divergence(i, j) = d;
      }
    }	
  }

  fdiv.close();

  if ((do_calculation)&&(divergences_file!="")) {
    fdiv.open(divergences_file.c_str(), ios::out | ios::trunc);
    if (!fdiv.good()) {
      cerr<<"Error: Cannot open output file '"<<divergences_file<<"'\n";
      cerr<<"Divergences will not be saved\n";
    } else {
      cerr<<"Saving divergences into file '"<<divergences_file<<"' ... ";
      for(int i=0; i<tagger_data.getN(); i++) {
	for(int j=i+1; j<tagger_data.getN(); j++) {
	  if (divergence(i, j)!=0) {
	    Compression::multibyte_write(i, fdiv);
	    Compression::multibyte_write(j, fdiv);
	    EndianUtil<double>::write(fdiv, divergence(i, j));
	  }
	}
      }
      cerr<<"done.\n";
    }
    fdiv.close();
  }
}
 
double 
HMMStatesMerging::clusters_distance(set<int> cl1, set<int> cl2) {
  set<int>::iterator it1, it2;
  double d=0;

  for(it1=cl1.begin(); it1!=cl1.end(); it1++) {
    for(it2=cl2.begin(); it2!=cl2.end(); it2++) {
      double dv=divergence(*it1, *it2);

      if (dv==INCOMPATIBLE) 
	return BIG_NUMBER;

      if ((restrictive) && (dv>threshold))
	return BIG_NUMBER;

      d+=dv;
    }
  }
  return d/((double)(cl1.size()*cl2.size()));
}

vector<set<int> > 
HMMStatesMerging::bottom_up_aglomerative(double th, string clustersfile) {
  vector<set<int> > clusters;
  int t2, t1;
  
  threshold=th;

  //1) Calculate the divergence between each pair of states 
  /////////////////////////////////////////////////////////////////////
  calculate_all_divergences();

  t1=Utils::get_usage_time();

  cerr<<"Performing bottom-up aglomerative clustering ...\n";
  cerr<<"Threshold = "<<threshold<<"\n";

  //2) Initial state
  /////////////////////////////////////////////////////////////////////
  if (clustersfile!="") {
    //Read previously defined clusters
    ////////////////////////////////////////////////////////////////////
    cerr<<"Reading previously defined clusters ...\n";
    ifstream ftagclus;
    ftagclus.open(clustersfile.c_str(), ios::in);
    if (!ftagclus.good()) {
      cerr<<"Error: Cannot open input '"<<clustersfile<<"'\n";
      exit(EXIT_FAILURE);
    }

    int nclusters, ntags, finetag;
    ftagclus>>nclusters;

    for(int i=0; i<nclusters; i++) {
      ftagclus>>ntags;
      set<int> auxcluster;
      while(ntags>0) {
	ftagclus>>finetag;
	auxcluster.insert(finetag);
	ntags--;
      }
      if(auxcluster.size()>0)
	clusters.push_back(auxcluster);
    }
    ftagclus.close();

    cerr<<"Initial status: "<<clusters.size()<<" clusters\n";
  } else { 
    //At the begining there are one cluster for each different state
    /////////////////////////////////////////////////////////////////
    cerr<<"Initializing, one cluster for each different state ...\n";
    set<int> auxcluster;
    for(int i=0; i<tagger_data.getN(); i++) {
      auxcluster.clear();
      auxcluster.insert(i);
      clusters.push_back(auxcluster);
    }
  }

  //3) Perform the clustering
  //////////////////////////////////////////////////////////////////////
  cerr<<"Performing clustering ... \n";
  while(true) {
    //Find two clusters to merge, the two with the lowest distance
    double min_distance=BIG_NUMBER;
    double d;
    vector<set<int> >::iterator cl1, cl2; //Clusters to merge
    vector<set<int> >::iterator iti, itj;

    for(iti=clusters.begin(); iti!=clusters.end(); iti++) {
      itj=iti;
      for(itj++; itj!=clusters.end(); itj++) {
        d=clusters_distance(*iti, *itj);
	if (d<min_distance) {
	  min_distance=d;
	  cl1=iti;
	  cl2=itj;
	}
      }
    }

    if (min_distance>threshold) { 
      //There isn't a couple of clusters that can be merged
      t2=Utils::get_usage_time();

      cerr<<"Total time needed for the clustering: "<<t2-t1<<" seconds.\n";
      return clusters;
    }

    //Merge clusters cl1 and cl2;
    set<int>:: iterator it;
    for(it=(*cl2).begin(); it!=(*cl2).end(); it++)
      (*cl1).insert(*it);
    clusters.erase(cl2);

  }

  cerr<<"This would never be printed\n"; 
  return clusters;
}

vector<set<int> > 
HMMStatesMerging::calculate_merged_ambiguity_classes(vector<set<int> > merged_states,
                                                     Collection& new_output) {
  vector<set<int> > merged_amb_classes;

  map<int, int> fine2coarse; //For a given fine tag, return the
			     //coarsed one

  map<int, int> fineamb2coarseamb; //For a given fine ambiguity class,
				   //returns the coarsed one

  for(size_t i=0; i<merged_states.size(); i++) {
    set<int>::iterator it;
    for(it=merged_states[i].begin(); it!=merged_states[i].end(); it++)
      fine2coarse[*it]=i;
  }

  for(int k=0; k<tagger_data.getM(); k++) {
    set<int> fineamb=tagger_data.getOutput()[k];
    set<int>::iterator it;
    set<int> coarseamb;
    for(it=fineamb.begin(); it!=fineamb.end(); it++) {
      coarseamb.insert(fine2coarse[*it]);
    }
    int index_amb=new_output[coarseamb];
    fineamb2coarseamb[k]=index_amb;
  }

  set<int> empty_set;
  merged_amb_classes.resize(new_output.size(), empty_set);

  for(int k=0; k<tagger_data.getM(); k++) {
    merged_amb_classes[fineamb2coarseamb[k]].insert(k);
  }

  return merged_amb_classes;
}
