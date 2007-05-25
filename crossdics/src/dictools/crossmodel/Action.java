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

import dics.elements.dtd.EElement;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class Action {

	/**
	 * 
	 */
	private EElement e;

	/**
	 * 
	 * 
	 */
	public Action() {

	}

	/**
	 * 
	 * @param action
	 */
	public void setAction(final EElement action) {
		e = action;
	}

	/**
	 * 
	 * 
	 */
	public final void print() {
		if (e != null) {
			System.out.println("action:");
			getE().print("L");
			getE().print("R");
		}
	}

	/**
	 * 
	 * @return
	 */
	public EElement getE() {
		return e;
	}

	/**
	 * 
	 * @param e
	 */
	public void setE(final EElement e) {
		this.e = e;
	}
}
