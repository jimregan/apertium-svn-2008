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

package dics.elements.dtd;

import java.io.DataOutputStream;
import java.io.IOException;
import java.util.HashMap;

/**
 * 
 * @author Enrique Benimeli Bofarull
 *
 */
public class SElement extends Element {

    /**
         * 
         */
    private static final HashMap<String, SElement> sElementList = new HashMap<String, SElement>();

    /**
         * 
         */
    private String n;

    /**
         * 
         */
    private String temp;

    /**
         * 
         * 
         */
    public SElement() {
	setTagName("s");
    }

    /**
         * 
         * @param value
         */
    public SElement(final String value) {
	setTagName("s");
	n = value;
    }

    /*
         * 
         */
    public SElement(final SElement sE) {
	n = sE.getValue();
    }

    /**
         * 
         * @return
         */
    @Override
    public final String getValue() {
	return n;
    }

    /**
         * 
         * @param value
         */
    @Override
    public final void setValue(final String value) {
	n = value;
    }

    @Override
    public final void printXML(final DataOutputStream dos) throws IOException {
	dos.writeBytes("<" + getTagName() + " n=\"" + getValue() + "\"/>");
    }

    /**
         * 
         * @return
         */
    public final boolean isAdj() {
	return is("adj");
    }

    /**
         * 
         * @return
         */
    public final boolean isN() {
	return is("n");
    }

    /**
         * 
         * @param value
         * @return
         */
    public final boolean is(final String value) {
	if (n.equals(value)) {
	    return true;
	}
	return false;
    }

    /**
         * 
         */
    @Override
    public String toString() {
	return "<" + getValue() + ">";
    }

    /**
         * 
         * @param sE
         * @return
         */
    public final boolean equals(final SElement sE) {
	return (getValue().equals(sE.getValue()));
    }

    /**
         * 
         * @param sE
         */
    public static final void putSElement(final SElement sE) {
	SElement.sElementList.put(sE.getValue(), sE);
    }

    /**
         * 
         * @param str
         * @return
         */
    public static final boolean exists(final String str) {
	return SElement.sElementList.containsKey(str);
    }

    /**
         * 
         * @param str
         * @return
         */
    public static final SElement get(final String str) {
	SElement sE = null;
	if (SElement.exists(str)) {
	    sE = SElement.sElementList.get(str);
	} else {
	    sE = new SElement(str);
	    SElement.putSElement(sE);
	}
	return sE;
    }

    public final String getTemp() {
	return temp;
    }

    public final void setTemp(String temp) {
	this.temp = temp;
    }

    @Override
    public Object clone() {
	try {
	    final SElement cloned = (SElement) super.clone();
	    return cloned;
	} catch (final Exception ex) {
	    return null;
	}
    }

}
