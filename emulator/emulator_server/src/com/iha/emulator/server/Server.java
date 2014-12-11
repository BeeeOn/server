package com.iha.emulator.server;

import org.apache.logging.log4j.Level;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.core.LoggerContext;
import org.apache.logging.log4j.core.config.Configuration;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Properties;

/**
 * Created by Shu on 4.12.2014.
 */
public class Server implements Runnable{

    private static final Logger logger = LogManager.getLogger(Server.class);

    private Integer         port;
    private Properties      properties;
    private ServerSocket    serverSocket;
    private Thread          runningThread;
    private boolean         running = true;

    public Server() {

    }

    @Override
    public void run() {
        logger.info("Starting server on port: " + port);
        synchronized(this){
            this.runningThread = Thread.currentThread();
        }
        try{
            openServerSocket();
        }catch (RuntimeException e){
            logger.fatal("Cannot start server",e);
            this.stop();
        }

        while(isRunning()){
            Socket clientSocket;
            try {
                logger.info("Waiting on client to connect...");
                clientSocket = this.serverSocket.accept();
            } catch (IOException e) {
                if(!isRunning()) {
                    logger.info("Server stopped!");
                    return;
                }
                throw new RuntimeException("Error accepting client connection", e);
            }
            logger.debug("Client connected -> " + clientSocket.getInetAddress().getHostAddress());
            new Thread(new Worker(clientSocket, "Multithreaded Server")).start();
        }
        logger.info("Server stopped!");
    }

    private synchronized boolean isRunning() {
        return this.running;
    }

    public synchronized void stop(){
        this.running = false;
        try {
            this.serverSocket.close();
        } catch (IOException e) {
            throw new RuntimeException("Error closing server", e);
        }
    }

    private void openServerSocket() {
        try {
            logger.debug("Opening server socket");
            this.serverSocket = new ServerSocket(this.port);
        } catch (IOException e) {
            throw new RuntimeException("Cannot open port " + port, e);
        }
    }

    public void loadProperties(String file) throws FileNotFoundException,IOException {
        InputStream input;
        properties = new Properties();
        if(!checkIfExists(file)){
            throw new FileNotFoundException("Properties file \"server.properties\" NOT found!");
        }
        input = new FileInputStream(file);
        properties.load(input);
        input.close();
    }

    public void init() throws NullPointerException,IllegalArgumentException,NumberFormatException{
        if(properties == null){
            throw new NullPointerException("Error loading properties");
        }
        logger.trace("Reading port from properties file");
        String portString = properties.getProperty("port");
        if(portString == null){
            throw new IllegalArgumentException("Missing field \"port\" in properties file");
        }
        port = Integer.valueOf(portString);
        logger.trace("Reading logger level from properties file");
        String loggerLevel = properties.getProperty("logger");
        if(loggerLevel == null){
            throw new IllegalArgumentException("Missing field \"logger\" in properties file");
        }
        logger.trace("Setting logger level");
        updateLoggerLevels(loggerLevel);
    }

    private void updateLoggerLevels(String levelString){
        Level level;
        switch (levelString.toLowerCase()){
            case "all":
                level = Level.ALL;
                break;
            case "trace":
                level = Level.TRACE;
                break;
            case "debug":
                level = Level.DEBUG;
                break;
            case "info":
                level = Level.INFO;
                break;
            case "warn":
                level = Level.WARN;
                break;
            case "error":
                level = Level.ERROR;
                break;
            case "fatal":
                level = Level.FATAL;
                break;
            case "off":
                level = Level.OFF;
                break;
            default:
                throw new IllegalArgumentException("Unknown logger level in properties file");
        }
        if(level != null){
            LoggerContext ctx = (LoggerContext) LogManager.getContext(false);
            Configuration conf = ctx.getConfiguration();
            conf.getLoggerConfig(LogManager.ROOT_LOGGER_NAME).setLevel(level);
            ctx.updateLoggers(conf);
        }
    }

    private boolean checkIfExists(String filename){
        File file = new File(filename);
        return file.exists();
    }

}
