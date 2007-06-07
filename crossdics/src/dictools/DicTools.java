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

import dics.elements.dtd.DictionaryElement;
import dics.elements.dtd.EElement;
import dics.elements.utils.DicSet;
import dics.elements.utils.EElementList;
import dics.elements.utils.EElementMap;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class DicTools {

    /**
         * 
         * @param value
         * @return
         */
    public static String clearTags(String value) {
	if (value != null) {
	    value = value.replaceAll("<g>", "");
	    value = value.replaceAll("<j/>", "");
	    value = value.replaceAll("<a/>", "");
	    value = value.replaceAll("</g>", "");
	    value = value.replaceAll("<b/>", " ");
	}
	return value;
    }

    /**
         * 
         * @param entries
         * @return
         */
    public static final EElementMap buildHash(final ArrayList<EElement> entries) {
	final EElementMap entriesMap = new EElementMap();
	for (final EElement e : entries) {
	    final String value = e.getValue("L");
	    final String key = DicTools.clearTags(value);
	    if (entriesMap.containsKey(key)) {
		final EElementList eList = entriesMap.get(key);
		eList.add(e);
		entriesMap.put(key, eList);
	    } else {
		final EElementList eList = new EElementList();
		eList.add(e);
		entriesMap.put(key, eList);
	    }
	}
	return entriesMap;
    }

    /**
         * 
         * @param entries
         * @param side
         * @return
         */
    public static final EElementMap buildHash(
	    final ArrayList<EElement> entries, final String side) {
	final EElementMap entriesMap = new EElementMap();
	for (final EElement e : entries) {
	    final String value = e.getValue(side);
	    final String key = DicTools.clearTags(value);

	    if (entriesMap.containsKey(key)) {
		final EElementList eList = entriesMap.get(key);
		eList.add(e);
		entriesMap.put(key, eList);
	    } else {
		final EElementList eList = new EElementList();
		eList.add(e);
		entriesMap.put(key, eList);
	    }
	}
	return entriesMap;
    }

    /**
         * 
         * @param entries
         * @param restriction
         * @return
         */
    public static final EElementMap buildHashMon(
	    final ArrayList<EElement> entries) {
	final EElementMap entriesMap = new EElementMap();

	for (final EElement e : entries) {
	    final String lemma = e.getLemma();
	    final String key = DicTools.clearTags(lemma);
	    if (entriesMap.containsKey(key)) {
		final EElementList eList = entriesMap.get(key);
		eList.add(e);
		entriesMap.put(key, eList);
	    } else {
		final EElementList eList = new EElementList();
		eList.add(e);
		entriesMap.put(key, eList);
	    }
	}
	return entriesMap;
    }

    /**
         * 
         * @param name
         * @return
         */
    public static String reverseDicName(final String name) {
	final String[] st = DicTools.getSourceAndTarget(name);
	final String newFileName = st[2] + "apertium-" + st[1] + "-" + st[0]
		+ "." + st[1] + "-" + st[0] + ".dix";
	return newFileName;
    }

    /**
         * 
         * @param name
         * @return
         */
    public static String[] getSourceAndTarget(final String name) {
	final String[] st = new String[3];
	final String langCode = "[a-z][a-z]+";
	final String langPair = langCode + "-" + langCode;
	String str = "";

	final String[] parts = name.split("apertium-");
	for (final String element : parts) {
	    str = element;
	}

	final String[] paths = name.split("apertium-" + langPair + "."
		+ langPair + ".dix");
	st[2] = paths[0];
	int k = 0;
	final String[] parts2 = str.split("[.]");
	for (int i = 0; i < (parts2.length - 1); i++) {
	    k = 0;
	    str = parts2[i];
	    final String[] parts3 = str.split("-");
	    for (final String element : parts3) {
		str = parts[i];
		if (k == 0) {
		    st[0] = element;
		}
		if (k == 1) {
		    st[1] = element;
		}
		k++;
	    }
	}
	return st;
    }

    /**
         * 
         * @param str
         * @param ext
         * @return
         */
    public static final String removeExtension(final String str) {
	String head = str.replaceAll("\\.dix", "");
	head = head.replaceAll("\\.metadix", "");
	head = head.replaceAll("/dics/", "/dix/");
	return head;
    }

    /**
         * 
         * @param bilAB
         * @param monA
         * @param monB
         */
    public static EElementList[] makeConsistent(final DictionaryElement bilAB,
	    final DictionaryElement monA, final DictionaryElement monB) {
	final EElementList[] consistentMons = new EElementList[2];
	final ArrayList<EElement> elements = bilAB.getEntries();

	final EElementMap bilABMapL = DicTools.buildHash(elements, "L");
	final EElementMap bilABMapR = DicTools.buildHash(elements, "R");

	final EElementMap monAMap = DicTools.buildHashMon(monA.getEntries());
	final EElementMap monBMap = DicTools.buildHashMon(monB.getEntries());

	final EElementList monAConsistent = DicTools.makeConsistent(bilABMapL,
		"L", monAMap);

	final EElementList monBConsistent = DicTools.makeConsistent(bilABMapR,
		"R", monBMap);

	Collections.sort(monAConsistent);
	consistentMons[0] = monAConsistent;
	Collections.sort(monBConsistent);
	consistentMons[1] = monBConsistent;

	return consistentMons;
    }

    /**
         * 
         * @param bilABMap
         * @param side
         * @param monMap
         */
    private static EElementList makeConsistent(final EElementMap bilABMap,
	    final String side, final EElementMap monMap) {
	final EElementList consistentMon = new EElementList();
	final Set<String> keySet = monMap.keySet();
	final Iterator<String> it = keySet.iterator();

	while (it.hasNext()) {
	    final String key = it.next();
	    final EElementList eList = monMap.get(key);
	    for (final EElement e : eList) {
		String lemma = e.getLemma();
		lemma = DicTools.clearTags(lemma);

		if (bilABMap.containsKey(lemma)) {
		    consistentMon.add(e);
		}
	    }
	}
	return consistentMon;
    }

    /**
         * 
         * @param bilAB
         * @param monA
         * @param monC
         * @return
         */
    public static final DicSet makeConsistentBilAndMonols(
	    final DictionaryElement bilAB, final DictionaryElement monA,
	    final DictionaryElement monC) {
	final EElementList[] commonCrossedMons = DicTools.makeConsistent(bilAB,
		monA, monC);
	final EElementList crossedMonA = commonCrossedMons[0];
	final EElementList crossedMonB = commonCrossedMons[1];

	final DictionaryElement monACrossed = new DictionaryElement(monA);
	monACrossed.setMainSection(crossedMonA);

	final DictionaryElement monBCrossed = new DictionaryElement(monC);
	monBCrossed.setMainSection(crossedMonB);

	final DicSet dicSet = new DicSet(bilAB, monACrossed, monBCrossed);
	return dicSet;
    }

    /**
         * 
         * @return
         */
    public static HashMap<String, String> getSdefDescriptions() {
	final HashMap<String, String> d = new HashMap<String, String>();

	d.put("aa", "adjective-adjective");
	d.put("acr", "acronym");
	d.put("adj", "adjective");
	d.put("al", "others");
	d.put("an", "adjective-noun");
	d.put("ant", "(antropónimo)");
	d.put("adv", "adverb");
	d.put("atn", "?????");
	d.put("cm", "?????");
	d.put("cni", "conditional");
	d.put("cnjadv", "adverbial conjunction");
	d.put("cnjcoo", "coordinative conjunction");
	d.put("cnjsub", "subordinate conjunction");
	d.put("def", "definite");
	d.put("dem", "demostrative");
	d.put("det", "determiner");
	d.put("detnt", "neutral determiner");
	d.put("enc", "(enclítico)");
	d.put("f", "femenin");
	d.put("fti", "future (futuro de indicativo))");
	d.put("fts", "future (subjunctive) (futuro de subjuntivo)");
	d.put("ger", "gerund");
	d.put("ifi", "past (pretérito perfecto o indefinido)");
	d.put("ij", "interjection");
	d.put("imp", "imperative");
	d.put("ind", "indefinite");
	d.put("inf", "infinitive");
	d.put("itg", "(interrogativo)");
	d.put("loc", "(locativo)");
	d.put("lpar", "([");
	d.put("lquest", "¿");
	d.put("m", "masculine");
	d.put("mf", "masculine-femenin");
	d.put("n", "noun");
	d.put("nn", "noun-noun");
	d.put("np", "proper noun");
	d.put("nt", "neuter");
	d.put("num", "numeral");
	d.put("p1", "1p");
	d.put("p2", "2p");
	d.put("p3", "3p");
	d.put("pii", "(pretérito imperfecto de indicativo)");
	d.put("pis", "(pretérito imperfecto de subjuntivo)");
	d.put("pl", "plural");
	d.put("pos", "possessive");
	d.put("pp", "participle");
	d.put("pr", "preposition");
	d.put("preadv", "preadverb");
	d.put("predet", "predeterminer");
	d.put("pri", "present simple (presente de indicativo)");
	d.put("prn", "pronoun");
	d.put("pro", "(proclítico)");
	d.put("prs", "(presente de subjuntivo)");
	d.put("ref", "reflexive");
	d.put("rel", "relative");
	d.put("rpar", ")]");
	d.put("sent", ".?;:!");
	d.put("sg", "singular");
	d.put("sp", "singular-plural");
	d.put("sup", "superlative");
	d.put("tn", "tonic");
	d.put("vaux", "auxiliar verb");
	d.put("vbhaver", "'haver' verb");
	d.put("vblex", "lexical verb");
	d.put("vbmod", "modal verb");
	d.put("vbser", "'ser' verb");
	return d;
    }

}
