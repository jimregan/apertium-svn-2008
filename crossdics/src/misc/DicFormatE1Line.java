

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
	    dos.writeBytes("<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n");
	    dos.writeBytes("<!--\n\tDictionary:\n");
	    if (dic.getSections() != null) {
		if (dic.isBil()) {
		    dos.writeBytes("\tBilingual dictionary: " + dic.getSL() + "-"
			    + dic.getTL() + "\n");
		}
		dos.writeBytes("\tSections: " + dic.getSections().size() + "\n");
		int ne = 0;
		for (SectionElement section : dic.getSections()) {
		    ne += section.getEElements().size();
		}
		dos.writeBytes("\tEntries: " + ne);
	    }

	    if (dic.getSdefs() != null) {
		dos.writeBytes("\n\tSdefs: " + dic.getSdefs().getSdefsElements().size()
			+ "\n");
	    }
	    if (dic.getPardefsElement() != null) {
		dos.writeBytes("\tParadigms: "
			+ dic.getPardefsElement().getPardefElements().size() + "\n");
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
			    //e.printXML(dos);
			    e.printXML1Line(dos);
			}
			dos.writeBytes("  </section>\n");
		    //s.printXML(dos);
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
