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

package dics.elements.utils;

import java.util.Vector;

import dics.elements.dtd.SElement;
import dictools.crossmodel.ConstantMap;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class SElementList extends Vector<SElement> {

    /**
         * 
         */
    static final long serialVersionUID = 0;

    /**
         * 
         * 
         */
    public SElementList() {
	super();
    }

    /**
         * 
         * @param sList
         */
    public SElementList(final SElementList sList) {
	super(sList);
    }

    /**
         * 
         * @param sEList
         * @return
         */
    public final boolean equals(final SElementList sEList) {
	if (size() != sEList.size()) {
	    return false;
	} else {
	    for (int i = 0; i < size(); i++) {
		final SElement sE1 = get(i);
		final SElement sE2 = sEList.get(i);
		if (!sE1.equals(sE2)) {
		    return false;
		}
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
	for (final SElement s : this) {
	    str += s.toString();
	}
	return str;
    }

    /**
         * 
         * @param sEList2
         * @return
         */
    public final boolean matches(final SElementList sEList2) {
	int i = 0;

	/*
         * System.err.println("pattern..."); this.print();
         * System.err.println("tagged..."); sEList2.print();
         */

	if ((sEList2.size() - size()) >= 0) {
	    for (final SElement sE2 : sEList2) {
		if (i < size()) {
		    final SElement sE1 = get(i);
		    if (sE1.getValue().charAt(0) == 'k') {
			// System.err.println("OK (k)!");
			return true;
		    }

		    if (!sE1.equals(sE2)) {
			return false;
		    }
		    // System.err.println(sE1.toString() + " = " +
		    // sE2.toString());
		    i++;
		} else {
		    return false;
		}
	    }
	    // System.err.println("OK!");
	    return true;
	} else {
	    return false;
	}
    }

    /**
         * 
         * 
         */
    public final void print() {
	for (final SElement s : this) {
	    System.err.print(s.toString());
	}
	System.err.println("");
    }

    /**
         * 
         * @param constants
         */
    public final ElementList assignValues(final ConstantMap constants,
	    final ConstantMap constants2) {
	final ElementList sEList2 = new ElementList();
	for (final SElement s : this) {
	    String key = constants.getKey(s.getValue());
	    if (key == null) {
		key = constants2.getKey(s.getValue());
	    }
	    final SElement sE2 = SElement.get(key);
	    sEList2.add(sE2);
	}
	return sEList2;
    }

}
