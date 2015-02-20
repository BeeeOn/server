package com.iha.emulator.ui.simulations.detailed;

import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.communication.protocol.ProtocolFactory;
import com.iha.emulator.communication.server.ServerListener;
import com.iha.emulator.control.AdapterController;
import com.iha.emulator.control.SensorController;
import com.iha.emulator.models.Server;
import com.iha.emulator.models.value.*;
import com.iha.emulator.models.value.implemented.HasGenerator;
import com.iha.emulator.resources.images.sensor_types.SensorIcon;
import com.iha.emulator.resources.images.sensor_types.SensorIconFactory;
import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.dialogs.adapter.AddAdapterDialogPresenter;
import com.iha.emulator.ui.dialogs.adapter.ChangeAdapterDetailsDialogPresenter;
import com.iha.emulator.ui.dialogs.adapter.DeleteAdaptersDialogPresenter;
import com.iha.emulator.ui.dialogs.log.ShowFullLogPresenter;
import com.iha.emulator.ui.dialogs.sensor.AddNewSensorDialogPresenter;
import com.iha.emulator.ui.dialogs.sensor.DeleteSensorsDialogPresenter;
import com.iha.emulator.ui.dialogs.server.ChangeServerDetailsDialogPresenter;
import com.iha.emulator.ui.panels.adapter.AdapterButton;
import com.iha.emulator.ui.panels.adapter.details.AdapterDetailsPresenter;
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
import org.dom4j.*;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Properties;
import java.util.stream.Collectors;

/**
 * Created by Filip Sutovsky on 14/11/14.
 */
public class DetailedSimulationPresenter implements Presenter{

    private static final Logger logger = LogManager.getLogger(DetailedSimulationPresenter.class);
    private static final boolean DEBUG_AUTO_CREATE = false;
    private static final int DEFAULT_SERVER_LISTENER_PORT = 7978;
    private static final String SAVES_DEFAULT_DIR = "saved_adapters";
    private static final String FXML_PATH = "DetailedSimulation.fxml";
    private static final String CSS_PATH = "/com/iha/emulator/resources/css/theme-light.css";

    private Stage window;

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
    public DetailedSimulationPresenter(Stage window) {
        this.window = window;
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
                //save it to list
                adapterControllersList.add(newAdapterController);
                //create new adapter
                newAdapterController.createAdapter(false,99999-i,false, Protocol.Version.ZERO_POINT_ONE,Double.valueOf(getProperty("defaultFirmware")));
                //create new server
                newAdapterController.createServer(false,"production","10.1.0.1",9092,"home4prod");
                //create logger
                newAdapterController.createLog(getView().getLogTabPane());
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
                //save new adapter button
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
                    currentAdapterController.setSaved(false);
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
            if(currentAdapterController == null || currentAdapterController.getLog().getBufferFile() == null) return;
            showFullLogPresenter = new ShowFullLogPresenter(stage,currentAdapterController);
            stage.setTitle("Full log for adapter: " + currentAdapterController.getAdapter().getName() + " / " + String.valueOf(currentAdapterController.getAdapter().getId()));
            Scene scene = new Scene((Parent) showFullLogPresenter.loadView());
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            stage.setScene(scene);
            stage.initModality(Modality.NONE);
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
            showException(logger,"Cannot load dialog for deleting adapter!",e,false,null);
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
            showException(logger,"Cannot load dialog for deleting sensor!",e,false,null);
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
        //save sensor panels from adapter to be set as current
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

    public void saveCurrentAdapter(AdapterController adapterController){
        if(adapterController == null) {
            if(this.currentAdapterController == null) return;
            adapterController = currentAdapterController;
        }
        final AdapterController tmpController = adapterController;
        logger.trace("Saving adapter: " + tmpController.toString());
        Task<Document> worker = new Task<Document>() {
            @Override
            protected Document call() throws Exception {
                logger.trace("Creating XML file");
                Document doc = DocumentFactory.getInstance().createDocument();
                doc.addElement("adapters");
                doc = tmpController.saveToXml(doc);
                return doc;
            }
        };
        worker.setOnSucceeded(event -> {
            logger.trace("Trying to save to XML file");
            String filename = Utilities.saveDialogForXML(window, SAVES_DEFAULT_DIR, worker.getValue().asXML());
            if(filename != null){
                showInformation("File saved", "Adapter/s successfully saved", "Saved to file \"" + filename + "\"");
                tmpController.setSaved(true);
            }
        });
        //create thread for background process
        Thread th = new Thread(worker);
        th.setDaemon(true);
        //run background process
        th.start();
    }

    public void saveAllAdapters(){
        if(getAdapterControllers().size() < 1) return;
        logger.trace("Saving all adapters: ");
        Task<Document> worker = new Task<Document>() {
            @Override
            protected Document call() throws Exception {
                logger.trace("Creating XML file");
                Document doc = DocumentFactory.getInstance().createDocument();
                doc.addElement("adapters");
                for(AdapterController adapterController : getAdapterControllers()){
                    doc = adapterController.saveToXml(doc);
                }
                return doc;
            }
        };
        worker.setOnSucceeded(event -> {
            logger.trace("Trying to save to XML file");
            String filename = Utilities.saveDialogForXML(window,SAVES_DEFAULT_DIR,worker.getValue().asXML());
            if(filename != null){
                showInformation("File saved", "Adapter/s successfully saved", "Saved to file \"" + filename + "\"");
                getAdapterControllers().forEach(adapterController -> adapterController.setSaved(true));
            }
        });
        //create thread for background process
        Thread th = new Thread(worker);
        th.setDaemon(true);
        //run background process
        th.start();
    }

    public void changeServerDetails(){
        if(this.currentAdapterController == null) return;
        logger.trace("Show change server details dialog.");
        ChangeServerDetailsDialogPresenter changeServerDetailsDialogPresenter;
        try{
            ObservableList<Server> tmp = null;
            try{
                tmp = Utilities.buildServersFromProperties(properties,null);

            }catch (IllegalArgumentException e){
                Platform.runLater(() -> showAddAdapterDialog(null));
                Platform.runLater(() -> showException(logger,"Cannot load settings from properties file" ,e,false,null));
            }
            Stage stage = new Stage();
            if(currentAdapterController == null) throw new NullPointerException("No current adapter");
            changeServerDetailsDialogPresenter = new ChangeServerDetailsDialogPresenter(stage,tmp,currentAdapterController);
            stage.setTitle("Change server details");
            Scene scene = new Scene((Parent) changeServerDetailsDialogPresenter.loadView());
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            stage.setScene(scene);
            stage.initModality(Modality.APPLICATION_MODAL);
            stage.setResizable(false);
            stage.show();
        } catch (IOException e) {
            showException(logger,"Cannot load server details dialog!",e,false,null);
        } catch (NullPointerException en){
            showException(logger,"Cannot load server",en,false,null);
        }
    }

    public void changeAdapterDetails(){
        if(this.currentAdapterController == null) return;
        logger.trace("Show change adapter details dialog.");
        ChangeAdapterDetailsDialogPresenter changeAdapterDetailsDialogPresenter;
        try{
            Stage stage = new Stage();
            if(currentAdapterController == null) throw new NullPointerException("No current adapter");
            changeAdapterDetailsDialogPresenter = new ChangeAdapterDetailsDialogPresenter(stage,currentAdapterController);
            stage.setTitle("Change adapter details");
            Scene scene = new Scene((Parent) changeAdapterDetailsDialogPresenter.loadView());
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            stage.setScene(scene);
            stage.initModality(Modality.APPLICATION_MODAL);
            stage.setResizable(false);
            stage.show();
        } catch (IOException e) {
            showException(logger,"Cannot load adapter details dialog!",e,false,null);
        } catch (NullPointerException en){
            showException(logger,"Cannot load adapter",en,false,null);
        }

    }

    public void open(){
        logger.trace("Trying to load from XML file");
        try {
            String content = Utilities.loadDialogForXML(window, SAVES_DEFAULT_DIR);
            if(content == null) return;
            parseAndLoadXML(content);
        } catch (DocumentException e) {
            DetailedSimulationPresenter.showException(logger, "Cannot parse loaded file", e, false, null);
        }
    }

    private void parseAndLoadXML(String content) throws DocumentException {
        logger.trace("Parsing XML file");
        Document doc = DocumentHelper.parseText(content);
        //root element = <adapters>
        Element rootElement = doc.getRootElement();
        for(Iterator<Element> it = rootElement.elementIterator("adapter"); it.hasNext();){
            Element adapterElement = it.next();
            logger.trace("XML -> Parsing adapter: " + adapterElement.attribute("id").getValue());
            AdapterController tmpAdapterController = null;
            try{
                //parse adapter info
                logger.trace("XML -> Parsing adapter info");
                Integer adapterId = Integer.valueOf(adapterElement.attribute("id").getValue());
                if(Utilities.isAdapterIdTaken(getAdapterControllers(),adapterId)) throw new IllegalArgumentException("Adapter with id \"" + adapterId + "\" already exist!");
                String adapterName = adapterElement.attribute("name").getValue();
                Double adapterProtocolVersion = Double.valueOf(adapterElement.attribute("protocol").getValue());
                Boolean adapterRegistered = Boolean.valueOf(adapterElement.attributeValue("registered"));
                Double adapterFirmware = Double.valueOf(adapterElement.attributeValue("firmware"));
                if(adapterId == null || adapterName == null || adapterProtocolVersion == null ||adapterRegistered == null ||adapterFirmware == null)
                    throw new NullPointerException("Adapter info missing");

                tmpAdapterController = new AdapterController();
                Protocol.Version protocolVersion = ProtocolFactory.getVersion(adapterProtocolVersion);
                if(protocolVersion == null)
                    throw new NullPointerException("Unknown protocol version -> " + adapterProtocolVersion);
                //CREATE ADAPTER
                tmpAdapterController.createAdapter(adapterName,false,adapterId,adapterRegistered, protocolVersion ,adapterFirmware);
                logger.trace("XML -> Adapter info OK -> ID: " + adapterId + " Name: " + adapterName + " Prot.: " + adapterProtocolVersion);
                //parse server info
                logger.trace("XML -> Parsing server info");
                Element serverElement = adapterElement.element("server");
                String serverName = serverElement.attribute("name").getValue();
                String serverIp = serverElement.attribute("ip").getValue();
                Integer serverPort = Integer.valueOf(serverElement.attribute("port").getValue());
                String serverDb = serverElement.attribute("db").getValue();
                if(serverName == null || serverIp == null || serverPort == null || serverDb == null)
                    throw new NullPointerException("Adapter info missing");
                //CREATE SERVER
                tmpAdapterController.createServer(false,serverName,serverIp,serverPort,serverDb);
                logger.trace("XML -> Server info OK -> Name: " + serverName + " IP: " + serverIp + " Port: " + serverPort + " DB: " + serverDb);

                //parse sensors
                logger.trace("XML -> Parsing sensors");
                Element sensorsElement = adapterElement.element("sensors");
                for(Iterator<Element> its = sensorsElement.elementIterator("sensor");its.hasNext();) {
                    Element sensorElement = its.next();
                    logger.trace("XML -> Parsing sensor -> " + sensorElement.attribute("id").getValue());
                    Integer sensorId = Integer.valueOf(sensorElement.attributeValue("id"));
                    String sensorName = sensorElement.elementText("name");
                    Integer sensorRefresh = Integer.valueOf(sensorElement.elementText("refresh"));
                    Integer sensorSignal = Integer.valueOf(sensorElement.elementText("signal"));
                    Integer sensorBattery = Integer.valueOf(sensorElement.elementText("battery"));
                    SensorIcon sensorIcon = SensorIconFactory.getByName(sensorElement.elementText("icon"));
                    String sensorColor = sensorElement.elementText("header_color");
                    if (sensorId == null || sensorName == null || sensorRefresh == null || sensorSignal == null || sensorBattery == null || sensorIcon == null || sensorColor == null) {
                        throw new NullPointerException("Sensor info missing");
                    }
                    Element valuesElement = sensorElement.element("values");
                    ObservableList<Value> values = FXCollections.observableArrayList();
                    for (Iterator<Element> itv = valuesElement.elementIterator("value"); itv.hasNext(); ) {
                        Element valueElement = itv.next();
                        logger.trace("XML -> Parsing sensor -> " + sensorId + " Parsing value -> " + valueElement.attributeValue("type"));
                        String valueType = valueElement.attributeValue("type");
                        String valueName = valueElement.attributeValue("name");
                        Boolean valueStoreHistory = Boolean.valueOf(valueElement.attributeValue("store_history"));
                        Boolean valueGenerateValue = Boolean.valueOf(valueElement.attributeValue("generate_value"));
                        String valueInitialValue = valueElement.elementText("initial_value");
                        if(valueType == null || valueName == null || valueStoreHistory == null || valueGenerateValue == null || valueInitialValue == null){
                            throw new NullPointerException("Value missing info");
                        }
                        //CREATE VALUE
                        Value tmpValue = ValueFactory.buildValue(valueType);
                        tmpValue.setName(valueName);
                        tmpValue.setStoreHistory(valueStoreHistory);
                        tmpValue.setGenerateValue(valueGenerateValue);
                        tmpValue.setInitialValue(tmpValue.fromStringToValueType(valueInitialValue));
                        tmpValue.setValue(tmpValue.fromStringToValueType(valueInitialValue));
                        Element generatorElement = null;
                        try{
                            generatorElement = valueElement.element("generator");
                        }catch (NullPointerException e){
                            //it is OK, if generator element doesn't exist
                        }
                        if(generatorElement != null){
                            String generatorType = generatorElement.attributeValue("type");
                            Value.Generator generator = ValueFactory.generatorByName(generatorType);
                            if(generator == null) throw new NullPointerException("Unknown generator type -> " + generatorType);
                            ((HasGenerator)tmpValue).setGeneratorType(generator);
                            switch (generator) {
                                case NORMAL_DISTRIBUTION:
                                    ((HasNormalDistribution)tmpValue).setMax(Double.valueOf(generatorElement.element("params").attributeValue("max")));
                                    ((HasNormalDistribution)tmpValue).setMin(Double.valueOf(generatorElement.element("params").attributeValue("min")));
                                    ((HasNormalDistribution)tmpValue).setDev(Double.valueOf(generatorElement.element("params").attributeValue("dev")));
                                    ((HasNormalDistribution)tmpValue).setAvg(Double.valueOf(generatorElement.element("params").attributeValue("avg")));
                                    break;
                                case LINEAR_DISTRIBUTION:
                                    ((HasLinearDistribution)tmpValue).setMax(Double.valueOf(generatorElement.element("params").attributeValue("max")));
                                    ((HasLinearDistribution)tmpValue).setMin(Double.valueOf(generatorElement.element("params").attributeValue("min")));
                                    ((HasLinearDistribution)tmpValue).setStep(Double.valueOf(generatorElement.element("params").attributeValue("step")));
                                    break;
                                case BOOLEAN_RANDOM:
                                    ((HasBooleanRandom)tmpValue).setProbability((Double.valueOf(generatorElement.element("params").attributeValue("probability"))));
                                    break;
                            }
                            Long generatorSeed = Long.valueOf(generatorElement.attributeValue("seed"));
                            tmpValue.setGeneratorSeed(generatorSeed);
                        }
                        values.add(tmpValue);
                    }
                    //CREATE SENSOR
                    SensorController tmpSensor =tmpAdapterController.createSensor(values,false,sensorId,sensorName,sensorBattery,sensorSignal,sensorRefresh,tmpAdapterController.getAdapter().getProtocol());
                    tmpAdapterController.createSensorPanel(view.getSensorPanelContainer(),sensorColor,sensorIcon,tmpSensor);
                    //tmpAdapterController.createSensor(view.getSensorPanelContainer(),sensorColor,sensorIcon,values,false,sensorId,sensorName,sensorBattery,sensorSignal,sensorRefresh,tmpAdapterController.getAdapter().getProtocol());
                }
            }catch (LoadException e){
                throw new DocumentException("Wrong format of file. Cannot create sensor " + adapterElement.attribute("id").getValue(),e);
            }catch (NumberFormatException e){
                throw new DocumentException("Wrong format of file. Error on in content of adapter " + adapterElement.attribute("id").getValue(),e);
            }catch (NullPointerException e){
                throw new DocumentException("Wrong format of file. Cannot find one or more required elements. Error on in content of adapter " + adapterElement.attribute("id").getValue(),e);
            }catch (IllegalArgumentException e ){
                showException(logger,"Adapter id already exist",e,false,null);
            }
            if(tmpAdapterController != null && tmpAdapterController.getAdapter() != null){
                //CREATE LOG
                tmpAdapterController.createLog(getView().getLogTabPane());
                //CREATE SCHEDULER
                tmpAdapterController.createScheduler();
                tmpAdapterController.setTrackServerResponse(true);
                tmpAdapterController.setDumpServerResponse(true);
                tmpAdapterController.bindSchedulerProcess(tmpAdapterController.getAdapter(),tmpAdapterController.getScheduler());
                tmpAdapterController.bindRegisterMessage(tmpAdapterController);
                //CREATE ADAPTER BUTTON
                addAdapterBtn(tmpAdapterController);
                //ADD ADAPTER TO OTHERS
                getAdapterControllersList().add(tmpAdapterController);
                try{
                    tmpAdapterController.getLog().setBuffered(true,String.valueOf(tmpAdapterController.getAdapter().getId()));
                }catch (IOException e){
                    throw new DocumentException("Wrong format of file. Error on in content of adapter " + adapterElement.attribute("id").getValue(),e);
                }
                tmpAdapterController.setSaved(true);
            }
        }
        if(getAdapterControllers().size() > 0){
            setCurrentAdapter(getAdapterControllers().get(0));
        }
    }

    public void init(String defaultPropertiesFileName){
        logger.info("Initialization");
        //application initialisation
        startMemoryCheck();
        //load properties from .properties file
        loadProperties(defaultPropertiesFileName);
        //point logger to textArea
        TextAreaAppender.setTextFlow(view.getApplicationLogTextArea());
        //init presenters and save their views to GUI
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
        //handle close event
        window.setOnCloseRequest(event -> {
            checkIfSaved();
            if(getUnsavedAdapters() == null){
                quit();
            }else {
                event.consume();
            }
        });
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
                    return !currentAdapterController.getAdapter().getStatus();
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

    private void showInformation(String title,String headerMessage,String message){
        logger.trace("Showing information");
        Alert dlg = new Alert(Alert.AlertType.INFORMATION, "");
        dlg.initModality(Modality.WINDOW_MODAL);
        dlg.initOwner(this.window);
        dlg.setTitle(title);
        dlg.getDialogPane().setContentText(message);
        dlg.getDialogPane().setHeaderText(headerMessage);
        dlg.show();
    }

    private void bindControlBtnsToAdaptersCount(){
        BooleanBinding adapterListZeroReturnTrue = new BooleanBinding() {
            {
                bind(adapterControllersList.sizeProperty());
            }
            @Override
            protected boolean computeValue() {
                return adapterControllersList.size() <= 0;
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
                return adapterControllersList.size() > 0;
            }
        });
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

    private void loadProperties(String defaultPropertiesFileName){
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

    private ObservableList<AdapterController> getUnsavedAdapters(){
        ArrayList<AdapterController> unsavedAdapters = null;
        if(getAdapterControllers().size() > 0){
             unsavedAdapters =
                    getAdapterControllers()
                            .stream()
                            .filter(a -> !a.isSaved())
                            .collect(Collectors.toCollection(ArrayList<AdapterController>::new));
        }
        if(unsavedAdapters == null || unsavedAdapters.size() == 0){
            return null;
        }
        else{
            logger.trace("Unsaved adapters count: " + unsavedAdapters.size());
            return FXCollections.observableArrayList(unsavedAdapters);
        }
    }

    private void checkIfSaved(){
        logger.debug("Checking unsaved adapters");
        if(getAdapterControllers().size() > 0){
            ObservableList<AdapterController> unsavedAdapters = getUnsavedAdapters();
            if(unsavedAdapters != null){
                ChoiceDialog<Utilities.SaveOption> dlg = Utilities.saveOnQuitDialog();
                dlg.showAndWait().ifPresent(result -> {
                    switch (result){
                        case SAVE_ALL:
                            saveAllAdapters();
                            break;
                        case SAVE_CURRENT:
                            saveCurrentAdapter(null);
                            break;
                        case DO_NOTHING:
                            unsavedAdapters.forEach(a -> a.setSaved(true));
                            break;
                    }
                });
            }
        }
    }

    private void removeTempFiles(){
        if(getAdapterControllers().size() < 1) return;
        getAdapterControllers().stream().filter(a->a.getLog().getBufferFile()!=null).forEach(a->a.getLog().closeBuffer());
    }

    public Display getView(){
        return this.view;
    }

    public void quit(){
        memoryChecker.stop();
        if(serverListener != null) serverListener.terminateServerListener();
        removeTempFiles();
        Platform.exit();
    }
    //endregion

}
