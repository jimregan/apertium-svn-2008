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

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;

import dics.elements.dtd.DictionaryElement;
import dics.elements.dtd.EElement;
import dics.elements.dtd.LElement;
import dics.elements.dtd.PardefElement;
import dics.elements.dtd.PardefsElement;
import dics.elements.dtd.RElement;
import dics.elements.dtd.SdefElement;
import dics.elements.dtd.SdefsElement;
import dics.elements.dtd.SectionElement;

/**
 * 
 * @author Enrique Benimeli Bofarull
 *
 */
public class DicReader {

    /**
     * 
     */
    private DictionaryElement dic;
    
    /**
     * 
     */
    private String action;
    
    /**
     * 
     */
    private boolean urlDic;
    
    /**
     * 
     */
    private String url;
    
    /**
     * 
     *
     */
    public DicReader(final String fileName) {
	DictionaryReader dicReader = new DictionaryReader(fileName);
	DictionaryElement dic = dicReader.readDic();
	setDic(dic);
    }

    public DicReader() {
	
    }

    /**
     * 
     *
     */
    public void getListOfParadigms() {
	DictionaryElement dic = this.getDic();
	PardefsElement paradigms = dic.getPardefsElement();
	
	for (PardefElement paradigm : paradigms.getPardefElements()) {
	    System.out.println(paradigm.getName());
	}
    }
    
    /**
     * 
     *
     */
    public void getListOfLemmas() {
	DictionaryElement dic = getDic();
	
	int nLemmas = 0;
	for (SectionElement section: dic.getSections()) {
	    for( EElement element: section.getEElements()) {
		if( element.getLemma() != null) {
		    System.out.println(element.getLemma());
		    nLemmas++;
		}
	    }
	}
	System.err.println("# " + nLemmas + " lemmas");
    }
    
    /**
     * 
     *
     */
    public final void getDefinitions() {
	DictionaryElement dic = getDic();
	
	SdefsElement sdefs = dic.getSdefs();
	
	for ( SdefElement sdef: sdefs.getSdefsElements()) {
	    System.out.println(sdef.getValue());    
	}
    }
    
    /**
     * 
     *
     */
    public final void getPairs() {
	DictionaryElement dic = getDic();
	
	int nLemmas = 0;
	for (SectionElement section: dic.getSections()) {
	    for( EElement element: section.getEElements()) {
		LElement left = element.getLeft();
		RElement right = element.getRight();

		String leftValue = left.getValueNoTags();
		String rightValue = right.getValueNoTags();

		if (!leftValue.equals("") && !rightValue.equals("")) {
		    System.out.println(leftValue + "/" + rightValue);
		}
		nLemmas++;
	    }
	}
	System.err.println("# " + nLemmas + " entries.");
    }
    
    /**
     * 
     *
     */
    public final void doit() {
	if (getAction().equals("list-paradigms")) {
	    getListOfParadigms();
	}
	if( getAction().equals("list-lemmas")) {
	    getListOfLemmas();
	}
	if( getAction().equals("list-definitions")) {
	    getDefinitions();
	}
	if( getAction().equals("list-pairs")) {
	    getPairs();
	}
	
    }
   

    /**
     * @return the dic
     */
    public final DictionaryElement getDic() {
        return dic;
    }

    /**
     * @param dic the dic to set
     */
    public final void setDic(DictionaryElement dic) {
        this.dic = dic;
    }

    /**
     * 
     * @param fileName
     */
    public final void setDic(final String fileName) {
	DictionaryReader dicReader = new DictionaryReader(fileName);
	DictionaryElement dic = dicReader.readDic();
	setDic(dic);
    }



    /**
     * @return the action
     */
    public final String getAction() {
        return action;
    }

    /**
     * @param action the action to set
     */
    public final void setAction(String action) {
        this.action = action;
    }

    /**
     * @return the urlDic
     */
    public final boolean isUrlDic() {
        return urlDic;
    }

    /**
     * @param urlDic the urlDic to set
     */
    public final void setUrlDic(boolean urlDic) {
        this.urlDic = urlDic;
    }

    /**
     * @return the url
     */
    public final String getUrl() {
        return url;
    }

    /**
     * @param url the url to set
     */
    public final void setUrl(String url) {
        this.url = url;
        try {
            URL theUrl = new URL(url);
            //BufferedReader in = new BufferedReader(new InputStreamReader(theUrl.openStream()));
            InputStream is = theUrl.openStream();
            DictionaryReader dicReader = new DictionaryReader();
            dicReader.setUrlDic(true);
            dicReader.setIs(is);
            DictionaryElement dic = dicReader.readDic();
            setDic(dic);
        } catch(MalformedURLException mfue) {
          
        } catch(IOException ioe) {

            
        }
    }


}
