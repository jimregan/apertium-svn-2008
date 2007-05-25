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

import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.w3c.dom.Text;

import dics.elements.dtd.EElement;
import dictools.XMLReader;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class CrossModelReader extends XMLReader {

	/**
	 * 
	 * @param fileName
	 */
	public CrossModelReader(final String fileName) {
		super(fileName);
	}

	/**
	 * 
	 * @return
	 */
	public CrossModel readCrossModel() {
		analize();
		final CrossModel crossModel = new CrossModel();

		Element root = getDocument().getDocumentElement();

		for (final Element childElement : readChildren(root)) {
			final String childElementName = childElement.getNodeName();
			if (childElementName.equals("cross-action")) {
				final CrossAction crossAction = readCrossAction(childElement);
				crossModel.addCrossAction(crossAction);
			}
		}

		root = null;
		setDocument(null);
		return crossModel;
	}

	/**
	 * 
	 * @param element
	 * @return
	 */
	public final CrossAction readCrossAction(final Element e) {
		final CrossAction crossAction = new CrossAction();
		final String id = getAttributeValue(e, "id");
		crossAction.setId(id);

		for (final Element childElement : readChildren(e)) {
			final String childElementName = childElement.getNodeName();
			if (childElementName.equals("pattern")) {
				final Pattern pattern = readPattern(childElement);
				crossAction.setPattern(pattern);
			}

			if (childElementName.equals("constants")) {
				final ConstantMap constants = readConstants(childElement);
				crossAction.setConstantMap(constants);
			}

			if (childElementName.equals("action")) {
				final Action action = readAction(childElement);
				crossAction.setAction(action);
			}

		}
		return crossAction;
	}

	/**
	 * 
	 * @param e
	 * @return
	 */
	public final Pattern readPattern(final Element e) {
		int i = 0;
		final Pattern pattern = new Pattern();

		for (final Element childElement : readChildren(e)) {
			final String childElementName = childElement.getNodeName();
			if (childElementName.equals("e")) {
				final EElement eE = readEElement(childElement);
				if (i == 0) {
					pattern.setAB(eE);
				}
				if (i == 1) {
					pattern.setBC(eE);
				}
				i++;
			}
		}
		return pattern;
	}

	/**
	 * 
	 * @param e
	 * @return
	 */
	public final ConstantMap readConstants(final Element e) {
		final ConstantMap constants = new ConstantMap();
		for (final Element childElement : readChildren(e)) {
			final String childElementName = childElement.getNodeName();
			if (childElementName.equals("constant")) {
				final Constant constant = readConstant(childElement);
				constants.put(constant.getValue(), constant.getName());
			}
		}
		return constants;
	}

	/**
	 * 
	 * @param e
	 * @return
	 */
	public final Constant readConstant(final Element e) {
		final Constant constant = new Constant();
		final String n = getAttributeValue(e, "n");
		constant.setName(n);

		if (e.hasChildNodes()) {
			final NodeList children = e.getChildNodes();
			for (int i = 0; i < children.getLength(); i++) {
				final Node child = children.item(i);
				if (child instanceof Text) {
					final Text textNode = (Text) child;
					final String str = textNode.getData().trim();
					constant.setValue(str);
				}
			}
		}

		return constant;
	}

	/**
	 * 
	 * @param e
	 * @return
	 */
	public final Action readAction(final Element e) {
		final int i = 0;
		final Action action = new Action();
		for (final Element childElement : readChildren(e)) {
			final String childElementName = childElement.getNodeName();
			if (childElementName.equals("e")) {
				final EElement eE = readEElement(childElement);
				if (i == 0) {
					action.setAction(eE);
				}
			}
		}
		return action;
	}

}
