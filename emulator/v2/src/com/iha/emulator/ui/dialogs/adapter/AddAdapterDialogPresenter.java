package com.iha.emulator.ui.dialogs.adapter;

import com.iha.emulator.models.Server;
import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.panels.PanelPresenter;
import com.iha.emulator.utilities.Utilities;
import javafx.beans.binding.BooleanBinding;
import javafx.beans.property.BooleanProperty;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.VBox;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.stage.StageStyle;
import javafx.util.Callback;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.controlsfx.dialog.ExceptionDialog;
import org.controlsfx.validation.ValidationMessage;
import org.controlsfx.validation.ValidationResult;
import org.controlsfx.validation.ValidationSupport;
import org.controlsfx.validation.Validator;
import org.controlsfx.validation.decoration.StyleClassValidationDecoration;
import org.controlsfx.validation.decoration.ValidationDecoration;

import java.io.IOException;
import java.io.InputStream;

/**
 * Created by Shu on 5.12.2014.
 */
public class AddAdapterDialogPresenter implements Presenter,PanelPresenter{

    private static final Logger logger = LogManager.getLogger(AddAdapterDialogPresenter.class);
    private static final String FXML_PATH = "AddAdapterDialog.fxml";
    private static final String CSS_PATH = "/com/iha/emulator/resources/css/theme-light.css";
    private ValidationSupport validationSupport = new ValidationSupport();
    private ValidationDecoration iconDecorator = new StyleClassValidationDecoration("validationError","validationWarn");

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
    }

    public AddAdapterDialogPresenter(Stage stage,ObservableList<Server> servers) {
        this.disableNext = new SimpleBooleanProperty(false);
        this.disablePrevious = new SimpleBooleanProperty(true);
        this.disableFinish = new SimpleBooleanProperty(true);
        this.serverInfoSet = new SimpleBooleanProperty(false);
        this.adapterInfoSet = new SimpleBooleanProperty(false);
        this.modifyServer = new SimpleBooleanProperty(false);
        validationSupport.setValidationDecorator(iconDecorator);
        this.window = stage;
        this.servers = servers;
    }

    @SuppressWarnings("unchecked")
    public void initialize(){
        //check if we have any servers
        if(this.servers == null){
            this.servers = FXCollections.observableArrayList();
        }else{
            //add default server
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
        //add change listener, if new item is selected in combo box, other fields are changed
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
        validationSupport.validationResultProperty().addListener((observable, oldValue, newValue) -> {
            if(validationSupport.isInvalid()){
                setServerInfoSet(false);
            }else{
                setServerInfoSet(true);
            }
        });
    }



    public void next(){
        if(getServerInfoSet() && selectedServer != null){
            logger.trace("Showing next panel");
            enableAdapterPanel();
            setDisablePrevious(false);
            setDisableNext(true);
            setDisableFinish(false);
        }else{
            logger.trace("Server info not filled. Cannot \"Next\" dialog");
            String message = null;
            for(ValidationMessage msg : validationSupport.getValidationResult().getErrors()){
                if(message == null){
                    message = msg.getText();
                }else {
                    message = message + "\n"  + msg.getText();
                }
            }
            showWarning("Server information","Please fill all necessary information",message);
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
        logger.trace("Finishing");
    }

    public void checkId(){

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
                showException("Cannot load dialog for showing adapters in database!",e);
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

    public void showException(String header,Exception e){
        logger.error(e.getMessage(),e);
        //create exception dialog
        ExceptionDialog dlg = new ExceptionDialog(e);
        //define default header
        if (header == null) header = "Ooops. Something went wrong!.";
        dlg.getDialogPane().setHeaderText(header);
        dlg.initStyle(StageStyle.DECORATED);
        //show exception dialog
        dlg.show();
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
        //server field validator
        validationSupport.registerValidator(view.getServerNameTxtField(),false, Validator.createEmptyValidator("Name is required"));
        validationSupport.registerValidator(view.getServerIpTxtField(),false, Validator.createEmptyValidator("Ip is required"));
        validationSupport.registerValidator(view.getServerIpTxtField(),false,(Control c, String newValue) ->
                ValidationResult.fromErrorIf( view.getServerIpTxtField(), "Ip must be in IP4 address format (xxx.xxx.xxx.xxx)", !Utilities.isIp(newValue)));
        validationSupport.registerValidator(view.getServerPortTxtField(),false, Validator.createEmptyValidator("Port is required"));
        validationSupport.registerValidator(view.getServerPortTxtField(),false,(Control c, String newValue) ->
                ValidationResult.fromErrorIf( view.getServerPortTxtField(), "Port must be an integer number with maximum of 5 digits", !Utilities.isIntegerNumber(newValue)));
        validationSupport.registerValidator(view.getServerDbNameTxtField(),false, Validator.createEmptyValidator("Database name is required"));
        //set placeholders
        view.getServerNameTxtField().setPromptText("Example: devel");
        view.getServerIpTxtField().setPromptText("Example: 10.1.0.1");
        view.getServerPortTxtField().setPromptText("Example: 7080");
        view.getServerDbNameTxtField().setPromptText("Example: home4");
        //disable checkbox
        view.getServerModifyCheckBox().setDisable(true);

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
