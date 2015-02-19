package com.iha.emulator.ui.dialogs.server;

import com.iha.emulator.control.AdapterController;
import com.iha.emulator.models.Server;
import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.panels.PanelPresenter;
import com.iha.emulator.utilities.Utilities;
import javafx.beans.property.BooleanProperty;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
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

import java.io.IOException;
import java.io.InputStream;

/**
 * Created by Shu on 5.12.2014.
 */
public class ChangeServerDetailsDialogPresenter implements Presenter,PanelPresenter{

    private static final Logger logger = LogManager.getLogger(ChangeServerDetailsDialogPresenter.class);
    private static final String FXML_PATH = "ChangeServerDetailsDialog.fxml";
    private static final String CSS_PATH = "/com/iha/emulator/resources/css/theme-light.css";
    private ValidationSupport serverValidationSupport = new ValidationSupport();
    private ValidationDecoration iconDecorator = new StyleClassValidationDecoration("validationError","validationWarn");

    private Display view;
    private Stage window;

    private BooleanProperty serverInfoSet;

    private ObservableList<Server> servers;
    private Server selectedServer;
    private AdapterController adapterController;

    public interface Display {
        public Node getView();
        public void setPresenter(ChangeServerDetailsDialogPresenter presenter);
        public Button getChangeBtn();
        public Button getCloseBtn();
        public VBox getServerContainer();
        public TextField getServerNameTxtField();
        public TextField getServerIpTxtField();
        public TextField getServerPortTxtField();
        public TextField getServerDbNameTxtField();
        public ComboBox getServerComboBox();
    }

    public ChangeServerDetailsDialogPresenter(Stage stage, ObservableList<Server> servers, AdapterController adapterController) {
        this.serverInfoSet = new SimpleBooleanProperty(false);
        serverValidationSupport.setValidationDecorator(iconDecorator);
        this.window = stage;
        this.servers = servers;
        this.adapterController = adapterController;
        adapterController.disable();
    }

    @SuppressWarnings("unchecked")
    public void initialize(){
        //----------------SERVER-------------------
        //check if we have any servers
        if(this.servers == null){
            this.servers = FXCollections.observableArrayList();
        }
        this.servers.add(0,new Server(false,"New server...","",0,""));
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
        //try to select server from current adapter
        logger.trace("Matching adapter's server with existing servers");
        boolean found = false;
        Server tmp = adapterController.getServerController().getModel();
        for(Server server : servers){
            if(server.equals(tmp)){
                logger.trace("MATCH");
                view.getServerComboBox().getSelectionModel().select(tmp);
                view.getServerNameTxtField().setText(tmp.getName());
                view.getServerIpTxtField().setText(tmp.getIp());
                view.getServerPortTxtField().setText(String.valueOf(tmp.getPort()));
                view.getServerDbNameTxtField().setText(tmp.getDatabaseName());
                setServerInfoSet(true);
                found = true;
            }else {
                logger.trace("No match");
            }
        }
        if(!found){
            servers.add(tmp);
            setSelectedServer(tmp);
            view.getServerComboBox().getSelectionModel().select(tmp);
        }
        //save change listener, if new item is selected in combo box, other fields are changed
        view.getServerComboBox().valueProperty().addListener((observable, oldValue, newValue) -> {
            Server tmpServer = (Server) newValue;
            setSelectedServer(tmpServer);
        });
        //bind field validation support to variable holding completion of server info
        serverValidationSupport.validationResultProperty().addListener((observable, oldValue, newValue) -> {
                setServerInfoSet(!serverValidationSupport.isInvalid());
        });
    }

    public void close(){
        this.window.hide();
    }

    public void change(){
        if(getServerInfoSet() && changeServer()){
            close();
        }else{
            logger.trace("Server info not filled. Cannot \"Finish\" dialog");
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

    private boolean changeServer(){
        logger.trace("Changing server");
        if(selectedServer == null){
            logger.trace("No selected server!!");
            return false;
        }
        Server model = adapterController.getServerController().getModel();
        if(!model.getName().equals(view.getServerNameTxtField().getText())){
            model.setName(view.getServerNameTxtField().getText());
            adapterController.setSaved(false);
        }
        if(!model.getDatabaseName().equals(view.getServerDbNameTxtField().getText())){
            model.setDatabaseName(view.getServerDbNameTxtField().getText());
            adapterController.setSaved(false);
        }
        if(!model.equals(view.getServerIpTxtField().getText())){
            model.setIp(view.getServerIpTxtField().getText());
            adapterController.setSaved(false);
        }
        if(model.getPort() != Integer.valueOf(view.getServerPortTxtField().getText())){
            model.setPort(Integer.valueOf(view.getServerPortTxtField().getText()));
            adapterController.setSaved(false);
        }
        return true;
    }

    private void updateServerInfo(){
        if(selectedServer != null){
            if(!view.getServerIpTxtField().getText().isEmpty())
                selectedServer.setIp(view.getServerIpTxtField().getText());
            if(!view.getServerPortTxtField().getText().isEmpty())
                selectedServer.setPort(Integer.valueOf(view.getServerPortTxtField().getText()));
            if(!view.getServerDbNameTxtField().getText().isEmpty())
                selectedServer.setDatabaseName(view.getServerDbNameTxtField().getText());
            if(!view.getServerNameTxtField().getText().isEmpty())
                selectedServer.setName(view.getServerNameTxtField().getText());
        }
    }

    public void setSelectedServer(Server server){
        this.selectedServer = server;
        setServerTextFields(server.getName(), server.getIp(), String.valueOf(server.getPort()), server.getDatabaseName());
        updateServerInfo();
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
        //server
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

}
