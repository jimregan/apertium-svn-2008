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

import java.io.BufferedOutputStream;
import java.io.DataOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import dics.elements.dtd.ContentElement;
import dics.elements.dtd.EElement;
import dics.elements.dtd.LElement;
import dics.elements.dtd.PElement;
import dics.elements.dtd.RElement;
import dics.elements.dtd.SElement;
import dics.elements.dtd.SectionElement;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class CrossModel {

	/**
	 * 
	 */
	private CrossActionList crossActions;

	/**
	 * 
	 * 
	 */
	public CrossModel() {
		crossActions = new CrossActionList();
	}

	/**
	 * 
	 * @param crossAction
	 */
	public void addCrossAction(final CrossAction crossAction) {
		crossActions.add(crossAction);
	}

	/**
	 * 
	 * @return
	 */
	public CrossActionList getCrossActions() {
		return crossActions;
	}

	/**
	 * 
	 * @param e1
	 * @param e2
	 * @return
	 */
	public final CrossAction tagElements(final EElement e1, final EElement e2) {
		if ((e1 != null) && (e2 != null)) {
			final CrossAction ca = new CrossAction();
			final ConstantMap constants = new ConstantMap();

			ContentElement lE1 = e1.getSide("R");
			final LElement lE1Tagged = (LElement) tagElement(lE1, "w", e1
					.getValue("R"), constants, "LR");
			lE1 = null;

			ContentElement rE1 = e1.getSide("L");
			final RElement rE1Tagged = (RElement) tagElement(rE1, "x", e1
					.getValue("L"), constants, "RL");
			rE1 = null;

			ContentElement lE2 = e2.getSide("L");
			final LElement lE2Tagged = (LElement) tagElement(lE2, "y", e2
					.getValue("L"), constants, "LR");
			lE2 = null;

			ContentElement rE2 = e2.getSide("R");
			final RElement rE2Tagged = (RElement) tagElement(rE2, "z", e2
					.getValue("R"), constants, "RL");
			rE2 = null;

			final EElement e1Tagged = new EElement();
			final PElement p1Tagged = new PElement();
			p1Tagged.setLElement(lE1Tagged);
			p1Tagged.setRElement(rE1Tagged);
			e1Tagged.addChild(p1Tagged);

			String r = e1.getRestriction();

			if (r != null) {
				if (r.equals("LR")) {
					r = "RL";
				}
				if (r.equals("RL")) {
					r = "LR";
				}
				e1Tagged.setRestriction(r);
			}

			final EElement e2Tagged = new EElement();
			final PElement p2Tagged = new PElement();
			p2Tagged.setLElement(lE2Tagged);
			p2Tagged.setRElement(rE2Tagged);
			e2Tagged.addChild(p2Tagged);

			final Pattern pattern = new Pattern();
			pattern.setAB(e1Tagged);
			pattern.setBC(e2Tagged);

			ca.setPattern(pattern);
			ca.setConstantMap(constants);
			return ca;
		} else {
			return null;
		}

	}

	/**
	 * 
	 * @param e
	 * @return
	 */
	private ContentElement tagElement(final ContentElement cE,
			final String prefix, final String lemmaTag,
			final ConstantMap constants, final String side) {
		int i = 1;
		SElement sETagged = null;

		ContentElement cENew = null;

		if (side.equals("LR")) {
			cENew = new LElement();
		}

		if (side.equals("RL")) {
			cENew = new RElement();
		}

		cENew.setValue(lemmaTag);
		// System.err.print(lemmaTag + " / ");
		for (final SElement s : cE.getSElements()) {
			// <w1>
			String var = prefix + i;
			// <f>
			final String sValue = s.getValue();
			// <f,w1>
			if (!constants.containsKey(sValue)) {
				i++;
			}
			var = constants.insert(sValue, var);
			sETagged = SElement.get(var);
			// System.err.print("<" + sETagged.getValue() + ">");
			cENew.addChild(sETagged);
		}
		System.err.println("");

		return cENew;
	}

	/**
	 * 
	 * @param cA
	 * @return
	 */
	public final String matches(final CrossAction cA) {
		System.err.println("tagged...");
		cA.print();
		System.err.println("-----------------");
		for (final CrossAction crossAction : getCrossActions()) {
			/*
			 * System.err.println("pattern..."); crossAction.print();
			 */
			if (crossAction.matches(cA)) {
				System.err
						.println("MATCH! cross-action " + crossAction.getId());
				crossAction.print();
				return crossAction.getId();
			}
		}
		return null;
	}

	/**
	 * 
	 * @param id
	 * @return
	 */
	public final CrossAction getCrossAction(final String id) {
		for (final CrossAction ca : getCrossActions()) {
			if (ca.getId().equals(id)) {
				return ca;
			}
		}
		return null;
	}

	/**
	 * 
	 * @param fileName
	 */
	public void printXML(final String fileName) {
		BufferedOutputStream bos;
		FileOutputStream fos;
		DataOutputStream dos;

		try {
			fos = new FileOutputStream(fileName);
			bos = new BufferedOutputStream(fos);
			dos = new DataOutputStream(bos);
			dos.writeBytes("<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n");
			dos.writeBytes("<cross-model>\n");
			int i = 0;
			for (CrossAction crossAction : getCrossActions()) {
				crossAction.printXML(dos, i);
				i++;
			}
			dos.writeBytes("</cross-model>\n");

			fos = null;
			bos = null;
			dos.close();
			dos = null;
		} catch (final IOException e) {
			e.printStackTrace();
		} catch (final Exception eg) {
			eg.printStackTrace();
		}
	}

}
