package com.smarthome.emulator.simulations.basis;

import org.dom4j.Document;

import java.nio.ByteBuffer;

/**
 * Stores information about message as sender,log message and actual message for server.
 */
public class Message {
    /** message sender */
    Sensor sender = null;
    /** message to be written to log */
    String logMessage = null;
    /** message according to communication protocol for server */
    Document socketMessage = null;
    /**
     * Creates log message
     * @param message message to be written in log
     */
    public Message(String message){
        logMessage = message;
    }
    /**
     * Creates new message to be sent to server and written to log
     * @param message to be written to lgo
     * @param serverMessage message according to communication protocol to be sent to server
     * @param sensor that sent message
     */
    public Message(String message,Document serverMessage,Sensor sensor) {
        logMessage = message;
        socketMessage = serverMessage;
        sender = sensor;
    }
    /**
     * Appends string to message to be written to log
     * @param message string to be appended to log message
     */
    public void appendToMsg(String message){
        if(logMessage != null)
            logMessage = logMessage + message;
    }
    /**
     * Returns sensor, that sent this message
     * @return sensor, that sent this message
     */
    public Sensor getSender(){
        return this.sender;
    }

    public String getLogMessage(){
        return logMessage;
    }

    public Document getSocketMessage(){
        return socketMessage;
    }
}
