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
 * Class providing logic to user interactions for "Change server details dialog". Part Presenter of MVP design pattern.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class ChangeServerDetailsDialogPresenter implements Presenter,PanelPresenter{
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(ChangeServerDetailsDialogPresenter.class);
    /** path to FXML file */
    private static final String FXML_PATH = "ChangeServerDetailsDialog.fxml";
    /** server information validator */
    private ValidationSupport serverValidationSupport = new ValidationSupport();
    /** view */
    private Display view;
    /** window */
    private Stage window;
    /** all needed server information set property */
    private BooleanProperty serverInfoSet;
    /** list of possible server */
    private ObservableList<Server> servers;
    /** selected server */
    private Server selectedServer;
    /** adapter controller */
    private AdapterController adapterController;
    /**
     * Interface implemented by "Change server details dialog" view.
     */
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

    /**
     * Creates "Change server details dialog" presenter for given servers. While modifying server, adapter is disabled.
     * @param stage parent window
     * @param servers list of servers, that can be chosen
     * @param adapterController adapter containing server model to be changed
     */
    public ChangeServerDetailsDialogPresenter(Stage stage, ObservableList<Server> servers, AdapterController adapterController) {
        this.serverInfoSet = new SimpleBooleanProperty(true);
        ValidationDecoration iconDecorator = new StyleClassValidationDecoration("validationError", "validationWarn");
        serverValidationSupport.setValidationDecorator(iconDecorator);
        this.window = stage;
        this.servers = servers;
        this.adapterController = adapterController;
        adapterController.disable();
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
    /**
     * Closes dialog
     */
    public void close(){
        this.window.hide();
    }
    /**
     * If all needed information about server is set, applies changes and closes dialog. Otherwise notifies user.
     */
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

    /**
     * Checks if any information was changed, if it was, applies changes to server's model.
     * @return <code>true</code> everything OK, <code>false</code> no server selected
     */
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
        if(!model.getIp().equals(view.getServerIpTxtField().getText())){
            model.setIp(view.getServerIpTxtField().getText());
            adapterController.setSaved(false);
        }
        if(model.getPort() != Integer.valueOf(view.getServerPortTxtField().getText())){
            model.setPort(Integer.valueOf(view.getServerPortTxtField().getText()));
            adapterController.setSaved(false);
        }
        return true;
    }

    /**
     * Saves text from text fields to selected server model.
     */
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

    /**
     * Sets selected server model, fills fields with model information
     * @param server server model to be selected
     */
    public void setSelectedServer(Server server){
        this.selectedServer = server;
        setServerTextFields(server.getName(), server.getIp(), String.valueOf(server.getPort()), server.getDatabaseName());
        updateServerInfo();
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
     * Fills fields with given information
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
        //server
        //--server field validator
        serverValidationSupport.registerValidator(view.getServerNameTxtField(), false, Validator.createEmptyValidator("Name is required"));
        serverValidationSupport.registerValidator(view.getServerIpTxtField(), false, Validator.createEmptyValidator("Hostname or IP is required"));
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

}
