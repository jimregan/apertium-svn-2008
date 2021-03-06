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

package dictools.crossmodel;

import java.io.DataOutputStream;
import java.io.IOException;

import dics.elements.dtd.EElement;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class Pattern {

    /**
         * 
         */
    private EElement e1;

    /**
         * 
         */
    private EElement e2;

    /**
         * 
         * 
         */
    public Pattern() {

    }

    /**
         * 
         * @param ab
         * @param bc
         */
    public Pattern(final EElement ab, final EElement bc) {
	e1 = ab;
	e2 = bc;
    }

    /**
         * 
         * @return
         */
    public EElement getAB() {
	return e1;
    }

    /**
         * 
         * @return
         */
    public EElement getBC() {
	return e2;
    }

    /**
         * 
         * @param ab
         */
    public void setAB(final EElement ab) {
	e1 = ab;
    }

    /**
         * 
         * @param bc
         */
    public void setBC(final EElement bc) {
	e2 = bc;
    }

    /**
         * 
         * 
         */
    public final void print() {
	System.out.println("Pattern:");
	getAB().print("L");
	getAB().print("R");
	getBC().print("L");
	getBC().print("R");
    }

    /**
         * 
         */
    @Override
    public final String toString() {
	String e1 = getAB().toString2();
	String e2 = getBC().toString2();
	String str = e1 + "/" + e2;
	return str;
    }

    /**
         * 
         * @param dos
         */
    protected final void printXML(DataOutputStream dos) throws IOException {
	dos.writeBytes("\t<pattern>\n");
	e1.printXML(dos);
	e2.printXML(dos);
	dos.writeBytes("\t</pattern>\n");
    }

}
