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

package dics.elements.utils;

import java.util.ArrayList;

import dics.elements.dtd.Element;
import dics.elements.dtd.SElement;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class ElementList extends ArrayList<Element> implements Cloneable {

    /**
         * 
         */
    static final long serialVersionUID = 0;

    /**
         * 
         * 
         */
    public ElementList() {
	super();
    }

    /**
         * 
         */
    @Override
    public Object clone() {
	try {
	    final ElementList cloned = (ElementList) super.clone();
	    for (int i = 0; i < size(); i++) {
		final Element eCloned = (Element) cloned.get(i).clone();
		cloned.set(i, eCloned);
	    }
	    return cloned;
	} catch (final Exception ex) {
	    return null;
	}
    }

    /**
         * 
         * 
         */
    public final void print() {
	if (size() > 0) {
	    for (Element e : this) {
		String tmp = "";
		if (e instanceof SElement) {
		    tmp = ((SElement) e).getTemp();
		    if (tmp == null) {
			tmp = "";
		    }
		}
		System.out.print("<" + e.getValue() + "/" + tmp + ">");
	    }
	    System.out.println("");
	}
    }

        public final void printErr() {
	if (size() > 0) {
	    for (Element e : this) {
		String tmp = "";
		if (e instanceof SElement) {
		    tmp = ((SElement) e).getTemp();
		    if (tmp == null) {
			tmp = "";
		    }
		}
		System.err.print("<" + e.getValue() + "/" + tmp + ">");
	    }
	    System.err.println("");
	}
    }

    /**
         * 
         */
    @Override
    public final String toString() {
	String str = "";
	if (size() > 0) {
	    for (Element e : this) {
		String tmp = "";
		if (e instanceof SElement) {
		    tmp = ((SElement) e).getTemp();
		    if (tmp == null) {
			tmp = "";
		    }
		}
		str += "<" + e.getValue() + "/" + tmp + ">";
	    }
	}
	return str;
    }

}
