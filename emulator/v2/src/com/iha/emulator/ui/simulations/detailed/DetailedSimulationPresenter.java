package com.iha.emulator.ui.simulations.detailed;

import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.communication.server.ServerListener;
import com.iha.emulator.control.AdapterController;
import com.iha.emulator.control.SensorController;
import com.iha.emulator.models.Adapter;
import com.iha.emulator.models.Server;
import com.iha.emulator.models.value.Value;
import com.iha.emulator.models.value.ValueFactory;
import com.iha.emulator.resources.images.sensor_types.SensorIcon;
import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.dialogs.adapter.AddAdapterDialogPresenter;
import com.iha.emulator.ui.dialogs.adapter.DeleteAdaptersDialogPresenter;
import com.iha.emulator.ui.dialogs.log.ShowFullLogPresenter;
import com.iha.emulator.ui.dialogs.sensor.AddNewSensorDialogPresenter;
import com.iha.emulator.ui.dialogs.sensor.DeleteSensorsDialogPresenter;
import com.iha.emulator.ui.panels.adapter.AdapterButton;
import com.iha.emulator.ui.panels.adapter.details.AdapterDetailsPresenter;
import com.iha.emulator.ui.panels.server.details.ServerDetailsPresenter;
import com.iha.emulator.utilities.AdapterLogger;
import com.iha.emulator.utilities.MemoryChecker;
import com.iha.emulator.utilities.TextAreaAppender;
import com.iha.emulator.utilities.Utilities;
import javafx.animation.Animation;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
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
import javafx.util.Duration;
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
    private static final boolean DEBUG_AUTO_CREATE = true;
    private static final int DEFAULT_SERVER_LISTENER_PORT = 7978;

    private static final String FXML_PATH = "DetailedSimulation.fxml";
    private static final String CSS_PATH = "/com/iha/emulator/resources/css/theme-light.css";

    private Scene scene;

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
    private ServerListener serverListener;

    public interface Display {
        public void setPresenter(DetailedSimulationPresenter presenter);
        public Node getView();
        public TabPane getLogTabPane();
        public Tab getErrorTab();
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
        public MenuItem getNewSensorItem();
        public MenuItem getEnableAdapterItem();
        public MenuItem getDisableAdapterItem();
        public MenuItem getDisableAllAdaptersItem();
        public MenuItem getDeleteAdapterItem();
        public MenuItem getDeleteSensorItem();
        public ArrayList<AdapterButton> getAdapterBtns();
        public Button getEnableAdapterBtn();
        public Button getDisableAdapterBtn();
        public Button getEnableInternetBtn();
        public Button getDisableInternetBtn();
        public Button getDeleteAdapterBtn();
        public Button getSaveBtn();
        public Button getSaveAllBtn();
        public Button getPrintBtn();
        public Node getAdapterLogContainer();
        public Node getToBeSentLogContainer();
        public Node getErrorLogContainer();
        public ToggleGroup getLogMessageTypeRadBtnGroup();
        public RadioButton getFullLogMessageRadBtn();
        public RadioButton getPartialLogMessageRadBtn();
        public RadioButton getShortLogMessageRadBtn();
        public Button getShowFullLogBtn();
        public Button getAddNewSensorBtn();
        public FlowPane getSensorPanelContainer();
        public void removeAdapterBtn(AdapterController adapterController);
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
        if(!DEBUG_AUTO_CREATE){
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
                        Platform.runLater(() -> showException(logger,"Cannot load settings from properties file" ,e,false,null));
                    }
                    return null;
                }
            };
            showLoadingDialog(worker,"Loading...");
        }else{
            //check if list of adapter controllers exist
            AdapterController newAdapterController = null;
            try {
                //create new adapter controller
                newAdapterController = new AdapterController();
                //add it to list
                adapterControllersList.add(newAdapterController);
                //create new adapter
                newAdapterController.createAdapter(false,99999-i,false, Protocol.Version.ZERO_POINT_ONE,Double.valueOf(getProperty("defaultFirmware")));
                //create new server
                newAdapterController.createServer(false,"production","10.1.0.1",9092,"home4prod");
                //create logger
                newAdapterController.createLog();
                //config log
                newAdapterController.getLog().setType(AdapterLogger.toType(getProperty("defaultLogMessageType")));
                //create scheduler
                newAdapterController.createScheduler();
                //set response tracking
                newAdapterController.setTrackServerResponse(true);
                newAdapterController.setDumpServerResponse(true);
                //bind scheduler processing to adapter's status indicator
                newAdapterController.bindSchedulerProcess(newAdapterController.getAdapter(), newAdapterController.getScheduler());
                //bind register message
                newAdapterController.bindRegisterMessage(newAdapterController);
                //add new adapter button
                addAdapterBtn(newAdapterController);
                // set new adapter as current
                setCurrentAdapter(newAdapterController);
                try {
                    newAdapterController.getLog().setBuffered(true,String.valueOf(newAdapterController.getAdapter().getId()));
                } catch (IOException e) {
                    showException(logger,"Cannot create buffer file for new adapter log.",e,true,event -> quit());
                }
            } catch (IllegalArgumentException e){
                adapterControllersList.remove(newAdapterController);
                showException(
                        logger,
                        "Cannot create adapter. Error in properties file. Please review file an start application again.",
                        e,
                        true,
                        event -> quit());
            }
            i++;
        }

    }

    private void showAddAdapterDialog(ObservableList<Server> servers){
        AddAdapterDialogPresenter addAdapterDialogPresenter;
        try{
            Stage stage = new Stage();
            addAdapterDialogPresenter = new AddAdapterDialogPresenter(stage,servers,this,getProperty("defaultFirmware"));
            stage.setTitle("Add new adapter");
            Scene scene = new Scene((Parent) addAdapterDialogPresenter.loadView());
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            stage.setScene(scene);
            stage.setResizable(false);
            stage.show();
        } catch (IOException e) {
            showException(logger,"Cannot load dialog for adding adapter!",e,false,null);
        } catch (IllegalArgumentException ei){
            showException(logger,"Cannot create adapter. Error in properties file. Please review file an start application again.",ei,true,event->quit());
        }
    }

    public void addNewSensor(){
        logger.debug("Creating new sensor");
        if(!DEBUG_AUTO_CREATE){
            showAddSensorDialog();
        }else {
            if(currentAdapterController != null){
                try {
                    ObservableList<Value> tmpValues = FXCollections.observableArrayList();

                    Value actValue = ValueFactory.buildValue(Value.Type.ACTUATOR_ON_OFF);
                    tmpValues.addAll(actValue);
                    /*Value tempValue = ValueFactory.buildValue(Value.Type.SENSOR_TEMPERATURE);
                    tempValue.setGenerateValue(true);
                    ((TemperatureSensorValue) tempValue).setGeneratorType(Value.Generator.NORMAL_DISTRIBUTION);
                    Value humValue = ValueFactory.buildValue(Value.Type.SENSOR_HUMIDITY);
                    humValue.setGenerateValue(true);
                    ((HumiditySensorValue) humValue).setGeneratorType(Value.Generator.NORMAL_DISTRIBUTION);
                    //Value actValue = ValueFactory.buildValue(Value.Type.ACTUATOR_ON_OFF);
                    tmpValues.addAll(tempValue,humValue);*/
                    currentAdapterController.createSensor(view.getSensorPanelContainer(),"#0cdf56", SensorIcon.MULTI_SENSOR,tmpValues,false,1677721601+i,"Test sensor",90,91,5,currentAdapterController.getAdapter().getProtocol());
                    i++;
                } catch (LoadException e) {
                    showException(logger,"Cannot create new sensor",e,false,null);
                }
            }
        }
    }

    private void showAddSensorDialog(){
        AddNewSensorDialogPresenter addNewSensorDialogPresenter;
        try{
            Stage stage = new Stage();
            if(currentAdapterController == null) throw new NullPointerException("No current adapter");
            addNewSensorDialogPresenter = new AddNewSensorDialogPresenter(stage,view.getSensorPanelContainer(),currentAdapterController);
            stage.setTitle("Add new sensor / actuator");
            Scene scene = new Scene((Parent) addNewSensorDialogPresenter.loadView());
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            stage.setScene(scene);
            stage.initModality(Modality.APPLICATION_MODAL);
            stage.setResizable(false);
            stage.show();
        } catch (IOException e) {
            showException(logger,"Cannot load dialog for adding sensor!",e,false,null);
        } catch (NullPointerException en){
            showException(logger,"Cannot create new sensor",en,false,null);
        }
    }

    public void showFullLog(){
        ShowFullLogPresenter showFullLogPresenter;
        try{
            Stage stage = new Stage();
            if(currentAdapterController == null) return;
            showFullLogPresenter = new ShowFullLogPresenter(stage,currentAdapterController.getAdapter());
            stage.setTitle("Full log for adapter: " + currentAdapterController.getAdapter().getName() + " / " + String.valueOf(currentAdapterController.getAdapter().getId()));
            Scene scene = new Scene((Parent) showFullLogPresenter.loadView());
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            stage.setScene(scene);
            stage.initModality(Modality.APPLICATION_MODAL);
            stage.setResizable(true);
            stage.show();
        } catch (IOException e) {
            showException(logger,"Cannot load dialog showing full log!",e,false,null);
        }
    }


    public void enableCurrentAdapter(){
        if(currentAdapterController != null) currentAdapterController.enable();
    }

    public void disableCurrentAdapter(){
        if(currentAdapterController != null) currentAdapterController.disable();
    }

    public void disableAllAdapters(){
        adapterControllersList.forEach(com.iha.emulator.control.AdapterController::disable);
    }

    public void deleteAdapter(){
        DeleteAdaptersDialogPresenter deleteAdaptersDialogPresenter;
        try{
            Stage stage = new Stage();
            deleteAdaptersDialogPresenter = new DeleteAdaptersDialogPresenter(stage,this);
            stage.setTitle("Delete adapters");
            Scene scene = new Scene((Parent) deleteAdaptersDialogPresenter.loadView());
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            stage.setScene(scene);
            stage.setResizable(false);
            stage.show();
        } catch (IOException e) {
            showException(logger,"Cannot load dialog for adding adapter!",e,false,null);
        } catch (IllegalArgumentException ei){
            showException(logger,"Cannot create adapter. Error in properties file. Please review file an start application again.",ei,true,event->quit());
        }
    }

    public void deleteAdapter(AdapterController adapterController){
        logger.trace("Deleting adapter from emulator " + adapterController.toString());
        adapterController.disable();
        if(adapterControllersList.get().size() > 1){
            logger.trace("Removing logger");
        }else {
            logger.trace("Clearing adapter and server details");
            //clear adapter details
            adapterDetailsPresenter.clear();
            //clear server details
            serverDetailsPresenter.clear();
            //clear logs
            adapterController.getLog().clearContainers();
        }
        logger.trace("Removing adapter button");
        removeAdapterBtn(adapterController);
        logger.trace("Removing adapter controller");
        adapterController.delete();
        logger.trace("Removing adapter from list");
        adapterControllersList.get().remove(adapterController);
        if(adapterControllersList.get().size() != 0){
            logger.trace("Setting next current adapter");
            setCurrentAdapter(adapterControllersList.get().get((0)));
        }else {
            setCurrentAdapter(null);
        }
        logger.trace("ADAPTERS LIST COUNT: " + adapterControllersList.size());
        System.gc();
    }

    public void deleteSensor(){
        if(currentAdapterController == null) return;
        DeleteSensorsDialogPresenter deleteSensorsDialogPresenter;
        try{
            Stage stage = new Stage();
            deleteSensorsDialogPresenter = new DeleteSensorsDialogPresenter(stage,currentAdapterController);
            stage.setTitle("Delete sensors");
            Scene scene = new Scene((Parent) deleteSensorsDialogPresenter.loadView());
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            stage.setScene(scene);
            stage.setResizable(false);
            stage.show();
        } catch (IOException e) {
            showException(logger,"Cannot load dialog for adding adapter!",e,false,null);
        } catch (IllegalArgumentException ei){
            showException(logger,"Cannot create adapter. Error in properties file. Please review file an start application again.",ei,true,event->quit());
        }
    }

    public void enableInternetConnection(){
        if(currentAdapterController != null) currentAdapterController.setInternetConnection(true);
    }

    public void disableInternetConnection(){
        if(currentAdapterController != null) currentAdapterController.setInternetConnection(false);
    }

    public void setCurrentAdapter(AdapterController controller){
        if(currentAdapterController!= null && controller != null){
            if(controller.equals(currentAdapterController)) return;
        }
        if(controller == null){
            view.getEnableAdapterBtn().disableProperty().unbind();
            view.getDisableAdapterBtn().disableProperty().unbind();
            view.getEnableAdapterItem().disableProperty().unbind();
            view.getDisableAdapterItem().disableProperty().unbind();

            view.getEnableInternetBtn().disableProperty().unbind();
            view.getDisableInternetBtn().disableProperty().unbind();

            view.getEnableAdapterBtn().setDisable(true);
            view.getDisableAdapterBtn().setDisable(true);
            view.getEnableAdapterItem().setDisable(true);
            view.getDisableAdapterItem().setDisable(true);

            view.getEnableInternetBtn().setDisable(true);
            view.getDisableInternetBtn().setDisable(true);
            this.currentAdapterController = null;
            return;
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
        //bind log message type radio buttons to variables
        //bindLogMessageType(controller);
        //change current controller
        this.currentAdapterController = controller;
        //assign adapter data model to GUI
        adapterDetailsPresenter.addModel(currentAdapterController.getAdapter());
        //assign server data model to GUI
        serverDetailsPresenter.addModel(currentAdapterController.getServerController().getModel());
        //bind adapter control buttons
        bindAdapterControlButtons();
        //bind internet control buttons
        bindInternetControlBtns();
        //bind log areas
        bindLogs();

        //bind number of registered sensors to variable
        bindRegisteredCount();
        setStatus("Adapter " + currentAdapterController.getAdapter().getId() + " / " + currentAdapterController.getAdapter().getName() + " set as current",false);
    }

    private void switchSensorPanels(AdapterController oldController, AdapterController newController){
        //remove old sensor panels from sensor panel container
        if(oldController != null && oldController.getSensorControllersList() != null && oldController.getSensorControllersList().size() != 0){
            for(SensorController controller : oldController.getSensorControllersList()){
                view.getSensorPanelContainer().getChildren().remove(controller.getPanel().getView());
            }
        }
        //add sensor panels from adapter to be set as current
        if(newController.getSensorControllersList() != null && newController.getSensorControllersList().size() != 0) {
            for(SensorController controller : newController.getSensorControllersList()){
                view.getSensorPanelContainer().getChildren().add(0,controller.getPanel().getView());
            }
        }
    }

    public void addAdapterBtn(AdapterController controller){
        view.addAdapterBtn(new AdapterButton(controller));
    }

    public void removeAdapterBtn(AdapterController controller){
        view.removeAdapterBtn(controller);
    }

    public void saveCurrentAdapter(){

    }

    public void saveAllAdapters(){

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
        //initialize server listener
        initAndStartServerListener();
        //add handlers to message type change
        /*view.getLogMessageTypeRadBtnGroup().selectedToggleProperty().addListener(new ChangeListener<Toggle>() {
            @Override
            public void changed(ObservableValue<? extends Toggle> observable, Toggle oldValue, Toggle newValue) {
                if(currentAdapterController == null) return;
                currentAdapterController.getLog().setFullMessage(view.getFullLogMessageRadBtn().isSelected());
                currentAdapterController.getLog().setPartialMessage(view.getPartialLogMessageRadBtn().isSelected());
                currentAdapterController.getLog().setShortMessage(view.getShortLogMessageRadBtn().isSelected());
            }
        });*/
    }

    private void initAndStartServerListener(){
        if(properties != null){
            serverListener = new ServerListener(Integer.valueOf(properties.getProperty("serverListenerPort")),adapterControllersList);
        }else{
            serverListener = new ServerListener(DEFAULT_SERVER_LISTENER_PORT,adapterControllersList);
        }
        serverListener.setDaemon(true);
        try {
            serverListener.connect();
        } catch (IOException e) {
            showException(logger,"Cannot start listening for connections",e,false,null);
        }
        serverListener.start();
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
        BooleanBinding adapterListZeroReturnTrue = new BooleanBinding() {
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
        };
        view.getSaveBtn().disableProperty().bind(adapterListZeroReturnTrue);
        view.getSaveAllBtn().disableProperty().bind(adapterListZeroReturnTrue);
        view.getPrintBtn().disableProperty().bind(adapterListZeroReturnTrue);
        view.getNewSensorSubItem().disableProperty().bind(adapterListZeroReturnTrue);
        view.getNewSensorItem().disableProperty().bind(adapterListZeroReturnTrue);
        view.getEnableAdapterItem().disableProperty().bind(adapterListZeroReturnTrue);
        view.getDisableAdapterItem().disableProperty().bind(adapterListZeroReturnTrue);
        view.getDisableAllAdaptersItem().disableProperty().bind(adapterListZeroReturnTrue);
        view.getDeleteAdapterBtn().disableProperty().bind(adapterListZeroReturnTrue);
        view.getDeleteAdapterItem().disableProperty().bind(adapterListZeroReturnTrue);
        view.getDeleteSensorItem().disableProperty().bind(adapterListZeroReturnTrue);
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
    }

    private void bindLogMessageType(AdapterController newController){
        /*if(currentAdapterController != null){
            logger.debug("Current adapter message type: "
                    + " F-> " + currentAdapterController.getLog().getFullMessage() + " "
                    + " P-> " + currentAdapterController.getLog().getPartialMessage() + " "
                    + " S-> " + currentAdapterController.getLog().getShortMessage() + " ");
            if(currentAdapterController.getLog().fullMessageProperty().isBound())
                currentAdapterController.getLog().fullMessageProperty().unbindBidirectional(view.getFullLogMessageRadBtn().selectedProperty());
            if(currentAdapterController.getLog().partialMessageProperty().isBound())
                currentAdapterController.getLog().partialMessageProperty().unbindBidirectional(view.getPartialLogMessageRadBtn().selectedProperty());
            if(currentAdapterController.getLog().shortMessageProperty().isBound())
                currentAdapterController.getLog().shortMessageProperty().unbindBidirectional(view.getShortLogMessageRadBtn().selectedProperty());
        }

        logger.debug("New adapter message type: "
                + " F-> " + newController.getLog().getFullMessage() + " "
                + " P-> " + newController.getLog().getPartialMessage() + " "
                + " S-> " + newController.getLog().getShortMessage() + " ");*/
        view.getFullLogMessageRadBtn().setSelected(newController.getLog().getFullMessage());
        view.getPartialLogMessageRadBtn().setSelected(newController.getLog().getPartialMessage());
        view.getShortLogMessageRadBtn().setSelected(newController.getLog().getShortMessage());
        /*view.getFullLogMessageRadBtn().selectedProperty().bind(currentAdapterController.getLog().fullMessageProperty());
        view.().selectedProperty().bind(currentAdapterController.getLog().partialMessageProperty());
        view.getShortLogMessageRadBtn().selectedProperty().bind(currentAdapterController.getLog().shortMessageProperty());*/
        //unbind radio buttons if they are bound

        //set message type according to currently set adapter

        //bind message type to adapter's log
        /*view.getFullLogMessageRadBtn().selectedProperty().bindBidirectional(currentAdapterController.getLog().fullMessageProperty());
        view.getPartialLogMessageRadBtn().selectedProperty().bindBidirectional(currentAdapterController.getLog().partialMessageProperty());
        view.getShortLogMessageRadBtn().selectedProperty().bindBidirectional(currentAdapterController.getLog().shortMessageProperty());*/
        /*newController.getLog().fullMessageProperty().bindBidirectional(view.getFullLogMessageRadBtn().selectedProperty());
        newController.getLog().partialMessageProperty().bindBidirectional(view.getPartialLogMessageRadBtn().selectedProperty());
        newController.getLog().shortMessageProperty().bindBidirectional(view.getShortLogMessageRadBtn().selectedProperty());*/
    }

    private void bindRegisteredCount(){
        //bind adapters registered sensor count to size of sensors array
        adapterDetailsPresenter.bindRegisteredCount(currentAdapterController.getSensorControllersList().sizeProperty());
    }

    private void initPresentersAndViews(){
        //load server details
        try {
            serverDetailsPresenter = new ServerDetailsPresenter();
            view.addServerDetailsView(serverDetailsPresenter.loadView());
        } catch (IOException e) {
            showException(logger,"Cannot load Server Details", e, true, event -> quit());
        }
        try {
            adapterDetailsPresenter = new AdapterDetailsPresenter();
            view.addAdapterDetailsView(adapterDetailsPresenter.loadView());
        } catch (IOException e) {
            showException(logger,"Cannot load Adapter Details",e,true, event -> quit());
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
            scene = new Scene((Parent)view.getView());
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
                    Platform.runLater(() -> showException(logger,"Cannot load properties. PLease review properties file and run application again",e,true,event -> quit()));
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

    public String getProperty(String property) throws IllegalArgumentException{
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

    public static void showException(Logger log,String header,Exception e,boolean closeApp,EventHandler<DialogEvent> onCloseEvent){
        log.error(e.getMessage(),e);
        //create exception dialog
        ExceptionDialog dlg = new ExceptionDialog(e);
        //define default header
        if (header == null) header = "Ooops. Something went wrong!.";
        dlg.getDialogPane().setHeaderText(header);
        dlg.initStyle(StageStyle.DECORATED);
        if(closeApp && onCloseEvent != null){
            dlg.setOnCloseRequest(onCloseEvent);
        }
        //show exception dialog
        dlg.show();
    }

    public void showError(String message,String title) {
        Alert dlg = new Alert(Alert.AlertType.ERROR, message);
        dlg.initModality(Modality.WINDOW_MODAL);
        dlg.setTitle(title);
        dlg.show();
    }

    public ObservableList<AdapterController> getAdapterControllersList() {
        return adapterControllersList;
    }

    public ObservableList<AdapterController> getAdapterControllers() {
        return adapterControllersList.get();
    }


    @Override
    public void bind() {
        view.setPresenter(this);
    }

    public void quit(){
        memoryChecker.stop();
        if(serverListener != null) serverListener.terminateServerListener();
        Platform.exit();
    }
    //endregion

}
