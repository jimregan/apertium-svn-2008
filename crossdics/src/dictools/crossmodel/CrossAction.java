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
import dics.elements.dtd.TextElement;
import dics.elements.utils.ElementList;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class CrossAction implements Comparable<CrossAction> {

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
     */
    private int occurrences;

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
        // System.err.println("CROSS-ACTION:");
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
        if (actionSet != null) {
            getActionSet().printXML(dos);
        }
        dos.writeBytes("</cross-action>\n");
        dos.writeBytes("<!-- " + getOccurrences() + " entries like this -->\n\n");
    }

    /**
     * 
     * @return
     */
    public final ElementList processEntries() {
        ElementList eList = new ElementList();
        try {
            HashMap<String, String> hm = new HashMap<String, String>();
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
            j = tagElements(e1L, eList, j, hm, "1");
            eList.add(new SElement("b"));

            j = new Integer(0); // añadido
            j = tagElements(e1R, eList, j, hm, "2");
            eList.add(new SElement("b"));

            // e2
            addRestrictionCode(eList, e2);

            j = new Integer(0); // añadido
            j = tagElements(e2L, eList, j, hm, "3");
            eList.add(new SElement("b"));
            j = new Integer(0); // añadido
            j = tagElements(e2R, eList, j, hm, "4");
            eList.add(new SElement("j"));

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
        getActionSet().setNumberOfConstants(0);

        HashMap<String, String> hm = new HashMap<String, String>();
        ElementList eList = new ElementList();

        Pattern pattern = getPattern();

        Integer j = new Integer(0);

        EElement e1 = pattern.getAB();
        ContentElement e1L = e1.getSide("L");
        ContentElement e1R = e1.getSide("R");
        EElement e2 = pattern.getBC();
        ContentElement e2L = e2.getSide("L");
        ContentElement e2R = e2.getSide("R");

        addRestrictionCode(eList, e1);
        j = tagPattern(e1L, eList, j, hm, "1");
        eList.add(new SElement("b"));
        j = new Integer(0); // añadido

        j = tagPattern(e1R, eList, j, hm, "2");
        eList.add(new SElement("b"));

        addRestrictionCode(eList, e2);

        j = new Integer(0); // añadido
        j = tagPattern(e2L, eList, j, hm, "3");
        eList.add(new SElement("b"));

        j = new Integer(0); // añadido
        j = tagPattern(e2R, eList, j, hm, "4");
        eList.add(new SElement("j"));

        ActionSet aSet = getActionSet();

        if (aSet != null) {
            for (Action a : aSet) {
                EElement ea = a.getE();
                j = new Integer(0); // añadido
                j = tagAction(ea.getSide("L"), j, hm, "5");
                j = new Integer(0); // añadido
                j = tagAction(ea.getSide("R"), j, hm, "6");
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
            Integer j, HashMap<String, String> hm, final String pref) {
        int p = 1;
        for (int k = 0; k < ce.getChildren().size(); k++) {
            Element e = ce.getChildren().get(k);

            if (e instanceof TextElement) {
                // adds lemma to list of elements
                TextElement tE = (TextElement) e;
                if (!tE.getValue().equals("l1") && !tE.getValue().equals("l2") && !tE.getValue().equals("l3") && !tE.getValue().equals("l4")) {
                    eList.add(tE);
                }
            }

            if (e instanceof SElement) {
                String pos;
                SElement sElement = (SElement) e.clone();
                ce.getChildren().set(k, sElement);
                String value = sElement.getValue();
                if (hm.containsKey(value)) {
                    pos = hm.get(value);
                } else {
                    pos = pref + "-" + p;
                    p++;
                    hm.put(value, pos);
                }
                if (Character.isUpperCase(value.charAt(0)) && Character.isDigit(value.charAt(value.length() - 1))) {
                    if (value.charAt(0) == 'S') {
                        String n = new String("0");
                        sElement.setValue(n);
                        sElement.setTemp(value);
                    } else {
                        String n = pos.toString();
                        sElement.setValue(n);
                        sElement.setTemp(value);
                    }
                } else {
                    if (!Character.isDigit(value.charAt(0))) {
                        sElement.setValue(value);
                        getActionSet().incrementNumberOfConstants();
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
            HashMap<String, String> hm, final String pref) {
        int p = 1;
        for (int k = 0; k < ce.getChildren().size(); k++) {
            Element e = ce.getChildren().get(k);

            if (e instanceof TextElement) {
                TextElement tE = (TextElement) e;
                String text = tE.getValue();
                if (!text.equals("l1") && !text.equals("l4")) {
                } else {
                    
                }
            }


            if (e instanceof SElement) {
                String pos;
                SElement sElement = (SElement) e.clone();
                ce.getChildren().set(k, sElement);
                String value = sElement.getValue();
                if (hm.containsKey(value)) {
                    pos = hm.get(value);
                } else {
                    pos = pref + "-" + p;
                    p++;
                    hm.put(value, pos);
                }

                /*
                 * SElement sElement = (SElement) e.clone();
                 * ce.getChildren().set(k, sElement); String value =
                 * sElement.getValue();
                 */
                if (Character.isUpperCase(value.charAt(0)) && Character.isDigit(value.charAt(value.length() - 1))) {
                    if (value.charAt(0) == 'S') {
                        String n = new String("0");
                        sElement.setValue(n);
                        sElement.setTemp(value);
                    } else {
                        String n = pos.toString();
                        sElement.setValue(n);
                        sElement.setTemp(value);
                    }
                /*
                 * String pos; if (hm.containsKey(value)) { pos =
                 * hm.get(value); } else { pos = pref + "-" + p;
                 * hm.put(value, pos); p++; } String n = pos.toString();
                 * sElement.setValue(n); sElement.setTemp(value);
                 */
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
            Integer j, HashMap<String, String> hm, final String pref) {
        int p = 1;
        for (int k = 0; k < ce.getChildren().size(); k++) {
            Element e = ce.getChildren().get(k);

            if (e instanceof TextElement) {
                // adds lemma to list of elements
                TextElement tE = (TextElement) e;
                tE.setTemp(tE.getValue());
                // System.out.println("..tagging '" + tE.getValue() + "'");
                eList.add(tE);
            }

            if (e instanceof SElement) {
                String value = e.getValue();
                String pos;
                if (hm.containsKey(value)) {
                    pos = hm.get(value);
                } else {
                    pos = pref + "-" + p;
                    hm.put(value, pos);
                    getConstants().put(pos.toString(), value);
                    p++;
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

    /**
     * @return the occurrences
     */
    public final int getOccurrences() {
        return occurrences;
    }

    /**
     * @param occurrences
     *                the occurrences to set
     */
    public final void setOccurrences(int occurrences) {
        this.occurrences = occurrences;
    }

    /**
     * 
     * 
     */
    public final void incrementOccurrences() {
        occurrences++;
    }

    /**
     * 
     */
    public int compareTo(final CrossAction anotherEElement)
            throws ClassCastException {

        if (anotherEElement == null) {
            return -1;
        }

        if (!(anotherEElement instanceof CrossAction)) {
            throw new ClassCastException("A CrossAction object expected.");
        }

        final int occ1 = getOccurrences();

        final int occ2 = (anotherEElement).getOccurrences();

        if (occ1 == occ2) {
            return 0;
        }
        if (occ1 > occ2) {
            return -1;
        }
        return 1;
    }
}
