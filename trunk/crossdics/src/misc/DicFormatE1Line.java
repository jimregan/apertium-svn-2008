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

package misc;

import java.io.BufferedOutputStream;
import java.io.DataOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import dics.elements.dtd.DictionaryElement;
import dics.elements.dtd.EElement;
import dics.elements.dtd.SectionElement;
import dictools.DictionaryReader;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class DicFormatE1Line {

    /**
         * 
         */
    private DictionaryElement dic;

    /**
         * 
         * @param dicFileName
         */
    public DicFormatE1Line(String dicFileName) {
	DictionaryReader dicReader = new DictionaryReader(dicFileName);
	dic = dicReader.readDic();

    }

    /**
         * 
         * @param fileName
         */
    public void printXML(final String fileName) {
	BufferedOutputStream bos;
	FileOutputStream fos;
	DataOutputStream dos;

	dic.setFileName(fileName);
	try {
	    fos = new FileOutputStream(fileName);
	    bos = new BufferedOutputStream(fos);
	    dos = new DataOutputStream(bos);
	    dos.writeBytes("<?xml version=\"1.0\" encoding=\"" + dic.getXmlEncoding() + "\"?>\n");
	    dos.writeBytes("<!--\n\tDictionary:\n");
	    if (dic.getSections() != null) {
		if (dic.isBil()) {
		    dos.writeBytes("\tBilingual dictionary: " + dic.getLeftLanguage()
			    + "-" + dic.getRightLanguage() + "\n");
		}
		dos
			.writeBytes("\tSections: " + dic.getSections().size()
				+ "\n");
		int ne = 0;
		for (SectionElement section : dic.getSections()) {
		    ne += section.getEElements().size();
		}
		dos.writeBytes("\tEntries: " + ne);
	    }

	    if (dic.getSdefs() != null) {
		dos.writeBytes("\n\tSdefs: "
			+ dic.getSdefs().getSdefsElements().size() + "\n");
	    }
	    if (dic.getPardefsElement() != null) {
		dos.writeBytes("\tParadigms: "
			+ dic.getPardefsElement().getPardefElements().size()
			+ "\n");
	    }

	    if (dic.getComments() != null) {
		dos.writeBytes(dic.getComments());
	    }
	    dos.writeBytes("\n-->\n");
	    dos.writeBytes("<dictionary>\n");
	    if (dic.getAlphabet() != null) {
		dic.getAlphabet().printXML(dos);
	    }
	    if (dic.getSdefs() != null) {
		dic.getSdefs().printXML(dos);
	    }
	    if (dic.getPardefsElement() != null) {
		dic.getPardefsElement().printXML(dos);
	    }
	    if (dic.getSections() != null) {
		for (final SectionElement s : dic.getSections()) {
		    String attributes = "";
		    if (s.getID() != null) {
			attributes += " id=\"" + s.getID() + "\"";
		    }
		    if (s.getType() != null) {
			attributes += " type=\"" + s.getType() + "\"";
		    }
		    dos.writeBytes("  <section " + attributes + ">\n");
		    for (final EElement e : s.getEElements()) {
			// e.printXML(dos);
			e.printXML1Line(dos);
		    }
		    dos.writeBytes("  </section>\n");
		    // s.printXML(dos);
		}
	    }
	    dos.writeBytes("</dictionary>\n");
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
