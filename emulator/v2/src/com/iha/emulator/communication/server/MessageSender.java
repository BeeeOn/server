package com.iha.emulator.communication.server;

import com.iha.emulator.utilities.watchers.ResponseTracker;
import org.dom4j.Document;

import java.io.IOException;

/**
 * Interface used for sending messages to server.
 *
 * @see com.iha.emulator.communication.server.ssl
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public interface MessageSender {
    /**
     * Method used for sending message to server.
     *
     * @param message Dom4j XML document with message for server
     * @param responseTracker instance of class tracking server response time
     * @param type message type
     * @return server response
     * @throws IOException defined in implementation class
     * @throws WrongResponseException defined in implementation class
     */
    public String sendMessage(Document message,ResponseTracker responseTracker,OutMessage.Type type) throws IOException, WrongResponseException;
}
