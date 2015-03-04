package com.iha.emulator.ui.simulations.chooser;

import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.simulations.detailed.DetailedSimulationPresenter;
import com.iha.emulator.ui.simulations.performance.PerformanceSimulationPresenter;
import com.iha.emulator.utilities.Utilities;
import javafx.application.Platform;
import javafx.concurrent.Task;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

/**
 * Created by Shu on 25.2.2015.
 */
public class ChooserPresenter implements Presenter {

    private static final Logger logger = LogManager.getLogger(ChooserPresenter.class);
    private static final String FXML_PATH = "Chooser.fxml";
    private static final String CSS_PATH = "/com/iha/emulator/resources/css/theme-light.css";

    private Display view;
    private Stage window;

    private Properties loadedProperties;

    public interface Display {
        public void setPresenter(ChooserPresenter presenter);
        public Node getView();
    }

    public ChooserPresenter(Stage window) {
        this.window = window;
    }

    public void runDetailedSimulation(){
        logger.info("Starting Detailed Simulation");
        DetailedSimulationPresenter dPresenter = new DetailedSimulationPresenter(window);
        window.setTitle("BeeeOn Emulator - Detailed Simulation");
        try {
            Scene dScene = dPresenter.loadView();
            if(dScene != null) window.setScene(dScene);
            else{
                throw new IOException("Detailed Simulation scene is null");
            }
            window.centerOnScreen();
        } catch (IOException e) {
            Utilities.showException(logger, "Cannot load Detailed Simulation.", e, true, event1 -> {
                dPresenter.quit();
            });
        }
        dPresenter.init(loadedProperties);
    }

    public void runPerformanceSimulation(){
        logger.info("Starting Performance Simulation");
        PerformanceSimulationPresenter pPresenter = new PerformanceSimulationPresenter(window);
        window.setTitle("BeeeOn Emulator - Performance Simulation");
        try {
            Scene dScene = pPresenter.loadView();
            if(dScene != null) window.setScene(dScene);
            else{
                throw new IOException("Performance Simulation scene is null");
            }
            window.centerOnScreen();
        } catch (IOException e) {
            Utilities.showException(logger, "Cannot load Performance Simulation.", e, true, event1 -> {
                pPresenter.quit();
            });
        }
        pPresenter.init(loadedProperties);
    }

    public void init(String defaultPropertiesFileName){
        logger.info("Initialization");
        //load properties from .properties file
        Task<Properties> worker = Utilities.loadProperties(defaultPropertiesFileName);
        worker.run();
        worker.setOnSucceeded(event -> {
            loadedProperties = worker.getValue();
            /*try{
                Utilities.setLoggerLevel(Utilities.getProperty(loadedProperties, "logLevel"));
            }catch (IllegalArgumentException e){
                Utilities.showException(logger, "Cannot retrieve \"logLevel\" from properties. Setting logger to level INFO", e, false, null);
                Utilities.setLoggerLevel("INFO");
            }*/
        });
        worker.setOnFailed(event -> {
            Utilities.showException(logger, "Cannot load properties. Please review properties file and run application again", null, true, event1 -> quit());
        });
    }

    public Scene loadView() throws IOException {
        logger.trace("Loading ChooserView from: " + FXML_PATH);
        InputStream fxmlStream = null;
        try
        {
            //load resource FXML
            fxmlStream = getClass().getResourceAsStream(FXML_PATH);
            FXMLLoader loader = new FXMLLoader();
            loader.load(fxmlStream);
            //remember view
            view = loader.getController();
            //build scene
            Scene scene = new Scene((Parent) view.getView());
            // bind view and presenter
            bind();
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            logger.trace("OK");
            return scene;
        }
        finally
        {
            if (fxmlStream != null) fxmlStream.close();
        }
    }

    @Override
    public void bind() {
        view.setPresenter(this);
    }

    public void quit(){
        Platform.exit();
    }
}
