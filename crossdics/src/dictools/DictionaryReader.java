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

package dictools;

import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.w3c.dom.Text;

import dics.elements.dtd.AlphabetElement;
import dics.elements.dtd.DictionaryElement;
import dics.elements.dtd.EElement;
import dics.elements.dtd.GElement;
import dics.elements.dtd.IElement;
import dics.elements.dtd.LElement;
import dics.elements.dtd.PElement;
import dics.elements.dtd.ParElement;
import dics.elements.dtd.PardefElement;
import dics.elements.dtd.PardefsElement;
import dics.elements.dtd.RElement;
import dics.elements.dtd.ReElement;
import dics.elements.dtd.SElement;
import dics.elements.dtd.SdefElement;
import dics.elements.dtd.SdefsElement;
import dics.elements.dtd.SectionElement;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class DictionaryReader extends XMLReader {

	/**
	 * 
	 * @param fileName
	 */
	public DictionaryReader(final String fileName) {
		super(fileName);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see dictools.IDicReader#loadDic()
	 */
	public DictionaryElement readDic() {
		analize();
		final DictionaryElement dic = new DictionaryElement();

		Element root = document.getDocumentElement();

		final NodeList children = root.getChildNodes();
		for (int i = 0; i < children.getLength(); i++) {
			final Node child = children.item(i);
			if (child instanceof Element) {
				final Element childElement = (Element) child;
				final String childElementName = childElement.getNodeName();
				// Alphabet
				if (childElementName.equals("alphabet")) {
					final AlphabetElement alphabetElement = readAlphabet(childElement);
					dic.setAlphabet(alphabetElement);
				}
				// Definitions
				if (childElementName.equals("sdefs")) {
					final SdefsElement sdefsElement = readSdefs(childElement);
					dic.setSdefs(sdefsElement);
				}
				if (childElementName.equals("section")) {
					final SectionElement sectionElement = readSection(childElement);
					dic.addSection(sectionElement);
				}
				if (childElementName.equals("pardefs")) {
					final PardefsElement pardefsElement = readPardefs(childElement);
					dic.setPardefs(pardefsElement);
				}
			}
		}
		root = null;
		document = null;
		return dic;
	}

	/**
	 * 
	 * @param e
	 * @return
	 */
	public AlphabetElement readAlphabet(final Element e) {
		String alphabet = "";
		if (e.hasChildNodes()) {
			final NodeList nodeList = e.getChildNodes();
			for (int j = 0; j < nodeList.getLength(); j++) {
				final Node node = nodeList.item(j);
				if (node instanceof Text) {
					final Text textNode = (Text) node;
					alphabet = textNode.getData().trim();
				}
			}
		}
		final AlphabetElement alphabetElement = new AlphabetElement(alphabet);

		return alphabetElement;
	}

	/**
	 * 
	 * @param e
	 */
	public SdefsElement readSdefs(final Element e) {
		final SdefsElement sdefsElement = new SdefsElement();

		for (Element childElement : readChildren(e)) {
			final String childElementName = childElement.getNodeName();
			if (childElementName.equals("sdef")) {
				final SdefElement sdefElement = readSdef(childElement);
				SElement sE = SElement.get(sdefElement.getValue());
				sdefsElement.addSdefElement(sdefElement);
			}
		}

		return sdefsElement;
	}

	/**
	 * 
	 * @param e
	 */
	public SdefElement readSdef(final Element e) {
		final String n = getAttributeValue(e, "n");
		final SdefElement sdefElement = new SdefElement(n);
		return sdefElement;
	}

	/**
	 * 
	 * @param e
	 */
	public PardefsElement readPardefs(final Element e) {
		final PardefsElement pardefsElement = new PardefsElement();

		for (Element childElement : readChildren(e)) {
			final String childElementName = childElement.getNodeName();
			if (childElementName.equals("pardef")) {
				final PardefElement pardefElement = readPardef(childElement);
				pardefsElement.addPardefElement(pardefElement);
			}
		}

		return pardefsElement;

	}

	/**
	 * 
	 * @param e
	 */
	public PardefElement readPardef(final Element e) {
		final String n = getAttributeValue(e, "n");
		final PardefElement pardefElement = new PardefElement(n);

		for (Element childElement : readChildren(e)) {
			final String childElementName = childElement.getNodeName();
			if (childElementName.equals("e")) {
				final EElement eElement = readEElement(childElement);
				pardefElement.addEElement(eElement);
			}
		}

		return pardefElement;
	}

	/**
	 * 
	 * @param e
	 * @return
	 */
	public SectionElement readSection(final Element e) {
		final String id = getAttributeValue(e, "id");
		final String type = getAttributeValue(e, "type");
		final SectionElement sectionElement = new SectionElement(id, type);

		// Si contiene elementos 'e'
		if (e.hasChildNodes()) {
			final NodeList children = e.getChildNodes();
			for (int i = 0; i < children.getLength(); i++) {
				final Node child = children.item(i);
				if (child instanceof Element) {
					final Element childElement = (Element) child;
					final String childElementName = childElement.getNodeName();
					if (childElementName.equals("e")) {
						final EElement eElement = readEElement(childElement);
						sectionElement.addEElement(eElement);
					}
				}
			}
		}
		return sectionElement;
	}

	/**
	 * 
	 * @param e
	 * @return
	 */
	public IElement readIElement(final Element e) {
		final IElement iElement = new IElement();
		final IElement iE = (IElement) readContentElement(e, iElement);
		return iE;
	}

	/**
	 * 
	 * @param e
	 * @return
	 */
	public LElement readLElement(final Element e) {
		final LElement lElement = new LElement();
		final LElement lE = (LElement) readContentElement(e, lElement);
		return lE;
	}

	/**
	 * 
	 * @param e
	 * @return
	 */
	public RElement readRElement(final Element e) {
		final RElement rElement = new RElement();
		final RElement rE = (RElement) readContentElement(e, rElement);
		return rE;
	}

	/**
	 * 
	 * @param e
	 * @return
	 */
	public GElement readGElement(final Element e) {
		final GElement gElement = new GElement();
		final GElement gE = (GElement) readContentElement(e, gElement);
		return gE;
	}

	/**
	 * 
	 * @param e
	 * @return
	 */
	public PElement readPElement(final Element e) {
		final PElement pElement = new PElement();

		// Si contiene elementos 'e'
		if (e.hasChildNodes()) {
			final NodeList children = e.getChildNodes();
			for (int i = 0; i < children.getLength(); i++) {
				final Node child = children.item(i);
				if (child instanceof Element) {
					final Element childElement = (Element) child;
					final String childElementName = childElement.getNodeName();
					if (childElementName.equals("l")) {
						final LElement lElement = readLElement(childElement);
						pElement.setLElement(lElement);
					}
					if (childElementName.equals("r")) {
						final RElement rElement = readRElement(childElement);
						pElement.setRElement(rElement);
					}

				}
			}
		}
		return pElement;
	}

	/**
	 * 
	 * @param e
	 * @return
	 */
	public ParElement readParElement(final Element e) {
		final String n = getAttributeValue(e, "n");
		final ParElement parElement = new ParElement(n);
		return parElement;
	}

	/**
	 * 
	 * @param e
	 * @return
	 */
	public ReElement readReElement(final Element e) {
		String value = "";
		// Si contiene elementos 'e'
		if (e.hasChildNodes()) {
			final NodeList children = e.getChildNodes();
			for (int i = 0; i < children.getLength(); i++) {
				final Node child = children.item(i);
				if (child instanceof Text) {
					final Text textNode = (Text) child;
					value += textNode.getData().trim();
				}
			}
		}
		final ReElement reElement = new ReElement(value);
		return reElement;
	}

}
