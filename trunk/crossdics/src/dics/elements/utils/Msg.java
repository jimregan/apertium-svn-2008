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

package dics.elements.utils;

import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import javax.swing.JLabel;

/**
 *
 * @author Enrique Benimeli Bofarull
 *
 */
public class Msg {

    /**
     *
     */
    private boolean debug;
    /**
     *
     */
    private String logFileName;
    /**
     *
     */
    private DataOutputStream log;
    /**
     *
     */
    private JLabel label;
    /**
     *
     */
    private int type = 0;
    /**
     *
     */
    static final int NORMAL = 0;
    /**
     *
     */
    static final int LABEL = 1;

    /**
     *
     *
     */
    public Msg(final String logFileName) {
        debug = false;
    }

    /**
     *
     */
    public Msg(JLabel label) {
        this.label = label;
        this.setType(this.LABEL);
    }

    /**
     *
     *
     */
    public Msg() {
        debug = false;
    }

    /**
     *
     * @param logFileName
     */
    private final void openLogStream(final String logFileName) {
        try {
            File file = new File(logFileName);
            FileOutputStream fos = new FileOutputStream(file);
            log = new DataOutputStream(fos);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     *
     * @param text
     */
    public final void err(final String text) {
        System.err.println(text);
    }

    /**
     *
     * @param text
     */
    public final void out(final String text) {

        switch (this.getType()) {
            case LABEL:
                label.setText(text);
                break;
            default:
                System.out.println(text);
                break;
        }
    }

    /**
     * 
     */
    public final void msg(final String text) {

        // Only for Java components (JLabel, etc)
        switch (this.getType()) {
            case LABEL:
                label.setText(text);
                break;
            default:
                break;
        }
    }

    /**
     *
     * @param text
     */
    public final void log(final String text) {
        if (isDebug()) {
            if (log == null) {
                openLogStream(logFileName);
            }
            try {
                log.writeBytes(text + "\n");
            } catch (IOException ioe) {
                System.err.println("Error writing log file " + getLogFileName());
            }
        }
    }

    /**
     * @return the debug
     */
    public final boolean isDebug() {
        return debug;
    }

    /**
     * @param debug
     *                the debug to set
     */
    public final void setDebug(boolean debug) {
        this.debug = debug;
    }

    /**
     * @return the logFileName
     */
    public final String getLogFileName() {
        return logFileName;
    }

    /**
     * @param logFileName
     *                the logFileName to set
     */
    public final void setLogFileName(String logFileName) {
        this.logFileName = logFileName;
        if (isDebug()) {
            openLogStream(logFileName);
        }
    }

    /**
     *
     */
    public final void setType(int t) {
        this.type = t;
    }

    /**
     *
     */
    public final int getType() {
        return type;
    }

    /**
     *
     */
    public final void setLabel(JLabel label) {
        this.label = label;
        this.type = this.LABEL;
    }
}