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
import java.util.HashMap;

import dics.elements.dtd.SElement;
import dics.elements.utils.ElementList;
import dics.elements.utils.SElementList;

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
         */
    private HashMap<String, ElementList> tails;

    /**
         * 
         */
    private CrossAction crossAction;

    /**
         * 
         * @return
         */
    public final int getNumberOfConstants() {
	/*
         * int n = 0; for (Action action : this) { n +=
         * action.getNumberOfConstants(); } return n;
         */
	return numberOfConstants;
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
    public final int getPatternLength() {
	return patternLength;
    }

    /**
         * 
         * @return
         */
    public final void calculatePatternLength() {
	// 3 because of 3 <b> tags
	// 2 because of 2 restrictions
	patternLength = patternLength - getNumberOfTails() - 2 - 3;
    }

    /**
         * 
         * @return
         */
    private final Integer getNumberOfTails() {
	int nTails = 0;
	CrossAction cA = getCrossAction();

	SElementList e1L = cA.getPattern().getAB().getSElements("L");
	SElementList e1R = cA.getPattern().getAB().getSElements("R");
	SElementList e2L = cA.getPattern().getBC().getSElements("L");
	SElementList e2R = cA.getPattern().getBC().getSElements("R");

	if (containsTail(e1L)) {
	    nTails++;
	}
	if (containsTail(e1R)) {
	    nTails++;
	}
	if (containsTail(e2L)) {
	    nTails++;
	}
	if (containsTail(e2R)) {
	    nTails++;
	}

	return new Integer(nTails);
    }

    /**
         * 
         * @param list
         * @return
         */
    private final boolean containsTail(final SElementList list) {
	for (SElement s : list) {
	    if (s.getValue().equals("0")) {
		return true;
	    }
	}
	return false;
    }

    /**
         * 
         * @return
         */
    public final int getNumberOfRestrictions() {
	CrossAction cA = getCrossAction();
	int nR = 0;
	if (cA.getPattern().getAB().hasRestriction()) {
	    nR++;
	}
	if (cA.getPattern().getBC().hasRestriction()) {
	    nR++;
	}
	return nR;
    }

    /**
         * 
         * @param patternLength
         */
    public final void setPatternLength(Integer patternLength) {
	this.patternLength = patternLength;
    }

    /**
         * @return the tails
         */
    public final HashMap<String, ElementList> getTails() {
	return tails;
    }

    /**
         * @param tails
         *                the tails to set
         */
    public final void setTails(HashMap<String, ElementList> tails) {
	this.tails = tails;
    }

    /**
         * @return the crossAction
         */
    public final CrossAction getCrossAction() {
	return crossAction;
    }

    /**
         * @param crossAction
         *                the crossAction to set
         */
    public final void setCrossAction(CrossAction crossAction) {
	this.crossAction = crossAction;
    }

}
