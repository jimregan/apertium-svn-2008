package misc;

/*
 * Copyright (C) 2007 Universitat d'Alacant / Universidad de Alicante
 * Author: Enrique Benimeli Bofarull
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

import java.util.HashMap;

import dics.elements.dtd.ContentElement;
import dics.elements.dtd.DictionaryElement;
import dics.elements.dtd.EElement;
import dics.elements.dtd.Element;
import dics.elements.dtd.ParElement;
import dics.elements.dtd.PardefElement;
import dics.elements.dtd.PardefsElement;
import dics.elements.dtd.RElement;
import dics.elements.dtd.SElement;
import dics.elements.dtd.SdefElement;
import dics.elements.dtd.SdefsElement;
import dics.elements.dtd.SectionElement;
import dictools.DictionaryReader;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class AddGender {

    /**
     * 
     */
    private String[] arguments;

    /**
     * 
     */
    private String morphDic;
    
    /**
     * 
     */
    private String bilDic;
    
    /**
     * 
     */
    private String out;

    /**
     * 
     *
     */
    public final void processArguments() {
	morphDic = arguments[1];
	bilDic = arguments[2];
	out = arguments[3];
    }
    
    /**
     * 
     *
     */
    public final void doAddGender() {
	this.processArguments();
	
	DictionaryReader reader = new DictionaryReader(morphDic);
	System.out.println("Reading morphological '" + morphDic + "'");
	DictionaryElement dic = reader.readDic();
	//dic.printXML("morf-es.dix");

	HashMap<String,SElement> ng = new HashMap<String,SElement>();

	PardefsElement pars = dic.getPardefsElement();

	for (SectionElement section : dic.getSections()) {
	    for (EElement ee : section.getEElements()) {
		String lemma = ee.getLemma();
		if (lemma != null ) {
		    String parName = ee.getParadigmValue();
		    if (parName != null && parName.endsWith("__n")) {
			PardefElement par = pars.getParadigmDefinition(parName);
			if (par != null) {
			    for (EElement eepar : par.getEElements()) {
				RElement r = eepar.getP().getR();
				for (Element er : r.getChildren()) {
				    if (er instanceof SElement) {
					SElement s = (SElement)er;
					String sv = er.getValue();
					if (sv.equals("m") || sv.equals("f") || sv.equals("mf")) {
					    ng.put(lemma, s);
					    //System.out.println(lemma + " (" + sv + ")");
					}
				    }
				}
			    }
			}
		    }
		}
	    }	    
	}

	System.out.println(ng.size() + " nouns read.");

	DictionaryReader reader2 = new DictionaryReader(bilDic);
	DictionaryElement bil = reader2.readDic();

	SdefsElement sdefs = bil.getSdefs();
	SdefElement n = new SdefElement("n");
	SdefElement m = new SdefElement("m");
	SdefElement f = new SdefElement("f");
	SdefElement mf = new SdefElement("mf");
	sdefs.addSdefElement(n);
	sdefs.addSdefElement(m);
	sdefs.addSdefElement(f);
	sdefs.addSdefElement(mf);
	
	int genderFound = 0;
	int genderNotFound = 0;
	for (SectionElement section : bil.getSections()) {
	    for (EElement ee : section.getEElements()) {
		if (!ee.isRegEx()) {
		    String parName = ee.getParadigmValue();
		    if (parName != null) {
			if (parName.contains("NC") ) {
			    ContentElement leftSide = ee.getSide("L");
			    ContentElement rightSide = ee.getSide("R");
			    String text = leftSide.getValue();
			    
			    SElement gender = ng.get(text);
			    if (gender != null) {
				//System.out.println(text + " (" + gender.getValue() + ")");
				genderFound++;
			    SElement noun = new SElement("n");
			    leftSide.addChild(noun);
			    rightSide.addChild(noun);
			    leftSide.addChild(gender);
			    // and remove par element if NC
			    ParElement par = null;
			    for (Element e : ee.getChildren()) {
				if (e instanceof ParElement) {
				    par = (ParElement)e;
				}
			    }

			    ee.getChildren().remove(par);
			    } else {
				genderNotFound++;
				System.err.println("(" + genderNotFound +") I could not find gender for '" + text + "'");
			    }
			    
			}
		    }
		}
	    }
	}
	
	System.out.println("I found gender for " + genderFound + " lemmas.");
	System.out.println("I could not find gender for " + genderNotFound + " lemmas (see addgender.err).");

	System.out.println("Updated bilingual dictionary: '" + out + "'");
	bil.printXML(out);	
    }

    /**
     * @return the arguments
     */
    public final String[] getArguments() {
        return arguments;
    }

    /**
     * @param arguments the arguments to set
     */
    public final void setArguments(String[] arguments) {
        this.arguments = arguments;
    }

}
