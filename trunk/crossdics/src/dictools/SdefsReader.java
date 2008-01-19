package dictools;

import org.w3c.dom.Element;

import dics.elements.dtd.SElement;
import dics.elements.dtd.SdefElement;
import dics.elements.dtd.SdefsElement;

/**
 * 
 * @author Enrique Benimeli Bofarull
 * 
 */
public class SdefsReader extends XMLReader {

    /**
         * 
         * @param fileName
         */
    public SdefsReader(final String fileName) {
	super(fileName);
    }

    /**
         * 
         * @return
         */
    public SdefsElement readSdefs() {
	analize();
	Element root = getDocument().getDocumentElement();
	String elementName = root.getNodeName();
	SdefsElement sdefsElement = null;

	// Symbol definitions
	if (elementName.equals("sdefs")) {
	    sdefsElement = readSdefs(root);
	}

	root = null;
	setDocument(null);
	return sdefsElement;
    }

    /**
         * 
         * @param e
         * @return
         */
    public SdefsElement readSdefs(final Element e) {
	final SdefsElement sdefsElement = new SdefsElement();

	for (final Element childElement : readChildren(e)) {
	    final String childElementName = childElement.getNodeName();
	    if (childElementName.equals("sdef")) {
		final SdefElement sdefElement = readSdef(childElement);
		final SElement sE = SElement.get(sdefElement.getValue());
		sdefsElement.addSdefElement(sdefElement);
	    }
	}

	return sdefsElement;
    }

    /**
         * 
         * @param e
         * @return
         */
    public SdefElement readSdef(final Element e) {
	final String n = getAttributeValue(e, "n");
	final String c = getAttributeValue(e, "c");
	final SdefElement sdefElement = new SdefElement(n);
	sdefElement.setComment(c);
	return sdefElement;
    }

}
