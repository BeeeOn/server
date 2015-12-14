package com.smarthome.emulator.simulations.basis;

import com.smarthome.emulator.Emulator;
import com.smarthome.emulator.simulations.Error;
import com.smarthome.emulator.simulations.forms.LogFileViewer;
import com.smarthome.emulator.simulations.utilities.StyledDocumentPrinter;

import javax.print.attribute.HashPrintRequestAttributeSet;
import javax.print.attribute.PrintRequestAttributeSet;
import javax.print.attribute.standard.JobName;
import javax.swing.*;
import javax.swing.border.LineBorder;
import javax.swing.text.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.*;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

/**
 * Creates and writes to three types of logs. Also stores server response time.
 */
public class Logger {
    /** {@link javax.swing.JScrollPane} for "log" test area */
    private JScrollPane logScroller;
    /** {@link javax.swing.JScrollPane} for "to be sent log" test area */
    private JScrollPane toBeSentScroller;
    /** {@link javax.swing.JScrollPane} for "error log" test area */
    private JScrollPane errorScroller;
    /** {@link javax.swing.JTextPane}  for "log" */
    private JTextPane logTextArea;
    /** {@link javax.swing.JTextPane}  for "to be sent log" */
    private JTextPane toBeSentTextArea;
    /** {@link javax.swing.JTextPane}  for "error log" */
    private JTextPane errorTextArea;
    /**
     * not visible print text area
     */
    private JTextPane printTextArea;
    /** {@link javax.swing.JScrollBar}  for "log" */
    private JScrollBar logVerticalScrollBar;
    /** {@link javax.swing.JScrollBar}  for "to be sent log" */
    private JScrollBar toBeSentVerticalScrollBar;
    /** {@link javax.swing.JScrollBar}  for "error log" */
    private JScrollBar errorVerticalScrollBar;
    /** "log text area regular text style */
    private Style logStyle;
    /** "log text area bold text style */
    private Style boldLogStyle;
    /** "log text area regular text style */
    private Style toBeSentStyle;
    /** "log text area regular text style */
    private Style errorStyle;
    /** "log" document containing text */
    private StyledDocument logDoc;
    /** "to be sent log" document containing text */
    private StyledDocument toBeSentDoc;
    /** "error log" document containing text */
    private StyledDocument errorDoc;
    /**
     * document to be printed
     */
    private StyledDocument printDoc;
    /** indicator of saving response time */
    private boolean trackResponse = false;
    /** response information */
    private ArrayList<Response> responses = null;
    /** indicator of dumping saved responses */
    private boolean dumpResponses = false;
    /** time format for writing into logs */
    SimpleDateFormat ft =new SimpleDateFormat("HH:mm:ss");
    /** Bold text style number */
    public static final int BOLD_STYLE = 1;
    /** Regular text style number */
    public static final int REGULAR_STYLE = 0;

    public static final int LOG_TAB_NUMBER = 0;
    public static final int TO_BE_SENT_TAB_NUMBER = 1;
    public static final int ERROR_TAB_NUMBER = 2;

    /**
     * array of text styles for printing
     */
    private Style[] PRINT_STYLES = new Style[2];
    /** automatic scrolling of "log" indicator */
    private boolean scrollLog = true;
    private boolean buffered = false;
    private long max_per_second_time = 0L;
    private int max_per_second = 0;
    private int max_per_second_counter = 0;

    private File file = null;
    private BufferedWriter writer = null;

    private int logLinesCounter = 0;
    private int writeMark = 0;
    /**
     * Creates new text areas for {@link javax.swing.JScrollPane}'s given as parameters. Messages will be later writen
     * to these text areas
     * @param logScroller <code>JScrollPane</code> for log
     * @param toBeSentScroller <code>JScrollPane</code> for to be sent log
     * @param errorScroller <code>JScrollPane</code> for error log
     */
    public Logger(JScrollPane logScroller, JScrollPane toBeSentScroller, JScrollPane errorScroller,boolean buffered){
        this.logScroller = logScroller;
        this.toBeSentScroller = toBeSentScroller;
        this.errorScroller = errorScroller;
        this.buffered = buffered;
        createLogTextArea();
        createToBeSentTextArea();
        createErrorTextArea();
        createPrintTextArea();
        createBuffer();
    }
    /**
     * Removes text area created in constructor from {@link javax.swing.JScrollPane}'s view ports.
     */
    public void delete(){
        errorScroller.setViewportView(null);
        logScroller.setViewportView(null);
        toBeSentScroller.setViewportView(null);
        errorScroller.validate();
        logScroller.validate();
        toBeSentScroller.validate();
        deleteBuffer();
    }
    private void createBuffer(){
        if(buffered){
            try {
                file = File.createTempFile("temp_log",".tmp");
                writer = new BufferedWriter(new FileWriter(file));
            }catch (IOException e){
                //TODO proper error
                System.out.println("Cannot create temp file");
            }
        }
    }
    /**
     * Merges all logs into one document and after confirming print dialog prints this document.
     */
    public void printLogs(String header){
        Boolean success = getPrintDoc(printDoc,header,PRINT_STYLES);
        if(success){
            StyledDocumentPrinter printer = new StyledDocumentPrinter(printDoc);
            printer.addPrintCompletedListener(new StyledDocumentPrinter.PrintCompletedListener() {
                @Override
                public void printCompleted(StyledDocumentPrinter.PrintCompletedEvent event) {
                    clearPrinterDoc();
                }
            });
            PrintRequestAttributeSet set = new HashPrintRequestAttributeSet();
            set.add(new JobName("Performance_simulation_logs_" + ft.format(new Date()), null));
            printer.printDocument(set);
        }
    }

    public void deleteBuffer(){
        if(buffered && writer!= null && file != null){
            try {
                writer.close();
                if(!file.delete()){
                    System.out.println("Cannot delete temp file");
                    file.deleteOnExit();
                }
                System.out.println("Closing and deleting temp file");
            } catch (IOException e) {
                //TODO proper error
                System.out.println("Cannot close temp file");
            }
        }
    }

    public void clearBuffer(){
        deleteBuffer();
        createBuffer();
    }
    /**
     * Adds new {@link com.smarthome.emulator.simulations.basis.Response} to {@link com.smarthome.emulator.simulations.basis.Logger#responses} list.
     * Sets response value as <code>start - end</code> and time of response as <code>end</code>
     * @param start
     * @param end
     */
    public synchronized void addResponse(long start,long end){
        if(start!=0 && start < end){
            Response response = new Response(start,end);
            responses.add(response);
            /*if(max_per_second_time != response.getTime()){
                if(max_per_second < max_per_second_counter){
                    max_per_second = max_per_second_counter;
                    System.out.println("New Max: " + max_per_second);
                }
                max_per_second_time = response.getTime();
                max_per_second_counter = 0;
            }
            else{
                max_per_second_counter++;
            }*/
        }
    }
    /**
     * If there are any responses in {@link com.smarthome.emulator.simulations.basis.Logger#responses}, returns value of last one, else returns <code>0L</code>
     * @return last response value or <code>0L</code>
     */
    public synchronized Long getLastResponseValue(){
        if(responses.size() != 0)
            return responses.get(responses.size()-1).getValue();
        else
            return 0L;
    }
    /**
     * Clears {@link com.smarthome.emulator.simulations.basis.Logger#responses} list
     */
    public void dumpResponses(){
        responses.clear();
    }
    /**
     * Returns new {@link java.util.ArrayList} of {@link com.smarthome.emulator.simulations.basis.Response}s saved since last call of this method,
     * then clears {@link com.smarthome.emulator.simulations.basis.Logger#responses} list
     * @return responses saved since last call of this method
     */
    public ArrayList<Response> getNewResponses(){
        ArrayList<Response> newResponses = new ArrayList<>();
        for(int i = 0;i<responses.size();i++){
            newResponses.add(responses.get(i));
        }
        responses.clear();
        return newResponses;
    }
    /**
     * Clears all logs text areas
     * <p>
     *     Shows errors:
     *     <ul>
     *         <li>LOGGER_CLEAR if failed to clear logs</li>
     *     </ul>
     * </p>
     */
    public void clearAllLogs(){
        try {
            toBeSentDoc.remove(0,toBeSentDoc.getLength());
            logDoc.remove(0,logDoc.getLength());
            errorDoc.remove(0,errorDoc.getLength());
        } catch (BadLocationException e) {
            Emulator.showError(Error.LOGGER_CLEAR.getMessage());
        }
    }
    /**
     * Gets texts from all logs and puts them into one {@link javax.swing.text.StyledDocument} given as parameter.
     * <p>
     *     Shows errors:
     *     <ul>
     *         <li>LOGGER_PRINT if failed to merge logs into one</li>
     *     </ul>
     * </p>
     * @param printDoc document to store all logs in
     * @param title title of document
     * @param styles array of styles to use (on <code>0</code> index regular style, on <code>1</code> index bold style)
     * @return <code>true</code> if document merging successful, <code>false</code> otherwise
     */
    public boolean getPrintDoc(StyledDocument printDoc,String title,Style[] styles){
        BufferedReader reader = null;
        try {
            //clean print document
            printDoc.remove(0,printDoc.getLength());
            //add caption
            printDoc.insertString(0, title + "\n\n", styles[BOLD_STYLE]);
            //add caption
            printDoc.insertString(printDoc.getLength(),"Log\n", styles[BOLD_STYLE]);
            //add log document
            if(buffered){
                reader = new BufferedReader(new FileReader(file));
                String currentLine;
                while ((currentLine = reader.readLine()) != null) {
                    printDoc.insertString(printDoc.getLength(), currentLine + "\n", styles[REGULAR_STYLE]);
                }
            }
            printDoc.insertString(printDoc.getLength(),logDoc.getText(0,logDoc.getLength()), styles[REGULAR_STYLE]);
            //add caption
            printDoc.insertString(printDoc.getLength(),"To be sent Log\n",styles[BOLD_STYLE]);
            //add to be sent document
            printDoc.insertString(printDoc.getLength(),toBeSentDoc.getText(0,toBeSentDoc.getLength()), styles[REGULAR_STYLE]);
            //add caption
            printDoc.insertString(printDoc.getLength(),"Error Log\n", styles[BOLD_STYLE]);
            //add error document
            printDoc.insertString(printDoc.getLength(),errorDoc.getText(0,errorDoc.getLength()), styles[REGULAR_STYLE]);
            return true;
        } catch (BadLocationException e) {
            Emulator.showError(Error.LOGGER_PRINT.getMessage());
            return false;
        } catch (FileNotFoundException e) {
            System.out.println("Cannot find temp file!");
            return false;
        } catch (IOException e) {
            System.out.println("Cannot access temp file!");
            return false;
        }finally {
            if (reader != null) {
                try {
                    reader.close();
                } catch (IOException e) {
                    System.out.println("Cannot reader of temp file!");
                }
            }
        }
    }
    /**
     * Writes message given as parameter to "log" text area in text style given as parameters (<code>0</code> regular, <code>1</code> bold).
     * <p>
     *     Shows errors:
     *     <ul>
     *         <li>LOGGER_WRITE_LOG if failed to write to log</li>
     *     </ul>
     * </p>
     * @param msg message to be written
     * @param style style in which should be message written (<code>0</code> regular, <code>1</code> bold)
     */
    public synchronized void writeLogMessage(String msg,int style){
        try {
            if(buffered){
                //TODO change magic number
                if(writeMark == 0 && logLinesCounter > 20){
                    writeMark = logDoc.getLength();
                }
                if(writer!=null && logLinesCounter > 40){
                    writer.write(logDoc.getText(0, writeMark));
                    writer.flush();
                    logDoc.remove(0, writeMark);
                    writeMark = 0;
                    logLinesCounter = 0;
                }
            }
            if(style == REGULAR_STYLE)
                logDoc.insertString(logDoc.getLength(),ft.format(new Date()) + " - " + msg + "\n", logStyle);
            else if(style == BOLD_STYLE)
                logDoc.insertString(logDoc.getLength(),ft.format(new Date()) + " - " + msg + "\n", boldLogStyle);
            logLinesCounter++;
            //scroll the scrollbar after adding text
            if(getScrollLog()) logVerticalScrollBar.setValue(logVerticalScrollBar.getMaximum());
        } catch (BadLocationException e) {
            Emulator.showError(Error.LOGGER_WRITE_LOG.getMessage());
        } catch (IOException e) {
            System.out.println("Cannot write to temp file");
        }
    }
    /**
     * Removes first line from "to be sent log"
     */
    public synchronized void removeFirstLineFromToBeSent(){
        Element first = toBeSentDoc.getDefaultRootElement().getElement(0);
        try {
            toBeSentDoc.remove(first.getStartOffset(),first.getEndOffset());
        } catch (BadLocationException e) {
            writeErrorMessage(Error.LOGGER_SENT_FIRST_LINE.getMessage());
        }
    }
    /**
     * Writes message given as parameter to "to be sent log" text area
     * <p>
     *     Shows errors:
     *     <ul>
     *         <li>LOGGER_WRITE_SENT if failed to write to log</li>
     *     </ul>
     * </p>
     * @param msg message to be written
     */
    public synchronized void writeToBeSentMessage(String msg){
        try {
            toBeSentDoc.insertString(toBeSentDoc.getLength(), ft.format(new Date()) + " - " + msg + "\n", toBeSentStyle);
            //scroll the scrollbar after adding text
            //toBeSentVerticalScrollBar.setValue(toBeSentVerticalScrollBar.getMaximum());
        } catch (BadLocationException e) {
            Emulator.showError(Error.LOGGER_WRITE_SENT.getMessage());
        }
    }
    /**
     * Writes message given as parameter to "error log" text area.
     * <p>
     *     Shows errors:
     *     <ul>
     *         <li>LOGGER_WRITE_ERROR if failed to write to log</li>
     *     </ul>
     * </p>
     * @param msg message to be written
     */
    public synchronized void writeErrorMessage(String msg){
        try {
            errorDoc.insertString(errorDoc.getLength(),ft.format(new Date()) + " - " + msg + "\n", errorStyle);
            //scroll the scrollbar after adding text
            if(getScrollLog()) errorVerticalScrollBar.setValue(errorVerticalScrollBar.getMaximum());
        } catch (BadLocationException e) {
            Emulator.showError(Error.LOGGER_WRITE_ERROR.getMessage());
        }

    }
    /**
     * Sets text areas created in constructor as view ports of given {@link javax.swing.JScrollPane}s
     */
    public void setActiveViewPorts(){
        logScroller.setViewportView(logTextArea);
        toBeSentScroller.setViewportView(toBeSentTextArea);
        errorScroller.setViewportView(errorTextArea);
    }
    /**
     * Returns if logger saves response times from server
     * @return <code>true</code> if logger saves responses, <code>false</code> otherwise
     */
    public boolean isTrackResponse(){
        return this.trackResponse;
    }
    /**
     * Sets whether responses should be saved or not. Creates list of responses if there is not one.
     * @param b <code>true</code> if save responses, <code>false</code> otherwise
     */
    public void setTrackResponse(boolean b){
        if(b && responses == null)
            responses = new ArrayList<>();
        this.trackResponse = b;
    }
    /**
     * Returns whether there are responses stored or not.
     * @return <code>false</code> if responses are saved, <code>true</code> otherwise
     */
    public boolean isDumpResponses(){
        return this.dumpResponses;
    }
    /**
     * Sets if responses should be stored or not
     * @param b <code>false</code> if save responses, <code>true</code> otherwise</code>
     */
    public void setDumpResponses(boolean b){
        this.dumpResponses = b;
    }

    /**
     * Returns indicator of automatic scrolling of scrollbar
     * @return <code>true</code> if automatic scrolling is on, <code>false</code> otherwise
     */
    public boolean getScrollLog(){
        return this.scrollLog;
    }
    /**
     * Sets automatic "log" scrolling
     * @param b <code>true</code> automatic scrolling on, <code>false</code> otherwise
     */
    public void setScrollLog(boolean b){
        this.scrollLog = b;
    }
    public File getFile(){
        return this.file;
    }
    /**
     * Clears print document
     * <p>
     * Shows errors:
     * <ul>
     * <li>PRINT_CLEAR_DOC if print document cannot be cleared</li>
     * </ul>
     * </p>
     */
    private void clearPrinterDoc(){
        try {
            printDoc.remove(0, printDoc.getLength());
        } catch (BadLocationException e) {
            Emulator.showError(Error.PRINT_CLEAR_DOC.getMessage());
        }
    }
    /**
     * Creates {@link javax.swing.JTextPane} for "error log"
     */
    private void createErrorTextArea(){
        errorTextArea = new JTextPane();
        errorTextArea.setBackground(new Color(-1));
        errorTextArea.setEditable(false);
        errorTextArea.setForeground(new Color(-12303292));
        errorTextArea.setVisible(true);
        errorTextArea.putClientProperty("charset", "UTF8");
        errorScroller.setViewportView(errorTextArea);
        createErrorTextAreaDocument();
        errorScroller.validate();
        errorVerticalScrollBar = errorScroller.getVerticalScrollBar();
    }
    /**
     * Creates {@link javax.swing.JTextPane} for "to be sent log"
     */
    private void createToBeSentTextArea(){
        toBeSentTextArea = new JTextPane();
        toBeSentTextArea.setBackground(new Color(-1));
        toBeSentTextArea.setEditable(false);
        toBeSentTextArea.setForeground(new Color(-12303292));
        toBeSentTextArea.setVisible(true);
        toBeSentTextArea.putClientProperty("charset", "UTF8");
        toBeSentScroller.setViewportView(toBeSentTextArea);
        createToBeSentTextAreaDocument();
        toBeSentScroller.validate();
        toBeSentVerticalScrollBar = toBeSentScroller.getVerticalScrollBar();
    }
    /**
     * Creates {@link javax.swing.JTextPane} for "log"
     */
    private void createLogTextArea(){
        logTextArea = new JTextPane();
        logTextArea.setBackground(new Color(-1));
        logTextArea.setEditable(false);
        logTextArea.setForeground(new Color(-12303292));
        logTextArea.setVisible(true);
        logTextArea.putClientProperty("charset", "UTF8");
        logScroller.setViewportView(logTextArea);
        createLogTextAreaDocument();
        logScroller.validate();
        logVerticalScrollBar = logScroller.getVerticalScrollBar();
        logTextArea.setInheritsPopupMenu(true);
    }
    /**
     * Creates {@link javax.swing.text.StyledDocument} for "log"
     */
    private void createLogTextAreaDocument(){
        logDoc = (StyledDocument) logTextArea.getDocument();
        logStyle = logDoc.addStyle("logDocument",null);
        boldLogStyle = logDoc.addStyle("boldLogDocument",null);
        StyleConstants.setBackground(logStyle,Color.WHITE);
        StyleConstants.setBackground(boldLogStyle,Color.WHITE);
        StyleConstants.setBold(boldLogStyle,true);
    }
    /**
     * Creates {@link javax.swing.text.StyledDocument} for "to be sent log"
     */
    private void createToBeSentTextAreaDocument(){
        toBeSentDoc = (StyledDocument) toBeSentTextArea.getDocument();
        toBeSentStyle = toBeSentDoc.addStyle("logDocument",null);
        StyleConstants.setBackground(toBeSentStyle,Color.WHITE);
    }
    /**
     * Creates {@link javax.swing.text.StyledDocument} for "error log"
     */
    private void createErrorTextAreaDocument(){
        errorDoc = (StyledDocument) errorTextArea.getDocument();
        errorStyle = errorDoc.addStyle("logDocument",null);
        StyleConstants.setBackground(errorStyle,Color.WHITE);
    }
    /**
     * Creates not visible text area for print document
     */
    private void createPrintTextArea(){
        printTextArea = new JTextPane();
        printTextArea.setBackground(new Color(-1));
        printTextArea.setEditable(false);
        printTextArea.setForeground(new Color(-12303292));
        printTextArea.setVisible(false);
        printTextArea.putClientProperty("charset", "UTF8");
        createPrintDocument();
    }
    /**
     * Creates document for print text area and assigns text styles to it ( <code>regular</code> , <code>bold</code> )
     */
    private void createPrintDocument(){
        printDoc = (StyledDocument) printTextArea.getDocument();
        PRINT_STYLES[0] = printDoc.addStyle("printDocument",null);
        PRINT_STYLES[1] = printDoc.addStyle("boldPrintDocument",null);
        StyleConstants.setBackground(PRINT_STYLES[0], Color.WHITE);
        StyleConstants.setBackground(PRINT_STYLES[1], Color.WHITE);
        StyleConstants.setBold(PRINT_STYLES[1], true);
    }


}
