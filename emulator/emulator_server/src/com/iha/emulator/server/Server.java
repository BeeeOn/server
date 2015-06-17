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
 * Class providing logic of concurrent server. New {@link com.iha.emulator.server.Worker} is given to every client
 * that connects to EmulatorServer.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class Server implements Runnable{
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(Server.class);
    /** server port */
    private Integer         port;
    /** properties from configuration file */
    private Properties      properties;
    /** socket for server */
    private ServerSocket    serverSocket;
    /** thread running server */
    private Thread          runningThread;
    /** running flag */
    private boolean         running = true;
    /** database information from properties */
    private DatabaseInfo dbInfo;

    /**
     * Server opens socket on {@link #port} and starts to listen for new clients. Every client, that connects,
     * will be given new {@link com.iha.emulator.server.Worker} to process request.
     */
    @Override
    public void run() {
        logger.info("Starting server on port: " + port);
        //set thread for server
        synchronized(this){
            this.runningThread = Thread.currentThread();
        }
        //open server socket
        try{
            openServerSocket();
        }catch (RuntimeException e){
            logger.fatal("Cannot start server",e);
            this.stop();
        }
        //keep running
        while(isRunning()){
            Socket clientSocket;
            try {
                //accept connection on server socket
                logger.info("Waiting on client to connect...");
                clientSocket = this.serverSocket.accept();
            } catch (IOException e) {
                if(!isRunning()) {
                    logger.info("Server stopped!");
                    return;
                }
                throw new RuntimeException("Error accepting client connection", e);
            }
            //when new client connects, give him new Worker
            logger.debug("Client connected -> " + clientSocket.getInetAddress().getHostAddress());
            new Thread(new Worker(clientSocket,dbInfo)).start();
        }
        logger.info("Server stopped!");
    }

    /**
     * Gets if server is running
     * @return <code>true</code> if server is running, <code>false</code> otherwise
     */
    private synchronized boolean isRunning() {
        return this.running;
    }

    /**
     * Stops server from running and closes server socker
     */
    public synchronized void stop(){
        this.running = false;
        try {
            this.serverSocket.close();
        } catch (IOException e) {
            throw new RuntimeException("Error closing server", e);
        }
    }

    /**
     * Open socket on {@link #port}, where will server listen for new requests
     */
    private void openServerSocket() {
        try {
            logger.debug("Opening server socket");
            this.serverSocket = new ServerSocket(this.port);
        } catch (IOException e) {
            throw new RuntimeException("Cannot open port " + port, e);
        }
    }

    /**
     * Load properties from configuration file given as parameter
     *
     * @param file name of configuration file
     * @throws FileNotFoundException cannot find given filename
     * @throws IOException cannot open or read given file
     */
    public void loadProperties(String file) throws FileNotFoundException,IOException {
        InputStream input;
        //create properties
        properties = new Properties();
        //check if file exists
        if(!checkIfExists(file)){
            throw new FileNotFoundException("Properties file \""+file+"\" NOT found!");
        }
        //open it
        input = new FileInputStream(file);
        //load properties
        properties.load(input);
        //close file
        input.close();
    }

    /**
     * Initializes server by setting needed runtime variables from loaded properties. Also creates default
     * {@link com.iha.emulator.server.DatabaseInfo} from properties.
     *
     * @throws NullPointerException cannot find properties
     * @throws IllegalArgumentException wrong configuration file format, more information in exception message
     * @throws NumberFormatException cannot convert server port to Integer number
     */
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
        //reading db info
        logger.trace("Reading database info from properties file");
        dbInfo = new DatabaseInfo(properties.getProperty("dbHost"),properties.getProperty("dbPort"),properties.getProperty("dbUser"),properties.getProperty("dbPass"));
    }
    /**
     * Sets log4j2 logger level for application.
     * @param levelString level as string
     */
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

    /**
     * Checks, if file with given filename exists
     *
     * @param filename filename to be checked
     * @return <code>true</code> if file exists, <code>false</code> otherwise
     */
    private boolean checkIfExists(String filename){
        File file = new File(filename);
        return file.exists();
    }

}
