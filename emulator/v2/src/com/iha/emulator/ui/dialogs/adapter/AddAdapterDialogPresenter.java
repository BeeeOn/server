package com.iha.emulator.ui.dialogs.adapter;

import com.iha.emulator.communication.eserver.EmulatorServerClient;
import com.iha.emulator.communication.eserver.model.AdapterInfo;
import com.iha.emulator.communication.eserver.task.ServerTask;
import com.iha.emulator.communication.eserver.task.TaskParser;
import com.iha.emulator.communication.eserver.task.implemented.CheckIdTask;
import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.control.AdapterController;
import com.iha.emulator.models.Server;
import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.panels.PanelPresenter;
import com.iha.emulator.ui.simulations.detailed.DetailedSimulationPresenter;
import com.iha.emulator.utilities.AdapterLogger;
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
 * Created by Shu on 5.12.2014.
 */
public class AddAdapterDialogPresenter implements Presenter,PanelPresenter{

    private String DEFAULT_FIRMWARE = "0";
    private static final Logger logger = LogManager.getLogger(AddAdapterDialogPresenter.class);
    private static final String FXML_PATH = "AddAdapterDialog.fxml";
    private static final String CSS_PATH = "/com/iha/emulator/resources/css/theme-light.css";
    private ValidationSupport serverValidationSupport = new ValidationSupport();
    private ValidationSupport adapterValidationSupport = new ValidationSupport();

    private Display view;
    private Stage window;

    private BooleanProperty disableNext;
    private BooleanProperty disablePrevious;
    private BooleanProperty disableFinish;
    private BooleanProperty serverInfoSet;
    private BooleanProperty adapterInfoSet;
    private BooleanProperty modifyServer;

    private ObservableList<Server> servers;
    private Server selectedServer;
    private Protocol.Version selectedVersion;

    private DetailedSimulationPresenter parent;

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
        public RadioButton getAdapterNoRegisteredRadBtn();
        public RadioButton getAdapterYesRegisteredRadBtn();
        public ComboBox getAdapterProtocolComboBox();
    }

    public AddAdapterDialogPresenter(Stage stage,ObservableList<Server> servers,DetailedSimulationPresenter parent,String defaultFirmware) {
        this.disableNext = new SimpleBooleanProperty(false);
        this.disablePrevious = new SimpleBooleanProperty(true);
        this.disableFinish = new SimpleBooleanProperty(true);
        this.serverInfoSet = new SimpleBooleanProperty(false);
        this.adapterInfoSet = new SimpleBooleanProperty(false);
        this.modifyServer = new SimpleBooleanProperty(false);
        ValidationDecoration iconDecorator = new StyleClassValidationDecoration("validationError", "validationWarn");
        serverValidationSupport.setValidationDecorator(iconDecorator);
        this.window = stage;
        this.servers = servers;
        this.parent = parent;
        this.DEFAULT_FIRMWARE = defaultFirmware;
    }

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
        view.getAdapterProtocolComboBox().setItems(getProtocolVersions());
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



    public void next(){
        if(getServerInfoSet() && selectedServer != null){
            logger.trace("Showing next panel");
            updateServerInfo();
            enableAdapterPanel();
            setDisablePrevious(false);
            setDisableNext(true);
            setDisableFinish(false);
        }else{
            logger.trace("Server info not filled. Cannot \"Next\" dialog");
            String message = null;
            for(ValidationMessage msg : serverValidationSupport.getValidationResult().getErrors()){
                if(message == null){
                    message = msg.getText();
                }else {
                    message = message + "\n"  + msg.getText();
                }
            }
            showWarning("Server information","Please fill all necessary information",message);
        }
    }

    private void updateServerInfo(){
        if(selectedServer != null){
            selectedServer.setIp(view.getServerIpTxtField().getText());
            selectedServer.setPort(Integer.valueOf(view.getServerPortTxtField().getText()));
            selectedServer.setDatabaseName(view.getServerDbNameTxtField().getText());
            selectedServer.setName(view.getServerNameTxtField().getText());
        }
    }

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

    public void finish(){
        if(getAdapterInfoSet() && selectedVersion != null){
            logger.trace("Finishing");
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

    private boolean addAdapter(){
        //check if list of adapter controllers exist
        AdapterController newAdapterController = null;
        try {
            //create new adapter controller
            newAdapterController = new AdapterController();
            //save it to list
            parent.getAdapterControllersList().add(newAdapterController);
            //create new adapter
            newAdapterController.createAdapter(
                    view.getAdapterNameLbl().getText(),
                    false,
                    Integer.valueOf(view.getAdapterIdLbl().getText()),
                    view.getAdapterYesRegisteredRadBtn().isSelected(),
                    selectedVersion,
                    Double.valueOf(view.getAdapterFirmwareLbl().getText()));
            //create new server
            newAdapterController.createServer(selectedServer);
            //create logger
            newAdapterController.createLog(parent.getView().getLogTabPane());
            //config log
            newAdapterController.getLog().setType(AdapterLogger.toType(parent.getProperty("defaultLogMessageType")));
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
            parent.addAdapterBtn(newAdapterController);
            // set new adapter as current
            parent.setCurrentAdapter(newAdapterController);
            try {
                newAdapterController.getLog().setBuffered(true,String.valueOf(newAdapterController.getAdapter().getId()));
            } catch (IOException e) {
                DetailedSimulationPresenter.showException(
                        logger,
                        "Cannot create buffer file for new adapter log.",
                        e,
                        true,
                        event -> parent.quit());
                return false;
            }
            return true;
        } catch (IllegalArgumentException e){
            parent.getAdapterControllersList().remove(newAdapterController);
            DetailedSimulationPresenter.showException(
                    logger,
                    "Cannot create adapter. Error in properties file. Please review file an start application again.",
                    e,
                    true,
                    event -> parent.quit());
            return false;
        }
    }

    public void checkId(){
        Task<Object> worker = new Task<Object>() {
            @Override
            protected Object call() throws Exception {
                EmulatorServerClient server = new EmulatorServerClient();
                try{
                    server.connect();
                }catch (IOException e){
                    Platform.runLater(() -> DetailedSimulationPresenter.showException(logger, "Cannot connect to server", e, false, null));
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
                    Platform.runLater(()-> DetailedSimulationPresenter.showException(logger,"Cannot read from socket",e,false,null));
                }catch (DocumentException de){
                    Platform.runLater(()-> DetailedSimulationPresenter.showException(logger,"Cannot parse server message",de,false,null));
                }catch (IllegalStateException ie){
                    Platform.runLater(()-> DetailedSimulationPresenter.showException(logger,"Error on server",ie,false,null));
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

    private void showAdapterFromServer(AdapterInfo adapterInfo){
        //TODO show dialog, that id was found and if user want to set data from server
        if(adapterInfo == null){
            Alert dlg = new Alert(Alert.AlertType.INFORMATION,"");
            dlg.initModality(Modality.WINDOW_MODAL);
            dlg.initOwner(window);
            dlg.setTitle("Check adapter ID");
            dlg.setHeaderText("ID was not found in database");
            dlg.getDialogPane().setContentText("Setting registered to: \"No\"");
            dlg.show();
            view.getAdapterNameLbl().setText("");
            view.getAdapterFirmwareLbl().setText(DEFAULT_FIRMWARE);
            view.getAdapterNoRegisteredRadBtn().setSelected(true);
        } else {
            Alert dlg = new Alert(Alert.AlertType.INFORMATION,"");
            dlg.initModality(Modality.WINDOW_MODAL);
            dlg.initOwner(window);
            dlg.setTitle("Check adapter ID");
            dlg.setHeaderText("ID was found in database");
            dlg.getDialogPane().setContentText("Setting adapter information:\n" +
                    "ID: " + adapterInfo.getId() + "\n" +
                    "Name: " + adapterInfo.getName() + "\n" +
                    "Version: " + adapterInfo.getVersion() + "\n" +
                    "Registered: Yes");
            dlg.show();
            view.getAdapterIdLbl().setText(adapterInfo.getId());
            if(adapterInfo.getName() == null || adapterInfo.getName().equals("null")){
                view.getAdapterNameLbl().setText("EA"+adapterInfo.getId());
            }else{
                view.getAdapterNameLbl().setText(adapterInfo.getName());
            }
            view.getAdapterFirmwareLbl().setText(adapterInfo.getVersion());
            view.getAdapterYesRegisteredRadBtn().setSelected(true);
        }
    }

    private ObservableList<Protocol.Version> getProtocolVersions(){
        return FXCollections.observableArrayList(Protocol.Version.values());
    }

    public void showAdapters(){
        ShowAdaptersDialogPresenter showAdaptersDialogPresenter;
        try{
            Stage stage = new Stage();
            showAdaptersDialogPresenter = new ShowAdaptersDialogPresenter(stage,selectedServer.getDatabaseName());
            stage.setTitle("Adapters in database");
            Scene scene = new Scene((Parent) showAdaptersDialogPresenter.loadView());
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            stage.setScene(scene);
            //stage.setResizable(false);
            stage.show();
            showAdaptersDialogPresenter.refresh();
            } catch (IOException e) {
                DetailedSimulationPresenter.showException(logger, "Cannot load dialog for showing adapters in database!", e, false,null);
            }
    }

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

    private void enableServerPanel(){
        view.getServerContainer().setVisible(true);
        view.getAdapterContainer().setVisible(false);
    }

    private void enableAdapterPanel(){
        view.getServerContainer().setVisible(false);
        view.getAdapterContainer().setVisible(true);
    }

    private void showWarning(String title,String headerMessage,String message){
        Alert dlg = new Alert(Alert.AlertType.WARNING, "");
        dlg.initModality(Modality.WINDOW_MODAL);
        dlg.initOwner(this.window);
        dlg.setTitle(title);
        dlg.getDialogPane().setContentText(message);
        dlg.getDialogPane().setHeaderText(headerMessage);
        dlg.show();
    }

    private void setServerTextFields(String name,String ip,String port,String dbName){
        view.getServerNameTxtField().setText(name);
        view.getServerIpTxtField().setText(ip);
        view.getServerPortTxtField().setText(port.equals("0") ? "" : port);
        view.getServerDbNameTxtField().setText(dbName);
    }

    @Override
    public void addModel(Object model) {

    }

    @Override
    public Object getModel() {
        return null;
    }

    @Override
    public Node getView() {
        return view.getView();
    }

    @Override
    public void clear() {

    }

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
        serverValidationSupport.registerValidator(view.getServerIpTxtField(), false, (Control c, String newValue) ->
                ValidationResult.fromErrorIf(view.getServerIpTxtField(), "Ip must be in IP4 address format (xxx.xxx.xxx.xxx)", !Utilities.isIp(newValue)));
        serverValidationSupport.registerValidator(view.getServerPortTxtField(), false, Validator.createEmptyValidator("Port is required"));
        serverValidationSupport.registerValidator(view.getServerPortTxtField(), false, (Control c, String newValue) ->
                ValidationResult.fromErrorIf(view.getServerPortTxtField(), "Port must be an integer number with maximum of 5 digits", !Utilities.isIntegerNumber(newValue, 1, 5)));
        serverValidationSupport.registerValidator(view.getServerDbNameTxtField(), false, Validator.createEmptyValidator("Database name is required"));
        //--set placeholders
        view.getServerNameTxtField().setPromptText("Example: devel");
        view.getServerIpTxtField().setPromptText("Example: 10.1.0.1");
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

    public boolean getDisableNext() {
        return disableNext.get();
    }

    public BooleanProperty disableNextProperty() {
        return disableNext;
    }

    public void setDisableNext(boolean disableNext) {
        this.disableNext.set(disableNext);
    }

    public boolean getDisablePrevious() {
        return disablePrevious.get();
    }

    public BooleanProperty disablePreviousProperty() {
        return disablePrevious;
    }

    public void setDisablePrevious(boolean disablePrevious) {
        this.disablePrevious.set(disablePrevious);
    }

    public boolean getDisableFinish() {
        return disableFinish.get();
    }

    public BooleanProperty disableFinishProperty() {
        return disableFinish;
    }

    public void setDisableFinish(boolean disableFinish) {
        this.disableFinish.set(disableFinish);
    }

    public boolean getServerInfoSet() {
        return serverInfoSet.get();
    }

    public BooleanProperty serverInfoSetProperty() {
        return serverInfoSet;
    }

    public void setServerInfoSet(boolean serverInfoSet) {
        this.serverInfoSet.set(serverInfoSet);
    }

    public boolean getAdapterInfoSet() {
        return adapterInfoSet.get();
    }

    public BooleanProperty adapterInfoSetProperty() {
        return adapterInfoSet;
    }

    public void setAdapterInfoSet(boolean adapterInfoSet) {
        this.adapterInfoSet.set(adapterInfoSet);
    }

    public boolean getModifyServer() {
        return modifyServer.get();
    }

    public BooleanProperty modifyServerProperty() {
        return modifyServer;
    }

    public void setModifyServer(boolean modifyServer) {
        this.modifyServer.set(modifyServer);
    }
}
