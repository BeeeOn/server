package com.iha.emulator.communication.server;

import com.iha.emulator.control.SensorController;
import com.iha.emulator.models.Sensor;
import javafx.scene.text.Text;
import org.dom4j.Document;

/**
 * Stores information about message as sender,log message and actual message for server.
 */
public class OutMessage {

    public enum Type{
        REGISTER_ADAPTER,
        SENSOR_MESSAGE
    }

    /** message sender */
    SensorController senderController = null;
    /** message to be written to log */
    String logMessage = null;
    /** message according to communication protocol for server */
    Document socketMessage = null;
    /** Message type */
    Type type = null;

    Text unsent = null;
    /**
     * Creates new message to be sent to server and written to log
     * @param message to be written to lgo
     * @param serverMessage message according to communication protocol to be sent to server
     * @param senderController that sent message
     */
    public OutMessage(String message, Document serverMessage, SensorController senderController, Type type,Text unsent) {
        this.logMessage = message;
        this.socketMessage = serverMessage;
        this.senderController = senderController;
        this.type = type;
        this.unsent = unsent;
    }
    /**
     * Appends string to message to be written to log
     * @param message string to be appended to log message
     */
    public void appendToMsg(String message){
        if(logMessage != null)
            logMessage = logMessage + message;
    }
    public SensorController getSenderController(){
        return this.senderController;
    }
    /**
     * Returns sensor, that sent this message
     * @return sensor, that sent this message
     */
    public Sensor getSender(){
        return this.senderController.getModel();
    }

    public String getLogMessage(){
        return this.logMessage;
    }

    public Document getSocketMessage(){
        return this.socketMessage;
    }

    public Type getType(){
        return this.type;
    }

    public Text getUnsent() {
        return unsent;
    }
}
