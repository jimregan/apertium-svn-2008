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
 * Class Translations  (source file)
 *
 *  @author   Felipe Sánchez-Martínez - fsanchez@dlsi.ua.es
 */

#include "Translations.H"
#include "Utils.H"

#include <apertium/tagger_utils.h>
#include <apertium/utf_converter.h>

#include <cmath>
#include <cfloat>

Translations::Translations(Segment* seg) {
  segment=seg;
  all_translations_ok=true;

  string str_regex="[@#]"LEMMA"?"TAG"+";

  //int res=regcomp (&regex_wrongly_translated_word, str_regex.c_str(), REG_EXTENDED | REG_ICASE | REG_NOSUB);
  //if(res!=0) {
  //  char errmsg[2048];
  //  regerror(res, &regex_wrongly_translated_word, errmsg, 2048);
  //  cerr<<"Regex '"<<str_regex<<"' compilation error: "<<errmsg<<"\n";
  //  exit(1);
  //}

  regex_wrongly_translated_word.compile(str_regex);
}

Translations::~Translations() {
  //regfree(&regex_wrongly_translated_word);
}

int
Translations::get_number_translations() {
  return translations.size();
}

wstring 
Translations::get_translation_at(int pos) {
  return translations[pos];
}

void
Translations::set_path_translation(wstring trans, int path) {
  /*
    if (trim(trad).length()>0) {  
    int index_trad=translations[trim(trad)];
    path_translation[path]=index_trad;
    } else {
    cerr<<"Warning: Path "<<path<<" has a null translation\n";
    }
  */

  int index_trans=translations[tagger_utils::trim(trans)];
  path_translation[path]=index_trans;

  //int res=regexec(&regex_wrongly_translated_word, trans.c_str(), 0, NULL, 0);
  //if (res==0)
  //  all_translations_ok=false;

  string res=regex_wrongly_translated_word.match(UtfConverter::toUtf8(trans));
  if(res.length()>0)
    all_translations_ok=false;

  //If this segment has more than one path and one of them has a null
  //translation, then this set of translations should be ignored
  if ((segment->get_number_paths()>1) && (tagger_utils::trim(trans).length()==0))
    all_translations_ok=false;
}

wstring
Translations::get_path_translation(int path) {
  if (path_translation.find(path)!=path_translation.end())
    return translations[path_translation[path]];
  else {
    cerr<<"Warning:Translations::get_path_translation was called but there is no translation for the path "<<path<<", returning empty string\n";
    return L"";
  }
}

int
Translations::get_index_path_translation(int path) {
  if (path_translation.find(path)!=path_translation.end())
    return path_translation[path];
  else {
    cerr<<"Warning: Translations::get_index_path_translation was called but there is no translation for the path "<<path<<", returning -1\n";
    return -1;
  }
}

void
Translations::set_path_weigh(int path, double weigh) {
  path_weigh[path]=weigh;
}

double
Translations::get_path_weigh(int path) {
  return path_weigh[path];
}

void 
Translations::set_translation_likelihood(int index_trans, double likelihood) {
  translation_likelihood[index_trans]=likelihood;

  if (isnan(likelihood))
    wcerr<<L"Error: Likelihood of translation '"<<translations[index_trans]<<L"' in NAN\n";
  if (isinf(likelihood))
    wcerr<<L"Error: Likelihood of translation '"<<translations[index_trans]<<L"' in INF\n";
}

double 
Translations::get_translation_likelihood(int index_trans) {
  if (translation_likelihood.find(index_trans)!=translation_likelihood.end())
    return translation_likelihood[index_trans];
  else {
    wcerr<<L"Warining: Translations::get_translation_likelihood was called but there is no likelihood for the translation index "<<index_trans<<L", returning 0.0\n";
    return 0.0;
  }
}

vector<int> 
Translations::get_paths_same_translation(int index_trans) {
  vector<int> same_trans;
   
  for (int i=0; i<segment->get_number_paths(); i++) {
    if ((path_translation.find(i)!=path_translation.end()) &&
	(path_translation[i]==index_trans))
      same_trans.push_back(i);
  }
  
  return same_trans;
}

void
Translations::evaluate_translations_likelihood(string likelihood_script) {
  double sum_likelihood=0;
  double likelihood;

  Utils::print_debug(L"LIKELIHOOD script: ");
  Utils::print_debug(UtfConverter::fromUtf8(likelihood_script));
  Utils::print_debug(L"\n");

  //Evaluate the likelihood of each translation in the target language <-> p(tau(g_i,s)|M_TL)
  for (int i=0; i<get_number_translations(); i++) {
    if((are_translations_ok()) && (get_number_translations()>1))
      likelihood=Utils::likelihood(likelihood_script, get_translation_at(i)); 
    else
      likelihood=1;

    Utils::print_debug(L"TRANSLATION TO EVALUATE: ");
    Utils::print_debug(get_translation_at(i));
    Utils::print_debug(L"\n");
    Utils::print_debug(L"LIKEKLIHOOD: ");
    Utils::print_debug(likelihood);
    Utils::print_debug(L"\n");

    if ((likelihood<=0)||isnan(likelihood)) {
      wcerr<<L"Error: Likelihood NULL or NAN: "<<get_translation_at(i)<<L"\n";
      likelihood=DBL_MIN;
    }
    if (likelihood>1) {
      wcerr<<L"Warning: Likelihood of '"<<get_translation_at(i)<<L"' is "<<likelihood<<L"\n";
      likelihood=1;
    }

    set_translation_likelihood(i,likelihood);
    sum_likelihood+=likelihood;
  }

  //Normalizamos las verosimilitudes
  for(int i=0; i<get_number_translations(); i++) {
    likelihood=get_translation_likelihood(i);
    set_translation_likelihood(i,likelihood/sum_likelihood);
      
    Utils::print_debug(L"TRANSLATION TO NORMALIZE: ");
    Utils::print_debug(get_translation_at(i));
    Utils::print_debug(L"\n");
    Utils::print_debug(L"NORMALIZED LIKELIHOOD: ");
    Utils::print_debug(get_translation_likelihood(i));
    Utils::print_debug(L"\n");
  }
}

void 
Translations::set_previoulsy_evaluated_translations_likelihood(map<wstring, double>& translations_likelihoods) {
  double sum_likelihood=0;
  double likelihood;

  //Take the likelihood of each translation in the target language <-> p(tau(g_i,s)|M_TL)
  for (int i=0; i<get_number_translations(); i++) {
    if((are_translations_ok()) && (get_number_translations()>1))
      likelihood=translations_likelihoods[get_translation_at(i)];
    else
      likelihood=1;

    Utils::print_debug(L"TRANSLATION TO EVALUATE: ");
    Utils::print_debug(get_translation_at(i));
    Utils::print_debug(L"\n");
    Utils::print_debug(L"LIKEKLIHOOD: ");
    Utils::print_debug(likelihood);
    Utils::print_debug(L"\n\n");

    if ((likelihood<=0)||isnan(likelihood)) {
      wcerr<<L"Error: Likelihood NULL or NAN: "<<get_translation_at(i)<<L"\n";
      likelihood=DBL_MIN;
      //      all_likelihoods_ok=false;
    }
    if (likelihood>1) {
      wcerr<<L"Warning: Likelihood of '"<<get_translation_at(i)<<L"' is "<<likelihood<<L"\n";
      likelihood=1;
      //      all_likelihoods_ok=false;
    }

    set_translation_likelihood(i,likelihood);
    sum_likelihood+=likelihood;
  }

  //Normalizamos las verosimilitudes
  for(int i=0; i<get_number_translations(); i++) {
    likelihood=get_translation_likelihood(i);
    set_translation_likelihood(i,likelihood/sum_likelihood);
      
    Utils::print_debug(L"TRANSLATION TO NORMALIZE: ");
    Utils::print_debug(get_translation_at(i));
    Utils::print_debug(L"\n");
    Utils::print_debug(L"NORMALIZED LIKELIHOOD: ");
    Utils::print_debug(get_translation_likelihood(i));
    Utils::print_debug(L"\n\n");
  }
}


void
Translations::calculate_probability_each_path() {
  //Firts we calculate the contribution of each path to the
  //translation <-> p(g_i|tau(g_i,s)). The contribution is
  //approximated with equiprobabilities
  for(int i=0; i<get_number_translations(); i++) {
    vector<int> paths_same_trans;

    paths_same_trans=get_paths_same_translation(i);

    for(int j=0; j<paths_same_trans.size(); j++) 
      set_path_weigh(paths_same_trans[j],((double)1.0)/((double)paths_same_trans.size()));
  }


  for (int i=0; i<segment->get_number_paths(); i++) {
    double prob; // p(g_i|s,M_tag)

    prob=get_path_weigh(i);

    if (prob>0)
      prob*=get_translation_likelihood(get_index_path_translation(i));

    path_prob[i]=prob;

    if (isnan(prob)) 
      cerr<<"Error: Probability of path "<<i<<"in NAN\n";
    if (isinf(prob)) 
      cerr<<"Error: Probability of path "<<i<<"in INF\n";

  }
}

double 
Translations::get_path_probability(int path) {
  return path_prob[path];
}

bool 
Translations::are_translations_ok() {
  return all_translations_ok;
}
