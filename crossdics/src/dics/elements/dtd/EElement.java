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

import dics.elements.utils.ElementList;
import dics.elements.utils.SElementList;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class EElement extends Element implements Cloneable,
		Comparable<EElement> {

	public static int nEElements = 0;

	/**
	 * 
	 */
	private ElementList children;

	/**
	 * 
	 */
	private String r;

	/**
	 * 
	 */
	private String lm;

	/**
	 * 
	 */
	private String a;

	/**
	 * 
	 */
	private String c;

	/**
	 * 
	 */
	private boolean shared = false;

	/**
	 * 
	 */
	private boolean common = true;

	/**
	 * 
	 */
	private boolean foreign = false;

	/**
	 * 
	 */
	private boolean locked = false;

	/**
	 * 
	 * 
	 */
	public EElement() {
		children = new ElementList();
		EElement.incrEElements();
	}

	/**
	 * 
	 * @param r
	 * @param lm
	 * @param a
	 * @param c
	 */
	public EElement(final String r, final String lm, final String a,
			final String c) {
		children = new ElementList();
		this.r = r;
		this.lm = lm;
		this.a = a;
		this.c = c;
		EElement.incrEElements();

	}

	/**
	 * 
	 * @return
	 */
	public final ElementList getChildren() {
		return children;
	}

	public static final void incrEElements() {
		EElement.nEElements++;
	}

	/**
	 * 
	 * @param value
	 */
	public final void setLemma(final String value) {
		lm = value;
	}

	/**
	 * 
	 * @return
	 */
	public final String getLemma() {
		return lm;
	}

	/**
	 * 
	 * @param value
	 */
	public final void setRestriction(final String value) {
		r = value;
	}

	/**
	 * 
	 * @return
	 */
	public final String getRestriction() {
		return r;
	}

	/**
	 * 
	 * @param comment
	 */
	public final void setComment(final String comment) {
		c = comment;
	}

	/**
	 * 
	 * @return
	 */
	public final String getComment() {
		return c;
	}

	/**
	 * 
	 * @param value
	 * @param side
	 */
	public final void setComments(final String value, final String side) {
		for (final Element e : children) {
			if (e instanceof IElement) {
				((IElement) e).setComments(value);
			}
			if (e instanceof PElement) {
				((PElement) e).setComments(value, side);
			}
		}
	}

	/**
	 * 
	 * @param value
	 */
	public final void setAuthor(final String value) {
		a = value;
	}

	/**
	 * 
	 * @return
	 */
	public final String getAuthor() {
		return a;
	}

	/**
	 * @return
	 */
	public final String getHash() {
		final String str = getValue("L") + "---" + getSElementsString("L")
				+ "---" + getValue("R") + "---" + getSElementsString("R");
		return str;
	}

	/**
	 * 
	 * @param value
	 */
	public final void setTranslation(final String value, final String side) {
		for (final Element e : children) {
			if (e instanceof IElement) {
				((IElement) e).setValue(value);
			}
			if (e instanceof PElement) {
				if (side.equals("L")) {
					((PElement) e).getL().setValue(value);
				}
				if (side.equals("R")) {
					((PElement) e).getR().setValue(value);
				}
			}
		}
	}

	/**
	 * 
	 * @param e
	 */
	public final void addChild(final Element e) {
		children.add(e);
	}

	/**
	 * Comprueba si dos entradas son iguales (de la lengua comn a los dos
	 * elementos)
	 * 
	 * @param e
	 * @return
	 */
	public boolean equalsBil(final EElement e) {
		final String value1 = getValue("L");
		final String value2 = e.getValue("L");

		if (value1.equals(value2)) {
			return true;
		}
		return false;
	}

	/**
	 * 
	 * @return
	 */
	public final PElement getP() {
		for (final Element e : children) {
			if (e instanceof PElement) {
				return (PElement) e;
			}
		}
		return null;
	}

	/**
	 * 
	 * @return
	 */
	public final IElement getI() {
		for (final Element e : children) {
			if (e instanceof IElement) {
				return (IElement) e;
			}
		}
		return null;
	}

	/**
	 * 
	 * @param side
	 * @return
	 */
	public final String getValue(final String side) {
		for (final Element e : children) {
			if (e instanceof IElement) {
				return ((IElement) e).getValue();
			}
			if (e instanceof PElement) {
				if (side.equals("L")) {
					return ((PElement) e).getL().getValue();
				}
				if (side.equals("R")) {
					return ((PElement) e).getR().getValue();
				}
			}
		}
		return getLemma();
	}

	/**
	 * 
	 * @param side
	 * @return
	 */
	public ContentElement getSide(final String side) {
		for (final Element e : children) {
			if (e instanceof IElement) {
				return ((IElement) e);
			}
			if (e instanceof PElement) {
				if (side.equals("L")) {
					return ((PElement) e).getL();
				}
				if (side.equals("R")) {
					return ((PElement) e).getR();
				}
			}
		}
		return null;
	}

	/**
	 * 
	 * @param side
	 * @return
	 */
	public final ElementList getChildren(final String side) {
		for (final Element e : children) {
			if (e instanceof IElement) {
				return ((IElement) e).getChildren();
			}
			if (e instanceof PElement) {
				if (side.equals("L")) {
					return ((PElement) e).getL().getChildren();
				}
				if (side.equals("R")) {
					return ((PElement) e).getR().getChildren();
				}
			}
		}

		return null;
	}

	/**
	 * 
	 * @param side
	 * @param value
	 * @return
	 */
	public final String setValue(final String side, final String value) {
		for (final Element e : children) {
			if (e instanceof IElement) {
				((IElement) e).setValue(value);
			}
			if (e instanceof PElement) {
				if (side.equals("L")) {
					((PElement) e).getL().setValue(value);
				}
				if (side.equals("R")) {
					((PElement) e).getR().setValue(value);
				}
			}
		}

		return null;
	}

	/**
	 * 
	 * @param side
	 * @param value
	 * @return
	 */
	public final String setChildren(final String side, final ElementList value) {
		for (final Element e : children) {
			if (e instanceof IElement) {
				((IElement) e).setChildren(value);
			}
			if (e instanceof PElement) {
				if (side.equals("L")) {
					((PElement) e).getL().setChildren(value);
				}
				if (side.equals("R")) {
					((PElement) e).getR().setChildren(value);
				}
			}
		}

		return null;
	}

	/**
	 * 
	 * @return
	 */
	public final boolean isRegularExpr() {
		for (final Element e : children) {
			if (e instanceof ReElement) {
				return true;
			}
		}
		return false;
	}

	/**
	 * 
	 */
	@Override
	public final void printXML(final DataOutputStream dos) throws IOException {
		String attributes = "";
		if (r != null) {
			attributes += " r=\"" + r + "\"";
		}
		if (lm != null) {
			attributes += " lm=\"" + lm + "\"";
		}
		if (a != null) {
			attributes += " a=\"" + a + "\"";
		}
		if (c != null) {
			attributes += " c=\"" + c + "\"";
		}
		if (comments != null) {
			dos.writeBytes(tab(2) + "<!-- \n");
			dos.writeBytes(comments);
			if (!isCommon()) {
				dos.writeBytes(tab(2)
						+ "esta entrada no aparece en el otro morfolgico\n");
			}
			dos.writeBytes(tab(2) + "-->\n");
		}
		dos.writeBytes(tab(2) + "<e" + attributes + ">\n");
		if (children != null) {
			for (final Element e : children) {
				e.printXML(dos);
			}
		}
		dos.writeBytes(tab(2) + "</e>\n\n");
	}

	/**
	 * 
	 * @param side
	 * @return
	 */
	public final String getCategory(final String side) {
		final ArrayList<String> categories = new ArrayList<String>();
		categories.add("adj");
		categories.add("adv");
		categories.add("preadv");
		categories.add("det");
		categories.add("np");
		categories.add("n");
		categories.add("pr");
		categories.add("prn");
		categories.add("rel");
		categories.add("num");
		categories.add("vblex");
		categories.add("vbser");
		categories.add("vbhaver");
		categories.add("vbmod");
		categories.add("cnjadv");
		categories.add("cnjcoo");
		categories.add("cnjsub");
		categories.add("detnt");
		categories.add("predet");
		categories.add("ij");

		// Aadir otras categorias que se quieran comprobar

		for (final String s : categories) {
			if (is("L", s)) {
				return s;
			}
		}
		return null;
	}

	/**
	 * 
	 * @param side
	 * @param categories
	 * @return
	 */
	public final String getCategory(final String side,
			final ArrayList<String> categories) {
		for (final String s : categories) {
			if (is("L", s)) {
				return s;
			}
		}
		return null;
	}

	/**
	 * 
	 * @param side
	 * @param value
	 * @return
	 */
	public final boolean is(final String side, final String value) {
		for (final Element e : children) {
			if (e instanceof IElement) {
				final IElement i = (IElement) e;
				return i.is(value);
			}
			if (e instanceof PElement) {
				final PElement p = (PElement) e;
				if (side.equals("L")) {
					final LElement lE = p.getL();
					return lE.is(value);
				}
				if (side.equals("R")) {
					final RElement rE = p.getR();
					return rE.is(value);
				}
			}
		}
		return false;
	}

	public final int getNumberOfSElements(final String side) {
		for (final Element e : children) {
			if (e instanceof IElement) {
				final IElement i = (IElement) e;
				return i.getSElements().size();
			}
			if (e instanceof PElement) {
				final PElement p = (PElement) e;
				if (side.equals("L")) {
					final LElement lE = p.getL();
					return lE.getSElements().size();
				}
				if (side.equals("R")) {
					final RElement rE = p.getR();
					return rE.getSElements().size();
				}
			}
		}
		return 0;
	}

	/**
	 * 
	 * @param side
	 * @return
	 */
	public final boolean isAdj(final String side) {
		return is(side, "adj");
	}

	/**
	 * 
	 * @param side
	 * @return
	 */
	public final boolean isNoun(final String side) {
		return is(side, "n");
	}

	/**
	 * 
	 * @return
	 */
	public final boolean isLR() {
		if (r.equals("LR")) {
			return true;
		} else {
			return false;
		}
	}

	/**
	 * 
	 * @return
	 */
	public final boolean isRL() {
		if (r.equals("RL")) {
			return true;
		} else {
			return false;
		}
	}

	/**
	 * 
	 * @param value
	 * @return
	 */
	public final boolean hasRestriction(final String value) {
		if (r == null) {
			return true;
		} else {
			if (r.equals(value)) {
				return true;
			} else {
				return false;
			}
		}
	}

	/**
	 * Has restriction LR, RL or LR/RL
	 * 
	 * @return
	 */
	public final boolean hasRestriction() {
		if (r == null) {
			return false;
		} else {
			if (r.equals("LR") || r.equals("RL")) {
				return true;
			}
		}
		return false;
	}

	/**
	 * 
	 * @return
	 */
	public final boolean is_LR_or_LRRL() {
		if (r == null) {
			return true;
		} else {
			if (r.equals("LR")) {
				return true;
			}
		}
		return false;
	}

	/**
	 * 
	 * @return
	 */
	public final boolean is_RL_or_LRRL() {
		if (r == null) {
			return true;
		} else {
			if (r.equals("RL")) {
				return true;
			}
		}
		return false;
	}

	/**
	 * 
	 * @param elements
	 * @return
	 */
	public final boolean containsSElements(final String side,
			final SElementList elementsB) {
		final SElementList elementsA = getSElements(side);
		if (elementsA.size() != elementsB.size()) {
			return false;
		} else {
			int i = 0;
			for (final SElement s1 : elementsA) {
				boolean exists = false;
				for (final SElement s2 : elementsB) {
					if (s1.equals(s2) && (exists == false)) {
						exists = true;
					}
				}
				if (exists) {
					i++;
				}
			}
			if (i == elementsA.size()) {
				return true;
			}
		}
		return false;
	}

	/**
	 * 
	 * @param side
	 * @return
	 */
	public final SElementList getSElements(final String side) {
		SElementList elementsA = null;
		for (final Element e : children) {
			if (e instanceof IElement) {
				final IElement i = (IElement) e;
				elementsA = i.getSElements();
			}
			if (e instanceof PElement) {
				final PElement p = (PElement) e;
				if (side.equals("L")) {
					final LElement lE = p.getL();
					elementsA = lE.getSElements();
				}
				if (side.equals("R")) {
					final RElement rE = p.getR();
					elementsA = rE.getSElements();
				}
			}
		}
		return elementsA;
	}

	/**
	 * 
	 * @param side
	 */
	public final void printSElements(final String side) {
		final SElementList elements = getSElements(side);
		if (elements != null) {
			for (final SElement s : elements) {
				System.err.print(s);
			}
		}
	}

	/**
	 * 
	 * @param side
	 */
	public final void print(final String side) {
		System.err.print(getSide(side).getValue() + " / ");
		printSElements(side);
		System.err.println("");
	}

	/**
	 * 
	 * @param side
	 * @return
	 */
	public final String getSElementsString(final String side) {
		final SElementList elements = getSElements(side);
		String str = "";
		if (elements != null) {
			for (final SElement s : elements) {
				str += "<s n=\"" + s.getValue() + "\"/>";
			}
		}
		return str;
	}

	/**
	 * 
	 * @param side
	 * @return
	 */
	public final String getInfo(final String side) {
		final SElementList elements = getSElements(side);
		String str = "( ";
		for (final SElement s : elements) {
			str += s.getValue() + " ";
		}
		str += ")";
		return str;
	}

	/**
	 * 
	 * @return
	 */
	public final String getParadigmValue() {
		// Returns value of first paradigm
		for (final Element e : children) {
			if (e instanceof ParElement) {
				final ParElement parE = (ParElement) e;
				return parE.getValue();
			}
		}
		return null;
	}

	/**
	 * 
	 * @param value
	 */
	public final void setShared(final boolean value) {
		shared = value;
	}

	/**
	 * 
	 * @return
	 */
	public final boolean isShared() {
		return shared;
	}

	/**
	 * 
	 * @param value
	 */
	public final void setCommon(final boolean value) {
		common = value;
	}

	/**
	 * 
	 * @return
	 */
	public final boolean isCommon() {
		return common;
	}

	/**
	 * 
	 * @param value
	 */
	public final void setForeign(final boolean value) {
		foreign = value;
	}

	/**
	 * 
	 * @return
	 */
	public final boolean isForeign() {
		return foreign;
	}

	/**
	 * 
	 * @param value
	 */
	public final void setLocked(final boolean value) {
		locked = value;
	}

	/**
	 * 
	 * @return
	 */
	public final boolean isLocked() {
		return locked;
	}

	/**
	 * 
	 * @param String
	 */
	public final void changeCategory(final String side, final String newCategory) {
		for (final Element e : children) {
			if (e instanceof IElement) {
				final IElement i = (IElement) e;
				i.changeFirstSElement(newCategory);
			}
			if (e instanceof PElement) {
				final PElement p = (PElement) e;
				if (side.equals("L")) {
					final LElement lE = p.getL();
					lE.changeFirstSElement(newCategory);
				}
				if (side.equals("R")) {
					final RElement rE = p.getR();
					rE.changeFirstSElement(newCategory);
				}
			}
		}

	}

	/**
	 * 
	 */
	@Override
	public final String toString() {
		String str = "";
		for (final Element e : children) {
			if (e instanceof IElement) {
				final IElement i = (IElement) e;
				str += i.toString();
			}
			if (e instanceof PElement) {
				final PElement p = (PElement) e;

				final LElement lE = p.getL();
				str += lE.toString();

				final RElement rE = p.getR();
				str += rE.toString();
			}
			if (e instanceof ParElement) {
				final ParElement par = (ParElement) e;
				str += par.toString();
			}

		}
		return str;
	}

	public final String toString2() {
		String str = "";
		for (final Element e : children) {
			if (e instanceof IElement) {
				final IElement i = (IElement) e;
				str += i.toString2();
			}
			if (e instanceof PElement) {
				final PElement p = (PElement) e;

				final LElement lE = p.getL();
				str += lE.toString2();

				final RElement rE = p.getR();
				str += rE.toString2();
			}

		}
		return str;
	}

	/**
	 * 
	 * @return
	 */
	public final boolean isRegEx() {
		for (final Element e : children) {
			if (e instanceof ReElement) {
				return true;
			}
		}
		return false;
	}

	/**
	 * 
	 * @return
	 */
	public final ReElement getRegEx() {
		for (final Element e : children) {
			if (e instanceof ReElement) {
				return (ReElement) e;
			}
		}
		return null;
	}

	/**
	 * 
	 */
	@Override
	public Object clone() {
		try {
			final EElement cloned = (EElement) super.clone();
			cloned.children = (ElementList) children.clone();
			return cloned;
		} catch (final Exception ex) {
			return null;
		}
	}

	/**
	 * 
	 */
	public int compareTo(final EElement anotherEElement)
			throws ClassCastException {

		if (anotherEElement == null) {
			return -1;
		}

		if (!(anotherEElement instanceof EElement)) {
			throw new ClassCastException("An EElement object expected.");
		}

		final String lemma1 = getValue("L");

		final String lemma2 = (anotherEElement).getValue("L");

		if (lemma1.compareTo(lemma2) == 0) {
			return 0;
		}
		if (lemma1.compareTo(lemma2) < 0) {
			return -1;
		}
		return 1; //     
	}

	/**
	 * 
	 * @param e2
	 * @return
	 */
	public final boolean matches(final EElement e2) {

		final String r1 = getRestriction();
		final String r2 = e2.getRestriction();
		if (r1 != null) {
			if (r2 != null) {
				if (!e2.hasRestriction(r1)) {
					return false;
				}
			}
		}

		final SElementList sEList1L = getSElements("L");
		final SElementList sEList2L = e2.getSElements("L");

		final SElementList sEList1R = getSElements("R");
		final SElementList sEList2R = e2.getSElements("R");

		if ((sEList1L != null) && (sEList2L != null)) {
			if (!sEList1L.matches(sEList2L)) {
				return false;
			}
		} else {
			return false;
		}

		if ((sEList1R != null) && (sEList2R != null)) {
			if (!sEList1R.matches(sEList2R)) {
				return false;
			}
		} else {
			return false;
		}

		return true;
	}

	/**
	 * 
	 * @return
	 */
	public final EElement reverse() {

		final EElement eRev = (EElement) clone();

		if (getRestriction() != null) {
			if (getRestriction().equals("LR")) {
				eRev.setRestriction("RL");
			} else {
				if (getRestriction().equals("RL")) {
					eRev.setRestriction("LR");
				}
			}
		}
		for (final Element e : eRev.children) {
			if (e instanceof PElement) {
				final LElement lE = ((PElement) e).getL();
				final RElement rE = ((PElement) e).getR();

				final String auxValue = lE.getValue();
				final ElementList auxChildren = lE.getChildren();

				lE.setValue(rE.getValue());
				lE.setChildren(rE.getChildren());

				rE.setValue(auxValue);
				rE.setChildren(auxChildren);
			}
		}
		return eRev;
	}

}
