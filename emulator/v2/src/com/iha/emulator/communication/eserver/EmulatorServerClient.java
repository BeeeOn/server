package com.iha.emulator.communication.eserver;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

/**
 * Class providing connection to Emulator Server. This class sends messages, which implements
 * interface {@link com.iha.emulator.communication.eserver.task.ServerTask}. Implemented messages
 * can be found in package {@link com.iha.emulator.communication.eserver.task.implemented}.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class EmulatorServerClient {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(EmulatorServerClient.class);
    /** Default emulator server ip address ( ant-2.fit.vutbr.cz ) */
    private static final String HOST_IP = "147.229.176.131";
    /** Default Emulator server listening port */
    private static final int HOST_PORT = 6983;
    /** Connection socket */
    private Socket socket;
    /** Field holding connection address (host,port) */
    private InetAddress address;

    /**
     * Creates EmulatorServerClient, which will be connecting to server on address
     * given as <code>host</code> parameter. If <code>host</code> is <code>null</code>,
     * client will connect to default host (ant-2.fit.vutbr.cz).
     * @param host Hostname or IP of server, that runs Emulator Server
     * @throws UnknownHostException if given host cannot be reached
     */
    public EmulatorServerClient(String host) throws UnknownHostException {
        logger.trace("Getting host address");
        this.address = InetAddress.getByName(HOST_IP);
        if(host == null){
            this.address = InetAddress.getByName(HOST_IP);
        }else {
            this.address = InetAddress.getByName(host);
        }
        logger.trace("  OK");
    }
    /**
     * Creates new connection by creating new socket
     * @throws IOException if cannot create new socket
     */
    public void connect() throws IOException {
        logger.debug("Connecting to server");
        socket = new Socket(address,HOST_PORT);
        //socket.setSoTimeout(5000);
        logger.debug("Connection established successfully");
    }

    /**
     * Sends message in EmulatorServer protocol format given as string parameter.
     * @param message EmulatorServer protocol formatted message
     * @return EmulatorServer response as string
     * @throws IOException socket write or read I/O error
     */
    public String sendMessage(String message) throws IOException {
        PrintWriter out = new PrintWriter(socket.getOutputStream(),true);
        BufferedReader stdIn = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        try {
            logger.debug("Sending message:");
            logger.debug(message);
            if(message.isEmpty()) logger.error("Message for server is EMPTY!!!");
            out.println(message);
            out.flush();
            logger.debug("Message sent successfully");
            String serverInput;
            message = "";
            logger.debug("Receiving message from server:");
            while ((serverInput = stdIn.readLine()) != null) {
                message = message + serverInput;
                if(message.endsWith("</server_emulator>")) break;
            }
            logger.debug(message);
            logger.debug("Message received successfully");
            return message;
        }finally {
            out.close();
            stdIn.close();
            disconnect();
        }
    }
    /**
     * Disconnects from EmulatorServer by closing connected socket
     * @throws IOException socket close I/O error
     */
    public void disconnect() throws IOException {
        logger.debug("Disconnecting from server");
        socket.close();
    }
}
