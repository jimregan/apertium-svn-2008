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

package dictools;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Set;

import dics.elements.dtd.DictionaryElement;
import dics.elements.dtd.EElement;
import dics.elements.dtd.Element;
import dics.elements.dtd.ParElement;
import dics.elements.dtd.PardefElement;
import dics.elements.dtd.PardefsElement;
import dics.elements.dtd.SectionElement;
import dics.elements.utils.EElementList;
import dics.elements.utils.Msg;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class DicFindEquivPar {

    /**
     * 
     */
    private DictionaryElement dic;

    /**
     * 
     */
    private String outFileName;

    /**
     * 
     */
    private Msg msg;

    /**
     * 
     * 
     */
    public DicFindEquivPar() {
	msg = new Msg();

    }

    /**
     * 
     * 
     */
    public DicFindEquivPar(final String fileName) {
	msg = new Msg();
	DictionaryReader dicReader = new DictionaryReader(fileName);
	DictionaryElement dic = dicReader.readDic();
	setDic(dic);
    }

    /**
     * 
     * 
     */
    public final void findEquivalents() {
	ArrayList<PardefElement> canBeRemoved = new ArrayList<PardefElement>();

	PardefsElement pardefs1 = getDic().getPardefsElement();
	PardefsElement pardefs2 = getDic().getPardefsElement();

	HashMap<String, PardefElement> processed = new HashMap<String, PardefElement>();
	HashMap<String, String> equivalents = new HashMap<String, String>();

	msg.err("Equivalent paradigms:");
	for (PardefElement par1 : pardefs1.getPardefElements()) {
	    for (PardefElement par2 : pardefs2.getPardefElements()) {
		if (!processed.containsKey(par1.getName() + "---"
			+ par2.getName())) {
		    if (!par1.getName().equals(par2.getName())
			    && par1.equals(par2)) {
			msg.err("(" + par1.getName() + ", " + par2.getName()
				+ ")");
			canBeRemoved.add(par2);
			equivalents.put(par2.getName(), par1.getName());
			processed.put(par1.getName() + "---" + par2.getName(),
				par1);
			processed.put(par2.getName() + "---" + par1.getName(),
				par2);
		    }
		}
	    }
	}

	for (PardefElement par : canBeRemoved) {
	    pardefs1.remove(par);
	}
	replaceParadigm(equivalents);
	dic.printXML(getOutFileName());
    }

    /**
     * 
     * @param equivalents
     */
    private final void replaceParadigm(final HashMap<String, String> equivalents) {
	HashMap<String, Integer> counter = new HashMap<String, Integer>();

	DictionaryElement dic = getDic();

	// Paradigm definitions
	for (PardefElement pardef : dic.getPardefsElement().getPardefElements()) {
	    EElementList eList = pardef.getEElements();
	    processElementList(eList, equivalents, counter);
	}

	// Sections
	for (SectionElement section : dic.getSections()) {
	    EElementList eList = section.getEElements();
	    processElementList(eList, equivalents, counter);
	}

	Set keySet = counter.keySet();
	Iterator it = keySet.iterator();
	msg.err("\nReplacements:");
	while (it.hasNext()) {
	    String key = (String) it.next();
	    Integer iO = (Integer) counter.get(key);
	    msg.err("'" + key + "' has been replaced " + iO + " times.");
	}

    }

    /**
     * 
     * @param eList
     * @param equivalents
     * @param counter
     */
    private final void processElementList(final EElementList eList,
	    final HashMap<String, String> equivalents,
	    HashMap<String, Integer> counter) {
	for (EElement element : eList) {
	    for (Element e : element.getChildren()) {
		if (e instanceof ParElement) {
		    ParElement parE = (ParElement) e;
		    String prevParName = parE.getValue();
		    if (equivalents.containsKey(parE.getValue())) {
			incrementReplacementCounter(counter, parE.getValue());
			parE.setValue(equivalents.get(parE.getValue()));
			parE.addComments("<!-- '" + prevParName
				+ "' was replaced -->");
		    }
		}
	    }
	}
    }

    /**
     * 
     * @param counter
     * @param paradigmName
     */
    private final void incrementReplacementCounter(
	    final HashMap<String, Integer> counter, final String paradigmName) {
	if (!counter.containsKey(paradigmName)) {
	    counter.put(paradigmName, new Integer(1));
	} else {
	    Integer iO = counter.get(paradigmName);
	    int i = iO.intValue();
	    i++;
	    counter.put(paradigmName, new Integer(i));
	}
    }

    /**
     * @return the dic
     */
    private final DictionaryElement getDic() {
	return dic;
    }

    /**
     * @param dic
     *                the dic to set
     */
    private final void setDic(DictionaryElement dic) {
	this.dic = dic;
    }

    /**
     * @return the outFileName
     */
    public final String getOutFileName() {
	return outFileName;
    }

    /**
     * @param outFileName
     *                the outFileName to set
     */
    public final void setOutFileName(String outFileName) {
	this.outFileName = outFileName;
    }

}
