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
import dics.elements.utils.EElementList;
import dics.elements.utils.EElementMap;
import dics.elements.utils.EHashMap;
import dictools.crossmodel.Action;
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
	private final boolean DO_NOT_MATCH_CATEGORY = false;

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
	 * 
	 */
	public DicCross() {
		rMatrix = new int[3][3];
		fillOutRestrictionMatrix();
		processed = new EHashMap();
		speculProcessed = new EHashMap();
		regExProcessed = new EHashMap();
		nDCrossActions = new HashMap<String, CrossAction>();
		nDCrossModel = new CrossModel();
	}

	/**
	 * 
	 * 
	 */
	private final void readCrossModel() {
		try {
			final CrossModelReader cmr = new CrossModelReader(
					getCrossModelFileName());
			setCrossModel(cmr.readCrossModel());
			CrossModelFST fst = new CrossModelFST(getCrossModel());
			setCrossModelFST(fst);
			final int nCrossActions = getCrossModel().getCrossActions().size();
			System.err.println("Cross actions: " + nCrossActions);
		} catch (final Exception e) {
			e.printStackTrace();
			System.err.println("Error reading cross model.");
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

		// alphabet
		final AlphabetElement alphabet = crossAlphabets(dic1.getAlphabet(), dic2.getAlphabet());
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

		if (isCrossWithPatterns()) {
			getNDCrossModel().printXML("dix/ND-cross-model.xml");
		}

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
	private SectionElement[] crossSections(final SectionElement section1,
			final SectionElement section2, final SdefsElement sdefs) {

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
					crossElementAndPairs(e, candidates, section,
							speculSection, bil, dir);
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
					EElement e = cross(e1, e2, dir);
					if (e != null) {
						final String str = e.getHash();
						if (!getProcessed().containsKey(str)) {
							section.addEElement(e);
							getProcessed().put(str, e);
						}
					} else {
						e = speculate(e1, e2, MATCH_CATEGORY, dir);
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
		} catch(NullPointerException npe) {
			
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
	private final EElement cross(EElement e1, EElement e2,
			final int dir) {
		if (dir != 0) {
			EElement aux;
			aux = (EElement)e2.clone();
			e2 = (EElement)e1.clone();
			e1 = aux;
		}

		CrossAction crossAction = new CrossAction();
		Pattern entriesPattern = new Pattern(e1.reverse(), e2);
		crossAction.setPattern(entriesPattern);
		Action actionFST = getCrossModelFST().getAction(crossAction);

		if (actionFST != null) {
			final String actionID = actionFST.getName();
			EElement actionE = applyCrossAction(e1, e2, actionID, crossAction);
			return actionE;
		} else {
			insertNDCrossAction(crossAction);
			return null;
		}
	}

	/**
	 * 
	 * @param e1
	 * @param e2
	 * @param actionID
	 * @param crossAction
	 * @return
	 */
	private final EElement applyCrossAction(EElement e1, EElement e2,
			final String actionID, final CrossAction entries) {
		final CrossAction cA = getCrossModel().getCrossAction(actionID);
		Action action = cA.getAction();
		EElement eAction = action.getE();

		final int iR = resolveRestriction(e1.getRestriction(), e2
				.getRestriction());
		if (iR != NONE) {
			final String restriction = getRestrictionString(iR);
			EElement actionE = assignValues(e1, e2, eAction, entries);
			actionE.setRestriction(restriction);

			// comments & author
			final String author = mergeAttributes(e1.getAuthor(), e2.getAuthor());
			actionE.setAuthor(author);
			final String comment = mergeAttributes(e1.getComment(), e2.getComment());
			actionE.setComment(comment);

			actionE.addComments(actionID);
			return actionE;
		} else {
			return null;
		}
	}

	/**
	 * 
	 * @param eAction
	 * @param entries
	 * @return
	 */
	private final EElement assignValues(EElement e1, EElement e2,
			EElement eAction, CrossAction entries) {
		ConstantMap constants = entries.getConstants();
		EElement eCrossed = new EElement();
		PElement pE = new PElement();


		ContentElement lE = (ContentElement) eAction.getSide("L");
		ContentElement rE = (ContentElement) eAction.getSide("R");

		LElement lE2 = new LElement();
		RElement rE2 = new RElement();
		
		assignValuesSide(lE2, lE, constants, e1);
		assignValuesSide(rE2, rE, constants, e2);

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
	private final void assignValuesSide(ContentElement ceWrite, final ContentElement ceRead, final ConstantMap constants, final EElement ei) {
		ceWrite.addChild(new TextElement(ei.getSide("R").getValue()));
		for (Element e : ceRead.getSElements()) {
			if (!constants.containsKey(e.getValue())) {
				ceWrite.addChild(new SElement(e.getValue()));
			} else {
				String value = constants.get(e.getValue());
				ceWrite.addChild(new SElement(value));
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
				String comment = mergeAttributes(e1.getComment(), e2.getComment());
				e.setComment(comment);
			}
		} catch( NullPointerException npe) {

		} catch (Exception exception ) {

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
	 *            the crossWithPatterns to set
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
	 *            the bilAB to set
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
	 *            the bilBC to set
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
	 *            the crossModelFileName to set
	 */
	public final void setCrossModelFileName(final String crossModelFileName) {
		this.crossModelFileName = crossModelFileName;
	}

	/**
	 * @return the ndCrossActions
	 */
	private final void insertNDCrossAction(final CrossAction cA) {
		if (!nDCrossActions.containsKey(cA.getPattern().toString())) {
			this.nDCrossActions.put(cA.getPattern().toString(), cA);
			this.nDCrossModel.addCrossAction(cA);
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
	 *            the crossModelFST to set
	 */
	private final void setCrossModelFST(CrossModelFST crossModelFST) {
		this.crossModelFST = crossModelFST;
	}

}
