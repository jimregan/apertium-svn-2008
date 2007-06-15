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
import java.util.ArrayList;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class ActionSet extends ArrayList<Action> {

    /**
         * 
         */
    private static final long serialVersionUID = 1L;

    /**
         * 
         */
    private String name;

    /**
         * 
         */
    private int numberOfConstants = 0;

    /**
         * 
         */
    private Integer patternLength;

    /**
         * 
         * @return
         */
    public final int getNumberOfConstants() {
	int n = 0;
	for (Action action : this) {
	    n += action.getNumberOfConstants();
	}
	return n;
    }

    /**
         * 
         * @return
         */
    public final String getName() {
	return name;
    }

    /**
         * 
         * @param name
         */
    public final void setName(String name) {
	this.name = name;
    }

    /**
         * 
         * @param numberOfConstants
         */
    public final void setNumberOfConstants(int numberOfConstants) {
	this.numberOfConstants = numberOfConstants;
    }

    /**
         * 
         * 
         */
    public final void incrementNumberOfConstants() {
	numberOfConstants++;
    }

    /**
         * 
         * 
         */
    public final void print() {
	for (Action action : this) {
	    action.print();
	}
    }

    /**
         * 
         * @param dos
         * @throws IOException
         */
    public final void printXML(DataOutputStream dos) throws IOException {
	dos.writeBytes("<action-set>\n");
	for (Action action : this) {
	    action.printXML(dos);
	}
	dos.writeBytes("</action-set>\n\n");
    }

    /**
         * 
         * @return
         */
    public final Integer getPatternLength() {
	return patternLength;
    }

    /**
         * 
         * @param patternLength
         */
    public final void setPatternLength(Integer patternLength) {
	this.patternLength = patternLength;
    }

}
