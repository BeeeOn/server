package com.iha.emulator.utilities.server;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.*;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

/**
 * Created by Shu on 6.12.2014.
 */
public class EmulatorServerClient {

    private static final Logger logger = LogManager.getLogger(EmulatorServerClient.class);
    private static final String HOST_IP = "localhost"; //147.229.176.131
    private static final int HOST_PORT = 6983;

    private Socket socket;
    private InetAddress address;

    public EmulatorServerClient() throws UnknownHostException {
        logger.trace("Getting host address");
        this.address = InetAddress.getByName(HOST_IP);
        logger.trace("  OK");
    }

    public void connect() throws IOException {
        logger.debug("Connecting to server");
        socket = new Socket(address,HOST_PORT);
        //socket.setSoTimeout(5000);
        logger.debug("Connection established successfully");
    }

    public void sendMessage(String message) throws IOException {
        PrintWriter out = new PrintWriter(socket.getOutputStream(),true);
        BufferedReader stdIn = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        try {
            logger.debug("Sending message:");
            logger.debug(message);
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
        }finally {
            out.close();
            stdIn.close();
            disconnect();
        }

    }

    public void disconnect() throws IOException {
        logger.debug("Disconnecting from server");
        socket.close();
    }
}
