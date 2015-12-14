package com.iha.emulator.communication.server;

import com.iha.emulator.models.Server;
import com.iha.emulator.utilities.watchers.ResponseTracker;
import javafx.application.Platform;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.dom4j.Document;
import org.dom4j.Element;

import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManagerFactory;
import java.io.*;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.NotYetConnectedException;
import java.nio.channels.SocketChannel;
import java.security.KeyManagementException;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;

/**
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
@Deprecated
public class ServerController implements MessageSender{
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(ServerController.class);
    /** default size of buffer for message to server */
    public static final int MESSAGE_BUFFER_SIZE = 1000;
    /** default socket timeout */
    public static final int SOCKET_TIMEOUT = 3000;
    /** path to SSL certificate file */
    private static final String CA_CERT_PATH = "lib/certificates/cacert-ant-2.crt";
    /** client certificate alias */
    private static final String ALIAS_CA_CERT = "ca";
    /** server model */
    private Server model;
    /** default time of server response */
    private long responseStart = 0L;
    /** field holding created ssl context used in communication */
    private SSLContext sslContext;

    /**
     * Creates ServerController with {@link com.iha.emulator.models.Server} model given as parameter.
     * @param model class with server information
     */
    public ServerController(Server model) {
        //assign model
        setModel(model);
        logger.trace("Current dir: " + System.getProperty("user.dir"));
        //create initial SSL certificate
        createSSLCertificate();
    }

    public String sendMessage(Document message,ResponseTracker responseTracker,OutMessage.Type type) throws WrongResponseException, IOException {
        ByteBuffer messageBuffer = ByteBuffer.allocate(MESSAGE_BUFFER_SIZE);
        int bytesRead = 0;
        logger.trace("Opening socketChannel");
        try (SocketChannel socketChannel = SocketChannel.open()){
            //socketChannel = SocketChannel.open();
            socketChannel.socket().setSoTimeout(SOCKET_TIMEOUT);
            logger.trace("Trying to connect");
            socketChannel.connect(new InetSocketAddress(getModel().getIp(), getModel().getPort()));
            Platform.runLater(() -> getModel().setConn(true));
            ByteBuffer out = ByteBuffer.wrap(message.asXML().getBytes());
            if (responseTracker.isEnabled()) responseStart = System.currentTimeMillis();
            logger.trace("Sending message: " + type.toString());
            while (out.hasRemaining()) {
                socketChannel.write(out);
            }
            logger.trace("Message sent");

            messageBuffer.clear();
            try{
                bytesRead = socketChannel.read(messageBuffer);
            }catch (NotYetConnectedException e){
                switch (type){
                    case REGISTER_ADAPTER:
                        throw new WrongResponseException("Not yet connected (REGISTER_MESSAGE)",e);
                    case SENSOR_MESSAGE:
                        throw new WrongResponseException("Not yet connected (SENSOR_MESSAGE",e);
                }
            }
            //if socket suddenly closes
            if (bytesRead == -1) {
                switch (type){
                    case REGISTER_ADAPTER:
                        throw new WrongResponseException("End of read stream. Socket closed (REGISTER_MESSAGE)");
                    case SENSOR_MESSAGE:
                        throw new WrongResponseException("End of read stream. Socket closed (SENSOR_MESSAGE");
                }
            }
            if (responseTracker.isEnabled())
                responseTracker.addResponse(responseStart, System.currentTimeMillis());

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

    private void createSSLCertificate(){
        logger.debug("Creating certificate");
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
