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

import dics.elements.utils.ElementList;
import dics.elements.utils.SElementList;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class ContentElement extends Element implements Cloneable {

    /**
         * 
         */
    protected ElementList children;

    /**
         * 
         */
    protected String value;

    /**
         * 
         */
    protected String sElem;

    /**
         * 
         * 
         */
    public ContentElement() {
	children = new ElementList();
    }

    /**
         * 
         * @param value
         */
    public ContentElement(final String value) {
	children = new ElementList();
	this.value = value;
    }

    /**
         * 
         * @param e
         */
    public final void addChild(final Element e) {
	children.add(e);
    }

    /**
         * 
         * @return
         */
    public final SElementList getSElements() {
	final SElementList sEList = new SElementList();
	for (final Element e : children) {
	    if (e instanceof SElement) {
		final SElement sE = (SElement) e;
		sEList.add(sE);
	    }
	}
	return sEList;
    }

    /**
         * 
         * @return
         */
    public final String getValue() {
	return value;
    }

    /**
         * 
         * @param value
         * @return
         */
    public final boolean is(final String value) {
	if (getSElements().size() > 0) {
	    final SElement sE = getSElements().get(0);
	    if (sE != null) {
		if (sE.is(value)) {
		    return true;
		}
	    } else {
		return false;
	    }
	}
	return false;
    }

    /**
         * 
         * @return
         */
    public final boolean isAdj() {
	for (final SElement s : getSElements()) {
	    if (s.isAdj()) {
		return true;
	    }
	}
	return false;
    }

    /**
         * 
         * @return
         */
    public final boolean isN() {
	for (final SElement s : getSElements()) {
	    if (s.isN()) {
		return true;
	    }
	}
	return false;
    }

    /**
         * 
         */
    @Override
    protected void printXML(final DataOutputStream dos) throws IOException {
	if (getTagName() != null) {
	    dos.writeBytes(tab(4) + "<" + getTagName() + ">");
	} else {
	    dos.writeBytes("<!-- error tagname -->\n");
	}
	if (children != null) {
	    for (final Element e : children) {
		if (e != null) {
		    e.printXML(dos);
		}
	    }
	}
	String c = "";
	if (getComments() != null) {
	    c = getComments();
	}
	if (getTagName() != null) {
	    dos.writeBytes("</" + getTagName() + "> " + c + "\n");
	} else {
	    dos.writeBytes("<!-- error tagname -->\n");
	}
    }

    /**
         * 
         * @param value
         */
    public final void setSElem(final String value) {
	sElem += "<s n=\"" + value + "\"/>";
    }

    /**
         * 
         * @param value
         */
    public final void setValue(final String value) {
	this.value = value;
    }

    /**
         * 
         * @return
         */
    public final ElementList getChildren() {
	return children;
    }

    /**
         * 
         * @param value
         */
    public final void setChildren(final ElementList value) {
	children = value;
    }

    /**
         * 
         * @param value
         */
    public final void changeFirstSElement(final String value) {
	final SElement sE2 = new SElement(value);
	getSElements().set(0, sE2);
	int j = 0;
	for (int i = 0; i < children.size(); i++) {
	    final Element e = children.get(i);
	    if (e instanceof SElement) {
		if (j == 0) {
		    children.set(i, sE2);
		    return;
		}
		j++;
	    }

	}
    }

    /**
         * 
         * @return
         */
    private String getSElementsString() {
	String str = "";
	int i = 0;
	for (final SElement s : getSElements()) {
	    // para que no se considere la primera etiqueta, la de
	    // categoria,
	    // para encontrar paradigmas equivalentes.
	    if (i != 0) {
		str += s.toString();
	    }
	    i++;
	}
	return str;
    }

    /**
         * 
         * @return
         */
    public final String getString() {
	String str = "";
	for (final SElement s : getSElements()) {
	    str += s.toString();
	}
	return str;
    }

    /**
     * 
     * @return
     */
    public String getInfo() {
	String str = "(";
	int i = 0;
	for (final SElement s : getSElements()) {
	    // para que no se considere la primera etiqueta, la de
	    // categoria,
	    // para encontrar paradigmas equivalentes.
	    if (i != 0) {
		str += s.getValue() + ",";
	    }
	    i++;
	}
	str += ")";
	return str;
    }

    /**
         * 
         */
    @Override
    public String toString() {
	String tagName = this.getTagName();
	if (tagName == null)
	    tagName = "";

	String v = this.getValue();
	if (v == null)
	    v = "";
	
	String sList = this.getString();
	if (sList == null) {
	    sList = "";
	}
	
	String str = "<" + tagName + ">" + v + "</" + tagName  + ">" + sList;
	return str;
    }

    /**
         * 
         * 
         */
    public final void print() {
	System.err.print(value + " / ");
	final SElementList sList = getSElements();
	if (sList != null) {
	    for (final SElement s : getSElements()) {
		System.err.print(s);
	    }
	}
	System.err.println("");
    }

    /**
         * 
         */
    public Object clone() {
	ContentElement cloned = null;
	try {
	    cloned = (ContentElement) super.clone();
	    cloned.children = (ElementList) children.clone();
	} catch (Exception ex) {
	    return null;
	}
	return cloned;
    }

}
