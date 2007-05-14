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

import dics.elements.utils.EElementList;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class SectionElement extends Element {

    /**
         * 
         */
    private String id;

    /**
         * 
         */
    private String type;

    /**
         * 
         */
    private EElementList eElements;

    /**
         * 
         * 
         */
    public SectionElement() {
	setTagName("section");
	eElements = new EElementList();
    }

    /**
         * 
         * @param id
         * @param type
         */
    public SectionElement(final String id, final String type) {
	setTagName("section");
	eElements = new EElementList();
	this.id = id;
	this.type = type;
    }

    /**
         * 
         * @param value
         */
    public final void setID(final String value) {
	id = value;
    }

    /**
         * 
         * @return
         */
    public String getID() {
	return id;
    }

    /**
         * 
         * @param value
         */
    public final void setType(final String value) {
	type = value;
    }

    /**
         * 
         * @return
         */
    public String getType() {
	return type;
    }

    /**
         * 
         * @return
         */
    public EElementList getEElements() {
	return eElements;
    }

    /**
         * 
         * @param value
         */
    public void addEElement(final EElement value) {
	eElements.add(value);
    }

    /**
         * 
         */
    @Override
    public final void printXML(final DataOutputStream dos) throws IOException {
	String attributes = "";
	if (getID() != null) {
	    attributes += " id=\"" + getID() + "\"";
	}
	if (getType() != null) {
	    attributes += " type=\"" + getType() + "\"";
	}
	dos.writeBytes(tab(1) + "<" + getTagName() + "" + attributes + ">\n");
	for (final EElement e : eElements) {
	    e.printXML(dos);
	}
	dos.writeBytes(tab(1) + "</" + getTagName() + ">\n");
    }

}
