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
	private Action action;
	
	/**
	 * 
	 *
	 */
	public CrossModelFST(final CrossModel crossModel) {
		initialState = new State("");
		for (CrossAction crossAction : crossModel.getCrossActions()) {
			ElementList eList = crossAction.processVars();
			
			Action action = crossAction.getAction();
			add(eList, action);
		}
		
	}
	
	
	/**
	 * 
	 * @param eList
	 * @param action
	 */
	private final void add(final ElementList eList, final Action action) {
		initialState.add(eList, action, 0);
	}

	/**
	 * 
	 * @param entries
	 * @return
	 */
	public final Action getAction(CrossAction entries) {
		ElementList eList = entries.processEntries();
		ActionList actionList = new ActionList();
		initialState.getAction(eList, 0, actionList);
		
		if (actionList.size() > 0)  {
			//entries.print();			
			//System.out.print(actionList.size() + " candidate actions: ");
			//actionList.print();
			Action bestAction = actionList.getBestAction();
			setAction(bestAction);
			//System.out.println("Best action: " + bestAction.getName());
			//bestAction.print();
			return bestAction;
		} else {
			return null;
		}
		
	}

	/**
	 * 
	 * @return
	 */
	public final Action getAction() {
		return action;
	}

	/**
	 * 
	 * @param action
	 */
	public final void setAction(Action action) {
		this.action = action;
	}

}
