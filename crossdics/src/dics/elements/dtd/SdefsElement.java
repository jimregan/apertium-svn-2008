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

package dics.elements.dtd;

import java.io.DataOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;

import dictools.DicTools;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class SdefsElement extends Element {

	/**
	 * 
	 */
	private ArrayList<SdefElement> sdefsElements;

	/**
	 * 
	 * 
	 */
	public SdefsElement() {
		setTagName("sdefs");
		sdefsElements = new ArrayList<SdefElement>();
	}

	/**
	 * 
	 * @param value
	 */
	public final void addSdefElement(final SdefElement value) {
		setTagName("sdefs");
		sdefsElements.add(value);
	}

	/**
	 * 
	 * @return
	 */
	public final ArrayList<SdefElement> getSdefsElements() {
		return sdefsElements;
	}

	/**
	 * 
	 * @param dos
	 * @throws IOException
	 */
	@Override
	public final void printXML(final DataOutputStream dos) throws IOException {
		final HashMap<String, String> descriptions = DicTools
				.getSdefDescriptions();

		dos.writeBytes(tab(1) + "<" + getTagName() + ">\n");
		for (final SdefElement e : sdefsElements) {
			final String d = descriptions.get(e.getValue());
			if (d != null) {
				e.setComments("\t<!-- " + d + "-->");
			}
			e.printXML(dos);
		}
		dos.writeBytes(tab(1) + "</" + getTagName() + ">\n");

		if (comments != null) {
		dos.writeBytes(tab(1) + "<!-- \n");
		dos.writeBytes(tab(1) + getComments());
		dos.writeBytes(tab(1) + " -->\n");
		}
	}

	/**
	 * 
	 * @return
	 */
	public final ArrayList<String> getAllCategories() {
		final ArrayList<String> categories = new ArrayList<String>();

		for (final SdefElement sdef : getSdefsElements()) {
			categories.add(sdef.getValue());
		}
		return categories;
	}

	/**
	 * 
	 */
	@Override
	public final String toString() {
		String str = "";
		for (final SdefElement sdef : getSdefsElements()) {
			str += sdef.toString();
		}
		return str;
	}

}
