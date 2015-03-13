package com.iha.emulator.utilities.logging;

import com.iha.emulator.control.SensorController;
import com.iha.emulator.utilities.Utilities;
import com.iha.emulator.utilities.handlers.ErrorHandler;
import javafx.application.Platform;
import javafx.beans.property.BooleanProperty;
import javafx.beans.property.IntegerProperty;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.beans.property.SimpleIntegerProperty;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.scene.Node;
import javafx.scene.control.ScrollPane;
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
 * Created by Shu on 27.11.2014.
 */
public class AdapterLogger {
    private static final Logger logger = LogManager.getLogger(AdapterLogger.class);

    private static final String TIME_PATTERN = "HH:mm:ss(SSS)";
    private static final String DATE_TIME_PATTERN = "dd-MM-yyyy_HH-mm-ss";
    private static DateTimeFormatter timeFormatter = DateTimeFormatter.ofPattern(TIME_PATTERN);
    private static DateTimeFormatter dateTimeFormatter = DateTimeFormatter.ofPattern(DATE_TIME_PATTERN);
    private static final int TO_BE_SENT_TAB_INDEX = 1;
    private static final int ERROR_TAB_INDEX = 2;

    private static final int BUFFER_LINE_COUNT_MAX = 200;
    private static final int MAX_WARNING_COUNT = 1000;

    public enum Type{
        FULL,
        PARTIAL,
        SHORT
    }

    public enum Style {
        REGULAR,
        BOLD
    }

    private Type actualType;

    private TabPane tabPane = null;

    private Node adapterLogContainer;
    private Node toBeSentLogContainer;
    private Node errorLogContainer;

    private TextArea adapterLog;
    private TextFlow toBeSentLog;
    private TextArea errorLog;

    private MessageTracker messageTracker = new MessageTracker(false);
    private boolean showToBeSent = true;

    private BooleanProperty fullMessage;
    private BooleanProperty partialMessage;
    private BooleanProperty shortMessage;

    private boolean buffered = false;
    private File bufferFile;
    private BufferedWriter bufferWriter;
    private int bufferMark = 0;

    private ObservableList<Text> removeLater = FXCollections.observableArrayList();
    private ErrorHandler errorHandler = null;
    private IntegerProperty warningCounter;

    public AdapterLogger() {
        this.fullMessage = new SimpleBooleanProperty(true);
        this.partialMessage = new SimpleBooleanProperty(false);
        this.shortMessage = new SimpleBooleanProperty(false);
        this.warningCounter = new SimpleIntegerProperty(0);
        errorHandler = new ErrorHandler();
        bindChangeListener();
        initWarningCounter();
    }

    public AdapterLogger(TabPane tabPane){
        this();
        this.tabPane = tabPane;
    }

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

    public synchronized void checkIfRemove(){
        if(bufferMark >= BUFFER_LINE_COUNT_MAX){
            Platform.runLater(adapterLog::clear);
            bufferMark = 0;
        }
    }

    public synchronized void addTextToLog(String text){
        Platform.runLater(()-> adapterLog.appendText(text));
    }

    public synchronized Text sent(String message){
        Platform.runLater(() -> getMessageTracker().incrementWaitingCounter());
        if(!isShowToBeSent()) return null;
        Text messageText = new Text(timeFormatter.format(LocalTime.now()) + " - " + message + "\n");
        Platform.runLater(() -> {
            if(toBeSentLog!= null){
                toBeSentLog.getChildren().add(messageText);
            }
        });
        return messageText;
    }

    public synchronized void error(String message){
        error(message,false);
    }

    public synchronized void error(String message,boolean terminate){
        Platform.runLater(() -> {
            if(errorLog!= null){
                String messageWithTime = timeFormatter.format(LocalTime.now()) + " - " + message + "\n";
                errorLog.appendText(messageWithTime);
                if(tabPane != null && !tabPane.getSelectionModel().isSelected(ERROR_TAB_INDEX)) tabPane.getSelectionModel().select(ERROR_TAB_INDEX);
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

    public void createLogs(){
        if(adapterLog == null){
            adapterLog = new TextArea();
            adapterLog.setWrapText(true);
        }
        if(toBeSentLog == null){
            toBeSentLog = new TextFlow();
            stylize(toBeSentLog);
        }
        if(errorLog == null){
            errorLog = new TextArea();
            errorLog.setWrapText(true);
        }
    }

    public TextArea addAdapterLogTo(Node container){
        //create new textArea
        if(adapterLog == null){
            adapterLog = new TextArea();
            adapterLog.setWrapText(true);
        }
        StackPane paneContainer = (StackPane) container;
        paneContainer.getChildren().add(adapterLog);
        this.adapterLogContainer = container;
        return adapterLog;
    }

    public TextFlow addSentLogTo(Node container){
        //create new textArea
        if (toBeSentLog == null){
            toBeSentLog = new TextFlow();
            stylize(toBeSentLog);
        }
        ScrollPane paneContainer = (ScrollPane) container;
        paneContainer.setContent(toBeSentLog);
        paneContainer.setFitToWidth(true);
        this.toBeSentLogContainer = container;
        return toBeSentLog;
    }

    public TextArea addErrorLogTo(Node container){
        //create new textArea
        if(errorLog == null){
            errorLog = new TextArea();
            errorLog.setWrapText(true);
        }
        StackPane paneContainer = (StackPane) container;
        paneContainer.getChildren().add(errorLog);
        this.errorLogContainer = container;
        return errorLog;
    }

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

    public synchronized void moveLogsToFile() throws IOException {
        if(bufferWriter == null) return;
        logSentMessagesCount(bufferWriter);
        moveToBeSentToFile(bufferWriter);
        moveErrorToFile(bufferWriter);
    }

    public synchronized void logSentMessagesCount(BufferedWriter writer) throws IOException {
        if(writer == null) throw new IOException("Buffered writer is null. Cannot move log to buffer file!");
        if(getMessageTracker().isEnabled()){
            writer.write("Number of sent messages: " + getMessageTracker().getSentMessageCounter());
        }
    }

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

    public void deleteBufferFile(){
        if(bufferFile != null){
            bufferFile.delete();
            bufferFile = null;
        }
    }

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

    private void stylize(TextFlow area){
        area.getStyleClass().addAll("logTextArea");
    }

    private void bindChangeListener(){
        this.fullMessage.addListener(new MessageTypeChangeListener(Type.FULL));
        this.partialMessage.addListener(new MessageTypeChangeListener(Type.PARTIAL));
        this.shortMessage.addListener(new MessageTypeChangeListener(Type.SHORT));
    }

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

    public void openBufferFile() throws IOException {
        logger.trace("Opening buffer file");
        bufferWriter = new BufferedWriter(new FileWriter(bufferFile,true));
    }

    public void writeTaskLogHeaderToBuffer() throws IOException {
        bufferWriter.write("== TASK LOG =================================================\n");
        bufferWriter.flush();
    }

    public void writeAdapterLogHeaderToBuffer() throws IOException {
        bufferWriter.write("== ADAPTER LOG ===============================================\n");
        bufferWriter.flush();
    }

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

    public static Type toType(String typeString){
        String lowerTypeString = typeString.toLowerCase();
        Type type;
        switch (lowerTypeString){
            case "full":
                type = Type.FULL;
                break;
            case "partial":
                type = Type.PARTIAL;
                break;
            case "short":
                type = Type.SHORT;
                break;
            default:
                throw new IllegalArgumentException("Unknown value of property \"defaultLogMessageType\" ->" + typeString +" -> has to be: FULL or PARTIAL or SHORT");
        }
        return type;
    }

    public void setType(Type type){
        this.actualType = type;
    }

    @SuppressWarnings("unused")
    public TextArea getAdapterLog() {
        return adapterLog;
    }

    @SuppressWarnings("unused")
    public TextFlow getToBeSentLog() {
        return toBeSentLog;
    }

    @SuppressWarnings("unused")
    public TextArea getErrorLog() {
        return errorLog;
    }

    @SuppressWarnings("unused")
    public boolean getFullMessage() {
        return fullMessage.get();
    }

    public BooleanProperty fullMessageProperty() {
        return fullMessage;
    }

    @SuppressWarnings("unused")
    public void setFullMessage(boolean fullMessage) {
        this.fullMessage.set(fullMessage);
    }

    @SuppressWarnings("unused")
    public boolean getPartialMessage() {
        return partialMessage.get();
    }

    public BooleanProperty partialMessageProperty() {
        return partialMessage;
    }

    @SuppressWarnings("unused")
    public void setPartialMessage(boolean partialMessage) {
        this.partialMessage.set(partialMessage);
    }

    @SuppressWarnings("unused")
    public boolean getShortMessage() {
        return shortMessage.get();
    }

    public BooleanProperty shortMessageProperty() {
        return shortMessage;
    }

    public File getBufferFile(){
        return bufferFile;
    }

    @SuppressWarnings("unused")
    public void setShortMessage(boolean shortMessage) {
        this.shortMessage.set(shortMessage);
    }

    public ErrorHandler getErrorHandler() {
        return errorHandler;
    }

    public MessageTracker getMessageTracker() {
        return messageTracker;
    }

    public boolean isShowToBeSent() {
        return showToBeSent;
    }

    public int getWarningCounter() {
        return warningCounter.get();
    }

    public IntegerProperty warningCounterProperty() {
        return warningCounter;
    }

    public void setWarningCounter(int warningCounter) {
        this.warningCounter.set(warningCounter);
    }

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

    private class MessageTypeChangeListener implements ChangeListener<Boolean> {

        private Type type;

        public MessageTypeChangeListener(Type type){
            super();
            this.type = type;
        }

        @Override
        public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
            if(newValue) actualType = this.getType();
        }

        private Type getType() {
            return type;
        }
    }
}
