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
 * Created by Shu on 9.2.2015.
 */
public class ServerController implements MessageSender {

    private static final Logger logger = LogManager.getLogger(ServerController.class);
    public static final int MESSAGE_BUFFER_SIZE = 1000;
    public static final int SOCKET_TIMEOUT = 3000;
    public static final boolean DEBUG = true;
    private static final boolean USE_SSL = true;

    private static final String CA_CERT_PATH = "lib/certificates/cacert.crt";
    private static final String ALIAS_CA_CERT = "ca";
    private static final String SERVER_CN_CERTIFICATE = "ant-2.fit.vutbr.cz";

    private Server model;
    //private SocketChannel socketChannel;
    private long responseStart = 0L;

    private SSLContext sslContext;
    private String hostName;

    public ServerController(Server model) {
        setModel(model);
        if(USE_SSL){
            createSSLCertificate();
        }
    }

    private void figureHostName() throws UnknownHostException {
        logger.trace("Getting host name for: " + getModel().getIp());
        this.hostName = InetAddress.getByName(getModel().getIp()).getHostName();
        logger.debug("Hostname: " + this.hostName);
    }

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
