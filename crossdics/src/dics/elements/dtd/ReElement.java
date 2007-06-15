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
public class ReElement extends Element {

    /**
         * 
         */
    private String value;

    /**
         * 
         * @param value
         */
    public ReElement(final String value) {
	setTagName("re");
	this.value = value;
    }

    /**
         * 
         * @return
         */
    @Override
    public String getValue() {
	return value;
    }

    /**
         * 
         */
    @Override
    public final void printXML(final DataOutputStream dos) throws IOException {
	if (value == null) {
	    value = "";
	}
	dos.writeBytes(tab(3) + "<" + getTagName() + ">" + getValue() + "</"
		+ getTagName() + ">\n");
    }

}
