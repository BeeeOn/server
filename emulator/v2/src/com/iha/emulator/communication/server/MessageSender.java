package com.iha.emulator.communication.server;

import com.iha.emulator.utilities.watchers.ResponseTracker;
import org.dom4j.Document;

import java.io.IOException;

/**
 * Created by Shu on 18/03/15.
 */
public interface MessageSender {
    public String sendMessage(Document message,ResponseTracker responseTracker,OutMessage.Type type) throws IOException, WrongResponseException;
}
