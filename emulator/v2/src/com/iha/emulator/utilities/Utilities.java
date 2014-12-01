package com.iha.emulator.utilities;

import com.iha.emulator.Main;
import com.iha.emulator.ui.simulations.detailed.DetailedSimulationPresenter;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;
import java.util.prefs.Preferences;

/**
 * Created by Shu on 24.11.2014.
 */
public class Utilities {
    private static final Logger logger = LogManager.getLogger(Utilities.class);

    private static final String PATH_TO_RESOURCES = "/com/iha/emulator/resources/";
    private static Utilities instance;
    Preferences prefs = Preferences.userNodeForPackage(Main.class);

    public static Utilities getInstance(){
        if(instance==null){
            instance = new Utilities();
        }
        return instance;
    }

    public Properties loadDefaultProperties(String propertiesFileName) throws IOException {
        logger.trace("Loading default settings from .properties");
        InputStream input;
        Properties prop = new Properties();
        input = getClass().getResourceAsStream(PATH_TO_RESOURCES + propertiesFileName);
        prop.load(input);
        input.close();
        logger.trace("OK");
        return prop;
    }

    public Properties loadUserProperties(Properties defaultProperties){
        //last opened file location
        logger.trace("Loading user settings from Preferences");
        String lastOpenedFile = prefs.get("lastOpenedFile",null);
        if(lastOpenedFile != null) {
            defaultProperties.setProperty("lastOpenedFile",lastOpenedFile);
        }
        logger.trace("OK");
        return defaultProperties;
    }

    public void setLastOpenedFile(String path) {
        logger.trace("Setting last opened file: " + path);
        if (path != null) {
            prefs.put("lastOpenedFile",path);
        }else{
            prefs.remove("lastOpenedFile");
        }
    }
}
