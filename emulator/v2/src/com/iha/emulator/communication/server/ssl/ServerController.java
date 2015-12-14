package com.iha.emulator.communication.server.ssl;

import com.iha.emulator.communication.server.MessageSender;
import com.iha.emulator.communication.server.OutMessage;
import com.iha.emulator.communication.server.WrongResponseException;
import com.iha.emulator.models.Server;
import com.iha.emulator.utilities.watchers.ResponseTracker;
import javafx.application.Platform;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.dom4j.Document;
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
 * Class supervising {@link com.iha.emulator.models.Server} model, which holds information about server, and
 * providing temporary server connection secured with SSL/TLS.
 *
 * @see com.iha.emulator.communication.server.ssl
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class ServerController implements MessageSender {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(ServerController.class);
    /** default message buffer size */
    public static final int MESSAGE_BUFFER_SIZE = 1000;
    /** default socket timeout */
    public static final int SOCKET_TIMEOUT = 3000;
    /** path to certificate authority file */
    private static final String CA_CERT_PATH = "lib/certificates/cacert.crt";
    /** default certificate alias*/
    private static final String ALIAS_CA_CERT = "ca";
    /** debug certificate common name */
    private static final String SERVER_CN_CERTIFICATE = "ant-2.fit.vutbr.cz";
    /** server model */
    private Server model;
    /** default response start time */
    private long responseStart = 0L;
    /** SSL Context instance used for communication */
    private SSLContext sslContext;
    /** domain name for certificate domain verification */
    private String hostName;

    /**
     * Creates new ServerController with given model and calls {@link #createSSLCertificate()} to create SSL certificate
     * and initiate SSL context
     *
     * @param model server model
     */
    public ServerController(Server model) {
        setModel(model);
        createSSLCertificate();
    }

    /**
     * Gets domain name from {@link com.iha.emulator.models.Server#getIp()} and assigns it to field {@link #hostName}
     *
     * @throws UnknownHostException if cannot retrieve domain name for {@link #model}'s IP property
     */
    private void figureHostName() throws UnknownHostException {
        logger.trace("Getting host name for: " + getModel().getIp());
        this.hostName = InetAddress.getByName(getModel().getIp()).getHostName();
        logger.debug("Hostname: " + this.hostName);
    }

    /**
     * Creates new certificate from certificate authority (loaded from file {@link #CA_CERT_PATH}),
     * creates {@link java.security.KeyStore}, {@link javax.net.ssl.TrustManager}, initializes {@link javax.net.ssl.SSLContext}
     * and assigns it to {@link #sslContext} for later use.
     */
    private void createSSLCertificate(){
        logger.debug("Creating certificate sender");
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
     * <p>
     *     Method creates secure connection with server over SSL, sends message and returns server response for this message.
     * Connection is closed, after method is finished. Additionally this method tracks response time with {@link com.iha.emulator.utilities.watchers.ResponseTracker}
     * given as parameter.
     * </p>
     * <p>
     *     Notes:
     * </p>
     * <ul>
     *     <li>Emulator v2.0.5 and lower - doesn't verify hostname with certificate's common name</li>
     * </ul>
     *
     * @param message Dom4j XML document with message for server
     * @param responseTracker instance of class tracking server response time
     * @param type message type
     * @return server response as string
     * @throws IOException {@link javax.net.ssl.SSLSocket} I/O error
     * @throws WrongResponseException if socket reached end of stream while reading or socket is not valid (details in exception message)
     */
    public String sendMessage(Document message,ResponseTracker responseTracker,OutMessage.Type type) throws WrongResponseException, IOException {
        //if(hostName == null) figureHostName();
        if(sslContext == null) createSSLCertificate();
        //create socket
        SSLSocket  socket = (SSLSocket) sslContext.getSocketFactory().createSocket(getModel().getIp(),getModel().getPort());
        //set timeout
        socket.setSoTimeout(SOCKET_TIMEOUT);
        //create session
        SSLSession session = socket.getSession();
        //verify session validity
        if(!session.isValid()){
            logger.warn("Socket is not valid. Need to reinitialize certificate");
            sslContext = null;
            throw new WrongResponseException("Socket is not valid. Reinitializing certificate");
        }
        //verify server
        /*HostnameVerifier hostnameVerifier = new WildcardHostnameVerifier();
        if(!hostnameVerifier.verify(SERVER_CN_CERTIFICATE,session)){
            throw new SSLHandshakeException("Certification failed! Expected CN value:" + hostName + ", found " + session.getPeerPrincipal());
        }*/
        logger.debug("Host VERIFIED");
        Platform.runLater(() -> getModel().setConn(true));
        //safe to proceed, start communication exchange
        BufferedWriter w = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
        BufferedReader r = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        //send message
        if (responseTracker.isEnabled()) responseStart = System.currentTimeMillis();
        logger.trace("Sending message: " + type.toString() + "\n" + message.asXML());
        w.write(message.asXML());
        w.flush();
        logger.trace("Message sent");
        StringBuilder response = new StringBuilder();
        int actReceived = 0;
        boolean responseComplete = false;
        while((actReceived = r.read()) != -1){
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
        //save response time
        if (responseTracker.isEnabled())
            responseTracker.addResponse(responseStart, System.currentTimeMillis());
        //close streams and socket
        logger.trace("Closing streams and socket");
        w.close();
        r.close();
        socket.close();

        return response.toString();
    }

    /**
     * Creates Dom4j XML element with information about server from assigned{@link com.iha.emulator.models.Server} model.
     * This element is added to root element given as parameter.
     *
     * @param rootElement root Dom4j XML element for server
     * @return server XML element
     */
    public Element saveToXml(Element rootElement){
        return rootElement.addElement("server")
                .addAttribute("name",getModel().getName())
                .addAttribute("ip",getModel().getIp())
                .addAttribute("port", String.valueOf(getModel().getPort()))
                .addAttribute("db", getModel().getDatabaseName());
    }

    /**
     * Gets server model
     * @return server model
     */
    public Server getModel() {
        return model;
    }

    /**
     * Sets server model
     * @param model server model
     */
    public void setModel(Server model) {
        this.model = model;
    }

    /**
     * Returns server name and ip as string (format: name/ip)
     * @return server name and ip as string
     */
    public String toString(){
        return getModel().getName() + "/" + getModel().getIp();
    }

    /**
     * Deletes server model from controller
     */
    public void delete(){
        this.model = null;
    }
}
