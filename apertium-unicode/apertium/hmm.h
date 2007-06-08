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
/**
 *  First order hidden Markov model (HMM) implementation (header)
 *
 *  @author   Felipe S�nchez-Mart�nez - fsanchez@dlsi.ua.es
 */

#ifndef __HMM_H
#define __HMM_H

#include <cstdio>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <cfloat>

#include <apertium/collection.h>
#include <apertium/constant_manager.h>
#include <apertium/morpho_stream.h>
#include <apertium/tagger_data.h>
#include <apertium/tagger_utils.h>
#include <apertium/tagger_word.h>

using namespace std;

#define ZERO 1e-10

/** HMM
 *  first-order hidden Markov Model
 */
class HMM {
private:
   TaggerData *td;
   TTag eos; // end-of-sentence tag
   bool debug;  //If true, print error messages when tagging input text

   /** It allocs memory for the transition (a) and the emission (b) matrices.
    *  Before calling this method the number of ambiguity classes must be known.
    *  This methos is called within read_ambiguity_classes and read_dictionary.
    *  @see: read_ambiguity_classes, read_dictionary
    */
   void init(); 
   
   /** This method returns a knwon ambiguity class that is a subset of
    *  the one received as a parameter. This is useful when a new
    *  ambiguty class is found because of changes in the morphological
    *  dictionary used by the MT system.
    *  @param c set of tags (ambiguity class)
    *  @return a known ambiguity class 
    */
   set<TTag> find_similar_ambiguity_class(set<TTag> c);
   
public:  
 
   /** Constructor
    */
   HMM(TaggerData *t);

   /** Destructor
    */
   ~HMM();
  
   /** Used to set the end-of-sentence tag
    *  @param t the end-of-sentence tag
    */
   void set_eos(TTag t);
   
   /** Used to set the debug flag
    *
    */
   void set_debug(bool d);

   /** It reads the ambiguity classes from the stream received as
    *  input
    *  @param is the input stream
    */  
   void read_ambiguity_classes(FILE *in);
  
   /** It writes the ambiguity classes to the stream received as
    *  a parameter
    *  @param iosthe output stream
    */
   void write_ambiguity_classes(FILE *out);
  
   /** It reads the probabilities (matrices a and b) from the stream 
    *  received as a parameter
    *  @param is the input stream
    */
   void read_probabilities(FILE *in);

   /** It writes the probabilities (matrices a and b) to the stream 
    *  received as a parameter
    *  @param os the output stream
    */ 
   void write_probabilities(FILE *out);
  
   /** It reads the expanded dictionary received as a parameter and calculates
    *  the set of ambiguity classes that the tagger will manage.
    *  @param is the input stream with the expanded dictionary to read
    */
   void read_dictionary(FILE *is);  
           
   /** It initializes the transtion (a) and emission (b) probabilities
    *  from an untagged inputr text by means of Kupiec's method
    *  @param is the input stream with the untagged corpus to process
    */
   void init_probabilities_kupiec (FILE *is);
  
   /** It initializes the transtion (a) and emission (b) probabilities
    *  from a tagged input text by means of the expected-likelihood 
    *  estimate (ELE) method
    *  @param ftagged the input stream with the tagged corpus to process
    *  @param funtagged the same corpus to porcesss but untagged
    */   
   void init_probabilities_from_tagged_text(FILE *ftagged, FILE *funtagged);

   /** It applies the forbid and enforce rules found in tagger specification.
    *  To to so the transition matrix is modified by introducing null probabilitites
    *  in the involded transitions.
    */
   void apply_rules();
   
   /** Unsupervised training algorithm (Baum-Welch implementation).
    *  @param is the input stream with the untagged corpus to process
    */  
   void train (FILE *is);  
  
   /** Tagging algorithm (Viterbi implementation).
    *  @param in the input stream with the untagged text to tag
    *  @param out the output stream with the tagged text
    */
   void tagger (FILE *in, FILE *out);
        
   /** Prints the A matrix.
    */
   void print_A();

   /** Prints the B matrix.
    */ 
   void print_B();

   /** Prints the ambiguity classes.
    */
   void print_ambiguity_classes();

   void filter_ambiguity_classes(FILE *in, FILE *out);
};

#endif
