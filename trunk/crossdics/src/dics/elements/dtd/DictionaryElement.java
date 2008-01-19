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

import java.io.BufferedOutputStream;
import java.io.DataOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Set;
import dics.elements.utils.EElementList;
import dics.elements.utils.EElementMap;
import dics.elements.utils.ElementList;
import dictools.DicEquivPar;
import dictools.DicTools;

/**
 *
 * @author Enrique Benimeli Bofarull
 *
 */
public class DictionaryElement extends Element {

    /**
     *
     */
    protected AlphabetElement alphabet;
    /**
     *
     */
    protected SdefsElement sdefs;
    /**
     *
     */
    protected PardefsElement pardefs;
    /**
     *
     */
    protected ArrayList<SectionElement> sections;
    /**
     *
     */
    protected int nForeignEntries;
    /**
     *
     */
    protected int nEntries;
    /**
     *
     */
    protected int nShared;
    /**
     *
     */
    protected int nDifferent;
    /**
     *
     */
    protected HashMap<String, ArrayList<PardefElement>> equivPar;
    /**
     *
     */
    private String type;
    /**
     *
     */
    private String fileName;
    /**
     *
     */
    private String filePath;
    /**
     *
     */
    private String leftLanguage;
    /**
     *
     */
    private String rightLanguage;
    /**
     *
     */
    private String folder;
    /**
     *
     */
    private String xmlEncoding = "iso-8859-1";
    /**
     *
     */
    private String xmlVersion;

    /**
     *
     *
     */
    public DictionaryElement() {
        sections = new ArrayList<SectionElement>();
    }

    /**
     *
     * @param elementMap
     */
    public DictionaryElement(final EElementMap elementMap, final DictionaryElement dic) {
        sections = new ArrayList<SectionElement>();
        final SectionElement sectionElement = new SectionElement("main", "standard");
        addSection(sectionElement);

        setAlphabet(dic.getAlphabet());
        setSdefs(dic.getSdefs());

        final Set keySet = elementMap.keySet();
        final Iterator it = keySet.iterator();

        while (it.hasNext()) {
            final String key = (String) it.next();
            final EElementList eList = elementMap.get(key);
            for (final EElement e : eList) {
                addEElement(e);
            }
        }
    }

    /**
     *
     * @param dic
     */
    public DictionaryElement(final DictionaryElement dic) {
        sections = new ArrayList<SectionElement>();
        final SectionElement sectionElement = new SectionElement("main", "standard");
        addSection(sectionElement);
        setAlphabet(dic.getAlphabet());
        setSdefs(dic.getSdefs());

        if (dic.isMonol()) {
            setPardefs(dic.getPardefsElement());
        }

        final SectionElement sectionElementMain = dic.getSection("main");
        final ArrayList<EElement> eList = sectionElementMain.getEElements();
        for (final EElement e : eList) {
            if (!e.isShared()) {
                addEElement(e);
            }
        }
    }

    /**
     *
     * @param eList
     */
    public final void setMainSection(final EElementList eList) {
        for (final SectionElement section : sections) {
            if (section.getID().equals("main")) {
                final SectionElement sectionElementMain = new SectionElement(section.getID(), section.getType());
                sections.remove(section);
                for (final EElement e : eList) {
                    sectionElementMain.addEElement(e);
                }
                addSection(sectionElementMain);
            }
        }
    }

    /**
     *
     * @param value
     */
    public final void setType(final String value) {
        type = value;
    }

    /**
     *
     * @return
     */
    public final String getType() {
        return type;
    }

    /**
     *
     * @param value
     */
    public final void setFileName(final String value) {
        fileName = value;
    }

    /**
     *
     * @return
     */
    public final String getFileName() {
        return fileName;
    }

    /**
     *
     */
    public final void setFilePath(final String path) {
        this.filePath = path;
    }

    /**
     *
     */
    public final String getFilePath() {
        return this.filePath;
    }

    /**
     *
     * @param value
     */
    public final void setLeftLanguage(final String value) {
        leftLanguage = value;
    }

    /**
     *
     * @return
     */
    public final String getLeftLanguage() {
        return leftLanguage;
    }

    /**
     *
     * @param value
     */
    public final void setRightLanguage(final String value) {
        rightLanguage = value;
    }

    /**
     *
     * @return
     */
    public final String getRightLanguage() {
        return rightLanguage;
    }

    /**
     *
     * @return
     */
    public boolean isMonol() {
        if (type != null) {
            if (type.equals("MONOL")) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    /**
     *
     * @return
     */
    public boolean isBil() {
        if (type != null) {
            if (type.equals("BIL")) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    /**
     *
     *
     */
    public AlphabetElement getAlphabet() {
        return alphabet;
    }

    /**
     *
     * @param value
     */
    public void setAlphabet(final AlphabetElement value) {
        alphabet = value;
    }

    /**
     *
     * @param value
     */
    public void setSdefs(final SdefsElement value) {
        sdefs = value;
    }

    /**
     *
     * @return
     */
    public SdefsElement getSdefs() {
        return sdefs;
    }

    /**
     *
     * @param value
     */
    public void setPardefs(final PardefsElement value) {
        pardefs = value;
    }

    /**
     *
     * @param value
     */
    public void addSection(final SectionElement value) {
        sections.add(value);
    }

    /**
     *
     * @return
     */
    public ArrayList<SectionElement> getSections() {
        return sections;
    }

    /**
     *
     * @param id
     * @return
     */
    public SectionElement getSection(final String id) {
        for (final SectionElement section : sections) {
            if (section.getID().equals(id)) {
                return section;
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

        setFileName(fileName);
        try {
            fos = new FileOutputStream(fileName);
            bos = new BufferedOutputStream(fos);
            dos = new DataOutputStream(bos);
            dos.writeBytes("<?xml version=\"1.0\" encoding=\"" + getXmlEncoding() + "\"?>\n");
            dos.writeBytes("<!--\n\tDictionary:\n");
            if (sections != null) {
                if (isBil()) {
                    dos.writeBytes("\tBilingual dictionary: " + getLeftLanguage() + "-" + getRightLanguage() + "\n");
                }
                dos.writeBytes("\tSections: " + sections.size() + "\n");
                int ne = 0;
                for (SectionElement section : sections) {
                    ne += section.getEElements().size();
                }
                dos.writeBytes("\tEntries: " + ne);
            }
            
            if (sdefs != null) {
                dos.writeBytes("\n\tSdefs: " + sdefs.getSdefsElements().size() + "\n");
            }
            if (pardefs != null) {
                dos.writeBytes("\tParadigms: " + pardefs.getPardefElements().size() + "\n");
            }

            if (comments != null) {
                dos.writeBytes(comments);
            }
            dos.writeBytes("\n-->\n");
            dos.writeBytes("<dictionary>\n");
            if (alphabet != null) {
                alphabet.printXML(dos);
            }
            if (sdefs != null) {
                sdefs.printXML(dos);
            }
            if (pardefs != null) {
                pardefs.printXML(dos);
            }
            if (sections != null) {
                for (final SectionElement s : sections) {
                    s.printXML(dos);
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

    /**
     *
     * @param fileName
     */
    public void printXMLXInclude(final String fileName) {
        BufferedOutputStream bos;
        FileOutputStream fos;
        DataOutputStream dos;

        setFileName(fileName);

        try {
            fos = new FileOutputStream(fileName);
            bos = new BufferedOutputStream(fos);
            dos = new DataOutputStream(bos);
            dos.writeBytes("<?xml version=\"1.0\" encoding=\"" + getXmlEncoding() + "\"?>\n");
            dos.writeBytes("<!--\n\tDictionary:\n");
            if (sections != null) {
                if (isBil()) {
                    dos.writeBytes("\tBilingual dictionary: " + getLeftLanguage() + "-" + getRightLanguage() + "\n");
                }
                dos.writeBytes("\tSections: " + sections.size() + "\n");
                dos.writeBytes("\tEntries: " + (sections.get(0)).getEElements().size());
            }
            if (sdefs != null) {
                dos.writeBytes("\n\tSdefs: " + sdefs.getSdefsElements().size() + "\n");
            }
            dos.writeBytes("");

            if (comments != null) {
                dos.writeBytes(comments);
            }
            dos.writeBytes("\n-->\n");
            dos.writeBytes("<dictionary>\n");
            if (alphabet != null) {
                alphabet.printXML(dos);
            }
            String includeStr = "<xi:include xmlns:xi=\"http://www.w3.org/2001/XInclude\"";
            dos.writeBytes("\t" + includeStr + " href=\"" + getFolder() + "/sdefs.dix\"/>\n");
            sdefs.printXML(getFolder() + "/sdefs.dix");
            dos.writeBytes("\t" + includeStr + " href=\"" + getFolder() + "/pardefs.dix\"/>\n");
            pardefs.printXML(getFolder() + "/pardefs.dix");

            for (SectionElement section : sections) {
                ArrayList<String> includes = section.getIncludes();
                String attributes = "";
                if (section.getID() != null) {
                    attributes += " id=\"" + section.getID() + "\"";
                }
                if (section.getType() != null) {
                    attributes += " type=\"" + section.getType() + "\"";
                }
                dos.writeBytes(tab(1) + "<" + section.getTagName() + "" + attributes + ">\n");
                if (includes != null) {
                    for (final String s : includes) {
                        dos.writeBytes("\t" + s + "\n");
                    }
                }
                dos.writeBytes(tab(1) + "</" + section.getTagName() + ">\n");
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

    /**
     *
     * @return The 'e' elements
     */
    public EElementList getEntries() {
        for (final SectionElement s : sections) {
            if (s.getID().equals("main")) {
                return s.getEElements();
            }
        }
        return null;
    }

    /**
     *
     * @return
     */
    public EElementList getAllEntries() {
        for (final SectionElement s : sections) {
            return s.getEElements();
        }
        return null;
    }

    /**
     *
     * @param sectionID
     * @return
     */
    public EElementList getEntries(final String sectionID) {
        for (final SectionElement s : sections) {
            if (s.getID().equals(sectionID)) {
                return s.getEElements();
            }
        }
        return null;
    }

    /**
     *
     * @param e
     */
    public final void addEElement(final EElement e) {
        (getEntries()).add(e);
    }

    /**
     *
     * @param e
     */
    public final void removeEElement(final EElement e) {
        (getEntries()).remove(e);
    }

    /**
     *
     *
     */
    public final void countEntries() {
        nForeignEntries = 0;
        nEntries = 0;
        nShared = 0;
        nDifferent = 0;
        final ArrayList<EElement> list = getEntries();
        for (final EElement e : list) {
            if (e.isForeign()) {
                nForeignEntries++;
            } else {
                if (!e.isForeign()) {
                    nEntries++;
                }
            }
            if (e.isShared()) {
                nShared++;
            } else {
                if (!e.isShared()) {
                    nDifferent++;
                }
            }
        }
    }

    /**
     *
     * @return
     */
    public final int getNForeignEntries() {
        return nForeignEntries;
    }

    /**
     *
     * @return
     */
    public final int getNEntries() {
        return nEntries;
    }

    /**
     *
     * @return
     */
    public final int getSharedEntries() {
        return nShared;
    }

    /**
     *
     * @return
     */
    public final int getDifferentEntries() {
        return nDifferent;
    }

    /**
     *
     *
     */
    public final void reportMetrics() {
        countEntries();
        System.err.println(nShared + " shared entries.");
        System.err.println(nDifferent + " not shared entries.");
        System.err.println(nEntries + " own entries.");
        System.err.println(nForeignEntries + " foreign entries.");
    }

    /**
     *
     *
     */
    public final void removeNotCommon() {
        final ArrayList<EElement> elements = getEntries();
        final ArrayList<EElement> elementsCopy = new ArrayList<EElement>(elements);

        for (final EElement e : elementsCopy) {
            if (!e.isShared()) {
                elements.remove(e);
            }
        }
    }

    /**
     *
     * @param parName
     * @return
     */
    public final PardefElement getParadigmDefinition(final String parName) {
        return pardefs.getParadigmDefinition(parName);
    }

    /**
     *
     * @param pardefE
     */
    public final void addParDef(final PardefElement pardefE) {
        pardefs.addPardefElement(pardefE);
    }

    /**
     *
     * @return
     */
    public PardefsElement getPardefsElement() {
        return pardefs;
    }

    /**
     *
     * @return
     */
    public final void searchEquivalentParadigms() {
        final DicEquivPar dicEquivPar = new DicEquivPar(this);
        equivPar = dicEquivPar.findEquivalentsA();
    }

    /**
     *
     * @return
     */
    public HashMap<String, ArrayList<PardefElement>> getEquivalentParadigms() {
        return equivPar;
    }

    /**
     *
     * @param entry
     * @return
     */
    public final EElement getEElement(String entry) {
        final ArrayList<EElement> elements = getEntries();

        for (final EElement e : elements) {
            final String lemma = e.getLemma();
            entry = DicTools.clearTags(entry);

            if (lemma != null) {
                if (lemma.equals(entry)) {
                    return e;
                }
            }
        }
        return null;
    }

    /**
     *
     *
     */
    public void reverse() {
        for (SectionElement section : getSections()) {
            final ArrayList<EElement> elements = section.getEElements();

            for (final EElement ee : elements) {
                final ArrayList<Element> children = ee.getChildren();

                if (ee.getRestriction() != null) {
                    if (ee.getRestriction().equals("LR")) {
                        ee.setRestriction("RL");
                    } else {
                        if (ee.getRestriction().equals("RL")) {
                            ee.setRestriction("LR");
                        }
                    }
                }

                for (final Element e : children) {
                    if (e instanceof PElement) {
                        LElement lE = ((PElement) e).getL();
                        RElement rE = ((PElement) e).getR();

                        // final String auxValue = lE.getValue();
                        ElementList auxChildren = lE.getChildren();

                        // lE.setValue(rE.getValue());
                        lE.setChildren(rE.getChildren());

                        // rE.setValue(auxValue);
                        rE.setChildren(auxChildren);

                        ((PElement) e).setLElement(lE);
                        ((PElement) e).setRElement(rE);
                    }
                }
            }
        }
    }

    /**
     * @return the folder
     */
    public final String getFolder() {
        return folder;
    }

    /**
     * @param folder
     *                the folder to set
     */
    public final void setFolder(String folder) {
        this.folder = folder;
    }

    public final String getXmlEncoding() {
        return xmlEncoding;
    }

    public final void setXmlEncoding(String xmlEncoding) {
        this.xmlEncoding = xmlEncoding;
    }

    public final String getXmlVersion() {
        return xmlVersion;
    }

    public final void setXmlVersion(String xmlVersion) {
        this.xmlVersion = xmlVersion;
    }
}