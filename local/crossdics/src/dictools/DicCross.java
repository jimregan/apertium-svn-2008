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

import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Set;

import dics.elements.dtd.AlphabetElement;
import dics.elements.dtd.ContentElement;
import dics.elements.dtd.DictionaryElement;
import dics.elements.dtd.EElement;
import dics.elements.dtd.Element;
import dics.elements.dtd.LElement;
import dics.elements.dtd.PElement;
import dics.elements.dtd.RElement;
import dics.elements.dtd.SElement;
import dics.elements.dtd.SdefElement;
import dics.elements.dtd.SdefsElement;
import dics.elements.dtd.SectionElement;
import dics.elements.dtd.TextElement;
import dics.elements.utils.DicSet;
import dics.elements.utils.DictionaryElementList;
import dics.elements.utils.EElementList;
import dics.elements.utils.EElementMap;
import dics.elements.utils.EHashMap;
import dics.elements.utils.ElementList;
import dics.elements.utils.Msg;
import dictools.crossmodel.Action;
import dictools.crossmodel.ActionSet;
import dictools.crossmodel.ConstantMap;
import dictools.crossmodel.CrossAction;
import dictools.crossmodel.CrossModel;
import dictools.crossmodel.CrossModelFST;
import dictools.crossmodel.CrossModelReader;
import dictools.crossmodel.Pattern;

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
    private EHashMap processed;

    /**
         * 
         */
    private EHashMap speculProcessed;

    /**
         * 
         */
    private EHashMap regExProcessed;

    /**
         * 
         */
    private CrossModel crossModel;

    /**
         * 
         */
    private CrossModelFST crossModelFST;

    /**
         * 
         */
    private boolean crossWithPatterns = false;

    /**
         * 
         */
    private String crossModelFileName;

    /**
         * 
         */
    private HashMap<String, CrossAction> nDCrossActions;

    /**
         * 
         */
    private CrossModel nDCrossModel;

    /**
         * 
         */
    private String[] arguments;

    /**
         * 
         */
    private DicSet dicSet;

    /**
         * 
         */
    private int NDcounter;

    /**
         * 
         */
    private HashMap<String, Integer> usedPatterns;

    /**
         * 
         */
    private int taskOrder;

    /**
         * 
         */
    private Msg msg;

    /**
         * 
         * 
         */
    public DicCross() {
	msg = new Msg();
	msg.setLogFileName("cross.log");
	rMatrix = new int[3][3];
	fillOutRestrictionMatrix();
	processed = new EHashMap();
	speculProcessed = new EHashMap();
	regExProcessed = new EHashMap();
	nDCrossActions = new HashMap<String, CrossAction>();
	nDCrossModel = new CrossModel();
	usedPatterns = new HashMap<String, Integer>();
	taskOrder = 1;
    }

    /**
         * 
         * 
         */
    private final void readCrossModel() {
	try {
	    System.out.print("[" + (taskOrder++) + "] Reading cross model ("
		    + getCrossModelFileName() + ")");
	    final CrossModelReader cmr = new CrossModelReader(
		    getCrossModelFileName());
	    CrossModel cm = cmr.readCrossModel();
	    msg.out(" (" + cm.getCrossActions().size()
		    + " patterns processed).");

	    setCrossModel(cm);
	    CrossModelFST fst = new CrossModelFST(getCrossModel());
	    setCrossModelFST(fst);
	} catch (final Exception e) {
	    e.printStackTrace();
	    msg.out("Error reading cross model");
	    System.exit(-1);
	}
    }

    /**
         * 
         * 
         */
    private final void fillOutRestrictionMatrix() {
	// Note: B-A ^ B-C = A-C
	setRMatrixValue(LR, LR, NONE);
	setRMatrixValue(LR, RL, RL);
	setRMatrixValue(LR, BOTH, RL);

	setRMatrixValue(RL, LR, LR);
	setRMatrixValue(RL, RL, NONE);
	setRMatrixValue(RL, BOTH, LR);

	setRMatrixValue(BOTH, LR, LR);
	setRMatrixValue(BOTH, RL, RL);
	setRMatrixValue(BOTH, BOTH, BOTH);
    }

    /**
         * 
         * @param dic1
         * @param dic2
         * @return
         */
    public DictionaryElement[] crossDictionaries(final DicSet dicSet) {
	final DictionaryElement[] dics = new DictionaryElement[2];

	if (isCrossWithPatterns()) {
	    readCrossModel();
	}

	final DictionaryElement dic1 = dicSet.getBil1();
	final DictionaryElement dic2 = dicSet.getBil2();
	setBilAB(dic1);
	setBilBC(dic2);

	final DictionaryElement dic = new DictionaryElement();
	final DictionaryElement specul = new DictionaryElement();

	// encoding
	final String encoding = this.crossXmlEncodings(dic1.getXmlEncoding(),
		dic2.getXmlEncoding());
	dic.setXmlEncoding(encoding);

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

	msg.out("[" + (taskOrder++) + "] Sorting crossed dictionary...");
	Collections.sort(dic.getEntries());

	if (isCrossWithPatterns()) {
	    getNDCrossModel().printXML("dix/patterns-not-detected.xml");
	    // System.out.println("ND-all: " + this.NDcounter);
	}

	dics[0] = dic;
	dics[1] = specul;

	return dics;
    }

    /**
         * 
         * @param encoding1
         * @param encoding2
         */
    private final String crossXmlEncodings(final String encoding1,
	    final String encoding2) {
	return encoding1;
    }

    /**
         * 
         * @param alphabet1
         * @param alphabet2
         * @return
         */
    private final AlphabetElement crossAlphabets(
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
    private final SdefsElement crossSdefs(final SdefsElement sdefs1,
	    final SdefsElement sdefs2) {
	msg.out("[" + (taskOrder++) + "] Crossing definitions...");
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
		SdefElement sdef1 = sdefList.get(sdef2.getValue());

		if ((sdef1.getComment() != null)
			&& (sdef2.getComment() == null)) {
		    sdef2.setComment(sdef1.getComment());
		}

		if ((sdef1.getComment() != null)
			&& (sdef2.getComment() != null)) {
		    if (!sdef1.getComment().equals(sdef2.getComment())) {
			sdef2.setComment(sdef1.getComment() + "/"
				+ sdef2.getComment());
		    }
		}
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
    private SectionElement[] crossSections(final SectionElement section1,
	    final SectionElement section2, final SdefsElement sdefs) {

	msg.out("[" + (taskOrder++) + "] Crossing sections '"
		+ section1.getID() + "' and '" + section2.getID() + "'");
	final SectionElement[] sections = new SectionElement[2];

	final SectionElement section = new SectionElement();
	final SectionElement speculSection = new SectionElement("main",
		"standard");

	section.setID(section1.getID());
	section.setType(section1.getType());

	final EElementList elements1 = section1.getEElements();
	final EElementList elements2 = section2.getEElements();

	EElementMap section1Map = DicTools.buildHash(elements1);
	EElementMap section2Map = DicTools.buildHash(elements2);

	crossSectionsAB(elements1, section2Map, section, speculSection,
		getBilBC(), 0);
	section2Map = null;

	crossSectionsAB(elements2, section1Map, section, speculSection,
		getBilAB(), 1);
	section1Map = null;

	sections[0] = section;
	sections[1] = speculSection;

	return sections;
    }

    /**
         * 
         * @param elements
         * @param sectionMap
         * @param section
         * @param speculSection
         * @param bil
         * @param dir
         */
    private void crossSectionsAB(final EElementList elements,
	    final EElementMap sectionMap, final SectionElement section,
	    final SectionElement speculSection, final DictionaryElement bil,
	    final int dir) {

	for (final EElement e : elements) {
	    if (e.isRegEx()) {
		final String key = e.getRegEx().getValue();
		if (!getRegExProcessed().containsKey(key)) {
		    section.addEElement(e);
		    getRegExProcessed().put(e.getRegEx().getValue(), e);
		}
	    } else {
		final EElementList candidates = getPairs(e, sectionMap);
		if (isCrossWithPatterns()) {
		    crossElementAndPairs(e, candidates, section, speculSection,
			    bil, dir);
		}
	    }
	}

    }

    /**
         * 
         * @param e1
         * @param candidates
         * @param section
         * @param speculProcessed
         * @param bil
         * @param dir
         */
    private final void crossElementAndPairs(final EElement e1,
	    final EElementList candidates, final SectionElement section,
	    final SectionElement speculSection, final DictionaryElement bil,
	    final int dir) {
	if (candidates != null) {
	    for (EElement e2 : candidates) {
		if ((e1 != null) && (e2 != null)) {
		    EElementList eList = cross(e1, e2, dir);
		    if (eList != null) {
			for (EElement e : eList) {
			    if (e != null) {
				final String str = e.getHash();
				if (!getProcessed().containsKey(str)) {
				    section.addEElement(e);
				    getProcessed().put(str, e);
				    String actionID = e.getPatternApplied();
				    if (!usedPatterns.containsKey(actionID)) {
					usedPatterns.put(actionID, new Integer(
						1));
				    } else {
					Integer times = usedPatterns
						.get(actionID);
					int t = times.intValue();
					t++;
					usedPatterns.put(actionID, new Integer(
						t));
				    }
				}
			    }
			}
		    } else {
			EElement e = speculate(e1, e2, MATCH_CATEGORY, dir);
			if (e != null) {
			    final String strSpecul = e.getHash();
			    if (!getSpeculProcessed().containsKey(strSpecul)) {
				speculSection.addEElement(e);
				getSpeculProcessed().put(strSpecul, e);
			    }
			}
		    }

		}
	    }
	}
    }

    /**
         * 
         * @param e1
         * @param e2
         * @param matchCategory
         * @return
         */
    private final EElement speculate(EElement e1, EElement e2,
	    boolean matchCategory, int dir) {
	EElement e = null;
	try {
	    if (dir != 0) {
		EElement aux;
		aux = e2;
		e2 = e1;
		e1 = aux;
	    }

	    if (matchCategory) {
		if (e1.getCategory("L").equals(e2.getCategory("L"))) {
		    e = crossEntries(e1, e2);
		}
	    } else {
		e = crossEntries(e1, e2);
	    }
	} catch (NullPointerException npe) {

	}
	return e;
    }

    /**
         * 
         * @param a
         * @param b
         * @param dir
         * @return
         */
    private final EElementList cross(EElement e1, EElement e2, final int dir) {

	if (dir != 0) {
	    EElement aux;
	    aux = (EElement) e2.clone();
	    e2 = (EElement) e1.clone();
	    e1 = aux;
	}

	msg.log("crossing " + e1.getValue() + " and " + e2.getValue());
	CrossAction crossAction = new CrossAction();
	Pattern entriesPattern = new Pattern(e1.reverse(), e2);
	crossAction.setPattern(entriesPattern);
	ActionSet actionSetFST = getCrossModelFST().getActionSet(crossAction);
	boolean nonDefinedPattern = true;
	if (actionSetFST != null) {
	    final String actionID = actionSetFST.getName();

	    EElementList actionEList = applyCrossAction(e1, e2, actionID,
		    crossAction);
	    if (!actionID.equals("default")) {
		nonDefinedPattern = false;
	    }
	    return actionEList;
	}

	if (nonDefinedPattern) {
	    int iR = resolveRestriction(e1.reverse().getRestriction(), e2
		    .getRestriction());
	    if (iR != NONE) {
		String cat1 = e1.getCategory("L");
		String cat2 = e2.getCategory("L");
		if ((cat1 != null) && (cat2 != null)) {
		    if (cat1.equals(cat2)) {
			insertNDCrossAction(crossAction);
		    }
		}
	    }
	    return null;
	}
	return null;

    }

    /**
         * 
         * @param e1
         * @param e2
         * @param actionID
         * @param crossAction
         * @return
         */
    private final EElementList applyCrossAction(EElement e1, EElement e2,
	    final String actionID, final CrossAction entries) {
	EElementList elementList = new EElementList();
	final CrossAction cA = getCrossModel().getCrossAction(actionID);
	HashMap<String, ElementList> tails = cA.getActionSet().getTails();

	for (Action action : cA.getActionSet()) {

	    EElement eAction = action.getE();

	    final int iR = resolveRestriction(e1.getRestriction(), e2
		    .getRestriction());
	    if (iR != NONE) {

		final String restriction = getRestrictionString(iR);
		EElement actionE = assignValues(e1, e2, eAction, entries, tails);
		actionE.setPatternApplied(actionID);

		if (eAction.hasRestriction()) {
		    // restriction indicated in cross pattern
		    actionE.setComments("\tforced '" + eAction.getRestriction()
			    + "' restriction\n");
		    actionE.setRestriction(eAction.getRestriction());
		} else {
		    // automatically resolved restriction
		    actionE.setRestriction(restriction);
		}

		// cross comments & author
		final String author = mergeAttributes(e1.getAuthor(), e2
			.getAuthor());
		actionE.setAuthor(author);
		final String comment = mergeAttributes(e1.getComment(), e2
			.getComment());
		actionE.setComment(comment);

		actionE.addComments(actionID);
		elementList.add(actionE);
	    } else {
		return null;
	    }
	}
	return elementList;
    }

    /**
         * 
         * @param eAction
         * @param entries
         * @return
         */
    private final EElement assignValues(EElement e1, EElement e2,
	    EElement eAction, CrossAction entries,
	    HashMap<String, ElementList> tails) {
	ConstantMap constants = entries.getConstants();
	EElement eCrossed = new EElement();

	PElement pE = new PElement();

	ContentElement lE = eAction.getSide("L");
	ContentElement rE = eAction.getSide("R");

	LElement lE2 = new LElement();
	RElement rE2 = new RElement();

	assignValuesSide(lE2, lE, constants, e1, tails);
	assignValuesSide(rE2, rE, constants, e2, tails);

	pE.setLElement(lE2);
	pE.setRElement(rE2);
	eCrossed.addChild(pE);
	return eCrossed;
    }

    /**
         * 
         * @param ceWrite
         * @param ceRead
         * @param constants
         * @param ei
         */
    private final void assignValuesSide(ContentElement ceWrite,
	    final ContentElement ceRead, final ConstantMap constants,
	    final EElement ei, HashMap<String, ElementList> tails) {
	ceWrite.addChild(new TextElement(ei.getSide("R").getValue()));
	for (Element e : ceRead.getSElements()) {
	    String v = e.getValue();

	    if (!v.equals("0")) {
		if (!constants.containsKey(e.getValue())) {
		    ceWrite.addChild(new SElement(e.getValue()));
		} else {
		    String value = constants.get(e.getValue());
		    ceWrite.addChild(new SElement(value));
		}
	    } else {
		SElement sE = (SElement) e;
		if (!tails.containsKey(sE.getTemp())) {
		    // ceWrite.addChild(new SElement(e.getValue()));
		} else {
		    ElementList tail = tails.get(sE.getTemp());
		    for (Element elem : tail) {
			if (!constants.containsKey(elem.getValue())) {
			    ceWrite.addChild(new SElement(elem.getValue()));
			} else {
			    String value = constants.get(elem.getValue());
			    ceWrite.addChild(new SElement(value));
			}
			// ceWrite.addChild(elem);
		    }
		}
	    }
	}
    }

    /**
         * 
         * @param e
         * @param hm
         * @return
         */
    private final EElementList getPairs(final EElement e, final EElementMap hm) {
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
    private EElement crossEntries(final EElement e1, final EElement e2) {
	EElement e = null;
	try {
	    String r1 = e1.getRestriction();
	    String r2 = e2.getRestriction();
	    final int r = resolveRestriction(r1, r2);
	    if (r == NONE) {
		return e;
	    } else {
		e = new EElement();
		String restriction = getRestrictionString(r);
		e.setRestriction(restriction);
		PElement pE = new PElement();
		e.addChild(pE);
		LElement lE = new LElement();
		RElement rE = new RElement();
		pE.setLElement(lE);
		pE.setRElement(rE);

		e.getP().getL().setChildren(e1.getChildren("R"));
		e.getP().getR().setChildren(e2.getChildren("R"));
		String author = mergeAttributes(e1.getAuthor(), e2.getAuthor());
		e.setAuthor(author);
		String comment = mergeAttributes(e1.getComment(), e2
			.getComment());
		e.setComment(comment);
	    }
	} catch (NullPointerException npe) {

	} catch (Exception exception) {

	}
	return e;
    }

    /**
         * 
         * @param attr1
         * @param attr2
         * @return
         */
    private final String mergeAttributes(final String attr1, final String attr2) {
	String attr = null;
	if ((attr1 != null) && (attr2 != null)) {
	    attr = attr1 + "/" + attr2;
	    return attr;
	} else {
	    if ((attr1 == null) && (attr2 != null)) {
		return attr2;
	    }
	    if ((attr1 != null) && (attr2 == null)) {
		return attr1;
	    }
	}
	return attr;
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
	final int r = rMatrix[c1][c2];
	return r;
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
         * @return
         */
    private final CrossModel getCrossModel() {
	return crossModel;
    }

    /**
         * 
         * @param crossModel
         */
    private final void setCrossModel(final CrossModel crossModel) {
	this.crossModel = crossModel;
    }

    /**
         * @return the crossWithPatterns
         */
    private final boolean isCrossWithPatterns() {
	return crossWithPatterns;
    }

    /**
         * @param crossWithPatterns
         *                the crossWithPatterns to set
         */
    public final void setCrossWithPatterns(final boolean crossWithPatterns) {
	this.crossWithPatterns = crossWithPatterns;
    }

    /**
         * @return the processed
         */
    private final EHashMap getProcessed() {
	return processed;
    }

    /**
         * @return the regExProcessed
         */
    private final EHashMap getRegExProcessed() {
	return regExProcessed;
    }

    /**
         * @return the speculProcessed
         */
    private final EHashMap getSpeculProcessed() {
	return speculProcessed;
    }

    /**
         * @return the bilAB
         */
    private final DictionaryElement getBilAB() {
	return bilAB;
    }

    /**
         * @param bilAB
         *                the bilAB to set
         */
    private final void setBilAB(final DictionaryElement bilAB) {
	this.bilAB = bilAB;
    }

    /**
         * @return the bilBC
         */
    private final DictionaryElement getBilBC() {
	return bilBC;
    }

    /**
         * @param bilBC
         *                the bilBC to set
         */
    private final void setBilBC(final DictionaryElement bilBC) {
	this.bilBC = bilBC;
    }

    /**
         * @return the rMatrix
         */
    private final int[][] getRMatrix() {
	return rMatrix;
    }

    /**
         * 
         * @param i
         * @param j
         * @param value
         */
    private final void setRMatrixValue(final int i, final int j, final int value) {
	getRMatrix()[i][j] = value;
    }

    /**
         * @return the crossModelFileName
         */
    private final String getCrossModelFileName() {
	return crossModelFileName;
    }

    /**
         * @param crossModelFileName
         *                the crossModelFileName to set
         */
    public final void setCrossModelFileName(final String crossModelFileName) {
	this.crossModelFileName = crossModelFileName;
    }

    /**
         * @return the ndCrossActions
         */
    private final void insertNDCrossAction(final CrossAction cA) {
	if (!nDCrossActions.containsKey(cA.getPattern().toString())) {
	    nDCrossActions.put(cA.getPattern().toString(), cA);
	    nDCrossModel.addCrossAction(cA);
	} else {
	    CrossAction ca = nDCrossActions.get(cA.getPattern().toString());
	    ca.incrementOccurrences();
	    NDcounter++;
	}
    }

    /**
         * @return the nDCrossModel
         */
    private final CrossModel getNDCrossModel() {
	return nDCrossModel;
    }

    /**
         * @return the crossModelFST
         */
    private final CrossModelFST getCrossModelFST() {
	return crossModelFST;
    }

    /**
         * @param crossModelFST
         *                the crossModelFST to set
         */
    private final void setCrossModelFST(CrossModelFST crossModelFST) {
	this.crossModelFST = crossModelFST;
    }

    /**
         * 
         * 
         */
    public final void doCross() {
	processArguments();
	actionCross();
    }

    /**
         * 
         * @param dicSet
         */
    private final void actionCross() {
	DicSet dicSet = getDicSet();
	actionConsistent(dicSet, "yes");

	// final DicCross dc = new DicCross();
	// dc.setCrossWithPatterns(true); // true means uses patterns
	setCrossWithPatterns(true);
	// dc.setCrossModelFileName(getCrossModelFileName());
	setCrossModelFileName(getCrossModelFileName());

	// final DictionaryElement[] bils = dc.crossDictionaries(dicSet);
	final DictionaryElement[] bils = crossDictionaries(dicSet);

	final DictionaryElement bilCrossed = bils[0];
	final DictionaryElement bilSpecul = bils[1];

	final String sl = dicSet.getBil1().getTL();
	final String tl = dicSet.getBil2().getTL();
	bilCrossed.setType("BIL");
	bilCrossed.setSL(sl);
	bilCrossed.setTL(tl);

	msg.out("[" + (taskOrder++)
		+ "] Making morphological dicionaries consistent ...");

	final EElementList[] commonCrossedMons = DicTools.makeConsistent(
		bilCrossed, dicSet.getMon1(), dicSet.getMon2());
	final EElementList crossedMonA = commonCrossedMons[0];
	final EElementList crossedMonB = commonCrossedMons[1];

	final EElementList[] commonSpeculMons = DicTools.makeConsistent(
		bilSpecul, dicSet.getMon1(), dicSet.getMon2());
	final EElementList speculMonA = commonSpeculMons[0];
	final EElementList speculMonB = commonSpeculMons[1];

	final DictionaryElement monACrossed = new DictionaryElement(dicSet
		.getMon1());
	monACrossed.setMainSection(crossedMonA);

	final DictionaryElement monBCrossed = new DictionaryElement(dicSet
		.getMon2());
	monBCrossed.setMainSection(crossedMonB);

	final DictionaryElement monASpecul = new DictionaryElement(dicSet
		.getMon1());
	monASpecul.setMainSection(speculMonA);

	final DictionaryElement monBSpecul = new DictionaryElement(dicSet
		.getMon2());
	monBSpecul.setMainSection(speculMonB);

	final DictionaryElementList del = new DictionaryElementList();
	del.add(bilCrossed);
	del.add(bilSpecul);
	del.add(monACrossed);
	del.add(monBCrossed);
	del.add(monASpecul);
	del.add(monBSpecul);

	printXMLCrossedAndSpecul(del, sl, tl);

	msg.out("[" + (taskOrder++) + "] Done!");
    }

    /**
         * 
         * @param del
         * @param sl
         * @param tl
         */
    private final void printXMLCrossedAndSpecul(
	    final DictionaryElementList del, final String sl, final String tl) {
	final DictionaryElement bilCrossed = del.get(0);
	final DictionaryElement bilSpecul = del.get(1);
	final DictionaryElement monACrossed = del.get(2);
	final DictionaryElement monBCrossed = del.get(3);
	final DictionaryElement monASpecul = del.get(4);
	final DictionaryElement monBSpecul = del.get(5);

	int i = 0;
	String patterns = "";
	bilCrossed.addComments("");
	bilCrossed.addComments("Patterns applied:");
	System.err.println("Patterns applied:");
	for (CrossAction cA : getCrossModel().getCrossActions()) {
	    String cAName = cA.getId();
	    if (!usedPatterns.containsKey(cAName)) {
		if (i != 0) {
		    patterns += ", " + cAName;
		} else {
		    patterns += cAName;
		}
		i++;
	    } else {
		String msg = "\t" + cAName + " (" + usedPatterns.get(cAName)
			+ " times)";
		System.err.println(msg);
		bilCrossed.addComments(msg);

	    }
	}
	System.err.println("[" + (taskOrder++) + "] Patterns never applied: "
		+ patterns);

	msg.out("[" + (taskOrder++) + "] Generating crossed dictionaries ...");

	bilCrossed.printXML("dix/apertium-" + sl + "-" + tl + "." + sl + "-"
		+ tl + "-crossed.dix");

	// This dictionary is not printed anymore
	// Default cross action in cross model replaces this idea of dic. of speculations	
	//bilSpecul.printXML("dix/apertium-" + sl + "-" + tl + "." + sl + "-" + tl + "-crossed-specul.dix");

	monACrossed.printXML("dix/apertium-" + sl + "-" + tl + "." + sl
		+ "-crossed.dix");
	monBCrossed.printXML("dix/apertium-" + sl + "-" + tl + "." + tl
		+ "-crossed.dix");

	// These dictionaries are not printed anymore.
	// Default cross action in cross model replaces this idea of dic. of speculations
	//monASpecul.printXML("dix/apertium-" + sl + "-" + tl + "." + sl + "-crossed-specul.dix");
	//monBSpecul.printXML("dix/apertium-" + sl + "-" + tl + "." + tl + "-crossed-specul.dix");
    }

    /**
         * 
         * @param dicSet
         * @param removeNotCommon
         * @return
         */
    private final DicConsistent actionConsistent(final DicSet dicSet,
	    final String removeNotCommon) {
	final DicConsistent dicConsistent = new DicConsistent(dicSet);
	dicConsistent.makeConsistentDictionaries(removeNotCommon);
	dicSet.printXML("consistent");
	return dicConsistent;
    }

    /**
         * 
         * @param arguments
         */
    private void processArguments() {
	final int nArgs = getArguments().length;
	String sDicMonA, sDicMonC, sDicBilAB, sDicBilBC;
	sDicMonA = sDicMonC = sDicBilAB = sDicBilBC = null;
	boolean bilABReverse, bilBCReverse;
	bilABReverse = bilBCReverse = false;

	for (int i = 1; i < nArgs; i++) {
	    String arg = getArguments()[i];
	    if (arg.equals("-monA")) {
		i++;
		arg = getArguments()[i];
		sDicMonA = arg;
		System.err.println("Monolingual A: '" + sDicMonA + "'");
	    }

	    if (arg.equals("-monC")) {
		i++;
		arg = getArguments()[i];
		sDicMonC = arg;
		System.err.println("Monolingual C: '" + sDicMonC + "'");
	    }

	    if (arg.equals("-bilAB")) {
		i++;
		arg = getArguments()[i];
		if (arg.equals("-r")) {
		    bilABReverse = true;
		    i++;
		}
		if (arg.equals("-n")) {
		    bilABReverse = false;
		    i++;
		}

		arg = getArguments()[i];
		sDicBilAB = arg;
		System.err.println("Bilingual A-B: '" + sDicBilAB + "'");
	    }

	    if (arg.equals("-bilBC")) {
		i++;
		arg = getArguments()[i];

		if (arg.equals("-r")) {
		    bilBCReverse = true;
		    i++;
		}
		if (arg.equals("-n")) {
		    bilBCReverse = false;
		    i++;
		}
		arg = getArguments()[i];
		sDicBilBC = arg;
		System.err.println("Bilingual B-C: '" + sDicBilBC + "'");
	    }

	    if (arg.equals("-cross-model")) {
		i++;
		arg = getArguments()[i];
		setCrossModelFileName(arg);
		System.err.println("Cross model: " + arg);
	    }

	    if (arg.equals("-debug")) {
		i++;
		msg.setDebug(true);
		msg.out("debug: on");
	    }

	}

	msg.out("[" + (taskOrder++) + "] Loading bilingual AB (" + sDicBilAB
		+ ")");
	final DictionaryElement bil1 = DicTools.readBilingual(sDicBilAB,
		bilABReverse);
	msg.out("[" + (taskOrder++) + "] Loading bilingual BC (" + sDicBilBC
		+ ")");
	final DictionaryElement bil2 = DicTools.readBilingual(sDicBilBC,
		bilBCReverse);
	msg.out("[" + (taskOrder++) + "] Loading monolingual A (" + sDicMonA
		+ ")");
	final DictionaryElement mon1 = DicTools.readMonolingual(sDicMonA);
	msg.out("[" + (taskOrder++) + "] Loading monolingual C (" + sDicMonC
		+ ")");
	final DictionaryElement mon2 = DicTools.readMonolingual(sDicMonC);

	DicSet dicSet = new DicSet(mon1, bil1, mon2, bil2);
	setDicSet(dicSet);
    }

    /**
         * @return the arguments
         */
    public final String[] getArguments() {
	return arguments;
    }

    /**
         * @param arguments
         *                the arguments to set
         */
    public final void setArguments(String[] arguments) {
	this.arguments = arguments;
    }

    /**
         * @return the dicSet
         */
    private final DicSet getDicSet() {
	return dicSet;
    }

    /**
         * @param dicSet
         *                the dicSet to set
         */
    private final void setDicSet(DicSet dicSet) {
	this.dicSet = dicSet;
    }

    /**
         * @return the msg
         */
    public final Msg getMsg() {
	return msg;
    }

    /**
         * @param msg
         *                the msg to set
         */
    public final void setMsg(Msg msg) {
	this.msg = msg;
    }

}
