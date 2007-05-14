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
import java.util.Iterator;
import java.util.Set;

import dics.elements.dtd.DictionaryElement;
import dics.elements.dtd.EElement;
import dics.elements.utils.DicSet;
import dics.elements.utils.EElementList;
import dics.elements.utils.EElementMap;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class DicConsistent {

    /**
         * 
         */
    private DictionaryElement mon1;

    /**
         * 
         */
    private DictionaryElement mon2;

    /**
         * 
         */
    private DictionaryElement bil1;

    /**
         * 
         */
    private DictionaryElement bil2;

    /**
         * 
         */
    private EElementMap differentA;

    /**
         * 
         */
    private EElementMap differentC;

    /**
         * 
         * 
         */
    public DicConsistent(final DicSet dicset) {
	mon1 = dicset.getMon1();
	mon2 = dicset.getMon2();
	bil1 = dicset.getBil1();
	bil2 = dicset.getBil2();
    }

    /**
         * 
         * @param entries1
         * @param entries2
         */
    private final void compare() {
	EElementMap commonA = new EElementMap();
	EElementMap commonC = new EElementMap();

	differentA = new EElementMap();
	differentC = new EElementMap();

	final HashMap<String, EElementList> bilAMap = DicTools.buildHash(bil1
		.getEntries());
	final HashMap<String, EElementList> bilCMap = DicTools.buildHash(bil2
		.getEntries());
	final HashMap<String, EElementList> monAMap = DicTools
		.buildHashMon(mon1.getEntries());
	final HashMap<String, EElementList> monCMap = DicTools
		.buildHashMon(mon2.getEntries());

	// markCommonEntries(bilAMap, bilCMap, monAMap, commonA, differentA);
	// markCommonEntries(bilCMap, bilAMap, monCMap, commonC, differentC);
	markCommonEntries(bilAMap, bilCMap, monAMap, commonA, differentA);
	markCommonEntries(bilCMap, bilAMap, monCMap, commonC, differentC);

	commonA = commonC = null;
	differentA = differentC = null;
    }

    /**
         * 
         * 
         */
    private final void buildNotCommonDictionaries() {
	buildNotCommonDictionary(bil1);
	buildNotCommonDictionary(bil2);
	buildNotCommonDictionary(mon1);
	buildNotCommonDictionary(mon2);
    }

    /**
         * 
         * @param dic
         */
    private final void buildNotCommonDictionary(final DictionaryElement dic) {
	DictionaryElement dicNotCommon = new DictionaryElement(dic);
	String fnDic = dic.getFileName();
	fnDic = fnDic.replaceAll("\\.dix", "");
	fnDic = fnDic.replaceAll("\\.metadix", "");
	fnDic = fnDic.replaceAll("/dics/", "/dix/");

	dicNotCommon.printXML(fnDic + "-not-common.dix");
	dicNotCommon = null;

    }

    /**
         * 
         * @param bil1Map
         * @param bil2Map
         * @param monMap
         * @param common
         * @param different
         * @param comm
         */
    private final void markCommonEntries(
	    final HashMap<String, EElementList> bil1Map,
	    final HashMap<String, EElementList> bil2Map,
	    final HashMap<String, EElementList> monMap,
	    final EElementMap common, final EElementMap different) {

	final Set<String> keysBil1 = bil1Map.keySet();
	final Iterator<String> itBil1 = keysBil1.iterator();

	while (itBil1.hasNext()) {
	    final String str = itBil1.next();
	    final EElementList eList = bil1Map.get(str);
	    if (bil2Map.containsKey(str)) {
		common.put(str, eList);
		markShared(common, str, monMap);
	    } else {
		different.put(str, eList);
	    }
	}

    }

    /**
         * 
         * @param commonA
         * @param str
         * @param monA
         */
    private final void markShared(final EElementMap common, final String str,
	    final HashMap<String, EElementList> mon) {
	final String k = DicTools.clearTags(str);
	final EElementList list = common.get(k);
	for (final EElement e : list) {
	    e.setShared(true);
	    final String trad = e.getValue("R");
	    final String key = DicTools.clearTags(trad);
	    final EElementList monAList = mon.get(key);
	    if (monAList != null) {
		for (final EElement eMon : monAList) {
		    eMon.setShared(true);
		}
	    }
	}
    }

    /**
         * 
         * @param removeNotShared
         */
    public final void makeConsistentDictionaries(final String removeNotShared) {
	compare();
	buildNotCommonDictionaries();
	if (removeNotShared.equals("yes")) {
	    removeNotShared();
	}
    }

    /**
         * 
         * 
         */
    private final void removeNotShared() {
	mon1.removeNotCommon();
	mon2.removeNotCommon();
	bil1.removeNotCommon();
	bil2.removeNotCommon();
    }

    /**
         * 
         * @return
         */
    public final EElementMap getDifferentA() {
	return differentA;
    }

    /**
         * 
         * @return
         */
    public final EElementMap getDifferentC() {
	return differentC;
    }

}
