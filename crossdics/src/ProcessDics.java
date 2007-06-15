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
import dictools.DicSort;
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
		dicConsistent.setArguments(this.getArguments());
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
		dicMerge.setArguments(this.getArguments());
		dicMerge.doMerge();
	    }
	}

	if (getAction().equals("cross")) {
	    if (getArguments().length < 8) {
		System.err
			.println("Usage: java ProcessDics cross -bilAB [-r] <bilAB> -bilBC [-r] <bilBC> -monA <mon-A> -monC <monC>");
		System.exit(-1);
	    } else {
		DicCross dicCross = new DicCross();
		dicCross.setArguments(this.getArguments());
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
	    if (getArguments().length != 3) {
		System.err
			.println("Usage: java ProcessDics format <dic> <dic-formatted>");
		System.exit(-1);
	    } else {
		final DicFormat dicFormat = new DicFormat();
		dicFormat.setArguments(arguments);
		dicFormat.doFormat();
	    }
	}

	if (getAction().equals("sort")) {
	    if (getArguments().length != 4) {
		System.err
			.println("Usage: java ProcessDics format <dic> <dic-formatted>");
		System.exit(-1);
	    } else {
		DicSort dicSort = new DicSort();
		dicSort.setArguments(arguments);
		dicSort.doSort();
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
