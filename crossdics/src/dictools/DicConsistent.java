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
	 * 
	 */
	public DicConsistent(final DicSet dicset) {
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
	 * @param entries1
	 * @param entries2
	 */
	private final void compare() {
		final EElementMap bilAMap = DicTools.buildHash(getBil1().getEntries());
		final EElementMap bilCMap = DicTools.buildHash(getBil2().getEntries());
		final EElementMap monAMap = DicTools.buildHashMon(getMon1().getEntries());
		final EElementMap monCMap = DicTools.buildHashMon(getMon2().getEntries());

		// markCommonEntries(bilAMap, bilCMap, monAMap, commonA, differentA);
		// markCommonEntries(bilCMap, bilAMap, monCMap, commonC, differentC);
		markCommonEntries(bilAMap, bilCMap, monAMap, getCommonA(), getDifferentA());
		markCommonEntries(bilCMap, bilAMap, monCMap, getCommonC(), getDifferentC());
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
	 * @param bil1 the bil1 to set
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
	 * @param bil2 the bil2 to set
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
	 * @param mon1 the mon1 to set
	 */
	public final void setMon1(final DictionaryElement mon1) {
		this.mon1 = mon1;
	}

	/**
	 * @return the mon2
	 */
	public final DictionaryElement getMon2() {
		return mon2;
	}

	/**
	 * @param mon2 the mon2 to set
	 */
	public final void setMon2(final DictionaryElement mon2) {
		this.mon2 = mon2;
	}

	/**
	 * @param differentA the differentA to set
	 */
	public final void setDifferentA(final EElementMap differentA) {
		this.differentA = differentA;
	}

	/**
	 * @param differentC the differentC to set
	 */
	public final void setDifferentC(final EElementMap differentC) {
		this.differentC = differentC;
	}

	/**
	 * @return the commonA
	 */
	public final EElementMap getCommonA() {
		return commonA;
	}

	/**
	 * @param commonA the commonA to set
	 */
	public final void setCommonA(final EElementMap commonA) {
		this.commonA = commonA;
	}

	/**
	 * @return the commonC
	 */
	public final EElementMap getCommonC() {
		return commonC;
	}

	/**
	 * @param commonC the commonC to set
	 */
	public final void setCommonC(final EElementMap commonC) {
		this.commonC = commonC;
	}

	/**
	 * @return the notCommonSuffix
	 */
	public final String getNotCommonSuffix() {
		return notCommonSuffix;
	}

	/**
	 * @param notCommonSuffix the notCommonSuffix to set
	 */
	public final void setNotCommonSuffix(final String notCommonSuffix) {
		this.notCommonSuffix = notCommonSuffix;
	}

}
