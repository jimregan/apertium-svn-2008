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

import java.util.HashMap;

import dics.elements.dtd.AlphabetElement;
import dics.elements.dtd.DictionaryElement;
import dics.elements.dtd.EElement;
import dics.elements.dtd.PardefElement;
import dics.elements.dtd.PardefsElement;
import dics.elements.dtd.SdefElement;
import dics.elements.dtd.SdefsElement;
import dics.elements.dtd.SectionElement;
import dics.elements.utils.DicSet;
import dics.elements.utils.EElementList;
import dics.elements.utils.EHashMap;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class DicMerge {

    /**
         * 
         */
    private DictionaryElement bilAB1;

    /**
         * 
         */
    private DictionaryElement bilAB2;

    /**
         * 
         */
    private DictionaryElement monA1;

    /**
         * 
         */
    private DictionaryElement monA2;

    /**
         * 
         */
    private DictionaryElement monB1;

    /**
         * 
         */
    private DictionaryElement monB2;

    /**
         * 
         * @param dic1
         * @param dic2
         */
    public DicMerge(final DicSet ds1, final DicSet ds2) {
	setBilAB1(ds1.getBil1());
	setMonA1(ds1.getMon1());
	setMonB1(ds1.getMon2());

	setBilAB2(ds2.getBil1());
	setMonA1(ds2.getMon1());
	setMonA2(ds2.getMon2());
    }

    /**
         * 
         * @param bilAB1
         * @param bilAB2
         */
    public final void setBils(final DictionaryElement bAB1,
	    final DictionaryElement bAB2) {
	setBilAB1(bAB1);
	setBilAB2(bAB2);
    }

    /**
         * 
         * @param mA1
         * @param mA2
         */
    public final void setMonAs(final DictionaryElement mA1,
	    final DictionaryElement mA2) {
	setMonA1(mA1);
	setMonA2(mA2);
    }

    /**
         * 
         * @param mB1
         * @param mB2
         */
    public final void setMonBs(final DictionaryElement mB1,
	    final DictionaryElement mB2) {
	setMonB1(mB1);
	setMonB2(mB2);
    }

    /**
         * 
         * @return
         */
    public final DicSet merge() {
	final DictionaryElement bilAB = mergeBils(getBilAB1(), getBilAB2());
	String fileName = getBilAB1().getFileName();
	fileName = DicTools.removeExtension(fileName);
	bilAB.setFileName(fileName + "-merged.dix");

	bilAB.countEntries();
	getBilAB1().countEntries();
	getBilAB2().countEntries();
	bilAB.addComments("\n\tResult of merging 2 dictionaries:");
	bilAB.addComments("\t" + bilAB.getNEntries() + " entries ("
		+ getBilAB1().getNEntries() + " U " + getBilAB2().getNEntries()
		+ ")");

	final DictionaryElement monA = mergeMonols(getMonA1(), getMonA2());
	String monAfn = getMonA1().getFileName();
	monAfn = DicTools.removeExtension(monAfn);
	monA.setFileName(monAfn + "-merged.dix");

	monA.countEntries();
	getMonA1().countEntries();
	getMonA2().countEntries();
	monA.addComments("\n\tResult of merging 2 dictionaries:");
	monA.addComments("\t" + monA.getNEntries() + " entries ("
		+ getMonA1().getNEntries() + " U " + getMonA2().getNEntries()
		+ ")");

	final DictionaryElement monB = mergeMonols(getMonB1(), getMonB2());
	String monBfn = getMonB1().getFileName();
	monBfn = DicTools.removeExtension(monBfn);
	monB.setFileName(monBfn + "-merged.dix");

	monB.countEntries();
	getMonB1().countEntries();
	getMonB2().countEntries();
	monB.addComments("\n\tResult of merging 2 dictionaries:");
	monB.addComments("\t" + monB.getNEntries() + " entries ("
		+ getMonB1().getNEntries() + " U " + getMonB2().getNEntries()
		+ ")");

	final DicSet dicSet = new DicSet(bilAB, monA, monB);
	return dicSet;
    }

    /**
         * 
         * @param bAB1
         * @param bAB2
         * @return
         */
    private final DictionaryElement mergeBils(final DictionaryElement bAB1,
	    final DictionaryElement bAB2) {
	final DictionaryElement dic = new DictionaryElement();

	final AlphabetElement alphabet = mergeAlphabetElement(bAB1
		.getAlphabet(), bAB2.getAlphabet());
	dic.setAlphabet(alphabet);

	SdefsElement sdefs = new SdefsElement();
	sdefs = mergeSdefElements(bAB1.getSdefs(), bAB2.getSdefs());
	dic.setSdefs(sdefs);

	for (final SectionElement section1 : bAB1.getSections()) {
	    final SectionElement section2 = bAB2.getSection(section1.getID());
	    if (section2 != null) {
		final SectionElement section = mergeSectionElements(section1,
			section2);
		dic.addSection(section);
	    }
	}
	return dic;
    }

    /**
         * 
         * @param m1
         * @param m2
         * @return
         */
    private final DictionaryElement mergeMonols(final DictionaryElement m1,
	    final DictionaryElement m2) {
	final DictionaryElement mon = new DictionaryElement();
	final AlphabetElement alphabet = mergeAlphabetElement(m1.getAlphabet(),
		m2.getAlphabet());
	mon.setAlphabet(alphabet);

	final SdefsElement sdefs = mergeSdefElements(m1.getSdefs(), m2
		.getSdefs());
	mon.setSdefs(sdefs);

	final PardefsElement pardefs = mergePardefElements(m1
		.getPardefsElement(), m2.getPardefsElement());
	mon.setPardefs(pardefs);

	for (final SectionElement section1 : m1.getSections()) {
	    final SectionElement section2 = m2.getSection(section1.getID());
	    final SectionElement section = mergeSectionElements(section1,
		    section2);
	    mon.addSection(section);
	}

	return mon;
    }

    /**
         * 
         * @param sectionE1
         * @param sectionE2
         * @return
         */
    private final SectionElement mergeSectionElements(
	    final SectionElement sectionE1, final SectionElement sectionE2) {
	final SectionElement sectionElement = new SectionElement();
	final EHashMap eMap = new EHashMap();

	sectionElement.setID(sectionE1.getID());
	sectionElement.setType(sectionE1.getType());

	final EElementList elements1 = sectionE1.getEElements();
	for (final EElement e1 : elements1) {
	    final String e1Key = e1.toString();
	    if (!eMap.containsKey(e1Key)) {
		eMap.put(e1Key, e1);
		sectionElement.addEElement(e1);
	    }
	}

	final EElementList elements2 = sectionE2.getEElements();
	for (final EElement e2 : elements2) {
	    final String e2Key = e2.toString();
	    if (!eMap.containsKey(e2Key)) {
		eMap.put(e2Key, e2);
		sectionElement.addEElement(e2);
	    }
	}

	return sectionElement;
    }

    /**
         * 
         * @param sdefs1
         * @param sdefs2
         * @return
         */
    private final SdefsElement mergeSdefElements(final SdefsElement sdefs1,
	    final SdefsElement sdefs2) {
	final SdefsElement sdefs = new SdefsElement();
	final HashMap<String, SdefElement> sdefMap = new HashMap<String, SdefElement>();

	for (final SdefElement sdef1 : sdefs1.getSdefsElements()) {
	    final String sdef1Key = sdef1.toString();
	    if (!sdefMap.containsKey(sdef1Key)) {
		sdefMap.put(sdef1Key, sdef1);
		sdefs.addSdefElement(sdef1);
	    }
	}

	for (final SdefElement sdef2 : sdefs2.getSdefsElements()) {
	    final String sdef2Key = sdef2.toString();
	    if (!sdefMap.containsKey(sdef2Key)) {
		sdefMap.put(sdef2Key, sdef2);
		sdefs.addSdefElement(sdef2);
	    }
	}
	return sdefs;
    }

    /**
         * 
         * @param alphabet1
         * @param alphabet2
         * @return
         */
    private final AlphabetElement mergeAlphabetElement(
	    final AlphabetElement alphabet1, final AlphabetElement alphabet2) {
	final AlphabetElement alphabet = new AlphabetElement();
	// We take one of them
	alphabet.setAlphabet(alphabet1.getAlphabet());
	return alphabet;
    }

    /**
         * 
         * @param pardefs1
         * @param pardefs2
         * @return
         */
    private final PardefsElement mergePardefElements(
	    final PardefsElement pardefs1, final PardefsElement pardefs2) {
	final PardefsElement pardefs = new PardefsElement();
	final HashMap<String, PardefElement> pardefMap = new HashMap<String, PardefElement>();

	for (final PardefElement pardef1 : pardefs1.getPardefElements()) {
	    final String pardef1Key = pardef1.toString();
	    if (!pardefMap.containsKey(pardef1Key)) {
		pardefMap.put(pardef1Key, pardef1);
		pardefs.addPardefElement(pardef1);
	    }
	}
	for (final PardefElement pardef2 : pardefs2.getPardefElements()) {
	    final String pardef2Key = pardef2.toString();
	    if (!pardefMap.containsKey(pardef2Key)) {
		pardefMap.put(pardef2Key, pardef2);
		pardefs.addPardefElement(pardef2);
	    }
	}
	return pardefs;
    }

    /**
         * @return the bilAB1
         */
    private final DictionaryElement getBilAB1() {
	return bilAB1;
    }

    /**
         * @param bilAB1
         *                the bilAB1 to set
         */
    private final void setBilAB1(final DictionaryElement bilAB1) {
	this.bilAB1 = bilAB1;
    }

    /**
         * @return the bilAB2
         */
    private final DictionaryElement getBilAB2() {
	return bilAB2;
    }

    /**
         * @param bilAB2
         *                the bilAB2 to set
         */
    private final void setBilAB2(final DictionaryElement bilAB2) {
	this.bilAB2 = bilAB2;
    }

    /**
         * @return the monA1
         */
    private final DictionaryElement getMonA1() {
	return monA1;
    }

    /**
         * @param monA1
         *                the monA1 to set
         */
    private final void setMonA1(final DictionaryElement monA1) {
	this.monA1 = monA1;
    }

    /**
         * @return the monA2
         */
    private final DictionaryElement getMonA2() {
	return monA2;
    }

    /**
         * @param monA2
         *                the monA2 to set
         */
    private final void setMonA2(final DictionaryElement monA2) {
	this.monA2 = monA2;
    }

    /**
         * @return the monB1
         */
    private final DictionaryElement getMonB1() {
	return monB1;
    }

    /**
         * @param monB1
         *                the monB1 to set
         */
    private final void setMonB1(final DictionaryElement monB1) {
	this.monB1 = monB1;
    }

    /**
         * @return the monB2
         */
    private final DictionaryElement getMonB2() {
	return monB2;
    }

    /**
         * @param monB2
         *                the monB2 to set
         */
    private final void setMonB2(final DictionaryElement monB2) {
	this.monB2 = monB2;
    }

}
