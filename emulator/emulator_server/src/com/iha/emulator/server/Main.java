package com.iha.emulator.server;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.FileNotFoundException;
import java.io.IOException;

public class Main {

    private static final Logger logger = LogManager.getLogger(Server.class);
    private static final String DEFAULT_PROPERTIES_FILE = "server.properties";

    public static void main(String[] args) {
        Server server = new Server();
        try {
            server.loadProperties(DEFAULT_PROPERTIES_FILE);
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
