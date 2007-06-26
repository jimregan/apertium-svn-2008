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

import misc.AddGender;
import misc.AssignParadigm;
import dictools.DicConsistent;
import dictools.DicCross;
import dictools.DicFormat;
import dictools.DicGather;
import dictools.DicMerge;
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
	    System.err.println("Usage: java ProcessDics <action> [options]");
	    System.exit(-1);

	}
	setAction(getArguments()[0]);

	if (getAction().equals("consistent")) {
	    if (getArguments().length < 8) {
		System.err
			.println("Usage: java ProcessDics consistent -bilAB [-r] <bilAB> -bilBC [-r] <bilBC> -monA <mon-A> -monC <monC>");
		System.exit(-1);
	    } else {
		DicConsistent dicConsistent = new DicConsistent();
		dicConsistent.setArguments(getArguments());
		dicConsistent.doConsistent();
	    }
	}

	if (getAction().equals("merge")) {
	    if (getArguments().length < 8) {
		System.err
			.println("Usage: java ProcessDics merge -bilAB [-r] <bilAB> -bilAB2 [-r] <bilAB2> -monA <mon-A> - monA2 <monA2> -monB <monB> -monB2 <monB2>");
		System.exit(-1);
	    } else {
		DicMerge dicMerge = new DicMerge();
		dicMerge.setArguments(getArguments());
		dicMerge.doMerge();
	    }
	}

	if (getAction().equals("merge-morph")) {
	    if (getArguments().length < 6) {
		System.err
			.println("Usage: java ProcessDics merge-morph -monA1 monA1.dix -monA2 monA2.dix -out merged.dix");
		System.exit(-1);
	    } else {
		DicMerge dicMerge = new DicMerge();
		dicMerge.setArguments(getArguments());
		dicMerge.doMergeMorph();
	    }
	}

	if (getAction().equals("merge-bil")) {
	    if (getArguments().length < 6) {
		System.err
			.println("Usage: java ProcessDics merge-morph -bilAB1 bilAb1.dix -bilAB2 bilAB2.dix -out merged.dix");
		System.exit(-1);
	    } else {
		DicMerge dicMerge = new DicMerge();
		dicMerge.setArguments(getArguments());
		dicMerge.doMergeBil();
	    }
	}

	if (getAction().equals("cross")) {
	    if (getArguments().length < 8) {
		System.err
			.println("Usage: java ProcessDics cross -bilAB [-r] <bilAB> -bilBC [-r] <bilBC> -monA <mon-A> -monC <monC>");
		System.exit(-1);
	    } else {
		DicCross dicCross = new DicCross();
		dicCross.setArguments(getArguments());
		dicCross.doCross();
	    }
	}

	if (getAction().equals("reverse")) {
	    if ((getArguments().length > 3) || (getArguments().length < 2)) {
		System.err
			.println("Usage: java ProcessDics reverse <bil> <bil-reversed>");
		System.exit(-1);
	    } else {
		DicReverse dicReverse = new DicReverse();
		dicReverse.setArguments(arguments);
		dicReverse.doReverse();
	    }
	}

	if (getAction().equals("format")) {
	    if (getArguments().length != 4) {
		System.err
			.println("Usage: java ProcessDics format <-mon|-bil> <dic> <dic-formatted>");
		System.exit(-1);
	    } else {
		final DicFormat dicFormat = new DicFormat();
		dicFormat.setArguments(arguments);
		dicFormat.doFormat();
	    }
	}

	if (getAction().equals("sort")) {
	    if (getArguments().length != 5) {
		System.err
			.println("Usage: java ProcessDics sort <-mon|-bil> <-xinclude|-same-file> <dic> <dic-sorted>");
		System.exit(-1);
	    } else {
		DicSort dicSort = new DicSort();
		dicSort.setArguments(arguments);
		dicSort.doSort();
	    }
	}

	if (getAction().equals("gather")) {
	    if (getArguments().length != 3) {
		System.err
			.println("Usage: java ProcessDics gather <dic> <dic-sorted>");
		System.exit(-1);
	    } else {
		DicGather dicGather = new DicGather(arguments[1], arguments[2]);
		//dicGather.setArguments(arguments);
		dicGather.doGather();
	    }
	}
	
	if (getAction().equals("addgender")) {
	    if (getArguments().length != 4) {
		System.err
			.println("Usage: java ProcessDics addgender <morph-source> <bil> <out>");
		System.exit(-1);
	    } else {
		AddGender addGender = new AddGender();
		addGender.setArguments(arguments);
		addGender.doAddGender();
	    }
	}

	if (getAction().equals("assignparadigm")) {
	    if (getArguments().length != 4) {
		System.err
			.println("Usage: java ProcessDics assignparadigm <morph-source> <bil> <out>");
		System.exit(-1);
	    } else {
		AssignParadigm a = new AssignParadigm();
		a.setArguments(arguments);
		a.doAssignParadigm();
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
