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

import java.io.File;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Set;

import dics.elements.dtd.DictionaryElement;
import dics.elements.dtd.EElement;
import dics.elements.dtd.SectionElement;
import dics.elements.utils.EElementList;
import dics.elements.utils.SElementList;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class DicSort {

    /**
         * 
         */
    private DictionaryElement dic;

    /**
         * 
         */
    private String[] arguments;

    /**
         * 
         */
    public static final int BIL = 0;

    /**
         * 
         */
    public static final int MON = 1;

    /**
         * 
         */
    private int dicType;

    /**
         * 
         * 
         */
    public DicSort() {

    }

    /**
         * 
         * @param dic
         */
    public DicSort(final DictionaryElement dic) {
	this.dic = dic;
    }

    /**
         * 
         * @return
         */
    public final DictionaryElement sort() {
	DictionaryElement dicSorted = null;
	System.out.println("Dictype: " + dicType);
	if (this.dicType == BIL) {
	    dicSorted = sortBil();
	}

	if (this.dicType == MON) {
	    dicSorted = sortMon();
	}
	
	return dicSorted;
    }

    /**
         * 
         * 
         */
    public final void doSort() {
	this.processArguments();
	this.actionSort();
    }

    /**
         * 
         * 
         */
    private void processArguments() {
	if (arguments[1].equals("-mon")) {
	    this.dicType = MON;
	} else {
	    this.dicType = BIL;
	}
	DictionaryReader dicReader = new DictionaryReader(arguments[2]);
	DictionaryElement dic = dicReader.readDic();
	dic.setFileName(arguments[2]);
	dicReader = null;
	setDic(dic);
    }

    /**
         * 
         * 
         */
    public final void actionSort() {
	DictionaryElement dicSorted = null;
	//System.out.println("Dictype: " + dicType);
	if (this.dicType == BIL) {
	    dicSorted = sortBil();
	}

	if (this.dicType == MON) {
	    dicSorted = sortMon();
	}

	if (dicSorted != null) {
	    String formattedFileName = "sorted-dic.dix";
	    if (getArguments().length == 4) {
		if (getArguments()[3].equals("out.dix")) {
		    formattedFileName = DicTools
			    .removeExtension(getArguments()[3]);
		    formattedFileName = formattedFileName + "-sorted.dix";
		} else {
		    formattedFileName = getArguments()[3];
		}
		//dicSorted.printXMLXInclude(formattedFileName);
		dicSorted.printXML(formattedFileName);
	    }
	}
    }

    /**
         * @return the dicFormatted
         */
    private final DictionaryElement getDic() {
	return dic;
    }

    /**
         * @param dicFormatted
         *                the dicFormatted to set
         */
    private final void setDic(DictionaryElement dic) {
	this.dic = dic;
    }

    /**
         * @return the arguments
         */
    public final String[] getArguments() {
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
         * 
         * 
         */
    private final DictionaryElement sortBil() {
	int lrs = 0;
	int rls = 0;
	int n = 0;

	for (SectionElement section : dic.getSections()) {
	    EElementList eList = section.getEElements();
	    HashMap<String, EElementList> map = new HashMap<String, EElementList>();

	    for (EElement e : eList) {
		n++;
		SElementList sList = e.getSElements("L");
		if (e.hasRestriction()) {
		    String r = e.getRestriction();
		    if (r.equals("LR")) {
			lrs++;
		    }
		    if (r.equals("RL")) {
			rls++;
		    }
		}
		String cat;
		if (sList != null) {
		    if (sList.size() > 0) {
			cat = (String) sList.get(0).getValue();
		    } else {
			cat = "(none)";
		    }
		    EElementList l;
		    if (map.containsKey(cat)) {
			l = map.get(cat);
			l.add(e);
			map.put(cat, l);
		    } else {
			l = new EElementList();
			l.add(e);
			map.put(cat, l);
		    }
		}
	    }
	    System.out.println("lemmas: " + n);
	    System.out.println("LR: " + lrs);
	    System.out.println("RL: " + rls);

	    Set keySet = map.keySet();
	    Iterator it = keySet.iterator();

	    EElementList listAll = new EElementList();
	    while (it.hasNext()) {
		String cat = (String) it.next();
		EElementList list = (EElementList) map.get(cat);
		System.out.println(cat + ": " + list.size());
		if (list.size() > 0) {
		    Collections.sort(list);
		    EElement eHead = list.get(0);
		    eHead.addComments("******************************");
		    eHead.addComments("(" + cat + ") group");
		    eHead.addComments("******************************");
		    listAll.addAll(list);
		}
	    }
	    section.setEElements(listAll);

	}
	return dic;
    }

    /**
         * 
         * @return
         */
    private final DictionaryElement sortMon() {
	int lrs = 0;
	int rls = 0;

	int n = 0;
	for (SectionElement section : dic.getSections()) {
	    if (section.getID().equals("main")) {
		EElementList eList = section.getEElements();
		HashMap<String, EElementList> map = new HashMap<String, EElementList>();

		for (EElement e : eList) {
		    n++;
		    String par = e.getParadigmValue();
			if (e.hasRestriction()) {
			    String r = e.getRestriction();
			    if (r.equals("LR")) {
				lrs++;
			    }
			    if (r.equals("RL")) {
				rls++;
			    }
			}

		    String cat = null;
		    if (par == null) {
			cat = "(none)";
		    } else {
			String[] aux = par.split("__");
			if (aux.length == 2) {
			    cat = aux[1];
			} else {
			    cat = par;
			}
		    }
		    if (cat != null) {
			EElementList l;
			if (map.containsKey(cat)) {
			    l = map.get(cat);
			    l.add(e);
			    map.put(cat, l);
			} else {
			    l = new EElementList();
			    l.add(e);
			    map.put(cat, l);
			}
		    }

		}
		    System.out.println("lemmas: " + n);
		    System.out.println("LR: " + lrs);
		    System.out.println("RL: " + rls);

		Set keySet = map.keySet();
		Iterator it = keySet.iterator();

		EElementList listAll = new EElementList();
		boolean status = new File("categories").mkdir();
		while (it.hasNext()) {
		    DictionaryElement d = new DictionaryElement();
		    SectionElement sec = new SectionElement();
		    d.addSection(sec);
		    
		    String cat = (String) it.next();
		    EElementList list = (EElementList) map.get(cat);
		    System.out.println(cat + ": " + list.size());
		    dic.addXInclude("<xi:include xmlns:xi=\"http://www.w3.org/2001/XInclude\" href=\"" + "categories/" + cat + "-category.xml\"/>");
		    if (list.size() > 0) {
			Collections.sort(list);
			EElement eHead = list.get(0);
			eHead.addComments("******************************");
			eHead.addComments("(" + cat + ") group");
			eHead.addComments("******************************");
			listAll.addAll(list);
		    }
		    sec.setEElements(list);
		    sec.printXML( "categories/" + cat + "-category.dix");
		    
		}
		section.setEElements(listAll);
	    }
	}
	return dic;
    }

    /**
     * @return the dicType
     */
    public final int getDicType() {
        return dicType;
    }

    /**
     * @param dicType the dicType to set
     */
    public final void setDicType(int dicType) {
        this.dicType = dicType;
    }
}
