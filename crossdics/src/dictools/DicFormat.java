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
	Collections.sort(dicFormatted.getEntries());
	return dicFormatted;
    }

    /**
         * 
         * 
         */
    public final void doFormat() {
	this.processArguments();
	this.actionFormat();
    }

    /**
         * 
         * 
         */
    private void processArguments() {
	DictionaryReader dicReader = new DictionaryReader(arguments[1]);
	DictionaryElement dic = dicReader.readDic();
	dicReader = null;
	this.setDicFormatted(dic);
    }

    /**
         * 
         * 
         */
    private final void actionFormat() {
	final DictionaryElement dicFormatted = format();

	String formattedFileName = "formatted-dic.dix";
	if (getArguments().length == 3) {
	    if (getArguments()[2].equals("out.dix")) {
		formattedFileName = DicTools.removeExtension(getArguments()[2]);
		formattedFileName = formattedFileName + "-formatted.dix";
	    } else {
		formattedFileName = getArguments()[2];
	    }
	    dicFormatted.printXML(formattedFileName);
	}
    }

    /**
         * @return the dicFormatted
         */
    private final DictionaryElement getDicFormatted() {
	return dicFormatted;
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

}
