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

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class CrossAction {

	/**
	 * 
	 */
	private String id;

	/**
	 * 
	 */
	private Pattern pattern;

	/**
	 * 
	 */
	private ConstantMap constants;

	/**
	 * 
	 */
	private Action action;

	/**
	 * 
	 * 
	 */
	public CrossAction() {

	}

	/**
	 * 
	 * @param p
	 * @param cm
	 * @param a
	 */
	public CrossAction(final Pattern p, final ConstantMap cm, final Action a) {
		pattern = p;
		constants = cm;
		action = a;
	}

	/**
	 * 
	 * @param p
	 */
	public void setPattern(final Pattern p) {
		pattern = p;
	}

	/**
	 * 
	 * @param cm
	 */
	public void setConstantMap(final ConstantMap cm) {
		constants = cm;
	}

	/**
	 * 
	 * @param a
	 */
	public void setAction(final Action a) {
		action = a;
	}

	/**
	 * 
	 * @param ca
	 * @return
	 */
	public final boolean matches(final CrossAction ca) {
		final Pattern pattern1 = getPattern();
		final Pattern pattern2 = ca.getPattern();

		final ConstantMap cm1 = getConstants();
		final ConstantMap cm2 = ca.getConstants();

		if (cm1 != null) {
			if (!cm1.matches(cm2)) {
				return false;
			}
		}
		/*
		 * System.out.println("Comparing..."); pattern1.print();
		 * pattern2.print();
		 */

		if (!pattern1.matches(pattern2)) {
			return false;
		} else {
			return true;
		}
	}

	/**
	 * 
	 * @return
	 */
	public final ConstantMap getConstants() {
		return constants;
	}

	/**
	 * 
	 * @param constants
	 */
	public final void setConstants(final ConstantMap constants) {
		this.constants = constants;
	}

	/**
	 * 
	 * @return
	 */
	public final Action getAction() {
		return action;
	}

	/**
	 * 
	 * @return
	 */
	public final Pattern getPattern() {
		return pattern;
	}

	/**
	 * 
	 * @return
	 */
	public final String getId() {
		return id;
	}

	/**
	 * 
	 * @param id
	 */
	public final void setId(final String id) {
		this.id = id;
	}

	/**
	 * 
	 * 
	 */
	public final void print() {
		if (pattern != null) {
			getPattern().print();
		}
		if (constants != null) {
			getConstants().print();
		}
		if (action != null) {
			getAction().print();
		}
	}

}
