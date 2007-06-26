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
import dics.elements.dtd.LElement;
import dics.elements.dtd.PElement;
import dics.elements.dtd.ParElement;
import dics.elements.dtd.PardefElement;
import dics.elements.dtd.PardefsElement;
import dics.elements.dtd.RElement;
import dics.elements.dtd.SElement;
import dics.elements.dtd.SdefElement;
import dics.elements.dtd.SdefsElement;
import dics.elements.dtd.SectionElement;
import dics.elements.dtd.TextElement;
import dictools.DictionaryReader;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class AssignParadigm {

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
    public final void doAssignParadigm() {
	this.processArguments();

	DictionaryReader reader = new DictionaryReader(morphDic);
	reader.setReadParadigms(false);
	System.out.println("Reading morphological '" + morphDic + "'");
	DictionaryElement dic = reader.readDic();

	HashMap<String, String> np = new HashMap<String, String>();

	for (SectionElement section : dic.getSections()) {
	    for (EElement ee : section.getEElements()) {
		String parName = ee.getParadigmValue();
		if (parName != null) {
		    String right = ee.getSide("R").getValue();
		    np.put(right, parName);
		}
	    }
	}

	System.out.println(np.size() + " entries read.");
	DictionaryReader reader2 = new DictionaryReader(bilDic);
	DictionaryElement bil = reader2.readDic();

	for (SectionElement section : bil.getSections()) {
	    for (EElement ee : section.getEElements()) {
		if (!ee.isRegEx()) {
		    String left = ee.getSide("L").getValue();
		    String right = ee.getSide("R").getValue();

		    String leftNoTags = this.cleanTags(left);
		    String rightNoTags = this.cleanTags(right);
		    String par = np.get(rightNoTags);

		    EElement e = new EElement();
		    e.setComment("auto");

		    PElement p = new PElement();
		    e.addChild(p);
		    if (par == null) {
			System.err.println("No paradigm for '" + leftNoTags
				+ "'");
			par = "";
		    }
		    ParElement parE = new ParElement(par);
		    e.addChild(parE);

		    LElement l = new LElement();
		    RElement r = new RElement();
		    TextElement text = new TextElement(leftNoTags);
		    r.addChild(text);
		    p.setLElement(l);
		    p.setRElement(r);

		    dic.addEElement(e);
		}
	    }
	}
	System.out.println("Updated morphological dictionary: '" + out + "'");
	dic.printXML(out);
    }

    /**
         * 
         * @param value
         * @return
         */
    private final String cleanTags(final String value) {
	String[] vs = value.split("\\[");
	if (vs == null)
	    return value;
	if (vs.length > 1) {
	    return vs[0];
	} else {
	    return value;
	}

    }

    /**
         * @return the arguments
         */
    public final String[] getArguments() {
	return arguments;
    }

    /**
         * @param arguments
         *                the arguments to set
         */
    public final void setArguments(String[] arguments) {
	this.arguments = arguments;
    }

}
