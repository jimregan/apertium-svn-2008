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
import dics.elements.utils.ElementList;
import dics.elements.utils.SElementList;
import dictools.crossmodel.Action;
import dictools.crossmodel.ConstantMap;
import dictools.crossmodel.CrossAction;
import dictools.crossmodel.CrossActionList;
import dictools.crossmodel.CrossModel;
import dictools.crossmodel.CrossModelReader;
import dictools.crossmodel.CrossModelFST;
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
	private HashMap<String,CrossAction> nDCrossActions;

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
		nDCrossActions = new HashMap<String,CrossAction>();
		nDCrossModel = new CrossModel();
	}

	/**
	 * 
	 * 
	 */
	private final void readCrossModel() {
		try {
			final CrossModelReader cmr = new CrossModelReader(getCrossModelFileName());
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

		setRMatrixValue(RL, LR , LR);
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
		//sdefs.addComments("edit method 'getSdefDescriptions()' in class 'dictools.DicCross' in case you want to change any description");

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

		final EElementList elements1 = section1.getEElements();
		final EElementList elements2 = section2.getEElements();

		EElementMap section1Map = DicTools.buildHash(elements1);
		EElementMap section2Map = DicTools.buildHash(elements2);

		crossSectionsAB(elements1, section2Map, section, speculSection, getBilBC(),
				0);
		section2Map = null;

		crossSectionsAB(elements2, section1Map, section, speculSection, getBilAB(),
				1);
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
	public void crossSectionsAB(final EElementList elements,
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
					crossElementAndPairsWithPatterns(e, candidates, section,
							speculSection, bil, dir);
				} else {
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
	 * @param speculSection
	 * @param bil
	 * @param dir
	 */
	public final void crossElementAndPairs(final EElement e1,
			final EElementList candidates, final SectionElement section,
			final SectionElement speculSection, final DictionaryElement bil,
			final int dir) {
		if (candidates != null) {
			for (final EElement e2 : candidates) {

				if ((e1 != null) && (e2 != null)) {
					EElement e = crossSafely(e1, e2, dir, MATCH_CATEGORY,
					"SAFE");
					if (e != null) {
						final String str = e.getHash();
						if (!getProcessed().containsKey(str)) {
							section.addEElement(e);
							getProcessed().put(str, e);
						}
					} else {
						e = crossSafely(e1, e2, dir, DO_NOT_MATCH_CATEGORY,
						"SPECULATION");
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
	 * @param candidates
	 * @param section
	 * @param speculProcessed
	 * @param bil
	 * @param dir
	 */
	public final void crossElementAndPairsWithPatterns(final EElement e1,
			final EElementList candidates, final SectionElement section,
			final SectionElement speculSection, final DictionaryElement bil,
			final int dir) {
		if (candidates != null) {
			for (final EElement e2 : candidates) {
				if ((e1 != null) && (e2 != null)) {
					EElement e = crossWithPatterns(e1, e2, dir);
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
	public final EElement speculate(final EElement a, final EElement b, boolean matchCategory, int dir) {
		EElement e1,e2;
		EElement e = null;

		if (dir == 0) {
			e1 = a;
			e2 = b;
		} else {
			e2 = a;
			e1 = b;
		}

		if (matchCategory) {
			if (e1.getCategory("L").equals(e2.getCategory("L"))) {
				e = crossEntries(e1, e2);	
			} } else {
				e = crossEntries(e1, e2);
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
	private final EElement crossWithPatterns(EElement a, EElement b,
			final int dir) {
		EElement e1,e2,e1Rev;

		if (dir == 0) {
			e1 = (EElement)a.clone();
			e2 = (EElement)b.clone();
		} else {
			e1 = (EElement)b.clone();
			e2 = (EElement)a.clone();
		}
			CrossAction crossAction = new CrossAction();
			e1Rev = e1.reverse();
			Pattern entriesPattern = new Pattern(e1Rev,e2);
			crossAction.setPattern(entriesPattern);
			Action actionFST = getCrossModelFST().getAction(crossAction);
			
			//System.out.println("Pattern " +  actionFST.getName() + " detected.");
			//final String actionID = getCrossModel().matches(crossAction);

			if (actionFST != null) {
				final String actionID = actionFST.getName();
				EElement actionE = applyCrossAction(e1, e2, actionID, crossAction, dir);
				return actionE;
			} else {
				insertNDCrossAction(crossAction);
				//System.err.println("No pattern!");
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
	private final EElement applyCrossAction(EElement e1, EElement e2, final String actionID, final CrossAction entries, int dir) {

		final CrossAction cA = getCrossModel().getCrossAction(actionID);
		Action action = cA.getAction();

		EElement eAction = action.getE();

		// restrictions
		final String restriction;
		final int iR = resolveRestriction(e1.getRestriction(), e2.getRestriction());
		if (iR != NONE) {
			restriction = getRestrictionString(iR);
		} else {
			//System.err.println("No crossing is possible!");
			return null;
		}

		EElement actionE = assignValues(e1, e2, eAction, entries, dir);
		actionE.setRestriction(restriction);
		
		
		// comments & author
		final String author = mergeAttributes(e1.getAuthor(), e2.getAuthor());
		actionE.setAuthor(author);
		final String comment = mergeAttributes(e1.getComment(), e2.getComment());
		actionE.setComment(comment);

		actionE.addComments(actionID);


		return actionE;
	}

	/**
	 * 
	 * @param eAction
	 * @param entries
	 * @return
	 */
	private final EElement assignValues(EElement e1, EElement e2, EElement eAction, CrossAction entries, int dir) {
		ConstantMap constants = entries.getConstants();
		EElement eCrossed = new EElement();
		ContentElement lE = (ContentElement)eAction.getSide("L").clone();
		ContentElement rE = (ContentElement)eAction.getSide("R").clone();

		LElement lE2 = new LElement();
		lE2.addChild(new TextElement(e1.getSide("R").getValue()));
		for (Element e : lE.getSElements()) {
			if (!constants.containsKey(e.getValue())) {
				lE2.addChild(new SElement(e.getValue()));
			} else {
				String value = constants.get(e.getValue());
				lE2.addChild(new SElement(value));
			}

		}

		RElement rE2 = new RElement();
		rE2.addChild(new TextElement(e2.getSide("R").getValue()));
		for (Element e : rE.getSElements()) {
			if (!constants.containsKey(e.getValue())) {
				rE2.addChild(new SElement(e.getValue()));
			} else {
				String value = constants.get(e.getValue());
				rE2.addChild(new SElement(value));
			}
		}

		PElement pE = new PElement();
		pE.setLElement(lE2);
		pE.setRElement(rE2);

		eCrossed.addChild(pE);

		return eCrossed;


	}

	/**
	 * 
	 * @param a
	 * @param b
	 * @param dir
	 * @param sameCategory
	 * @return
	 */
	private final EElement crossSafely(EElement a, EElement b,
			final int dir, final boolean sameCategory, final String type) {
		EElement e1,e2;

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
			final String cat1L = e1.getCategory("L");
			final String cat1R = e1.getCategory("R");
			final String cat2L = e2.getCategory("L");
			final String cat2R = e2.getCategory("R");
			try {
				if (!sameCategory
						|| compare4Strings(cat1L, cat1R, cat2L, cat2R)) {

					final SElementList sE1L = e1.getSide("L").getSElements();
					final SElementList sE1R = e1.getSide("R").getSElements();
					final SElementList sE2L = e2.getSide("L").getSElements();
					final SElementList sE2R = e2.getSide("R").getSElements();
					// We get x, T, U, V, W and X
					final ArrayList<Object> vars = getSubstrings(sE1L, sE1R,
							sE2L, sE2R, sameCategory);

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
		// System.err.println("\nCASE " + thecase);
		String r1 = e1.getRestriction();
		final int iR1 = getRestrictionCode(r1);
		r1 = getRestrictionString2(iR1);
		r1 = reverseRestriction(r1);
		String r2 = e2.getRestriction();
		final int iR2 = getRestrictionCode(r2);
		r2 = getRestrictionString2(iR2);

		/*
		 * System.err.println(""); System.err.println("Crossing: " + r1 + " x " +
		 * r2); System.err.print("A: "); e1.getSide("R").print();
		 * System.err.print("B: "); e1.getSide("L").print();
		 * System.err.print("B: "); e2.getSide("L").print();
		 * System.err.print("C: "); e2.getSide("R").print();
		 * System.err.println("------------------------");
		 */
		if (e != null) {
			String r = e.getRestriction();
			final int iR = getRestrictionCode(r);
			r = getRestrictionString2(iR);
			/*
			 * System.err.println("Result: " + r); System.err.print("A: ");
			 * e.getSide("L").print(); System.err.print("C: ");
			 * e.getSide("R").print();
			 */
		} else {
			// System.err.println(r1 + " x " + r2 + " --> X");
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
		//e.addComments("CROSSING CASE 1");
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
		//e.addComments("CROSSING CASE 2");
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
		//e.addComments("CROSSING CASE 3");
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
		//e.addComments("CROSSING CASE 4");
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
		//e.addComments("CROSSING CASE 5");
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
		final SElementList sEList = new SElementList();
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
	public final EElementList getPairs(final EElement e, final EElementMap hm) {
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

			PElement pE = new PElement();
			LElement lE = new LElement();
			pE.setLElement(lE);
			RElement rE = new RElement();
			pE.setRElement(rE);
			e.addChild(pE);
			e.getP().getL().setValue(e1.getValue("R"));
			e.getP().getL().setChildren(e1.getChildren("R"));

			String author = mergeAttributes(e1.getAuthor(), e2
					.getAuthor());
			e.setAuthor(author);
			String comment = mergeAttributes(e1.getComment(), e2
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

			//e.addComments("A-C = " + restriction + " ::: A-B = " + r1 + " X B-C = " + r2);

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

	/**
	 * 
	 * @return
	 */
	public final CrossModel getCrossModel() {
		return crossModel;
	}

	/**
	 * 
	 * @param crossModel
	 */
	public final void setCrossModel(final CrossModel crossModel) {
		this.crossModel = crossModel;
	}

	/**
	 * @return the crossWithPatterns
	 */
	public final boolean isCrossWithPatterns() {
		return crossWithPatterns;
	}

	/**
	 * @param crossWithPatterns the crossWithPatterns to set
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
	 * @param bilAB the bilAB to set
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
	 * @param bilBC the bilBC to set
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
	public final String getCrossModelFileName() {
		return crossModelFileName;
	}

	/**
	 * @param crossModelFileName the crossModelFileName to set
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
	 * 
	 *
	 */
	private final void printNDCrossActions() {
		Set keySet = this.nDCrossActions.keySet();
		Iterator it = keySet.iterator();
		int i = 0;
		while (it.hasNext()) {
			it.next();
			i++;
		}
		System.out.println("Non-defined cross actions: " + i );
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
	public final CrossModelFST getCrossModelFST() {
		return crossModelFST;
	}

	/**
	 * @param crossModelFST the crossModelFST to set
	 */
	public final void setCrossModelFST(CrossModelFST crossModelFST) {
		this.crossModelFST = crossModelFST;
	}

}
