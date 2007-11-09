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

#include <apertium/utf_converter.h>

//map<string, TTag> Segment::tag_index;
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

/** TRANSLATION PENDING **/
Segment*
Segment::new_segment(MorphoStream &ms, TransferRules* tr) {
  TaggerWord *word=NULL;
  set<TTag> tags;
  set<TTag>::iterator itag;
  vector<TTag> auxvec;
   
  static int index_busca_corte=0;
  static vector<TaggerWord*> palabras_frase; 
   
  Segment* seg=new Segment();
   
  if ((index_busca_corte<=0) || (index_busca_corte>=palabras_frase.size())) {
    //Tenemos que leer una nueva frase (hasta encontrar un punto)
      
    //Liberamos la frase anterior
    for(int i=0; i<palabras_frase.size(); i++)
      delete palabras_frase[i];
    palabras_frase.clear();
      
    bool continuar=true;
    while(continuar) {
      word=ms.get_next_word();
            
      if(word==NULL)
	break;
         
      palabras_frase.push_back(word);
      tags=word->get_tags();
         	 
      if (tags.size()==0) //Palabra desconocida (fin lectura frase)
	continuar=false;

      //Comprobamos si ya tenemos un frase completa
      if ((tags.size()==1) && (((*tags.begin())==tag_index[L"TAG_SENT"]) ||
			       ((*tags.begin())==tag_index[L"TAG_kEOF"]) ||
			       ((*tags.begin())==tag_index[L"TAG_LQUEST"]))) {
	continuar=false; //Ya tenemos una frase completa      
      }
    }
           
    index_busca_corte=0;
      
    /*    
    cerr<<"SENTENCE: ";
    for(int i=0; i<palabras_frase.size(); i++) {
      cerr<<palabras_frase[i]->get_superficial_form()<<" ";
      //cerr<<palabras_frase[i]->get_string_tags()<<"\n";
    }
    cerr<<"\n";
    */
  }

  int ncaminos=1; //Nº de caminos del segmento que se devuelve
   
  //De la frase que tenemos en palabras_frase extraemos el siguiente segmento
  //a partir de index_busca_corte
  int punto_corte=-1;
  int avance; //Nº de palabras que podemos saltar en busca de un punto de corte
  for(int i=index_busca_corte; i<palabras_frase.size(); i++) {
    //cerr<<"Is segmentation point word at "<<i<<"? "<<flush;
    if (tr->is_segmentation_point(tag_index[L"TAG_kEOF"], palabras_frase, i, avance)) {
      //cerr<<"yes\n";
      //Podemos cortar por aquí
      punto_corte=i;
      break;
    } else{
      //cerr<<"no\n";
      i+=avance;
    }
  }

  if ((punto_corte==-1)&&(palabras_frase.size()>0)) {
    cerr<<"Error: (serious) No segmentation point was found in the whole sentence\n";
    cerr<<"Sentece length: "<<palabras_frase.size()<<"\n";
    cerr<<"SENTECE: ";
    for(int i=0; i<palabras_frase.size(); i++)
      cerr<<UtfConverter::toUtf8(palabras_frase[i]->get_superficial_form())<<" ";
    cerr<<"\n";   
    exit(1);
  }
   
  //Ya tenemos el segmento a devolver. Desde index_busca_corte hasta punto_corte
  //(ambos inclusive)
  for(int i=index_busca_corte; i<=punto_corte; i++) {
    tags=palabras_frase[i]->get_tags();
    seg->contador_caminos.push_back(auxvec);
    if (tags.size()>0) {
      ncaminos*=tags.size();
      for(itag=tags.begin(); itag!=tags.end(); itag++) //Inseertamos las etiquetas
	seg->contador_caminos.back().push_back(*itag);
    } else {
      seg->contador_caminos.back().push_back(-1); //Palabra desconocida
    }
    seg->vwords.push_back(*palabras_frase[i]);      
  }
      
  index_busca_corte=punto_corte+1; //Para la proxima busqueda

  //Ya tenemos el número de caminos, inicalizamos los fijos de cada nodo 
  //(nº de veces que se tiene que usar una etiqueta
  //antes de pasar a la siguiente)
  for(int i=0; i<seg->contador_caminos.size(); i++) {
    int fijo=1;
    for(int j=i+1; j<seg->contador_caminos.size(); j++) {
      fijo*=seg->contador_caminos[j].size();
    }
    seg->nfijo_caminos.push_back(fijo);
  }
     
  if(seg->vwords.size()==0)    //Se acabó
    seg->npaths=0;
  else
    seg->npaths=ncaminos;
      
  return seg;   
}
