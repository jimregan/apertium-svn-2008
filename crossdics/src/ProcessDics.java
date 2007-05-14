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

import dics.elements.dtd.DictionaryElement;
import dics.elements.utils.DicSet;
import dics.elements.utils.DictionaryElementList;
import dics.elements.utils.EElementList;
import dictools.DicConsistent;
import dictools.DicCross;
import dictools.DicFormat;
import dictools.DicMerge;
import dictools.DicReverse;
import dictools.DicTools;
import dictools.DictionaryReader;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class ProcessDics {

    /**
         * 
         */
    private String action;

    /**
         * 
         */
    private String sDicBilAB;

    /**
         * 
         */
    private String sDicBilAB2;

    /**
         * 
         */
    private String sDicBilBC;

    /**
         * 
         */
    private String sDicMonA;

    /**
         * 
         */
    private String sDicMonA2;

    /**
         * 
         */
    private String sDicMonC;

    /**
         * 
         */
    private String sDicMonB;

    /**
         * 
         */
    private String sDicMonB2;

    /**
         * 
         */
    private DicSet dicSet;

    /**
         * 
         */
    private String[] arguments;

    /**
         * 
         */
    private boolean bilABReverse = false;

    /**
         * 
         */
    private boolean bilAB2Reverse = false;

    /**
         * 
         */
    private boolean bilBCReverse = false;

    /**
         * Método principal.
         * 
         * @param args
         */
    public static void main(final String[] args) {
	final ProcessDics ps = new ProcessDics(args);
	ps.go();
    }

    /**
         * 
         * @param args
         */
    public ProcessDics(final String[] args) {
	arguments = args;
    }

    /**
         * 
         * 
         */
    public final void go() {
	checkAction();
    }

    /**
         * 
         * 
         */
    public final void checkAction() {
	if (arguments.length == 0) {
	    System.err.println("Usage: java ProcessDics <action> [options]");
	    System.exit(-1);

	}
	action = arguments[0];

	if (action.equals("consistent")) {
	    if (arguments.length < 8) {
		System.err
			.println("Usage: java ProcessDics consistent -bilAB [-r] <bilAB> -bilBC [-r] <bilBC> -monA <mon-A> -monC <monC>");
		System.exit(-1);
	    } else {
		processArguments(arguments);
		final DicSet dicSet = readDics();
		actionConsistent(dicSet, "yes");
	    }
	}

	if (action.equals("merge")) {
	    if (arguments.length < 8) {
		System.err
			.println("Usage: java ProcessDics merge -bilAB [-r] <bilAB> -bilAB2 [-r] <bilAB2> -monA <mon-A> - monA2 <monA2> -monB <monB> -monB2 <monB2>");
		System.exit(-1);
	    } else {
		processArguments(arguments);
		final DicSet[] dicSet = readDics2();
		final DicSet dicSet1 = dicSet[0];
		final DicSet dicSet2 = dicSet[1];
		actionMerge(dicSet1, dicSet2);
	    }
	}

	if (action.equals("cross")) {
	    if (arguments.length < 8) {
		System.err
			.println("Usage: java ProcessDics cross -bilAB [-r] <bilAB> -bilBC [-r] <bilBC> -monA <mon-A> -monC <monC>");
		System.exit(-1);
	    } else {
		processArguments(arguments);
		final DicSet dicSet = readDics();
		System.gc();
		actionCross(dicSet);
	    }
	}

	if (action.equals("reverse")) {
	    if ((arguments.length > 3) || (arguments.length < 2)) {
		System.err
			.println("Usage: java ProcessDics reverse <bil> <bil-reversed>");
		System.exit(-1);
	    } else {
		DictionaryReader dicReader = new DictionaryReader(arguments[1]);
		DictionaryElement bil = dicReader.readDic();
		dicReader = null;

		final DicReverse dicRev = new DicReverse(bil);
		bil = dicRev.reverse();

		String reverseFileName = "reversed-dic.dix";
		if (arguments.length == 3) {
		    if (arguments[2].equals("out.dix")) {
			reverseFileName = DicTools.reverseDicName(arguments[1]);
		    } else {
			reverseFileName = arguments[2];
		    }
		    bil.printXML(reverseFileName);
		}
	    }
	}

	if (action.equals("format")) {
	    if (arguments.length != 3) {
		System.err
			.println("Usage: java ProcessDics format <dic> <dic-formatted>");
		System.exit(-1);
	    } else {
		DictionaryReader dicReader = new DictionaryReader(arguments[1]);
		final DictionaryElement dic = dicReader.readDic();
		dicReader = null;

		final DicFormat dicFormat = new DicFormat(dic);

		final DictionaryElement dicFormatted = dicFormat.format();

		String formattedFileName = "formatted-dic.dix";
		if (arguments.length == 3) {
		    if (arguments[2].equals("out.dix")) {
			formattedFileName = DicTools
				.removeExtension(arguments[2]);
			formattedFileName = formattedFileName
				+ "-formatted.dix";
		    } else {
			formattedFileName = arguments[2];
		    }
		    dicFormatted.printXML(formattedFileName);
		}
	    }
	}
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
	// DicTools.printLogTitle("SIZE OF CONSISTENT DICTIONARIES");
	// dicSet.reportMetrics();
	dicSet.printXML("consistent");
	return dicConsistent;
    }

    /**
         * 
         * @param dicSet1
         * @param dicSet2
         */
    private final void actionMerge(final DicSet dicSet1, final DicSet dicSet2) {
	final DicMerge dm = new DicMerge(dicSet1, dicSet2);
	final DicSet dicSet = dm.merge();

	final DictionaryElement bil = dicSet.getBil1();
	final DictionaryElement monA = dicSet.getMon1();
	final DictionaryElement monB = dicSet.getMon2();

	bil.printXML(bil.getFileName());
	monA.printXML(monA.getFileName());
	monB.printXML(monB.getFileName());

    }

    /**
         * 
         * @param dicSet
         */
    private final void actionCross(final DicSet dicSet) {
	actionConsistent(dicSet, "yes");

	final DicCross dc = new DicCross();
	final DictionaryElement[] bils = dc.crossDictionaries(dicSet);

	final DictionaryElement bilCrossed = bils[0];
	final DictionaryElement bilSpecul = bils[1];

	// DicTools.printLogTitle("SIZE OF DICTIONARIES AFTER CROSSING");
	// dicSet.reportMetrics();

	final String sl = dicSet.getBil1().getTL();
	final String tl = dicSet.getBil2().getTL();
	bilCrossed.setType("BIL");
	bilCrossed.setSL(sl);
	bilCrossed.setTL(tl);

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

	// DicTools.printLogTitle("SIZE OF CROSSED BILINGUAL DICTIONARY");
	// bilCrossed.reportMetrics();

    }

    /**
         * 
         * @param del
         * @param sl
         * @param tl
         */
    public final void printXMLCrossedAndSpecul(final DictionaryElementList del,
	    final String sl, final String tl) {
	final DictionaryElement bilCrossed = del.get(0);
	final DictionaryElement bilSpecul = del.get(1);
	final DictionaryElement monACrossed = del.get(2);
	final DictionaryElement monBCrossed = del.get(3);
	final DictionaryElement monASpecul = del.get(4);
	final DictionaryElement monBSpecul = del.get(5);

	bilCrossed.printXML("dix/apertium-" + sl + "-" + tl + "." + sl + "-"
		+ tl + "-crossed.dix");

	bilSpecul.printXML("dix/apertium-" + sl + "-" + tl + "." + sl + "-"
		+ tl + "-crossed-specul.dix");

	monACrossed.printXML("dix/apertium-" + sl + "-" + tl + "." + sl
		+ "-crossed.dix");
	monBCrossed.printXML("dix/apertium-" + sl + "-" + tl + "." + tl
		+ "-crossed.dix");

	monASpecul.printXML("dix/apertium-" + sl + "-" + tl + "." + sl
		+ "-crossed-specul.dix");
	monBSpecul.printXML("dix/apertium-" + sl + "-" + tl + "." + tl
		+ "-crossed-specul.dix");
    }

    /**
         * 
         * @return
         */
    private DicSet readDics() {
	final DictionaryElement bil1 = readBilingual(sDicBilAB, bilABReverse);
	final DictionaryElement bil2 = readBilingual(sDicBilBC, bilBCReverse);

	final DictionaryElement mon1 = readMonolingual(sDicMonA);
	final DictionaryElement mon2 = readMonolingual(sDicMonC);

	dicSet = new DicSet(mon1, bil1, mon2, bil2);
	return dicSet;
    }

    /**
         * 
         * @return
         */
    private DicSet[] readDics2() {
	final DicSet[] dicSets = new DicSet[2];

	final DictionaryElement bilAB1 = readBilingual(sDicBilAB, bilABReverse);
	final DictionaryElement monA1 = readMonolingual(sDicMonA);
	final DictionaryElement monB1 = readMonolingual(sDicMonB);
	final DicSet dicSet1 = new DicSet(bilAB1, monA1, monB1);
	dicSets[0] = dicSet1;

	final DictionaryElement bilAB2 = readBilingual(sDicBilAB2,
		bilAB2Reverse);
	final DictionaryElement monA2 = readMonolingual(sDicMonA2);
	final DictionaryElement monB2 = readMonolingual(sDicMonB2);
	final DicSet dicSet2 = new DicSet(bilAB2, monA2, monB2);
	dicSets[1] = dicSet2;

	return dicSets;
    }

    /**
         * 
         * @param sMon
         * @return
         */
    private DictionaryElement readMonolingual(final String sMon) {
	DictionaryReader dicReader = new DictionaryReader(sMon);
	final DictionaryElement mon = dicReader.readDic();
	mon.setFileName(sMon);
	dicReader = null;
	return mon;
    }

    /**
         * 
         * @param sBil
         * @param reverse
         * @return
         */
    private DictionaryElement readBilingual(final String sBil,
	    final boolean reverse) {
	DictionaryReader dicReaderBil = new DictionaryReader(sBil);
	final DictionaryElement bil = dicReaderBil.readDic();
	bil.setFileName(sBil);
	bil.setType("BIL");

	if (reverse) {
	    bil.reverse();
	    final String reverseFileName = DicTools.reverseDicName(sBil);
	    bil.printXML(reverseFileName);
	    bil.setFileName(reverseFileName);
	}

	final String[] st = DicTools.getSourceAndTarget(bil.getFileName());
	bil.setSL(st[0]);
	bil.setTL(st[1]);
	dicReaderBil = null;
	return bil;
    }

    /**
         * 
         * @param arguments
         */
    private void processArguments(final String[] arguments) {
	final int nArgs = arguments.length;

	for (int i = 1; i < nArgs; i++) {
	    String arg = arguments[i];
	    if (arg.equals("-monA")) {
		i++;
		arg = arguments[i];
		sDicMonA = arg;
		System.err.println("Monolingual A: '" + sDicMonA + "'");
	    }

	    if (arg.equals("-monA2")) {
		i++;
		arg = arguments[i];
		sDicMonA2 = arg;
		System.err.println("Monolingual A (2): '" + sDicMonA2 + "'");
	    }

	    if (arg.equals("-monB")) {
		i++;
		arg = arguments[i];
		sDicMonB = arg;
		System.err.println("Monolingual B: '" + sDicMonB + "'");
	    }

	    if (arg.equals("-monB2")) {
		i++;
		arg = arguments[i];
		sDicMonB2 = arg;
		System.err.println("Monolingual B2: '" + sDicMonB2 + "'");
	    }

	    if (arg.equals("-monC")) {
		i++;
		arg = arguments[i];
		sDicMonC = arg;
		System.err.println("Monolingual C: '" + sDicMonC + "'");
	    }

	    if (arg.equals("-bilAB")) {
		i++;
		arg = arguments[i];
		if (arg.equals("-r")) {
		    bilABReverse = true;
		    i++;
		}
		if (arg.equals("-n")) {
		    bilABReverse = false;
		    i++;
		}

		arg = arguments[i];
		sDicBilAB = arg;
		System.err.println("Bilingual A-B: '" + sDicBilAB + "'");
	    }

	    if (arg.equals("-bilAB2")) {
		i++;
		arg = arguments[i];
		if (arg.equals("-r")) {
		    bilAB2Reverse = true;
		    i++;
		}
		if (arg.equals("-n")) {
		    bilAB2Reverse = false;
		    i++;
		}
		arg = arguments[i];
		sDicBilAB2 = arg;
		System.err.println("Bilingual A-B-(2): '" + sDicBilAB2 + "'");
	    }

	    if (arg.equals("-bilBC")) {
		i++;
		arg = arguments[i];

		if (arg.equals("-r")) {
		    bilBCReverse = true;
		    i++;
		}
		if (arg.equals("-n")) {
		    bilBCReverse = false;
		    i++;
		}
		arg = arguments[i];
		sDicBilBC = arg;
		System.err.println("Bilingual B-C: '" + sDicBilBC + "'");
	    }

	}
    }

}
