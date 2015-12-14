package com.iha.emulator.ui.simulations.detailed;

import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.communication.protocol.ProtocolFactory;
import com.iha.emulator.control.AdapterController;
import com.iha.emulator.control.SensorController;
import com.iha.emulator.control.scheduler.Scheduler;
import com.iha.emulator.models.Server;
import com.iha.emulator.models.value.*;
import com.iha.emulator.resources.images.sensor_types.SensorIcon;
import com.iha.emulator.resources.images.sensor_types.SensorIconFactory;
import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.dialogs.adapter.AddAdapterDialogPresenter;
import com.iha.emulator.ui.dialogs.adapter.ChangeAdapterDetailsDialogPresenter;
import com.iha.emulator.ui.dialogs.adapter.DeleteAdaptersDialogPresenter;
import com.iha.emulator.ui.dialogs.sensor.AddNewSensorDialogPresenter;
import com.iha.emulator.ui.dialogs.sensor.DeleteSensorsDialogPresenter;
import com.iha.emulator.ui.dialogs.server.ChangeServerDetailsDialogPresenter;
import com.iha.emulator.ui.panels.adapter.AdapterButton;
import com.iha.emulator.ui.panels.adapter.details.AdapterDetailsPresenter;
import com.iha.emulator.ui.panels.server.details.ServerDetailsPresenter;
import com.iha.emulator.utilities.Utilities;
import com.iha.emulator.utilities.logging.AdapterLogger;
import com.iha.emulator.utilities.logging.TextAreaAppender;
import com.iha.emulator.utilities.watchers.MemoryChecker;
import javafx.application.HostServices;
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
import org.controlsfx.dialog.ProgressDialog;
import org.dom4j.*;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Properties;
import java.util.stream.Collectors;

/**
 * Class providing logic to user interactions for "Detailed simulation". Part Presenter of MVP design pattern.
 * Also serving main window of "Detailed simulation"
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class DetailedSimulationPresenter implements Presenter{
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(DetailedSimulationPresenter.class);
    /** default location for saving XML files */
    private static final String SAVES_DEFAULT_DIR = "saved/adapters";
    /** path to FXML file */
    private static final String FXML_PATH = "DetailedSimulation.fxml";
    /** path to CSS style file */
    private static final String CSS_PATH = "/com/iha/emulator/resources/css/theme-light.css";
    /** view */
    private Display view;
    /** window */
    private Stage window;
    /** host services */
    private HostServices hostServices;
    /** instance of memory checker responsible for checking memory usage */
    private MemoryChecker memoryChecker = MemoryChecker.getInstance();
    /** properties loaded from configuration file */
    private Properties properties;

    //region PRESENTERS
    /** server details panel */
    private ServerDetailsPresenter serverDetailsPresenter;
    /** adapter details panel */
    private AdapterDetailsPresenter adapterDetailsPresenter;
    //endregion
    //region ADAPTERS
    /** currently selected adapter */
    private AdapterController currentAdapterController;
    /** list of all created adapters */
    private ObservableList<AdapterController> adapterControllers = FXCollections.observableArrayList();
    /** adapters list property */
    private ListProperty<AdapterController> adapterControllersList = new SimpleListProperty<>(adapterControllers);
    //endregion
    /**
     * Interface implemented by "Detailed simulation" view.
     */
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

    //region constructor

    /**
     * Creates new "Detailed simulation" presenter
     * @param window parent window
     */
    public DetailedSimulationPresenter(Stage window) {
        this.window = window;
    }
    //endregion

    //region public methods

    /**
     * Creates server models from properties and invokes showing of "Add adapter dialog". Uses separate {@link javafx.concurrent.Task}
     * and shows tasks progress in progress dialog.
     */
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
                    //create server models from properties
                    tmp = Utilities.buildServersFromProperties(properties,progress);
                    final ObservableList<Server> finalTmp = tmp;
                    //show add adapter dialog with created server models
                    Platform.runLater(() -> showAddAdapterDialog(finalTmp));
                }catch (IllegalArgumentException e){
                    //show add adapter dialog without server models
                    Platform.runLater(() -> showAddAdapterDialog(null));
                    Platform.runLater(() -> Utilities.showException(logger, "Cannot load settings from properties file", e, false, null));
                }
                return null;
            }
        };
        //show progress dialog
        showLoadingDialog(worker,"Loading...");
    }

    /**
     * Shows {@link com.iha.emulator.ui.dialogs.adapter.AddAdapterDialogPresenter} in new window.
     * @param servers default server models list
     */
    private void showAddAdapterDialog(ObservableList<Server> servers){
        AddAdapterDialogPresenter addAdapterDialogPresenter;
        try{
            //create window
            Stage stage = new Stage();
            //create presenter
            addAdapterDialogPresenter = new AddAdapterDialogPresenter(stage,servers,this,Utilities.getProperty(properties, "defaultFirmware"));
            stage.setTitle("Add new adapter");
            //create and initialize scene
            Scene scene = new Scene((Parent) addAdapterDialogPresenter.loadView());
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            stage.setScene(scene);
            stage.setResizable(false);
            //show window
            stage.show();
        } catch (IOException e) {
            Utilities.showException(logger, "Cannot load dialog for adding adapter!", e, false, null);
        } catch (IllegalArgumentException ei){
            Utilities.showException(logger, "Cannot create adapter. Error in properties file. Please review file an start application again.", ei, true, event -> quit());
        }
    }

    /**
     * Shows {@link com.iha.emulator.ui.dialogs.sensor.AddNewSensorDialogPresenter} in new window. Current adapter must be
     * selected.
     */
    public void addNewSensor(){
        logger.debug("Creating new sensor");
        AddNewSensorDialogPresenter addNewSensorDialogPresenter;
        try{
            //create window
            Stage stage = new Stage();
            //if no adapter is selected, error
            if(currentAdapterController == null) throw new NullPointerException("No current adapter");
            //create presenter
            addNewSensorDialogPresenter = new AddNewSensorDialogPresenter(stage,view.getSensorPanelContainer(),currentAdapterController);
            stage.setTitle("Add new sensor / actuator");
            Scene scene = new Scene((Parent) addNewSensorDialogPresenter.loadView());
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            stage.setScene(scene);
            stage.initModality(Modality.APPLICATION_MODAL);
            stage.setResizable(false);
            //show window
            stage.show();
        } catch (IOException e) {
            Utilities.showException(logger, "Cannot load dialog for adding sensor!", e, false, null);
        } catch (NullPointerException en){
            Utilities.showException(logger, "Cannot create new sensor", en, false, null);
        }
    }

    /**
     * Shows currently selected adapter's buffer file in default application used by operating system.
     */
    public void showFullLog(){
        if(hostServices != null && currentAdapterController!= null && currentAdapterController.getLog().getBufferFile()!= null){
            hostServices.showDocument(currentAdapterController.getLog().getBufferFile().getAbsolutePath());
        }
    }

    /**
     * Enables currently selected adapter.
     */
    public void enableCurrentAdapter(){
        if(currentAdapterController != null) currentAdapterController.enable();
    }
    /**
     * Disables currently selected adapter.
     */
    public void disableCurrentAdapter(){
        if(currentAdapterController != null) currentAdapterController.disable();
    }

    /**
     * Disables all created adapters
     */
    public void disableAllAdapters(){
        adapterControllersList.forEach(com.iha.emulator.control.AdapterController::disable);
    }

    /**
     * Shows {@link com.iha.emulator.ui.dialogs.adapter.DeleteAdaptersDialogPresenter} in new window
     */
    public void deleteAdapter(){
        DeleteAdaptersDialogPresenter deleteAdaptersDialogPresenter;
        try{
            //create window
            Stage stage = new Stage();
            //create presenter
            deleteAdaptersDialogPresenter = new DeleteAdaptersDialogPresenter(stage,this);
            stage.setTitle("Delete adapters");
            Scene scene = new Scene((Parent) deleteAdaptersDialogPresenter.loadView());
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            stage.setScene(scene);
            stage.setResizable(false);
            //show window
            stage.show();
        } catch (IOException e) {
            Utilities.showException(logger, "Cannot load dialog for deleting adapter!", e, false, null);
        }
    }

    /**
     * Disables given adapter and afterwards method deletes it and all it's sensors from emulator.
     * Also clears adapter, server details panels. If another adapters exist, first from list is selected as current.
     *
     * @param adapterController adapter to be deleted
     */
    public void deleteAdapter(AdapterController adapterController){
        logger.trace("Deleting adapter from emulator " + adapterController.toString());
        adapterController.disable();
        //if another adapter exists
        if(adapterControllersList.get().size() > 1){
            logger.trace("Removing logger");
        }else {
            //else no other adapters exist, so clear details panels
            logger.trace("Clearing adapter and server details");
            //clear adapter details
            adapterDetailsPresenter.clear();
            //clear server details
            serverDetailsPresenter.clear();
            //clear logs
            adapterController.getLog().clearContainers();
        }
        //remove adapter's button
        logger.trace("Removing adapter button");
        removeAdapterBtn(adapterController);
        logger.trace("Removing adapter controller");
        //delete adepter, logs, server
        adapterController.deleteAll();
        logger.trace("Removing adapter from list");
        //remove it from list
        adapterControllersList.get().remove(adapterController);
        //if another adapters exist
        if(adapterControllersList.get().size() != 0){
            //select first in list
            logger.trace("Setting next current adapter");
            setCurrentAdapter(adapterControllersList.get().get((0)));
        }else {
            setCurrentAdapter(null);
        }
        logger.trace("ADAPTERS LIST COUNT: " + adapterControllersList.size());
        //invoke garbage collector
        System.gc();
    }
    /**
     * Shows {@link com.iha.emulator.ui.dialogs.sensor.DeleteSensorsDialogPresenter} in new window
     */
    public void deleteSensor(){
        if(currentAdapterController == null) return;
        DeleteSensorsDialogPresenter deleteSensorsDialogPresenter;
        try{
            //create window
            Stage stage = new Stage();
            //create presenter
            deleteSensorsDialogPresenter = new DeleteSensorsDialogPresenter(stage,currentAdapterController);
            stage.setTitle("Delete sensors");
            Scene scene = new Scene((Parent) deleteSensorsDialogPresenter.loadView());
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            stage.setScene(scene);
            stage.setResizable(false);
            //show window
            stage.show();
        } catch (IOException e) {
            Utilities.showException(logger, "Cannot load dialog for deleting sensor!", e, false, null);
        }
    }

    /**
     * Enables internet connection for currently selected adapter
     */
    public void enableInternetConnection(){
        if(currentAdapterController != null) currentAdapterController.setInternetConnection(true);
    }
    /**
     * Enables internet connection for currently selected adapter
     */
    public void disableInternetConnection(){
        if(currentAdapterController != null) currentAdapterController.setInternetConnection(false);
    }

    /**
     * Selects given adapter as current and displays information about it to user interface. If given adapter is null,
     * disables control buttons in navigation.
     *
     * @param controller adapter to be selected as current
     */
    public void setCurrentAdapter(AdapterController controller){
        //do not choose adapter that is already chosen
        if(currentAdapterController!= null && controller != null){
            if(controller.equals(currentAdapterController)) return;
        }
        //if adapter is null, disable control buttons in navigation
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
        //notify user
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
        //assign adapter data model to GUI panel
        adapterDetailsPresenter.addModel(currentAdapterController.getAdapter());
        //assign server data model to GUI panel
        serverDetailsPresenter.addModel(currentAdapterController.getServerController().getModel());
        if(this.currentAdapterController.getServerReceiver() != null){
            serverDetailsPresenter.addSenderProperty(this.currentAdapterController.getServerReceiver().connProperty());
        }else{
            serverDetailsPresenter.addSenderProperty(null);
        }
        //bind adapter control buttons
        bindAdapterControlButtons();
        //bind internet control buttons
        bindInternetControlBtns();
        //bind log areas
        bindLogs();
        //bind number of registered sensors to variable
        bindRegisteredCount();
        //notify user
        setStatus("Adapter " + currentAdapterController.getAdapter().getId() + " / " + currentAdapterController.getAdapter().getName() + " set as current",false);
    }

    /**
     * Displays sensors' panels from #newController to senors' panels container. Sensors from #oldController are removed from
     * container.
     *
     * @param oldController adapter with sensors that are to be hidden
     * @param newController adapter with sensors that are to be shown
     */
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

    /**
     * Adds adapter button for given adapter to adapters' buttons container
     * @param controller adapter for which is button created
     */
    public void addAdapterBtn(AdapterController controller){
        view.addAdapterBtn(new AdapterButton(controller));
    }
    /**
     * Removes adapter button for given adapter from adapters' buttons container
     * @param controller adapter for which is button removed
     */
    public void removeAdapterBtn(AdapterController controller){
        view.removeAdapterBtn(controller);
    }

    /**
     * Creates XML document for given adapter and it's sensors, that is saved to file chosen by invoked file save dialog.
     * Uses separate {@link javafx.concurrent.Task}. If given adapter is null, currently selected adapter is saved.
     *
     * @param adapterController adapter to be saved
     */
    public void saveCurrentAdapter(AdapterController adapterController){
        //if given adapter null, save currently selected adapter
        if(adapterController == null) {
            if(this.currentAdapterController == null) return;
            adapterController = currentAdapterController;
        }
        final AdapterController tmpController = adapterController;
        logger.trace("Saving adapter: " + tmpController.toString());
        //initiate task to create XML document
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
        //set action on task success
        worker.setOnSucceeded(event -> {
            logger.trace("Trying to save to XML file");
            //show file save dialog
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
    /**
     * Creates XML document for all created adapters and their sensors, that is saved to file chosen by invoked file save dialog.
     * Uses separate {@link javafx.concurrent.Task}.
     */
    public void saveAllAdapters(){
        //if no adapters exist, return
        if(getAdapterControllers().size() < 1) return;
        logger.trace("Saving all adapters: ");
        //initiate task to create XML document
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
        //set action on task success
        worker.setOnSucceeded(event -> {
            logger.trace("Trying to save to XML file");
            //show file save dialog
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

    /**
     * Shows {@link com.iha.emulator.ui.dialogs.server.ChangeServerDetailsDialogPresenter} for currently selected adapter's
     * server model in new window. Also loads default server models form properties.
     */
    public void changeServerDetails(){
        if(this.currentAdapterController == null) return;
        logger.trace("Show change server details dialog.");
        ChangeServerDetailsDialogPresenter changeServerDetailsDialogPresenter;
        try{
            //load default server models from properties
            ObservableList<Server> tmp = null;
            try{
                tmp = Utilities.buildServersFromProperties(properties,null);

            }catch (IllegalArgumentException e){
                Platform.runLater(() -> Utilities.showException(logger, "Cannot load settings from properties file", e, false, null));
            }
            //create window
            Stage stage = new Stage();
            if(currentAdapterController == null) throw new NullPointerException("No current adapter");
            //create presenter
            changeServerDetailsDialogPresenter = new ChangeServerDetailsDialogPresenter(stage,tmp,currentAdapterController);
            stage.setTitle("Change server details");
            Scene scene = new Scene((Parent) changeServerDetailsDialogPresenter.loadView());
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            stage.setScene(scene);
            stage.initModality(Modality.APPLICATION_MODAL);
            stage.setResizable(false);
            //show window
            stage.show();
        } catch (IOException e) {
            Utilities.showException(logger, "Cannot load server details dialog!", e, false, null);
        } catch (NullPointerException en){
            Utilities.showException(logger, "Cannot load server", en, false, null);
        }
    }
    /**
     * Shows {@link com.iha.emulator.ui.dialogs.adapter.ChangeAdapterDetailsDialogPresenter} for currently selected adapter
     * in new window.
     */
    public void changeAdapterDetails(){
        if(this.currentAdapterController == null) return;
        logger.trace("Show change adapter details dialog.");
        ChangeAdapterDetailsDialogPresenter changeAdapterDetailsDialogPresenter;
        try{
            //create window
            Stage stage = new Stage();
            if(currentAdapterController == null) throw new NullPointerException("No current adapter");
            //create presenter
            changeAdapterDetailsDialogPresenter = new ChangeAdapterDetailsDialogPresenter(stage,currentAdapterController);
            stage.setTitle("Change adapter details");
            //create and initiate scene
            Scene scene = new Scene((Parent) changeAdapterDetailsDialogPresenter.loadView());
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            stage.setScene(scene);
            stage.initModality(Modality.APPLICATION_MODAL);
            stage.setResizable(false);
            //show window
            stage.show();
        } catch (IOException e) {
            Utilities.showException(logger, "Cannot load adapter details dialog!", e, false, null);
        } catch (NullPointerException en){
            Utilities.showException(logger, "Cannot load adapter", en, false, null);
        }

    }

    /**
     * Show dialog for loading XML file with saved adapters and add adapters from this XML.
     */
    public void open(){
        logger.trace("Trying to load from XML file");
        try {
            String content = Utilities.loadDialogForXML(window, SAVES_DEFAULT_DIR);
            if(content == null) return;
            parseAndLoadXML(content);
        } catch (DocumentException e) {
            Utilities.showException(logger, "Cannot parse loaded file", e, false, null);
        }
    }

    /**
     * Parses given XML document as string and creates adapter and their sensors.
     *
     * @param content XML document content containing adapters configuration
     * @throws DocumentException cannot parse read XML document
     */
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
                Double adapterFirmware = Double.valueOf(adapterElement.attributeValue("firmware"));
                if(adapterId == null || adapterName == null || adapterProtocolVersion == null ||adapterFirmware == null)
                    throw new NullPointerException("Adapter info missing");

                tmpAdapterController = new AdapterController();
                Protocol.Version protocolVersion = ProtocolFactory.getVersion(adapterProtocolVersion);
                if(protocolVersion == null)
                    throw new NullPointerException("Unknown protocol version -> " + adapterProtocolVersion);
                //CREATE ADAPTER
                tmpAdapterController.createAdapter(adapterName,false,adapterId,false, protocolVersion ,adapterFirmware);
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
                        //TODO fix forcefull history storage disabling
                        Boolean valueStoreHistory = false;
                        //Boolean valueStoreHistory = Boolean.valueOf(valueElement.attributeValue("store_history"));
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
                Utilities.showException(logger, "Adapter id already exist", e, false, null);
            }
            if(tmpAdapterController != null && tmpAdapterController.getAdapter() != null){
                //CREATE LOG
                tmpAdapterController.createLog(getView().getLogTabPane());
                //CREATE SCHEDULER
                tmpAdapterController.createScheduler(Scheduler.Type.DETAILED);
                tmpAdapterController.setTrackServerResponse(true);
                tmpAdapterController.setDumpServerResponse(true);
                tmpAdapterController.bindSchedulerProcess(tmpAdapterController.getAdapter(),tmpAdapterController.getScheduler());
                tmpAdapterController.bindRegisterMessage();
                //CREATE ADAPTER BUTTON
                addAdapterBtn(tmpAdapterController);
                //ADD ADAPTER TO OTHERS
                getAdapterControllersList().add(tmpAdapterController);
                try{
                    tmpAdapterController.getLog().setBuffered(true,"adapter_emu_" + String.valueOf(tmpAdapterController.getAdapter().getId())+"_",AddAdapterDialogPresenter.DEFAULT_LOG_PATH);
                    tmpAdapterController.getLog().writeAdapterLogHeaderToBuffer();
                }catch (IOException e){
                    throw new DocumentException("Cannot buffer adapter " + adapterElement.attribute("id").getValue() + " . Failed to create .tmp file!",e);
                }
                //create server receiver
                tmpAdapterController.createServerReceiver();
                tmpAdapterController.getServerReceiver().start();
                tmpAdapterController.setSaved(true);
            }
        }
        if(getAdapterControllers().size() > 0){
            setCurrentAdapter(getAdapterControllers().get(0));
        }
    }

    /**
     * Initializes window. Fills components with data, defines application onClose actions and initiates details panels.
     * Also starts memory usage checking.
     *
     * @param properties properties loaded from configuration file
     */
    public void init(Properties properties){
        logger.info("Initialization");
        //application initialisation
        startMemoryCheck();
        //load properties from .properties file
        this.properties = properties;
        //point logger to textArea
        TextAreaAppender.setTextFlow(view.getApplicationLogTextArea());
        //init presenters and save their views to GUI
        initPresentersAndViews();
        //disable toolbar buttons
        view.setAdapterBtns(true,true);
        view.setInternetBtns(true, true);
        view.getSaveBtn().setDisable(true);
        view.getSaveAllBtn().setDisable(true);
        //bind adapter file control buttons (Save, SaveAll, Print),others are bounded to current adapter
        bindControlBtnsToAdaptersCount();
        //handle close event
        window.setOnCloseRequest(event -> {
            checkIfSaved();
            if(getUnsavedAdapters() == null){
                quit();
            }else {
                event.consume();
            }
        });
        Runtime.getRuntime().addShutdownHook(new Thread(){
            public void run(){
                dumpLogsToFiles();
                logger.info("Shutting down");
            }
        });
    }

    /**
     * Binds adapter control buttons in navigation to currently selected adapter.
     */
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

    /**
     * Binds internet connection control buttons to currently selected adapter
     */
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

    /**
     * Binds application log areas to currently selected adapter
     */
    private void bindLogs(){
        if(currentAdapterController!= null && currentAdapterController.getLog() != null){
            AdapterLogger log = currentAdapterController.getLog();
            log.addAdapterLogTo(view.getAdapterLogContainer());
            log.addSentLogTo(view.getToBeSentLogContainer());
            log.addErrorLogTo(view.getErrorLogContainer());
        }
    }
    /**
     * Shows information dialog with given message.
     * @param title dialog title
     * @param headerMessage dialog header message
     * @param message information message
     */
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

    /**
     * Binds navigation buttons disable state to number of created adapters. If no adapter exists, relevant buttons are
     * disabled.
     */
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

    /**
     * Binds number of registered sensors property of currently selected adapter to adapter details panel relevant
     * property.
     */
    private void bindRegisteredCount(){
        //bind adapters registered sensor count to size of sensors array
        adapterDetailsPresenter.bindRegisteredCount(currentAdapterController.getSensorControllersList().sizeProperty());
    }

    /**
     * Creates presenter for details panels and initiates their views.
     */
    private void initPresentersAndViews(){
        //load server details
        try {
            serverDetailsPresenter = new ServerDetailsPresenter();
            view.addServerDetailsView(serverDetailsPresenter.loadView());
        } catch (IOException e) {
            Utilities.showException(logger, "Cannot load Server Details", e, true, event -> quit());
        }
        try {
            adapterDetailsPresenter = new AdapterDetailsPresenter();
            view.addAdapterDetailsView(adapterDetailsPresenter.loadView());
        } catch (IOException e) {
            Utilities.showException(logger, "Cannot load Adapter Details", e, true, event -> quit());
        }
    }

    /**
     * Load view from FXML file{@link com.iha.emulator.ui.simulations.detailed.DetailedSimulationPresenter#FXML_PATH} and after that
     * bind the view with a presenter. Also assigns CSS file {@link com.iha.emulator.ui.simulations.detailed.DetailedSimulationPresenter#CSS_PATH}
     * and creates {@link javafx.scene.Scene}, which is returned.
     * @return scene created from loaded view
     *
     * @throws IOException cannot load FXML file
     */
    public Scene loadView() throws IOException{
        logger.trace("Loading DetailedSimulationView from: " + FXML_PATH);
        InputStream fxmlStream = null;
        try {
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
     * Shows {@link org.controlsfx.dialog.ProgressDialog} for given {@link javafx.concurrent.Task} with given title-
     * @param worker task for which is progress dialog created
     * @param title dialog title
     */
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
    /**
     * Shows given message in dialog. Also can start or stop loop indicator
     * @param status message to be shown
     * @param indicate <code>true</code> show indicator, <code>false</code> hide indicator
     */
    public void setStatus(String status,boolean indicate){
        view.setStatusLine(status);
        view.setStatusIndicator(indicate);
        logger.info(status);
    }

    /**
     * Starts memory usage checking.
     */
    public void startMemoryCheck(){
        if(view != null && memoryChecker != null){
            logger.trace("Starting memory check");
            //assign where to show memory usage
            memoryChecker.setGuiElements(view.getMemCheckProgressBar(),view.getMemCheckStatusLabel());
            //start checking
            memoryChecker.start();
        }
    }

    /**
     * Shows error dialog with given message and title.
     * @param message error message
     * @param title dialog title
     */
    public void showError(String message,String title) {
        Alert dlg = new Alert(Alert.AlertType.ERROR, message);
        dlg.initModality(Modality.WINDOW_MODAL);
        dlg.setTitle(title);
        dlg.show();
    }

    /**
     * Gets list of adapters
     * @return list of adapters
     */
    public ObservableList<AdapterController> getAdapterControllersList() {
        return adapterControllersList;
    }
    /**
     * Gets list of adapters
     * @return list of adapters
     */
    public ObservableList<AdapterController> getAdapterControllers() {
        return adapterControllersList.get();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void bind() {
        view.setPresenter(this);
    }

    /**
     * Gets list of adapters, that have been modified but not saved to XML file
     * @return list of unsaved adapters
     */
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

    /**
     * Shows {@link javafx.scene.control.ChoiceDialog} with {@link com.iha.emulator.utilities.Utilities.SaveAdaptersOption}
     * actions, that should be made on list of unsaved adapters.
     */
    private void checkIfSaved(){
        logger.debug("Checking unsaved adapters");
        if(getAdapterControllers().size() > 0){
            ObservableList<AdapterController> unsavedAdapters = getUnsavedAdapters();
            if(unsavedAdapters != null){
                ChoiceDialog<Utilities.SaveAdaptersOption> dlg = Utilities.saveAdaptersOnQuitDialog();
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

    /**
     * Closes buffer files for all adapters in adapters list
     */
    private void dumpLogsToFiles(){
        if(getAdapterControllers().size() < 1) return;
        for(AdapterController adapterController : getAdapterControllers()){
            adapterController.getLog().closeBuffer();
        }
    }
    /**
     * {@inheritDoc}
     */
    public Display getView(){
        return this.view;
    }

    /**
     * Stops memory usage checking and closes all adapters' buffer files. Afterwards closes application.
     */
    public void quit(){
        memoryChecker.stop();
        //removeTempFiles();
        dumpLogsToFiles();
        Platform.exit();
    }

    /**
     * Gets host services
     * @return host services
     */
    public HostServices getHostServices() {
        return hostServices;
    }

    /**
     * Sets host services
     * @param hostServices host services
     */
    public void setHostServices(HostServices hostServices) {
        this.hostServices = hostServices;
    }

    //endregion

}
