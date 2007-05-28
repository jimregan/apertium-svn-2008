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

import java.io.DataOutputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Set;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class ConstantMap extends HashMap<String, String> {

	/**
	 * 
	 */
	static final long serialVersionUID = 0;

	/**
	 * 
	 * @param key
	 * @param value
	 */
	public final String insert(final String key, final String value) {
		if (!containsKey(key)) {
			put(key, value);
			return value;
		} else {
			return get(key);
		}
	}

	/**
	 * 
	 * 
	 */
	public final void print() {
		final Set keySet = keySet();
		final Iterator it = keySet.iterator();

		System.out.print("constants: ");
		while (it.hasNext()) {
			final String key = (String) it.next();
			final String value = get(key);
			System.out.print("<" + key + "," + value + "> ");
		}
		System.out.println("");
	}

	/**
	 * 
	 * @param dos
	 */
	public final void printXML(DataOutputStream dos) throws IOException {
		final Set keySet = keySet();
		final Iterator it = keySet.iterator();

		dos.writeBytes("\t<constants>\n");
		while (it.hasNext()) {
			final String key = (String) it.next();
			final String value = get(key);
			dos.writeBytes("\t\t<constant n=\"" + value + "\">" + key
					+ "</constant>\n");
		}
		dos.writeBytes("\t<constants>\n");
	}

	/**
	 * 
	 * @param cm2
	 * @return
	 */
	public final boolean matches(final ConstantMap cm2) {
		/*
		 * System.err.println("Comparing constants..."); this.print();
		 * cm2.print();
		 */
		final Set keySet = keySet();
		final Iterator it = keySet.iterator();

		while (it.hasNext()) {
			final String key = (String) it.next();
			final String value = get(key);

			if (!cm2.containsKey(key)) {
				return false;
			} else {
				final String value2 = cm2.get(key);
				if (!value.equals(value2)) {
					// System.err.println("Differences for key " + key + ": ("
					// + value + " <> " + value2 + ")");
					return false;
				}
			}
		}
		return true;
	}

	/**
	 * 
	 * @param value
	 * @return
	 */
	public final String getKey(final String value) {
		final Set keySet = keySet();
		final Iterator it = keySet.iterator();

		while (it.hasNext()) {
			final String key = (String) it.next();
			final String v = get(key);
			if (v.equals(value)) {
				return key;
			}
		}
		return null;
	}
}
