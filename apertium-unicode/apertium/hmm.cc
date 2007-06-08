/*
 * Copyright (C) 2005 Universitat d'Alacant / Universidad de Alicante
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
/*
 *  First order hidden Markov model (HMM) implementation (source)
 *
 *  @author	Felipe S�nchez-Mart�nez - fsanchez@dlsi.ua.es
 */

#include <apertium/hmm.h>
#include <apertium/tagger_utils.h>
#include <lttoolbox/endian_util.h>

#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <algorithm>

HMM::HMM(TaggerData *t)
{
  this->td = t;

  debug=false;
  eos = (td->getTagIndex())[L"TAG_SENT"];  
}

HMM::~HMM()
{
}

void
HMM::init()
{
}

void
HMM::set_eos(TTag t) 
{ 
  eos = t; 
} 

void
HMM::set_debug(bool d)
{ 
  debug = d; 
} 

void 
HMM::read_ambiguity_classes(FILE *in) 
{
  while(in)
  {
    int ntags = EndianUtil<int>::read(in);

    if(feof(in))
    {
      break;
    }
    set<TTag> ambiguity_class;

    for(; ntags != 0; ntags--)
    {
      ambiguity_class.insert(EndianUtil<TTag>::read(in));
    }
    
    if(ambiguity_class.size() != 0)
    {
      td->getOutput().add(ambiguity_class);
    }     
  }
  
  td->setProbabilities(td->getTagIndex().size(), td->getOutput().size());
}

void 
HMM::write_ambiguity_classes(FILE *out) 
{
  for(int i=0, limit = td->getOutput().size(); i != limit; i++) 
  {
    set<TTag> const &ac = (td->getOutput())[i];
    EndianUtil<int>::write(out, ac.size());
    for(set<TTag>::const_iterator it = ac.begin(), limit2 = ac.end();
        it != limit2; it++)
    {
      EndianUtil<TTag>::write(out, *it);
    }
  } 
}  

void 
HMM::read_probabilities(FILE *in)
{
  td->read(in);
}

void 
HMM::write_probabilities(FILE *out)
{
  td->write(out);  
}  

void 
HMM::init_probabilities_kupiec (FILE *is)
{
  int N = td->getN();
  int M = td->getM();
  int i, j, k, k1, k2, nw=0;
  double classes_ocurrences[M]; //M = Number of ambiguity classes
  double classes_pair_ocurrences[M][M];
  double tags_estimate[N]; //N = Number of tags (states)
  double tags_pair_estimate[N][N];
  Collection &output = td->getOutput();
 
  MorphoStream lexmorfo(is, true, td);
  
  TaggerWord *word=NULL;

  for(k=0; k<M; k++) {
    classes_ocurrences[k]=1; 
    for (k2=0; k2<M; k2++)
      classes_pair_ocurrences[k][k2]=1;
  }

  set<TTag> tags;
  tags.insert(eos);  
  k1=output[tags]; //The first tag (ambiguity class) seen is the end-of-sentence
  classes_ocurrences[k]++;
  
  //We count for each ambiguity class the number of ocurrences
  word = lexmorfo.get_next_word();
  while((word)) {
    if (++nw%10000==0) cerr<<'.'<<flush; 
    
    tags=word->get_tags();

    if (tags.size()==0) { //This is an unknown word
      tags = td->getOpenClass();
    }
    else if (output.has_not(tags)) { 
      wstring errors;
      errors = L"A new ambiguity class was found. I cannot continue.\n";
      errors+= L"Word '"+word->get_superficial_form()+L"' not found in the dictionary.\n";
      errors+= L"New ambiguity class: "+word->get_string_tags()+L"\n";
      errors+= L"Take a look at the dictionary and at the training corpus. Then, retrain.";      
      fatal_error(errors);      
    }    

    k2=output[tags];

    classes_ocurrences[k1]++;
    classes_pair_ocurrences[k1][k2]++;  //k1 followed by k2
    delete word;
    word=lexmorfo.get_next_word();

    k1=k2;

  }  

  //Estimation of the number of time each tags occurs in the training text
  for(i=0; i<N; i++) {  
    tags_estimate[i]=0;
    for(k=0; k<M;  k++) { 
  
      if(output[k].find(i) != output[k].end())
        tags_estimate[i] += classes_ocurrences[k]/output[k].size();	
    }
  }
  
  //Estimation of the number of times each tag pair occurs
  for(i=0; i<N; i++)
    for(j=0; j<N; j++)
      tags_pair_estimate[i][j]=0;

  set<TTag> tags1, tags2;
  set<TTag>::iterator itag1, itag2;
  for(k1=0; k1<M; k1++) {
    tags1=output[k1];
    for(k2=0; k2<M; k2++) {
      tags2=output[k2];
      double nocurrences=classes_pair_ocurrences[k1][k2]/((double)(tags1.size()*tags2.size()));
      for (itag1=tags1.begin(); itag1!=tags1.end(); itag1++) {
        for (itag2=tags2.begin(); itag2!=tags2.end(); itag2++)
          tags_pair_estimate[*itag1][*itag2]+=nocurrences;
      }
    }
  }

   //a[i][j] estimation.
  double sum;
  for(i=0; i<N; i++) {
    sum=0;
    for(j=0; j<N; j++)
      sum+=tags_pair_estimate[i][j];

    for(j=0; j<N; j++) {  
      if (sum>0)
        (td->getA())[i][j] = tags_pair_estimate[i][j]/sum;
      else {
        (td->getA())[i][j] = 0;
      }
    }
  }

  //b[i][k] estimation
  for(i=0; i<N; i++) {
    for(k=0; k<M; k++)  {
      if (output[k].find(i)!=output[k].end()) {
        if (tags_estimate[i]>0)
          (td->getB())[i][k] = (classes_ocurrences[k]/output[k].size())/tags_estimate[i];
        else 
	  (td->getB())[i][k] = 0;
      }
    }
  }
  cerr<<"\n";
}

void 
HMM::init_probabilities_from_tagged_text(FILE *ftagged, FILE *funtagged) {
  int i, j, k, nw=0;
  int N = td->getN();
  int M = td->getM();
  double tags_pair[N][N];
  double emission[N][M];
  
  MorphoStream stream_tagged(ftagged, true, td);
  MorphoStream stream_untagged(funtagged, true, td);
  
  TaggerWord *word_tagged=NULL, *word_untagged=NULL;
  Collection &output = td->getOutput();

  
  set<TTag> tags;
 
  // Init counters - each event appears at least once. 
  // Espected likelihood estimate (ELE) with a fixed initial count of 1
  for(i=0; i<N; i++) {
    for(j=0; j<N; j++)
      tags_pair[i][j]=0;
  }
  for(k=0; k<M; k++) {
    for(i=0; i<N; i++) {
      if (output[k].find(i)!=output[k].end())
        emission[i][k] = 0;
    }  
  }
 
  TTag tag1, tag2;  
  tag1 = eos; // The first seen tag is the end-of-sentence tag
  
  word_tagged = stream_tagged.get_next_word();
  word_untagged = stream_untagged.get_next_word();
  while(word_tagged) {
    wcerr<<*word_tagged;
    wcerr<<L" -- "<<*word_untagged<<L"\n"; 

    if (word_tagged->get_superficial_form()!=word_untagged->get_superficial_form()) {              
      wcerr<<L"\nTagged text (.tagged) and analyzed text (.untagged) streams are not aligned.\n";
      wcerr<<L"Take a look at tagged text (.tagged).\n";
      wcerr<<L"Perhaps this is caused by a multiword unit that is not a multiword unit in one of the two files.\n";
      wcerr<<*word_tagged<<L" -- "<<*word_untagged<<L"\n"; 
      exit(1);
    }

    if (++nw%100==0) wcerr<<L'.'<<flush; 
    
    tag2 = tag1;
   
    if (word_untagged==NULL) {
      wcerr<<L"word_untagged==NULL\n";
      exit(1);
    }

    if (word_tagged->get_tags().size()==0) // Unknown word
      tag1 = -1;
    else if (word_tagged->get_tags().size()>1) // Ambiguous word
      wcerr<<L"Error in tagged text. An ambiguous word was found: "<<word_tagged->get_superficial_form()<<L"\n";
    else
      tag1 = *(word_tagged->get_tags()).begin();


    if ((tag1>=0) && (tag2>=0))
      tags_pair[tag2][tag1]++;
    

    if (word_untagged->get_tags().size()==0) { // Unknown word
      tags = td->getOpenClass();
    }
    else if (output.has_not(word_untagged->get_tags())) { //We are training, there is no problem
      wstring errors;
      errors = L"A new ambiguity class was found. I cannot continue.\n";
      errors+= L"Word '"+word_untagged->get_superficial_form()+L"' not found in the dictionary.\n";
      errors+= L"New ambiguity class: "+word_untagged->get_string_tags()+L"\n";
      errors+= L"Take a look at the dictionary, then retrain.";
      fatal_error(errors);      
    }    
    else {
      tags = word_untagged->get_tags();
    }

    k=output[tags];
    if(tag1>=0)
      emission[tag1][k]++;
                   
    delete word_tagged;
    word_tagged=stream_tagged.get_next_word();
    delete word_untagged;
    word_untagged=stream_untagged.get_next_word();       
  }
  
  
  //Estimate of a[i][j]
  for(i=0; i<N; i++) {
    double sum=0;
    for(j=0; j<N; j++)  
      sum += tags_pair[i][j]+1.0;
    for(j=0; j<N; j++)  
      (td->getA())[i][j] = (tags_pair[i][j]+1.0)/sum;
  }
    
  
  //Estimate of b[i][k]
  for(i=0; i<N; i++) {
    int nclasses_appear=0;
    double times_appear=0.0;
    for(k=0; k<M; k++)  {
      if (output[k].find(i)!=output[k].end())  {
	nclasses_appear++;	
	times_appear+=emission[i][k];
      }
    }	      
    for(k=0; k<M; k++)  {
      if (output[k].find(i)!=output[k].end())
	(td->getB())[i][k] = (emission[i][k]+(((double)1.0)/((double)nclasses_appear)))/(times_appear+((double)1.0));
    }
   }
  
  wcerr<<L"\n";  
}
  
void
HMM::apply_rules()
{
  vector<TForbidRule> &forbid_rules = td->getForbidRules();
  vector<TEnforceAfterRule> &enforce_rules = td->getEnforceRules();
  int N = td->getN();
  int i, j, j2;
  bool found;
   
  for(i=0; i<(int) forbid_rules.size(); i++) {
    (td->getA())[forbid_rules[i].tagi][forbid_rules[i].tagj] = ZERO;
  }

  for(i=0; i<(int) enforce_rules.size(); i++) {
    for(j=0; j<N; j++) {
      found = false;
      for (j2=0; j2<(int) enforce_rules[i].tagsj.size(); j2++) {
	if (enforce_rules[i].tagsj[j2]==j) {
	  found = true;
	  break;
	}	  
      }
      if (!found)
        (td->getA())[enforce_rules[i].tagi][j] = ZERO;
    }
  }
    
  // Normalize probabilities
  for(i=0; i<N; i++) {
    double sum=0;
    for(j=0; j<N; j++) 
      sum += (td->getA())[i][j];
    for(j=0; j<N; j++) {
      if (sum>0)
	(td->getA())[i][j] = (td->getA())[i][j]/sum;
      else
	(td->getA())[i][j] = 0;
    }
  }
}

void 
HMM::read_dictionary (FILE *fdic) {
  int i, k, nw=0;
  TaggerWord *word=NULL;
  set <TTag> tags;
  Collection &output = td->getOutput();
  
  MorphoStream morpho_stream(fdic, true, td);
  
  // In the input dictionary there must be all punctuation marks, including the end-of-sentece mark
   
  word = morpho_stream.get_next_word();
  
  while (word) {
    if (++nw%10000==0) cerr<<'.'<<flush;
    
    tags = word->get_tags();

    if (tags.size()>0)
      k = output[tags];

    delete word;
    word = morpho_stream.get_next_word();
  }
  cerr<<"\n";
  
  // OPEN AMBIGUITY CLASS
  // It contains all tags that are not closed.
  // Unknown words are assigned the open ambiguity class
  k=output[td->getOpenClass()];

  int N = (td->getTagIndex()).size();  
  
  // Create ambiguity class holding one single tag for each tag.
  // If not created yet
  for(i = 0; i != N; i++) {
    set<TTag> amb_class;
    amb_class.insert(i);
    k=output[amb_class];
  }

  int M = output.size();
  
  cerr<< N <<" states and "<< M <<" ambiguity classes\n";
  td->setProbabilities(N, M);
}

void
HMM::filter_ambiguity_classes(FILE *in, FILE *out) {
  set<set<TTag> > ambiguity_classes;
  MorphoStream morpho_stream(in, true, td);
  
  TaggerWord *word = morpho_stream.get_next_word();
  
  while(word) {
    set<TTag> tags = word->get_tags();

    if(tags.size() > 0) {     
      if(ambiguity_classes.find(tags) == ambiguity_classes.end()) {
	ambiguity_classes.insert(tags);
	word->outputOriginal(out);
	//cerr<<word->get_string_tags()<<"\n";
      }
    }
    delete word;
    word = morpho_stream.get_next_word();
  }
}

void 
HMM::train (FILE *ftxt) {
  int i, j, k, t, len, nw = 0;
  TaggerWord *word=NULL;
  TTag tag; 
  set<TTag> tags, pretags;
  set<TTag>::iterator itag, jtag;
  map <int, double> gamma;
  map <int, double>::iterator jt, kt;
  map < int, map <int, double> > alpha, beta, xsi, phi;
  map < int, map <int, double> >::iterator it;
  double prob, loli;              
  vector < set<TTag> > pending;
  Collection &output = td->getOutput();
  
  int ndesconocidas=0;
  // alpha => forward probabilities
  // beta  => backward probabilities
  
  MorphoStream morpho_stream(ftxt, true, td);

  loli = 0;
  tag = eos;
  tags.clear();
  tags.insert(tag);
  pending.push_back(tags);

  alpha[0].clear();      
  alpha[0][tag] = 1;

  word = morpho_stream.get_next_word();

  while (word) {   

    //cerr<<"Enter para continuar\n";
    //getchar();

    if (++nw%10000==0) cerr<<'.'<<flush;

    //cerr<<*word<<"\n";

    pretags = pending.back();

    tags = word->get_tags();    
    
    if (tags.size()==0) { // This is an unknown word
      tags = td->getOpenClass();
      ndesconocidas++;
    }
    
    if (output.has_not(tags)) {
      wstring errors;
      errors = L"A new ambiguity class was found. I cannot continue.\n";
      errors+= L"Word '"+word->get_superficial_form()+L"' not found in the dictionary.\n";
      errors+= L"New ambiguity class: "+word->get_string_tags()+L"\n";
      errors+= L"Take a look at the dictionary, then retrain.";
      fatal_error(errors);      
    }
    
    k = output[tags];    
    len = pending.size();
    alpha[len].clear();     
      
    //Forward probabilities
    for (itag=tags.begin(); itag!=tags.end(); itag++) {
      i=*itag;
      for (jtag=pretags.begin(); jtag!=pretags.end(); jtag++) {
         j=*jtag;
         //cerr<<"previous alpha["<<len<<"]["<<i<<"]="<<alpha[len][i]<<"\n";
	 //cerr<<"alpha["<<len-1<<"]["<<j<<"]="<<alpha[len-1][j]<<"\n";
         //cerr<<"a["<<j<<"]["<<i<<"]="<<a[j][i]<<"\n";
         //cerr<<"b["<<i<<"]["<<k<<"]="<<b[i][k]<<"\n";
	 alpha[len][i] += alpha[len-1][j]*(td->getA())[j][i]*(td->getB())[i][k];
      }
      if (alpha[len][i]==0)
        alpha[len][i]=DBL_MIN;
      //cerr<<"alpha["<<len<<"]["<<i<<"]="<<alpha[len][i]<<"\n--------\n";
    }

    if (tags.size()>1) {
      pending.push_back(tags);
    } else {  // word is unambiguous
      tag = *tags.begin(); 
      beta[0].clear();
      beta[0][tag] = 1;   
      
      prob = alpha[len][tag];
      
      //cerr<<"prob="<<prob<<"\n";
      //cerr<<"alpha["<<len<<"]["<<tag<<"]="<<alpha[len][tag]<<"\n";
      loli -= log(prob);  
      
      for (t=0; t<len; t++) {  // loop from T-1 to 0	
	  pretags = pending.back();
	  pending.pop_back();
   	  k = output[tags];
	     beta[1-t%2].clear();
	     for (itag=tags.begin(); itag!=tags.end(); itag++) {
	       i=*itag;
	       for (jtag=pretags.begin(); jtag!=pretags.end(); jtag++) {
	         j = *jtag;	      
	         beta[1-t%2][j] += (td->getA())[j][i]*(td->getB())[i][k]*beta[t%2][i];
	         xsi[j][i] += alpha[len-t-1][j]*(td->getA())[j][i]*(td->getB())[i][k]*beta[t%2][i]/prob;
	       }
	       double previous_value = gamma[i];
       
	       gamma[i] +=  alpha[len-t][i]*beta[t%2][i]/prob;		       
	       if (isnan(gamma[i])) {
	          cout<<"NAN(3) gamma["<<i<<"] = "<<gamma[i]<<" alpha["<<len-t<<"]["<<i<<"]= "<<alpha[len-t][i]
	              <<" beta["<<t%2<<"]["<<i<<"] = "<<beta[t%2][i]<<" prob = "<<prob<<" previous gamma = "<<previous_value<<"\n";
	          exit(1);	               
	       }
	       if (isinf(gamma[i])) {
	          cout<<"INF(3) gamma["<<i<<"] = "<<gamma[i]<<" alpha["<<len-t<<"]["<<i<<"]= "<<alpha[len-t][i]
	              <<" beta["<<t%2<<"]["<<i<<"] = "<<beta[t%2][i]<<" prob = "<<prob<<" previous gamma = "<<previous_value<<"\n";
	          exit(1);	               
	       }
	       if (gamma[i]==0) {
	          //cout<<"ZERO(3) gamma["<<i<<"] = "<<gamma[i]<<" alpha["<<len-t<<"]["<<i<<"]= "<<alpha[len-t][i]
	          //    <<" beta["<<t%2<<"]["<<i<<"] = "<<beta[t%2][i]<<" prob = "<<prob<<" previous gamma = "<<previous_value<<"\n";
	          gamma[i]=DBL_MIN;
	          //exit(1);	               
	       }
	        phi[i][k] += alpha[len-t][i]*beta[t%2][i]/prob;
	     }
	     tags=pretags;
      }
	
      tags.clear();
      tags.insert(tag);
      pending.push_back(tags);
      alpha[0].clear();
      alpha[0][tag] = 1;
    }
    
    delete word; 
    word = morpho_stream.get_next_word();
  }  
  if ((pending.size()>1) || ((tag!=eos)&&(tag != (td->getTagIndex())[L"TAG_kEOF"]))) 
    cerr<<"Warning: Thee las tag is not the end-of-sentence-tag\n";
  
  
  int N = td->getN();
  int M = td->getM();
  
  //Clean previous values  
  for(i=0; i<N; i++) {
     for(j=0; j<N; j++)
        (td->getA())[i][j]=ZERO;
     for(k=0; k<M; k++)
        (td->getB())[i][k]=ZERO;
  }
  
  // new parameters
  for (it=xsi.begin(); it!=xsi.end(); it++) {
    i = it->first;
    for (jt=xsi[i].begin(); jt!=xsi[i].end(); jt++) {
      j = jt->first;
      if (xsi[i][j]>0) {        
        (td->getA())[i][j] = xsi[i][j]/gamma[i];
	
        if (isnan((td->getA())[i][j])) {
          cerr <<"Error: BW - NAN(1) a["<<i<<"]["<<j<<"]="<<(td->getA())[i][j]<<"\txsi["<<i<<"]["<<j<<"]="<<xsi[i][j]<<"\tgamma["<<i<<"]="<<gamma[i]<<"\n";
	       exit(1);
        }
	if (isinf((td->getA())[i][j])) {
          cerr <<"Error: BW - INF(1) a["<<i<<"]["<<j<<"]="<<(td->getA())[i][j]<<"\txsi["<<i<<"]["<<j<<"]="<<xsi[i][j]<<"\tgamma["<<i<<"]="<<gamma[i]<<"\n";
	       exit(1);
        }
	if ((td->getA())[i][j]==0) {
          //cerr <<"Error: BW - ZERO(1) a["<<i<<"]["<<j<<"]="<<(td->getA())[i][j]<<"\txsi["<<i<<"]["<<j<<"]="<<xsi[i][j]<<"\tgamma["<<i<<"]="<<gamma[i]<<"\n";
	  //     exit(1);
        }
      }
    }
  }

  for (it=phi.begin(); it!=phi.end(); it++) {
    i = it->first;
    for (kt=phi[i].begin(); kt!=phi[i].end(); kt++) {
      k = kt->first;
      if (phi[i][k]>0) {
        (td->getB())[i][k] = phi[i][k]/gamma[i];	
        
	if (isnan((td->getB())[i][k])) {
          cerr <<"Error: BW - NAN(2) b["<<i<<"]["<<k<<"]="<<(td->getB())[i][k]<<"\tphi["<<i<<"]["<<k<<"]="<<phi[i][k]<<"\tgamma["<<i<<"]="<<gamma[i]<<"\n";
	       exit(1);
        }
	if (isinf((td->getB())[i][k])) {
          cerr <<"Error: BW - INF(2) b["<<i<<"]["<<k<<"]="<<(td->getB())[i][k]<<"\tphi["<<i<<"]["<<k<<"]="<<phi[i][k]<<"\tgamma["<<i<<"]="<<gamma[i]<<"\n";
	       exit(1);
        }
	if ((td->getB())[i][k]==0) {
          //cerr <<"Error: BW - ZERO(2) b["<<i<<"]["<<k<<"]="<<(td->getB())[i][k]<<"\tphi["<<i<<"]["<<k<<"]="<<phi[i][k]<<"\tgamma["<<i<<"]="<<gamma[i]<<"\n";
	  //     exit(1);
        }
      }
    }
  }

    
  //It can be possible that a probability is not updated
  //We normalize the probabilitites
  for(i=0; i<N; i++) {
    double sum=0;
    for(j=0; j<N; j++)
      sum+=(td->getA())[i][j];
    for(j=0; j<N; j++)
      (td->getA())[i][j]=(td->getA())[i][j]/sum;
  }

  for(i=0; i<N; i++) {
    double sum=0;
    for(k=0; k<M; k++) {
      if(output[k].find(i)!=output[k].end())
        sum+=(td->getB())[i][k];
    }
    for(k=0; k<M; k++) {
      if(output[k].find(i)!=output[k].end())
        (td->getB())[i][k]=(td->getB())[i][k]/sum;
    }
  }

  cerr<<"Log="<<loli<<"\n";
}

void 
HMM::tagger(FILE *in, FILE *out)
{
  int i, j, k, nw;
  TaggerWord *word=NULL;
  TTag tag;
  
  set <TTag> tags, pretags;
  set <TTag>::iterator itag, jtag;
  
  double prob, loli, x;
  int N = td->getN();  
  double alpha[2][N];
  vector<TTag> best[2][N];
  
  vector <TaggerWord> wpend; 
  int nwpend;
  
  MorphoStream morpho_stream(in, debug, td);                             

  Collection &output = td->getOutput();
  
  loli = nw = 0;
  
  //Initialization
  tags.insert(eos);
  alpha[0][eos] = 1;
   
  word = morpho_stream.get_next_word();
 
  while (word) {
    wpend.push_back(*word);    	    
    nwpend = wpend.size();
    
    pretags = tags; // Tags from the previous word

    tags = word->get_tags();
  
    if (tags.size()==0) // This is an unknown word
      tags = td->getOpenClass();
                       
    if (output.has_not(tags)) {  // Encontrada una clase de ambig�edad desconocida hasta el momento      
      if (debug) {
        wstring errors;
	errors = L"A new ambiguity class was found. \n";
	errors+= L"Retraining the tagger is necessary so as to take it into account.\n";
	errors+= L"Word '"+word->get_superficial_form()+L"'.\n";
	errors+= L"New ambiguity class: "+word->get_string_tags()+L"\n";
	wcerr<<"Error: "<<errors;
      }
      tags = find_similar_ambiguity_class(tags);
    } 
         
    k = output[tags];  //Ambiguity class the word belongs to
    
    clear_array_double(alpha[nwpend%2], N);    
    clear_array_vector(best[nwpend%2], N);
    
    //Induction
    for (itag=tags.begin(); itag!=tags.end(); itag++) { //For all tag from the current word
      i=*itag;
      for (jtag=pretags.begin(); jtag!=pretags.end(); jtag++) {	//For all tags from the previous word
	j=*jtag;
	x = alpha[1-nwpend%2][j]*(td->getA())[j][i]*(td->getB())[i][k];
	if (alpha[nwpend%2][i]<=x) {
	  if (nwpend>1) 
	    best[nwpend%2][i] = best[1-nwpend%2][j];
	  best[nwpend%2][i].push_back(i);
	  alpha[nwpend%2][i] = x;
	}
      }
    }
    
    //Backtracking
    if (tags.size()==1) {       
      tag = *tags.begin();      
      
      prob = alpha[nwpend%2][tag];
      
      if (prob>0) 
	loli -= log(prob);
      else {
        if (debug)
	  wcerr<<L"Problem with word '"<<word->get_superficial_form()<<L"' "<<word->get_string_tags()<<L"\n";
      }
      for (unsigned t=0; t<best[nwpend%2][tag].size(); t++)
      {
        wstring const &micad = wpend[t].get_lexical_form(best[nwpend%2][tag][t], (td->getTagIndex())[L"TAG_kEOF"]);
        fputws_unlocked(micad.c_str(), out); 
      }
      
      //Return to the initial state
      wpend.clear();   
      alpha[0][tag] = 1;
    }
    
    delete word;
    word = morpho_stream.get_next_word();    
  }
  
  if ((tags.size()>1)&&(debug)) {
    string errors;
    errors = "The text to disambiguate has finished, but there are ambiguous words that has not been disambiguated.\n";
    errors+= "This message should never appears. If you are reading this ..... these are very bad news.\n";
    cerr<<"Error: "<<errors;
  }  
}


void
HMM::print_A() {
  int i,j;
    
  cout<<"TRANSITION MATRIX (A)\n------------------------------\n";  
  for(i=0; i != td->getN(); i++)
    for(j=0; j != td->getN(); j++) {
      cout<<"A["<<i<<"]["<<j<<"] = "<<(td->getA())[i][j]<<"\n";
    }    
}

void
HMM::print_B() {
  int i,k;  

  cout<<"EMISSION MATRIX (B)\n-------------------------------\n";
  for(i=0; i != td->getN(); i++)
    for(k=0; k != td->getM(); k++) {
      Collection &output = td->getOutput();
      if(output[k].find(i)!=output[k].end())
        cout<<"B["<<i<<"]["<<k<<"] = "<<(td->getB())[i][k]<<"\n";
    }
}

void HMM::print_ambiguity_classes() {
  set<TTag> ambiguity_class;
  set<TTag>::iterator itag;
  cout<<"AMBIGUITY CLASSES\n-------------------------------\n";
  for(int i=0; i != td->getM(); i++) {
    ambiguity_class = (td->getOutput())[i];
    cout <<i<<": ";
    for (itag=ambiguity_class.begin(); itag!=ambiguity_class.end(); itag++) {
      cout << *itag <<" ";
    }
    cout << "\n";
  }
}   

set<TTag>
HMM::find_similar_ambiguity_class(set<TTag> c) {
  int size_ret = -1;
  set<TTag> ret=td->getOpenClass(); //Se devolver� si no encontramos ninguna clase mejor
  bool skeep_class;
  Collection &output = td->getOutput();

  for(int k=0; k<td->getM(); k++) {
    if ((((int)output[k].size())>((int)size_ret)) && (((int)output[k].size())<((int)c.size()))) {
      skeep_class=false;
      // Test if output[k] is a subset of class
      for(set<TTag>::iterator it=output[k].begin(); it!=output[k].end(); it++) {
        if (c.find(*it)==c.end()) { 
	   skeep_class=true; //output[k] is not a subset of class
	   break;
	}
      }
      if (!skeep_class) {
        size_ret = output[k].size();
	     ret = output[k];
      }
    }
  }
  return ret;
}
