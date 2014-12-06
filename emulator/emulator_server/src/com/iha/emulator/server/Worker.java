package com.iha.emulator.server;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.*;
import java.net.Socket;

/**
 * Created by Shu on 5.12.2014.
 */
public class Worker implements Runnable{

    private static final Logger logger = LogManager.getLogger(Server.class);

    private Socket clientSocket = null;
    private String serverText = null;

    public Worker(Socket clientSocket,String serverText) {
        this.clientSocket = clientSocket;
        this.serverText = serverText;
    }

    @Override
    public void run() {
        try {
            logger.info("Serving request from ip-> " + clientSocket.getInetAddress().getHostAddress());
            BufferedReader stdIn = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            PrintWriter out = new PrintWriter(clientSocket.getOutputStream());
            String clientInput,message = "";
            logger.debug("Receiving message from client:");
            while ((clientInput = stdIn.readLine()) != null) {
                message = message + clientInput;
                if(message.endsWith("</emulator_server>"))
                    break;
            }
            logger.debug(message);
            message  = parseMessage();
            logger.debug("Sending message:");
            logger.debug(message);
            out.println(message);
            out.flush();
            logger.debug("Message sent successfully");
            logger.info("Request from -> " + clientSocket.getInetAddress().getHostAddress() + " -> served successfully");
            stdIn.close();
            out.close();
        } catch (IOException e) {
            //report exception somewhere.
            logger.error("Cannot get communication streams",e);
        }finally {
            try {
                clientSocket.close();
            } catch (IOException e) {
                logger.error("Cannot close socket",e);
            }
        }
    }

    private String parseMessage(){
        logger.trace("Parsing message");
        return "Server responnse trasldl asldald lasl </server_emulator>";
    }
}
