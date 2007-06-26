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

import dics.elements.dtd.DictionaryElement;

/**
 * 
 * @author Enrique Benimeli Bofarull
 *
 */
public class DicGather {

    /**
     * 
     */
    private String dic;
    
    /**
     * 
     */
    private String out;
    
    /**
     * 
     * @param dic
     * @param out
     */
    public DicGather(final String dic, final String out) {
	this.dic = dic;
	this.out = out;
    }
    
    /**
     * 
     *
     */
    public final void doGather() {
	DictionaryReader reader = new DictionaryReader(dic);
	DictionaryElement dic = reader.readDic();
	dic.printXML(out);
	
	System.out.println("Dictionary in a single file: '" + out + "'");
    }
}
