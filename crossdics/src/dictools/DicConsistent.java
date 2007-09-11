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

import java.util.Iterator;
import java.util.Set;

import dics.elements.dtd.DictionaryElement;
import dics.elements.dtd.EElement;
import dics.elements.utils.DicSet;
import dics.elements.utils.EElementList;
import dics.elements.utils.EElementMap;
import dics.elements.utils.Msg;

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
    private EElementMap commonA;

    /**
         * 
         */
    private EElementMap commonC;

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
         */
    private String notCommonSuffix;

    /**
         * 
         */
    private String[] arguments;

    /**
         * 
         */
    private DicSet dicSet;

    /**
         * 
         */
    private Msg msg;

    /**
         * 
         * 
         */
    public DicConsistent() {
	msg = new Msg();
    }

    /**
         * 
         * 
         */
    public DicConsistent(final DicSet dicset) {
	msg = new Msg();
	setMon1(dicset.getMon1());
	setMon2(dicset.getMon2());
	setBil1(dicset.getBil1());
	setBil2(dicset.getBil2());
	differentA = new EElementMap();
	differentC = new EElementMap();
	commonA = new EElementMap();
	commonC = new EElementMap();
	setNotCommonSuffix("-not-common.dix");
    }

    /**
         * 
         * 
         */
    public final void doConsistent() {
	processArguments();
	actionConsistent("yes");
    }

    /**
         * 
         * @param entries1
         * @param entries2
         */
    private final void compare() {
	final EElementMap bilAMap = DicTools.buildHash(getBil1().getEntries());
	final EElementMap bilCMap = DicTools.buildHash(getBil2().getEntries());
	final EElementMap monAMap = DicTools.buildHashMon(getMon1()
		.getEntries());
	final EElementMap monCMap = DicTools.buildHashMon(getMon2()
		.getEntries());

	// markCommonEntries(bilAMap, bilCMap, monAMap, commonA, differentA);
	// markCommonEntries(bilCMap, bilAMap, monCMap, commonC, differentC);
	markCommonEntries(bilAMap, bilCMap, monAMap, getCommonA(),
		getDifferentA());
	markCommonEntries(bilCMap, bilAMap, monCMap, getCommonC(),
		getDifferentC());
    }

    /**
         * 
         * 
         */
    private final void buildNotCommonDictionaries() {
	buildNotCommonDictionary(getBil1());
	buildNotCommonDictionary(getBil2());
	buildNotCommonDictionary(getMon1());
	buildNotCommonDictionary(getMon2());
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

	dicNotCommon.printXML(fnDic + getNotCommonSuffix());
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
    private final void markCommonEntries(final EElementMap bil1Map,
	    final EElementMap bil2Map, final EElementMap monMap,
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
	    final EElementMap mon) {
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
	getMon1().removeNotCommon();
	getMon2().removeNotCommon();
	getBil1().removeNotCommon();
	getBil2().removeNotCommon();
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

    /**
         * @return the bil1
         */
    public final DictionaryElement getBil1() {
	return bil1;
    }

    /**
         * @param bil1
         *                the bil1 to set
         */
    public final void setBil1(final DictionaryElement bil1) {
	this.bil1 = bil1;
    }

    /**
         * @return the bil2
         */
    public final DictionaryElement getBil2() {
	return bil2;
    }

    /**
         * @param bil2
         *                the bil2 to set
         */
    public final void setBil2(final DictionaryElement bil2) {
	this.bil2 = bil2;
    }

    /**
         * @return the mon1
         */
    public final DictionaryElement getMon1() {
	return mon1;
    }

    /**
         * @param mon1
         *                the mon1 to set
         */
    private final void setMon1(final DictionaryElement mon1) {
	this.mon1 = mon1;
    }

    /**
         * @return the mon2
         */
    public final DictionaryElement getMon2() {
	return mon2;
    }

    /**
         * @param mon2
         *                the mon2 to set
         */
    private final void setMon2(final DictionaryElement mon2) {
	this.mon2 = mon2;
    }

    /**
         * @return the commonA
         */
    public final EElementMap getCommonA() {
	return commonA;
    }

    /**
         * @return the commonC
         */
    public final EElementMap getCommonC() {
	return commonC;
    }

    /**
         * @return the notCommonSuffix
         */
    public final String getNotCommonSuffix() {
	return notCommonSuffix;
    }

    /**
         * @param notCommonSuffix
         *                the notCommonSuffix to set
         */
    private final void setNotCommonSuffix(final String notCommonSuffix) {
	this.notCommonSuffix = notCommonSuffix;
    }

    /**
         * 
         * @param removeNotCommon
         * @return
         */
    private final DicConsistent actionConsistent(String removeNotCommon) {
	final DicConsistent dicConsistent = new DicConsistent(getDicSet());
	dicConsistent.makeConsistentDictionaries(removeNotCommon);
	dicSet.printXML("consistent");
	return dicConsistent;
    }

    /**
         * 
         * 
         */
    private void processArguments() {
	final int nArgs = getArguments().length;
	String sDicMonA, sDicMonC, sDicBilAB, sDicBilBC;
	sDicMonA = sDicMonC = sDicBilAB = sDicBilBC = null;
	boolean bilABReverse, bilBCReverse;
	bilABReverse = bilBCReverse = false;

	for (int i = 1; i < nArgs; i++) {
	    String arg = getArguments()[i];
	    if (arg.equals("-monA")) {
		i++;
		arg = getArguments()[i];
		sDicMonA = arg;
		msg.err("Monolingual A: '" + sDicMonA + "'");
	    }

	    if (arg.equals("-monC")) {
		i++;
		arg = getArguments()[i];
		sDicMonC = arg;
		msg.err("Monolingual C: '" + sDicMonC + "'");
	    }

	    if (arg.equals("-bilAB")) {
		i++;
		arg = getArguments()[i];
		if (arg.equals("-r")) {
		    bilABReverse = true;
		    i++;
		}
		if (arg.equals("-n")) {
		    bilABReverse = false;
		    i++;
		}

		arg = getArguments()[i];
		sDicBilAB = arg;
		msg.err("Bilingual A-B: '" + sDicBilAB + "'");
	    }

	    if (arg.equals("-bilBC")) {
		i++;
		arg = getArguments()[i];

		if (arg.equals("-r")) {
		    bilBCReverse = true;
		    i++;
		}
		if (arg.equals("-n")) {
		    bilBCReverse = false;
		    i++;
		}
		arg = getArguments()[i];
		sDicBilBC = arg;
		msg.err("Bilingual B-C: '" + sDicBilBC + "'");
	    }

	}

	final DictionaryElement bil1 = DicTools.readBilingual(sDicBilAB,
		bilABReverse);
	final DictionaryElement bil2 = DicTools.readBilingual(sDicBilBC,
		bilBCReverse);
	final DictionaryElement mon1 = DicTools.readMonolingual(sDicMonA);
	final DictionaryElement mon2 = DicTools.readMonolingual(sDicMonC);

	DicSet dicSet = new DicSet(mon1, bil1, mon2, bil2);
	setDicSet(dicSet);
    }

    /**
         * @return the arguments
         */
    private final String[] getArguments() {
	return arguments;
    }

    /**
         * @param arguments
         *                the arguments to set
         */
    public final void setArguments(String[] arguments) {
	this.arguments = arguments;
    }

    /**
         * @return the dicSet
         */
    private final DicSet getDicSet() {
	return dicSet;
    }

    /**
         * @param dicSet
         *                the dicSet to set
         */
    private final void setDicSet(DicSet dicSet) {
	this.dicSet = dicSet;
    }

}
