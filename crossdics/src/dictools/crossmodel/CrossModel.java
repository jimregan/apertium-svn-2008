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
import java.util.Collections;

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
	    dos.writeBytes("<!-- Examples of patterns not found -->\n");
	    dos.writeBytes("<cross-model>\n");
	    int i = 0;
	    CrossActionList cal = getCrossActions();
	    Collections.sort(cal);
	    for (CrossAction crossAction : cal) {
		crossAction.printXML(dos, i);
		i++;
	    }
	    dos.writeBytes("</cross-model>\n");
	    dos.writeBytes("<!-- " + i + " cross actions. -->\n");

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
