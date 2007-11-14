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
 * Class Segment. It define a text segment  (source file)
 *
 *  @author   Felipe Sánchez-Martínez - fsanchez@dlsi.ua.es
 */

#include "Segment.H"

#include <deque>

#include <apertium/utf_converter.h>

map<wstring, TTag, Ltstr> Segment::tag_index;

Segment::Segment() {
  npaths=0;
}

int 
Segment::get_number_paths() {
  return npaths;
}

void 
Segment::set_tag_index( map<wstring, TTag, Ltstr> ti) {
  tag_index=ti;
}

wstring
Segment::get_path(vector <TTag>& etqpart, int path) {
  etqpart.clear();
  wstring s=L"";
   
  for(int i=0; i<contador_caminos.size(); i++) {
    int tag_position=((int)(path/nfijo_caminos[i]))%contador_caminos[i].size();            
    s+=vwords[i].get_lexical_form(contador_caminos[i][tag_position], tag_index[L"TAG_kEOF"]);
    if ((!vwords[i].get_plus_cut()) && (i<(contador_caminos.size()-1)))
      s+=L" ";
    etqpart.push_back(contador_caminos[i][tag_position]);
  }
   
  return s;
}

Segment*
Segment::new_segment(MorphoStream &ms, TransferRules* tr,  TaggerData &td) {
  TaggerWord *word=NULL;
  set<TTag> tags;
  set<TTag>::iterator itag;
  vector<TTag> auxvec;

  static int index_start=1;
  static deque<TaggerWord> wordsbuffer;
  static bool first_call=true;
  static bool end_of_corpus_reached=false;


  if (first_call) {
    TaggerWord eosword;
    eosword.add_tag(td.getTagIndex()[L"TAG_SENT"], L"", td.getPreferRules());
    wordsbuffer.push_back(eosword);

    //Fill the buffer of words
    while (wordsbuffer.size()<TAGGER_WORD_BUFFER_SIZE) {
      word=ms.get_next_word();
            
      if(word==NULL) {
	end_of_corpus_reached=true;
	break;
      }

      wordsbuffer.push_back(*word);
      delete word;
    }

    first_call=false;
  }

  /*
    cerr<<"BUFFER (begining): ";
    for (int i=0; i<wordsbuffer.size(); i++) {
    cerr<<"["<<wordsbuffer[i].get_superficial_form()<<"] ";
    }
    cerr<<"\n";
    cerr<<"Buffer size (begining): "<<wordsbuffer.size()<<"\n";
    cerr<<"Index start (begining): "<<index_start<<"\n";
  */

  Segment* seg=new Segment();
  int number_of_paths=1; 
   
  int segmentation_point=-1;
  int advance; //Number of word that can be skipped when looking for a segmentation point
  for(int i=index_start; i<wordsbuffer.size(); i++) {
    if (tr->is_segmentation_point(tag_index[L"TAG_kEOF"], wordsbuffer, i, advance)) {
      segmentation_point=i;
      break;
    } else{
      i+=advance;
    }
  }

  if ((segmentation_point==-1) && (!end_of_corpus_reached)) {
    cerr<<"Error: No segmentation point was found.\n";
    cerr<<"Try making the buffer longer, current maximum size is "<<TAGGER_WORD_BUFFER_SIZE<<"\n";
    cerr<<"See Segment.H, TAGGER_WORD_BUFFER_SIZE constant\n";
    exit(EXIT_FAILURE);
  }

  //cerr<<"Segmentation point: "<<segmentation_point<<"\n";

  //The segment to return is from index_start to segmentation_point
  for(int i=index_start; i<=segmentation_point; i++) {
    tags=wordsbuffer[i].get_tags();
    seg->contador_caminos.push_back(auxvec);
    if (tags.size()>0) {
      number_of_paths*=tags.size();
      for(itag=tags.begin(); itag!=tags.end(); itag++)
	seg->contador_caminos.back().push_back(*itag);
    } else {
      //seg->contador_caminos.back().push_back(-1); //Palabra desconocida

      tags=td.getOpenClass();
      number_of_paths*=tags.size();

      for(itag=tags.begin(); itag!=tags.end(); itag++)
	seg->contador_caminos.back().push_back(*itag);
    }
    seg->vwords.push_back(wordsbuffer[i]);
  }
  
  //Calculate which words can be removed from the buffer, we need some
  //words before the segment being return, more concretely, from the
  //last non-ambiguous word until the first word of the segment being
  //returned
  int preserve_word_from=-1;
  for (int i=(index_start-1); i>=0; i--) {
    if (wordsbuffer[i].get_tags().size()==1) {
      preserve_word_from=i;
      break;
    }
  }

  //cerr<<"Preserve words from index: "<<preserve_word_from<<"\n";

  for(int i=0; i<preserve_word_from; i++) {
    wordsbuffer.pop_front();
    segmentation_point--;
    index_start--; 
  }

  /*
    cerr<<"BUFFER (after removing words): ";
    for (int i=0; i<wordsbuffer.size(); i++) {
    cerr<<"["<<wordsbuffer[i].get_superficial_form()<<"] ";
    }
    cerr<<"\n";
    cerr<<"Buffer size (after removing words): "<<wordsbuffer.size()<<"\n";
    cerr<<"Index start (after removing words): "<<index_start<<"\n";
    cerr<<"Segmention point (after removing words): "<<segmentation_point<<"\n";
  */

  //Refill the buffer
  if (!end_of_corpus_reached) {
    while (wordsbuffer.size()<TAGGER_WORD_BUFFER_SIZE) {
      word=ms.get_next_word();
            
      if(word==NULL) {
	end_of_corpus_reached=true;
	break;
      }

      wordsbuffer.push_back(*word);
      delete word;
    }
  }

  /*
    cerr<<"BUFFER (after refill): ";
    for (int i=0; i<wordsbuffer.size(); i++) {
    cerr<<"["<<wordsbuffer[i].get_superficial_form()<<"] ";
    }
    cerr<<"\n";
    cerr<<"Buffer size (after refill): "<<wordsbuffer.size()<<"\n";
    cerr<<"Index start (after refill): "<<index_start<<"\n";
    cerr<<"Segmention point (after refill): "<<segmentation_point<<"\n";
  */
 
  //Now we retrieve words before and after this segment, for the
  //calculus of the alphas and betas in the pruning method
  for (int i=0; i<index_start; i++)
    seg->vwords_before.push_back(wordsbuffer[i]);

  bool found_forward=false;
  for(int i=segmentation_point+1; i<wordsbuffer.size(); i++) {
    seg->vwords_after.push_back(wordsbuffer[i]);
    if (wordsbuffer[i].get_tags().size()==1) {
      found_forward=true;
      break;
    }
  }

  if (!found_forward) {
    if (!end_of_corpus_reached) {
      cerr<<"Error: No unambiguous word was found when looking fordward.\n";
      cerr<<"Try making the buffer longer, current maximum size is "<<TAGGER_WORD_BUFFER_SIZE<<"\n";
      cerr<<"See Segment.H, TAGGER_WORD_BUFFER_SIZE constant\n";
      exit(EXIT_FAILURE);
    }  else {
      TaggerWord eosword;
      eosword.add_tag(td.getTagIndex()[L"TAG_SENT"], L"", td.getPreferRules());
      seg->vwords_after.push_back(eosword);
    }
  }
  
  index_start=segmentation_point+1; //For the next search

  //We have the total number of disambiguation paths for this segment.
  //Now we initialize the structure used to retrieve all the paths in
  //an efficient way. (nfijos_caminos = nº de veces que se tiene que
  //usar una etiqueta antes de pasar a la siguiente)
  for(int i=0; i<seg->contador_caminos.size(); i++) {
    int fijo=1;
    for(int j=i+1; j<seg->contador_caminos.size(); j++) {
      fijo*=seg->contador_caminos[j].size();
    }
    seg->nfijo_caminos.push_back(fijo);
  }
     
  if(seg->vwords.size()==0)    //That's all folks
    seg->npaths=0;
  else
    seg->npaths=number_of_paths;
 
  return seg;   
}
