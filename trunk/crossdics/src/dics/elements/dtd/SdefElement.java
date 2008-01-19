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
public class SdefElement extends Element {

    /**
         * 
         */
    private String n;

    /**
         * 
         */
    private String comment;

    /**
         * 
         * @param value
         */
    public SdefElement(final String value) {
	setTagName("sdef");
	n = value;
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
         * @param dos
         * @throws IOException
         */
    @Override
    public final void printXML(final DataOutputStream dos) throws IOException {
	if (comments == null) {
	    setComments("");
	}
	String comment = "";
	if (this.comment != null) {
	    comment = "\tc=\"" + getComment() + "\"";

	}
	dos.writeBytes(tab(2) + "<" + getTagName() + " n=\"" + getValue()
		+ "\" " + comment + "/> " + getComments() + "\n");
    }

    /**
         * 
         */
    @Override
    public final String toString() {
	final String str = "<" + getValue() + ">";
	return str;
    }

    /**
         * @return the comment
         */
    public final String getComment() {
	return comment;
    }

    /**
         * @param comment
         *                the comment to set
         */
    public final void setComment(String comment) {
	this.comment = comment;
    }

}
