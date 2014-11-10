
// Copyright (c) 2004-2005, Luc Maisonobe
// All rights reserved.
//
// Redistribution and use in source and binary forms, with
// or without modification, are permitted provided that
// the following conditions are met:
//
//    Redistributions of source code must retain the
//    above copyright notice, this list of conditions and
//    the following disclaimer.
//    Redistributions in binary form must reproduce the
//    above copyright notice, this list of conditions and
//    the following disclaimer in the documentation
//    and/or other materials provided with the
//    distribution.
//    Neither the names of spaceroots.org, spaceroots.com
//    nor the names of their contributors may be used to
//    endorse or promote products derived from this
//    software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
// USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
// IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

package com.smarthome.emulator.simulations.utilities;

import javax.print.*;
import javax.print.attribute.HashPrintRequestAttributeSet;
import javax.print.attribute.PrintRequestAttributeSet;
import javax.print.event.PrintJobEvent;
import javax.print.event.PrintJobListener;
import javax.swing.*;
import javax.swing.event.EventListenerList;
import javax.swing.text.*;
import java.awt.*;
import java.awt.font.LineMetrics;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;
import java.awt.print.PageFormat;
import java.awt.print.Printable;
import java.util.ArrayList;
import java.util.EventListener;
import java.util.EventObject;

/** This class handles printing of
 * <code>javax.swing.text.StyledDocument</code> instances.

 * <p>Lines are laid out regularly spaced vertically according to the
 * end of line separator (which is guessed automatically if not
 * specified using the {@link #setEndOfLineMarker setEndOfLineMarker}
 * method). Line endings can occur either at styles crossings or
 * embedded within spans of text using the same style. There is no
 * line wrapping at all (only line endings are used to layout the
 * lines).

 * <p>Not all style attributes defined in
 * <code>javax.swing.text.StyleConstants</p> are handled yet. Here is
 * a list of supported features:</p>
 * <ul>
 *   <li>all attributes directly induced by the font choice
 *   (<code>FontFamily</code>, <code>FontSize</code>,
 *   <code>Bold,</code> <code>Italic</code>)</li>
 *   <li><code>Underline</code></li>
 *   <li><code>StrikeThrough</code></li>
 *   <li><code>Foreground</code></li>
 *   <li><code>Background</code></li>
 * </ul>

 * @version $Id$
 * @author Luc Maisonobe
 */
public class StyledDocumentPrinter {

    /** Event triggered when a print job is completed. */
    public static class PrintCompletedEvent extends EventObject {

        private PrintCompletedEvent(StyledDocumentPrinter documentPrinter,
                                    PrintException pe) {
            super(documentPrinter);
            this.pe = pe;
        }

        /** Get the exception that was triggered during the print job.
         * @return an exception, or null if everything went well
         */
        public PrintException getPrintException() {
            return pe;
        }

        private PrintException pe;

    }

    /** Listener for end of printing notification.
     * @see #addPrintCompletedListener
     */
    public interface PrintCompletedListener
            extends EventListener {
        /** Invoked when a print has been completed.
         * @param event print completed event
         */
        public void printCompleted(PrintCompletedEvent event);
    }

    /** Build a printer for a styled document.
     * @param document document to print
     */
    public StyledDocumentPrinter(StyledDocument document) {
        this.document        = document;
        this.endOfLineMarker = null;
        listeners            = new EventListenerList();
    }

    /** Add a listener for <code>PrintCompleted</code> events.
     * @param l listener to add
     */
    public void addPrintCompletedListener(PrintCompletedListener l) {
        listeners.add(PrintCompletedListener.class, l);
    }

    public void removePrintCompletedListener(PrintCompletedListener l) {
        listeners.remove(PrintCompletedListener.class, l);
    }

    /** Notify all listeners that have registered interest.
     * @param pe if non-null, reference to an exception that was
     * triggered during the print job
     */
    private void firePrintCompleted(PrintException pe) {
        PrintCompletedEvent event = null;
        Object[] array = listeners.getListenerList();
        for (int i = array.length - 2; i >= 0; i -= 2) {
            if (array[i] == PrintCompletedListener.class) {
                if (event == null) {
                    // lazily create the event
                    event = new PrintCompletedEvent(this, pe);
                }
                ((PrintCompletedListener) array[i + 1]).printCompleted(event);
            }
        }
    }

    /** Set up the end of line marker.
     * <p>If this method is not called, the end of line marker will be
     * guessed at print time.</p>
     * @param endOfLineMarker end of line marker to use
     */
    public void setEndOfLineMarker(String endOfLineMarker) {
        this.endOfLineMarker = endOfLineMarker;
    }

    /** Print the document.
     * <p>Printing is done in a devoted thread, so this methods returns
     * immediately.</p>
     * @param attributeSet set of attributes used to select the print
     * service (may be null)
     */
    public void printDocument(PrintRequestAttributeSet attributeSet) {
        new Worker(attributeSet).start();
    }

    /** Underlying printing class.
     * <p>The print action is implemented by a private internal class to
     * use a devoted thread for the printing and to hide the
     * <code>Printable</code> and <code>PrintJobListener</code>
     * implemented interfaces.</p>
     */
    private class Worker
            extends Thread implements Printable, PrintJobListener {

        public Worker(PrintRequestAttributeSet attributeSet) {
            printing = false;
            this.attributeSet = new HashPrintRequestAttributeSet(attributeSet);
        }

        public void run() {
            PrintException exception = null;
            try {

                DocFlavor flavor = DocFlavor.SERVICE_FORMATTED.PRINTABLE;
                PrintService services[]  =
                        PrintServiceLookup.lookupPrintServices(flavor, null);
                PrintService defaultService =
                        PrintServiceLookup.lookupDefaultPrintService();
                PrintService service =
                        ServiceUI.printDialog(null, 200, 200, services, defaultService,
                                flavor, attributeSet);
                if (service != null) {
                    DocPrintJob job = service.createPrintJob();
                    job.addPrintJobListener(this);
                    printing = true;
                    lines    = null;
                    job.print(new SimpleDoc(this, flavor, null), attributeSet);
                    synchronized(this) {
                        while (printing) {
                            try {
                                wait();
                            } catch (InterruptedException ie) {
                            }
                        }
                    }
                }

            } catch (PrintException pe) {
                // store the exception for the listeners
                exception = pe;
            }

            /** Notify the listeners of the print job end. */
            final PrintException finalException = exception;
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    firePrintCompleted(finalException);
                }
            });

        }

        /** Print the document, according to its embedded styles.
         * @param g printing graphic context
         * @param format page format
         * @param index index of the page to print
         * @return code indicating if the requested page exists or not
         */
        public int print(Graphics g, PageFormat format, int index) {

            try {

                g2 = (Graphics2D) g;

                if (lines == null) {
                    lines = new ArrayList();
                    ascent = -1.0f;
                    height = -1.0f;
                    String eol = endOfLineMarker;
                    if (eol == null) {
                        String text = document.getText(0, document.getLength());
                        eol = identifyEndOfLineMarker(text);
                    }
                    int lastStart = analyzeLines(document.getDefaultRootElement(),
                            0, eol);
                    if (lastStart < document.getLength()) {
                        // the last line is not ended by a line separator
                        // nevertheless, we must not forget it
                        lines.add(new int[] { lastStart, document.getLength() });
                    }
                }

                int pageLines = (int) Math.floor(format.getImageableHeight() / height);
                if ((height < 0) || (lines.size() < (index * pageLines))) {
                    return Printable.NO_SUCH_PAGE;
                }

                // print the lines belonging to the specified page
                current = new Point2D.Float((float) format.getImageableX(),
                        (float) (format.getImageableY() + ascent));
                for (int i = index * pageLines;
                     i < Math.min(lines.size(), (index + 1) * pageLines);
                     ++i) {
                    current.x = (float) format.getImageableX();
                    printText(((int[]) lines.get(i))[0], ((int[]) lines.get(i))[1]);
                    current.y += height;
                }

                return Printable.PAGE_EXISTS;

            } catch (BadLocationException ble) {
                return Printable.NO_SUCH_PAGE;
            }
        }

        /** Print a span of text.
         * @param start start index of the span of text to print
         * @param end end index of the span of text to print
         * @exception BadLocationException if some position is wrong
         */
        private void printText(int start, int end)
                throws BadLocationException {

            while (start < end) {

                Element      element = document.getCharacterElement(start);
                int          next    = Math.min(end, element.getEndOffset());
                AttributeSet as      = element.getAttributes();
                String       text    = document.getText(start, next - start);

                g2.setFont(document.getFont(as));
                Rectangle2D rect = g2.getFontMetrics().getStringBounds(text, g2);

                Color background = document.getBackground(as);
                if (! background.equals(Color.WHITE)) {
                    // draw the background only if it is not the default white one
                    g2.setColor(background);
                    rect.setRect(current.x, current.y - ascent,
                            rect.getWidth(), rect.getHeight());
                    g2.fill(rect);
                }

                g2.setColor(document.getForeground(as));
                g2.drawString(text, current.x, current.y);

                if (StyleConstants.isUnderline(as)) {
                    g2.drawLine((int) current.x,
                            (int) current.y + 2,
                            (int) (current.x + rect.getWidth()),
                            (int) current.y + 2);
                }

                if (StyleConstants.isStrikeThrough(as)) {
                    g2.drawLine((int) current.x,
                            (int) (current.y - ascent + 2 + rect.getHeight() / 2),
                            (int) (current.x + rect.getWidth()),
                            (int) (current.y - ascent + 2 + rect.getHeight() / 2));
                }

                // prepare for next element in the ktext span
                current.x += (float) rect.getWidth();
                start = next;

            }

        }

        /** Gather height informations from all styles.
         * <p>Recursively analyze all document to identify the lines and
         * the maximal ascent and height encountered along these lines</p>
         * @param element styled element to analyze
         * @param startLine offset of the start of the current line
         * @param eol end of line marker
         * @return start of the last line still under analysis
         * @exception BadLocationException if some position is wrong
         */
        private int analyzeLines(Element element, int startLine, String eol)
                throws BadLocationException {

            if (element.getElementCount() == 0) {

                // analyze a leaf element
                AttributeSet attributes = element.getAttributes();
                Font         font       = document.getFont(attributes);
                int          start      = element.getStartOffset();
                if (start >= document.getLength()) {
                    // this is the implied final break added by AbstractDocument,
                    // we simply ignore it
                    return startLine;
                }

                // the additional space is here to compute the height of empty lines
                String text = document.getText(start, element.getEndOffset() - start)
                        + " ";
                for (int index = 0; index < text.length();) {

                    int next = text.indexOf(eol, index);
                    if (next < 0) {
                        next = text.length();
                    } else {
                        // there is a line ending within the element
                        lines.add(new int[] { startLine, start + next });
                        startLine = start + next + eol.length();
                    }

                    if (next > index) {
                        LineMetrics metrics =
                                font.getLineMetrics(text.substring(index, next),
                                        g2.getFontRenderContext());
                        ascent = (float) Math.max(ascent, metrics.getAscent());
                        height = (float) Math.max(height, metrics.getHeight());
                    }

                    index = next + eol.length();

                }

            } else {

                // recurse deeper in the elements tree
                for (int i = 0; i < element.getElementCount(); ++i) {
                    startLine = analyzeLines(element.getElement(i), startLine, eol);
                }

            }

            return startLine;

        }

        /** Identify the end of line marker.
         * <p>The end of line marker depends from the operating system that
         * created the text file beeing parsed. It is <code>\n</code> for
         * UNIX, <code>\r</code> for MAC and <code>\n\r</code> for
         * DOS/Windows.</p>
         * @param text sample text on which end of line marker will be guessed
         * @return the guessed end of line marker
         */
        private String identifyEndOfLineMarker(String text) {

            // counters for each type of end of line marker
            int nbN  = 0;
            int nbNR = 0;
            int nbR  = 0;

            // walk through the beginning of the text
            // (only the first 10000 characters or 50 lines are scanned)
            for (int i = 0;
                 (i < Math.min(text.length(), 10000))
                         && (Math.max(Math.max(nbN, nbNR), nbR) < 50);
                 ++i) {
                if (text.charAt(i) == '\n') {
                    if ((i < text.length() - 1) && (text.charAt(i + 1) == '\r')) {
                        ++i;
                        ++nbNR;
                    } else {
                        ++nbN;
                    }
                } else if (text.charAt(i) == '\r') {
                    ++nbR;
                }
            }

            if ((nbN == 0) && (nbNR == 0) && (nbR == 0)) {
                // use system default
                return System.getProperty("line.separator");
            } else if (nbN >= Math.max(nbNR, nbR)) {
                // Unix-like end of lines
                return new String("\n");
            } else if (nbNR >= Math.max(nbN, nbR)) {
                // DOS-like end of lines
                return new String("\n\r");
            } else {
                // Mac-like end of lines
                return new String("\r");
            }

        }

        private void completed() {
            synchronized(this) {
                printing = false;
                notify();
            }
        }

        public void printDataTransferCompleted(PrintJobEvent pje) {
            completed();
        }

        public void printJobCanceled(PrintJobEvent pje) {
            completed();
        }

        public void printJobCompleted(PrintJobEvent pje) {
            completed();
        }

        public void printJobFailed(PrintJobEvent pje) {
            completed();
        }

        public void printJobNoMoreEvents(PrintJobEvent pje) {
        }

        public void printJobRequiresAttention(PrintJobEvent pje) {
        }

        /** Print indicator. */
        private boolean printing;

        /** Attributes used for the print job. */
        private PrintRequestAttributeSet attributeSet;

        /** Graphical context. */
        private Graphics2D g2;

        /** Maximal line ascent observed on all document. */
        private float ascent;

        /** Maximal line height observed on all document. */
        private float height;

        /** Lines array. */
        private ArrayList lines;

        /** Current print position. */
        private Point2D.Float current;

    }

    /** Document to print. */
    private StyledDocument document;

    /** End of line marker. */
    private String endOfLineMarker;

    /** Event listeners list. */
    private EventListenerList listeners;

}
