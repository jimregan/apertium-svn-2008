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

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Set;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class ActionSetList extends HashMap<String, ActionSet> {

    /**
         * 
         */
    private static final long serialVersionUID = 1L;

    /**
         * 
         * 
         */
    public final void print() {
	Set keySet = keySet();
	Iterator it = keySet.iterator();
	int max = size();
	int cont = 1;

	System.err.print("{ ");
	while (it.hasNext()) {
	    String key = (String) it.next();
	    ActionSet actionSet = get(key);

	    for (Action action : actionSet) {
		System.err.print(action.getName() + " ("
			+ action.getPatternLength() + "/"
			+ action.getNumberOfConstants() + ")");
		if (cont < max) {
		    System.err.print(", ");
		    cont++;
		}
	    }
	}
	System.err.println(" }");
    }

    /**
         * 
         * @return
         */
    public final ActionSet getBestActionSetOld() {
	ActionSet bestActionSet = null;
	ArrayList<ActionSet> actionSetList = new ArrayList<ActionSet>();
	int maxLength = 0;
	Set keySet = keySet();
	Iterator it = keySet.iterator();

	while (it.hasNext()) {
	    String key = (String) it.next();
	    ActionSet actionSet = get(key);
	    if (actionSet.getPatternLength() > maxLength) {
		actionSetList = new ArrayList<ActionSet>();
		actionSetList.add(actionSet);
		maxLength = actionSet.getPatternLength();
	    } else {
		if (actionSet.getPatternLength() == maxLength) {
		    actionSetList.add(actionSet);
		}
	    }
	}

	int maxNConstants = -1;
	for (ActionSet actionSet : actionSetList) {
	    if (actionSet.getNumberOfConstants() > maxNConstants) {
		bestActionSet = actionSet;
		maxNConstants = actionSet.getNumberOfConstants();
	    }
	}

	return bestActionSet;
    }

    /**
         * 
         * @return
         */
    public final ActionSet getBestActionSet() {
	ActionSet bestActionSet = null;
	ArrayList<ActionSet> actionSetList = new ArrayList<ActionSet>();

	Set keySet = keySet();
	Iterator it = keySet.iterator();
	int maxNConstants = -1;

	int i = 1;
	// System.err.println("Candidates:");
	while (it.hasNext()) {
	    String key = (String) it.next();
	    ActionSet actionSet = get(key);
	    // System.err.println("\tcandidate (" + i + "): " +
                // actionSet.getName());
	    // System.err.println("\t\tconstants: " +
                // actionSet.getNumberOfConstants());
	    // System.err.println("\t\tlength: " +
                // actionSet.getPatternLength());
	    if (actionSet.getNumberOfConstants() > maxNConstants) {
		actionSetList = new ArrayList<ActionSet>();
		actionSetList.add(actionSet);
		maxNConstants = actionSet.getNumberOfConstants();
	    } else {
		if (actionSet.getNumberOfConstants() == maxNConstants) {
		    actionSetList.add(actionSet);
		}
	    }
	    i++;
	}

	int maxLength = 0;

	ArrayList<ActionSet> actionSetList2 = new ArrayList<ActionSet>();
	for (ActionSet actionSet : actionSetList) {
	    if (actionSet.getPatternLength() > maxLength) {
		bestActionSet = actionSet;
		maxLength = actionSet.getPatternLength();
	    } else {
		if (actionSet.getPatternLength() == maxLength) {
		    actionSetList2.add(actionSet);
		}
	    }
	}

	if (actionSetList2.size() > 1) {
	    int maxR = 0;
	    for (ActionSet actionSet2 : actionSetList2) {
		if (actionSet2.getNumberOfRestrictions() > maxR) {
		    bestActionSet = actionSet2;
		    maxR = actionSet2.getNumberOfRestrictions();
		}
	    }
	}
	// System.err.println("Winner: " + bestActionSet.getName());
	return bestActionSet;
    }
}
