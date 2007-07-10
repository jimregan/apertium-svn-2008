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
import java.util.Vector;

import dics.elements.dtd.Element;
import dics.elements.dtd.SElement;
import dics.elements.utils.ElementList;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class State {

    /**
         * 
         */
    private String value;

    /**
         * 
         */
    private String temp;

    /**
         * 
         */
    private TransitionSet transitions;

    /**
         * 
         */
    private ActionSet actionSet;

    /**
         * 
         */
    private static int nStates;

    /**
         * 
         */
    private Vector<String> lrrl;

    /**
         * 
         */
    private Vector<String> lr;

    /**
         * 
         */
    private Vector<String> rl;

    /**
         * 
         */
    private boolean restrictionMatched;

    /**
         * 
         */
    private ElementList entries;

    /**
         * 
         */
    private ActionSetList actionSetList;

    /**
         * 
         * @param value
         */
    public State(final String value) {
	this.value = value;
	transitions = new TransitionSet();
	incrementNStates();
    }

    /**
         * 
         * 
         */
    private final void incrementNStates() {
	State.nStates++;
    }

    /**
         * 
         * @param pattern
         * @param actionSet
         * @param i
         */
    public final void add(ElementList pattern, ActionSet actionSet, int i) {
	TransitionSet transitions = getTransitions();
	if (i < pattern.size()) {
	    final Element e = pattern.get(i);
	    State state;
	    if (!transitions.containsKey(e.getValue())) {
		state = new State(e.getValue());
		state.setTemp(((SElement) e).getTemp());
		transitions.put(state.getValue(), state);
	    } else {
		state = transitions.get(e.getValue());
	    }
	    if (state.getValue().equals("j")) {
		actionSet.setPatternLength(new Integer(i));
		actionSet.calculatePatternLength();
		state.setActionSet(actionSet);
	    } else {
		state.add(pattern, actionSet, i + 1);
	    }
	}
    }

    /**
         * 
         * @param entries
         * @param i
         * @param actionSetList
         */
    public final void getActionSet(final ElementList entries, int i,
	    ActionSetList actionSetList, HashMap<String, ElementList> tails) {
	if (i < entries.size()) {
	    lrrl = new Vector<String>();
	    lrrl.add("LR");
	    lrrl.add("RL");
	    lrrl.add("LR-RL");

	    lr = new Vector<String>();
	    lr.add("LR");
	    lr.add("LR-RL");

	    rl = new Vector<String>();
	    rl.add("RL");
	    rl.add("LR-RL");

	    setRestrictionMatched(false);

	    setEntries(entries);
	    setActionSetList(actionSetList);

	    final Element e = entries.get(i);
	    SElement sE = (SElement) e;
	    String v = sE.getValue();

	    processRestriction(v, "LR-RL", lrrl, i, tails);
	    processRestriction(v, "LR", lr, i, tails);
	    processRestriction(v, "RL", rl, i, tails);

	    if (!isRestrictionMatched()) {
		String real = sE.getTemp();
		// System.out.println(v + "/" + real);
		processItem(real, i, tails);
		processItem(v, i, tails);
	    }
	}
    }

    /**
         * This method will be removed
         * 
         * @param v
         * @param value
         * @param values
         * @param state
         * @param entries
         * @param i
         * @param actionList
         * @return
         */
    private final void processRestriction(final String v, final String value,
	    final Vector<String> values, int i,
	    HashMap<String, ElementList> tails) {
	if (!isRestrictionMatched()) {
	    if (v.equals(value)) {
		setRestrictionMatched(true);
		for (String val : values) {
		    if (getTransitions().containsKey(val)) {
			State state = getTransitions().get(val);
			state.getActionSet(getEntries(), i + 1,
				getActionSetList(), tails);
		    }
		}
	    }
	}
    }

    /**
         * 
         * @param value
         * @param i
         */
    private final void processItem(final String value, int i,
	    HashMap<String, ElementList> tails) {
	if (getTransitions().containsKey(value)) {
	    State state = getTransitions().get(value);

	    if (state.getValue().equals("j")) {
		ActionSet actionSet = state.getActionSet();
		actionSet.setTails(tails);

		setActionSet(state.getActionSet());
		getActionSetList().put(this.getActionSet().getName(),
			this.getActionSet());
	    } else {
		state.getActionSet(getEntries(), i + 1, getActionSetList(),
			tails);
	    }
	}

	if (getTransitions().containsKey("0")) {
	    if (tails == null) {
		tails = new HashMap<String, ElementList>();
	    }
	    // System.out.print("entries: ");
	    // this.getEntries().print();
	    ElementList tail = new ElementList();
	    ElementList entries = getEntries();

	    Element e = entries.get(i);
	    while (!e.getValue().equals("b") && !e.getValue().equals("j")) {
		tail.add(e);
		i++;
		e = entries.get(i);
	    }
	    // System.out.print("S = ");
	    // s.print();

	    // CrossModelFST.addTail(s);

	    State state = getTransitions().get("0");
	    if (tail.size() > 0) {
		if (!tails.containsKey(state.getTemp())) {
		    tails.put(state.getTemp(), tail);
		}
	    }

	    state.getActionSet(getEntries(), i, getActionSetList(), tails);

	    /*
                 * if (getTransitions().containsKey(e.getValue())) {
                 * System.out.println("Next: " + e.getValue()); State state =
                 * getTransitions().get(e.getValue());
                 * 
                 * if (state.getValue().equals("j")) {
                 * System.out.println("Accepted pattern with S");
                 * setActionSet(state.getActionSet());
                 * getActionSetList().put(this.getActionSet().getName(),
                 * this.getActionSet()); } else {
                 * state.getActionSet(getEntries(), i + 1, getActionSetList()); } }
                 */

	}

    }

    /**
         * @return the value
         */
    public final String getValue() {
	return value;
    }

    /**
         * @param value
         *                the value to set
         */
    public final void setValue(String value) {
	this.value = value;
    }

    /**
         * @return the nStates
         */
    public static final int getNStates() {
	return State.nStates;
    }

    /**
         * 
         * @return
         */
    private final TransitionSet getTransitions() {
	return transitions;
    }

    /**
         * 
         * @return
         */
    private final boolean isRestrictionMatched() {
	return restrictionMatched;
    }

    /**
         * 
         * @param restrictionMatched
         */
    private final void setRestrictionMatched(boolean restrictionMatched) {
	this.restrictionMatched = restrictionMatched;
    }

    /**
         * 
         * @return
         */
    private final ElementList getEntries() {
	return entries;
    }

    /**
         * 
         * @param entries
         */
    private final void setEntries(ElementList entries) {
	this.entries = entries;
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

    /**
         * 
         * @return
         */
    public final ActionSetList getActionSetList() {
	return actionSetList;
    }

    /**
         * 
         * @param actionSetList
         */
    public final void setActionSetList(ActionSetList actionSetList) {
	this.actionSetList = actionSetList;
    }

    /**
         * @return the temp
         */
    public final String getTemp() {
	return temp;
    }

    /**
         * @param temp
         *                the temp to set
         */
    public final void setTemp(String temp) {
	this.temp = temp;
    }
}
