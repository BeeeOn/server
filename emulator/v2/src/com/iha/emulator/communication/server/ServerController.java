package com.iha.emulator.communication.server;

import com.iha.emulator.models.Server;
import com.iha.emulator.utilities.ResponseTracker;
import javafx.application.Platform;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.dom4j.Document;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.SocketTimeoutException;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

/**
 * Created by Shu on 9.2.2015.
 */
public class ServerController {

    private static final Logger logger = LogManager.getLogger(ServerController.class);
    private static final int SOCKET_TIMEOUT = 5000;

    private Server model;
    private SocketChannel socketChannel;
    private long responseStart = 0L;

    ByteBuffer reusableBuffer = ByteBuffer.allocate(5024);

    public ServerController(Server model) {
        setModel(model);
    }

    public String sendMessage(Document message,ResponseTracker responseTracker,OutMessage.Type type) throws WrongResponseException, IOException, SocketTimeoutException {
        try{
            logger.trace("Sending message: " + type.toString());
            socketChannel = SocketChannel.open();
            socketChannel.socket().setSoTimeout(SOCKET_TIMEOUT);
            if(!socketChannel.connect(new InetSocketAddress(getModel().getIp(),getModel().getPort()))){
                throw new SocketTimeoutException("Unsuccessful connect!");
            }
            Platform.runLater(() -> getModel().setConn(true));
            ByteBuffer out = ByteBuffer.wrap(message.asXML().getBytes());
            if(responseTracker.isEnabled()) responseStart = System.currentTimeMillis();
            while (out.hasRemaining()){
                socketChannel.write(out);
            }
            logger.trace("Message sent");
            int bytesRead = 0;
            switch (type){
                case SENSOR_MESSAGE:
                    reusableBuffer.clear();
                    bytesRead = socketChannel.read(reusableBuffer);
                    //if socket suddenly closes
                    if(bytesRead == -1){
                        throw new WrongResponseException("Wrong response from server");
                    }
                    if(responseTracker.isEnabled()) responseTracker.addResponse(responseStart,System.currentTimeMillis());
                    break;
                case REGISTER_ADAPTER:
                    if(responseTracker.isEnabled()) responseTracker.addResponse(responseStart,System.currentTimeMillis());
                    return null;
            }
            reusableBuffer.flip();
            reusableBuffer.position(0);
            return new String(reusableBuffer.array()).substring(0,bytesRead);
        } finally {
            if(socketChannel != null) socketChannel.close();
        }
    }

    public Server getModel() {
        return model;
    }

    public void setModel(Server model) {
        this.model = model;
    }

    public void delete(){
        this.model = null;
        reusableBuffer = null;
        socketChannel = null;
    }
}
