/*
 * Copyright (C) 2006 Universitat d'Alacant / Universidad de Alicante
 * author: Felipe Sánchez-Martínez
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

#include <apertium/LexTor.H>
#include <apertium/StringUtils.H>

#include <algorithm>
#include <string>
#include <cmath>

#define PI 3.14159265358979323846264338327950288

bool LexTor::debug;
double LexTor::angleth;

LexTor::LexTor() {
  lextor_data=NULL;
  tlmodel=NULL;
}
  
LexTor::LexTor(const LexTor& lt) {
  lextor_data=lt.lextor_data;
  tlmodel=lt.tlmodel;
}
  
LexTor::~LexTor() {
}

void 
LexTor::set_lextor_data(LexTorData* ltd) {
  lextor_data=ltd;
}

void 
LexTor::set_tlmodel(LexTorData* tlm) {
  tlmodel=tlm;
}

void 
LexTor::set_bildic(FSTProcessor *fstp) {
  fstpbil=fstp;
}

void 
LexTor::trainwrd(istream& is, int left, int right, double weigth_exponent) {
  if (lextor_data==NULL) {
    cerr<<"Error in LexTor::trainwrd, you must call set_lextor_data before training\n";
    exit(EXIT_FAILURE);
  }

  lextor_data->ensure_stopwords_ok();

  cerr<<"Number of words to take into account on the left side: "<<left<<"\n";
  cerr<<"Number of words to take into account on the right side: "<<right<<"\n";

  set<string> words2workwith=lextor_data->get_words();
  set<string>::iterator itword;

  map<string, COUNT_DATA_TYPE> wordsum;

  cerr<<"Words to work with:\n";
  for(itword=words2workwith.begin(); itword!=words2workwith.end(); itword++) {
    cerr<<*itword<<"\n";
  }
  cerr<<"\n";

  is.clear();
  is.seekg(ios::beg);

  int nw=0;

  map<string, map<string, COUNT_DATA_TYPE> > context;
  deque<string> buffer;
  unsigned word_index=(unsigned)left;

  unsigned buffer_max_size=(unsigned)(left+1+right);
  
  LexTorWord *ltword;
  ltword=LexTorWord::next_word(is);
  while(ltword!=NULL) {
    if ((++nw%250000)==0)
      cerr<<nw<<" words processed\n";
      
    if(debug) {
      cerr<<"Word read from corpus: "<<ltword->get_word_string()
	  <<", reduced: "<<lextor_data->reduce(ltword->get_word_string())<<flush;
      getchar();
    }

    string reduced_word=lextor_data->reduce(ltword->get_word_string());

    if (!lextor_data->is_stopword(reduced_word)) {
      if (buffer.size()>=buffer_max_size) {
	buffer.pop_front();
      }
      buffer.push_back(reduced_word);

      wordsum[reduced_word]+=1.0;

      //The buffer is already full
      if (buffer.size()==buffer_max_size) {
	for(itword=words2workwith.begin(); itword!=words2workwith.end(); itword++) {
	  if (buffer[word_index]==(*itword)) {
	    if(debug) {
	      cerr<<"WINDOW: ";
	      for (unsigned i=0; i<buffer.size(); i++) {
		if(i==word_index)
		  cerr<<"[>>>>"<<buffer[i]<<"<<<<] ";
		else
		  cerr<<"["<<buffer[i]<<"] ";
	      }
	      cerr<<"\n";
	    }

	    int distance=(-1)*left;
	    for(unsigned i=0; i<buffer.size(); i++) {
	      if ((i!=word_index) && (buffer[i]!=(*itword))) {
		if (debug) {
		  cerr<<"   WORD: ["<<buffer[i]<<"] ";
		  cerr<<"   DISTANCE: "<<distance<<" ";
		  cerr<<"   ADDED COUNT: "<<1.0/pow(fabs((double)distance),weigth_exponent)<<" ";
		  cerr<<"   TO ["<<*itword<<"]\n";
		}
		context[*itword][buffer[i]]+=1.0/pow(fabs((double)distance),weigth_exponent);
	      }
	      distance++;
	    }
	    if (debug)
	      getchar();
	    break;
	  }
	}
      }
    }

    delete ltword;
    ltword=LexTorWord::next_word(is);
  }

  cerr<<"Corpus has "<<nw<<" words\n";

  //Set the count of each word
  map<string, COUNT_DATA_TYPE>::iterator itws;
  for(itws=wordsum.begin(); itws!=wordsum.end(); itws++) {
    lextor_data->set_wordcount(itws->first,itws->second);
    //if(debug) {
    cerr<<"wordcount("<<itws->first<<") = "<<itws->second<<"\n";
    //}
  }

  //All co-occurrences have been collected. We need to filter them
  //so as to take into account only the n most frequents
  for(itword=words2workwith.begin(); itword!=words2workwith.end(); itword++) {
    PairStringCountComparer comparer;
    vector<pair<string, COUNT_DATA_TYPE> > context_v;
    map<string, COUNT_DATA_TYPE>::iterator itm;

    while(context[*itword].size()>0) {
      itm=context[*itword].begin();
      context_v.push_back(*itm);
      context[*itword].erase(itm);
    }

    sort(context_v.begin(), context_v.end(), comparer);
    string w=*itword;
    lextor_data->set_cooccurrence_context(w, context_v);
    lextor_data->set_lexchoice_sum(w, wordsum[w]);

    //if (debug) {
    cerr<<"lexchoice_sum("<<w<<") = "<<wordsum[w]<<"\n";
    //}
  }
}

void 
LexTor::trainlch(istream& is, int left, int right, LexTorData& tlwordmodel, 
                 FSTProcessor& dic, FSTProcessor& bildic, double weigth_exponent) {
  if (lextor_data==NULL) {
    cerr<<"Error in LexTor::trainlch, you must call set_lextor_data before training\n";
    exit(EXIT_FAILURE);
  }

  lextor_data->ensure_stopwords_ok();

  cerr<<"Number of words to take into account on the left side: "<<left<<"\n";
  cerr<<"Number of words to take into account on the right side: "<<right<<"\n";

  set<string> words2workwith=lextor_data->get_words();
  set<string>::iterator itword;

  map<string, COUNT_DATA_TYPE> wordsum;
  map<string, COUNT_DATA_TYPE> lechsum;

  cerr<<"Words to work with:\n";
  for(itword=words2workwith.begin(); itword!=words2workwith.end(); itword++) {
    cerr<<*itword<<"\n";
  }
  cerr<<"\n";

  //For a given lexical choice it stores its translation
  map<string, string> lexchoice_translation;
  map<string, set<string> > lexical_choices_of_word;

  cerr<<"Lexical choices:\n";
  for(itword=words2workwith.begin(); itword!=words2workwith.end(); itword++) {
    set<string> lexical_choices=lextor_data->get_lexical_choices(*itword);
    lexical_choices_of_word[*itword]=lexical_choices;
    set<string>::iterator itlch;
    for(itlch=lexical_choices.begin(); itlch!=lexical_choices.end(); itlch++) {
      lexchoice_translation[*itlch]=tlwordmodel.reduce(bildic.biltrans(*itlch,false));
      cerr<<*itlch<<", tr:"<<lexchoice_translation[*itlch]<<"\n";
    }
  }
  cerr<<"\n";


  is.clear();
  is.seekg(ios::beg);

  int nw=0;

  map<string, map<string, COUNT_DATA_TYPE> > context;
  deque<LexTorWord> buffer;

  int word_index=left;
  unsigned buffer_max_size=left+right+1;

  LexTorWord *ltword;
  ltword=LexTorWord::next_word(is,&dic);
  while(ltword!=NULL) {
    if (debug) {
      cerr<<"Word read from corpus: "<<ltword->get_word_string()<<", reduce: "<<lextor_data->reduce(ltword->get_word_string());
      getchar();
    }
    if ((++nw%250000)==0)
      cerr<<nw<<" words processed\n";

    string reduced_word=lextor_data->reduce(ltword->get_word_string());

    if (!lextor_data->is_stopword(reduced_word)) {	
      if (buffer.size()>=buffer_max_size) {
	buffer.pop_front();
      }
      buffer.push_back(*ltword);

      wordsum[reduced_word]+=1.0;

      //The buffer is already full
      if (buffer.size()==buffer_max_size) {

	string reduced_buffer_word=lextor_data->reduce(buffer[word_index].get_word_string());

        for(itword=words2workwith.begin(); itword!=words2workwith.end(); itword++) {
	  if (reduced_buffer_word==(*itword)) {
	    //We translate each word in the context
	    //Note: Words in the context can also be ambiguous (with more than one lexical choice)
	    //In that case the count will come from all the possible
	    //translations 
	    vector<string> translation_buffer[buffer_max_size];
	    string reduced_buffer[buffer_max_size];

	    for (int i=0; i<(int)buffer_max_size; i++) {
	      reduced_buffer[i]=lextor_data->reduce(buffer[i].get_word_string());	      
	    }

	    if(debug) {
	      cerr<<"WINDOW: ";
	      for (unsigned i=0; i<buffer.size(); i++) {
		if(i==(unsigned)word_index)
		  cerr<<"[>>>>"<<reduced_buffer[i]<<"<<<<] ";
		else
		  cerr<<"["<<reduced_buffer[i]<<"] ";
	      }
	      cerr<<"\n";
	      cerr<<"TRANSLATED: ";
	    }

	    for (int i=0; i<(int)buffer_max_size; i++) {
	      string str_translations="";
	      for(int j=0; j<buffer[i].n_lexical_choices(); j++) {
		string aux_tr=buffer[i].translate(bildic,j);
		if (aux_tr.length()>0) {
		  string tr=tlwordmodel.reduce(aux_tr);
		  translation_buffer[i].push_back(tr);
		  str_translations+=tr+"/";
		} else {
		  cerr<<"Warning in LexTor::trainlch: translation of ["<<buffer[i].get_word_string()
		      <<"] is empty\n";
		}
	      }
	      if (debug) {
		if (i==word_index)
		  cerr<<"[>>>>"<<str_translations<<"<<<<] ";
		else
		  cerr<<"["<<str_translations<<"] ";
	      }
	    }

	    if(debug)
	      cerr<<"\n";

	    set<string> lexical_choices=lexical_choices_of_word[*itword];
	    set<string>::iterator itlch;

	    map<string, map<string, COUNT_DATA_TYPE> > local_context;
	    map<string, COUNT_DATA_TYPE> sumvotes_context;

	    //For each lexical choice the counts from the TL are collected
	    for(itlch=lexical_choices.begin(); itlch!=lexical_choices.end(); itlch++) {
	      for (int i=0; i<(int)buffer_max_size; i++) {
		if ((i!=word_index)&&(reduced_buffer[i]!=(*itword))) {
		  COUNT_DATA_TYPE target_vote=0;

		  //The counts of the TL co-occurrence model are transferred to the SL. If the SL word is ambiguous
		  //it will have more than one translation into TL, so we need to normalize using the frequency of words
		  //in the TL
		  double translation_weighs[translation_buffer[i].size()];
		  double sum=0.0;
		  if (translation_buffer[i].size()>1) {
		    for(int j=0; j<(int)translation_buffer[i].size(); j++) {
		      translation_weighs[j]=tlwordmodel.get_lexchoice_sum(translation_buffer[i][j]);
		      sum+=translation_weighs[j];

		      //!!!!! Para hacer que no tenga en cuenta las polisemicas del contexto
		      ///////translation_weighs[j]=0;
		      //!!!!!

		      if (debug) {
			cerr<<"Frequency of translation ["<<translation_buffer[i][j]<<"] ="
			    <<translation_weighs[j]<<"\n";
		      }
		    }
		  } else {
		    translation_weighs[0]=1;
		    sum=1;
		  }

		  for(int j=0; j<(int)translation_buffer[i].size(); j++) {
		    translation_weighs[j]=translation_weighs[j]/sum;
		    if (debug) {
		      cerr<<"Weight of translation ["<<translation_buffer[i][j]<<"] ="
			  <<translation_weighs[j]<<"\n";
		    }
		  }

		  for(int j=0; j<(int)translation_buffer[i].size(); j++) {
		    if (lexchoice_translation[*itlch].length()==0) {
		      cerr<<"Error: Translation of lexical choice '"<<*itlch<<"' is empty\n";
		    }

		    double aux_vote=0;
		    //aux_vote=tlwordmodel.vote_from_word(lexchoice_translation[*itlch], 
		    //				    translation_buffer[i][j])*translation_weighs[j];
		    if (tlwordmodel.get_wordcount(lexchoice_translation[*itlch])>0) {
		      aux_vote=(tlwordmodel.vote_from_word(lexchoice_translation[*itlch],translation_buffer[i][j])/
				tlwordmodel.get_wordcount(lexchoice_translation[*itlch]))*translation_weighs[j];
		      if (debug) {
			cerr<<"C("<<lexchoice_translation[*itlch]<<", "<<translation_buffer[i][j]<<") = "
			    <<tlwordmodel.vote_from_word(lexchoice_translation[*itlch],translation_buffer[i][j])<<"\n";
			cerr<<"C("<<lexchoice_translation[*itlch]<<") = "<<tlwordmodel.get_wordcount(lexchoice_translation[*itlch])<<"\n";
		      }
		    } else {
		      if (tlwordmodel.vote_from_word(lexchoice_translation[*itlch],translation_buffer[i][j])>0) {
			cerr<<"Error in LexTor::trainlch: TL vote is not null, but its word count is null.\n";
			cerr<<"lexchoice_translation: "<<lexchoice_translation[*itlch]<<"\n";
			cerr<<"translation_buffer: "<<translation_buffer[i][j]<<"\n";
		      }
		    }
		    target_vote+=aux_vote;

		    if(debug) {
		      cerr<<"Target vote for ["<<lexchoice_translation[*itlch]
			  <<"] from ["<<translation_buffer[i][j]<<"] = "<<aux_vote<<"\n";
		    }
		  }

		  if (target_vote>0) {
		    local_context[*itlch][reduced_buffer[i]]+=target_vote;
		    sumvotes_context[reduced_buffer[i]]+=target_vote;
		  }
		}
	      }
	    }

	    if (debug) {
	      cerr<<"COUNTS NORMALIZATION\n";
	    }

	    //Now we normalize the counts and estimate the number of
	    //times each lexical choice has been seen.
	    map<string, COUNT_DATA_TYPE> local_lexsum;
	    double local_lexsumsum=0.0;
	    for(itlch=lexical_choices.begin(); itlch!=lexical_choices.end(); itlch++) {
	      int distance=(-1)*left;
	      for (int i=0; i<(int)buffer_max_size; i++) { 
		if ((i!=word_index)&&(reduced_buffer[i]!=(*itword))) {
		  if (local_context[*itlch][reduced_buffer[i]]>0) {
		    double cc=local_context[*itlch][reduced_buffer[i]]/sumvotes_context[reduced_buffer[i]];
		    double count_to_apply=cc/pow(fabs((double)distance),weigth_exponent);
		    context[*itlch][reduced_buffer[i]]+=count_to_apply;
		    if (debug) {
		      cerr<<"Lexical choice: ["<<*itlch
                          <<"], context word: ["<<reduced_buffer[i]<<"], "
			  <<"normalize count: "<<cc<<"\n";
		      cerr<<"Distance: "<<distance<<", count to apply: "
			  <<count_to_apply<<"\n";

		    }

		    local_lexsum[*itlch]+=count_to_apply;
		    local_lexsumsum+=count_to_apply;

		    if (debug) {
		      cerr<<"local_lexsum["<<*itlch<<"] = "<<local_lexsum[*itlch]<<"\n";
		      cerr<<"local_lexsumsum = "<<local_lexsumsum<<"\n";
		    }

		  }
		}
		distance++;
	      }
	    }

	    for(itlch=lexical_choices.begin(); itlch!=lexical_choices.end(); itlch++) {
	      if ((local_lexsum[*itlch]>0) && (local_lexsumsum>0))
		lechsum[*itlch]+=local_lexsum[*itlch]/local_lexsumsum;
	      if (debug) {
		cerr<<"lechsum["<<*itlch<<"] = "<<lechsum[*itlch]<<"\n";
	      }
	    }
	    

	    if(debug) {
	      cerr<<"\n";
	      getchar();
	    }

	    break;
	  }
	}
      }
    } 

    delete ltword;
    ltword=LexTorWord::next_word(is,&dic);
  }
  
  cerr<<"Corpus has "<<nw<<" words\n";

  //Set the count of each word
  map<string, COUNT_DATA_TYPE>::iterator itws;
  for(itws=wordsum.begin(); itws!=wordsum.end(); itws++) {
    lextor_data->set_wordcount(itws->first,itws->second);
    //if(debug) {
    cerr<<"wordcount("<<itws->first<<") = "<<itws->second<<"\n";
    //}
  }

  //All co-occurrences have been collected. We need to filter them
  //so as to take into account only the n most frequent
  for(itword=words2workwith.begin(); itword!=words2workwith.end(); itword++) {
    set<string> lexical_choices=lexical_choices_of_word[*itword];
    set<string>::iterator itlch;
    for(itlch=lexical_choices.begin(); itlch!=lexical_choices.end(); itlch++) {
      PairStringCountComparer comparer;
      vector<pair<string, COUNT_DATA_TYPE> > context_v;
      map<string, COUNT_DATA_TYPE>::iterator itm;

      while(context[*itlch].size()>0) {
	itm=context[*itlch].begin();
	//cerr<<itm->first<<" "<<itm->second<<"\n";
	context_v.push_back(*itm);
	context[*itlch].erase(itm);
      }
    
      sort(context_v.begin(), context_v.end(), comparer);
      string lch=*itlch;
      lextor_data->set_cooccurrence_context(lch, context_v);
      //lextor_data->set_lexchoice_sum(lch, tlwordmodel.get_lexchoice_sum(lexchoice_translation[lch]));

      //cerr<<"lexchoice_sum("<<lch<<") = lexchoice_sum_target("<<lexchoice_translation[lch]<<") ="
      //    <<tlwordmodel.get_lexchoice_sum(lexchoice_translation[lch])<<"\n";
    }
  } 

  //Set the count of each lexical choice
  map<string, COUNT_DATA_TYPE>::iterator itlcs;
  for(itlcs=lechsum.begin(); itlcs!=lechsum.end(); itlcs++) {
    lextor_data->set_lexchoice_sum(itlcs->first,itlcs->second);
    //if(debug) {
    cerr<<"lexchoice_sum("<<itlcs->first<<") = "<<itlcs->second<<"\n";
    //}
  }


  cerr<<"Training done\n"; 
}

void 
LexTor::lexical_selector(istream& is, FSTProcessor &fstp, int left, int right, double weigth_exponent, LexTorEval* lteval) {
  if (lextor_data==NULL) {
    cerr<<"Error in LexTor::lexical_selector, you must call set_lextor_data before\n";
    exit(EXIT_FAILURE);
  }

  deque<LexTorWord> buffer;
  deque<LexTorWord> window;

  LexTorWord  nullword("NULLWORD", &fstp);

  for(int i=0; i<(left+right+1); i++)
    window.push_back(nullword);

  int retain=0;

  LexTorWord* ltword=NULL;
  ltword=LexTorWord::next_word(is, &fstp);

  while(ltword) {
    //cerr<<"Word read: "<<ltword->get_word_string()
    //<<", reduced: "<<lextor_data->reduce(ltword->get_word_string())<<" ";
    //cerr<<"# lexical choices: "<<ltword->n_lexical_choices()<<"\n";

    if (!lextor_data->is_stopword(lextor_data->reduce(ltword->get_word_string()))) { 
      if (window.size()>=(unsigned)(left+1+right)) 
	window.pop_front();
      
      window.push_back(*ltword);

      if (ltword->n_lexical_choices()>1) {
	retain++;
	if (retain>1)
	  buffer.push_back(*ltword);
      } else {
	if (retain>0) 
	  buffer.push_back(*ltword);
	else {
	  cout<<ltword->get_lexical_choice(-1,true);
	  if (lteval) 
	    lteval->evalword(*ltword, -1, lextor_data);
	}
      }

      if (window[left].n_lexical_choices()>1) {

	if (debug) {
	  cerr<<"WINDOW: ";
	  for(int i=0; i<(int)window.size(); i++) {
	    if(i==left)
	      cerr<<"[>>>>"<<window[i].get_word_string()<<"<<<<] ";
	    else
	      cerr<<"["<<window[i].get_word_string()<<"] ";
	  }
	  cerr<<"\n";
	  cerr<<"BUFFER: ";
	  for(int i=0; i<(int)buffer.size(); i++)
	    cerr<<"["<<buffer[i].get_word_string()<<"] ";
	  cerr<<"\n\n";
  
	}

	int winner=estimate_winner_lch(window, left, weigth_exponent);

	cout<<window[left].get_lexical_choice(winner,true);
	if (lteval) 
	  lteval->evalword(window[left], winner, lextor_data);
	
	//For debug
	/*
	  cout<<" | ";
	  for(int j=0; j<window[left].n_lexical_choices(); j++) {
	  if (j>0)
	  cout<<"|";
	  cout<<window[left].get_lexical_choice(j,false);
	  }
	  cout<<"\n";
	*/

	//Now those words that were retained must be released
	if(retain>0) {
	  while ((buffer.size()>0)&&(buffer[0].n_lexical_choices()==1)) {
	    cout<<buffer[0].get_lexical_choice(-1,true);
	    if (lteval) 
	      lteval->evalword(buffer[0], -1, lextor_data);
	    buffer.pop_front();
	  }
	  if ((buffer.size()>0)&&(buffer[0].n_lexical_choices()>1))
	    buffer.pop_front(); 

	  retain--;
	}
      } 
    } else { //It's a stopword
      if (retain>0) 
	buffer.push_back(*ltword);
      else {
	cout<<ltword->get_lexical_choice(-1,true);
	if (lteval) 
	  lteval->evalword(*ltword, -1, lextor_data);
      }
    }

    delete ltword;
    ltword=LexTorWord::next_word(is, &fstp);
  }

  if (retain>0) {
    for(unsigned i=left+1; i<window.size(); i++) {
      if (window[i].n_lexical_choices()>1) {
	int winner=estimate_winner_lch(window, i, weigth_exponent);

	cout<<window[i].get_lexical_choice(winner,true);
	if (lteval)
	  lteval->evalword(window[i], winner, lextor_data);

	//For debug
	/*
	  cout<<" | ";
	  for(int j=0; j<window[i].n_lexical_choices(); j++) {
	  if (j>0)
	  cout<<"|";
	  cout<<window[i].get_lexical_choice(j,false);
	  }
	  cout<<"\n";
	*/

	//Now those words that were retained must be released
	if(retain>0) {
	  while ((buffer.size()>0)&&(buffer[0].n_lexical_choices()==1)) {
	    cout<<buffer[0].get_lexical_choice(-1,true);
	    if (lteval) 
	      lteval->evalword(buffer[0], -1, lextor_data);
	    buffer.pop_front();
	  }
	  if ((buffer.size()>0)&&(buffer[0].n_lexical_choices()>1))
	    buffer.pop_front(); 

	  retain--;
	}

      }
    }
  }

  //cerr<<"retain: "<<retain<<"\n";
}

int 
LexTor::estimate_winner_lch(deque<LexTorWord>& window, int word_index, double weigth_exponent) {
  //return estimate_winner_lch_cosine(window, word_index, weigth_exponent);
  return estimate_winner_lch_voting(window, word_index, weigth_exponent);
  //return estimate_winner_lch_mostprob(window, word_index, weigth_exponent);
  //return estimate_winner_lch_votingtl(window, word_index, weigth_exponent);
  //return -1;
}

int 
LexTor::estimate_winner_lch_voting(deque<LexTorWord>& window, int word_index, double weigth_exponent) {
  double lexchoices_count[window[word_index].n_lexical_choices()];

  if (debug) {
    cerr<<"WINDOW: ";
    for(unsigned i=0; i<window.size(); i++) {
      if (i==(unsigned)word_index)
	cerr<<"[>>>>"<<lextor_data->reduce(window[i].get_word_string())<<"<<<<] ";
      else
	cerr<<"["<<lextor_data->reduce(window[i].get_word_string())<<"] ";
    }
    cerr<<"\n";
  }

  //
  double sum_lexchoices=0.0;
  for(int i=0; i<window[word_index].n_lexical_choices(); i++) {
    double aux_lexchoice_sum=lextor_data->get_lexchoice_sum(lextor_data->reduce_lexical_choice(window[word_index].get_lexical_choice(i,false)));
    sum_lexchoices+=aux_lexchoice_sum;
    if (debug) {
      cerr<<"lexchoice_sum("<<lextor_data->reduce_lexical_choice(window[word_index].get_lexical_choice(i,false))<<") = "<<aux_lexchoice_sum<<"\n";
    }
  }
  double wordcount=lextor_data->get_wordcount(lextor_data->reduce(window[word_index].get_word_string()));
  if (debug) {
    cerr<<"wordcount("<<lextor_data->reduce(window[word_index].get_word_string())<<") = "<<wordcount<<"\n";
  }
  //

  for(int i=0; i<window[word_index].n_lexical_choices(); i++) {
    lexchoices_count[i]=0;
    string reduced_lexchoice=lextor_data->reduce_lexical_choice(window[word_index].get_lexical_choice(i,false));
    if (debug) {
      cerr<<"lexical choice: "<<window[word_index].get_lexical_choice(i)<<" reduced: "<<reduced_lexchoice<<"\n";
    }

    int distance=(-1)*(word_index);
    for(int j=0; j<(int)window.size(); j++) { 
      //For all words in the context window
      if(j!=word_index) {
	COUNT_DATA_TYPE vote=0;

	string reduced_word=lextor_data->reduce(window[j].get_word_string());

	if (lextor_data->get_wordcount(reduced_word)>0) {
	  vote=lextor_data->vote_from_word(reduced_lexchoice, reduced_word)/
	    (((lextor_data->get_lexchoice_sum(reduced_lexchoice))/sum_lexchoices)*wordcount);

	  lexchoices_count[i]+=vote/pow(fabs((double)distance),weigth_exponent);
	}

	if (debug) {
	  cerr<<"Count for "<<reduced_lexchoice<<" from "<<reduced_word<<" is "<<vote<<"\n";
	  cerr<<"Vote: "<<lextor_data->vote_from_word(reduced_lexchoice, reduced_word)<<" word count: "
	      <<lextor_data->get_wordcount(reduced_word)<<"\n";
	  cerr<<"["<<reduced_word<<"] DISTANCE: "<<distance<<", ";
	  cerr<<" Count to apply: "<<vote/pow(fabs((double)distance),weigth_exponent)<<"\n";
	}
      }
      distance++;
    }
  }

  //Now the winner is calculated
  int winner=-1; //This will make the default one to be used if unchanged
  COUNT_DATA_TYPE winner_vote=-100000000;
  for(int i=0; i<window[word_index].n_lexical_choices(); i++) {
    if ((lexchoices_count[i]>0) && (lexchoices_count[i]>winner_vote)) {
      winner_vote=lexchoices_count[i];
      winner=i;
    } 
    /*
      else if ((lexchoices_count[i]>0) && (lexchoices_count[i]==winner_vote)) {
      //Take the most probable one, the one with the highest sum
      COUNT_DATA_TYPE sum_i=lextor_data->get_lexchoice_sum(lextor_data->reduce(window[word_index].get_lexical_choice(i)));
      COUNT_DATA_TYPE sum_win=lextor_data->get_lexchoice_sum(lextor_data->reduce(window[word_index].get_lexical_choice(winner)));
      if (sum_i>sum_win)
      winner=i;
      }
    */
  }
  
  if (debug) {
    cerr<<"WINNER: "<<winner<<" "<<window[word_index].get_lexical_choice(winner)<<"\n";
  }
  return winner;
}

int 
LexTor::estimate_winner_lch_mostprob(deque<LexTorWord>& window, int word_index,  double weigth_exponent) {
  int winner=-1;
  double greatest_sum=-1;
  for(int i=0; i<window[word_index].n_lexical_choices(); i++) {
    string reduced_lexchoice=lextor_data->reduce_lexical_choice(window[word_index].get_lexical_choice(i,false));
    double sumlch=lextor_data->get_lexchoice_sum(reduced_lexchoice);


    if (debug) {
      cerr<<"sum("<<reduced_lexchoice<<") = "<<sumlch<<"\n";
    }

    if (sumlch>greatest_sum) {
      greatest_sum=sumlch;
      winner=i;
    }
  }

  if (greatest_sum==0)
    winner=-1;

  if (debug) 
    cerr<<"WINNER: "<<winner<<" "<<window[word_index].get_lexical_choice(winner)<<"\n";

  return winner;
}

int 
LexTor::estimate_winner_lch_cosine(deque<LexTorWord>& window, int word_index, double weigth_exponent) {
  map<string, double> vcontext;

  int distance=(-1)*(word_index);
  for(int i=0; i<(int)window.size(); i++) {
    if (i!=word_index) {
      string reduced_word=lextor_data->reduce(window[i].get_word_string());
      vcontext[reduced_word]+=1.0/pow(fabs((double)distance),weigth_exponent);
    }
    distance++;
  }

  if (debug) {
    cerr<<"CONTEXT VECTOR\n-------------------\n";
    map<string, double>::iterator it;
    for(it=vcontext.begin(); it!=vcontext.end(); it++)
      cerr<<it->first<<", "<<it->second<<"\n";
  }

  ////double max_cosine=-2;
  double min_angle=360;
  int winner=-1;
  double diff_angle=-1;
  for(int i=0; i<window[word_index].n_lexical_choices(); i++) {
    string reduced_lexchoice=lextor_data->reduce_lexical_choice(window[word_index].get_lexical_choice(i,false));

    double aux_cosine=cosine(vcontext, reduced_lexchoice);
    double aux_angle=(acos(aux_cosine)*180)/PI;
    if (debug) {
      cerr<<"cos("<<lextor_data->reduce(window[word_index].get_word_string())<<", "
	  <<reduced_lexchoice<<") = "<<aux_cosine<<"; ang = "<<aux_angle<<" grades\n";
    }

    if (aux_angle<min_angle) {
      if (min_angle!=360) {
	diff_angle=min_angle-aux_angle;
      }
      min_angle=aux_angle;
      winner=i;
    } else if ((min_angle!=360)&&(diff_angle==-1)) {
      diff_angle=fabs(min_angle-aux_angle);
    }


    /*
      if (aux_cosine>max_cosine) {
      diff_angle=abs(min_angle-aux_angle);
      winner=i;
      max_cosine=aux_cosine;
      min_angle=aux_angle;
      }
    */
  }

  if (debug) {
    cerr<<"DIFF ANGLE: "<<diff_angle<<"\n";
  }
  if (diff_angle<=angleth)
    winner=-1;

  if (debug) 
    cerr<<"WINNER: "<<winner<<" "<<window[word_index].get_lexical_choice(winner)<<"\n";
  
  return winner;
}

int 
LexTor::estimate_winner_lch_votingtl(deque<LexTorWord>& window, int word_index, double weigth_exponent) {
  if (tlmodel==NULL) {
    cerr<<"Error in LexTor::estimate_winner_lch_votingtl: you must call LexTor::set_tlmodel before.\n";
    exit(EXIT_FAILURE);
  }  

  double lexchoices_count[window[word_index].n_lexical_choices()];
  vector<string> translation_window[window.size()];
  string reduced_window[window.size()];

  for (unsigned i=0; i<window.size(); i++) 
    reduced_window[i]=lextor_data->reduce(window[i].get_word_string());	      
  
  if(debug) {
    cerr<<"WINDOW: ";
    for (unsigned i=0; i<window.size(); i++) {
      if(i==(unsigned)word_index)
	cerr<<"[>>>>"<<reduced_window[i]<<"<<<<] ";
      else
	cerr<<"["<<reduced_window[i]<<"] ";
    }
    cerr<<"\n";
    cerr<<"TRANSLATED: ";
  }

  //We translate each word in the context
  //Note: Words in the context can also be ambiguous (with more than one lexical choice)
  for (unsigned i=0; i<window.size(); i++) {
    string str_translations="";
    for(int j=0; j<window[i].n_lexical_choices(); j++) {
      string tr=tlmodel->reduce(window[i].translate(*fstpbil,j));
      translation_window[i].push_back(tr);
      str_translations+=tr+"/";
    }
    if (debug) {
      if (i==(unsigned)word_index)
	cerr<<"[>>>>"<<str_translations<<"<<<<] ";
      else
	cerr<<"["<<str_translations<<"] ";
    }
  }

  if(debug)
    cerr<<"\n";

  //For each lexical choice the counts from the TL are collected
  for(unsigned i=0; i<(unsigned)window[word_index].n_lexical_choices(); i++) {
    lexchoices_count[i]=0;

    for (unsigned k=0; k<window.size(); k++) {
      if ((k!=(unsigned)word_index)&&(reduced_window[k]!=reduced_window[word_index])) {
	COUNT_DATA_TYPE target_vote=0;

	//If the SL word is ambiguous it will have more than one
	//translation into TL, so we need to normalize using the
	//frequency of words in the TL
	double translation_weighs[translation_window[k].size()];
	double sum=0.0;
	if (translation_window[k].size()>1) {
	  for(unsigned j=0; j<translation_window[k].size(); j++) {
	    translation_weighs[j]=tlmodel->get_lexchoice_sum(translation_window[k][j]);
	    sum+=translation_weighs[j];

	    //!!!!! Para hacer que no tenga en cuenta las
	    //!!!!! polisemicas del contexto
	    ///////translation_weighs[j]=0;
	    //!!!!!
	    //!!!!!

	    if (debug) {
	      cerr<<"Frequency of translation ["<<translation_window[k][j]<<"] ="
		  <<translation_weighs[j]<<"\n";
	    }
	  }
	} else {
	  translation_weighs[0]=1;
	  sum=1;
	}

	for(unsigned j=0; j<translation_window[k].size(); j++) {
	  translation_weighs[j]=translation_weighs[j]/sum;
	  if (debug) {
	    cerr<<"Weight of translation ["<<translation_window[k][j]<<"] ="
		<<translation_weighs[j]<<"\n";
	  }
	}

	for(unsigned j=0; j<translation_window[k].size(); j++) {
	  double aux_vote=0;
	  //aux_vote=tlwordmodel.vote_from_word(lexchoice_translation[*itlch], 
	  //				    translation_buffer[i][j])*translation_weighs[j];
	  if(debug) 
	    cerr<<translation_window[word_index][i]<<" "<<translation_window[k][j]<<" "
		<<tlmodel->vote_from_word(translation_window[word_index][i],translation_window[k][j])<<" "
		<<tlmodel->get_wordcount(translation_window[k][j])<<" "<<translation_weighs[j]<<"\n";
	
	  if (tlmodel->get_wordcount(translation_window[k][j])>0) {
	    aux_vote=(tlmodel->vote_from_word(translation_window[word_index][i],translation_window[k][j])/
		      tlmodel->get_wordcount(translation_window[k][j]))*translation_weighs[j];
	  } 
	  target_vote+=aux_vote;

	  if(debug) {
	    cerr<<"Target vote for ["<<translation_window[word_index][i]
		<<"] from ["<<translation_window[k][j]<<"] = "<<aux_vote<<"\n";
	  }
	}

	lexchoices_count[i]+=target_vote;
      }
    }
  }


  if(debug) {
    for(int i=0; i<window[word_index].n_lexical_choices(); i++) 
      cerr<<"lexchoicecount["<<i<<"] = "<<lexchoices_count[i]<<"\n";
    //getchar();
  }

  //Now the winner is calculated
  int winner=-1; //This will make the default one to be used if unchanged
  COUNT_DATA_TYPE winner_vote=-100000000;
  for(int i=0; i<window[word_index].n_lexical_choices(); i++) {
    if ((lexchoices_count[i]>0) && (lexchoices_count[i]>winner_vote)) {
      winner_vote=lexchoices_count[i];
      winner=i;
    } 
  }

  if (debug) 
    cerr<<"WINNER: "<<winner<<" "<<window[word_index].get_lexical_choice(winner)<<"\n";

  return winner;
}

double 
LexTor::cosine(map<string, double>& vcontext, const string& reduced_lexchoice) {
  map<string, double>::iterator itc;

  //We calculate the scalar product between vcontext and the lexchoice vector
  double scalar_product=0;
  for(itc=vcontext.begin(); itc!=vcontext.end(); itc++) {
    scalar_product+=(itc->second)*(lextor_data->vote_from_word(reduced_lexchoice, itc->first));
  }

  //We calculate the module of vcontext, ||vcontext||
  double module_vcontext=0;
  for(itc=vcontext.begin(); itc!=vcontext.end(); itc++) {
    module_vcontext+=(itc->second)*(itc->second);
  }
  module_vcontext=sqrt(module_vcontext);

  //We get the module of the lexchoice vector, ||lexchoice vector||
  double module_lexchoice_vector=lextor_data->get_module_lexchoice_vector(reduced_lexchoice);

  if (module_vcontext==0) { 
    cerr<<"Error in LexTor::vectors_cosine: module_vcontext is equal zero.\n"
	<<"The cosine cannot be compute\n";
    if (debug) {
      cerr<<"CONTEXT VECTOR\n-------------------\n";
      map<string, double>::iterator it;
      for(it=vcontext.begin(); it!=vcontext.end(); it++)
	cerr<<it->first<<", "<<it->second<<"\n";
    }

    return -2;
    //exit(EXIT_FAILURE);
  }

  if (module_lexchoice_vector==0) {
    if (debug) {
      cerr<<"Warning in LexTor::vectors_cosine: module_lexchoice_vector is equal zero.\n"
	  <<"The cosine cannot be compute\n";
      cerr<<"reduced lexical choice: "<<reduced_lexchoice<<"\n";
    }
    return -2;
  }

  return scalar_product/(module_vcontext*module_lexchoice_vector);
}
