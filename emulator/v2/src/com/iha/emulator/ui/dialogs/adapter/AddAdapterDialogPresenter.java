package com.iha.emulator.ui.dialogs.adapter;

import com.iha.emulator.communication.eserver.EmulatorServerClient;
import com.iha.emulator.communication.eserver.model.AdapterInfo;
import com.iha.emulator.communication.eserver.task.ServerTask;
import com.iha.emulator.communication.eserver.task.TaskParser;
import com.iha.emulator.communication.eserver.task.implemented.CheckIdTask;
import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.control.AdapterController;
import com.iha.emulator.control.scheduler.Scheduler;
import com.iha.emulator.models.Server;
import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.panels.PanelPresenter;
import com.iha.emulator.ui.simulations.detailed.DetailedSimulationPresenter;
import com.iha.emulator.utilities.Utilities;
import javafx.application.Platform;
import javafx.beans.binding.BooleanBinding;
import javafx.beans.property.BooleanProperty;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.concurrent.Task;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.VBox;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.util.Callback;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.controlsfx.validation.ValidationMessage;
import org.controlsfx.validation.ValidationResult;
import org.controlsfx.validation.ValidationSupport;
import org.controlsfx.validation.Validator;
import org.controlsfx.validation.decoration.StyleClassValidationDecoration;
import org.controlsfx.validation.decoration.ValidationDecoration;
import org.dom4j.DocumentException;

import java.io.IOException;
import java.io.InputStream;

/**
 * Class providing logic to user interactions for "Add adapter dialog". Part Presenter of MVP design pattern.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class AddAdapterDialogPresenter implements Presenter,PanelPresenter{
    /** default firmware version */
    private String DEFAULT_FIRMWARE = "0";
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(AddAdapterDialogPresenter.class);
    /** path to FXML file */
    private static final String FXML_PATH = "AddAdapterDialog.fxml";
    /** path to CSS file */
    private static final String CSS_PATH = "/com/iha/emulator/resources/css/theme-light.css";
    /** path to default directory for adapters' logs */
    public static final String DEFAULT_LOG_PATH = "logs/detailed";
    /** server information validator */
    private ValidationSupport serverValidationSupport = new ValidationSupport();
    /** adapter information validator */
    private ValidationSupport adapterValidationSupport = new ValidationSupport();
    /** view */
    private Display view;
    /** window */
    private Stage window;
    /** dialog navigation "Next" disable property */
    private BooleanProperty disableNext;
    /** dialog navigation "Previous" disable property */
    private BooleanProperty disablePrevious;
    /** dialog navigation "Finish" disable property */
    private BooleanProperty disableFinish;
    /** all needed server information set property */
    private BooleanProperty serverInfoSet;
    /** all needed adapter information set property */
    private BooleanProperty adapterInfoSet;
    /** server information modification property */
    private BooleanProperty modifyServer;
    /** list of possible server */
    private ObservableList<Server> servers;
    /** selected server */
    private Server selectedServer;
    /** selected protocol version */
    private Protocol.Version selectedVersion;
    /** parent presenter */
    private DetailedSimulationPresenter parent;

    /**
     * Interface implemented by "Add adapter dialog" view.
     */
    public interface Display {
        public Node getView();
        public void setPresenter(AddAdapterDialogPresenter presenter);
        public Button getPreviousBtn();
        public Button getNextBtn();
        public Button getFinishBtn();
        public VBox getServerContainer();
        public VBox getAdapterContainer();
        public TextField getServerNameTxtField();
        public TextField getServerIpTxtField();
        public TextField getServerPortTxtField();
        public TextField getServerDbNameTxtField();
        public ComboBox getServerComboBox();
        public CheckBox getServerModifyCheckBox();
        public MenuItem getCheckIdMenuItem();
        public MenuItem getShowAdaptersMenuItem();
        public TextField getAdapterIdLbl();
        public TextField getAdapterNameLbl();
        public TextField getAdapterFirmwareLbl();
        public ComboBox getAdapterProtocolComboBox();
    }

    /**
     * Creates new "Add adapter dialog" presenter. Sets default servers and default adapter firmware version
     * @param stage parent window
     * @param servers list of default server
     * @param parent parent presenter
     * @param defaultFirmware default adapter firmware version
     */
    public AddAdapterDialogPresenter(Stage stage,ObservableList<Server> servers,DetailedSimulationPresenter parent,String defaultFirmware) {
        //initializes properties
        this.disableNext = new SimpleBooleanProperty(false);
        this.disablePrevious = new SimpleBooleanProperty(true);
        this.disableFinish = new SimpleBooleanProperty(true);
        this.serverInfoSet = new SimpleBooleanProperty(false);
        this.adapterInfoSet = new SimpleBooleanProperty(false);
        this.modifyServer = new SimpleBooleanProperty(false);
        //initializes validation
        ValidationDecoration iconDecorator = new StyleClassValidationDecoration("validationError", "validationWarn");
        serverValidationSupport.setValidationDecorator(iconDecorator);
        this.window = stage;
        this.servers = servers;
        this.parent = parent;
        this.DEFAULT_FIRMWARE = defaultFirmware;
    }

    /**
     * Initializes dialog. Fills components with data and sets validation options.
     */
    @SuppressWarnings("unchecked")
    public void initialize(){
        //----------------SERVER-------------------
        //check if we have any servers
        if(this.servers == null){
            this.servers = FXCollections.observableArrayList();
        }else{
            //save default server
            this.servers.add(new Server(false,"New server...","",0,""));
        }
        //populate combo box
        view.getServerComboBox().setItems(servers);
        view.getServerComboBox().setCellFactory(new Callback<ListView<Server>, ListCell<Server>>() {
            @Override
            public ListCell<Server> call(ListView<Server> param) {
                return new ListCell<Server>(){
                    @Override
                    protected void updateItem(Server s,boolean bln){
                        super.updateItem(s,bln);
                        if( s != null ){
                            setText(s.getName());
                        }else{
                            setText(null);
                        }
                    }
                };
            }
        });
        //save change listener, if new item is selected in combo box, other fields are changed
        view.getServerComboBox().valueProperty().addListener((observable, oldValue, newValue) -> {
            if(oldValue == null){
                view.getServerModifyCheckBox().setDisable(false);
            }
            Server tmpServer = (Server) newValue;
            setServerTextFields(tmpServer.getName(), tmpServer.getIp(), String.valueOf(tmpServer.getPort()), tmpServer.getDatabaseName());
            selectedServer = tmpServer;
            view.getServerModifyCheckBox().requestFocus();
        });
        //bind field validation support to variable holding completion of server info
        serverValidationSupport.validationResultProperty().addListener((observable, oldValue, newValue) -> {
                setServerInfoSet(!serverValidationSupport.isInvalid());
        });
        //----------------ADAPTER-------------------
        view.getAdapterFirmwareLbl().setText(DEFAULT_FIRMWARE);
        view.getAdapterProtocolComboBox().setItems(Utilities.getProtocolVersions());
        view.getAdapterProtocolComboBox().setCellFactory(new Callback<ListView<Protocol.Version>, ListCell<Protocol.Version>>() {
            @Override
            public ListCell call(ListView<Protocol.Version> param) {
                return new ListCell<Protocol.Version>(){
                    @Override
                    protected void updateItem(Protocol.Version s,boolean bln){
                        super.updateItem(s,bln);
                        if( s != null ){
                            setText(String.valueOf(s.getVersion()));
                        }else{
                            setText(null);
                        }
                    }
                };
            }
        });
        //save change listener, if new item is selected in combo box, other fields are changed
        view.getAdapterProtocolComboBox().valueProperty().addListener((observable, oldValue, newValue) -> {
            selectedVersion = (Protocol.Version) newValue;
        });
        //bind field validation support to variable holding completion of protocol version
        adapterValidationSupport.validationResultProperty().addListener((observable, oldValue, newValue) -> {
                setAdapterInfoSet(!adapterValidationSupport.isInvalid());
        });
    }

    /**
     * Next button clicked. If all needed information about server is set, show adapter's information dialog
     */
    public void next(){
        //if server was selected and all information about server is set
        if(getServerInfoSet() && selectedServer != null){
            logger.trace("Showing next panel");
            updateServerInfo();
            //show adapter's information dialog
            enableAdapterPanel();
            //disable or enable appropriate navigation buttons
            setDisablePrevious(false);
            setDisableNext(true);
            setDisableFinish(false);
        }else{
            //not all needed info is set, notify user
            logger.trace("Server info not filled. Cannot \"Next\" dialog");
            String message = null;
            //gather validation messages
            for(ValidationMessage msg : serverValidationSupport.getValidationResult().getErrors()){
                if(message == null){
                    message = msg.getText();
                }else {
                    message = message + "\n"  + msg.getText();
                }
            }
            //notify user
            showWarning("Server information","Please fill all necessary information",message);
        }
    }

    /**
     * Update selected server information from text fields.
     */
    private void updateServerInfo(){
        if(selectedServer != null){
            selectedServer.setIp(view.getServerIpTxtField().getText());
            selectedServer.setPort(Integer.valueOf(view.getServerPortTxtField().getText()));
            selectedServer.setDatabaseName(view.getServerDbNameTxtField().getText());
            selectedServer.setName(view.getServerNameTxtField().getText());
        }
    }

    /**
     * Hide adapter's information dialog and show server information dialog.
     */
    public void previous(){
        if(getServerInfoSet()){
            logger.trace("Showing previous panel");
            enableServerPanel();
            setDisablePrevious(true);
            setDisableNext(false);
            setDisableFinish(true);
        }else {
            logger.trace("Fill adapter info");
        }
    }

    /**
     * If all needed server and adapter information is set, add adapter to parent's adapters list and close dialog. Otherwise
     * notify user about unfilled information.
     */
    public void finish(){
        if(getAdapterInfoSet() && selectedVersion != null){
            logger.trace("Finishing");
            //all info set, add adapter
            if(addAdapter())
                window.hide();
        }else{
            logger.trace("Adapter info not filled. Cannot \"Finish\" dialog");
            String message = null;
            for(ValidationMessage msg : adapterValidationSupport.getValidationResult().getErrors()){
                if(message == null){
                    message = msg.getText();
                }else {
                    message = message + "\n"  + msg.getText();
                }
            }
            showWarning("Adapter information","Please fill all necessary information",message);
        }
    }

    /**
     * Creates new {@link com.iha.emulator.control.AdapterController} with {@link com.iha.emulator.models.Adapter} model
     * and adds it to parent's adapters list. Also adds adapter button to list of adapters in user interface.
     * @return <code>true</code> adapter added, <code>false</code> error occurred while adding adapter
     */
    private boolean addAdapter(){
        //check if list of adapter controllers exist
        AdapterController newAdapterController = null;
        try {
            if(Utilities.isAdapterIdTaken(parent.getAdapterControllers(),Integer.valueOf(view.getAdapterIdLbl().getText()))){
                showWarning("Warning","Adapter ID\"" + view.getAdapterIdLbl().getText() + "\" id taken.","Please choose another ID");
                return false;
            }
            //create new adapter controller
            newAdapterController = new AdapterController();
            //save it to list
            parent.getAdapterControllersList().add(newAdapterController);
            //create new adapter
            newAdapterController.createAdapter(
                    view.getAdapterNameLbl().getText(),
                    false,
                    Integer.valueOf(view.getAdapterIdLbl().getText()),
                    false,
                    selectedVersion,
                    Double.valueOf(view.getAdapterFirmwareLbl().getText()));
            //create new server
            newAdapterController.createServer(selectedServer);
            //create logger
            newAdapterController.createLog(parent.getView().getLogTabPane());
            //config log
            //newAdapterController.getLog().setType(AdapterLogger.toType(parent.getProperty("defaultLogMessageType")));
            //create scheduler
            newAdapterController.createScheduler(Scheduler.Type.DETAILED);
            //set response tracking
            newAdapterController.setTrackServerResponse(true);
            newAdapterController.setDumpServerResponse(true);
            //bind scheduler processing to adapter's status indicator
            newAdapterController.bindSchedulerProcess(newAdapterController.getAdapter(), newAdapterController.getScheduler());
            //bind register message
            newAdapterController.bindRegisterMessage();
            //save new adapter button
            parent.addAdapterBtn(newAdapterController);
            try {
                newAdapterController.getLog().setBuffered(true,"adapter_emu_" + String.valueOf(newAdapterController.getAdapter().getId())+"_",DEFAULT_LOG_PATH);
                newAdapterController.getLog().writeAdapterLogHeaderToBuffer();
            } catch (IOException e) {
                Utilities.showException(
                        logger,
                        "Cannot create buffer file for new adapter log.",
                        e,
                        true,
                        event -> parent.quit());
                return false;
            }
            //create server receiver
            newAdapterController.createServerReceiver();
            newAdapterController.getServerReceiver().start();
            // set new adapter as current
            parent.setCurrentAdapter(newAdapterController);
            return true;
        } catch (IllegalArgumentException e){
            parent.getAdapterControllersList().remove(newAdapterController);
            Utilities.showException(
                    logger,
                    "Cannot create adapter. Error in properties file. Please review file an start application again.",
                    e,
                    true,
                    event -> parent.quit());
            return false;
        }
    }

    /**
     * Check if filled adapter ID exists in database. Creates separate {@link javafx.concurrent.Task}.
     */
    public void checkId(){
        Task<Object> worker = new Task<Object>() {
            @Override
            protected Object call() throws Exception {
                //create emulator server client
                EmulatorServerClient server = new EmulatorServerClient(selectedServer.getIp());
                try{
                    //connect to emulator server
                    server.connect();
                }catch (IOException e){
                    Platform.runLater(() -> Utilities.showException(logger, "Cannot connect to server", e, false, null));
                }
                try{
                    //composite message for server
                    ServerTask task = new CheckIdTask(selectedServer.getDatabaseName(),view.getAdapterIdLbl().getText());
                    //send message and wait for response
                    String messageFromServer = server.sendMessage(task.buildMessage());
                    //determine result state (OK/ERROR)
                    TaskParser.parseTaskResult(messageFromServer);
                    //if ok, parse response
                    task.parseResponse(messageFromServer);
                    //show response in table
                    Platform.runLater(()->showAdapterFromServer(((CheckIdTask)task).getResult()));
                }catch (IOException e){
                    Platform.runLater(()-> Utilities.showException(logger,"Cannot read from socket",e,false,null));
                }catch (DocumentException de){
                    Platform.runLater(()-> Utilities.showException(logger,"Cannot parse server message",de,false,null));
                }catch (IllegalStateException ie){
                    Platform.runLater(()-> Utilities.showException(logger,"Error on server",ie,false,null));
                }
                return null;
            }
        };
        //create thread for background process
        Thread th = new Thread(worker);
        th.setDaemon(true);
        //run background process
        th.start();
    }

    /**
     * Shows adapter information gathered by communication with emulator server.
     * @param adapterInfo information about adapter from server
     */
    private void showAdapterFromServer(AdapterInfo adapterInfo){
        if(adapterInfo == null){
            //no info found, so adapter was not found in database
            //show dialog notifying user, that adapter doesn't exist yet
            Alert dlg = new Alert(Alert.AlertType.INFORMATION,"");
            dlg.initModality(Modality.WINDOW_MODAL);
            dlg.initOwner(window);
            dlg.setTitle("Check adapter ID");
            dlg.setHeaderText("ID was not found in database");
            dlg.getDialogPane().setContentText("Setting registered to: \"No\"");
            dlg.show();
            view.getAdapterNameLbl().setText("");
            view.getAdapterFirmwareLbl().setText(DEFAULT_FIRMWARE);
        } else {
            //show dialog notifying user about adapter information
            Alert dlg = new Alert(Alert.AlertType.INFORMATION,"");
            dlg.initModality(Modality.WINDOW_MODAL);
            dlg.initOwner(window);
            dlg.setTitle("Check adapter ID");
            dlg.setHeaderText("ID was found in database");
            dlg.getDialogPane().setContentText("Setting adapter information:\n" +
                    "ID: " + adapterInfo.getId() + "\n" +
                    "Name: " + adapterInfo.getName() + "\n" +
                    "Version: " + adapterInfo.getVersion());
            dlg.show();
            //set text field according to gathered adapter info
            view.getAdapterIdLbl().setText(adapterInfo.getId());
            if(adapterInfo.getName() == null || adapterInfo.getName().equals("null")){
                view.getAdapterNameLbl().setText("EA"+adapterInfo.getId());
            }else{
                view.getAdapterNameLbl().setText(adapterInfo.getName());
            }
            view.getAdapterFirmwareLbl().setText(adapterInfo.getVersion());
        }
    }

    /**
     * Shows new window with {@link com.iha.emulator.ui.dialogs.adapter.ShowAdaptersDialogPresenter} dialog.
     */
    public void showAdapters(){
        ShowAdaptersDialogPresenter showAdaptersDialogPresenter;
        try{
            //create window
            Stage stage = new Stage();
            //create presenter
            showAdaptersDialogPresenter = new ShowAdaptersDialogPresenter(stage,selectedServer);
            stage.setTitle("Adapters in database");
            //create and set scene, load view
            Scene scene = new Scene((Parent) showAdaptersDialogPresenter.loadView());
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            //set CSS style
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            stage.setScene(scene);
            //show window
            stage.show();
            //get adapters from DB
            showAdaptersDialogPresenter.refresh();
        } catch (IOException e) {
            Utilities.showException(logger, "Cannot load dialog for showing adapters in database!", e, false,null);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Node loadView() throws IOException {
        logger.trace("Loading AddAdapterDialogView from: " + FXML_PATH);
        InputStream fxmlStream = null;
        try
        {
            //load resource FXML
            fxmlStream = getClass().getResourceAsStream(FXML_PATH);
            FXMLLoader loader = new FXMLLoader();
            loader.load(fxmlStream);
            //remember view
            view = loader.getController();
            // bind view and presenter
            bind();
            //initialize combobox items
            initialize();
            logger.trace("OK");
            return view.getView();
        }
        finally
        {
            if (fxmlStream != null) fxmlStream.close();
        }
    }

    /**
     * Shows panel with server information
     */
    private void enableServerPanel(){
        view.getServerContainer().setVisible(true);
        view.getAdapterContainer().setVisible(false);
    }
    /**
     * Shows panel with adapter information
     */
    private void enableAdapterPanel(){
        view.getServerContainer().setVisible(false);
        view.getAdapterContainer().setVisible(true);
    }

    /**
     * Shows warning dialog with given title and message
     * @param title dialog title
     * @param headerMessage dialog header message
     * @param message warning message
     */
    private void showWarning(String title,String headerMessage,String message){
        Alert dlg = new Alert(Alert.AlertType.WARNING, "");
        dlg.initModality(Modality.WINDOW_MODAL);
        dlg.initOwner(this.window);
        dlg.setTitle(title);
        dlg.getDialogPane().setContentText(message);
        dlg.getDialogPane().setHeaderText(headerMessage);
        dlg.show();
    }

    /**
     * Fill server text fields with given values
     * @param name server name
     * @param ip server hostname
     * @param port server port
     * @param dbName database name
     */
    private void setServerTextFields(String name,String ip,String port,String dbName){
        view.getServerNameTxtField().setText(name);
        view.getServerIpTxtField().setText(ip);
        view.getServerPortTxtField().setText(port.equals("0") ? "" : port);
        view.getServerDbNameTxtField().setText(dbName);
    }

    /**
     * {@inheritDoc}
     *
     * Empty
     * @param model null
     */
    @Override
    public void addModel(Object model) {

    }

    /**
     * {@inheritDoc}
     *
     * Empty
     * @return null
     */
    @Override
    public Object getModel() {
        return null;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Node getView() {
        return view.getView();
    }

    /**
     * {@inheritDoc}
     *
     * Empty
     */
    @Override
    public void clear() {

    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void bind() {
        view.setPresenter(this);
        //control buttons
        view.getNextBtn().disableProperty().bind(this.disableNextProperty());
        view.getPreviousBtn().disableProperty().bind(this.disablePreviousProperty());
        view.getFinishBtn().disableProperty().bind(this.disableFinishProperty());
        //server
        BooleanBinding textFieldToModify = new BooleanBinding() {
            {
                bind(modifyServerProperty());
            }
            @Override
            protected boolean computeValue() {
                return !getModifyServer();
            }
        };
        view.getServerModifyCheckBox().selectedProperty().bindBidirectional(modifyServerProperty());
        view.getServerNameTxtField().disableProperty().bind(textFieldToModify);
        view.getServerIpTxtField().disableProperty().bind(textFieldToModify);
        view.getServerPortTxtField().disableProperty().bind(textFieldToModify);
        view.getServerDbNameTxtField().disableProperty().bind(textFieldToModify);
        //--server field validator
        serverValidationSupport.registerValidator(view.getServerNameTxtField(), false, Validator.createEmptyValidator("Name is required"));
        serverValidationSupport.registerValidator(view.getServerIpTxtField(), false, Validator.createEmptyValidator("Ip is required"));
        /*serverValidationSupport.registerValidator(view.getServerIpTxtField(), false, (Control c, String newValue) ->
                ValidationResult.fromErrorIf(view.getServerIpTxtField(), "Ip must be in IP4 address format (xxx.xxx.xxx.xxx)", !Utilities.isIp(newValue)));*/
        serverValidationSupport.registerValidator(view.getServerPortTxtField(), false, Validator.createEmptyValidator("Port is required"));
        serverValidationSupport.registerValidator(view.getServerPortTxtField(), false, (Control c, String newValue) ->
                ValidationResult.fromErrorIf(view.getServerPortTxtField(), "Port must be an integer number with maximum of 5 digits", !Utilities.isIntegerNumber(newValue, 1, 5)));
        serverValidationSupport.registerValidator(view.getServerDbNameTxtField(), false, Validator.createEmptyValidator("Database name is required"));
        //--set placeholders
        view.getServerNameTxtField().setPromptText("Example: devel");
        view.getServerIpTxtField().setPromptText("Example: ant-2.fit.vutbr.cz");
        view.getServerPortTxtField().setPromptText("Example: 7080");
        view.getServerDbNameTxtField().setPromptText("Example: home4");
            //disable checkbox
        view.getServerModifyCheckBox().setDisable(true);
        //adapter
        //--checkId button
        view.getCheckIdMenuItem().disableProperty().bind(new BooleanBinding() {
            {
                bind(view.getAdapterIdLbl().textProperty());
            }
            @Override
            protected boolean computeValue() {
                return view.getAdapterIdLbl().getText().equals("");
            }
        });
        //--adapter field validator
        adapterValidationSupport.registerValidator(view.getAdapterIdLbl(), false, Validator.createEmptyValidator("ID is required"));
        adapterValidationSupport.registerValidator(view.getAdapterIdLbl(), false, (Control c, String newValue) ->
                ValidationResult.fromErrorIf(view.getAdapterIdLbl(), "ID must be an integer number", !Utilities.isIntegerNumber(newValue, 1, 10)));
        adapterValidationSupport.registerValidator(view.getAdapterFirmwareLbl(), false, Validator.createEmptyValidator("Firmware is required"));
        adapterValidationSupport.registerValidator(view.getAdapterFirmwareLbl(), false, (Control c, String newValue) ->
                ValidationResult.fromErrorIf(view.getAdapterFirmwareLbl(), "Firmware must be a number", !Utilities.isNumeric(newValue)));
        adapterValidationSupport.registerValidator(view.getAdapterProtocolComboBox(), false, (Control c, Protocol.Version version) ->
                ValidationResult.fromErrorIf(view.getAdapterProtocolComboBox(), "Protocol version is required", (version == null)));
        //--adapter placeholders
        view.getAdapterIdLbl().setPromptText("Example: 51914");
        view.getAdapterNameLbl().setPromptText("Example: EA51914");
        view.getAdapterFirmwareLbl().setPromptText("Example: 0");
        //--adapter bind finish disable property to validator
    }

    /**
     * Gets if next button is disabled
     * @return <code>true</code> if next button is disabled, <code>false</code> otherwise
     */
    public boolean getDisableNext() {
        return disableNext.get();
    }

    /**
     * Next button is disabled property
     * @return next button is disabled property
     */
    public BooleanProperty disableNextProperty() {
        return disableNext;
    }

    /**
     * Sets if next button is disabled
     * @param disableNext <code>true</code> if next button is disabled, <code>false</code> otherwise
     */
    public void setDisableNext(boolean disableNext) {
        this.disableNext.set(disableNext);
    }
    /**
     * Gets if previous button is disabled
     * @return <code>true</code> if previous button is disabled, <code>false</code> otherwise
     */
    public boolean getDisablePrevious() {
        return disablePrevious.get();
    }
    /**
     * Previous button is disabled property
     * @return previous button is disabled property
     */
    public BooleanProperty disablePreviousProperty() {
        return disablePrevious;
    }
    /**
     * Sets if previous button is disabled
     * @param disablePrevious if previous button is disabled, <code>false</code> otherwise
     */
    public void setDisablePrevious(boolean disablePrevious) {
        this.disablePrevious.set(disablePrevious);
    }
    /**
     * Gets if previous button is disabled
     * @return <code>true</code> if previous button is disabled, <code>false</code> otherwise
     */
    public boolean getDisableFinish() {
        return disableFinish.get();
    }
    /**
     * Previous button is disabled property
     * @return previous button is disabled property
     */
    public BooleanProperty disableFinishProperty() {
        return disableFinish;
    }
    /**
     * Sets if previous button is disabled
     * @param disableFinish <code>true</code> if previous button is disabled, <code>false</code> otherwise
     */
    public void setDisableFinish(boolean disableFinish) {
        this.disableFinish.set(disableFinish);
    }

    /**
     * Gets if all needed server information is set
     * @return <code>true</code> all set, <code>false</code> otherwise
     */
    public boolean getServerInfoSet() {
        return serverInfoSet.get();
    }

    /**
     * All needed server information is set property
     * @return all needed server information is set property
     */
    public BooleanProperty serverInfoSetProperty() {
        return serverInfoSet;
    }

    /**
     * Sets if all needed server information is set
     * @param serverInfoSet <code>true</code> all set, <code>false</code> otherwise
     */
    public void setServerInfoSet(boolean serverInfoSet) {
        this.serverInfoSet.set(serverInfoSet);
    }
    /**
     * Gets if all needed adapter information is set
     * @return <code>true</code> all set, <code>false</code> otherwise
     */
    public boolean getAdapterInfoSet() {
        return adapterInfoSet.get();
    }
    /**
     * All needed adapter information is set property
     * @return all needed adapter information is set property
     */
    public BooleanProperty adapterInfoSetProperty() {
        return adapterInfoSet;
    }
    /**
     * Sets if all needed adapter information is set
     * @param adapterInfoSet <code>true</code> all set, <code>false</code> otherwise
     */
    public void setAdapterInfoSet(boolean adapterInfoSet) {
        this.adapterInfoSet.set(adapterInfoSet);
    }

    /**
     * Gets if server is modified
     * @return <code>true</code> if modified, <code>false</code> otherwise
     */
    public boolean getModifyServer() {
        return modifyServer.get();
    }

    /**
     * Server is modified property
     * @return server is modified property
     */
    public BooleanProperty modifyServerProperty() {
        return modifyServer;
    }

    /**
     * Sets if server is modified
     * @param modifyServer <code>true</code> if modified, <code>false</code> otherwise
     */
    public void setModifyServer(boolean modifyServer) {
        this.modifyServer.set(modifyServer);
    }
}
