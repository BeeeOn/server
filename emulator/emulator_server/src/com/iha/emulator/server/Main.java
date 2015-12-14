package com.iha.emulator.server;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;

/**
 * Application providing database statements for BeeeOn Emulator.<br>
 *
 * <a href="https://ant-2.fit.vutbr.cz/projects/server/wiki/Emulator_server" target="_blank">See for more info</a>
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class Main {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(Server.class);
    /** default properties file path */
    private static final String DEFAULT_PROPERTIES_FILE = "server.properties";

    /**
     * Loads application arguments, loads properties file and starts EmulatorServer.
     * @param args arg[0] = path to properties file
     */
    public static void main(String[] args) {
        String propertiesString;
        if(args.length != 0 && args[0].endsWith(".properties")){
            propertiesString = args[0];
            logger.trace("Reading properties file from argument: " + propertiesString);
        }else{
            logger.trace("Reading default properties file");
            propertiesString = DEFAULT_PROPERTIES_FILE;
        }
        Server server = new Server();
        try {
            server.loadProperties(propertiesString);
            logger.info("Initializing server");
            server.init();
            new Thread(server).start();
        } catch (IOException ei){
            logger.fatal("Cannot load properties",ei);
            System.exit(-1);
        } catch (NumberFormatException enf){
            logger.fatal("Cannot parse \"port\" number from properties file",enf);
        } catch (NullPointerException|IllegalArgumentException en){
            logger.fatal(en.getMessage(),en);
            System.exit(-1);
        }
    }
}
