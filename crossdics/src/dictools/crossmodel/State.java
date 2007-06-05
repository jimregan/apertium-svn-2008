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
	private TransitionSet transitions;
	
	/**
	 * 
	 */
	private Action action;
	
	/**
	 * 
	 * @param value
	 */
	public State(final String value) {
		this.value = value;
		transitions = new TransitionSet();
	}
	
	/**
	 * 
	 * @param pattern
	 * @param i
	 */
	public final void add(ElementList pattern, Action action, int i) {
		if (i < pattern.size()) {
			final Element e = pattern.get(i);
			State state;
			if (!transitions.containsKey(e.getValue())) {
				state = new State(e.getValue());
				transitions.put(state.getValue(), state);
			} else {
				state = transitions.get(e.getValue());
			}
			if (state.getValue().equals("j")) {
				action.setPatternLength(new Integer(i));
				state.setAction(action);
			} else {
				state.add(pattern, action, i+1);
			}
		}
	}

	/**
	 * 
	 * @param entries
	 * @param i
	 */
	public final void getAction(final ElementList entries, int i, ActionList actionList) {
		
		Action action = null;
		if (i < entries.size()) {
			final Element e = entries.get(i);
			SElement sE = (SElement)e;
			String real = sE.getTemp();
			
			State state = null;
			// si existe uno concreto (1/adj)
			if (transitions.containsKey(real)) {
				state = transitions.get(real);
				if (state != null) {
					if (state.getValue().equals("j")) {
						action = state.getAction();
						actionList.put(action.getName(), action);
					} else {
						state.getAction(entries, i+1, actionList);
					}
				} 
			}
			// uno general (1)
			String var = sE.getValue();
			if (transitions.containsKey(var)) {
				state = transitions.get(var);
				if (state != null) {
					if (state.getValue().equals("j")) {
						action = state.getAction();
						actionList.put(action.getName(), action);
					} else {
						state.getAction(entries, i+1, actionList);
					}
				} 
			}
		}
	}

	/**
	 * @return the value
	 */
	public final String getValue() {
		return value;
	}

	/**
	 * @param value the value to set
	 */
	public final void setValue(String value) {
		this.value = value;
	}

	/**
	 * @return the action
	 */
	public final Action getAction() {
		return action;
	}

	/**
	 * @param action the action to set
	 */
	public final void setAction(Action action) {
		this.action = action;
	}
}
