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

package dics.elements.dtd;

import java.io.DataOutputStream;
import java.io.IOException;
import java.util.ArrayList;

import dics.elements.utils.EElementList;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class PardefElement extends Element {

    /**
         * 
         */
    private EElementList eElements;

    /**
         * 
         */
    private String n;

    /**
         * 
         * @param value
         */
    public PardefElement(final String value) {
	n = value;
	eElements = new EElementList();
    }

    /**
         * Duplicates paradigm definition
         * 
         * @param orig
         * @param name
         */
    public PardefElement(final PardefElement orig, final String name) {
	n = name;
	eElements = new EElementList();

	for (final EElement e : orig.eElements) {
	    final EElement e2 = (EElement) e.clone();
	    eElements.add(e2);
	}
    }

    /**
         * 
         * @return
         */
    public final String getName() {
	return n;
    }

    /**
         * 
         * @param value
         */
    public final void addEElement(final EElement value) {
	eElements.add(value);
    }

    /**
         * 
         */
    @Override
    public final void printXML(final DataOutputStream dos) throws IOException {
	if (comments != null) {
	    dos.writeBytes(tab(2) + "<!--\n");
	    dos.writeBytes(tab(2) + comments);
	    dos.writeBytes(tab(2) + "-->\n");
	}
	dos.writeBytes(tab(2) + "<pardef n=\"" + n + "\">\n");
	for (final EElement e : eElements) {
	    e.printXML(dos);
	}
	dos.writeBytes(tab(2) + "</pardef>\n");
    }

    /**
         * 
         * @param pardef2
         * @return
         */
    public final boolean equals(final PardefElement pardef2) {

	final ArrayList<EElement> v1 = eElements;
	final ArrayList<EElement> v2 = pardef2.eElements;

	final int maxi = v1.size();
	final int maxj = v2.size();

	if (maxi != maxj) {
	    return false;
	}

	final boolean[] c1 = new boolean[maxi];
	final boolean[] c2 = new boolean[maxj];

	for (int i = 0; i < maxi; i++) {
	    final String sv1 = v1.get(i).toString();
	    for (int j = 0; j < maxj; j++) {
		final String sv2 = v2.get(j).toString();
		if ((sv1 != null) && (sv2 != null)) {
		    if (!c1[i] && !c2[j] && sv1.equals(sv2)) {
			c1[i] = true;
			c2[j] = true;
		    }
		}
	    }
	}
	for (int i = 0; i < maxi; i++) {
	    if (!c1[i]) {
		return false;
	    }
	}
	for (int j = 0; j < maxj; j++) {
	    if (!c2[j]) {
		return false;
	    }
	}
	return true;
    }

    /**
         * 
         */
    @Override
    public final String toString() {
	String str = "";
	str += "<" + getName() + ">";
	for (final EElement e : eElements) {
	    str += e.toString();
	}
	return str;
    }

    /**
         * 
         * @param category
         * @return
         */
    public boolean hasCategory(final String category) {
	return (n.endsWith("__" + category));
    }

    /**
         * 
         * @param e
         * @param mon
         * @param category
         * @param equivCategory
         * @return
         */
    public static final PardefElement duplicateParadigm(final EElement e,
	    final DictionaryElement mon, final String category,
	    final String equivCategory) {
	// Hay que cambiar la categoria (primer "s") en cada elemento 'e' en la
	// definicin del paradigma
	PardefElement dupPardefE = null;
	final String paradigmValue = e.getParadigmValue();
	String dupParadigmValue = e.getParadigmValue();
	dupParadigmValue = dupParadigmValue.replaceAll("__" + equivCategory,
		"__" + category);

	final PardefElement parDefInMon = mon
		.getParadigmDefinition(dupParadigmValue);

	if (parDefInMon == null) {
	    final PardefElement pardefE = mon
		    .getParadigmDefinition(paradigmValue);
	    dupPardefE = new PardefElement(pardefE, dupParadigmValue);
	    dupPardefE.addComments("equivalent to '" + paradigmValue + "'");
	    dupPardefE.changeCategory(category);
	} else {
	    parDefInMon.changeCategory(category);
	    return parDefInMon;
	}
	return dupPardefE;
    }

    /**
         * 
         * @param newCategory
         */
    private final void changeCategory(final String newCategory) {
	for (final EElement e : eElements) {
	    e.changeCategory("R", newCategory);
	}
    }

    /**
         * @return the eElements
         */
    public final EElementList getEElements() {
	return eElements;
    }

    /**
         * @param elements
         *                the eElements to set
         */
    public final void setEElements(EElementList elements) {
	eElements = elements;
    }

}
