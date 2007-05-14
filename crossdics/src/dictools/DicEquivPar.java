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

import dics.elements.dtd.DictionaryElement;
import dics.elements.dtd.PardefElement;
import dics.elements.dtd.PardefsElement;
import dics.elements.utils.DicSet;

/**
 * @author Enrique Benimeli Bofarull
 * 
 */
public class DicEquivPar {

    /**
         * 
         */
    private DicSet dicSet;

    /**
         * 
         */
    private HashMap<String, ArrayList<PardefElement>> equivA;

    /**
         * 
         */
    private HashMap<String, ArrayList<PardefElement>> equivC;

    /**
         * 
         * @param dics
         */
    public DicEquivPar(final DicSet dics) {
	dicSet = dics;
    }

    /**
         * 
         * @param mon
         */
    public DicEquivPar(final DictionaryElement mon) {
	final DicSet ds = new DicSet(mon);
	dicSet = ds;
    }

    /**
         * 
         * 
         */
    public final void findEquivalents() {
	System.err.println("Searching for equivalents...");
	final DictionaryElement monA = dicSet.getMon1();
	final DictionaryElement monC = dicSet.getMon2();

	final PardefsElement pardefsA = monA.getPardefsElement();
	final PardefsElement pardefsC = monC.getPardefsElement();

	System.err.println("Searching for equivalents in monA...");
	equivA = this.findEquivalents(pardefsA);
	System.err.println("Searching for equivalents in monC...");
	equivC = this.findEquivalents(pardefsC);
    }

    public final HashMap<String, ArrayList<PardefElement>> findEquivalentsA() {
	System.err.println("Searching for equivalents...");
	final DictionaryElement monA = dicSet.getMon1();

	final PardefsElement pardefsA = monA.getPardefsElement();

	System.err.println("Searching for equivalents in monA...");
	equivA = this.findEquivalents(pardefsA);
	return equivA;
    }

    /**
         * 
         * @return
         */
    public HashMap<String, ArrayList<PardefElement>> getEquivalentsA() {
	return equivA;
    }

    /**
         * 
         * @return
         */
    public HashMap<String, ArrayList<PardefElement>> getEquivalentsC() {
	return equivC;
    }

    /**
         * 
         * @param pardefs
         */
    private HashMap<String, ArrayList<PardefElement>> findEquivalents(
	    final PardefsElement pardefs) {
	final ArrayList<PardefElement> pardefscopy = new ArrayList<PardefElement>(
		pardefs.getPardefElements());
	final HashMap<String, ArrayList<PardefElement>> equivalents = new HashMap<String, ArrayList<PardefElement>>();

	System.err.println(pardefs.getPardefElements().size() + " paradigmas");
	int nEquiv = 0;
	for (final PardefElement pardefA1 : pardefs.getPardefElements()) {
	    // lista de equivalentes

	    for (final PardefElement pardefA2 : pardefscopy) {
		// System.err.println("Comparando " + pardefA1.getName() + " y "
		// + pardefA2.getName() + " ...");
		if (!pardefA1.getName().equals(pardefA2.getName())
			&& pardefA1.equals(pardefA2)) {

		    System.err.println(pardefA1.getName()
			    + " es equivalente a " + pardefA2.getName());
		    nEquiv++;
		    /*
                         * System.err.println("pardefA1:");
                         * System.err.println(pardefA1.toString());
                         * System.err.println("pardefA2:");
                         * System.err.println(pardefA2.toString());
                         */

		    if (!equivalents.containsKey(pardefA1.getName())) {
			final ArrayList<PardefElement> equivList = new ArrayList<PardefElement>();
			equivList.add(pardefA2);
			equivalents.put(pardefA1.getName(), equivList);
		    } else {
			final ArrayList<PardefElement> list = equivalents
				.get(pardefA1.getName());
			list.add(pardefA2);
			equivalents.put(pardefA1.getName(), list);
		    }
		} else {
		    // System.err.println(pardefA1.getName() + " NO es
		    // equivalente a " + pardefA2.getName() );
		}
	    }
	}
	System.err.println((nEquiv / 2) + " paradigmas equivalentes.");
	return equivalents;
    }

    /**
         * 
         * @param lemma
         * @param equivPar
         * @param category
         * @return
         */
    public static PardefElement getEquivalentParadigm(final String parName,
	    final HashMap<String, ArrayList<PardefElement>> equivPar,
	    final String category) {
	// System.err.println("Searching for equivalent paradigm with category
	// '" + category + "' for '" + lemma + "'");
	final ArrayList<PardefElement> list = (ArrayList<PardefElement>) equivPar
		.get(parName);
	if (list != null) {
	    for (final PardefElement pE : list) {
		if (pE.hasCategory(category)) {
		    return pE;
		}
	    }
	}
	return null;
    }

}
