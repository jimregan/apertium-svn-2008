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

import java.util.Collections;

import dics.elements.dtd.DictionaryElement;
import dics.elements.dtd.EElement;
import dics.elements.dtd.SectionElement;
import dics.elements.utils.EElementList;
import dics.elements.utils.EHashMap;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class DicFormat {

    /**
         * 
         */
    private DictionaryElement dicFormatted;

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
     */
    private String out;
    

    /**
         * 
         * 
         */
    public DicFormat() {

    }

    /**
         * 
         * @param dic
         */
    public DicFormat(final DictionaryElement dic) {
	dicFormatted = dic;
    }

    /**
         * 
         * @return
         */
    public final DictionaryElement format() {
	final EHashMap eMap = new EHashMap();
	for (SectionElement section : dicFormatted.getSections()) {
		int duplicated = 0;
		final EElementList elements = section.getEElements();
		for (final EElement e : elements) {
		    final String e1Key = e.toString();
		    if (!eMap.containsKey(e1Key)) {
			eMap.put(e1Key, e);
		    } else {
			//EElement other = (EElement)eMap.get(e1Key);
			String left = e.getValue("L");
			String right = e.getValue("R");
			System.err.println("Duplicated: " + left + "/" + right);
			duplicated++;
		    }
		}
		String errorMsg = duplicated + " duplicated entries in section '" + section.getID() + "'";
		System.err.println(errorMsg);
		System.out.println(errorMsg);
	}
	
	DicSort dicSort = new DicSort(dicFormatted);
	/*
	dicSort.setDicType(getDicType());
	dicSort.setOut(this.getOut());
	DictionaryElement formatted = dicSort.sort();
	formatted.printXML(getOut());
	return formatted;
	*/
	return dicFormatted;
    }

    /**
         * 
         * 
         */
    public final void doFormat() {
	processArguments();
	actionFormat();
    }

    /**
         * 
         * 
         */
    private void processArguments() {
	if (arguments[1].equals("-mon")) {
	    dicType = DicSort.MON;
	} else {
	    dicType = DicSort.BIL;
	}
	DictionaryReader dicReader = new DictionaryReader(arguments[2]);
	DictionaryElement dic = dicReader.readDic();
	dicReader = null;
	setDicFormatted(dic);
	
	if (getArguments().length == 4) {
	    if (getArguments()[3].equals("out.dix")) {
		out = DicTools.removeExtension(getArguments()[3]);
		out = out + "-formatted.dix";
	    } else {
		this.setOut(getArguments()[3]);
	    }
	}
    }

    /**
         * 
         * 
         */
    private final void actionFormat() {
	final DictionaryElement dicFormatted = format();
	    dicFormatted.printXML(this.getOut());
    }

    /**
         * @param dicFormatted
         *                the dicFormatted to set
         */
    private final void setDicFormatted(DictionaryElement dicFormatted) {
	this.dicFormatted = dicFormatted;
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

    /**
     * @return the out
     */
    public final String getOut() {
        return out;
    }

    /**
     * @param out the out to set
     */
    public final void setOut(String out) {
        this.out = out;
    }

}
