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

#include <apertium/LexTorData.H>
#include <apertium/StringUtils.H>
#include <apertium/LexTorWord.H>
#include <apertium/LexTor.H>

#include <lttoolbox/Compression.H>
#include <lttoolbox/EndianUtil.H>

LexTorData::LexTorData() {
  n_stopwords=0;
  n_words=0;
  n_words_per_set=0;
  n_set=0;

  index2word.push_back(NULLWORD);
  word2index[NULLWORD]=0;
  n_words++;
}
  
LexTorData::LexTorData(const LexTorData& ltd) {
  n_stopwords=ltd.n_stopwords;
  n_words=ltd.n_words;
  n_words_per_set=ltd.n_words_per_set;
  n_set=ltd.n_set;

  word2index=ltd.word2index;
  index2word=ltd.index2word;

  lexchoice_set=ltd.lexchoice_set;
  lexchoice_sum=ltd.lexchoice_sum;
  //lexchoice_prob=ltd.lexchoice_prob;

  stopwords=ltd.stopwords;
  words=ltd.words;
  lexical_choices=ltd.lexical_choices;
  reduced_lexical_choices=ltd.reduced_lexical_choices;
}
  
LexTorData::~LexTorData() {
}

COUNT_DATA_TYPE
LexTorData::vote_from_word(const string& lexical_choice, const string& word) {
  WORD_DATA_TYPE ind_lexchoice=word2index[StringUtils::tolower(lexical_choice)];
  WORD_DATA_TYPE ind_word=word2index[StringUtils::tolower(word)];

  //To avoid creating a null entry in lexchoice_set[lexical_choice]
  if (lexchoice_set[ind_lexchoice].find(ind_word)==lexchoice_set[ind_lexchoice].end())
    return 0;
  else
    return lexchoice_set[ind_lexchoice][ind_word];
}

//double 
//LexTorData::get_lexchoice_prob(const string& lexical_choice) {
//  return lexchoice_prob[word2index[lexical_choice]];
//}


void 
LexTorData::set_wordcount(const string& word, COUNT_DATA_TYPE c) {
  WORD_DATA_TYPE ind_word=word2index[StringUtils::tolower(word)];
  wordcount[ind_word]=c;
}

COUNT_DATA_TYPE 
LexTorData::get_wordcount(const string& word) {
  WORD_DATA_TYPE ind_word=word2index[StringUtils::tolower(word)];

  if (wordcount.find(ind_word)==wordcount.end())
    return 0;
  else
    return wordcount[ind_word];
}

COUNT_DATA_TYPE
LexTorData::get_lexchoice_sum(const string& lexical_choice) {
  return lexchoice_sum[word2index[StringUtils::tolower(lexical_choice)]];
}

void 
LexTorData::set_lexchoice_sum(const string& lexical_choice, COUNT_DATA_TYPE sum) {
  lexchoice_sum[word2index[StringUtils::tolower(lexical_choice)]]=sum;
}

bool
LexTorData::is_stopword(const string& word) {
  return (stopwords.find(StringUtils::tolower(word))!=stopwords.end());
}

void 
LexTorData::read(istream& is) {
  //cerr<<"LexTorData::read------------------------------------\n";
  n_stopwords=(WORD_DATA_TYPE)Compression::multibyte_read(is);
  n_words=(WORD_DATA_TYPE)Compression::multibyte_read(is);
  n_words_per_set=(WORD_DATA_TYPE)Compression::multibyte_read(is);
  n_set=(WORD_DATA_TYPE)Compression::multibyte_read(is);

  //cerr<<n_stopwords<<"\n";
  //cerr<<n_words<<"\n";
  //cerr<<n_words_per_set<<"\n";
  //cerr<<n_set<<"\n";


  //Read the set of stopwords
  //cerr<<"stopwords--------------------------------------------\n";
  for (unsigned i=0; i<n_stopwords; i++) {
    WORD_DATA_TYPE len;
    char str[1024];

    len=(WORD_DATA_TYPE)Compression::multibyte_read(is);
    is.read(str, len*sizeof(char));
    str[len]='\0';
    stopwords.insert(str);
    //cerr<<"len: "<<len<<" str: "<<str<<"\n";
  }

  //Read the list of words
  //cerr<<"list of words----------------------------------------\n";
  for(unsigned i=1; i<n_words; i++) {
    WORD_DATA_TYPE len;
    char str[1024];

    len=(WORD_DATA_TYPE)Compression::multibyte_read(is);
    is.read(str, len*sizeof(char));
    str[len]='\0';
    index2word.push_back(str);
    word2index[str]=i;
    wordcount[i]=EndianUtil<COUNT_DATA_TYPE>::read(is);
    //cerr<<"len: "<<len<<" str: "<<str<<" index: "<<i<<" word_count: "<<wordcount[i]<<"\n";
  }

  //Read data of each set associate to each lexical choice (or word)
  for(unsigned i=0; i<n_set; i++) {
    WORD_DATA_TYPE lexchoice;
    COUNT_DATA_TYPE sum;
    //double prob;

    lexchoice=(WORD_DATA_TYPE)Compression::multibyte_read(is);
    sum=EndianUtil<COUNT_DATA_TYPE>::read(is);

    //cerr<<"lexchoice: "<<lexchoice<<" sum: "<<sum<<" "<<index2word[lexchoice]<<"\n";

    reduced_lexical_choices.insert(index2word[lexchoice]);

    lexchoice_sum[lexchoice]=sum;
    //lexchoice_prob[lexchoice]=prob;

    /////lexical_choices.insert(index2word[lexchoice]);

    for(unsigned j=0; j<n_words_per_set; j++) {
      WORD_DATA_TYPE word;
      COUNT_DATA_TYPE count;

      word=(WORD_DATA_TYPE)Compression::multibyte_read(is);
      count=EndianUtil<COUNT_DATA_TYPE>::read(is);
      //cerr<<"     word: "<<word<<" count: "<<count<<"\n";
      lexchoice_set[lexchoice][word]=count;
    }
  }

  //First we read the number of words to take into account
  WORD_DATA_TYPE nwords2workwith;

  nwords2workwith=(WORD_DATA_TYPE)Compression::multibyte_read(is);
  for (unsigned i=0; i<nwords2workwith; i++) {
    WORD_DATA_TYPE word;

    word=(WORD_DATA_TYPE)Compression::multibyte_read(is);
    words.insert(index2word[word]);
    //cerr<<"word: "<<index2word[word]<<"\n";
  }
}

void 
LexTorData::write(ostream& os) {
  //cerr<<"LexTorData::write------------------------------------\n";
  //cerr<<n_stopwords<<"\n";
  //cerr<<n_words<<"\n";
  //cerr<<n_words_per_set<<"\n";
  //cerr<<n_set<<"\n";
  Compression::multibyte_write(n_stopwords, os);
  Compression::multibyte_write(n_words, os);
  Compression::multibyte_write(n_words_per_set, os);
  Compression::multibyte_write(n_set, os);

  //Write the set of stopwords
  //cerr<<"stopwords--------------------------------------------\n";
  set<string>::iterator it;
  for (it=stopwords.begin(); it!=stopwords.end(); it++) {
    WORD_DATA_TYPE len=(WORD_DATA_TYPE) it->length();
    //cerr<<"len: "<<len<<" str: "<<*it<<"\n";
    Compression::multibyte_write(len, os);
    os.write(it->c_str(), len*sizeof(char));
  }

  //Write the list of words
  //cerr<<"list of words----------------------------------------\n";
  for(unsigned i=1; i<index2word.size(); i++) {
    WORD_DATA_TYPE len=index2word[i].length();
    //cerr<<"len: "<<len<<" str: "<<index2word[i]<<" index: "<<i<<" word_count: "<<wordcount[i]<<"\n";
    Compression::multibyte_write(len, os);
    os.write(index2word[i].c_str(), len*sizeof(char));
    EndianUtil<COUNT_DATA_TYPE>::write(os, wordcount[i]);
  }

  //Write data of each set associate to each lexical choice (or word)
  map<WORD_DATA_TYPE, map<WORD_DATA_TYPE, COUNT_DATA_TYPE> >::iterator it_lch_set;
  map<WORD_DATA_TYPE, COUNT_DATA_TYPE>::iterator it_w_lch_set;
  //map<WORD_DATA_TYPE, double>::iterator it_lch_prob;

  for(it_lch_set=lexchoice_set.begin(); it_lch_set!=lexchoice_set.end(); it_lch_set++) {
    WORD_DATA_TYPE lexchoice=it_lch_set->first;
    COUNT_DATA_TYPE sum=lexchoice_sum[lexchoice];
    //double prob=lexchoice_prob[lexchoice];

    //cerr<<"lexchoice: "<<lexchoice<<" sum: "<<sum<<" "<<index2word[lexchoice]<<"\n";
    Compression::multibyte_write(lexchoice, os);    
    //os.write(reinterpret_cast<char * const> (&prob), sizeof(double));
    EndianUtil<COUNT_DATA_TYPE>::write(os, sum);

    int nwritten_words=0;
    for(it_w_lch_set=it_lch_set->second.begin(); 
        it_w_lch_set!=it_lch_set->second.end(); 
        it_w_lch_set++) {
      WORD_DATA_TYPE word=it_w_lch_set->first;
      COUNT_DATA_TYPE count=it_w_lch_set->second;
      //cerr<<"     word: "<<word<<" count: "<<count<<"\n";
      Compression::multibyte_write(word, os);
      EndianUtil<COUNT_DATA_TYPE>::write(os, count);
      nwritten_words++;
    }

    //If there were less written words than expected
    while (nwritten_words<n_words_per_set){
      WORD_DATA_TYPE word=word2index[NULLWORD];
      COUNT_DATA_TYPE count=0;
      //cerr<<"     word: "<<word<<" count: "<<count<<"\n";
      Compression::multibyte_write(word, os);
      EndianUtil<COUNT_DATA_TYPE>::write(os, count);
      nwritten_words++;
    }
  }

  //First we write the number of words to take into account
  WORD_DATA_TYPE nwords2workwith=words.size();
  Compression::multibyte_write(nwords2workwith, os);

  set<string>::iterator sit;
  for(sit=words.begin(); sit!=words.end(); sit++) {
    WORD_DATA_TYPE word=word2index[*sit];
    Compression::multibyte_write(word, os);
    //cerr<<"word: "<<*sit<<"\n";
  }
}

void 
LexTorData::read_stopwords(istream& is) {
  while (!is.eof()) {
    string w;
    getline(is,w);
    w=StringUtils::tolower(w);
    if (w.length()>0) {
      stopwords.insert(w);
      cerr<<"stopword: "<<w<<"\n";
    }
  }
  n_stopwords=stopwords.size();
  cerr<<"# stopwords read from file: "<<n_stopwords<<"\n";
}  

void 
LexTorData::read_words(istream& is) {
  while(!is.eof()) {
    string w;
    getline(is,w);
    w=StringUtils::tolower(w);
    if (w.length()>0) {
      words.insert(w);
      new_word_register(w);
    }
  }
  n_set=words.size();
  cerr<<"# words: "<<n_set<<"\n";
}

void 
LexTorData::read_lexical_choices(FSTProcessor& fstp) {
  set<string>::iterator it;
  int nlexchoices=0;

  for(it=words.begin(); it!=words.end(); it++) {
    LexTorWord ambiguousword(*it, &fstp);
    nlexchoices+=ambiguousword.n_lexical_choices();

    for(int i=0; i<ambiguousword.n_lexical_choices(); i++) {
      lexical_choices[*it].insert(ambiguousword.get_lexical_choice(i,false));
      //lexical_choices[*it].insert(reduce_lexical_choice(ambiguousword.get_lexical_choice(i,false)));
    }
  }

  n_set=nlexchoices;

  cerr<<"# lexical choices: "<<n_set<<"\n";
}

set<string>
LexTorData::get_words() {
  return words;
}

set<string> 
LexTorData::get_lexical_choices(const string& word) {
  return lexical_choices[StringUtils::tolower(word)];
}

void 
LexTorData::set_nwords_per_set(int i){
  n_words_per_set=i;
  cerr<<"# words per co-ocurrence model: "<<n_words_per_set<<"\n";
}

void 
LexTorData::set_cooccurrence_context(const string& lexical_choice, 
                                     const vector<pair<string, COUNT_DATA_TYPE> >& context) {
  cerr<<"Co-occurrence model for lexical_choice/word: "<<lexical_choice<<"\n";

  if (context.size()==0) {
    cerr<<"Warning: co-occurrence model for lexical_choice/word: "<<lexical_choice<<" is empty\n";
    cerr<<"It seems that training corpus is too small or thematically homogeneous\n";
    n_set--;
  }

  new_word_register(lexical_choice);

  for (unsigned i=0; ((i<n_words_per_set)&&(i<context.size())); i++) {
    cerr<<context[i].first<<" "<<context[i].second<<"\n";

    new_word_register(context[i].first);

    lexchoice_set[word2index[StringUtils::tolower(lexical_choice)]][word2index[StringUtils::tolower(context[i].first)]]=context[i].second;

    //////wordcount[word2index[StringUtils::tolower(context[i].first)]]+=context[i].second;
  }
}

void
LexTorData::ensure_stopwords_ok() {
  set<string>::iterator its, itw;
  set<string> swaux;

  //Notice that stopwords consist of lemma and first tag while words
  //consist of lemma and one (the first one) or more tags

  for(its=stopwords.begin(); its!=stopwords.end(); its++) {
    bool is_ok=true;
    for(itw=words.begin(); itw!=words.end(); itw++) {
      //cerr<<"sw: "<<*its<<" w: "<<*itw<<"\n";
      if (itw->find(*its)==0) {
	cerr<<"Warning: Word '"<<*itw<<"' for which co-ocurrence models will"
	    <<" be estimated is also a stopword. ";
	cerr<<"Removing it from the stopwords list\n";
	is_ok=false;
	break;
      }
    }
    if(is_ok)
      swaux.insert(*its);
  }

  stopwords=swaux;

  cerr<<n_stopwords-stopwords.size()<<" stopwords were discarded\n";

  n_stopwords=stopwords.size();

  cerr<<"# stopwords finally taken into account: "<<n_stopwords<<"\n";
}

string 
LexTorData::reduce(const string& s) {
  string str;

  if ((s.length()>0) && (s[0]=='^') && (s[s.length()-1]=='$'))
    str=StringUtils::tolower(s.substr(1, s.length()-1));
  else
    str=StringUtils::tolower(s);

  set<string>::iterator it;
  for(it=words.begin(); it!=words.end(); it++) {
    if (str.find(*it)==0) {
      return (*it);
    }
  }

  unsigned p=str.find(">");
  unsigned i=0;
  if (p==string::npos) { //s could correspond to an unknown word
    p=str.length();
    if ((str.length()>0) && (str[0]=='*'))
      i=1; // to remove the star (unknown word mark)
  }
  else
    p++;
  
  if (i>=p) {
    cerr<<"Warning in LexTorData::reduce: input string: '"<<s<<"', string after operation: '"<<str<<"'\n";
    cerr<<"begin index: "<<i<<", end index: "<<p<<"\n";
    i=0;
  }

  return str.substr(i,p);
}

string 
LexTorData::reduce_lexical_choice(const string& s) {
  string str;

  if ((s.length()>0) && (s[0]=='^') && (s[s.length()-1]=='$'))
    str=StringUtils::tolower(s.substr(1, s.length()-1));
  else
    str=StringUtils::tolower(s);

  set<string>::iterator it;
  for(it=reduced_lexical_choices.begin(); it!=reduced_lexical_choices.end(); it++) {
    if (str.find(*it)==0) {
      return (*it);
    }
  }

  //return StringUtils::substitute(str," d<", " D<");

  return str;
}

void 
LexTorData::new_word_register(const string& word) {
  string w=StringUtils::tolower(word);

  if (word2index.find(w)==word2index.end()) {
    index2word.push_back(w);
    int ind=index2word.size()-1;
    if (ind>MAX_WORD_INDEX) {
      cerr<<"Error: The number of words to be considered is greater that the maximum allowed by\n";
      cerr<<"the data type used to store words\n";
      cerr<<"Edit source file LexTorData.H and change the WORD_DATA_TYPE define\n";
      exit(EXIT_FAILURE);
    }
    word2index[w]=(WORD_DATA_TYPE)ind;
    n_words=index2word.size();
    wordcount[(WORD_DATA_TYPE)ind]=0;
  }
}

/*
vector<pair<WORD_DATA_TYPE, double> > 
LexTorData::get_cooccurrence_vector(const string& lexical_choice) {
  vector<pair<WORD_DATA_TYPE, double> > v;
  WORD_DATA_TYPE ind_lexchoice=word2index[StringUtils::tolower(lexical_choice)];
  map<WORD_DATA_TYPE, COUNT_DATA_TYPE>::iterator it;

  for(it=lexchoice_set[ind_lexchoice].begin(); it!= lexchoice_set[ind_lexchoice].end(); it++) 
    v.push_back(*it);
  
  return v;
}
*/


double 
LexTorData::get_module_lexchoice_vector(const string& lexical_choice) {
  WORD_DATA_TYPE ind_lexchoice=word2index[StringUtils::tolower(lexical_choice)];
  map<WORD_DATA_TYPE, COUNT_DATA_TYPE>::iterator it;

  double module=0;

  for(it=lexchoice_set[ind_lexchoice].begin(); it!= lexchoice_set[ind_lexchoice].end(); it++) 
    module+=(it->second)*(it->second);

  module=sqrt(module);

  return module;
}

double 
LexTorData::cosine(const string& reduced_lexch1, const string& reduced_lexch2) {
  WORD_DATA_TYPE ind_lexchoice1=word2index[StringUtils::tolower(reduced_lexch1)];
  WORD_DATA_TYPE ind_lexchoice2=word2index[StringUtils::tolower(reduced_lexch2)];
  map<WORD_DATA_TYPE, COUNT_DATA_TYPE>::iterator it;

  //We calculate the scalar product
  double scalar_product=0;
  for(it=lexchoice_set[ind_lexchoice1].begin(); it!= lexchoice_set[ind_lexchoice1].end(); it++) {
    if (lexchoice_set[ind_lexchoice2].find(it->first)!=
        lexchoice_set[ind_lexchoice2].end()) {
      scalar_product+=(it->second)*lexchoice_set[ind_lexchoice2][it->first];
    }
  }

  //We get the module of the lexchoice vectors, ||lexchoice vector||
  double module_lexch1_vector=get_module_lexchoice_vector(reduced_lexch1);
  double module_lexch2_vector=get_module_lexchoice_vector(reduced_lexch2);


  if (module_lexch1_vector==0) {
    if (LexTor::debug) {
      cerr<<"Warning in LexTorData::cosine: module_lexch1_vector is equal zero.\n"
	  <<"The cosine cannot be compute\n";
      cerr<<"reduced lexical choice: "<<reduced_lexch1<<"\n";
    }
    return -2;
  }

  if (module_lexch2_vector==0) {
    if (LexTor::debug) {
      cerr<<"Warning in LexTorData::cosine: module_lexch2_vector is equal zero.\n"
	  <<"The cosine cannot be compute\n";
      cerr<<"reduced lexical choice: "<<reduced_lexch2<<"\n";
    }
    return -2;
  }

  return scalar_product/(module_lexch1_vector*module_lexch2_vector);
}
