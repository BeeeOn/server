package com.iha.emulator.ui.simulations.detailed;

import com.iha.emulator.control.AdapterController;
import com.iha.emulator.models.Sensor;
import com.iha.emulator.models.Server;
import com.iha.emulator.models.value.Value;
import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.dialogs.adapter.AddAdapterDialogPresenter;
import com.iha.emulator.ui.panels.adapter.AdapterButton;
import com.iha.emulator.ui.panels.adapter.details.AdapterDetailsPresenter;
import com.iha.emulator.ui.panels.sensor.SensorPanelPresenter;
import com.iha.emulator.ui.panels.server.details.ServerDetailsPresenter;
import com.iha.emulator.utilities.AdapterLogger;
import com.iha.emulator.utilities.MemoryChecker;
import com.iha.emulator.utilities.TextAreaAppender;
import com.iha.emulator.utilities.Utilities;
import javafx.application.Platform;
import javafx.beans.binding.BooleanBinding;
import javafx.beans.property.ListProperty;
import javafx.beans.property.LongProperty;
import javafx.beans.property.SimpleListProperty;
import javafx.beans.property.SimpleLongProperty;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.concurrent.Task;
import javafx.event.EventHandler;
import javafx.fxml.FXMLLoader;
import javafx.fxml.LoadException;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.FlowPane;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.stage.StageStyle;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.controlsfx.dialog.ExceptionDialog;
import org.controlsfx.dialog.ProgressDialog;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Properties;

/**
 * Created by Filip Sutovsky on 14/11/14.
 */
public class DetailedSimulationPresenter implements Presenter{

    private static final Logger logger = LogManager.getLogger(DetailedSimulationPresenter.class);

    private static final String FXML_PATH = "DetailedSimulation.fxml";
    private static final String CSS_PATH = "/com/iha/emulator/resources/css/theme-light.css";

    private MemoryChecker memoryChecker = MemoryChecker.getInstance();
    private Properties properties;

    //region PRESENTERS
    private ServerDetailsPresenter serverDetailsPresenter;
    private AdapterDetailsPresenter adapterDetailsPresenter;
    //endregion
    //region ADAPTERS
    private AdapterController currentAdapterController;
    private ObservableList<AdapterController> adapterControllers = FXCollections.observableArrayList();
    private ListProperty<AdapterController> adapterControllersList = new SimpleListProperty<>(adapterControllers);
    //endregion

    public interface Display {
        public void setPresenter(DetailedSimulationPresenter presenter);
        public Node getView();
        public void setStatusLine(String status);
        public void setStatusIndicator(boolean visible);
        public ProgressBar getMemCheckProgressBar();
        public Label getMemCheckStatusLabel();
        public void setAdapterBtns(boolean enableBtnDisabled,boolean disableBtnDisabled);
        public void setInternetBtns(boolean enableBtnDisabled,boolean disableBtnDisabled);
        public TextArea getApplicationLogTextArea();
        public void addServerDetailsView(Node serverDetailsView);
        public void addAdapterDetailsView(Node adapterDetailsView);
        public void addAdapterBtn(AdapterButton newBtn);
        public MenuItem getNewSensorSubItem();
        public MenuItem getNewAdapterSubItem();
        public ArrayList<AdapterButton> getAdapterBtns();
        public Button getEnableAdapterBtn();
        public Button getDisableAdapterBtn();
        public Button getEnableInternetBtn();
        public Button getDisableInternetBtn();
        public Button getSaveBtn();
        public Button getSaveAllBtn();
        public Button getPrintBtn();
        public Node getAdapterLogContainer();
        public Node getToBeSentLogContainer();
        public Node getErrorLogContainer();
        public RadioButton getFullLogMessageRadBtn();
        public RadioButton getPartialLogMessageRadBtn();
        public RadioButton getShortLogMessageRadBtn();
        public Button getAddNewSensorBtn();
        public FlowPane getSensorPanelContainer();
    }
    //region variables
    private Display view;
    //endregion

    //region constructor
    public DetailedSimulationPresenter() {
    }
    //endregion

    //region public methods
    /** TODO erase i*/
    private int i = 0;

    public void addNewAdapter(){
        logger.debug("Creating new adapter");

        //define background process
        Task<Object> worker = new Task<Object>() {
            @Override
            protected Object call() throws Exception {
                LongProperty progress = new SimpleLongProperty(0);
                progress.addListener(new ChangeListener<Number>() {
                    @Override
                    public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                            if((Long)newValue > 0 && (Long)newValue < 100)
                                updateProgress((long)newValue,100);
                    }
                });
                ObservableList<Server> tmp;
                try{
                    tmp = Utilities.buildServersFromProperties(properties,progress);
                    final ObservableList<Server> finalTmp = tmp;
                    Platform.runLater(() -> showAddAdapterDialog(finalTmp));
                }catch (IllegalArgumentException e){
                    Platform.runLater(() -> showAddAdapterDialog(null));
                    Platform.runLater(() -> showException("Cannot load settings from properties file" ,e,false));
                }
                return null;
            }
        };
        showLoadingDialog(worker,"Loading...");


        //check if list of adapter controllers exist
        AdapterController newAdapterController = null;
        try {
            //create new adapter controller
             newAdapterController = new AdapterController();
            //add it to list
            adapterControllers.add(newAdapterController);
            //create new adapter
            newAdapterController.createAdapter(false,51910+i,false,0.1,Double.valueOf(getProperty("defaultFirmware")));
            //create new server
            newAdapterController.createServer(false,"devel","10.1.0.1",9092,"home4");
            //create logger
            newAdapterController.createLog();
            //config log
            newAdapterController.getLog().setType(AdapterLogger.toType(getProperty("defaultLogMessageType")));
            //add new adapter button
            addAdapterBtn(newAdapterController);
            // set new adapter as current
            setCurrentAdapter(newAdapterController);
        } catch (IllegalArgumentException e){
            adapterControllers.remove(newAdapterController);
            showException("Cannot create adapter. Error in properties file. Please review file an start application again.", e,true);
        }
        i++;
    }

    private void showAddAdapterDialog(ObservableList<Server> servers){
        AddAdapterDialogPresenter addAdapterDialogPresenter;
        try{
            Stage stage = new Stage();
            addAdapterDialogPresenter = new AddAdapterDialogPresenter(stage,servers);
            stage.setTitle("Add new adapter");
            Scene scene = new Scene((Parent) addAdapterDialogPresenter.loadView());
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            stage.setScene(scene);
            stage.setResizable(false);
            stage.show();
        } catch (IOException e) {
            showException("Cannot load dialog for adding adapter!",e,false);
        }
    }

    public void addNewSensor(){
        logger.debug("Creating new sensor");
        if(currentAdapterController != null){
            try {
                currentAdapterController.createSensor(view.getSensorPanelContainer(),false,31231231,"new sensor",80,70,3600,currentAdapterController.getAdapter().getProtocol());
            } catch (LoadException e) {
                showException("Cannot create new sensor",e,false);
            }
        }
    }

    public void enableCurrentAdapter(){
        if(currentAdapterController != null) currentAdapterController.start();
    }

    public void disableCurrentAdapter(){
        if(currentAdapterController != null) currentAdapterController.stop();
    }

    public void enableInternetConnection(){
        if(currentAdapterController != null) currentAdapterController.setInternetConnection(true);
    }

    public void disableInternetConnection(){
        if(currentAdapterController != null) currentAdapterController.setInternetConnection(false);
    }

    public void setCurrentAdapter(AdapterController controller){
        if(currentAdapterController!= null){
            if(controller.equals(currentAdapterController)) return;
        }
        setStatus("Setting current adapter ->" + controller.getAdapter().getId(),true);
        //deselect old adapter button
        view.getAdapterBtns().stream().filter(aBtn -> aBtn.getController().equals(currentAdapterController)).forEach(aBtn -> {
            aBtn.setSelected(false);
        });
        //hide old sensor panels and show actual
        switchSensorPanels(currentAdapterController, controller);

        // select new adapter button
        view.getAdapterBtns().stream().filter(aBtn -> aBtn.getController().equals(controller)).forEach(aBtn -> {
            aBtn.setSelected(true);
        });
        //change current controller
        this.currentAdapterController = controller;
        //assign adapter data model to GUI
        adapterDetailsPresenter.addModel(currentAdapterController.getAdapter());
        //assign server data model to GUI
        serverDetailsPresenter.addModel(currentAdapterController.getServer());
        //bind adapter control buttons
        bindAdapterControlButtons();
        //bind internet control buttons
        bindInternetControlBtns();
        //bind log areas
        bindLogs();
        //bind log message type radio buttons to variables
        bindLogMessageType();
        //bind number of registered sensors to variable
        bindRegisteredCount();
        setStatus("Adapter " + currentAdapterController.getAdapter().getId() + " / " + currentAdapterController.getAdapter().getName() + " set as current",false);
    }

    private void switchSensorPanels(AdapterController oldController, AdapterController newController){
        //remove old sensor panels from sensor panel container
        if(oldController != null && oldController.getSensorPanelPresenters() != null && oldController.getSensorPanelPresenters().size() != 0){
            for (SensorPanelPresenter presenter: oldController.getSensorPanelPresenters()){
                view.getSensorPanelContainer().getChildren().remove(presenter.getView());
            }
        }
        //add sensor panels from adapter to be set as current
        if(newController.getSensorPanelPresenters() != null && newController.getSensorPanelPresenters().size() != 0) {
            for(SensorPanelPresenter presenter: newController.getSensorPanelPresenters()){
                view.getSensorPanelContainer().getChildren().add(0,presenter.getView());
            }
        }

    }

    private void addAdapterBtn(AdapterController controller){
        view.addAdapterBtn(new AdapterButton(controller));
    }

    public void saveCurrentAdapter(){
        /*currentAdapterController.getLog().log("Full message","Partial message","Short message");
        currentAdapterController.getLog().sent("new line");
        currentAdapterController.getLog().error("new line");*/
        for (Sensor sensor : currentAdapterController.getSensors()){
            for (Value value: sensor.getValues()){
                value.nextValue();
            }
        }
    }

    public void saveAllAdapters(){
        currentAdapterController.getLog().notifyDataSent();
    }

    public void init(String defaultPropertiesFileName){
        logger.info("Initialization");
        //application initialisation
        startMemoryCheck();
        //load properties from .properties file
        loadProperties(defaultPropertiesFileName);
        //point logger to textArea
        TextAreaAppender.setTextFlow(view.getApplicationLogTextArea());
        //init presenters and add their views to GUI
        initPresentersAndViews();
        //disable toolbar buttons
        view.setAdapterBtns(true,true);
        view.setInternetBtns(true, true);
        view.getSaveBtn().setDisable(true);
        view.getSaveAllBtn().setDisable(true);
        view.getPrintBtn().setDisable(true);
        //bind adapter file control buttons (Save, SaveAll, Print),others are bounded to current adapter
        bindControlBtnsToAdaptersCount();
    }

    private void bindAdapterControlButtons(){
        if(currentAdapterController != null){
            view.getEnableAdapterBtn().disableProperty().bind(currentAdapterController.getAdapter().statusProperty());
            view.getDisableAdapterBtn().disableProperty().bind(new BooleanBinding() {
                {
                    bind(currentAdapterController.getAdapter().statusProperty());
                }
                @Override
                protected boolean computeValue() {
                    if(currentAdapterController.getAdapter().getStatus())
                        return false;
                    else
                        return true;
                }
            });
        }
    }

    private void bindInternetControlBtns(){
        if(currentAdapterController != null){
            view.getEnableInternetBtn().disableProperty().bind(currentAdapterController.internetConnectionProperty());
            view.getDisableInternetBtn().disableProperty().bind(new BooleanBinding() {
                {
                    bind(currentAdapterController.internetConnectionProperty());
                }
                @Override
                protected boolean computeValue() {
                    if(currentAdapterController.getInternetConnection())
                        return false;
                    else
                        return true;
                }
            });
        }
    }

    private void bindLogs(){
        if(currentAdapterController!= null && currentAdapterController.getLog() != null){
            AdapterLogger log = currentAdapterController.getLog();
            log.addAdapterLogTo(view.getAdapterLogContainer());
            log.addSentLogTo(view.getToBeSentLogContainer());
            log.addErrorLogTo(view.getErrorLogContainer());
        }
    }

    private void bindControlBtnsToAdaptersCount(){
        view.getSaveBtn().disableProperty().bind(new BooleanBinding() {
            {
                bind(adapterControllersList.sizeProperty());
            }
            @Override
            protected boolean computeValue() {
                if(adapterControllersList.size() > 0){
                    return false;
                }else {
                    return true;
                }
            }
        });
        view.getSaveAllBtn().disableProperty().bind(new BooleanBinding() {
            {
                bind(adapterControllersList.sizeProperty());
            }
            @Override
            protected boolean computeValue() {
                if(adapterControllersList.size() > 0){
                    return false;
                }else {
                    return true;
                }
            }
        });
        view.getPrintBtn().disableProperty().bind(new BooleanBinding() {
            {
                bind(adapterControllersList.sizeProperty());
            }
            @Override
            protected boolean computeValue() {
                if(adapterControllersList.size() > 0){
                    return false;
                }else {
                    return true;
                }
            }
        });
        view.getAddNewSensorBtn().visibleProperty().bind(new BooleanBinding() {
            {
                bind(adapterControllersList.sizeProperty());
            }
            @Override
            protected boolean computeValue() {
                if(adapterControllersList.size() > 0){
                    return true;
                }else{
                    return false;
                }
            }
        });
        view.getNewSensorSubItem().disableProperty().bind(new BooleanBinding() {
            {
                bind(adapterControllersList.sizeProperty());
            }
            @Override
            protected boolean computeValue() {
                if(adapterControllersList.size() > 0){
                    return false;
                }else{
                    return true;
                }
            }
        });
    }

    private void bindLogMessageType(){
        view.getFullLogMessageRadBtn().selectedProperty().bindBidirectional(currentAdapterController.getLog().fullMessageProperty());
        view.getPartialLogMessageRadBtn().selectedProperty().bindBidirectional(currentAdapterController.getLog().partialMessageProperty());
        view.getShortLogMessageRadBtn().selectedProperty().bindBidirectional(currentAdapterController.getLog().shortMessageProperty());
    }

    private void bindRegisteredCount(){
        //bind adapters registered sensor count to size of sensors array
        adapterDetailsPresenter.bindRegisteredCount(currentAdapterController.getSensors().sizeProperty());
    }

    private void initPresentersAndViews(){
        //load server details
        try {
            serverDetailsPresenter = new ServerDetailsPresenter();
            view.addServerDetailsView(serverDetailsPresenter.loadView());
        } catch (IOException e) {
            showException("Cannot load Server Details",e,true);
        }
        try {
            adapterDetailsPresenter = new AdapterDetailsPresenter();
            view.addAdapterDetailsView(adapterDetailsPresenter.loadView());
        } catch (IOException e) {
            showException("Cannot load Adapter Details",e,true);
        }

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
            logger.trace("OK");
            return scene;
        }
        finally
        {
            if (fxmlStream != null) fxmlStream.close();
        }
    }

    private void loadProperties(String defaultPropertiesFileName){
        /** FIXGUI change progress dialog css */
        setStatus("Loading application settings",true);
        //define background process
        Task<Object> worker = new Task<Object>() {
            @Override
            protected Object call() throws Exception {
                //update progress bar
                updateProgress(1,99);
                //update progress message
                updateMessage("Loading application settings...");
                //load properties
                try{
                    properties = Utilities.getInstance().loadDefaultProperties(defaultPropertiesFileName);
                    updateProgress(50,99);
                    //unlikely to happen, just to be sure
                    if(properties == null){
                        throw new IOException("Properties is null");
                    }
                    updateProgress(99, 99);
                    //update Status line
                    Platform.runLater(() -> setStatus("Application settings successfully loaded.",false));
                }catch (IOException e){
                    //show exception dialog
                    Platform.runLater(() -> showException("Cannot load properties. PLease review properties file and run application again",e,true));
                }
                return null;
            }
        };
        showLoadingDialog(worker,"Loading...");
    }

    private void showLoadingDialog(Task worker,String title){
        //create progress dialog
        ProgressDialog dlg = new ProgressDialog(worker);
        dlg.setTitle(title);
        dlg.getDialogPane().setHeaderText("");
        dlg.initStyle(StageStyle.UNIFIED);
        //create thread for background process
        Thread th = new Thread(worker);
        th.setDaemon(true);
        //run background process
        th.start();
    }

    private String getProperty(String property) throws IllegalArgumentException{
        if(properties == null) throw new IllegalArgumentException("Properties not found");
        String tmp = properties.getProperty(property);
        if( tmp == null){
            throw new IllegalArgumentException("Missing property \"" + property + "\" in properties file");
        }else{
            return tmp;
        }
    }

    public void setStatus(String status,boolean indicate){
        view.setStatusLine(status);
        view.setStatusIndicator(indicate);
        logger.info(status);
    }

    public void startMemoryCheck(){
        if(view != null && memoryChecker != null){
            logger.trace("Starting memory check");
            //assign where to show memory usage
            memoryChecker.setGuiElements(view.getMemCheckProgressBar(),view.getMemCheckStatusLabel());
            //start checking
            memoryChecker.start();
        }
    }

    public void showException(String header,Exception e,boolean closeApp){
        logger.error(e.getMessage(),e);
        //create exception dialog
        ExceptionDialog dlg = new ExceptionDialog(e);
        //define default header
        if (header == null) header = "Ooops. Something went wrong!.";
        dlg.getDialogPane().setHeaderText(header);
        dlg.initStyle(StageStyle.DECORATED);
        if(closeApp){
            dlg.setOnCloseRequest(new EventHandler<DialogEvent>() {
                @Override
                public void handle(DialogEvent event) {
                    quit();
                }
            });
        }
        //show exception dialog
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
