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

import dics.elements.dtd.DictionaryElement;
import dics.elements.dtd.EElement;
import dics.elements.dtd.SElement;
import dics.elements.utils.DicSet;
import dics.elements.utils.SElementList;
import dictools.DicCross;
import dictools.DicMerge;
import dictools.DicTools;
import dictools.DictionaryReader;

/**
 * HowTo class
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class HowTo {

    /**
         * @param args
         */
    public static void main(final String[] args) {

	// HOW TO READ A DICTIONARY

	// Dic Readers
	final DictionaryReader monAReader = new DictionaryReader(
		"dics/apertium-es-ca.ca.dix");
	final DictionaryReader monCReader = new DictionaryReader(
		"dics/apertium-es-pt.pt.dix");
	final DictionaryReader bilABReader = new DictionaryReader(
		"dics/apertium-es-ca.es-ca.dix");
	final DictionaryReader bilBCReader = new DictionaryReader(
		"dics/apertium-es-pt.es-pt.dix");

	// Dictionaries
	final DictionaryElement monA = monAReader.readDic();
	final DictionaryElement monC = monCReader.readDic();
	final DictionaryElement bilAB = bilABReader.readDic();
	final DictionaryElement bilBC = bilBCReader.readDic();
	final DicSet dicSet = new DicSet(monA, bilAB, monC, bilBC);

	// HOW TO CROSS DICTIONARIES
	final DicCross dc = new DicCross();
	final DictionaryElement[] bils = dc.crossDictionaries(dicSet);
	final DictionaryElement bilACcrossed = bils[0];

	// Morfological dictionaries (consistent with bilingual)
	final DicSet solDicSet = DicTools.makeConsistentBilAndMonols(
		bilACcrossed, monA, monC);
	final DictionaryElement monAcrossed = solDicSet.getMon1();
	final DictionaryElement monCcrossed = solDicSet.getMon2();

	// HOW TO PRINT TO FILE
	bilACcrossed.printXML("dix/apertium-ca-pt.ca-pt-crossed.dix");

	// HOW TO REVERSE A DICTIONARY
	bilACcrossed.reverse();
	bilACcrossed.printXML("dix/apertium-pt-ca.pt-ca-crossed.dix");

	monAcrossed.printXML("dix/apertium-ca-pt.ca-crossed.dix");
	monCcrossed.printXML("dix/apertium-ca-pt.pt-crossed.dix");

	// HOW TO JOIN TWO COLLECTIONS
	final DicSet toMerge = new DicSet(bilACcrossed, monCcrossed,
		monAcrossed);
	final DicMerge dm = new DicMerge(toMerge, toMerge);
	final DicSet merged = dm.merge();
	// we get the same dicSet
	final DictionaryElement bilACmerged = merged.getBil1();
	final DictionaryElement monAmerged = merged.getMon1();
	final DictionaryElement monCmerged = merged.getMon2();
	bilACmerged.printXML("dix/apertium-pt-ca.pt-ca-merged.dix");

	// How to iterate over the elements in 'bilACcrossed'
	// for each <e> tag
	for (final EElement e : bilACcrossed.getEntries()) {
	    // 'r' attribute in <e>
	    final String r = e.getRestriction();
	    if (r != null) {
		System.out.println("R: " + r);
	    }

	    // Left side (<l> tag)
	    System.out.print(e.getValue("L") + " / ");
	    // <s> tags in <l>
	    final SElementList sList = e.getSElements("L");
	    for (final SElement s : sList) {
		System.out.print(s);
	    }
	    System.out.println("");

	    // Right side (<r> tag)
	    System.out.print(e.getValue("R") + " / ");
	    // <s> tags in <r>
	    final SElementList sList2 = e.getSElements("R");
	    for (final SElement s : sList2) {
		System.out.print(s);
	    }
	    System.out.println("\n");
	}

    }

}
