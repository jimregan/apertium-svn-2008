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
	setValue(value);
    }

    /**
         * 
         * @param cE
         */
    public ContentElement(final ContentElement cE) {
	children = (ElementList) cE.getChildren().clone();
	this.value = new String(cE.getValue());
    }

    /**
         * 
         * @param e
         */
    public final void addChild(final Element e) {
	getChildren().add(e);
    }

    /**
         * 
         * @return
         */
    public final SElementList getSElements() {
	final SElementList sEList = new SElementList();
	for (final Element e : getChildren()) {
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
    @Override
    public final String getValueNoTags() {
	String str = "";
	for (Element e : children) {
	    if (!(e instanceof SElement)) {
		if (e instanceof TextElement) {
		    TextElement tE = (TextElement)e;
		    str += tE.getValue();
		} else {
		    str += e.getValueNoTags();
		}
	    }
	}
	return str;
    }

    /**
         * 
         * @return
         */
    @Override
    public final String getValue() {
	String str = "";
	for (Element e : children) {
	    if (!(e instanceof SElement)) {
		if (e instanceof GElement) {
		    str += "<g>" + ((GElement) e).getValue() + "</g>";
		} else {
		    str += e.getValue();
		}
	    }
	}
	return str;
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
         */
    @Override
    protected void printXML(final DataOutputStream dos) throws IOException {
	if (getTagName() != null) {
	    dos.writeBytes(tab(4) + "<" + getTagName() + ">");
	} else {
	    dos.writeBytes("<!-- error tagname -->\n");
	}
	if (getChildren() != null) {
	    for (final Element e : getChildren()) {
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
         */
    protected void printXML1Line(final DataOutputStream dos) throws IOException {
	if (getTagName() != null) {
	    dos.writeBytes("<" + getTagName() + ">");
	} else {
	    dos.writeBytes("<!-- error tagname -->");
	}
	if (getChildren() != null) {
	    for (final Element e : getChildren()) {
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
	    dos.writeBytes("</" + getTagName() + ">" + c + "");
	} else {
	    dos.writeBytes("<!-- error tagname -->\n");
	}
    }

    /**
         * 
         * @param value
         */
    @Override
    public final void setValue(final String value) {
	boolean textE = false;
	for (final Element e : getChildren()) {
	    if (e instanceof TextElement) {
		textE = true;
		((TextElement) e).setValue(value);
	    }
	}
	if (!textE) {
	    final TextElement tE = new TextElement(value);
	    getChildren().add(0, tE);
	}
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
    public String toStringOld() {
	String tagName = getTagName();
	if (tagName == null) {
	    tagName = "";
	}

	String v = getValue();
	if (v == null) {
	    v = "";
	}

	String sList = getString();
	if (sList == null) {
	    sList = "";
	}

	final String str = "<" + tagName + ">" + v + sList + "</" + tagName
		+ ">";
	return str;
    }

    /**
     * 
     */
    public String toString() {
	String str = "";
	
	String tagName = getTagName();
	if (tagName == null) {
	    tagName = "";
	}
	
	str += "<" + tagName + ">";
	for (Element e : this.getChildren()) {
	    String v = e.toString();
	    str += v;
	}
	str += "</" + tagName + ">";
	
	return str;
    }

    /**
         * toString() without lemma
         * 
         * @return
         */
    public String toString2() {
	String tagName = getTagName();
	if (tagName == null) {
	    tagName = "";
	}

	String sList = getString();
	if (sList == null) {
	    sList = "";
	}

	// final String str = "<" + tagName + ">" + "</" + tagName + ">" +
	// sList;
	final String str = sList;
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
    @Override
    public Object clone() {
	ContentElement cloned = null;
	try {
	    cloned = (ContentElement) super.clone();
	    cloned.children = (ElementList) children.clone();
	} catch (final Exception ex) {
	    return null;
	}
	return cloned;
    }

}
