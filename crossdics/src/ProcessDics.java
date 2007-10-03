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

import misc.DicFormatE1Line;
import misc.GetTranslation;
import dics.elements.utils.Msg;
import dictools.DicConsistent;
import dictools.DicCross;
import dictools.DicFindEquivPar;
import dictools.DicFormat;
import dictools.DicGather;
import dictools.DicMerge;
import dictools.DicReader;
import dictools.DicReverse;
import dictools.DicSort;

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
    private String[] arguments;

    /**
         * 
         */
    private Msg msg;

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
	msg = new Msg();
	setArguments(args);
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
	if (getArguments().length == 0) {
	    msg.err("Usage: java ProcessDics <action> [options]");
	    System.exit(-1);

	}
	setAction(getArguments()[0]);

	if (getAction().equals("consistent")) {
	    if (getArguments().length < 8) {
		msg
			.err("Usage: java ProcessDics consistent -bilAB [-r] <bilAB> -bilBC [-r] <bilBC> -monA <mon-A> -monC <monC>");
		System.exit(-1);
	    } else {
		DicConsistent dicConsistent = new DicConsistent();
		dicConsistent.setArguments(getArguments());
		dicConsistent.doConsistent();
	    }
	}

	if (getAction().equals("merge")) {
	    if (getArguments().length < 8) {
		msg
			.err("Usage: java ProcessDics merge -bilAB [-r] <bilAB> -bilAB2 [-r] <bilAB2> -monA <mon-A> - monA2 <monA2> -monB <monB> -monB2 <monB2>");
		System.exit(-1);
	    } else {
		DicMerge dicMerge = new DicMerge();
		dicMerge.setArguments(getArguments());
		dicMerge.doMerge();
	    }
	}

	if (getAction().equals("merge-morph")) {
	    if (getArguments().length < 6) {
		msg
			.err("Usage: java ProcessDics merge-morph -monA1 monA1.dix -monA2 monA2.dix -out merged.dix");
		System.exit(-1);
	    } else {
		DicMerge dicMerge = new DicMerge();
		dicMerge.setArguments(getArguments());
		dicMerge.doMergeMorph();
	    }
	}

	if (getAction().equals("merge-bil")) {
	    if (getArguments().length < 6) {
		msg
			.err("Usage: java ProcessDics merge-bil -bilAB1 bilAb1.dix -bilAB2 bilAB2.dix -out merged.dix");
		System.exit(-1);
	    } else {
		DicMerge dicMerge = new DicMerge();
		dicMerge.setArguments(getArguments());
		dicMerge.doMergeBil();
	    }
	}

	if (getAction().equals("cross")) {
	    if (getArguments().length < 8) {
		msg
			.err("Usage: java ProcessDics cross -bilAB [-r] <bilAB> -bilBC [-r] <bilBC> -monA <mon-A> -monC <monC>");
		System.exit(-1);
	    } else {
		DicCross dicCross = new DicCross();
		dicCross.setArguments(getArguments());
		dicCross.doCross();
	    }
	}

	if (getAction().equals("reverse")) {
	    if ((getArguments().length > 3) || (getArguments().length < 2)) {
		msg.err("Usage: java ProcessDics reverse <bil> <bil-reversed>");
		System.exit(-1);
	    } else {
		DicReverse dicReverse = new DicReverse();
		dicReverse.setArguments(arguments);
		dicReverse.doReverse();
	    }
	}

	if (getAction().equals("format")) {
	    if (getArguments().length != 4) {
		msg
			.err("Usage: java ProcessDics format <-mon|-bil> <dic> <dic-formatted>");
		System.exit(-1);
	    } else {
		final DicFormat dicFormat = new DicFormat();
		dicFormat.setArguments(arguments);
		dicFormat.doFormat();
	    }
	}

	if (getAction().equals("sort")) {
	    if (getArguments().length != 5) {
		msg
			.err("Usage: java ProcessDics sort <-mon|-bil> <-xinclude|-same-file> <dic> <dic-sorted>");
		System.exit(-1);
	    } else {
		DicSort dicSort = new DicSort();
		dicSort.setArguments(arguments);
		dicSort.doSort();
	    }
	}

	if (getAction().equals("gather")) {
	    if (getArguments().length != 3) {
		msg.err("Usage: java ProcessDics gather <dic> <dic-sorted>");
		System.exit(-1);
	    } else {
		DicGather dicGather = new DicGather(arguments[1], arguments[2]);
		// dicGather.setArguments(arguments);
		dicGather.doGather();
	    }
	}

	if (getAction().equals("get-bil-omegawiki")) {
	    if (getArguments().length != 4) {
		msg
			.err("Usage: java ProcessDics get-bil-omegawiki <source-lang> <target-lang> <dic-out>");
		System.exit(-1);
	    } else {
		GetTranslation gt = new GetTranslation(arguments[1],
			arguments[2]);
		gt.setOutFileName(arguments[3]);
		gt.printDictionary();
	    }
	}

	if (getAction().equals("format-1line")) {
	    if (getArguments().length != 3) {
		msg.err("Usage: java ProcessDics format-1line <dic> <dic-out>");
		System.exit(-1);
	    } else {
		DicFormatE1Line dicFormat = new DicFormatE1Line(arguments[1]);
		dicFormat.printXML(arguments[2]);
	    }
	}

	if (getAction().equals("dic-reader")) {
	    if (getArguments().length < 3) {
		msg.err("Usage: java ProcessDics dic-reader <action> [-url] <dic>");
		System.exit(-1);
	    } else {
		DicReader dicReader = new DicReader();
		dicReader.setAction(arguments[1]);
		if (arguments[2].equals("-url")) {
		    dicReader.setUrlDic(true);
		    dicReader.setUrl(arguments[3]);
		    System.out.println("URL: " + arguments[3]);
		} else {
		    dicReader.setDic(arguments[2]);
		}
		dicReader.doit();
	    }
	}

	if (getAction().equals("equiv-paradigms")) {
	    if (getArguments().length != 3) {
		msg.err("Usage: java ProcessDics equiv-paradigms <dic> <dic-out>");
		System.exit(-1);
	    } else {
		DicFindEquivPar finder = new DicFindEquivPar(arguments[1]);
		finder.setOutFileName(arguments[2]);
		finder.findEquivalents();
	    }
	}

    }

    /**
         * @return the action
         */
    private final String getAction() {
	return action;
    }

    /**
         * @param action
         *                the action to set
         */
    private final void setAction(final String action) {
	this.action = action;
    }

    /**
         * @return the arguments
         */
    private final String[] getArguments() {
	return arguments;
    }

    /**
         * @param arguments
         *                the arguments to set
         */
    private final void setArguments(final String[] arguments) {
	this.arguments = arguments;
    }

}
