package com.iha.emulator.utilities.logging;

import com.iha.emulator.control.SensorController;
import com.iha.emulator.control.task.SimulationTask;
import com.iha.emulator.control.task.TaskParameters;
import com.iha.emulator.utilities.Utilities;
import com.iha.emulator.utilities.handlers.ErrorHandler;
import javafx.application.Platform;
import javafx.beans.property.IntegerProperty;
import javafx.beans.property.SimpleIntegerProperty;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.scene.Node;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.Tab;
import javafx.scene.control.TabPane;
import javafx.scene.control.TextArea;
import javafx.scene.layout.StackPane;
import javafx.scene.text.Text;
import javafx.scene.text.TextFlow;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.time.LocalDateTime;
import java.time.LocalTime;
import java.time.format.DateTimeFormatter;
import java.util.Iterator;

/**
 * Class providing GUI components and method for adapter to write in three types of log:<br>
 * - "Adapter's Log" - global messages by adapter and it's sensors
 * - "To Be Sent Log" - showing unsent messages
 * - "Error Log" - showing errors that occurred on adapter and it's sensors <br>
 * Also provides logs buffering to given file.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class AdapterLogger {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(AdapterLogger.class);
    /** default time pattern */
    private static final String TIME_PATTERN = "HH:mm:ss(SSS)";
    /** default date-time pattern */
    private static final String DATE_TIME_PATTERN = "dd-MM-yyyy_HH-mm-ss";
    /** time formatter */
    private static DateTimeFormatter timeFormatter = DateTimeFormatter.ofPattern(TIME_PATTERN);
    /** date-time formatter */
    private static DateTimeFormatter dateTimeFormatter = DateTimeFormatter.ofPattern(DATE_TIME_PATTERN);
    /** index of "Adapter Log" in TabPane */
    private static final int ADAPTER_LOG_TAB_INDEX = 0;
    /** index of "To Be Sent Log" in TabPane */
    private static final int TO_BE_SENT_TAB_INDEX = 1;
    /** index of "Error Log" in TabPane */
    private static final int ERROR_TAB_INDEX = 2;
    /** maximum number of lines showed in application*/
    private static final int BUFFER_LINE_COUNT_MAX = 200;
    /** maximum warning messages count, after which error event is raised */
    private static final int MAX_WARNING_COUNT = 100000;
    /** panel containing logs areas */
    private TabPane tabPane = null;
    /** "Adapter's Log" GUI component container */
    private Node adapterLogContainer;
    /** "To Be Sent Log" GUI component container */
    private Node toBeSentLogContainer;
    /** "Error Log" GUI component container */
    private Node errorLogContainer;
    /** "Adapter's Log" GUI component */
    private TextArea adapterLog;
    /** "To Be Sent Log" GUI component */
    private TextFlow toBeSentLog;
    /** "Error Log" GUI component */
    private TextArea errorLog;
    /** Instance of message tracker*/
    private MessageTracker messageTracker = new MessageTracker(false);
    /** flag, if "To Be Sent Log" should be shown */
    private boolean showToBeSent = true;
    /** flag, if adapter's logs should be buffered */
    private boolean buffered = false;
    /** Logs buffer file*/
    private File bufferFile;
    /** Logs file buffered writer */
    private BufferedWriter bufferWriter;
    /** helper variable to determine, when should be log GUI area cleared */
    private int bufferMark = 0;
    /** list of "To Be Sent Log" messages, that are to be deleted*/
    private ObservableList<Text> removeLater = FXCollections.observableArrayList();
    /** handler for error events*/
    private ErrorHandler errorHandler = null;
    /** connection warning counter*/
    private IntegerProperty warningCounter;

    /**
     * Creates new instance of AdapterLogger, initializes warning messages counter and error handler.
     */
    public AdapterLogger() {
        this.warningCounter = new SimpleIntegerProperty(0);
        errorHandler = new ErrorHandler();
        initWarningCounter();
    }

    /**
     * Creates new instance of AdapterLogger, sets it's parent component and initializes warning messages counter
     * and error handler.
     *
     * @param tabPane container for logs components
     */
    public AdapterLogger(TabPane tabPane){
        this();
        this.tabPane = tabPane;
    }

    /**
     * Write message to buffer file and "Adapter's Log" GUI component. Checks, if GUI component should clear it's messages.
     * @param message message to be written
     */
    public synchronized void log(String message){
        if(adapterLog!= null){
            //don't wont to show empty messages
            if(message.equals("")) return;
            String timeAndMessage = timeFormatter.format(LocalTime.now())  + " - " + message + "\n";
            //write message to file
            if(buffered && bufferWriter != null){
                try {
                    bufferWriter.write(timeAndMessage);
                    bufferWriter.flush();
                    bufferMark++;
                } catch (IOException e) {
                    Utilities.showException(logger, "Cannot write line \"" + message + "\" to buffer file!", e, false, null);
                }
            }
            //delete already buffered messages from log
            checkIfRemove();
            //write message to log
            addTextToLog(timeAndMessage);
        }else{
            logger.error("No adapterLog");
        }
    }

    /**
     * Checks if number of messages in "Adapter's Log" is bigger than {@link #BUFFER_LINE_COUNT_MAX}. If it is,
     * "Adapter's Log" component is cleared and mar is set to 0.
     */
    public synchronized void checkIfRemove(){
        if(bufferMark >= BUFFER_LINE_COUNT_MAX){
            Platform.runLater(adapterLog::clear);
            bufferMark = 0;
        }
    }

    /**
     * Adds given string to "Adapter's Log" component.
     * @param text text to be shown.
     */
    public synchronized void addTextToLog(String text){
        Platform.runLater(()-> adapterLog.appendText(text));
    }

    /**
     * If showing unsent messages is enabled, adds new message to "To Be Sent Log" component.
     * @param message message to be written
     * @return GUI component representing given message
     */
    public synchronized Text sent(String message){
        //increment waiting messages counter
        Platform.runLater(() -> getMessageTracker().incrementWaitingCounter());
        //if unsent messages are not enabled, return null
        if(!isShowToBeSent()) return null;
        //create message GUI component
        Text messageText = new Text(timeFormatter.format(LocalTime.now()) + " - " + message + "\n");
        //add component to "To Be Sent Log" component
        Platform.runLater(() -> {
            if(toBeSentLog!= null){
                toBeSentLog.getChildren().add(messageText);
            }
        });
        return messageText;
    }

    /**
     * Writes error message to "Error Log" component. Changes color of error tab according to error level. Also if
     * termination is set, error event is fired.
     * @param message message to be written
     * @param terminate <code>true</code> if error event should be fired, <code>false</code> otherwise
     */
    public synchronized void error(String message,boolean terminate){
        Platform.runLater(() -> {
            if(errorLog!= null){
                String messageWithTime = timeFormatter.format(LocalTime.now()) + " - " + message + "\n";
                errorLog.appendText(messageWithTime);
                if(tabPane != null){
                    //change color of error tab
                    Tab errorTab = tabPane.getTabs().get(ERROR_TAB_INDEX);
                    if(!errorTab.isSelected() && message.contains("Warning:")){
                        //yellow background if only warning
                        errorTab.setStyle("-fx-background-color: #ffde00;-fx-fill: #ffffff;-fx-text-fill: #ffffff;");
                    }else if(!message.contains("Warning:")){
                        //fatal error - red background
                        errorTab.setStyle("-fx-background-color: #eb4242;-fx-fill: #ffffff;-fx-text-fill: #ffffff;");
                        tabPane.getSelectionModel().select(ERROR_TAB_INDEX);
                    }
                }
                if(errorHandler != null){
                    if(message.contains("Warning: Cannot connect to server! -> Connection timed out: connect") && !terminate){
                        setWarningCounter(getWarningCounter() + 1);
                    }else if(terminate){
                        errorHandler.fireErrorEvent();
                    }
                }
            }
        });
    }

    /**
     * Removes text of successfully sent message from "To Be Sent Log". If cannot remove immediately, message is moved to
     * {@link #removeLater} list and is dealt with later.
     * @param message text of message, that is to be removed
     */
    public synchronized void notifyDataSent(Text message){
        Platform.runLater(() ->{
            getMessageTracker().decrementWaitingCounter();
            getMessageTracker().incrementSentCounter();
        });
        if(!isShowToBeSent()) return;
        processRemoveLaterMessages();
        if(toBeSentLog.getChildren().size() > 0){
            Platform.runLater(() -> toBeSentLog.getChildren().remove(message));
        }else{
            synchronized (this){
                if(!removeLater.contains(message)){
                    logger.warn("To Be Sent message doesn't exist in log yet. Saving to remove later.");
                    removeLater.add(message);
                }
            }

        }
    }

    /**
     * Process removing of messages, that were supposed to be removed.
     */
    public synchronized void processRemoveLaterMessages(){
        if(removeLater.size() > 0){
            synchronized (this){
                for(Iterator<Text> it = removeLater.iterator(); it.hasNext();){
                    Text message = it.next();
                    logger.trace("Removing saved ToBeSent messages");
                    Platform.runLater(() -> {
                        if(toBeSentLog.getChildren().remove(message)){
                            logger.trace("Message: " + message.getText() + " removed");
                            it.remove();
                        }
                    });
                }
                logger.warn("Remove later after clearing: " + removeLater.size());
            }
        }
    }

    /**
     * Create GUI components for all three logs
     */
    public void createLogs(){
        if(adapterLog == null){
            adapterLog = new TextArea();
            adapterLog.setWrapText(true);
            adapterLog.setEditable(false);
        }
        if(toBeSentLog == null){
            toBeSentLog = new TextFlow();
            stylize(toBeSentLog);
        }
        if(errorLog == null){
            errorLog = new TextArea();
            errorLog.setWrapText(true);
            errorLog.setEditable(false);
        }
    }

    /**
     * If "Adapter's Log" GUI component doesn't exit, method creates it and adds it to given container.
     * @param container container for log component
     * @return log GUI component
     */
    public TextArea addAdapterLogTo(Node container){
        //create new textArea
        if(adapterLog == null){
            adapterLog = new TextArea();
            adapterLog.setWrapText(true);
            adapterLog.setEditable(false);
        }
        StackPane paneContainer = (StackPane) container;
        paneContainer.getChildren().clear();
        paneContainer.getChildren().add(adapterLog);
        this.adapterLogContainer = container;
        // listener for tab color change
        tabPane.getTabs().get(ADAPTER_LOG_TAB_INDEX).selectedProperty().addListener(new ChangeListener<Boolean>() {
            @Override
            public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
                if(newValue){
                    Platform.runLater(()-> tabPane.getTabs().get(ADAPTER_LOG_TAB_INDEX).setStyle("-fx-background-color: #a6dcff;-fx-fill: #242424;-fx-text-fill: #242424;"));
                }else {
                    Platform.runLater(()->tabPane.getTabs().get(ADAPTER_LOG_TAB_INDEX).setStyle("-fx-background-color: #c6c6c6;-fx-fill: #242424;-fx-text-fill: #242424;"));
                }
            }
        });
        return adapterLog;
    }
    /**
     * If "To Be Sent Log" GUI component doesn't exit, method creates it and adds it to given container.
     * @param container container for log component
     * @return log GUI component
     */
    public TextFlow addSentLogTo(Node container){
        //create new textArea
        if (toBeSentLog == null){
            toBeSentLog = new TextFlow();
            stylize(toBeSentLog);
        }
        ScrollPane paneContainer = (ScrollPane) container;
        paneContainer.setContent(null);
        paneContainer.setContent(toBeSentLog);
        paneContainer.setFitToWidth(true);
        this.toBeSentLogContainer = container;
        // listener for tab color change
        tabPane.getTabs().get(TO_BE_SENT_TAB_INDEX).selectedProperty().addListener(new ChangeListener<Boolean>() {
            @Override
            public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
                if(!tabPane.getTabs().get(TO_BE_SENT_TAB_INDEX).isDisable() && newValue){
                    Platform.runLater(()-> tabPane.getTabs().get(TO_BE_SENT_TAB_INDEX).setStyle("-fx-background-color: #a6dcff;-fx-fill: #242424;-fx-text-fill: #242424;"));
                }else if(!tabPane.getTabs().get(TO_BE_SENT_TAB_INDEX).isDisable()){
                    Platform.runLater(()->tabPane.getTabs().get(TO_BE_SENT_TAB_INDEX).setStyle("-fx-background-color: #c6c6c6;-fx-fill: #242424;-fx-text-fill: #242424;"));
                }
            }
        });
        return toBeSentLog;
    }
    /**
     * If "Error Log" GUI component doesn't exit, method creates it and adds it to given container.
     * @param container container for log component
     * @return log GUI component
     */
    public TextArea addErrorLogTo(Node container){
        //create new textArea
        if(errorLog == null){
            errorLog = new TextArea();
            errorLog.setWrapText(true);
            errorLog.setEditable(false);
        }
        StackPane paneContainer = (StackPane) container;
        paneContainer.getChildren().clear();
        paneContainer.getChildren().add(errorLog);
        this.errorLogContainer = container;
        // listener for tab color change
        tabPane.getTabs().get(ERROR_TAB_INDEX).selectedProperty().addListener(new ChangeListener<Boolean>() {
            @Override
            public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
                if(newValue){
                    Platform.runLater(()-> tabPane.getTabs().get(ERROR_TAB_INDEX).setStyle("-fx-background-color: #a6dcff;-fx-fill: #242424;-fx-text-fill: #242424;"));
                }else {
                    Platform.runLater(()->tabPane.getTabs().get(ERROR_TAB_INDEX).setStyle("-fx-background-color: #c6c6c6;-fx-fill: #242424;-fx-text-fill: #242424;"));
                }
            }
        });
        return errorLog;
    }

    /**
     * Clear contents of all logs components
     */
    public void clearContainers(){
        if(this.adapterLogContainer != null){
            ((StackPane)adapterLogContainer).getChildren().clear();
        }
        if(this.toBeSentLogContainer != null){
            ((ScrollPane)toBeSentLogContainer).setContent(null);
        }
        if(this.errorLogContainer != null){
            ((StackPane)errorLogContainer).getChildren().clear();
        }
    }

    /**
     * Closes buffer file, and clears logs GUI components content
     */
    public void delete(){
        closeBuffer();
        //adapterLog.getChildren().clear();
        Platform.runLater(()->{
            adapterLog.clear();
            toBeSentLog.getChildren().clear();
            errorLog.clear();
        });
        removeLater.clear();
    }

    /**
     * Closes buffer file, writes all logs to buffer file before closing.
     */
    public void closeBuffer(){
        setWarningCounter(0);
        if(this.bufferWriter != null){
            try {
                bufferWriter.flush();
                moveLogsToFile();
                bufferWriter.close();
                bufferWriter = null;
            } catch (IOException e) {
                Utilities.showException(logger,"Cannot close logger buffer",e,false,null);
            }
        }
    }

    /**
     * Writes all logs to buffer file
     * @throws IOException cannot write to file
     */
    public synchronized void moveLogsToFile() throws IOException {
        if(bufferWriter == null) return;
        logSentMessagesCount(bufferWriter);
        moveToBeSentToFile(bufferWriter);
        moveErrorToFile(bufferWriter);
    }

    /**
     * Write unsent messages count to buffer file
     * @param writer writer of buffer file
     * @throws IOException buffer file doesn't exist
     */
    public synchronized void logSentMessagesCount(BufferedWriter writer) throws IOException {
        if(writer == null) throw new IOException("Buffered writer is null. Cannot move log to buffer file!");
        if(getMessageTracker() != null && getMessageTracker().isEnabled()){
            writer.write("Number of sent messages: " + getMessageTracker().getSentMessageCounter());
        }
    }
    /**
     * Write unsent messages to buffer file
     * @param writer writer of buffer file
     * @throws IOException buffer file doesn't exist
     */
    public synchronized void moveToBeSentToFile(BufferedWriter writer) throws IOException {
        if(toBeSentLog == null|| writer == null) throw new IOException("ToBeSent log or buffered writer is null. Cannot move log to buffer file!");
        writer.newLine();
        writer.newLine();
        writer.write("== TO BE SENT LOG =============================================\n");
        if(isShowToBeSent()){
            for(Node node : toBeSentLog.getChildren()){
                Text text = (Text) node;
                writer.write(text.getText());
            }
        }else if(getMessageTracker().isEnabled()){
            writer.write("Number of waiting messages: " + getMessageTracker().getWaitingMessageCounter());
        }
        writer.newLine();
        writer.newLine();
        writer.flush();
    }
    /**
     * Write error messages to buffer file
     * @param writer writer of buffer file
     * @throws IOException buffer file doesn't exist
     */
    public synchronized void moveErrorToFile(BufferedWriter writer) throws IOException {
        if(errorLog == null|| writer == null) throw new IOException("ToBeSent log or buffered writer is null. Cannot move log to buffer file!");
        writer.newLine();
        writer.newLine();
        writer.write("== ERROR LOG ================================================\n");
        writer.write(errorLog.getText());
        writer.newLine();
        writer.newLine();
        writer.flush();
    }

    /**
     * Deletes buffer file from system.
     */
    public void deleteBufferFile(){
        if(bufferFile != null){
            bufferFile.delete();
            bufferFile = null;
        }
    }

    /**
     * Remove all messages still present in "To Be Sent Log" by given sensor
     * @param sensorController sensor, for which should be messages removed
     */
    public synchronized void removeUnsentMessagesBySensor(SensorController sensorController){
        if(toBeSentLog != null && toBeSentLog.getChildren().size() !=0){
            for (Iterator<Node> it = toBeSentLog.getChildren().iterator(); it.hasNext(); ) {
                Node child = it.next();
                if (Utilities.containsIgnoreCase(((Text) child).getText(), sensorController.toString())) {
                    it.remove();
                }
            }
        }
    }

    /**
     * Adds CSS class to given component
     * @param area area to be stylized
     */
    private void stylize(TextFlow area){
        area.getStyleClass().addAll("logTextArea");
    }

    /**
     * Creates and opens buffer file. If path given as parameter is null, temp file is created.
     *
     * @param buffered flag, if logs should be buffered
     * @param filename buffer file filename
     * @param path path to buffer file
     * @throws IOException cannot create file or directory
     */
    public void setBuffered(boolean buffered,String filename,String path) throws IOException {
        this.buffered = buffered;
        if(buffered){
            if(bufferFile == null){
                logger.trace("Buffer file doesn't exist, creating new log file");
                if(path == null){
                    logger.trace("Creating tmp file");
                    bufferFile = File.createTempFile(filename ,".log");
                }else {
                    logger.trace("Creating file with path: " + path);
                    File dir = new File(path);
                    if(!dir.exists()){
                        logger.trace("Dir doesn't exist. Creating dir");
                        if(!dir.mkdirs()){
                            throw new IOException("Cannot create directory in path: " + path);
                        }
                    }
                    logger.trace("Setting file");
                    bufferFile = new File(dir.getAbsolutePath(),filename + dateTimeFormatter.format(LocalDateTime.now()) + ".log");
                }
                logger.trace("Log file location: " + bufferFile.getAbsolutePath());
                openBufferFile();
                logger.debug("Created  log buffer file.");
            }
        }
        logger.debug("Log is now buffered");
    }

    /**
     * Open previously saved buffer file to write
     * @throws IOException cannot open file
     */
    public void openBufferFile() throws IOException {
        logger.trace("Opening buffer file");
        bufferWriter = new BufferedWriter(new FileWriter(bufferFile,true));
    }

    /**
     * Writes default information about {@link com.iha.emulator.control.task.SimulationTask} to log.
     * @param task task providing information
     * @throws IOException cannot write to buffer file
     */
    public void writeTaskLogHeaderToBuffer(SimulationTask task) throws IOException {
        TaskParameters params = task.getTaskParameters();
        bufferWriter.write("== TASK LOG =================================================\n");
        bufferWriter.write("Task parameters: \n");
        bufferWriter.write("\tServer: \n");
        bufferWriter.write("\t" + "  Name: " + task.getServerController().getModel().getName() + "\n" +
                            "\t" + "  IP: " + task.getServerController().getModel().getIp() + "\n" +
                            "\t" + "  Port: " +task.getServerController().getModel().getPort() + "\n" +
                            "\t" + "  Database: " +task.getServerController().getModel().getDatabaseName() + "\n");
        bufferWriter.write("\tAdapters: " + "\n" +
                            "\t" + "  Adapters count: " + params.getAdaptersCount() + "\n" +
                            "\t" + "  Protocol: " + params.getProtocolVersion().getVersion() + "\n" +
                            "\t" + "  Start ID: " + params.getStartId() + "\n" +
                            "\t" + "  Sensors count per adapter: " + (params.getSensorsCountMin()!=0 ? params.getSensorsCountMin() + " -> " : "") + params.getSensorsCountMax() + "\n" +
                            "\t" + "  Refresh time range: " + (params.getRefreshTimeMin()!=0 ? params.getRefreshTimeMin() + " -> " : "") + params.getRefreshTimeMax() + "\n");
        bufferWriter.newLine();
        bufferWriter.flush();
    }

    /**
     * Write default header of "Adapter's Log" to buffer file
     * @throws IOException cannot write to file
     */
    public void writeAdapterLogHeaderToBuffer() throws IOException {
        bufferWriter.write("== ADAPTER LOG ===============================================\n");
        bufferWriter.flush();
    }

    /**
     * Initializes connection warnings counter. If there are more connection warnings then {@link #MAX_WARNING_COUNT},
     * error message is sent and error event is fired.
     */
    private void initWarningCounter(){
        warningCounterProperty().addListener(new ChangeListener<Number>() {
            @Override
            public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                if((int)newValue >= MAX_WARNING_COUNT && getErrorHandler()!= null){
                    error("Too many connection warnings. Stopping task",true);
                }
            }
        });
    }

    /**
     * Gets GUI component for "Adapter's Log"
     * @return GUI component for "Adapter's Log"
     */
    public TextArea getAdapterLog() {
        return adapterLog;
    }

    /**
     * Gets GUI component for "To Be Sent Log"
     * @return GUI component for "To Be Sent Log"
     */
    public TextFlow getToBeSentLog() {
        return toBeSentLog;
    }

    /**
     * Gets GUI component for "Error Log"
     * @return GUI component for "Error Log"
     */
    public TextArea getErrorLog() {
        return errorLog;
    }

    /**
     * Gets buffer file
     * @return buffer file
     */
    public File getBufferFile(){
        return bufferFile;
    }

    /**
     * Gets error events handler
     * @return error events handler
     */
    public ErrorHandler getErrorHandler() {
        return errorHandler;
    }

    /**
     * Gets instance of message tracker
     * @return instance of message tracker
     */
    public MessageTracker getMessageTracker() {
        return messageTracker;
    }

    /**
     * Gets flag, if unsent messages should be shown
     * @return <code>true</code> if unsent messages should be shown, <code>false</code> otherwise
     */
    public boolean isShowToBeSent() {
        return showToBeSent;
    }

    /**
     * Gets value of connection warnings counter
     * @return value of connection warnings counter
     */
    public int getWarningCounter() {
        return warningCounter.get();
    }

    /**
     * Connection warnings counter property, that can be bound
     * @return connection warnings counter property
     */
    public IntegerProperty warningCounterProperty() {
        return warningCounter;
    }

    /**
     * Sets value of connection warnings counter
     * @param warningCounter value of connection warnings counter
     */
    public void setWarningCounter(int warningCounter) {
        this.warningCounter.set(warningCounter);
    }

    /**
     * Sets flag, if unsent messages should be shown. Also disables "To Be Sent Log" tab, if not.
     * @param showToBeSent <code>true</code> if unsent message should be shown, <code>false</code> otherwise
     */
    public void setShowToBeSent(boolean showToBeSent) {
        this.showToBeSent = showToBeSent;
        if(tabPane != null){
            logger.trace("Disabling \"To Be sent\" tab.");
            Platform.runLater(()->tabPane.getTabs().get(TO_BE_SENT_TAB_INDEX).setDisable(!showToBeSent));
            logger.trace("OK");
        }else {
            logger.trace("Cannot disable \"To Be sent\" tab. TabPane is null");
        }
    }
}
