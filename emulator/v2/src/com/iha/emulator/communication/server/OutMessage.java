package com.iha.emulator.communication.server;

import com.iha.emulator.control.SensorController;
import com.iha.emulator.models.Sensor;
import javafx.scene.text.Text;
import org.dom4j.Document;

/**
 * Class stores information about message as sender,log message and actual message for server.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class OutMessage {
    /**
     * Enum holding possible message types
     */
    public enum Type{
        REGISTER_ADAPTER,
        SENSOR_MESSAGE
    }
    /** message sender field*/
    SensorController senderController = null;
    /** message to be written to log*/
    String logMessage = null;
    /** Dom4j XML document message*/
    Document socketMessage = null;
    /** message type*/
    Type type = null;
    /** "To Be Sent Log" message */
    Text unsent = null;

    /**
     * Creates new OutMessage with given adapter's log message, XML message for server, sensor/actuator sending message,
     *  message type {@link com.iha.emulator.communication.server.OutMessage.Type} and unsent message in "To Be Sent Log",
     *  which will be deleted after successful communication with server.
     * @param message message to be written in adapter's log
     * @param serverMessage XML message for server
     * @param senderController sensor/actuator controller, which sends this message
     * @param type message type
     * @param unsent "To Be Sent Log" message
     */
    public OutMessage(String message, Document serverMessage, SensorController senderController, Type type,Text unsent) {
        this.logMessage = message;
        this.socketMessage = serverMessage;
        this.senderController = senderController;
        this.type = type;
        this.unsent = unsent;
    }
    /**
     * Appends string to message, that will be printed in adapter's log
     * @param message string to be appended to log message
     */
    public void appendToMsg(String message){
        if(logMessage != null)
            logMessage = logMessage + message;
    }
    /**
     * Gets sensor/actuator controller, which sends this message
     * @return sensor/actuator controller, which sends this message
     */
    public SensorController getSenderController(){
        return this.senderController;
    }
    /**
     * Gets model of sensor/actuator, that sent this message
     * @return model of sensor/actuator, that sent this message
     */
    public Sensor getSender(){
        return this.senderController.getModel();
    }

    /**
     * Gets message to be printed in adapter's log
     * @return message to be printed in adapter's log
     */
    public String getLogMessage(){
        return this.logMessage;
    }
    /**
     * Gets Dom4j XML document with message for server
     * @return Dom4j XML document with message for server
     */
    public Document getSocketMessage(){
        return this.socketMessage;
    }
    /**
     * Gets message type (see {@link com.iha.emulator.communication.server.OutMessage.Type}
     * @return message type (see {@link com.iha.emulator.communication.server.OutMessage.Type}
     */
    public Type getType(){
        return this.type;
    }
    /**
     * Gets message written in "To Be Sent Log", which should be deleted after successful communication with server
     * @return message written in "To Be Sent Log"
     */
    public Text getUnsent() {
        return unsent;
    }
}
