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

import dics.elements.dtd.DictionaryElement;
import dictools.DicTools;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class DicSet {

    /**
         * 
         */
    private DictionaryElement mon1;

    /**
         * 
         */
    private DictionaryElement bil1;

    /**
         * 
         */
    private DictionaryElement mon2;

    /**
         * 
         */
    private DictionaryElement bil2;

    /**
         * 
         * @param mon1
         * @param bil1
         * @param mon2
         * @param bil2
         */
    public DicSet(final DictionaryElement mon1, final DictionaryElement bil1,
	    final DictionaryElement mon2, final DictionaryElement bil2) {
	this.mon1 = mon1;
	this.mon1.setType("MONOL");
	this.bil1 = bil1;
	this.bil1.setType("BIL");
	this.mon2 = mon2;
	this.mon2.setType("MONOL");
	this.bil2 = bil2;
	this.bil2.setType("BIL");
    }

    /**
         * 
         * @param mon1
         */
    public DicSet(final DictionaryElement mon1) {
	this.mon1 = mon1;
	bil1 = null;
	mon2 = null;
	bil2 = null;
    }

    /**
         * 
         * @param bilAB
         * @param monA
         * @param monB
         */
    public DicSet(final DictionaryElement bilAB, final DictionaryElement monA,
	    final DictionaryElement monB) {
	mon1 = monA;
	bil1 = bilAB;
	mon2 = monB;
	bil2 = null;
    }

    /**
         * 
         * @return
         */
    public DictionaryElement getMon1() {
	return mon1;
    }

    /**
         * 
         * @return
         */
    public DictionaryElement getMon2() {
	return mon2;
    }

    /**
         * 
         * @return
         */
    public DictionaryElement getBil1() {
	return bil1;
    }

    /**
         * 
         * @return
         */
    public DictionaryElement getBil2() {
	return bil2;
    }

    /**
         * 
         * 
         */
    /*
         * public final void printSize() {
         * System.err.println(mon1.getEntries().size() + " entradas en el
         * diccionario monolinge es-ca");
         * System.err.println(mon2.getEntries().size() + " entradas en el
         * diccionario monolinge es-pt");
         * System.err.println(bil1.getEntries().size() + " entradas en el
         * diccionario bilinge es-ca");
         * System.err.println(bil2.getEntries().size() + " entradas en el
         * diccionario bilinge es-pt"); }
         */

    /**
         * 
         * 
         */
    public final void reportMetrics() {
	System.err.println("monA");
	mon1.reportMetrics();
	System.err.println("monC");
	mon2.reportMetrics();
	System.err.println("bilAB");
	bil1.reportMetrics();
	System.err.println("bilBC");
	bil2.reportMetrics();
    }

    /**
         * 
         * @param suffix
         */
    public final void printXML(final String suffix) {
	printMonolXML(suffix);
	getBil1().printXML(
		DicTools.removeExtension(getBil1().getFileName()) + "-"
			+ suffix + ".dix");
	getBil2().printXML(
		DicTools.removeExtension(getBil2().getFileName()) + "-"
			+ suffix + ".dix");
    }

    /**
         * 
         * @param suffix
         */
    public final void printMonolXML(final String suffix) {
	getMon1().printXML(
		DicTools.removeExtension(getMon1().getFileName()) + "-"
			+ suffix + ".dix");
	getMon2().printXML(
		DicTools.removeExtension(getMon2().getFileName()) + "-"
			+ suffix + ".dix");
    }

}
