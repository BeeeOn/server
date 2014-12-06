package com.iha.emulator.utilities;

import javafx.application.Platform;
import javafx.beans.InvalidationListener;
import javafx.beans.property.BooleanProperty;
import javafx.beans.property.Property;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.scene.Node;
import javafx.scene.control.TextArea;
import javafx.scene.layout.StackPane;
import javafx.scene.text.Text;
import javafx.scene.text.TextFlow;
import javafx.scene.web.WebView;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.time.LocalTime;
import java.time.format.DateTimeFormatter;

/**
 * Created by Shu on 27.11.2014.
 */
public class AdapterLogger {
    private static final Logger logger = LogManager.getLogger(AdapterLogger.class);

    private static final String TIME_PATTERN = "HH:mm:ss(SSS)";
    private static DateTimeFormatter timeFormatter = DateTimeFormatter.ofPattern(TIME_PATTERN);

    public enum Type{
        FULL,
        PARTIAL,
        SHORT
    }

    public enum Style {
        REGULAR,
        BOLD
    }

    private TextFlow adapterLog;
    private TextFlow toBeSentLog;
    private TextFlow errorLog;

    private BooleanProperty fullMessage;
    private BooleanProperty partialMessage;
    private BooleanProperty shortMessage;

    private Type actualType;

    private Property<StringBuilder> fullString;

    public AdapterLogger() {
        this.fullMessage = new SimpleBooleanProperty(true);
        this.partialMessage = new SimpleBooleanProperty(false);
        this.shortMessage = new SimpleBooleanProperty(false);
        bondChangeListener();

    }

    public synchronized void log(String fullMessage,String partialMessage,String shortMessage){
        Platform.runLater(() -> {
            if(adapterLog!= null){
                String msg = null;
                //determine which message to use
                switch (actualType){
                    case FULL:
                        msg = fullMessage;
                        break;
                    case PARTIAL:
                        msg = partialMessage;
                        break;
                    case SHORT:
                        msg = shortMessage;
                        break;
                }
                //don't wont to show empty messages
                if(msg.equals("")) return;
                //write message with timestamp
                adapterLog.getChildren().add(new Text(timeFormatter.format(LocalTime.now()) + " - " + msg + "\n"));
            }
        });
    }

    public synchronized void sent(String message){
        Platform.runLater(() -> {
            if(toBeSentLog!= null){
                toBeSentLog.getChildren().add(new Text(timeFormatter.format(LocalTime.now()) + " - " + message + "\n"));
            }
        });
    }

    public synchronized void error(String message){
        Platform.runLater(() -> {
            if(errorLog!= null){
                errorLog.getChildren().add(new Text(timeFormatter.format(LocalTime.now()) + " - " + message + "\n"));
            }
        });
    }

    public synchronized void notifyDataSent(){
        if(toBeSentLog.getChildren().size() > 0){
            toBeSentLog.getChildren().remove(0);
        }else{
            logger.error("Trying to remove first toBeSent line, but there are no lines");
        }
    }


    public TextFlow addAdapterLogTo(Node container){
        //create new textArea
        if(adapterLog == null){
            adapterLog = new TextFlow();
            stylize(adapterLog);
        }
        StackPane paneContainer = (StackPane) container;
        paneContainer.getChildren().clear();
        //add new
        paneContainer.getChildren().add(adapterLog);
        return adapterLog;
    }

    public TextFlow addSentLogTo(Node container){
        //create new textArea
        if(toBeSentLog == null){
            toBeSentLog = new TextFlow();
            stylize(toBeSentLog);
        }
        StackPane paneContainer = (StackPane) container;
        paneContainer.getChildren().clear();
        //add new
        paneContainer.getChildren().add(toBeSentLog);
        return toBeSentLog;
    }

    public TextFlow addErrorLogTo(Node container){
        //create new textArea
        if(errorLog == null){
            errorLog = new TextFlow();
            stylize(errorLog);
        }
        StackPane paneContainer = (StackPane) container;
        paneContainer.getChildren().clear();
        //add new
        paneContainer.getChildren().add(errorLog);
        return errorLog;
    }

    private void stylize(TextFlow area){
        area.getStyleClass().addAll("logTextArea");

    }

    private void bondChangeListener(){
        this.fullMessage.addListener(new MessageTypeChangeListener(Type.FULL));
        this.partialMessage.addListener(new MessageTypeChangeListener(Type.PARTIAL));
        this.shortMessage.addListener(new MessageTypeChangeListener(Type.SHORT));
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
    public TextFlow getAdapterLog() {
        return adapterLog;
    }

    @SuppressWarnings("unused")
    public TextFlow getToBeSentLog() {
        return toBeSentLog;
    }

    @SuppressWarnings("unused")
    public TextFlow getErrorLog() {
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

    @SuppressWarnings("unused")
    public void setShortMessage(boolean shortMessage) {
        this.shortMessage.set(shortMessage);
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
