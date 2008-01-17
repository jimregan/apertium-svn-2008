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
public class PElement extends Element {

    /**
         * 
         */
    private LElement l;

    /**
         * 
         */
    private RElement r;

    /**
         * 
         * 
         */
    public PElement() {
	setTagName("p");
    }

    /**
         * 
         * @param pE
         */
    public PElement(final PElement pE) {
	setTagName("p");
	// l = new LElement(pE.getL());
	l = (LElement) pE.getL().clone();
	// r = new RElement(pE.getR());
	r = (RElement) pE.getR().clone();
    }

    /**
         * 
         * @param l
         */
    public final void setLElement(final LElement l) {
	this.l = l;
    }

    /**
         * 
         * @param r
         */
    public final void setRElement(final RElement r) {
	this.r = r;
    }

    /**
         * 
         * @return
         */
    public final LElement getL() {
	return l;
    }

    /**
         * 
         * @return
         */
    public final RElement getR() {
	return r;
    }

    /**
         * 
         * @param value
         * @param side
         */
    public final void setComments(final String value, final String side) {
	if (side.equals("L")) {
	    l.setComments(value);
	}
	if (side.equals("R")) {
	    r.setComments(value);
	}
    }

    /**
         * 
         */
    @Override
    public final void printXML(final DataOutputStream dos) throws IOException {
	dos.writeBytes(tab(3) + "<" + getTagName() + ">\n");
        if (l!= null)
            l.printXML(dos);
        if( r!= null)
            r.printXML(dos);
	dos.writeBytes(tab(3) + "</" + getTagName() + ">\n");
    }

    /**
         * 
         */
    public final void printXML1Line(final DataOutputStream dos)
	    throws IOException {
	dos.writeBytes("<" + getTagName() + ">");
	l.printXML1Line(dos);
	r.printXML1Line(dos);
	dos.writeBytes("</" + getTagName() + ">");
    }

    /**
         * 
         */
    public final String toString() {
	String str = "";

	str += getL().toString();
	str += getR().toString();

	return str;
    }

}
