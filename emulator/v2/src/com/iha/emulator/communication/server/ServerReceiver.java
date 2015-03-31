package com.iha.emulator.communication.server;

import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.communication.protocol.ProtocolFactory;
import com.iha.emulator.communication.server.ssl.ServerController;
import com.iha.emulator.control.AdapterController;
import com.iha.emulator.utilities.Utilities;
import com.iha.emulator.utilities.watchers.ResponseTracker;
import javafx.application.Platform;
import javafx.beans.property.BooleanProperty;
import javafx.beans.property.SimpleBooleanProperty;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;

import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLSocket;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.StandardSocketOptions;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

/**
 * Created by Shu on 17/03/15.
 */
public class ServerReceiver extends Thread implements MessageSender{

    private static final Logger logger = LogManager.getLogger(ServerReceiver.class);
    private static final long DEFAULT_SLEEP_TIME = 3000;

    private boolean terminate = false;
    private boolean enabled = false;
    private boolean initialized = false;
    private AdapterController adapterController;
    private SocketChannel socketChannel;
    private ServerController serverController;
    private BooleanProperty conn;

    private SSLSocket socket;
    private SSLContext sslContext;
    private String hostName;

    public ServerReceiver(AdapterController adapterController) {
        this.adapterController = adapterController;
        this.serverController = adapterController.getServerController();
        this.conn = new SimpleBooleanProperty(false);
        setName("ServerReceiver/"+adapterController.getAdapter().getId());
    }

    public synchronized void connect() throws IOException {
        disconnect();
        logger.trace("Trying to connect");
        //create socket channel
        socketChannel = SocketChannel.open();
        //set socket option to keep connection alive
        socketChannel.setOption(StandardSocketOptions.SO_KEEPALIVE,true);
        //connect to server
        socketChannel.connect(new InetSocketAddress(serverController.getModel().getIp(),serverController.getModel().getPort()));
        Platform.runLater(()-> Platform.runLater(()->setConn(true)));
    }

    public void run(){
        while (!terminate){
            if((socketChannel == null || !socketChannel.isConnected()) && initialized){
                logger.warn("No connection, trying to connect to server");
                Platform.runLater(() -> adapterController.sendMessage("Server receiver disconnected, trying to connect"));
                reconnect();
            }else{
                try {
                    if(initialized){
                        listenForMessages();
                    }
                } catch (IOException e) {
                    //determine error
                    logger.trace("IO error");
                } catch (WrongResponseException e) {
                    Platform.runLater(()->adapterController.sendError("Warning: Server closed connection",e,false));
                    reconnect();
                } catch (DocumentException e) {
                    logger.error("Cannot parse server message");
                    Platform.runLater(() -> adapterController.sendError(getName() + " -> cannot parse server message", e, false));
                }
            }
            if(!enabled){
                logger.warn("Don't proceed. Adapter disabled");
                Platform.runLater(()->adapterController.sendMessage("Server receiver paused"));
                synchronized (this){
                    while(!enabled)
                        try {
                            wait();
                        } catch (InterruptedException e) {
                            logger.fatal("OnDisabled wait fail -> " + e.getMessage(),e);
                            terminate();
                            break;
                        }
                }
            }
        }
    }

    public void listenForMessages() throws IOException, WrongResponseException, DocumentException {
        if(socketChannel == null || !socketChannel.isConnected()){
            logger.fatal(getName() + " socket not connected");
            throw new IOException("Listen for message. SocketChannel null or not connected");
        }
        adapterController.sendMessage("Server receiver listening");
        //buffer for response message
        ByteBuffer messageBuffer = ByteBuffer.allocate(ServerController.MESSAGE_BUFFER_SIZE);
        //response message byte size
        int bytesRead;
        //clear response message buffer, just to be sure
        messageBuffer.clear();
        logger.debug("Listening for new messages from server");
        //read response message from server
        bytesRead = socketChannel.read(messageBuffer);
        //if socket suddenly closes
        if (bytesRead == -1) {
            throw new WrongResponseException("Connection closed by server");
        }
        //prepare response buffer
        messageBuffer.flip();
        messageBuffer.position(0);
        //process message as string
        try{
            processMessage(DocumentHelper.parseText(new String(messageBuffer.array()).substring(0, bytesRead)));
        }catch (IllegalArgumentException | NullPointerException e){
            final int finalBytesRead = bytesRead;
            Platform.runLater(() -> Utilities.showException(logger, "Cannot parse message or error on socket reading!\n" + new String(messageBuffer.array()).substring(0, finalBytesRead), e, false, null));
        }
    }

    private void processMessage(Document inDocument) throws IllegalArgumentException, NullPointerException{
        logger.debug("Processing incoming message: \n" + inDocument.asXML());
        logger.trace("Building protocol");
        Protocol protocol = getMessageProtocolVersion(inDocument);
        if(protocol == null ) throw new NullPointerException("Cannot build protocol");
        protocol.checkProtocolVersion(inDocument);
        try{
            protocol.parseInAdapterMessage(inDocument,adapterController);
        }catch (NumberFormatException e){
            throw new IllegalArgumentException("Id is not in right format");
        }

    }

    public Protocol getMessageProtocolVersion(Document inDocument) throws IllegalArgumentException{
        Element rootElement = inDocument.getRootElement();
        String protocolVersion = rootElement.attribute("protocol_version").getValue();
        logger.trace("Incoming message protocol: " + protocolVersion);
        return ProtocolFactory.buildProtocol(Double.valueOf(protocolVersion));
    }

    public String sendMessage(Document message,ResponseTracker responseTracker,OutMessage.Type type) throws IOException, WrongResponseException {
        try {
            if(initialized) pauseReceiver();
            connect();
        } catch (InterruptedException e) {
            logger.fatal("Cannot put to sleep");
        }
        if(socketChannel == null || !socketChannel.isConnected()){
            throw new IOException("Cannot send message. SocketChannel null or not connected");
        }
        //buffer for response message
        ByteBuffer messageBuffer = ByteBuffer.allocate(ServerController.MESSAGE_BUFFER_SIZE);
        //response message byte size
        int bytesRead;
        //response start
        long responseStart = 0L;
        logger.trace("Sending from server receiver message: " + type.toString());
        //convert XML to ByteBuffer
        ByteBuffer out = ByteBuffer.wrap(message.asXML().getBytes());
        //track response start
        if (responseTracker.isEnabled()) responseStart = System.currentTimeMillis();
        //send message
        while (out.hasRemaining()) {
            socketChannel.write(out);
        }
        logger.trace("Message sent");
        //clear response message buffer, just to be sure
        messageBuffer.clear();
        //read response message from server
        bytesRead = socketChannel.read(messageBuffer);
        //if socket suddenly closes
        if (bytesRead == -1) {
            throw new WrongResponseException("Wrong response on message from server");
        }
        //track response end
        if (responseTracker.isEnabled()){
            responseTracker.addResponse(responseStart, System.currentTimeMillis());
        }
        //prepare response buffer
        messageBuffer.flip();
        messageBuffer.position(0);
        //return response message as string
        return new String(messageBuffer.array()).substring(0, bytesRead);
    }

    public synchronized void reconnect(){
        Platform.runLater(()->setConn(false));
        logger.trace("Trying to reconnect");
        OutMessage registerMessage = adapterController.createRegisterMessage();
        try {
            //TODO parse register message response
            sendMessage(registerMessage.getSocketMessage(),adapterController.getScheduler().getResponseTracker(),registerMessage.getType());
            adapterController.messageSuccessfullySent(registerMessage);
        }  catch (IOException | WrongResponseException e) {
            logger.debug("Connection unsuccessful");
        }
    }

    public synchronized void disconnect() throws IOException {
        Platform.runLater(()->setConn(false));
        if(socketChannel != null){
            logger.trace("disconnecting");
            socketChannel.close();
            socketChannel = null;
        }
    }

    public synchronized void enable(){
        if(!enabled && initialized){
            logger.debug("ENABLED");
            enabled = true;
            notify();
        }
    }

    public synchronized void initialize(){
        initialized = true;
        enable();
    }

    public synchronized void disable(){
        if(enabled){
            try {
                disconnect();
            } catch (IOException e) {
                logger.error(getName() + " -> cannot disconnect -> " + e.getMessage(),e);
            }
            logger.debug(getName() + " -> DISABLED");
            enabled = false;
        }
    }

    public void pauseReceiver() throws InterruptedException {
        logger.debug(getName() + " -> pausing server receiver for 3 seconds");
        sleep(DEFAULT_SLEEP_TIME);
    }

    public boolean isConnected() {
        return socketChannel != null && socketChannel.isConnected();
    }

    public void terminate(){
        logger.debug("TERMINATING");
        this.terminate = true;
    }

    public boolean getConn() {
        return conn.get();
    }

    public BooleanProperty connProperty() {
        return conn;
    }

    public void setConn(boolean conn) {
        this.conn.set(conn);
    }

    public boolean isInitialized() {
        return initialized;
    }

    public void setInitialized(boolean initialized) {
        this.initialized = initialized;
    }
}
