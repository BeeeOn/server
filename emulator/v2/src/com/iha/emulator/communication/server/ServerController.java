package com.iha.emulator.communication.server;

import com.iha.emulator.models.Server;
import com.iha.emulator.utilities.watchers.ResponseTracker;
import javafx.application.Platform;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.dom4j.Document;
import org.dom4j.Element;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

/**
 * Created by Shu on 9.2.2015.
 */
public class ServerController implements MessageSender{

    private static final Logger logger = LogManager.getLogger(ServerController.class);
    public static final int MESSAGE_BUFFER_SIZE = 1000;
    public static final int SOCKET_TIMEOUT = 3000;
    public static final boolean DEBUG = true;

    private Server model;
    //private SocketChannel socketChannel;
    private long responseStart = 0L;

    public ServerController(Server model) {
        setModel(model);
    }

    public String sendMessage(Document message,ResponseTracker responseTracker,OutMessage.Type type) throws WrongResponseException, IOException {
        ByteBuffer messageBuffer = ByteBuffer.allocate(MESSAGE_BUFFER_SIZE);
        int bytesRead = 0;
        try (SocketChannel socketChannel = SocketChannel.open()){
            //socketChannel = SocketChannel.open();
            socketChannel.socket().setSoTimeout(SOCKET_TIMEOUT);
            logger.trace("Sending message: " + type.toString());
            socketChannel.connect(new InetSocketAddress(getModel().getIp(), getModel().getPort()));
            Platform.runLater(() -> getModel().setConn(true));
            ByteBuffer out = ByteBuffer.wrap(message.asXML().getBytes());
            if (responseTracker.isEnabled()) responseStart = System.currentTimeMillis();
            while (out.hasRemaining()) {
                socketChannel.write(out);
            }
            logger.trace("Message sent");
            if(!DEBUG){
                switch (type) {
                    case SENSOR_MESSAGE:
                        messageBuffer.clear();
                        bytesRead = socketChannel.read(messageBuffer);
                        //if socket suddenly closes
                        if (bytesRead == -1) {
                            throw new WrongResponseException("Wrong response from server");
                        }
                        if (responseTracker.isEnabled())
                            responseTracker.addResponse(responseStart, System.currentTimeMillis());
                        break;
                    case REGISTER_ADAPTER:
                        if (responseTracker.isEnabled())
                            responseTracker.addResponse(responseStart, System.currentTimeMillis());
                        return null;
                }
            }else {
                messageBuffer.clear();
                bytesRead = socketChannel.read(messageBuffer);
                //if socket suddenly closes
                if (bytesRead == -1) {
                    throw new WrongResponseException("Wrong response from server");
                }
                if (responseTracker.isEnabled())
                    responseTracker.addResponse(responseStart, System.currentTimeMillis());
            }
            messageBuffer.flip();
            messageBuffer.position(0);
        }
        return new String(messageBuffer.array()).substring(0, bytesRead);
    }

    public Element saveToXml(Element rootElement){
        return rootElement.addElement("server")
                .addAttribute("name",getModel().getName())
                .addAttribute("ip",getModel().getIp())
                .addAttribute("port", String.valueOf(getModel().getPort()))
                .addAttribute("db", getModel().getDatabaseName());
    }

    public Server getModel() {
        return model;
    }

    public void setModel(Server model) {
        this.model = model;
    }

    public String toString(){
        return getModel().getName() + "/" + getModel().getIp();
    }

    public void delete(){
        this.model = null;
    }
}
