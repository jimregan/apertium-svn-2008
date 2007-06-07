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

import java.util.HashMap;

import dics.elements.dtd.Element;
import dics.elements.dtd.SElement;
import dics.elements.utils.ElementList;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class CrossModelFST {

    /**
         * 
         */
    private State initialState;

    /**
         * 
         */
    private ActionSet actionSet;

    /**
         * 
         * 
         */
    public CrossModelFST(final CrossModel crossModel) {
	initialState = new State("");
	String str;
	HashMap<String, CrossAction> patterns = new HashMap<String, CrossAction>();

	for (CrossAction crossAction : crossModel.getCrossActions()) {
	    ElementList eList = crossAction.processVars();
	    str = getElementListString(eList);
	    if (!patterns.containsKey(str)) {
		patterns.put(str, crossAction);
		// System.out.println(crossAction.getId() + ": " + str);
		ActionSet actionSet = crossAction.getActionSet();
		add(eList, actionSet);
	    } else {
		CrossAction cA = patterns.get(str);
		System.err.println("Duplicated pattern: '"
			+ crossAction.getId() + "' is the same as '"
			+ cA.getId() + "' are the same (will be ignored).");
	    }
	}
	// System.out.println("States: " + State.getNStates() );
    }

    /**
         * 
         * @param eList
         * @param action
         */
    private final void add(final ElementList eList, final ActionSet actionSet) {
	initialState.add(eList, actionSet, 0);
    }

    /**
         * 
         * @param entries
         * @return
         */
    public final ActionSet getActionSet(CrossAction entries) {
	ElementList eList = entries.processEntries();
	// String str = getElementListString(eList);
	// System.out.println(str);
	ActionSetList actionSetList = new ActionSetList();
	initialState.getActionSet(eList, 0, actionSetList);

	if (actionSetList.size() > 0) {
	    // System.out.println(str);
	    // entries.print();
	    ActionSet bestActionSet = actionSetList.getBestActionSet();
	    setActionSet(bestActionSet);
	    return bestActionSet;
	} else {
	    return null;
	}
    }

    /**
         * 
         * @param eList
         */
    private final String getElementListString(ElementList eList) {
	String str = "";
	for (Element e : eList) {
	    String real = ((SElement) e).getTemp();
	    if (real != null) {
		str += "<" + e.getValue() + "/" + real + ">";
	    } else {
		str += "<" + e.getValue() + ">";
	    }
	}
	return str;
    }

    /**
         * 
         * @return
         */
    public final ActionSet getActionSet() {
	return actionSet;
    }

    /**
         * 
         * @param actionSet
         */
    public final void setActionSet(ActionSet actionSet) {
	this.actionSet = actionSet;
    }

}
