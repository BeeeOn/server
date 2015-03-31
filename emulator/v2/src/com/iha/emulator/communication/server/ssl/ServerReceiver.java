package com.iha.emulator.communication.server.ssl;

import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.communication.protocol.ProtocolFactory;
import com.iha.emulator.communication.server.MessageSender;
import com.iha.emulator.communication.server.OutMessage;
import com.iha.emulator.communication.server.WrongResponseException;
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
import javax.net.ssl.SSLSession;
import javax.net.ssl.SSLSocket;
import javax.net.ssl.TrustManagerFactory;
import java.io.*;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.security.KeyManagementException;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;

/**
 * Created by Shu on 17/03/15.
 */
public class ServerReceiver extends Thread implements MessageSender{

    private static final Logger logger = LogManager.getLogger(ServerReceiver.class);
    private static final long DEFAULT_SLEEP_TIME = 3000;
    private static final int SOCKET_TIMEOUT = 10000;
    private static final String CA_CERT_PATH = "lib/certificates/cacert.crt";
    private static final String ALIAS_CA_CERT = "ca";
    private static final String SERVER_CN_CERTIFICATE = "ant-2.fit.vutbr.cz";

    private boolean terminate = false;
    private boolean enabled = false;
    private boolean initialized = false;
    private AdapterController adapterController;
    //private SocketChannel socketChannel;
    private ServerController serverController;
    private BooleanProperty conn;

    private SSLSocket socket;
    private SSLContext sslContext;
    private String hostName;
    private PrintWriter socketWriter = null;
    private BufferedReader socketReader = null;

    public ServerReceiver(AdapterController adapterController) {
        this.adapterController = adapterController;
        this.serverController = adapterController.getServerController();
        this.conn = new SimpleBooleanProperty(false);
        setName("ServerReceiver/"+adapterController.getAdapter().getId());
        //initialize certificate
        createSSLCertificate();
    }

    private void figureOutHostName() throws UnknownHostException {
        logger.trace("Getting host name for: " + serverController.getModel().getIp());
        this.hostName = InetAddress.getByName(serverController.getModel().getIp()).getHostName();
        logger.debug("Hostname: " + this.hostName);
    }

    private void createSSLCertificate(){
        logger.debug("Creating certificate receiver");
        try(InputStream inStreamCert = new FileInputStream(new File(CA_CERT_PATH))){
            logger.trace("Certificate file loaded");
            CertificateFactory cf = CertificateFactory.getInstance("X.509");
            Certificate ca = cf.generateCertificate(inStreamCert);
            logger.trace("Certificate generated");
            // Create a KeyStore containing our trusted CAs
            String keyStoreType = KeyStore.getDefaultType();
            KeyStore keyStore = KeyStore.getInstance(keyStoreType);
            keyStore.load(null, null);
            keyStore.setCertificateEntry(ALIAS_CA_CERT, ca);
            logger.trace("KeyStore initialized");
            // Create a TrustManager that trusts the CAs in our KeyStore
            String tmfAlgorithm = TrustManagerFactory.getDefaultAlgorithm();
            TrustManagerFactory tmf = TrustManagerFactory.getInstance(tmfAlgorithm);
            tmf.init(keyStore);
            logger.trace("Algorithm initialized");
            // Create an SSLContext that uses our TrustManager
            sslContext = SSLContext.getInstance("TLS");
            sslContext.init(null, tmf.getTrustManagers(), null);
            logger.trace("SSL context initialized");
        } catch (FileNotFoundException e) {
            logger.fatal("Certificate file: " + CA_CERT_PATH + " not found!");
        } catch (IOException e) {
            logger.fatal("Certificate file: " + CA_CERT_PATH + " IO exception");
        } catch (CertificateException e) {
            logger.fatal("No such certificate exception",e);
        } catch (NoSuchAlgorithmException e) {
            logger.fatal("No such algorithm exception", e);
        } catch (KeyStoreException e) {
            logger.fatal("KeyStore exception", e);
        } catch (KeyManagementException e) {
            logger.fatal("KeyManagement exception", e);
        }
    }

    public synchronized void connect() throws IOException {
        disconnect();
        logger.trace("Trying to connect");
        //if(hostName == null) figureOutHostName();
        if(sslContext == null) createSSLCertificate();
        //create socket
        socket = (SSLSocket) sslContext.getSocketFactory().createSocket(serverController.getModel().getIp(),serverController.getModel().getPort());
        //set timeout
        socket.setKeepAlive(true);
        //socket.setSoTimeout(SOCKET_TIMEOUT);
        //create session
        SSLSession session = socket.getSession();
        //verify session validity
        if(!session.isValid()){
            logger.warn("Socket is not valid. Need to reinitialize certificate");
            Platform.runLater(()->adapterController.sendError("Warning: Socket is not valid. Reinitializing certificate",null,false));
            sslContext = null;
            return;
        }
        //verify server
        /*HostnameVerifier hostnameVerifier = new WildcardHostnameVerifier();
        if(!hostnameVerifier.verify(SERVER_CN_CERTIFICATE,session)){
            logger.warn("Peer host: " + session.getPeerHost());
            throw new SSLHandshakeException("Certification failed! Expected CN value:" + hostName + ", found " + session.getPeerPrincipal());
        }*/
        logger.debug("Host VERIFIED");
        //safe to proceed, start communication exchange
        socketWriter = new PrintWriter(socket.getOutputStream());
        socketReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        //set connection
        Platform.runLater(()->setConn(true));
    }

    public void run(){
        while (!terminate){
            if((socket == null || socketWriter == null || socketReader == null) && initialized && enabled){
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
            if(!enabled) {
                logger.warn("Don't proceed. Adapter disabled");
                Platform.runLater(()->{
                    adapterController.sendMessage("Server receiver paused");
                });
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
        if(socket == null || socketWriter == null || socketReader == null){
            logger.fatal(getName() + " socket not connected");
            throw new IOException("Listen for message. Socket streams are closed");
        }
        adapterController.sendMessage("Server receiver listening");
        logger.debug("Listening for new messages from server");
        StringBuilder response = new StringBuilder();
        int actReceived = 0;
        boolean responseComplete = false;
        while((actReceived = socketReader.read()) != -1){
            response.append((char)actReceived);
            if(response.toString().endsWith("</server_adapter>") || response.toString().endsWith("/>")){
                responseComplete = true;
                break;
            }
        }
        logger.debug("Message from server received: " + response.toString());
        //if socket closes before message is completely read
        if(!responseComplete){
            throw new WrongResponseException("End of read stream");
        }
        //process message as string
        try{
            processMessage(DocumentHelper.parseText(trimEndOfLine(response.toString())));
        }catch (IllegalArgumentException | NullPointerException e){
            Platform.runLater(() -> Utilities.showException(logger, "Cannot parse message or error on socket reading!\n" + response.toString(), e, false, null));
        }
    }

    private String trimEndOfLine(String response){
        if(response.startsWith("\n")) response = response.replaceFirst("\\n","");
        return response;
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
        if(socket == null || socketWriter == null || socketReader == null){
            throw new IOException("Cannot send message. Socket streams are closed. Maybe server is down?");
        }
        //send message
        logger.trace("Sending message: " + type.toString() + "\n" + message.asXML());
        socketWriter.write(message.asXML());
        socketWriter.flush();
        logger.trace("Message sent");
        StringBuilder response = new StringBuilder();
        int actReceived = 0;
        boolean responseComplete = false;
        while((actReceived = socketReader.read()) != -1){
            response.append((char)actReceived);
            if(response.toString().endsWith("</server_adapter>") || response.toString().endsWith("/>")){
                responseComplete = true;
                break;
            }
        }
        //if socket closes before message is completely read
        if(!responseComplete){
            switch (type){
                case REGISTER_ADAPTER:
                    throw new WrongResponseException("End of read stream (REGISTER_MESSAGE) -> Response: " + response.toString());
                case SENSOR_MESSAGE:
                    throw new WrongResponseException("End of read stream (SENSOR_MESSAGE) -> Response: " + response.toString());
            }
        }
        logger.trace("Response message: \n" + response);
        return response.toString();
    }

    public synchronized void reconnect(){
        Platform.runLater(()->setConn(false));
        logger.trace("Trying to reconnect");
        OutMessage registerMessage = adapterController.createRegisterMessage();
        try {
            //TODO parse register message response
            sendMessage(registerMessage.getSocketMessage(),adapterController.getScheduler().getResponseTracker(),registerMessage.getType());
        } catch (UnknownHostException e) {
            Platform.runLater(()->{
                adapterController.sendError("Unable to get hostname",e,false);
                adapterController.disable();
            });
        } catch (IOException | WrongResponseException e) {
            logger.debug("Connection unsuccessful");
        }finally {
            adapterController.messageSuccessfullySent(registerMessage);
        }
    }

    public synchronized void disconnect() throws IOException {
        Platform.runLater(()->setConn(false));
        if(socket != null){
            logger.trace("Disconnecting");
            if(socketWriter != null) {
                logger.trace("Disconnecting -> Closing writer");
                socketWriter.close();
                socketWriter = null;
            }
            if(socketReader != null) {
                logger.trace("Disconnecting -> Closing reader");
                socketReader.close();
                socketReader = null;
            }
            logger.trace("Disconnecting -> Closing socket");
            socket.close();
            logger.trace("Disconnecting -> OK");
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
            enabled = false;
            try {
                disconnect();
            } catch (IOException e) {
                logger.error(getName() + " -> cannot disconnect -> " + e.getMessage(),e);
            }
            logger.debug(getName() + " -> DISABLED");
        }
    }

    public void pauseReceiver() throws InterruptedException {
        logger.debug(getName() + " -> pausing server receiver for 3 seconds");
        sleep(DEFAULT_SLEEP_TIME);
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
