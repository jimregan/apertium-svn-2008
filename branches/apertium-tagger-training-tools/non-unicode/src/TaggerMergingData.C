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

/** Class TaggerMergingData (source file)
 */

#include "TaggerMergingData.H"
#include "SmoothUtils.H"

#include <lttoolbox/Compression.H>
#include <lttoolbox/EndianUtil.H>

TaggerMergingData::TaggerMergingData():TaggerData() {
  coarse_a=NULL;
  coarse_b=NULL;

}

TaggerMergingData::~TaggerMergingData() {

  if(coarse_a!=NULL) {
    for(int i=0; i<COARSE_N; i++)
      delete [] coarse_a[i];
    delete [] coarse_a;
  }

  if(coarse_b!=NULL) {
    for(int i=0; i<COARSE_N; i++)
      delete [] coarse_b[i];
    delete [] coarse_b;
  }
}

Collection& 
TaggerMergingData::getOutput() {
  return  coarse_output;
}

map<int, int>& 
TaggerMergingData::getFine2Coarse() {
  return fine2coarse;
}

int 
TaggerMergingData::getEosTag() {
  return fine2coarse[TaggerData::getTagIndex()["TAG_SENT"]];
}

int 
TaggerMergingData::getFineEofTag() {
  return TaggerData::getTagIndex()["TAG_kEOF"];
}

int 
TaggerMergingData::getUndefTag() {
  return fine2coarse[TaggerData::getTagIndex()["TAG_kUNDEF"]];
}

int 
TaggerMergingData::getFineUndefTag() {
  return TaggerData::getTagIndex()["TAG_kUNDEF"];
}

double **
TaggerMergingData::getA() {
  return coarse_a;
}

double **
TaggerMergingData::getB() {
  return coarse_b;
}

int
TaggerMergingData::getN() {
  return COARSE_N;
}

int
TaggerMergingData::getM() {
  return COARSE_M;
}

void
TaggerMergingData::read_counts(istream& fcounts, 
			       map<int, map<int, double> >& tags_pair,
			       map<int, map<int, double> >& emis,
			       map<int, double>& tags_count,
			       map<int, double>& ambclass_count,
			       map<int, double>& tags_count_for_emis) {

  cerr<<"Reading counts ... "<<flush;

  for (int i=0; i<TaggerData::getN(); i++) {
    for(int j=0; j<TaggerData::getN(); j++) {
      tags_pair[i][j]=EndianUtil<double>::read(fcounts);
    }
  }

  for(int i=0; i<TaggerData::getN(); i++) {
    for(int k=0; k<TaggerData::getM(); k++) {
      if (TaggerData::getOutput()[k].find(i)!=TaggerData::getOutput()[k].end()) {
	int ii=(int)Compression::multibyte_read(fcounts);
	int kk=(int)Compression::multibyte_read(fcounts);
	double d=EndianUtil<double>::read(fcounts);
	if ((ii!=i) || (kk!=k)) {
	  cerr<<"\nError when reading counts, indexes do not agree\n";
	  exit(EXIT_FAILURE);
	}
	emis[i][k]=d;
      }
    }
  }

  for (int i=0; i<TaggerData::getN(); i++) {
    tags_count[i]=EndianUtil<double>::read(fcounts);
  }

  for (int k=0; k<TaggerData::getM(); k++) {
    ambclass_count[k]=EndianUtil<double>::read(fcounts);
  }

  for (int i=0; i<TaggerData::getN(); i++) {
    tags_count_for_emis[i]=EndianUtil<double>::read(fcounts);
  }

  cerr<<"done.\n";
}

void 
TaggerMergingData::calculate_parameters_merging(istream& fcounts, int corpus_length) {
  map<int, map<int, double> > tags_pair;
  map<int, map<int, double> > emis;
  map<int, double> tags_count;
  map<int, double> ambclass_count;
  map<int, double> tags_count_for_emis;

  map<int, map<int, double> > coarse_tags_pair; 
  map<int, map<int, double> > coarse_emis; 
  map<int, double> coarse_tags_count; 
  map<int, double> coarse_ambclass_count;
  map<int, double> coarse_tags_count_for_emis; 

  set<int>::iterator iti, itj, itk, itcoarse;

  read_counts(fcounts, tags_pair, emis, tags_count, ambclass_count, tags_count_for_emis);

  cerr<<"Calculating new parameters ... "<<flush;

  //Calculate coarse tag counts from fine-grained tag counts
  for(int i=0; i<COARSE_N; i++) {
    for(int j=0; j<COARSE_N; j++) {
      coarse_tags_pair[i][j]=0.0;

      for(iti=coarse2fine[i].begin(); iti!=coarse2fine[i].end(); iti++) {
	for(itj=coarse2fine[j].begin(); itj!=coarse2fine[j].end(); itj++) {
	  coarse_tags_pair[i][j]+=tags_pair[*iti][*itj];
	}
      }
    }
  }

  //Calculate coarse amb. class emission counts
  for(int k=0; k<COARSE_M; k++) {
    for(itcoarse=coarse_output[k].begin(); itcoarse!=coarse_output[k].end(); itcoarse++) {
      coarse_emis[*itcoarse][k]=0.0;
      for(iti=coarse2fine[*itcoarse].begin(); iti!=coarse2fine[*itcoarse].end(); iti++) {
	for(itk=coarseamb2fineamb[k].begin(); itk!=coarseamb2fineamb[k].end(); itk++) {
	  if (TaggerData::getOutput()[*itk].find(*iti)!=TaggerData::getOutput()[*itk].end()) {
	    coarse_emis[*itcoarse][k]+=emis[*iti][*itk];
	  }
	}
      }
    }
  }

  for(int i=0; i<COARSE_N; i++) {
    coarse_tags_count[i]=0.0;
    coarse_tags_count_for_emis[i]=0.0;
    for(iti=coarse2fine[i].begin(); iti!=coarse2fine[i].end(); iti++) {
      coarse_tags_count[i]+=tags_count[*iti];
      coarse_tags_count_for_emis[i]+=tags_count_for_emis[*iti];
    }
  }

  for(int k=0; k<COARSE_M; k++) {
    coarse_ambclass_count[k]=0.0;
    //cerr<<"k="<<k<<" ";
    for(itk=coarseamb2fineamb[k].begin(); itk!=coarseamb2fineamb[k].end(); itk++) {
      //cerr<<*itk<<"("<<ambclass_count[*itk]<<") ";
      coarse_ambclass_count[k]+=ambclass_count[*itk];
    }
    //cerr<<coarse_ambclass_count[k]<<"\n";
  }

  SmoothUtils::calculate_smoothed_parameters(*this, coarse_tags_count, coarse_tags_pair, 
					     coarse_ambclass_count, coarse_emis, 
					     coarse_tags_count_for_emis, corpus_length);
  cerr<<"done.\n";
}

void 
TaggerMergingData::read_tagsclusters(istream& ftagsclus) {

  int ntags;
  set<int> empty_set;

  ftagsclus>>COARSE_N;
  coarse2fine.resize(COARSE_N, empty_set);
  cerr<<"Reading fine tags merging structure ...\n";
  int finetag;
  for(int i=0; i<COARSE_N; i++) {
    ftagsclus>>ntags;
    while(ntags>0) {
      ftagsclus>>finetag;
      fine2coarse[finetag]=i;
      coarse2fine[i].insert(finetag);
      ntags--;
    }
  }

  ftagsclus>>COARSE_M;
  cerr<<"Reading ambiguity classes merging structure ...\n";
  int nambs;
  int fineamb;
  coarseamb2fineamb.resize(COARSE_M, empty_set);
  for(int i=0; i<COARSE_M; i++) {
    ftagsclus>>nambs;
    while(nambs>0) {
      ftagsclus>>fineamb;
      fineamb2coarseamb[fineamb]=i;
      coarseamb2fineamb[i].insert(fineamb);
      nambs--;
    }
  }

  cerr<<"Reading ambiguity classes of coarse tags ...\n";
  set<int> coarse_amb_class;
  int coarsetag;
  for(int i=0; i<COARSE_M; i++) {
    ftagsclus>>ntags;
    coarse_amb_class.clear();
    while(ntags>0) {
      ftagsclus>>coarsetag;
      coarse_amb_class.insert(coarsetag);
      ntags--;
    }
    if(coarse_amb_class.size()>0)
      coarse_output.add(coarse_amb_class);
  }

  cerr<<TaggerData::getN()<<" fine tags, "<<getN()<<" coarse tags.\n";
  cerr<<TaggerData::getM()<<" amb. classes of fine tags, "<<getM()<<" amb. classes of coarse tags.\n";

  initialize_parameters();

  /*  
      vector<set<int> >::iterator itv;
      set<int>::iterator its;
      cout<<COARSE_N<<"\n";
      for(int i=0; i<COARSE_N; i++) {
      cout<<coarse2fine[i].size()<<" ";
      for(its=coarse2fine[i].begin(); its!=coarse2fine[i].end(); its++)
      cout<<*its<<" ";
      cout<<"\n";
      }

      cout<<COARSE_M<<"\n";
      for(int k=0; k<COARSE_M; k++) {
      cout<<coarseamb2fineamb[k].size()<<" ";
      for(its=coarseamb2fineamb[k].begin(); its!=coarseamb2fineamb[k].end(); its++)
      cout<<*its<<" ";
      cout<<"\n";
      }

      for(int k=0; k<coarse_output.size(); k++) {
      set<int> amb_class=coarse_output[k];
      cout<<amb_class.size()<<" ";
      for(its=amb_class.begin(); its!=amb_class.end(); its++)
      cout<<*its<<" ";
      cout<<"\n";
      }
  */
}

void 
TaggerMergingData::initialize_parameters() {

  if ((COARSE_N!=0) && (COARSE_M!=0)) {
    // nxn matrix
    coarse_a = new double * [COARSE_N];
    for(int i=0; i<COARSE_N; i++) {
      coarse_a[i] = new double[COARSE_N];
      for(int j=0; j<COARSE_N; j++)
	coarse_a[i][j] = 0.0;
    }

    // nxm matrix
    coarse_b = new double * [COARSE_N];
    for(int i=0; i<COARSE_N; i++) {
      coarse_b[i] = new double[COARSE_M];
      for(int k=0; k<COARSE_M; k++)
	coarse_b[i][k] = 0.0;
    }
  } 
}
