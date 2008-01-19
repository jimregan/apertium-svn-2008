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
     */
    private ActionSet bestActionSet;

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

                System.err.print(actionSet.getName() + " (" + actionSet.getPatternLength() + "/" + actionSet.getNumberOfConstants() + ")");
                if (cont < max) {
                    System.err.print(", ");
                    cont++;
                }
        }
        System.err.println(" }");
    }


    /**
     * 
     * @return
     */
    public final ActionSet getBestActionSet() {
        ActionSet defaultActionSet = null;
        ArrayList<ActionSet> actionSetList = new ArrayList<ActionSet>();
        Set keySet = keySet();
        Iterator it = keySet.iterator();
        int maxNConstants = -1;
        boolean defaultCanBeApplied = false;

        while (it.hasNext()) {
            String key = (String) it.next();
            ActionSet actionSet = get(key);
            if (!actionSet.getName().equals("default")) {
                if (actionSet.getNumberOfConstants() > maxNConstants) {
                    actionSetList = new ArrayList<ActionSet>();
                    actionSetList.add(actionSet);
                    maxNConstants = actionSet.getNumberOfConstants();
                } else {
                    if (actionSet.getNumberOfConstants() == maxNConstants) {
                        actionSetList.add(actionSet);
                    }
                }
            } else {
                defaultCanBeApplied = true;
                defaultActionSet = actionSet;
            }
        }

        ArrayList<ActionSet> longestPatterns = this.getLongestPatterns(actionSetList);
        this.getMostConcretePatterns(longestPatterns);
        
        if (!isDefinedBestAction() && defaultCanBeApplied) {
            setBestActionSet(defaultActionSet);
        }
        return bestActionSet;
    }
    
    /**
     * 
     * @param actionSetList
     * @return
     */
    private final ArrayList<ActionSet> getLongestPatterns(ArrayList<ActionSet> actionSetList) {
        int maxLength = 0;
        ArrayList<ActionSet> longestPatterns = new ArrayList<ActionSet>();
        for (ActionSet actionSet : actionSetList) {
            int patternLength = actionSet.getNumberOfConstants();
            if (patternLength > maxLength) {
                this.setBestActionSet(actionSet);
                maxLength = patternLength;
            } else {
                if (patternLength == maxLength) {
                    longestPatterns.add(actionSet);
                }
            }
        }
        return longestPatterns;
    }
    
    /**
     * 
     * @param actionSetList
     */
    private final void getMostConcretePatterns(final ArrayList<ActionSet> actionSetList) {
        if (actionSetList.size() > 1) {
            int maxR = 0;
            for (ActionSet actionSet2 : actionSetList) {
                if (actionSet2.getNumberOfRestrictions() > maxR) {
                    this.setBestActionSet(actionSet2);
                    maxR = actionSet2.getNumberOfRestrictions();
                }
            }
        }
    }
    
    /**
     * 
     * @return
     */
    private final boolean isDefinedBestAction() {
        return this.bestActionSet != null;
    }
    
    /**
     * 
     * @param action
     */
    private final void setBestActionSet(ActionSet action) {
        this.bestActionSet = action;
        
    }
    
    
}
