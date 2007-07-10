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

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.w3c.dom.Text;
import org.xml.sax.SAXException;

import dics.elements.dtd.AElement;
import dics.elements.dtd.BElement;
import dics.elements.dtd.ContentElement;
import dics.elements.dtd.EElement;
import dics.elements.dtd.GElement;
import dics.elements.dtd.IElement;
import dics.elements.dtd.JElement;
import dics.elements.dtd.LElement;
import dics.elements.dtd.PElement;
import dics.elements.dtd.ParElement;
import dics.elements.dtd.RElement;
import dics.elements.dtd.ReElement;
import dics.elements.dtd.SElement;
import dics.elements.dtd.TextElement;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class XMLReader {

    /**
         * 
         */
    protected DocumentBuilderFactory factory;

    /**
         * 
         */
    protected DocumentBuilder builder;

    /**
         * 
         */
    protected Document document;

    /**
         * 
         */
    protected File dicFile;

    /*
         * 
         */
    protected BElement bElement;

    /**
         * 
         */
    protected AElement aElement;

    /**
         * 
         */
    protected JElement jElement;

    /**
         * 
         * @param fileName
         */
    public XMLReader(final String fileName) {
	setDicFile(new File(fileName));
	try {
	    setFactory(DocumentBuilderFactory.newInstance());
	    setBuilder(getFactory().newDocumentBuilder());
	} catch (final ParserConfigurationException pce) {
	    pce.printStackTrace();
	} catch (final Exception e) {
	    e.printStackTrace();
	}
	bElement = new BElement();
	aElement = new AElement();
	jElement = new JElement();

    }

    /**
         * 
         * 
         */
    protected final void analize() {
	try {
	    setDocument(getBuilder().parse(getDicFile()));
	} catch (final FileNotFoundException fnfe) {
	    System.err.println("Error: could not find '" + getDicFile()
		    + "' file.");
	    System.exit(-1);
	} catch (final SAXException saxE) {
	    saxE.printStackTrace();
	    System.err.println("Error: could not parse '" + getDicFile() + "'");
	} catch (final IOException ioE) {
	    ioE.printStackTrace();
	    System.err.println("I/O error");
	} catch (final Exception e) {
	    e.printStackTrace();
	    System.err
		    .println("Error: the XML document is probably not well-formed");
	} finally {
	    setBuilder(null);
	    setFactory(null);
	}
    }

    /**
         * 
         * @param e
         * @param tagName
         * @return
         */
    protected ArrayList<org.w3c.dom.Element> readChildren(final Element e) {
	final ArrayList<org.w3c.dom.Element> eList = new ArrayList<org.w3c.dom.Element>();
	if (e.hasChildNodes()) {
	    final NodeList children = e.getChildNodes();
	    for (int i = 0; i < children.getLength(); i++) {
		final Node child = children.item(i);

		/*
                 * if (child instanceof Comment) { Comment comment =
                 * (Comment)child; System.out.println("Comment: " +
                 * comment.getTextContent()); }
                 */

		if (child instanceof Element) {
		    final Element childElement = (Element) child;
		    eList.add(childElement);
		}
	    }
	}
	return eList;
    }

    /**
         * 
         * @param e
         * @return
         */
    protected EElement readEElement(final Element e) {

	final String r = getAttributeValue(e, "r");
	final String lm = getAttributeValue(e, "lm");
	final String a = getAttributeValue(e, "a");
	final String c = getAttributeValue(e, "c");

	final EElement eElement = new EElement(r, lm, a, c);

	if (e.hasChildNodes()) {
	    final NodeList children = e.getChildNodes();
	    for (int i = 0; i < children.getLength(); i++) {
		final Node child = children.item(i);
		if (child instanceof Element) {
		    final Element childElement = (Element) child;
		    final String childElementName = childElement.getNodeName();
		    if (childElementName.equals("i")) {
			final IElement iElement = readIElement(childElement);
			eElement.addChild(iElement);
		    }
		    if (childElementName.equals("p")) {
			final PElement pElement = readPElement(childElement);
			eElement.addChild(pElement);
		    }
		    if (childElementName.equals("par")) {
			final ParElement parElement = readParElement(childElement);
			eElement.addChild(parElement);
		    }
		    if (childElementName.equals("re")) {
			final ReElement reElement = readReElement(childElement);
			eElement.addChild(reElement);
		    }
		}
	    }
	}
	return eElement;
    }

    /**
         * 
         * @param e
         * @param attrName
         * @return
         */
    protected String getAttributeValue(final Element e, final String attrName) {
	String value = "";
	if (e.hasAttributes()) {
	    final NamedNodeMap attributes = e.getAttributes();
	    for (int i = 0; i < attributes.getLength(); i++) {
		final Node attribute = attributes.item(i);
		final String name = attribute.getNodeName();
		value = attribute.getNodeValue();
		if (name.equals(attrName)) {
		    return value;
		} // end-if
	    } // end-for
	} // end-if
	return null;
    }

    /**
         * 
         * @param e
         * @return
         */
    protected SElement readSElement(final Element e) {
	final String n = getAttributeValue(e, "n");
	return SElement.get(n);
    }

    /**
         * 
         * @param child
         * @return
         */
    protected String loadGElementText(final Node child) {
	String text = "<g>";
	if (child.hasChildNodes()) {
	    final NodeList children = child.getChildNodes();
	    for (int i = 0; i < children.getLength(); i++) {
		final Node node = children.item(i);
		if (node instanceof Text) {
		    final Text textNode = (Text) node;
		    text += textNode.getData().trim();
		} else {
		    final String tag = node.getNodeName();
		    if (tag.equals("b")) {
			text += "<b/>";
		    }
		    if (tag.equals("j")) {
			text += "<j/>";
		    }
		    if (tag.equals("a")) {
			text += "<a/>";
		    }
		}
	    }
	}
	text += "</g>";
	return text;
    }

    /**
         * 
         * @param e
         * @return
         */
    protected IElement readIElement(final Element e) {
	final IElement iElement = new IElement();
	final IElement iE = (IElement) readContentElement(e, iElement);
	return iE;
    }

    /**
         * 
         * @param e
         * @return
         */
    protected ContentElement readContentElement(final Element e,
	    ContentElement cElement) {

	String text = "";

	if (e.hasChildNodes()) {
	    final NodeList children = e.getChildNodes();
	    for (int i = 0; i < children.getLength(); i++) {
		final Node child = children.item(i);
		if (child instanceof Text) {
		    final Text textNode = (Text) child;
		    final String str = textNode.getData().trim();
		    text += str;
		    final TextElement tE = new TextElement(str);
		    cElement.addChild(tE);
		} else {
		    final Element childElement = (Element) child;
		    final String tag = childElement.getNodeName();
		    text += processTagText(tag, child);
		    dics.elements.dtd.Element element = processTagE(tag, child);
		    cElement.addChild(element);
		}
	    }
	} else {
	    text = "";
	}
	// cElement.setValue(text);
	return cElement;
    }

    /**
         * 
         * @param tag
         * @param child
         * @return
         */
    protected final String processTagText(final String tag, final Node child) {
	String text = "";
	/*
         * if (tag.equals("b")) { text = text + "<b/>"; } if (tag.equals("j")) {
         * text = text + "<j/>"; } if (tag.equals("a")) { text = text + "<a/>"; }
         */
	if (tag.equals("g")) {
	    text = text + loadGElementText(child);
	}
	return text;
    }

    /**
         * 
         * @param tag
         * @param child
         * @return
         */
    protected final dics.elements.dtd.Element processTagE(final String tag,
	    final Node child) {
	if (tag.equals("s")) {
	    final Element childElement = (Element) child;
	    final SElement sElement = readSElement(childElement);
	    return sElement;
	}
	if (tag.equals("b")) {
	    return getBElement();
	}
	if (tag.equals("j")) {
	    return getJElement();
	}
	if (tag.equals("a")) {
	    return getAElement();
	}
	if (tag.equals("g")) {
	    final Element childElement = (Element) child;
	    final GElement gElement = readGElement(childElement);
	    return gElement;
	}
	return null;
    }

    /**
         * 
         * @param e
         * @return
         */
    protected LElement readLElement(final Element e) {
	final LElement lElement = new LElement();
	final LElement lE = (LElement) readContentElement(e, lElement);
	return lE;
    }

    /**
         * 
         * @param e
         * @return
         */
    protected RElement readRElement(final Element e) {
	final RElement rElement = new RElement();
	final RElement rE = (RElement) readContentElement(e, rElement);
	return rE;
    }

    /**
         * 
         * @param e
         * @return
         */
    protected GElement readGElement(final Element e) {
	GElement gElement = new GElement();
	GElement gE = (GElement) readContentElement(e, gElement);
	return gE;
    }

    /**
         * 
         * @param e
         * @return
         */
    protected PElement readPElement(final Element e) {
	final PElement pElement = new PElement();

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
    protected ParElement readParElement(final Element e) {
	final String n = getAttributeValue(e, "n");
	final ParElement parElement = new ParElement(n);
	return parElement;
    }

    /**
         * 
         * @param e
         * @return
         */
    protected ReElement readReElement(final Element e) {
	String value = "";

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

    /**
         * @return the aElement
         */
    protected final AElement getAElement() {
	return aElement;
    }

    /**
         * @param element
         *                the aElement to set
         */
    protected final void setAElement(final AElement element) {
	aElement = element;
    }

    /**
         * @return the bElement
         */
    protected final BElement getBElement() {
	return bElement;
    }

    /**
         * @param element
         *                the bElement to set
         */
    protected final void setBElement(final BElement element) {
	bElement = element;
    }

    /**
         * @return the builder
         */
    protected final DocumentBuilder getBuilder() {
	return builder;
    }

    /**
         * @param builder
         *                the builder to set
         */
    protected final void setBuilder(final DocumentBuilder builder) {
	this.builder = builder;
    }

    /**
         * @return the dicFile
         */
    protected final File getDicFile() {
	return dicFile;
    }

    /**
         * @param dicFile
         *                the dicFile to set
         */
    protected final void setDicFile(final File dicFile) {
	this.dicFile = dicFile;
    }

    /**
         * @return the document
         */
    protected final Document getDocument() {
	return document;
    }

    /**
         * @param document
         *                the document to set
         */
    protected final void setDocument(final Document document) {
	this.document = document;
    }

    /**
         * @return the factory
         */
    protected final DocumentBuilderFactory getFactory() {
	return factory;
    }

    /**
         * @param factory
         *                the factory to set
         */
    protected final void setFactory(final DocumentBuilderFactory factory) {
	this.factory = factory;
    }

    /**
         * @return the jElement
         */
    protected final JElement getJElement() {
	return jElement;
    }

    /**
         * @param element
         *                the jElement to set
         */
    protected final void setJElement(final JElement element) {
	jElement = element;
    }

}
