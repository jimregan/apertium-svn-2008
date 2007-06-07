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
import java.util.HashMap;

import dics.elements.dtd.ContentElement;
import dics.elements.dtd.EElement;
import dics.elements.dtd.Element;
import dics.elements.dtd.SElement;
import dics.elements.utils.ElementList;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class CrossAction {

    /**
         * 
         */
    private String id;

    /**
         * 
         */
    private Pattern pattern;

    /**
         * 
         */
    private ConstantMap constants;

    /**
         * 
         */
    private ActionSet actionSet;

    /**
         * 
         * 
         */
    public CrossAction() {
	constants = new ConstantMap();
	pattern = new Pattern();
	actionSet = new ActionSet();
    }

    /**
         * 
         * @param p
         * @param cm
         * @param a
         */
    /*
         * public CrossAction(final Pattern p, final ConstantMap cm, final
         * Action a) { constants = new ConstantMap(); pattern = new Pattern();
         * actionSet = new ActionSet(); pattern = p; constants = cm; action = a; }
         */

    /**
         * 
         * @param p
         * @param cm
         * @param aSet
         */
    public CrossAction(final Pattern p, final ConstantMap cm,
	    final ActionSet aSet) {
	constants = new ConstantMap();
	pattern = new Pattern();
	actionSet = new ActionSet();
	pattern = p;
	constants = cm;
	actionSet = aSet;
    }

    /**
         * 
         * @param p
         */
    public void setPattern(final Pattern p) {
	pattern = p;
    }

    /**
         * 
         * @param cm
         */
    public void setConstantMap(final ConstantMap cm) {
	constants = cm;
    }

    /**
         * 
         * @param a
         */
    /*
         * public void setAction(final Action a) { action = a;
         * action.setName(getId()); }
         */

    /**
         * 
         * @return
         */
    public final ConstantMap getConstants() {
	return constants;
    }

    /**
         * 
         * @param constants
         */
    public final void setConstants(final ConstantMap constants) {
	this.constants = constants;
    }

    /**
         * 
         * @return
         */
    /*
         * public final Action getAction() { return action; }
         */

    /**
         * 
         * @return
         */
    public final Pattern getPattern() {
	return pattern;
    }

    /**
         * 
         * @return
         */
    public final String getId() {
	return id;
    }

    /**
         * 
         * @param id
         */
    public final void setId(final String id) {
	this.id = id;
    }

    /**
         * 
         * 
         */
    public final void print() {
	System.out.println("CROSS-ACTION:");
	if (pattern != null) {
	    getPattern().print();
	}
	if (constants != null) {
	    getConstants().print();
	}
	if (actionSet != null) {
	    getActionSet().print();
	}
    }

    /**
         * 
         * @param dos
         */
    public final void printXML(DataOutputStream dos, int id) throws IOException {
	dos.writeBytes("<cross-action id=\"ND-" + id + "\">\n");
	getPattern().printXML(dos);
	if (this.actionSet != null) {
	    getActionSet().printXML(dos);
	}
	dos.writeBytes("</cross-action>\n\n");
    }

    /**
         * 
         * @return
         */
    public final ElementList processEntries() {
	ElementList eList = new ElementList();
	try {
	    HashMap<String, Integer> hm = new HashMap<String, Integer>();
	    Pattern pattern = getPattern();

	    Integer j = new Integer(0);

	    EElement e1 = pattern.getAB();
	    ContentElement e1L = (ContentElement) e1.getSide("L").clone();
	    ContentElement e1R = (ContentElement) e1.getSide("R").clone();
	    EElement e2 = pattern.getBC();
	    ContentElement e2L = (ContentElement) e2.getSide("L").clone();
	    ContentElement e2R = (ContentElement) e2.getSide("R").clone();

	    // e1
	    addRestrictionCode(eList, e1);
	    j = tagElements(e1L, eList, j, hm);
	    eList.add(new SElement("b"));
	    j = tagElements(e1R, eList, j, hm);
	    eList.add(new SElement("b"));

	    // e2
	    addRestrictionCode(eList, e2);
	    j = tagElements(e2L, eList, j, hm);
	    eList.add(new SElement("b"));
	    j = tagElements(e2R, eList, j, hm);
	    eList.add(new SElement("j"));

	    /*
                 * ActionSet aSet = getActionSet(); if (aSet != null) { EElement
                 * ea = a.getE(); j = tagAction(ea.getSide("L"), j, hm); j =
                 * tagAction(ea.getSide("R"), j, hm); }
                 */
	} catch (Exception e) {
	    e.printStackTrace();
	}

	return eList;
    }

    /**
         * 
         * @param eList
         * @param e
         */
    private final void addRestrictionCode(ElementList eList, final EElement e) {
	if (e.hasRestriction()) {
	    eList.add(new SElement(e.getRestriction()));
	} else {
	    eList.add(new SElement("LR-RL"));
	}
    }

    /**
         * 
         * @return
         */
    public final ElementList processVars() {
	this.getActionSet().setNumberOfConstants(0);

	HashMap<String, Integer> hm = new HashMap<String, Integer>();
	ElementList eList = new ElementList();

	Pattern pattern = getPattern();

	Integer j = new Integer(0);

	EElement e1 = pattern.getAB();
	ContentElement e1L = (ContentElement) e1.getSide("L");
	ContentElement e1R = (ContentElement) e1.getSide("R");
	EElement e2 = pattern.getBC();
	ContentElement e2L = (ContentElement) e2.getSide("L");
	ContentElement e2R = (ContentElement) e2.getSide("R");

	addRestrictionCode(eList, e1);
	j = tagPattern(e1L, eList, j, hm);
	eList.add(new SElement("b"));
	j = tagPattern(e1R, eList, j, hm);
	eList.add(new SElement("b"));

	addRestrictionCode(eList, e2);
	j = tagPattern(e2L, eList, j, hm);
	eList.add(new SElement("b"));
	j = tagPattern(e2R, eList, j, hm);
	eList.add(new SElement("j"));

	ActionSet aSet = this.getActionSet();

	if (aSet != null) {
	    for (Action a : aSet) {
		EElement ea = a.getE();
		j = tagAction(ea.getSide("L"), j, hm);
		j = tagAction(ea.getSide("R"), j, hm);
	    }
	}

	return eList;
    }

    /**
         * 
         * @param ce
         * @param eList
         * @param j
         * @param hm
         */
    private final Integer tagPattern(ContentElement ce, ElementList eList,
	    Integer j, HashMap<String, Integer> hm) {
	for (int k = 0; k < ce.getChildren().size(); k++) {
	    Element e = ce.getChildren().get(k);

	    if (e instanceof SElement) {
		Integer pos;
		SElement sElement = (SElement) e.clone();
		ce.getChildren().set(k, sElement);
		String value = sElement.getValue();
		if (hm.containsKey(value)) {
		    pos = hm.get(value);
		} else {
		    j++;
		    pos = new Integer(j);
		    hm.put(value, pos);
		}
		if (Character.isUpperCase(value.charAt(0))
			&& Character.isDigit(value.charAt(value.length() - 1))) {

		    String n = pos.toString();
		    sElement.setValue(n);
		    sElement.setTemp(value);
		} else {
		    if (!Character.isDigit(value.charAt(0))) {
			sElement.setValue(value);
			this.getActionSet().incrementNumberOfConstants();
		    }
		}
		eList.add(sElement);
	    }
	}
	return j;
    }

    /**
         * 
         * @param ce
         * @param eList
         * @param j
         * @param hm
         * @return
         */
    private final Integer tagAction(ContentElement ce, Integer j,
	    HashMap<String, Integer> hm) {
	for (int k = 0; k < ce.getChildren().size(); k++) {
	    Element e = ce.getChildren().get(k);
	    if (e instanceof SElement) {
		SElement sElement = (SElement) e.clone();
		ce.getChildren().set(k, sElement);
		String value = sElement.getValue();
		if (Character.isUpperCase(value.charAt(0))
			&& Character.isDigit(value.charAt(value.length() - 1))) {
		    Integer pos;
		    if (hm.containsKey(value)) {
			pos = hm.get(value);
		    } else {
			j++;
			pos = new Integer(j);
			hm.put(value, pos);
		    }
		    String n = pos.toString();
		    sElement.setValue(n);
		    sElement.setTemp(value);
		} else {
		    if (!Character.isDigit(value.charAt(0))) {
			sElement.setValue(value);
		    }
		}
	    }
	}
	return j;
    }

    /**
         * 
         * @param ce
         * @param eList
         * @param j
         * @param hm
         * @return
         */
    private final Integer tagElements(ContentElement ce, ElementList eList,
	    Integer j, HashMap<String, Integer> hm) {
	for (int k = 0; k < ce.getChildren().size(); k++) {
	    Element e = ce.getChildren().get(k);
	    if (e instanceof SElement) {
		String value = e.getValue();
		Integer pos;
		if (hm.containsKey(value)) {
		    pos = hm.get(value);
		} else {
		    j++;
		    pos = new Integer(j);
		    hm.put(value, pos);
		    getConstants().put(pos.toString(), value);
		}
		SElement sElement = (SElement) e;
		sElement.setValue(pos.toString());
		sElement.setTemp(value);
		eList.add(sElement);
	    }
	}
	return j;
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
	actionSet.setName(getId());
    }
}
