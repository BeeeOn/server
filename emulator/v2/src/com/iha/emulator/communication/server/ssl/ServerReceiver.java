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
 * Class providing stable server connection secured with SSL/TLS. Used for registering adapter,
 * receiving and processing messages initiated by server.
 *
 * @see com.iha.emulator.communication.server.ssl
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class ServerReceiver extends Thread implements MessageSender{
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(ServerReceiver.class);
    /** default thread sleep time */
    private static final long DEFAULT_SLEEP_TIME = 3000;
    /** default socket timeout - used for debugging */
    private static final int SOCKET_TIMEOUT = 10000;
    /** path to certificate authority file */
    private static final String CA_CERT_PATH = "lib/certificates/cacert.crt";
    /** default certificate alias*/
    private static final String ALIAS_CA_CERT = "ca";
    /** debug certificate common name */
    private static final String SERVER_CN_CERTIFICATE = "ant-2.fit.vutbr.cz";
    /** thread termination indicator */
    private boolean terminate = false;
    /** field indicating, if thread should communicate with server or wait */
    private boolean enabled = false;
    /** field indicating, if connection was initialized */
    private boolean initialized = false;
    /** controller of adapter using this connection */
    private AdapterController adapterController;
    /** controller of server model for this connection */
    private ServerController serverController;
    /** connected property, can be bound */
    private BooleanProperty conn;
    /** SSL socket instance used for communication */
    private SSLSocket socket;
    /** SSL Context instance used for communication */
    private SSLContext sslContext;
    /** domain name for certificate domain verification */
    private String hostName;
    /** formatted socket output writer */
    private PrintWriter socketWriter = null;
    /** formatted socket input reader */
    private BufferedReader socketReader = null;
    /**
     * Creates new ServerReceiver with given adapter controller, for which is connection initialized, and
     * calls {@link #createSSLCertificate()} to create SSL certificate and initiate SSL context
     * @param adapterController adapter controller using stable connection
     */
    public ServerReceiver(AdapterController adapterController) {
        this.adapterController = adapterController;
        this.serverController = adapterController.getServerController();
        this.conn = new SimpleBooleanProperty(false);
        setName("ServerReceiver/"+adapterController.getAdapter().getId());
        //initialize certificate
        createSSLCertificate();
    }
    /**
     * Gets domain name from {@link com.iha.emulator.models.Server#getIp()} and assigns it to field {@link #hostName}
     *
     * @throws UnknownHostException if cannot retrieve domain name for server model IP property
     */
    private void figureOutHostName() throws UnknownHostException {
        logger.trace("Getting host name for: " + serverController.getModel().getIp());
        this.hostName = InetAddress.getByName(serverController.getModel().getIp()).getHostName();
        logger.debug("Hostname: " + this.hostName);
    }
    /**
     * Creates new certificate from certificate authority (loaded from file {@link #CA_CERT_PATH}),
     * creates {@link java.security.KeyStore}, {@link javax.net.ssl.TrustManager}, initializes {@link javax.net.ssl.SSLContext}
     * and assigns it to {@link #sslContext} for later use.
     */
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

    /**
     * Creates secure connection with server over SSL, checks if socket is valid and assigns socket's input/output stream
     *  to formatted reader/writer.
     * @throws IOException {@link javax.net.ssl.SSLSocket} I/O error
     */
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

    /**
     * If connection is enabled (after successful adapter registering), starts to listen to server messages. Disabled if adapter is disabled.
     * If connection is unexpectedly closed, method will try to reconnect.
     */
    public void run(){
        while (!terminate){
            //if connection is closed, try to reconnect
            if((socket == null || socketWriter == null || socketReader == null) && initialized && enabled){
                logger.warn("No connection, trying to connect to server");
                Platform.runLater(() -> adapterController.sendMessage("Server receiver disconnected, trying to connect"));
                reconnect();
            }else{
                //if connection is initialized (set after successful adapter registering ), starts to listen for messages
                //from server
                try {
                    if(initialized){
                        listenForMessages();
                    }
                } catch (IOException e) {
                    //determine error
                    logger.trace("IO error");
                } catch (WrongResponseException e) {
                    //server closed connection, try to reconnect
                    Platform.runLater(()->adapterController.sendError("Warning: Server closed connection",e,false));
                    reconnect();
                } catch (DocumentException e) {
                    //cannot parse message from server
                    logger.error("Cannot parse server message");
                    Platform.runLater(() -> adapterController.sendError(getName() + " -> cannot parse server message", e, false));
                }
            }
            //if adapter is disabled or not initialized, put thread to sleep
            if(!enabled) {
                logger.warn("Don't proceed. Adapter disabled");
                Platform.runLater(()-> adapterController.sendMessage("Server receiver paused"));
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

    /**
     * Listening and processing of messages sent by server. Connection must be previously established, otherwise
     * exception is thrown.
     *
     * @throws IOException connection not established or {@link javax.net.ssl.SSLSocket} read I/O error
     * @throws WrongResponseException if connection was closed, but none or not full message was received
     * @throws DocumentException if received message could not be parsed as XML
     */
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
        //parse string as XML and process message
        try{
            processMessage(DocumentHelper.parseText(trimEndOfLine(response.toString())));
        }catch (IllegalArgumentException | NullPointerException e){
            Platform.runLater(() -> Utilities.showException(logger, "Cannot parse message or error on socket reading!\n" + response.toString(), e, false, null));
        }
    }

    /**
     * <p>Removes EOL character from start of string.</p>
     * <p>
     *     Notes: <br>
     *         Used on messages from server.
     * </p>
     *
     *
     * @param response string, that should be modified
     * @return string without EOL character as first character in string
     */
    private String trimEndOfLine(String response){
        if(response.startsWith("\n")) response = response.replaceFirst("\\n","");
        return response;
    }

    /**
     * Processes message from server as Dom4j XML document. Checks if protocol version is equal to version used
     * by assigned adapter and afterwards passes message to adapter's {@link com.iha.emulator.communication.protocol.Protocol}
     * instance for parsing.
     *
     * @param inDocument message from server as Dom4j XML document
     * @throws IllegalArgumentException if message cannot be parsed by {@link com.iha.emulator.communication.protocol.Protocol} instance.
     * (details in exception message).
     * @throws NullPointerException if trying to parse XML element that should be in message, but is not.
     */
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

    /**
     * Creates and returns instance of {@link com.iha.emulator.communication.protocol.Protocol} from protocol version
     *  number from message.
     *
     * @param inDocument message from server as Dom4j XML document
     * @return instance of {@link com.iha.emulator.communication.protocol.Protocol} implementation
     * @throws IllegalArgumentException if protocol version from message is not implemented
     */
    public Protocol getMessageProtocolVersion(Document inDocument) throws IllegalArgumentException{
        Element rootElement = inDocument.getRootElement();
        String protocolVersion = rootElement.attribute("protocol_version").getValue();
        logger.trace("Incoming message protocol: " + protocolVersion);
        return ProtocolFactory.buildProtocol(Double.valueOf(protocolVersion));
    }
    /**
     * <p>
     *     Sends register message after establishing stable connection. This message must be sent after each successful
     *     connection. Connection must be previously established, otherwise thread will be put to sleep for {@link #DEFAULT_SLEEP_TIME}
     *     and afterwards will try to reconnect.
     * </p>
     *
     * @param message Dom4j XML document with register message for server
     * @param responseTracker instance of class tracking server response time
     * @param type message type
     * @return server response as string
     * @throws IOException {@link javax.net.ssl.SSLSocket} I/O error or connection is not established
     * @throws WrongResponseException if socket reached end of stream while reading or socket is not valid (details in exception message)
     */
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

    /**
     * Creates new register message and adds it to the top of assigned adapter's message queue.
     * Afterwards separately running implementation of interface {@link com.iha.emulator.control.scheduler.Scheduler}
     * will pick up this message and try to send it.
     */
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

    /**
     * Closes socket's input/output stream and socket itself.
     *
     * @throws IOException closing I/O error
     */
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

    /**
     * Enable listening for messages. If thread is asleep, notifies it
     */
    public synchronized void enable(){
        if(!enabled && initialized){
            logger.debug("ENABLED");
            enabled = true;
            notify();
        }
    }

    /**
     * Sets initialization indicator and enables thread
     */
    public synchronized void initialize(){
        initialized = true;
        enable();
    }

    /**
     * Disables thread. If connection is active, closes it.
     */
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

    /**
     * Puts thread to sleep for {@link #DEFAULT_SLEEP_TIME}
     * @throws InterruptedException if thread was interrupted during sleep
     */
    public void pauseReceiver() throws InterruptedException {
        logger.debug(getName() + " -> pausing server receiver for 3 seconds");
        sleep(DEFAULT_SLEEP_TIME);
    }

    /**
     * Terminate thread
     */
    public void terminate(){
        logger.debug("TERMINATING");
        this.terminate = true;
    }

    /**
     * Gets connection status
     * @return connection status
     */
    public boolean getConn() {
        return conn.get();
    }

    /**
     * Gets connection status property, can be bound
     * @return connection status property
     */
    public BooleanProperty connProperty() {
        return conn;
    }

    /**
     * Sets connection status
     * @param conn connection status
     */
    public void setConn(boolean conn) {
        this.conn.set(conn);
    }

    /**
     * Gets connection initialization indicator
     * @return connection initialization indicator
     */
    public boolean isInitialized() {
        return initialized;
    }

    /**
     * Sets connection initialization indicator
     * @param initialized connection initialization indicator
     */
    public void setInitialized(boolean initialized) {
        this.initialized = initialized;
    }
}
