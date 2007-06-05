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
public class ActionList extends HashMap<String,Action> {

	/**
	 * 
	 *
	 */
	public final void print() {
		Set keySet = this.keySet();
		Iterator it = keySet.iterator();
		int max = this.size();
		int cont = 1;
		
		System.out.print("{ ");
		while (it.hasNext()) {
			String key = (String)it.next();
			Action action = (Action)this.get(key);
			System.out.print(action.getName() + " (" + action.getPatternLength() + "/" + action.getNumberOfConstants() + ")");
			if (cont < max ) {
				System.out.print(", ");
				cont++;
			}
		}
		System.out.println(" }");
	}
	
	/**
	 * 
	 * @return
	 */
	public final Action getBestAction() {
		Action bestAction = null;
		ArrayList<Action> actionList = new ArrayList<Action>();
		int maxLength = 0;
		Set keySet = this.keySet();
		Iterator it = keySet.iterator();
		
		while (it.hasNext()) {
			String key = (String)it.next();
			Action action = (Action)this.get(key);
			if (action.getPatternLength() > maxLength) {
				actionList = new ArrayList<Action>();
				actionList.add(action);
				maxLength = action.getPatternLength();
			} else {
				if (action.getPatternLength() == maxLength) {
					actionList.add(action);	
				}
			}
		}
		
		int maxNConstants = -1;
		for (Action action : actionList) {
			if (action.getNumberOfConstants() > maxNConstants) {
				bestAction = action;
				maxNConstants = action.getNumberOfConstants();
			}
		}
		
		return bestAction;
	}
}
