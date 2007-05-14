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
 * @author Enrique Benimeli Bofarull
 *
 */
public class HowTo {

    /**
     * @param args
     */
    public static void main(String[] args) {

	// HOW TO READ A DICTIONARY
	
	// Dic Readers
	DictionaryReader monAReader = new DictionaryReader("dics/apertium-es-ca.ca.dix");
	DictionaryReader monCReader = new DictionaryReader("dics/apertium-es-pt.pt.dix");
	DictionaryReader bilABReader = new DictionaryReader("dics/apertium-es-ca.es-ca.dix");	
	DictionaryReader bilBCReader = new DictionaryReader("dics/apertium-es-pt.es-pt.dix");

	// Dictionaries
	DictionaryElement monA = monAReader.readDic();
	DictionaryElement monC = monCReader.readDic();
	DictionaryElement bilAB = bilABReader.readDic();
	DictionaryElement bilBC = bilBCReader.readDic();
	DicSet dicSet = new DicSet(monA, bilAB, monC, bilBC);
	
	// HOW TO CROSS DICTIONARIES
	DicCross dc = new DicCross();
	DictionaryElement[] bils = dc.crossDictionaries(dicSet);
	DictionaryElement bilACcrossed = bils[0];
	

	// Morfological dictionaries (consistent with bilingual)
	DicSet solDicSet = DicTools.makeConsistentBilAndMonols(bilACcrossed, monA, monC);
	DictionaryElement monAcrossed = solDicSet.getMon1();
	DictionaryElement monCcrossed = solDicSet.getMon2();
	
	// HOW TO PRINT TO FILE
	bilACcrossed.printXML("dix/apertium-ca-pt.ca-pt-crossed.dix");
	
	// HOW TO REVERSE A DICTIONARY
	bilACcrossed.reverse();
	bilACcrossed.printXML("dix/apertium-pt-ca.pt-ca-crossed.dix");
	
	monAcrossed.printXML("dix/apertium-ca-pt.ca-crossed.dix");
	monCcrossed.printXML("dix/apertium-ca-pt.pt-crossed.dix");
	
	// HOW TO JOIN TWO COLLECTIONS
	DicSet toMerge = new DicSet(bilACcrossed, monCcrossed, monAcrossed);
	DicMerge dm = new DicMerge(toMerge, toMerge);
	DicSet merged = dm.merge();
	// we get the same dicSet
	DictionaryElement bilACmerged = merged.getBil1();
	DictionaryElement monAmerged = merged.getMon1();
	DictionaryElement monCmerged = merged.getMon2();
	bilACmerged.printXML("dix/apertium-pt-ca.pt-ca-merged.dix");
		
	// How to iterate over the elements in 'bilACcrossed'
	// for each <e> tag
	for (EElement e : bilACcrossed.getEntries()) {
	    // 'r' attribute in <e>
	    String r = e.getRestriction();
	    if (r != null) {
	    System.out.println("R: " + r);
	    }
	    
	    // Left side (<l> tag)
	    System.out.print(e.getValue("L") + " / ");
	    // <s> tags in <l>
	    SElementList sList = e.getSElements("L");
	    for (SElement s : sList) {
		System.out.print(s);
	    }
	    System.out.println("");

	    // Right side (<r> tag)
	    System.out.print(e.getValue("R") + " / ");
	    // <s> tags in <r>
	    SElementList sList2 = e.getSElements("R");
	    for (SElement s : sList2) {
		System.out.print(s);
	    }
	    System.out.println("\n");	    
	}
	
	
	
    }

}
