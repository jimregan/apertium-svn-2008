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
/**
 * Utility functions. (source file)
 *
 *  @author   Felipe Sánchez-Martínez - fsanchez@dlsi.ua.es
 */

#include <iostream>


#include "SmoothUtils.H"
#include <cmath>

#include <apertium/utf_converter.h>

double 
SmoothUtils::lambda(double count) {
  double sqrtval=sqrt(count);
  return sqrtval/(1+sqrtval);
}

map<int, double> 
SmoothUtils::prob_amb_class_from_tag(TaggerData& tagger_data, 
				     int i, 
				     map<int, double> &prob_ambclass) {
  double sum_ambclass=0.0;
  map<int, double> prob_ambclass_tag;

  for(int k=0; k<tagger_data.getM(); k++) {
    if (tagger_data.getOutput()[k].find(i)!=tagger_data.getOutput()[k].end()) { 
      sum_ambclass+=prob_ambclass[k];
    }
  }

  for(int k=0; k<tagger_data.getM(); k++) {
    if (tagger_data.getOutput()[k].find(i)!=tagger_data.getOutput()[k].end()) { 
      prob_ambclass_tag[k]=prob_ambclass[k]/sum_ambclass;
    }
  }

  return prob_ambclass_tag;
}

void
SmoothUtils::calculate_smoothed_parameters(TaggerData& tagger_data,
					   map<int, double> &tags_count, 
					   map<int, map<int, double> > &tags_pairs, 
					   map<int, double> &ambclass_count, 
					   map<int, map<int, double> > &emis, 
					   map<int, double> &tags_count_for_emis,
					   double corpus_length) {
  map<int, double> prob_tag;
  double mu=lambda(corpus_length);
  double sum_tag=0.0;

  //cerr<<"Checking that counts are correct\n-----------------------------\n";
  for(int i=0; i<tagger_data.getN(); i++) {
    double sum=0.0;
    for(int j=0; j<tagger_data.getN(); j++) {
      sum+=tags_pairs[i][j];
    }
    if (fabs(sum-tags_count[i])>0.0001) {
      cerr<<"Error: sum does not agree for tag "<<UtfConverter::toUtf8(tagger_data.getArrayTags()[i])
          <<": "<<sum<<" -- "<<tags_count[i]<<"\n";
    }
  }

  for(int k=0; k<tagger_data.getM(); k++) {
    double sum=0.0;
    for(int i=0; i<tagger_data.getN(); i++) {
      if (tagger_data.getOutput()[k].find(i)!=tagger_data.getOutput()[k].end()) {
	sum+=emis[i][k];
      }
    }
    if (fabs(sum-ambclass_count[k])>0.0001) {
      cerr<<"Error: sum does not agree for amb. class "<<k<<": "<<sum<<" -- "<<ambclass_count[k]<<"\n";
    }
  }

  //cerr<<"Calculating smoothed parameters\n----------------------------\n";

  for(int i=0; i<tagger_data.getN(); i++) {
    sum_tag+=tags_count[i];
  }
  for(int i=0; i<tagger_data.getN(); i++) {
    prob_tag[i]=(mu*(tags_count[i]/sum_tag)) + ((1.0-mu)*(1.0/((double)tagger_data.getN())));
  }

  //cerr<<"\n";
  //for(int i=0; i<tagger_data.getN(); i++)
  //  cerr<<"prob_tag["<<i<<"]="<<prob_tag[i]<<"\n";
  //cerr<<"\n";


  for (int i=0; i<tagger_data.getN(); i++) {
    for (int j=0; j<tagger_data.getN(); j++) {
      double lambda_value=lambda(tags_count[i]);
      double frec_ij;
      if (tags_count[i]>0)
	frec_ij=tags_pairs[i][j]/tags_count[i];
      else
	frec_ij=0.0;

      tagger_data.getA()[i][j]=(lambda_value*frec_ij) +	((1.0-lambda_value)*prob_tag[j]);

      //cerr<<"\nCalculating A["<<i<<"]["<<j<<"]\n-------------------------\n";
      //cerr<<"lambda(tags_count["<<i<<"])=lambda("<<tags_count[i]<<")="<<lambda_value<<"\n";
      //cerr<<"tags_pairs["<<i<<"]["<<j<<"]="<<tags_pairs[i][j]<<"\n";
      //cerr<<"tags_count["<<i<<"]="<<tags_count[i]<<"\n";
      //cerr<<"prob_tag["<<j<<"]="<<prob_tag[j]<<"\n";
      //cerr<<"A["<<i<<"]["<<j<<"]="<<tagger_data.getA()[i][j]<<"\n";
    }
  }


  map<int, double> prob_ambclass;
  double sum_ambclass=0.0;
  for(int k=0; k<tagger_data.getM(); k++) {
    sum_ambclass+=ambclass_count[k];
  }
  for(int k=0; k<tagger_data.getM(); k++) {
    double frec_k;
    if (sum_ambclass>0)
      frec_k=ambclass_count[k]/sum_ambclass;
    else
      frec_k=0.0;
    prob_ambclass[k]=(mu*frec_k) + ((1.0-mu)*(1.0/((double)tagger_data.getM())));
  }

  for(int j=0; j<tagger_data.getN(); j++) {
    map<int, double> prob_ambclass_tag;
    prob_ambclass_tag=prob_amb_class_from_tag(tagger_data, j, prob_ambclass);

    //cerr<<"\nCalculation Bs for tag "<<j<<"\n----------------------------\n";
    //for(int k=0; k<tagger_data.getM(); k++) {
    //  if(tagger_data.getOutput()[k].find(j)!=tagger_data.getOutput()[k].end()) 
    //	cerr<<"prob_ambclass_tag["<<k<<"]="<<prob_ambclass_tag[k]<<"\n";
    //}

    for(int k=0; k<tagger_data.getM(); k++) {
      if(tagger_data.getOutput()[k].find(j)!=tagger_data.getOutput()[k].end()) {
	double lambda_value=lambda(tags_count_for_emis[j]);
	double frec_jk;
	if(tags_count_for_emis[j]>0)
	  frec_jk=emis[j][k]/tags_count_for_emis[j];
	else
	  frec_jk=0.0;

	tagger_data.getB()[j][k]=(lambda_value*frec_jk) + ((1.0-lambda_value)*prob_ambclass_tag[k]);

	//cerr<<"\nCalculating B["<<j<<"]["<<k<<"]\n-------------------------\n";
	//cerr<<"lambda(tags_count_for_emis["<<j<<"])=lambda("<<tags_count_for_emis[j]<<")="<<lambda_value<<"\n";
	//cerr<<"tags_count_for_emis["<<j<<"]="<<tags_count_for_emis[j]<<"\n";
	//cerr<<"emis["<<j<<"]["<<k<<"]="<<emis[j][k]<<"\n";
	//cerr<<"prob_ambclass["<<k<<"]="<<prob_ambclass[k]<<"\n";
	//cerr<<"prob_ambclass_tag["<<k<<"]="<<prob_ambclass_tag[k]<<"\n";
	//cerr<<"B["<<j<<"]["<<k<<"]="<<tagger_data.getB()[j][k]<<"\n";
      }
    }
  }
}
