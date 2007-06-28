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
 * Implementation of the method that trains a HMM-based part-of-speech
 * tagger using information from the target language (TL) by means of
 * the apertium MT system. (source file)
 *
 * For a deeper description on how the method works read the paper:
 *
 * Exploring the use of target-language information to train the
 * part-of-speech tagger of machine translation systems.  By Felipe
 * Sánchez-Martínez, Juan Antonio Pérez-Ortiz and Mikel L. Forcada.
 * In Lecture Notes in Computer Science 3230 (Advances in Natural
 * Language Processing, Proceedings of EsTAL - España for Natural
 * Language Processing), p. 137-148, October 20-22, 2004, Alicante,
 * Spain.  
 * © Springer-Verlag Berling Heidelberg 2004
 * http://www.dlsi.ua.es/~fsanchez/pub/pdf/sanchez04b.pdf
 *
 *  @author   Felipe Sánchez-Martínez - fsanchez@dlsi.ua.es
 */

#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <cmath>
#include <cfloat>

#include <apertium/Collection.H>
#include <apertium/ConstantManager.H>
#include <apertium/TaggerData.H>
#include <apertium/TaggerUtils.H>
#include <apertium/TaggerWord.H>

#include <apertium/MorphoStream.H>
#include <apertium/HMM.H>

#include <apertium/TSXReader.H>

#include "HMM_TL_driven_trainer.H"
#include "Segment.H"
#include "Translations.H"
#include "Utils.H"
#include "PathsPruner.H"

HMM_TL_driven_trainer::HMM_TL_driven_trainer(string tsxfile, TransferRules* tr) {
  TSXReader treader;
  treader.read(tsxfile);
  tagger_data=treader.getTaggerData();

  hmm = new HMM(&tagger_data);
  hmm->set_debug(true);
  hmm->set_eos((tagger_data.getTagIndex())["TAG_SENT"]);
  TaggerWord::setArrayTags(tagger_data.getArrayTags());

  use_tags_rules=true;

  transfer_rules=tr;  

  Segment::set_tag_index(tagger_data.getTagIndex());
}

HMM_TL_driven_trainer::~HMM_TL_driven_trainer() {
  delete hmm;
}

void 
HMM_TL_driven_trainer::read_dictionary(FILE *fdic) {
  cerr<<"Calculating ambiguity classes ..."<<flush;
  hmm->read_dictionary(fdic);
}

void 
HMM_TL_driven_trainer::read_parameters(FILE *in) {
  cerr<<"Reading parameters ... ";
  tagger_data.read(in);
  cerr<<" done.\n";
}

void 
HMM_TL_driven_trainer::write_parameters(FILE *out) {
  cerr<<"Writing parameters ... ";
  tagger_data.write(out);
  cerr<<" done.\n";
}

void
HMM_TL_driven_trainer::set_use_tags_rules(bool b) {
  use_tags_rules=b;
}

void 
HMM_TL_driven_trainer::train(FILE *is, int corpus_length, int save_after_nwords, string filename, ofstream& fpaths, ifstream& ftrans, ifstream& flike){
  int i, j, k;

  map<int, map<int, double> > tags_pair; //NxN
  map<int, map<int, double> > emis; //NxM

  map<int, double> tags_count; //N
  map<int, double> ambclass_count; //M

  string cadena;
  Segment* seg=NULL;
  vector<Translations*> translations;

  MorphoStream morpho_stream(is, true, &tagger_data);

  //Contadores y estadísticas 
  int cuenta_palabras=0;
  int cuenta_caminos=0;
  int cuenta_segmentos=0;

  int cuenta_traducciones[1];
  cuenta_traducciones[TL1]=0;

  int translations_ok[1];
  translations_ok[TL1]=0;

  map<int, long> caminos_long_segmentos; 
  map<int, long> long_segmentos; 
  map<int, long> trads_long_segmentos[1];

  int ambiguous_segments=0;

  //int segment_allpaths_forbidden=0;

  //Vector con las etiquetas de un camino en concreto
  vector<TTag> etqpart;
   
  //Ultima etiqueta del segmento de texto anterior al que esta
  //siendo tratado, sigma_I-1 (no homografa por la politica de corte)
  TTag last_etq_segmento_ant;      

  int next_save_probs;

  if (save_after_nwords>0)
    next_save_probs=save_after_nwords;
  else
    next_save_probs=2*corpus_length;

  cerr<<"Target-language driven HMM-based part-of-speech training method.......\n"
      <<"   Training corpus will be processed for "<<corpus_length<<" words\n"
      <<"   HMM parameters will be calculated each time "<<save_after_nwords
      <<" words are processed\n"
      <<"   Calculated parameter will be saved in files with the name '"<<filename<<".N.prob'\n"
      <<"   Are fobidden and enforce rules going to be used? "<<use_tags_rules<<"\n"
      <<"   Translation script is: '"<<Utils::translation_script<<"'\n"
      <<"   Likelihood estimation script is: '"<<Utils::likelihood_script<<"'\n"
      <<"Ready for training...... go!\n\n";

  //Counters initialization.
  for(i=0; i<tagger_data.getN(); i++) {
    tags_count[i]=0;
    for(j=0; j<tagger_data.getN(); j++)
      tags_pair[i][j]=0;
  }
  for(k=0; k<tagger_data.getM(); k++) {
    ambclass_count[k]=0;
    for(i=0; i<tagger_data.getN(); i++) {
      if (tagger_data.getOutput()[k].find(i)!=tagger_data.getOutput()[k].end())
	emis[i][k]=0;
    }  
  }

  //Inicialización de los bigramas permitidos y prohibidos
  if (use_tags_rules)
    init_allowed_bigrams();

  last_etq_segmento_ant=tagger_data.getTagIndex()["TAG_SENT"];

  //Comienza el algoritmo de entrenamiento   
  while(true) {
    //cerr<<"Press enter\n";
    //getchar();

    if (seg!=NULL)
      delete seg;
    for(i=0; i<translations.size(); i++)
      delete translations[i];
    translations.clear();

    //Tomamos el segmento siguiente
    seg=Segment::new_segment(morpho_stream, transfer_rules);
    if(seg->get_number_paths()==0)
      break;
          
    //One translations
    translations.push_back(new Translations(seg));

    //Para tenerlo en cuenta a la hora de actualizar las estadísticas
    bool hay_caminos_prohibidos=false;

    //Si el segmento tiene demasiados caminos nos lo saltamos
    if (seg->get_number_paths()>MAX_PATH_PER_SEGMENT) { 
      cerr<<"Warning: Segment has "<<seg->get_number_paths()<<" disambiguation paths. Skipping\n";      
      cerr<<"SEGMENT: ";
      for(int npalabras=0; npalabras<(seg->vwords.size()); npalabras++)
        cerr<<seg->vwords[npalabras].get_superficial_form()<<" ";
      cerr<<"\n";
      
      //Nos quedamos con la última etiqueta del segmento que nos saltamos
      cadena=seg->get_path(etqpart, 0);
      last_etq_segmento_ant=etqpart.back();
      continue;
    }
    
    Utils::print_debug("\nA NEW SEGMENT BEGINS (");
    Utils::print_debug(cuenta_palabras);
    Utils::print_debug(")-----------------------------\n");
    Utils::print_debug("LAST TAG PREVIOUS SEGMENT: ");
    Utils::print_debug(last_etq_segmento_ant);
    Utils::print_debug(" - NPATHS: ");
    Utils::print_debug(seg->get_number_paths());
    Utils::print_debug("\n--------------------------------------------------------\n");
    Utils::print_debug("SEGMENT: \n");
    for(int npalabras=0; npalabras<(seg->vwords.size()); npalabras++){
      Utils::print_debug(seg->vwords[npalabras].get_superficial_form()+" ");
      Utils::print_debug(seg->vwords[npalabras].get_string_tags()+"\n");
    }
    Utils::print_debug("\n");
    

    map<string, double> translations_likelihoods;
    //Calculamos sus traducciones
    for(int ncamino=0; ncamino<seg->get_number_paths(); ncamino++) {
      cadena=seg->get_path(etqpart, ncamino);      
      if (is_feasible_path(last_etq_segmento_ant, etqpart)) {
	if (seg->get_number_paths()==1) {

	  if(fpaths.is_open()) {
	    continue;
	  }

	  Utils::print_debug("PATH ");
	  Utils::print_debug(ncamino);
	  Utils::print_debug(" TO TRANSLATE: "+cadena+"\n");
	  Utils::print_debug("TRANSLATION: ");
	  Utils::print_debug(TRANSLATION_UNIQUE_PATH);
	  Utils::print_debug("\n");
	  translations[TL1]->set_path_translation(TRANSLATION_UNIQUE_PATH, ncamino);
	} else {

	  if(fpaths.is_open()) {
	    fpaths<<cadena<<"^.<sent>$[\n]"<<flush;
	    continue;
	  }

	  Utils::print_debug("PATH ");
	  Utils::print_debug(ncamino);
	  Utils::print_debug(" TO TRANSLATE: "+cadena+"\n");
	  string tradcadena;

	  if(ftrans.is_open()) {
	    getline(ftrans, tradcadena);
	    if (!(translations[TL1]->are_translations_ok()))
	      tradcadena=TRANSLATION_NOT_USED;
	  } else {
	    if(translations[TL1]->are_translations_ok())
	      tradcadena=Utils::translate(Utils::translation_script, cadena);
	    else
	      tradcadena=TRANSLATION_NOT_USED;
	  }

	  if(flike.is_open()) {
	    string strlikelihood;
	    getline(flike, strlikelihood);	  
	    translations_likelihoods[tradcadena]=atof(strlikelihood.c_str());
	  }

	  translations[TL1]->set_path_translation(tradcadena, ncamino);
	  Utils::print_debug("TRANSLATION: "+tradcadena+"\n");
	  if (trim(tradcadena).length()==0) {
	    cerr<<"Error: There is a path with a null translation: "<<cadena<<"\n";
	    cerr<<"SEGMENT: ";
	    for(int npalabras=0; npalabras<(seg->vwords.size()); npalabras++)
	      cerr<<seg->vwords[npalabras].get_superficial_form()<<" ";
	    cerr<<"\n";
	  }
	}
      } 
      else {
	Utils::print_debug("PATH ");
	Utils::print_debug(ncamino);
	Utils::print_debug(" FORBIDDEN: ");
	Utils::print_debug(cadena);
	Utils::print_debug("\n");

	hay_caminos_prohibidos=true;
      }
    }

    if (fpaths.is_open()) {
      //Batch mode
      cadena=seg->get_path(etqpart, 0);
      last_etq_segmento_ant=etqpart.back();
      continue;
    }

    //Comprobamos que el segmento tenga traducciones. Por errores en el
    //traductor puede suceder que no, porque se haya comido una palabra
    //y el segmento conste de solo una palabra por ejemplo.
    if(translations[TL1]->get_number_translations()<=0) {
      cerr<<"Warning: This segment has no translations into TL 1. Skipping\n";
      cerr<<"SEGMENT: ";
      for(int npalabras=0; npalabras<(seg->vwords.size()); npalabras++)
	cerr<<seg->vwords[npalabras].get_superficial_form()<<" ";
      cerr<<"\n"; 
      //Nos quedamos con la última etiqueta del segmento que nos saltamos
      cadena=seg->get_path(etqpart, 0);
      last_etq_segmento_ant=etqpart.back();
      continue;
    }

    //We test if there are transaltions, this segment could have all
    //the paths forbidden
    if((translations[TL1]->get_number_translations()==0)&&(hay_caminos_prohibidos)) {
      cerr<<"Warning: This segment has all its paths forbidden. Skipping\n";
      cerr<<"SEGMENT: ";
      for(int npalabras=0; npalabras<(seg->vwords.size()); npalabras++)
	cerr<<seg->vwords[npalabras].get_superficial_form()<<" ";
      cerr<<"\n";
      //Nos quedamos con la última etiqueta del segmento que nos saltamos
      cadena=seg->get_path(etqpart, 0);
      last_etq_segmento_ant=etqpart.back();
      continue;
    }

    if (translations[TL1]->are_translations_ok())
      translations_ok[TL1]++;

    Utils::print_debug("Are translations ok? ");
    Utils::print_debug(translations[TL1]->are_translations_ok());
    Utils::print_debug("\n");

    if (!translations[TL1]->are_translations_ok()) {
      cerr<<"Warning: This segment has no OK translations. Skipping\n";
      cerr<<"SEGMENT: ";
      for(int npalabras=0; npalabras<(seg->vwords.size()); npalabras++)
	cerr<<seg->vwords[npalabras].get_superficial_form()<<" ";
      cerr<<"\n";
      //We store the last of the segment being skipped
      cadena=seg->get_path(etqpart, 0);
      last_etq_segmento_ant=etqpart.back();
      continue;
    }

    if (flike.is_open()) {
      translations[TL1]->set_previoulsy_evaluated_translations_likelihood(translations_likelihoods);
    } else {
      translations[TL1]->evaluate_translations_likelihood(Utils::likelihood_script);
    }
    translations[TL1]->calculate_probability_each_path();

    update_counts(seg, translations, tags_pair, emis, last_etq_segmento_ant, tags_count, ambclass_count);

    //Actualizamos contadores
    cuenta_segmentos++;
    cuenta_palabras+=seg->vwords.size();
    cuenta_caminos+=seg->get_number_paths();
    if (translations[TL1]->are_translations_ok())
      cuenta_traducciones[TL1]+=translations[TL1]->get_number_translations(); 

    caminos_long_segmentos[seg->vwords.size()]+=seg->get_number_paths();
    long_segmentos[seg->vwords.size()]++;

    if (translations[TL1]->are_translations_ok())
      trads_long_segmentos[TL1][seg->vwords.size()]+=translations[TL1]->get_number_translations();

    if (seg->get_number_paths()>1)
      ambiguous_segments++;

    //Nos quedamos con la última etiqueta del segmento
    cadena=seg->get_path(etqpart, 0);
    last_etq_segmento_ant=etqpart.back();

    //Vemos si tenemos que guardar los parámetros
    if (cuenta_palabras>=next_save_probs) {
      cout<<"Number of processed words: "<<cuenta_palabras<<"\n";
      cout<<"Number of segments treated: "<<cuenta_segmentos<<"\n";
      cout<<"Number of ambiguous segments: "<<ambiguous_segments<<"\n";
      cout<<"Total number of paths translated: "<<cuenta_caminos<<"\n";
      cout<<"Total number of translations: "<<cuenta_traducciones[TL1]<<"\n";
      cout<<"Number of segment correctly translated: "<<translations_ok[TL1]<<"\n";

      map<int, long>::iterator itmap;   
      cout<<"Average number of paths per segment length\n";       
      for(itmap=long_segmentos.begin(); itmap!=long_segmentos.end(); itmap++)
	cout<<(*itmap).first<<" "<<((double)caminos_long_segmentos[(*itmap).first])/((double)(*itmap).second)<<"\n";

      cout<<"Average number of different translations per segment length\n";
      for(itmap=long_segmentos.begin(); itmap!=long_segmentos.end(); itmap++)
	cout<<(*itmap).first<<" "<<((double)trads_long_segmentos[TL1][(*itmap).first])/((double)(*itmap).second)<<"\n";
      
      cout<<"Time (user+sys): "<<Utils::get_usage_time()<<"\n";
      cout<<"--------------------------------------------------------\n"<<flush;

      //calculate_parameters(tags_pair, emis);
      calculate_smoothed_parameters(tags_count, tags_pair, ambclass_count, emis, corpus_length);
                  
      char aux[15];
      sprintf(aux, "%d", next_save_probs);
      FILE *fprob;
	 	 
      string probfile = filename+"."+aux+".prob";  // File with all the tagger parameters
      fprob=fopen(probfile.c_str(), "w");	        
      tagger_data.write(fprob);
      fclose(fprob);

      next_save_probs+=save_after_nwords;
    }
	 
    if (cuenta_palabras>=corpus_length) {
      break;
    }	 
  }
    
  //calculate_parameters(tags_pair, emis);  
  calculate_smoothed_parameters(tags_count, tags_pair, ambclass_count, emis, corpus_length);

  if (seg!=NULL)
    delete seg;
  for(i=0; i<translations.size(); i++)
    delete translations[i];
  translations.clear();
}

void 
HMM_TL_driven_trainer::train_pruning(FILE *is, int corpus_length, int save_after_nwords, string filename, double mixing_c, ofstream& fpaths, ifstream& ftrans, ifstream& flike) {
  int i, j, k;

  map<int, map<int, double> > tags_pair; //NxN
  map<int, map<int, double> > emis; //NxM

  map<int, double> tags_count; //N
  map<int, double> ambclass_count; //M

  string cadena;

  Segment* seg=NULL;
  PathsPruner *pruner=NULL;

  vector<Translations*> translations;

  MorphoStream morpho_stream(is, true, &tagger_data);

  tagger_data_initial_model=tagger_data;

  PathsPruner::set_tagger_data(tagger_data);

  //Contadores y estadísticas 
  int cuenta_palabras=0;
  int cuenta_caminos=0;
  int cuenta_segmentos=0;

  int cuenta_traducciones[1];
  cuenta_traducciones[TL1]=0;

  int translations_ok[1];
  translations_ok[TL1]=0;

  map<int, long> caminos_long_segmentos; 
  map<int, long> long_segmentos; 
  map<int, long> trads_long_segmentos[1];

  map<int, pair<int, int> > pruning_stats; 
  // pruning_stats[seg. length].first = num. paths
  // pruning_stats[seg. length].second = num.paths treated
  map<int, pair<int, double> > pruning_probmass;
  //pruning_probmass[seg. length].first = num. seg. with the given length
  //pruning_probmass[seg. length].second = sum probability mass taken into account

  long nwords_translated=0;
  long nwords_to_translate_without_pruning=0;

  int ambiguous_segments=0;

  //int segment_allpaths_forbidden=0;

  //Vector con las etiquetas de un camino en concreto
  vector<TTag> etqpart;
   
  //Ultima etiqueta del segmento de texto anterior al que esta
  //siendo tratado, sigma_I-1 (no homografa por la politica de corte)
  TTag last_etq_segmento_ant;      

  int next_save_probs;
  if (save_after_nwords>0)
    next_save_probs=save_after_nwords;
  else
    next_save_probs=2*corpus_length;

  int next_update_pruning_model;
  if (PathsPruner::latency>0)
    next_update_pruning_model=PathsPruner::latency;
  else
    next_update_pruning_model=2*corpus_length;

  cerr<<"Target-language driven HMM-based part-of-speech training method.......\n"
      <<"   Training corpus will be processed for "<<corpus_length<<" words\n"
      <<"   HMM parameters will be calculated each time "<<save_after_nwords<<" words are processed\n"
      <<"   Calculated parameter will be saved in files with the name '"<<filename<<".N.prob'\n"
      <<"   Are fobidden and enforce rules going to be used? "<<use_tags_rules<<"\n"
      <<"   Translation script is: '"<<Utils::translation_script<<"'\n"
      <<"   Likelihood estimation script is: '"<<Utils::likelihood_script<<"'\n"
      <<"   Disambiguation path pruning techniques will be used\n"
      <<"      Path pruning mode: "<<PathsPruner::mode<<"\n"
      <<"      HMM parameters updating latency: "<<PathsPruner::latency<<" words\n"
      <<"      Mass probability: "<<PathsPruner::probmass<<"\n"
      <<"      Constant to be used when mixing parameters: "<<mixing_c<<"\n"
      <<"Ready for training...... go!\n\n";

  //Counters initialization. The initial count (0.5) will be applied when 
  //calculating the HMM parameters
  for(i=0; i<tagger_data.getN(); i++) {
    for(j=0; j<tagger_data.getN(); j++)
      tags_pair[i][j]=0;
  }
  for(k=0; k<tagger_data.getM(); k++) {
    for(i=0; i<tagger_data.getN(); i++) {
      if (tagger_data.getOutput()[k].find(i)!=tagger_data.getOutput()[k].end())
	emis[i][k]=0;
    }  
  }

  //Inicialización de los bigramas permitidos y prohibidos
  if (use_tags_rules)
    init_allowed_bigrams();

  last_etq_segmento_ant=tagger_data.getTagIndex()["TAG_SENT"];
  //Comienza el algoritmo de entrenamiento   
  while(true) {
    //cerr<<"Press enter\n";
    //getchar();

    if (seg!=NULL)
      delete seg;
    if (pruner!=NULL)
      delete pruner;

    for(i=0; i<translations.size(); i++)
      delete translations[i];
    translations.clear();

    //Tomamos el segmento siguiente
    seg=Segment::new_segment(morpho_stream, transfer_rules);
    if(seg->get_number_paths()==0)
      break;
      
    pruner=new PathsPruner(seg, last_etq_segmento_ant);

    //One translations
    translations.push_back(new Translations(seg));

    //Para tenerlo en cuenta a la hora de actualizar las estadísticas
    bool hay_caminos_prohibidos=false;

    //Si el segmento tiene demasiados caminos nos lo saltamos
    if (seg->get_number_paths()>MAX_PATH_PER_SEGMENT) { 
      cerr<<"Warning: Segment has "<<seg->get_number_paths()<<" disambiguation paths. Skipping\n";      
      cerr<<"SEGMENT: ";
      for(int npalabras=0; npalabras<(seg->vwords.size()); npalabras++)
        cerr<<seg->vwords[npalabras].get_superficial_form()<<" ";
      cerr<<"\n";
      
      //Nos quedamos con la última etiqueta del segmento que nos saltamos
      cadena=seg->get_path(etqpart, 0);
      last_etq_segmento_ant=etqpart.back();
      continue;
    }
    
    Utils::print_debug("\nA NEW SEGMENT BEGINS (");
    Utils::print_debug(cuenta_palabras);
    Utils::print_debug(")-----------------------------\n");
    Utils::print_debug("LAST TAG PREVIOUS SEGMENT: ");
    Utils::print_debug(last_etq_segmento_ant);
    Utils::print_debug(" - NPATHS: ");
    Utils::print_debug(seg->get_number_paths());
    Utils::print_debug("\n--------------------------------------------------------\n");
    Utils::print_debug("SEGMENT: \n");
    for(int npalabras=0; npalabras<(seg->vwords.size()); npalabras++){
      Utils::print_debug(seg->vwords[npalabras].get_superficial_form()+" ");
      Utils::print_debug(seg->vwords[npalabras].get_string_tags()+"\n");
    }
    Utils::print_debug("\n");
    
    //First we calculate the a priori likelihood in order to take into
    //account only the most promising paths
    pruner->compute_paths_ranking();

    //Now we take statistics from the pruner object
    pruning_stats[seg->vwords.size()].first+=seg->get_number_paths();
    pruning_stats[seg->vwords.size()].second+=pruner->get_number_considered_paths();
    pruning_probmass[seg->vwords.size()].first++;
    pruning_probmass[seg->vwords.size()].second+=pruner->get_probmass_considered();

    nwords_translated+=(seg->vwords.size())*(pruner->get_number_considered_paths());
    nwords_to_translate_without_pruning+=(seg->vwords.size())*(seg->get_number_paths());

    //Calculamos sus traducciones
    //for(int ncamino=0; ncamino<seg->get_number_paths(); ncamino++) {
    int ncamino;
    pruner->reset_paths_counter();
    ncamino=pruner->get_next_path();
    while (ncamino>=0) {
      cadena=seg->get_path(etqpart, ncamino);      
      if (is_feasible_path(last_etq_segmento_ant, etqpart)) {
	if (seg->get_number_paths()==1) {
	  Utils::print_debug("PATH ");
	  Utils::print_debug(ncamino);
	  Utils::print_debug(" TO TRANSLATE: "+cadena+"\n");
	  Utils::print_debug("TRANSLATION: ");
	  Utils::print_debug(TRANSLATION_UNIQUE_PATH);
	  Utils::print_debug("\n");
	  translations[TL1]->set_path_translation(TRANSLATION_UNIQUE_PATH, ncamino);
	} else {
	  Utils::print_debug("PATH ");
	  Utils::print_debug(ncamino);
	  Utils::print_debug(" TO TRANSLATE: "+cadena+"\n");
	  string tradcadena;

	  if(translations[TL1]->are_translations_ok())
	    tradcadena=Utils::translate(Utils::translation_script, cadena);
	  else
	    tradcadena=TRANSLATION_NOT_USED;

	  translations[TL1]->set_path_translation(tradcadena, ncamino);
	  Utils::print_debug("TRANSLATION: "+tradcadena+"\n");
	  if (trim(tradcadena).length()==0) {
	    cerr<<"Error: There is a path with a null translation: "<<cadena<<"\n";
	    cerr<<"SEGMENT: ";
	    for(int npalabras=0; npalabras<(seg->vwords.size()); npalabras++)
	      cerr<<seg->vwords[npalabras].get_superficial_form()<<" ";
	    cerr<<"\n";
	  }
	}
      } else {
	Utils::print_debug("PATH ");
	Utils::print_debug(ncamino);
	Utils::print_debug(" FORBIDDEN: ");
	Utils::print_debug(cadena);
	Utils::print_debug("\n");

	hay_caminos_prohibidos=true;
      }
      ncamino=pruner->get_next_path();
    }

    //Comprobamos que el segmento tenga traducciones. Por errores en el
    //traductor puede suceder que no, porque se haya comido una palabra
    //y el segmento conste de solo una palabra por ejemplo.
    if(translations[TL1]->get_number_translations()<=0) {
      cerr<<"Warning: This segment has no translations into TL 1. Skipping\n";
      cerr<<"SEGMENT: ";
      for(int npalabras=0; npalabras<(seg->vwords.size()); npalabras++)
	cerr<<seg->vwords[npalabras].get_superficial_form()<<" ";
      cerr<<"\n"; 
      //Nos quedamos con la última etiqueta del segmento que nos saltamos
      cadena=seg->get_path(etqpart, 0);
      last_etq_segmento_ant=etqpart.back();
      continue;
    }

    //We test if there are transaltions, this segment could have all
    //the paths forbidden
    if((translations[TL1]->get_number_translations()==0) &&
       (hay_caminos_prohibidos)) {

      cerr<<"Warning: This segment has all its paths forbidden. Skipping\n";
      cerr<<"SEGMENT: ";
      for(int npalabras=0; npalabras<(seg->vwords.size()); npalabras++)
	cerr<<seg->vwords[npalabras].get_superficial_form()<<" ";
      cerr<<"\n";
      //Nos quedamos con la última etiqueta del segmento que nos saltamos
      cadena=seg->get_path(etqpart, 0);
      last_etq_segmento_ant=etqpart.back();
      continue;
    }

    if (translations[TL1]->are_translations_ok())
      translations_ok[TL1]++;

    Utils::print_debug("Are translations ok? ");
    Utils::print_debug(translations[TL1]->are_translations_ok());
    Utils::print_debug("\n");

    if (!translations[TL1]->are_translations_ok()) {
      cerr<<"Warning: This segment has no OK translations. Skipping\n";
      cerr<<"SEGMENT: ";
      for(int npalabras=0; npalabras<(seg->vwords.size()); npalabras++)
	cerr<<seg->vwords[npalabras].get_superficial_form()<<" ";
      cerr<<"\n";
      //We store the last of the segment being skipped
      cadena=seg->get_path(etqpart, 0);
      last_etq_segmento_ant=etqpart.back();
      continue;
    }

    translations[TL1]->evaluate_translations_likelihood(Utils::likelihood_script);
    translations[TL1]->calculate_probability_each_path();

    update_counts(seg, translations, tags_pair, emis, last_etq_segmento_ant, tags_count, ambclass_count);

    //Actualizamos contadores
    cuenta_segmentos++;
    cuenta_palabras+=seg->vwords.size();
    cuenta_caminos+=seg->get_number_paths();
    if (translations[TL1]->are_translations_ok())
      cuenta_traducciones[TL1]+=translations[TL1]->get_number_translations(); 

    caminos_long_segmentos[seg->vwords.size()]+=seg->get_number_paths();
    long_segmentos[seg->vwords.size()]++;

    if (translations[TL1]->are_translations_ok())
      trads_long_segmentos[TL1][seg->vwords.size()]+=translations[TL1]->get_number_translations();

    if (seg->get_number_paths()>1)
      ambiguous_segments++;

    //Nos quedamos con la última etiqueta del segmento
    cadena=seg->get_path(etqpart, 0);
    last_etq_segmento_ant=etqpart.back();

    //We test if we need to update the molde used for prunning
    if (cuenta_palabras>=next_update_pruning_model) {

      //calculate_parameters(tags_pair, emis);
      calculate_smoothed_parameters(tags_count, tags_pair, ambclass_count, emis, corpus_length);

      cout<<"Mixing parameters, number of processed words: "<<cuenta_palabras<<"\n";

      mix_parameters((double)cuenta_palabras, (double)corpus_length, mixing_c);
      PathsPruner::set_tagger_data(tagger_data);           
	 	 
      /////next_save_probs+=PathsPruner::latency;
      next_update_pruning_model+=PathsPruner::latency;
    }

    //Vemos si tenemos que guardar los parámetros
    if (cuenta_palabras>=next_save_probs) {
      cout<<"Number of processed words: "<<cuenta_palabras<<"\n";
      cout<<"Number of segments treated: "<<cuenta_segmentos<<"\n";
      cout<<"Number of ambiguous segments: "<<ambiguous_segments<<"\n";
      cout<<"Total number of paths translated: "<<cuenta_caminos<<"\n";
      cout<<"Total number of translations: "<<cuenta_traducciones[TL1]<<"\n";
      cout<<"Number of segment correctly translated: "<<translations_ok[TL1]<<"\n";

      cout<<"Words to translate without pruning: "<<nwords_to_translate_without_pruning<<"\n";
      cout<<"Words translated after pruning: "<<nwords_translated<<"\n";
      cout<<"Ratio of pruning: "<<(double)nwords_translated/(double)nwords_to_translate_without_pruning<<"\n";

      map<int, long>::iterator itmap;   
      cout<<"Average number of paths per segment length\n";
      cout<<"--------------------------------------------------------\n"<<flush;
      for(itmap=long_segmentos.begin(); itmap!=long_segmentos.end(); itmap++)
	cout<<(*itmap).first<<" "<<((double)caminos_long_segmentos[(*itmap).first])/((double)(*itmap).second)<<"\n";

      cout<<"Average number of different translations per segment length\n";
      cout<<"--------------------------------------------------------\n"<<flush;
      for(itmap=long_segmentos.begin(); itmap!=long_segmentos.end(); itmap++)
	cout<<(*itmap).first<<" "<<((double)trads_long_segmentos[TL1][(*itmap).first])/((double)(*itmap).second)<<"\n";


      cout<<"Total number of paths treated per segment length\n";
      cout<<" seg. length  /  number of paths  /  number of considered paths\n";
      cout<<"--------------------------------------------------------\n"<<flush;
      for(map<int, pair<int, int> >::iterator it=pruning_stats.begin(); 
	  it!=pruning_stats.end(); it++) {
	cout<<(*it).first<<" "<<(*it).second.first<<" "<<(*it).second.second
            <<" --> "<<((double)(*it).second.second)/((double)(*it).second.first)<<"\n";
      }

      cout<<"Average of probability mass taken into account per segment length\n";
      cout<<" seg. length  / medium probability mass\n";
      cout<<"--------------------------------------------------------\n"<<flush;
      for(map<int, pair<int, double> >::iterator it=pruning_probmass.begin(); 
	  it!=pruning_probmass.end(); it++) {
	cout<<(*it).first<<" "<<" --> "<<((double)(*it).second.second)/((double)(*it).second.first)<<"\n";
      }

      cout<<"Time (user+sys): "<<Utils::get_usage_time()<<"\n";
      cout<<"--------------------------------------------------------\n"<<flush;

      //calculate_parameters(tags_pair, emis);
      calculate_smoothed_parameters(tags_count, tags_pair, ambclass_count, emis, corpus_length);

      char aux[15];
      sprintf(aux, "%d", next_save_probs);
      FILE *fprob;

      string probfile = filename+"."+aux+".nomixed.prob";  // File with all the tagger parameters
      fprob=fopen(probfile.c_str(), "w");	        
      tagger_data.write(fprob); 
      fclose(fprob);

      mix_parameters((double)cuenta_palabras, (double)corpus_length, mixing_c);
      //PathsPruner::set_tagger_data(tagger_data);           
	 	 
      probfile = filename+"."+aux+".prob";  // File with all the tagger parameters
      fprob=fopen(probfile.c_str(), "w");	        
      tagger_data.write(fprob);
      fclose(fprob);

      next_save_probs+=save_after_nwords;
    }
	 
    if (cuenta_palabras>=corpus_length) {
      break;
    }	 
  }
    
  //calculate_parameters(tags_pair, emis);  
  calculate_smoothed_parameters(tags_count, tags_pair, ambclass_count, emis, corpus_length);

  mix_parameters((double)cuenta_palabras, (double)corpus_length, mixing_c);
  PathsPruner::set_tagger_data(tagger_data);           

  if (seg!=NULL)
    delete seg;
  if(pruner!=NULL)
    delete pruner;
  for(i=0; i<translations.size(); i++)
    delete translations[i];
  translations.clear();
}

void 
HMM_TL_driven_trainer::update_counts(Segment* seg, vector<Translations*> &trans, 
				     map<int, map<int, double> > &tags_pair, 
				     map<int, map<int, double> > &emis, 
				     TTag last_tag_prev_segment,
				     map<int, double> &tags_count, 
				     map<int, double> &ambclass_count) {
  TTag tag1, tag2;
  set<TTag> tags;
  string cadena;

  vector<TTag> etqpart;

  double sum_probs=0;
  int ncamino;

  map<int, double> prob_each_path;
  double sum_prob_each_path=0.0;

  
  //Calculate the probability of each path, taking into account all
  //the translations provided
  for (int i=0; i<seg->get_number_paths(); i++) {
    double prob_este_camino=1; // p(g_i|s,M_tag)

    for(int j=0; j<trans.size(); j++) {
      if (trans[j]->are_translations_ok())
	prob_este_camino*=trans[j]->get_path_probability(i);
    }

    if (isnan(prob_este_camino))
      cerr<<"Error: Final prob. of path "<<i<<" is NAN\n";
    if (isinf(prob_este_camino))
      cerr<<"Error: Final prob. of path "<<i<<" is INF\n";

    prob_each_path[i]=prob_este_camino;
    sum_prob_each_path+=prob_este_camino;
  }

  if (isnan(sum_prob_each_path))
    cerr<<"Error: sum_prob_each_path is NAN\n";
  if (isinf(sum_prob_each_path))
    cerr<<"Error: sum_prob_each_path is INF\n";

  //Now, normalize the probabilities
  for (int i=0; i<seg->get_number_paths(); i++) {
    prob_each_path[i]=prob_each_path[i]/sum_prob_each_path;
  }

  //Update the number of times each ambiguity class have been seen
  //(this is not an estimation)
  for(int i=0; i<seg->vwords.size(); i++) {
    tags=seg->vwords[i].get_tags();
    if (tags.size()==0)
      tags=tagger_data.getOpenClass();

    int k=tagger_data.getOutput()[tags];
    if ((k>=tagger_data.getM())||(k<0)) {
      cerr<<"Error: Ambiguity class number out of range: "<<k<<"\n";
      cerr<<"Word: "<<seg->vwords[i].get_superficial_form()<<"\n";
      cerr<<"Ambiguity class: "<<seg->vwords[i].get_string_tags()<<"\n";
      cerr<<"Amb. class size: "<<tags.size()<<"\n";
      exit(EXIT_FAILURE);
    }
    ambclass_count[k]++;
  }

  for (int i=0; i<seg->get_number_paths(); i++) {

    double prob_este_camino=prob_each_path[i];
    cadena=seg->get_path(etqpart, i);
    
    Utils::print_debug("PATH ");
    Utils::print_debug(i);
    Utils::print_debug(", PROB ");
    Utils::print_debug(prob_este_camino);
    Utils::print_debug("\n");
    Utils::print_debug("              TAGS: ");
    for(int j=0; j<etqpart.size(); j++) {
      if(etqpart[j]==-1)
	Utils::print_debug("__UNKNOWN__");
      else
	Utils::print_debug(tagger_data.getArrayTags()[etqpart[j]]);
      Utils::print_debug(" ");
    }
    Utils::print_debug("\n");
    

    tag1=last_tag_prev_segment;
    for(int j=0; j<etqpart.size(); j++) {

      //Update tags_cout
      if (tag1>=0) {
	tags_count[tag1]+=prob_este_camino;
      } else {
	set<TTag>::iterator itag;
	int k=tagger_data.getOutput()[tagger_data.getOpenClass()];
	for(itag=tagger_data.getOpenClass().begin(); itag!=tagger_data.getOpenClass().end(); itag++) {
	  tags_count[*itag]+=prob_este_camino/((double)tagger_data.getOpenClass().size());
	}
      }

      tag2=etqpart[j];
      tags=seg->vwords[j].get_tags();
      if(tag2>=tagger_data.getN()) {
	cerr<<"Error: Tag number out of range: "<<tag2<<"\n";
	cerr<<"Word: "<<seg->vwords[j].get_superficial_form()<<"\n";
	cerr<<"Ambiguity class: "<<seg->vwords[j].get_string_tags()<<"\n";
	cerr<<"Tag2: "<<tag2<<" "<<tagger_data.getArrayTags()[tag2]<<"\n";
	exit(EXIT_FAILURE);
      }
	    
      if(tag2>=0) { //No es una palabra desconocida
	int k=tagger_data.getOutput()[tags];
	if ((k>=tagger_data.getM())||(k<0)) {
	  cerr<<"Error: Ambiguity class number out of range: "<<k<<"\n";
	  cerr<<"Word: "<<seg->vwords[j].get_superficial_form()<<"\n";
	  cerr<<"Ambiguity class: "<<seg->vwords[j].get_string_tags()<<"\n";
	  cerr<<"Amb. class size: "<<tags.size()<<"\n";
	  cerr<<"Tag2: "<<tag2<<" "<<tagger_data.getArrayTags()[tag2]<<"\n";
	  exit(EXIT_FAILURE);
	} 
	emis[tag2][k]+=prob_este_camino;
	/////tags_count[tag2]+=prob_este_camino;
	
      } else { //Palabra desconocida
	set<TTag>::iterator itag;
	int k=tagger_data.getOutput()[tagger_data.getOpenClass()];
	for(itag=tagger_data.getOpenClass().begin(); itag!=tagger_data.getOpenClass().end(); itag++) {
	  emis[*itag][k]+=prob_este_camino/((double)tagger_data.getOpenClass().size());
	  ////tags_count[*itag]+=prob_este_camino/((double)tagger_data.getOpenClass().size());
	}
      }
        
      //cerr<<"tag1 = "<<tag1<<"; tag2 = "<<tag2<<"\n";

      if((tag1>=0)&&(tag2>=0)) { //Ninguna de las dos es desconocida
	//cerr<<"Ambas son conocidas\n";
	tags_pair[tag1][tag2]+=prob_este_camino;
      } 
        
      else if ((tag1<0)&&(tag2>=0)) { //La primera es desconocida
	set<TTag>::iterator itag;
	//cerr<<"La primera es desconocida\n";
	for(itag=tagger_data.getOpenClass().begin(); itag!=tagger_data.getOpenClass().end(); itag++)
	  tags_pair[*itag][tag2]+=prob_este_camino/((double)tagger_data.getOpenClass().size());
      } 

      else if ((tag1>=0)&&(tag2<0)) { //La segunda es desconocida
	set<TTag>::iterator itag;
	//cerr<<"La segunda es desconocida\n";
	for(itag=tagger_data.getOpenClass().begin(); itag!=tagger_data.getOpenClass().end(); itag++)
	  tags_pair[tag1][*itag]+=prob_este_camino/((double)tagger_data.getOpenClass().size());	       
      } 

      else { //Ambas son desconocidas
	//cerr<<"Ambas son desconocidas\n";
	double prob_aux=prob_este_camino/((double)tagger_data.getOpenClass().size());
	prob_aux=prob_aux/((double)tagger_data.getOpenClass().size());		  
	set<TTag>::iterator itag, jtag;
	for(itag=tagger_data.getOpenClass().begin(); itag!=tagger_data.getOpenClass().end(); itag++){
	  for(jtag=tagger_data.getOpenClass().begin(); jtag!=tagger_data.getOpenClass().end(); jtag++){
	    tags_pair[*itag][*jtag]+=prob_aux;
	  }
	}
      }
	   
      tag1=tag2;
    }  	           
  }  
}


void 
HMM_TL_driven_trainer::init_allowed_bigrams() {
  bool found;
  
  cerr<<"Initializing allowed bigrams ..."<<flush;

  for (int i=0; i<tagger_data.getN(); i++) {
    for(int j=0; j<tagger_data.getN(); j++)
      allowed_bigrams[i][j] = true;
  }
          
  for(unsigned int i=0; i<tagger_data.getForbidRules().size(); i++) {
    allowed_bigrams[tagger_data.getForbidRules()[i].tagi][tagger_data.getForbidRules()[i].tagj]=false;
  }

  for(unsigned int i=0; i<tagger_data.getEnforceRules().size(); i++) {
    for(int j=0; j<tagger_data.getN(); j++) {
      found = false;
      for (int j2=0; j2<tagger_data.getEnforceRules()[i].tagsj.size(); j2++) {
	if (tagger_data.getEnforceRules()[i].tagsj[j2]==j) {
	  found = true;
	  break;
	}	  
      }
      if (!found)
        allowed_bigrams[tagger_data.getEnforceRules()[i].tagi][j] = false;
    }
  }
  
  cerr<<" done.\n";
}

/** TRANSLATION PENDING */
bool 
HMM_TL_driven_trainer::is_feasible_path(const TTag& last_etq_ant, const vector<TTag>& etqpart) {
  TTag etqi, etqj;
  
  if(!use_tags_rules)
    return true;

  etqi=last_etq_ant; // Será no homografa
  for(int j=0; j<etqpart.size(); j++) {
    etqj=etqpart[j];
      
    if(etqj>=tagger_data.getN()) {
      cerr<<"Error: Una etiqueta fuera de rango: "<<etqj<<"\n";
    }
      
    if ((etqi>=0)&&(etqj>=0)) { //Etiquetas pertenecientes a palabras conocidas
      if(!allowed_bigrams[etqi][etqj]) {
	return false;
      }
    }
      
    etqi=etqj;
  }
   
  //Todos los bigramas son válidos
  return true;
}


void 
HMM_TL_driven_trainer::calculate_parameters(map<int, map<int, double> > &tags_pairs, map<int, map<int, double> > &emis) {
  int i, j, k;

  //a[i][j]
  for(i=0; i<tagger_data.getN(); i++) {
    double sum=0;
    for(j=0; j<tagger_data.getN(); j++) {
      if (isnan(tags_pairs[i][j])) {
	cerr<<"Error: tags_pairs["<<i<<"]["<<j<<"] is NAN\n";
      }
      sum += tags_pairs[i][j]+0.5;
    }  
    
    if (isnan(sum)) {
      cerr<<"Error: al calcular A, sum para etq "<<i<<" is NAN\n";
    }
    
    for(j=0; j<tagger_data.getN(); j++) { 
      tagger_data.getA()[i][j] = (tags_pairs[i][j] + 0.5)/sum;
      if (isnan(tagger_data.getA()[i][j])) {
	cerr<<"Error: a["<<i<<"]["<<j<<" ] es NAN\n";
      }
    }
  }

  //b[i][k]
  for(i=0; i<tagger_data.getN(); i++) { 
    int nclases_aparece=0;
    double veces_aparece=0.0;
    for(k=0; k<tagger_data.getM(); k++)  {
      if (tagger_data.getOutput()[k].find(i)!=tagger_data.getOutput()[k].end())  { 
        //Tag i appears in ambiguity class k
	nclases_aparece++;	
	if(isnan(emis[i][k])) {
	  cerr<<"Error: emis["<<i<<"]["<<k<<"] is NAN\n";
	}
	veces_aparece+=emis[i][k];
      }
    }	      
    if (isnan(veces_aparece)) {
      cerr<<"Error: al calcular B para la etq "<<i<<" veces_aparece es NAN\n";
    } 
    for(k=0; k<tagger_data.getM(); k++)  { 
      if (tagger_data.getOutput()[k].find(i)!=tagger_data.getOutput()[k].end()) { 
	tagger_data.getB()[i][k] = (emis[i][k]+(((double)0.5)/((double)nclases_aparece)))/
                  (veces_aparece+((double)0.5));
	if(isnan(tagger_data.getB()[i][k])) {
	  cerr<<"Error: b["<<i<<"]["<<k<<"] is NAN\n";
	}
      }
    }
  }
}

double 
HMM_TL_driven_trainer::lambda(double count) {
  double sqrtval=sqrt(count);
  return sqrtval/(1+sqrtval);
}

map<int, double> 
HMM_TL_driven_trainer::prob_amb_class_from_tag(int i, map<int, double> &prob_ambclass) {
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
HMM_TL_driven_trainer::calculate_smoothed_parameters(map<int, double> &tags_count, 
                                                     map<int, map<int, double> > &tags_pairs, 
                                                     map<int, double> &ambclass_count, 
                                                     map<int, map<int, double> > &emis, 
                                                     double corpus_length) {
  map<int, double> prob_tag;
  double mu=lambda(corpus_length);
  double sum_tag=0.0;

  map<int, double> tags_count_for_emis;

  //cerr<<"Checking that counts are correct\n-----------------------------\n";
  for(int i=0; i<tagger_data.getN(); i++) {
    double sum=0.0;
    for(int j=0; j<tagger_data.getN(); j++) {
      sum+=tags_pairs[i][j];
    }
    if (fabs(sum-tags_count[i])>0.0001) {
      cerr<<"Error: sum does not agree for tag "<<tagger_data.getArrayTags()[i]
          <<": "<<sum<<" -- "<<tags_count[i]<<"\n";
    }
  }

  for(int k=0; k<tagger_data.getM(); k++) {
    double sum=0.0;
    for(int i=0; i<tagger_data.getN(); i++) {
      if (tagger_data.getOutput()[k].find(i)!=tagger_data.getOutput()[k].end()) {
	sum+=emis[i][k];
	tags_count_for_emis[i]+=emis[i][k];
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
    prob_ambclass_tag=prob_amb_class_from_tag(j, prob_ambclass);

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

void
HMM_TL_driven_trainer::mix_parameters(double words_processed, double corpus_length, double mixing_c) {
  //First we calculate the weigh of the new calculated parameters and the weigh of
  //the initial one (1-weigh)
  double weigh=words_processed/(corpus_length/mixing_c);
  double mixed_value;

  if (weigh>1)
    weigh=1;

  Utils::print_debug("Mixing parameters\n");
  Utils::print_debug("Words processed: ");
  Utils::print_debug(words_processed);
  Utils::print_debug("\n");
  Utils::print_debug("Weigh new model: ");
  Utils::print_debug(weigh);
  Utils::print_debug(" weigh initial model: ");
  Utils::print_debug(1.0-weigh);
  Utils::print_debug("\n");
  cout<<"Mixed parameters, weigh new model "<<weigh<<"\n";
  for(int i=0; i<tagger_data.getN(); i++) {
    for(int j=0; j<tagger_data.getN(); j++) {
      mixed_value =  weigh*tagger_data.getA()[i][j] + 
	             (1.0-weigh)*tagger_data_initial_model.getA()[i][j];
      tagger_data.getA()[i][j]=mixed_value;
    }
  }

  for(int i=0; i<tagger_data.getN(); i++) {
    for(int k=0; k<tagger_data.getM(); k++) {
      if(tagger_data.getOutput()[k].find(i)!=tagger_data.getOutput()[k].end()) {
	mixed_value = weigh*tagger_data.getB()[i][k] +
	              (1.0-weigh)*tagger_data_initial_model.getB()[i][k];
	tagger_data.getB()[i][k]=mixed_value;
      }
    }
  }
}

void
HMM_TL_driven_trainer::print_A() {
  int i,j;

  cout<<"TRANSITION MATRIX (A)\n------------------------------\n";
  for(i=0; i<tagger_data.getN(); i++)
    for(j=0; j<tagger_data.getN(); j++) {
      cout<<"A["<<i<<"]["<<j<<"] = "<<tagger_data.getA()[i][j]<<"\n";
    }
}

void
HMM_TL_driven_trainer::print_B() {
  int i,k;

  cout<<"EMISSION MATRIX (B)\n-------------------------------\n";
  for(i=0; i<tagger_data.getN(); i++)
    for(k=0; k<tagger_data.getM(); k++) {
      if(tagger_data.getOutput()[k].find(i)!=tagger_data.getOutput()[k].end())
        cout<<"B["<<i<<"]["<<k<<"] = "<<tagger_data.getB()[i][k]<<"\n";
    }
}

