package com.iha.emulator.ui.simulations.detailed;

import com.iha.emulator.ui.Presenter;
import com.iha.emulator.utilities.MemoryChecker;
import com.iha.emulator.utilities.Utilities;
import javafx.application.Platform;
import javafx.concurrent.Task;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Label;
import javafx.scene.control.ProgressBar;
import javafx.stage.Modality;
import javafx.stage.StageStyle;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.controlsfx.dialog.ExceptionDialog;
import org.controlsfx.dialog.ProgressDialog;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

/**
 * Created by Filip Sutovsky on 14/11/14.
 */
public class DetailedSimulationPresenter implements Presenter{

    private static final Logger logger = LogManager.getLogger(DetailedSimulationPresenter.class);

    private static final String FXML_PATH = "DetailedSimulation.fxml";
    private static final String CSS_PATH = "DetailedSimulation.css";

    private MemoryChecker memoryChecker = MemoryChecker.getInstance();
    protected Utilities utilities = Utilities.getInstance();
    private Properties properties;

    public interface Display {
        public void setPresenter(DetailedSimulationPresenter presenter);
        public Node getView();
        public void setStatusLine(String status);
        public void setStatusIndicator(boolean visible);
        public ProgressBar getMemCheckProgressBar();
        public Label getMemCheckStatusLabel();
        public void setAdapterBtns(boolean enableBtnDisabled,boolean disableBtnDisabled);
        public void setInternetBtns(boolean enableBtnDisabled,boolean disableBtnDisabled);
    }

    //region variables
    private DetailedSimulationView view;
    //endregion

    //region constructor
    public DetailedSimulationPresenter() {
        logger.trace("trace message");
    }
    //endregion

    //region public methods
    public void init(String defaultPropertiesFileName){
        logger.info("Initialization");
        startMemoryCheck();
        loadProperties(defaultPropertiesFileName);
        view.setAdapterBtns(false,false);
        view.setInternetBtns(false,false);
    }
    /**
     *  Load view from FXML file{@link com.iha.emulator.ui.simulations.detailed.DetailedSimulationPresenter#FXML_PATH} and after that
     *  bind the view with a presenter. Also assigns CSS file {@link com.iha.emulator.ui.simulations.detailed.DetailedSimulationPresenter#CSS_PATH}
     *   and creates {@link javafx.scene.Scene}, which is returned.
     *   @return scene created from loaded view
     * */
    public Scene loadView() throws IOException{
        logger.trace("Loading DetailedSimulationView from: " + FXML_PATH);
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
            Scene scene = new Scene((Parent)view.getView());
            // bind view and presenter
            bind();
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            return scene;
        }
        finally
        {
            if (fxmlStream != null) fxmlStream.close();
        }
    }

    private void loadProperties(String defaultPropertiesFileName){
        /** FIXGUI change progress dialog css */
        logger.info("Loading application settings");
        Task<Object> worker = new Task<Object>() {
            @Override
            protected Object call() throws Exception {
                updateProgress(1,99);
                updateMessage("Loading default application settings...");
                try{
                    properties = utilities.loadDefaultProperties(defaultPropertiesFileName);
                    if(properties == null){
                        throw new IOException("Properties is null");
                    }
                }catch (IOException e){
                    Platform.runLater(() -> showException("",e));
                }
                updateProgress(50,99);
                updateMessage("Loading user settings...");
                properties = utilities.loadUserProperties(properties);
                updateProgress(99, 99);
                Platform.runLater(() -> setStatus("Application settings loaded",false));
                logger.info("Application settings successfully loaded.");
                return null;
            }
        };
        ProgressDialog dlg = new ProgressDialog(worker);
        dlg.setTitle("Loading...");
        dlg.getDialogPane().setHeaderText("");
        dlg.initStyle(StageStyle.UNIFIED);
        Thread th = new Thread(worker);
        th.setDaemon(true);
        th.start();
    }

    public void setStatus(String status,boolean indicate){
        view.setStatusLine(status);
        view.setStatusIndicator(indicate);
    }

    public void startMemoryCheck(){
        if(view != null && memoryChecker != null){
            logger.trace("Starting memory check");
            memoryChecker.setGuiElements(view.getMemCheckProgressBar(),view.getMemCheckStatusLabel());
            memoryChecker.start();
        }
    }

    public void showException(String header,Exception e){
        logger.error(e.getMessage(),e);
        ExceptionDialog dlg = new ExceptionDialog(e);
        if (header == null) header = "Ooops. Something went wrong!.";
        dlg.getDialogPane().setHeaderText(header);
        dlg.initStyle(StageStyle.DECORATED);
        dlg.show();
    }

    public void showError(String message,String title){
        Alert dlg = new Alert(Alert.AlertType.ERROR,message);
        dlg.initModality(Modality.WINDOW_MODAL);
        dlg.setTitle(title);
        dlg.show();
    }

    @Override
    public void bind() {
        view.setPresenter(this);
    }

    public void quit(){
        memoryChecker.stop();

        Platform.exit();
    }
    //endregion

}
