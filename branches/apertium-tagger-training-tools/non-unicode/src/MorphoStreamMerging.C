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

/** Class MorphoStreamMerging (source file)
 */

#include "MorphoStreamMerging.H"
#include "TaggerWordMerging.H"

MorphoStreamMerging::MorphoStreamMerging(FILE *ftxt, bool d, TaggerMergingData *t):MorphoStream(ftxt, d, t) {
  tmd=t;
}

MorphoStreamMerging::~MorphoStreamMerging() {
}

TaggerWordMerging*
MorphoStreamMerging::get_next_word() {
  TaggerWord *w;
  TaggerWordMerging *wreturn;

  w=MorphoStream::get_next_word();

  if(w==NULL) return NULL;

  wreturn = new TaggerWordMerging(*w);

  set<int> finetags;
  set<int>::iterator it;
  finetags=w->get_tags();
  int coarsetag, finetag;
  string raw_lexical_form;

  //cerr<<"Word: "<<w->get_superficial_form()<<"\n";
  //cerr<<"Fine tags:\n";
  //for(it=w->get_tags().begin(); it!=w->get_tags().end(); it++) {
  //  TTag t=*it;
  //  cerr<<"    "<<t<<" "<<w->get_lexical_form(t, tmd->getFineEofTag())<<"\n";
  //}
  //cerr<<"Coarse tags:\n";
  for(it=finetags.begin(); it!=finetags.end(); it++) {
    finetag=*it;
    coarsetag=tmd->getFine2Coarse()[finetag];
    wreturn->add_tag(coarsetag, finetag);
    //cerr<<"    "<<coarsetag<<" "<<wreturn->get_lexical_form(coarsetag, tmd->getFineEofTag())<<"\n";
  }

  if (w->get_tags().size()==0) { //Unknown word
    wreturn->add_tag(tmd->getUndefTag(), tmd->getFineUndefTag());
  }

  delete w;
  return wreturn;
}
