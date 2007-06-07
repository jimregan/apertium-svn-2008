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

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class Element implements Cloneable {

    /**
         * 
         */
    private String value;

    /**
         * 
         */
    private String valueNoTags = "";

    /**
         * 
         */
    private String TAGNAME;

    /**
         * 
         */
    protected String comments;

    /**
         * 
         * @param dos
         * @throws IOException
         */
    protected void printXML(final DataOutputStream dos) throws IOException {
	dos.writeBytes("<" + getTagName() + "/>");
    }

    /**
         * 
         * @param nTabs
         * @return
         */
    protected String tab(final int nTabs) {
	String sTabs = "";
	for (int i = 0; i < nTabs; i++) {
	    sTabs += "  ";
	}
	return sTabs;
    }

    /**
         * 
         * @param value
         */
    protected void setTagName(final String value) {
	TAGNAME = value;
    }

    /**
         * 
         * @return
         */
    protected String getTagName() {
	return TAGNAME;
    }

    /**
         * 
         * @param value
         */
    public void addComments(final String value) {
	if (comments == null) {
	    comments = "";
	}
	comments += tab(3) + value + "\n";
    }

    /**
         * 
         * @param value
         * @param side
         */
    public void setComments(final String value) {
	comments = value;
    }

    /**
         * 
         * @return
         */
    public String getComments() {
	return comments;
    }

    /**
         * 
         */

    @Override
    public Object clone() {
	try {
	    final Element cloned = (Element) super.clone();
	    return cloned;
	} catch (final CloneNotSupportedException ex) {
	    return null;
	}

    }

    /**
         * 
         * @return
         */
    public String getValue() {
	return value;
    }

    /**
         * 
         * @param value
         */
    protected void setValue(String value) {
	this.value = value;
    }

    public String getValueNoTags() {
	return valueNoTags;
    }

    public void setValueNoTags(String valueNoTags) {
	this.valueNoTags = valueNoTags;
    }

}
