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

package dictools;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Set;

import dics.elements.dtd.AlphabetElement;
import dics.elements.dtd.DictionaryElement;
import dics.elements.dtd.EElement;
import dics.elements.dtd.LElement;
import dics.elements.dtd.PElement;
import dics.elements.dtd.RElement;
import dics.elements.dtd.SElement;
import dics.elements.dtd.SdefElement;
import dics.elements.dtd.SdefsElement;
import dics.elements.dtd.SectionElement;
import dics.elements.utils.DicSet;
import dics.elements.utils.EElementList;
import dics.elements.utils.SElementList;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class DicCross {

    /**
     * Restrictions for crossing
     */
    private int[][] rMatrix;

    /**
     * Left-Right restriction
     */
    private final int LR = 0;

    /**
     * Right-Left restriction
     */
    private final int RL = 1;

    /**
     * Left-Right and Right-Left restrictions.
     */
    private final int BOTH = 2;

    /**
     * 
     */
    private final int NONE = 3;

    /**
     * Bilingual dictionary A-B
     */
    private DictionaryElement bilAB;

    /**
     * Bilingual dictionary B-C
     */
    private DictionaryElement bilBC;

    /**
     * 
     */
    private final boolean MATCH_CATEGORY = true;

    /**
     * 
     */
    private final boolean DO_NOT_MATCH_CATEGORY = false;

    /**
     * 
     */
    //private CrossModel crossModel;

    /**
     * 
     * 
     */
    public DicCross() {
	rMatrix = new int[3][3];
	fillOutRestrictionMatrix();
	/*
	try {
	    System.err.println("Reading cross-model.xml ...");
	CrossModelReader cmr = new CrossModelReader("cross-model.xml");
	crossModel = cmr.readCrossModel();
	System.err.println("OK! ...");
	int nCrossActions = crossModel.getCrossActions().size();
	System.err.println("Cross actions: " + nCrossActions);

	} catch (Exception e) {
	    e.printStackTrace();
	}
	 */

    }

    /**
     * 
     * 
     */
    private final void fillOutRestrictionMatrix() {
	// Note: B-A ^ B-C = A-C
	rMatrix[LR][LR] = NONE;
	rMatrix[LR][RL] = RL;
	rMatrix[LR][BOTH] = RL;

	rMatrix[RL][LR] = LR;
	rMatrix[RL][RL] = NONE;
	rMatrix[RL][BOTH] = LR;

	rMatrix[BOTH][LR] = LR;
	rMatrix[BOTH][RL] = RL;
	rMatrix[BOTH][BOTH] = BOTH;
    }

    /**
     * 
     * @param dic1
     * @param dic2
     * @return
     */
    public DictionaryElement[] crossDictionaries(final DicSet dicSet) {
	final DictionaryElement[] dics = new DictionaryElement[2];

	final DictionaryElement dic1 = dicSet.getBil1();
	final DictionaryElement dic2 = dicSet.getBil2();

	bilAB = dic1;
	bilBC = dic2;

	final DictionaryElement dic = new DictionaryElement();
	final DictionaryElement specul = new DictionaryElement();

	// alphabet
	final AlphabetElement alphabet = crossAlphabets(dic1.getAlphabet(),
		dic2.getAlphabet());
	dic.setAlphabet(alphabet);
	specul.setAlphabet(alphabet);

	// sdefs
	final SdefsElement sdefs = crossSdefs(dic1.getSdefs(), dic2.getSdefs());
	dic.setSdefs(sdefs);
	specul.setSdefs(sdefs);

	// sections
	for (final SectionElement section1 : dic1.getSections()) {
	    final SectionElement section2 = dic2.getSection(section1.getID());
	    final SectionElement[] sections = crossSections(section1, section2,
		    sdefs);

	    final SectionElement section = sections[0];
	    dic.addSection(section);

	    final SectionElement speculSection = sections[1];
	    specul.addSection(speculSection);
	}

	Collections.sort(dic.getEntries());

	dics[0] = dic;
	dics[1] = specul;

	return dics;
    }

    /**
     * 
     * @param alphabet1
     * @param alphabet2
     * @return
     */
    public final AlphabetElement crossAlphabets(
	    final AlphabetElement alphabet1, final AlphabetElement alphabet2) {
	final AlphabetElement alphabet = new AlphabetElement();

	final String a1 = alphabet1.getAlphabet();
	final String a2 = alphabet2.getAlphabet();

	final HashMap<Object, Object> a3 = new HashMap<Object, Object>();

	int i = 0;
	for (i = 0; i < a1.length(); i++) {
	    final char c = a1.charAt(i);
	    if (!a3.containsKey(c)) {
		a3.put(c, null);
	    }
	}

	int j = 0;
	for (j = 0; j < a1.length(); j++) {
	    final char c = a2.charAt(j);
	    if (!a3.containsKey(c)) {
		a3.put(c, null);
	    }
	}

	return alphabet;
    }

    /**
     * Crossed two <code>&lt;sdefs&gt;</code> elements.
     * 
     * @param sdefs1
     * @param sdefs2
     * @return
     */
    public final SdefsElement crossSdefs(final SdefsElement sdefs1,
	    final SdefsElement sdefs2) {
	final SdefsElement sdefs = new SdefsElement();

	HashMap<String, SdefElement> sdefList = new HashMap<String, SdefElement>();

	for (final SdefElement sdef1 : sdefs1.getSdefsElements()) {
	    if (!sdefList.containsKey(sdef1.getValue())) {
		sdefList.put(sdef1.getValue(), sdef1);
	    }
	}
	for (final SdefElement sdef2 : sdefs2.getSdefsElements()) {
	    if (!sdefList.containsKey(sdef2.getValue())) {
		sdefList.put(sdef2.getValue(), sdef2);
	    } else {
		sdefList.put(sdef2.getValue(), sdef2);
	    }
	}

	Set<String> keys = sdefList.keySet();
	Iterator<String> it = keys.iterator();

	while (it.hasNext()) {
	    final String str = it.next();
	    final SdefElement sdef = sdefList.get(str);
	    sdefs.addSdefElement(sdef);
	}
	sdefList = null;
	it = null;
	keys = null;

	return sdefs;
    }

    /**
     * Crossed two <code>&lt;section&gt;</code> elements.
     * 
     * @param section1
     * @param section2
     * @return
     */
    public SectionElement[] crossSections(final SectionElement section1,
	    final SectionElement section2, final SdefsElement sdefs) {

	final SectionElement[] sections = new SectionElement[2];

	final SectionElement section = new SectionElement();
	final SectionElement speculSection = new SectionElement("main",
	"standard");

	section.setID(section1.getID());
	section.setType(section1.getType());

	EElementList elements1 = section1.getEElements();
	EElementList elements2 = section2.getEElements();

	HashMap<String, EElementList> section1Map = DicTools
	.buildHash(elements1);
	HashMap<String, EElementList> section2Map = DicTools
	.buildHash(elements2);

	HashMap<String, EElement> processed = new HashMap<String, EElement>();
	HashMap<String, EElement> speculProcessed = new HashMap<String, EElement>();
	HashMap<String, EElement> regExProcessed = new HashMap<String, EElement>();

	crossSectionsAB(elements1, regExProcessed, section2Map, section,
		speculSection, processed, speculProcessed, bilBC, 0);
	section2Map = null;

	crossSectionsAB(elements2, regExProcessed, section1Map, section,
		speculSection, processed, speculProcessed, bilAB, 1);
	section1Map = null;

	processed = null;
	speculProcessed = null;
	regExProcessed = null;

	sections[0] = section;
	sections[1] = speculSection;

	return sections;
    }

    /**
     * 
     * @param elements
     * @param regExProcessed
     * @param sectionMap
     * @param section
     * @param speculSection
     * @param processed
     * @param speculProcessed
     * @param bil
     * @param dir
     */
    public void crossSectionsAB(final ArrayList<EElement> elements,
	    final HashMap<String, EElement> regExProcessed,
	    final HashMap<String, EElementList> sectionMap,
	    final SectionElement section, final SectionElement speculSection,
	    final HashMap<String, EElement> processed,
	    final HashMap<String, EElement> speculProcessed,
	    final DictionaryElement bil, final int dir) {

	for (final EElement e : elements) {
	    if (e.isRegEx()) {
		final String key = e.getRegEx().getValue();
		if (!regExProcessed.containsKey(key)) {
		    section.addEElement(e);
		    regExProcessed.put(e.getRegEx().getValue(), e);
		}
	    } else {
		final EElementList candidates = getPairs(e, sectionMap);
		crossElementAndPairs(e, candidates, section, speculSection,
			processed, speculProcessed, bil, dir);
	    }
	}

    }

    /**
     * 
     * @param e1
     * @param candidates
     * @param section
     * @param speculSection
     * @param processed
     * @param speculProcessed
     * @param bil
     * @param dir
     */
    public final void crossElementAndPairs(final EElement e1,
	    final EElementList candidates, final SectionElement section,
	    final SectionElement speculSection,
	    final HashMap<String, EElement> processed,
	    final HashMap<String, EElement> speculProcessed,
	    final DictionaryElement bil, final int dir) {
	if (candidates != null) {
	    for (final EElement e2 : candidates) {

		if (e1 != null && e2 != null) {
		    //EElement e = crossWithPatterns(e1, e2, dir);
		    //}

		    EElement e = crossSafely(e1, e2, dir, MATCH_CATEGORY, "SAFE");
		    if (e != null) {
			final String str = e.getHash();
			if (!processed.containsKey(str)) {
			    section.addEElement(e);
			    processed.put(str, e);
			}
		    } else {
			e = crossSafely(e1, e2, dir, DO_NOT_MATCH_CATEGORY,
			"SPECULATION");
			if (e != null) {
			    final String strSpecul = e.getHash();
			    if (!speculProcessed.containsKey(strSpecul)) {
				speculSection.addEElement(e);
				speculProcessed.put(strSpecul, e);
			    }
			}
		    }
		}
	    }
	}
    }

    /**
     * 
     * @param a
     * @param b
     * @param dir
     * @return
     */
    /*
    public final EElement crossWithPatterns(final EElement a, final EElement b,
	    final int dir) {
	EElement e1, e2;
	if (dir == 0) {
	    e1 = a;
	    e2 = b;
	} else {
	    e2 = a;
	    e1 = b;
	}

	EElement e = new EElement();

	if (e1.is_RL_or_LRRL() && e2.is_LR_or_LRRL()) {
	    String cat1L = e1.getCategory("L");
	    String cat1R = e1.getCategory("R");
	    String cat2L = e2.getCategory("L");
	    String cat2R = e2.getCategory("R");
	    try {
		if (compare4Strings(cat1L, cat1R, cat2L, cat2R)) {

		    CrossModel cm = new CrossModel();
		    CrossAction crossAction = cm.tagElements(e1, e2);
		    CrossModel origCM = this.getCrossModel();
		    String action = origCM.matches(crossAction);

		    if (action != null) {
			e1.print("L");
			e1.print("R");
			e2.print("L");
			e2.print("R");
			System.err.println("----------------");
			System.err.println("Cross action detected: " + action);

		    }
		    crossAction = null;
		}
	    } catch (final NullPointerException excep) {
	    }
	}
	return e;

    }
     */

    /**
     * 
     * @param a
     * @param b
     * @param dir
     * @param sameCategory
     * @return
     */
    public final EElement crossSafely(final EElement a, final EElement b,
	    final int dir, final boolean sameCategory, final String type) {

	EElement e1, e2;
	if (dir == 0) {
	    e1 = a;
	    e2 = b;
	} else {
	    e2 = a;
	    e1 = b;
	}

	EElement e = null;
	int thecase = -1;

	// Only to consider A -> B and B -> C (and LR/RL in both entries)

	if (e1.is_RL_or_LRRL() && e2.is_LR_or_LRRL()) {
	    String cat1L = e1.getCategory("L");
	    String cat1R = e1.getCategory("R");
	    String cat2L = e2.getCategory("L");
	    String cat2R = e2.getCategory("R");
	    try {
		if (!sameCategory
			|| compare4Strings(cat1L, cat1R, cat2L, cat2R)) {


		    final SElementList sE1L = e1.getSide("L").getSElements();
		    final SElementList sE1R = e1.getSide("R").getSElements();
		    final SElementList sE2L = e2.getSide("L").getSElements();
		    final SElementList sE2R = e2.getSide("R").getSElements();
		    // We get x, T, U, V, W and X
		    final ArrayList<Object> vars = getSubstrings(sE1L, sE1R, sE2L, sE2R, sameCategory);

		    thecase = checkCase(vars);
		    switch (thecase) {
		    case 1:
			e = crossEntries_Case1(e1, e2, vars);
			break;
		    case 2:
			e = crossEntries_Case2(e1, e2, vars);
			break;
		    case 3:
			e = crossEntries_Case3(e1, e2, vars);
			break;
		    case 4:
			e = crossEntries_Case4(e1, e2, vars);
			break;
		    case 5:
			e = crossEntries_Case5(e1, e2, vars);
			break;
		    }

		}
	    } catch (final NullPointerException excep) {
	    }

	    if (sameCategory && (e != null)) {
		logCrossing(e1, e2, e, thecase);
	    }

	}

	return e;
    }

    /**
     * 
     * @param e1
     * @param e2
     * @param e
     * @param thecase
     */
    private final void logCrossing(final EElement e1, final EElement e2,
	    final EElement e, final int thecase) {
	//System.err.println("\nCASE " + thecase);
	String r1 = e1.getRestriction();
	final int iR1 = getRestrictionCode(r1);
	r1 = getRestrictionString2(iR1);
	r1 = reverseRestriction(r1);
	String r2 = e2.getRestriction();
	final int iR2 = getRestrictionCode(r2);
	r2 = getRestrictionString2(iR2);

	/*
	System.err.println("");
	System.err.println("Crossing: " + r1 + " x " + r2);
	System.err.print("A: ");
	e1.getSide("R").print();
	System.err.print("B: ");
	e1.getSide("L").print();
	System.err.print("B: ");
	e2.getSide("L").print();
	System.err.print("C: ");
	e2.getSide("R").print();
	System.err.println("------------------------");
	*/
	if (e != null) {
	    String r = e.getRestriction();
	    final int iR = getRestrictionCode(r);
	    r = getRestrictionString2(iR);
	    /*
	    System.err.println("Result: " + r);
	    System.err.print("A: ");
	    e.getSide("L").print();
	    System.err.print("C: ");
	    e.getSide("R").print();
	    */
	} else {
	    //System.err.println(r1 + " x " + r2 + " --> X");
	}

    }

    /**
     * 
     * @param e1
     * @param e2
     * @param vars
     * @return
     */
    public final EElement crossEntries_Case1(final EElement e1,
	    final EElement e2, final ArrayList<Object> vars) {
	final EElement e = crossEntries(e1, e2);
	e.addComments("CROSSING CASE 1");
	return e;
    }

    /**
     * 
     * @param e1
     * @param e2
     * @param vars
     * @return
     */
    public final EElement crossEntries_Case2(final EElement e1,
	    final EElement e2, final ArrayList<Object> vars) {
	final EElement e = crossEntries(e1, e2);
	final LElement lE = (LElement) e.getSide("L");
	final SElementList sEs = (SElementList) vars.get(4);
	for (final SElement s : sEs) {
	    lE.addChild(s);
	}
	e.addComments("CROSSING CASE 2");
	return e;
    }

    /**
     * 
     * @param e1
     * @param e2
     * @param vars
     * @return
     */
    public final EElement crossEntries_Case3(final EElement e1,
	    final EElement e2, final ArrayList<Object> vars) {
	final EElement e = crossEntries(e1, e2);
	final RElement rE = (RElement) e.getSide("R");
	final SElementList V = (SElementList) vars.get(3);
	for (final SElement s : V) {
	    rE.addChild(s);
	}
	e.addComments("CROSSING CASE 3");
	return e;
    }

    /**
     * 
     * @param e1
     * @param e2
     * @param vars
     * @return
     */
    public final EElement crossEntries_Case4(final EElement e1,
	    final EElement e2, final ArrayList<Object> vars) {
	final SElementList V2 = (SElementList) vars.get(7);

	final EElement e = crossEntries(e1, e2);
	final RElement rE = (RElement) e.getSide("R");
	for (final SElement s : V2) {
	    rE.addChild(s);
	}
	e.addComments("CROSSING CASE 4");
	return e;
    }

    /**
     * 
     * @param e1
     * @param e2
     * @param vars
     * @return
     */
    public final EElement crossEntries_Case5(final EElement e1,
	    final EElement e2, final ArrayList<Object> vars) {
	final SElementList W2 = (SElementList) vars.get(6);

	final EElement e = crossEntries(e1, e2);
	final LElement lE = (LElement) e.getSide("L");
	for (final SElement s : W2) {
	    lE.addChild(s);
	}
	e.addComments("CROSSING CASE 5");
	return e;
    }

    /**
     * 
     * @param vars
     * @return
     */
    public final int checkCase(final ArrayList<Object> vars) {
	// SElement x = (SElement) vars.get(0);
	// SElementList T = (SElementList) vars.get(1);
	final SElementList U = (SElementList) vars.get(2);
	final SElementList V = (SElementList) vars.get(3);
	final SElementList W = (SElementList) vars.get(4);
	final SElementList X = (SElementList) vars.get(5);

	// case 1
	if (U.isEmpty() && V.isEmpty() && W.isEmpty() && X.isEmpty()) {
	    return 1;
	}
	// case 2
	if (U.isEmpty() && V.isEmpty() && !W.isEmpty() && !X.isEmpty()
		&& !W.equals(X)) {
	    return 2;
	}
	// case 3
	if (!U.equals(V) && !U.isEmpty() && !V.isEmpty() && W.isEmpty()
		&& X.isEmpty()) {
	    return 3;
	}
	// case 4

	if ((U.size() > 1) && (V.size() > 1)) {
	    final SElementList[] u2v2 = getU2V2(W, U, V);
	    final SElementList U2 = u2v2[0];
	    final SElementList V2 = u2v2[1];
	    if (!W.equals(X) && (u2v2 != null)) {
		vars.add(U2);
		vars.add(V2);
		return 4;
	    }
	}
	// case 5
	if ((W.size() > 1) && (X.size() > 1)) {
	    final SElementList[] w2x2 = getU2V2(V, W, X);
	    final SElementList W2 = w2x2[0];
	    final SElementList X2 = w2x2[1];
	    if (!U.equals(V) && (w2x2 != null)) {
		vars.add(W2);
		vars.add(X2);
		return 5;
	    }
	}
	return -1;
    }

    /**
     * 
     * @param W
     * @param U
     * @param V
     * @return
     */
    public final SElementList[] getU2V2(final SElementList W,
	    final SElementList U, final SElementList V) {
	final SElementList[] u2v2 = null;
	final SElementList U2 = splitString(U, W);
	final SElementList V2 = splitString(V, W);
	if ((U2 == null) || (V2 == null)) {
	    return null;
	}
	u2v2[0] = U2;
	u2v2[1] = V2;

	return u2v2;
    }

    /**
     * 
     * @param WU2
     * @param W
     * @return
     */
    public final SElementList splitString(final SElementList WU2,
	    final SElementList W) {
	SElementList sEList = new SElementList();
	int i = 0;
	if (!W.isEmpty() && !WU2.isEmpty() && (W.size() <= WU2.size())) {
	    for (i = 0; i < W.size(); i++) {
		final SElement sE_W = W.get(i);
		final SElement sE_WU2 = WU2.get(i);
		if (!sE_W.equals(sE_WU2)) {
		    return null;
		}
	    }

	    for (int j = i; j < WU2.size(); j++) {
		final SElement sE = WU2.get(j);
		sEList.add(sE);
	    }
	}
	if (sEList.isEmpty()) {
	    return null;
	}
	return sEList;
    }

    /**
     * 
     * @param sE1L
     * @param sE1R
     * @param sE2L
     * @param sE2R
     * @param sameCategory
     * @return
     */
    private ArrayList<Object> getSubstrings(final SElementList sE1L,
	    final SElementList sE1R, final SElementList sE2L,
	    final SElementList sE2R, final boolean sameCategory) {
	SElement x = null;

	final ArrayList<Object> vars = new ArrayList<Object>();

	final SElementList T = new SElementList();
	SElementList U, V, W, X;

	final int minLength = getMinLength(sE1L, sE1R, sE2L, sE2R);

	for (int i = 0; i < minLength; i++) {
	    final SElement s1L = sE1L.get(i);
	    final SElement s1R = sE1R.get(i);
	    final SElement s2L = sE2L.get(i);
	    final SElement s2R = sE2R.get(i);

	    if (compare4Strings(s1L.getValue(), s1R.getValue(), s2L.getValue(),
		    s2R.getValue())) {
		// Common category
		if (i == 0) {
		    x = sE1L.get(i);
		} else {
		    T.add(sE1L.get(i));
		}
	    }

	}

	U = getTail(x, T, sE1R, sameCategory);
	V = getTail(x, T, sE1L, sameCategory);
	W = getTail(x, T, sE2L, sameCategory);
	X = getTail(x, T, sE2R, sameCategory);
	vars.add(0, x); // category
	vars.add(1, T); // longest common string
	vars.add(2, U);
	vars.add(3, V);
	vars.add(4, W);
	vars.add(5, X);

	return vars;
    }

    /**
     * 
     * @param x
     * @param T
     * @param sE
     * @return
     */
    public final SElementList getTail(final SElement x, final SElementList T,
	    final SElementList sE, final boolean sameCategory) {
	final SElementList tail = new SElementList();

	int i = 0;
	int j = 0;
	for (final SElement s : sE) {
	    if (i == 0) {
		if (sameCategory && !x.getValue().equals(s.getValue())) {
		    return null;
		}
	    } else {
		if (j < T.size()) {
		    final SElement t = T.get(j);
		    if (!s.getValue().equals(t.getValue())) {
			return null;
		    }
		    j++;
		} else {
		    tail.add(s);
		}
	    }
	    i++;
	}
	return tail;
    }

    /**
     * 
     * @param str1
     * @param str2
     * @param str3
     * @param str4
     * @return
     */
    private final boolean compare4Strings(final String str1, final String str2,
	    final String str3, final String str4) {
	if (str1.equals(str2) && str2.equals(str3) && str3.equals(str4)) {
	    // System.err.println("Same: " + "(" + str1 + "," + str2 + "," +
	    // str3 + "," + str4 + ")");
	    return true;
	} else {
	    return false;
	}
    }

    /**
     * 
     * @param str1
     * @param str2
     * @param str3
     * @param str4
     * @return
     */
    private final int getMinLength(final SElementList sE1L,
	    final SElementList sE1R, final SElementList sE2L,
	    final SElementList sE2R) {
	final int l1 = sE1L.size();
	final int l2 = sE1R.size();
	final int l3 = sE2L.size();
	final int l4 = sE2R.size();

	return (Math.min(Math.min(l1, l2), Math.min(l3, l4)));
    }

    /**
     * 
     * @param e
     * @param hm
     * @return
     */
    public final EElementList getPairs(final EElement e,
	    final HashMap<String, EElementList> hm) {
	EElementList pairs = null;
	String lemma = e.getValue("L");
	lemma = DicTools.clearTags(lemma);
	pairs = hm.get(lemma);
	return pairs;
    }

    /**
     * 
     * @param e1
     * @param e2
     * @return
     */
    public EElement crossEntries(final EElement e1, final EElement e2) {

	if ((e1 != null) && (e2 != null)) {
	    final EElement e = new EElement();
	    String restriction = "";
	    String r1 = e1.getRestriction();
	    String r2 = e2.getRestriction();
	    final int iR = resolveRestriction(r1, r2);
	    if (iR != NONE) {
		restriction = getRestrictionString(iR);
		e.setRestriction(restriction);
	    } else {
		return null;
	    }

	    final PElement pE = new PElement();
	    final LElement lE = new LElement();
	    pE.setLElement(lE);
	    final RElement rE = new RElement();
	    pE.setRElement(rE);
	    e.addChild(pE);
	    e.getP().getL().setValue(e1.getValue("R"));
	    e.getP().getL().setChildren(e1.getChildren("R"));

	    final String author = mergeAttributes(e1.getAuthor(), e2
		    .getAuthor());
	    e.setAuthor(author);
	    final String comment = mergeAttributes(e1.getComment(), e2
		    .getComment());
	    e.setComment(comment);

	    e.getP().getR().setValue(e2.getValue("R"));
	    e.getP().getR().setChildren(e2.getChildren("R"));

	    if (r1 == null) {
		r1 = "LR/RL";
	    }
	    if (r2 == null) {
		r2 = "LR/RL";
	    }
	    if (restriction == null) {
		restriction = "LR/RL";
	    }
	    if (r1.equals("LR")) {
		r1 = "RL";
	    }
	    if (r1.equals("RL")) {
		r1 = "LR";
	    }

	    e.addComments("A-C = " + restriction + " ::: A-B = " + r1
		    + " X B-C = " + r2);

	    return e;
	} else {
	    return null;
	}

    }

    /**
     * 
     * @param attr1
     * @param attr2
     * @return
     */
    public final String mergeAttributes(final String attr1, final String attr2) {
	if ((attr1 != null) && (attr2 != null)) {
	    final String attr = attr1 + "/" + attr2;
	    return attr;
	} else {
	    if ((attr1 == null) && (attr2 != null)) {
		return attr2;
	    }
	    if ((attr1 != null) && (attr2 == null)) {
		return attr1;
	    }
	}
	return null;
    }

    /**
     * 
     * @param r1
     * @param r2
     * @return
     */
    private final int resolveRestriction(final String r1, final String r2) {
	final int c1 = getRestrictionCode(r1);
	final int c2 = getRestrictionCode(r2);

	final int newRestriction = rMatrix[c1][c2];

	return newRestriction;
    }

    /**
     * 
     * @param r
     * @return
     */
    private final int getRestrictionCode(final String r) {
	if (r != null) {
	    if (r.equals("LR")) {
		return LR;
	    } else {
		if (r.equals("RL")) {
		    return RL;
		} else {
		    return NONE;
		}
	    }
	} else {
	    return BOTH;
	}
    }

    /**
     * 
     * @param code
     * @return
     */
    private final String getRestrictionString(final int code) {
	switch (code) {
	case LR:
	    return "LR";
	case RL:
	    return "RL";
	case BOTH:
	    return null;
	case NONE:
	    return null;
	default:
	    return null;
	}
    }

    /**
     * 
     * @param code
     * @return
     */
    private final String getRestrictionString2(final int code) {
	switch (code) {
	case LR:
	    return "LR";
	case RL:
	    return "RL";
	case BOTH:
	    return "BOTH";
	case NONE:
	    return "NONE";
	default:
	    return null;
	}
    }

    /**
     * 
     * @param r
     * @return
     */
    private final String reverseRestriction(final String r) {
	if (r.equals("LR")) {
	    return "RL";
	}
	if (r.equals("RL")) {
	    return "LR";
	}
	if (r.equals("BOTH")) {
	    return "BOTH";
	}
	return "NONE";
    }

    /*
    public final CrossModel getCrossModel() {
        return crossModel;
    }
     */

    /*
    public final void setCrossModel(CrossModel crossModel) {
        this.crossModel = crossModel;
    }
     */

}
