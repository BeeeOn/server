package com.iha.emulator.ui.simulations.chooser;

import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.dialogs.thread.ThreadTesterPresenter;
import com.iha.emulator.ui.simulations.detailed.DetailedSimulationPresenter;
import com.iha.emulator.ui.simulations.performance.PerformanceSimulationPresenter;
import com.iha.emulator.utilities.Utilities;
import javafx.application.HostServices;
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
 * Class providing logic to user interactions for "Application simulation chooser". Part Presenter of MVP design pattern.
 * Also serving as entry point of application.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class ChooserPresenter implements Presenter {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(ChooserPresenter.class);
    /** path to FXML file */
    private static final String FXML_PATH = "Chooser.fxml";
    /** path to CSS file */
    private static final String CSS_PATH = "/com/iha/emulator/resources/css/theme-light.css";
    /** view */
    private Display view;
    /** window */
    private Stage window;
    /** properties loaded from configuration file */
    private Properties loadedProperties;
    /** host services for application */
    private HostServices hostServices;
    /**
     * Interface implemented by "Application simulation chooser" view.
     */
    public interface Display {
        public void setPresenter(ChooserPresenter presenter);
        public Node getView();
    }

    /**
     * Creates new "Application simulation chooser" presenter.
     * @param window parent window
     */
    public ChooserPresenter(Stage window) {
        this.window = window;
        logger.info("Running JVM bits: " + System.getProperty("sun.arch.data.model"));
    }

    /**
     * Invokes "Detailed simulation".
     */
    public void runDetailedSimulation(){
        logger.info("Starting Detailed Simulation");
        //create presenter
        DetailedSimulationPresenter dPresenter = new DetailedSimulationPresenter(window);
        //set services
        dPresenter.setHostServices(hostServices);
        window.setTitle("BeeeOn Emulator - Detailed Simulation");
        try {
            //create and initialize scene
            Scene dScene = dPresenter.loadView();
            if(dScene != null) window.setScene(dScene);
            else{
                throw new IOException("Detailed Simulation scene is null");
            }
            //load scene to window
            window.centerOnScreen();
        } catch (IOException e) {
            Utilities.showException(logger, "Cannot load Detailed Simulation.", e, true, event1 -> {
                dPresenter.quit();
            });
        }
        //initialize "Detailed simulation"
        dPresenter.init(loadedProperties);
    }

    /**
     * Invokes "Performance simulation".
     */
    public void runPerformanceSimulation(){
        logger.info("Starting Performance Simulation");
        //create presenter
        PerformanceSimulationPresenter pPresenter = new PerformanceSimulationPresenter(window);
        //set services
        pPresenter.setHostServices(hostServices);
        window.setTitle("BeeeOn Emulator - Performance Simulation");
        try {
            //create and initialize scene
            Scene dScene = pPresenter.loadView();
            if(dScene != null) window.setScene(dScene);
            else{
                throw new IOException("Performance Simulation scene is null");
            }
            //load scene to window
            window.centerOnScreen();
        } catch (IOException e) {
            Utilities.showException(logger, "Cannot load Performance Simulation.", e, true, event1 -> {
                pPresenter.quit();
            });
        }
        //initialize "Performance simulation"
        pPresenter.init(loadedProperties);
    }

    /**
     * Invokes "Max thread test".
     */
    public void runMaxThreadTest(){
        ThreadTesterPresenter tPresenter;
        window.setTitle("BeeeOn Emulator - Max thread tester");
        try{
            //create presenter
            tPresenter = new ThreadTesterPresenter(window);
            //create and initialize scene
            Scene scene = new Scene((Parent) tPresenter.loadView());
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            window.setScene(scene);
            window.setResizable(true);
            window.show();
            //show window
            tPresenter.init();
        } catch (IOException e) {
            Utilities.showException(logger, "Cannot load dialog for thread tester!", e, false, null);
        }
    }

    /**
     * Initializes dialog. Loads properties from configuration file. Also set's log4j2 logger level loaded from properties.
     *
     * @param defaultPropertiesFileName deafult properties filename
     */
    public void init(String defaultPropertiesFileName){
        logger.info("Initialization");
        //load properties from .properties file
        Task<Properties> worker = Utilities.loadProperties(defaultPropertiesFileName);
        worker.run();
        worker.setOnSucceeded(event -> {
            loadedProperties = worker.getValue();
            try{
                Utilities.setLoggerLevel(Utilities.getProperty(loadedProperties, "logLevel"));
            }catch (IllegalArgumentException e){
                Utilities.showException(logger, "Cannot retrieve \"logLevel\" from properties. Setting logger to level INFO", e, false, null);
                Utilities.setLoggerLevel("INFO");
            }
        });
        worker.setOnFailed(event -> {
            Utilities.showException(logger, "Cannot load properties. Please review properties file and run application again", null, true, event1 -> quit());
        });
    }
    /**
     * {@inheritDoc}
     */
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
    /**
     * {@inheritDoc}
     */
    @Override
    public void bind() {
        view.setPresenter(this);
    }

    /**
     * Quit the application
     */
    public void quit(){
        Platform.exit();
    }

    /**
     * Gets application host services
     * @return application host services
     */
    public HostServices getHostServices() {
        return hostServices;
    }

    /**
     * Sets application host services
     * @param hostServices application host services
     */
    public void setHostServices(HostServices hostServices) {
        this.hostServices = hostServices;
    }
}
