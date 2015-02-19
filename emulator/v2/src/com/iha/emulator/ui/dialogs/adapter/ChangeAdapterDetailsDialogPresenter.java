package com.iha.emulator.ui.dialogs.adapter;

import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.communication.protocol.ProtocolFactory;
import com.iha.emulator.control.AdapterController;
import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.panels.PanelPresenter;
import com.iha.emulator.ui.simulations.detailed.DetailedSimulationPresenter;
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
public class ChangeAdapterDetailsDialogPresenter implements Presenter,PanelPresenter{

    private String DEFAULT_FIRMWARE = "0";
    private static final Logger logger = LogManager.getLogger(ChangeAdapterDetailsDialogPresenter.class);
    private static final String FXML_PATH = "ChangeAdapterDetailsDialog.fxml";
    private static final String CSS_PATH = "/com/iha/emulator/resources/css/theme-light.css";
    private ValidationSupport adapterValidationSupport = new ValidationSupport();
    private ValidationDecoration iconDecorator = new StyleClassValidationDecoration("validationError","validationWarn");

    private Display view;
    private Stage window;

    private AdapterController adapterController;
    private BooleanProperty adapterInfoSet;
    private Protocol.Version selectedVersion;
    private DetailedSimulationPresenter parent;

    public interface Display {
        public Node getView();
        public void setPresenter(ChangeAdapterDetailsDialogPresenter presenter);
        public Button getChangeBtn();
        public Button getCloseBtn();
        public VBox getAdapterContainer();
        public TextField getAdapterNameLbl();
        public TextField getAdapterFirmwareLbl();
        public RadioButton getAdapterNoRegisteredRadBtn();
        public RadioButton getAdapterYesRegisteredRadBtn();
        public ComboBox getAdapterProtocolComboBox();
    }

    public ChangeAdapterDetailsDialogPresenter(Stage stage, AdapterController adapterController) {
        this.adapterInfoSet = new SimpleBooleanProperty(false);
        this.window = stage;
        this.adapterController = adapterController;
    }

    @SuppressWarnings("unchecked")
    public void initialize(){
        //----------------ADAPTER-------------------
        //firmware
        view.getAdapterFirmwareLbl().setText(String.valueOf(adapterController.getAdapter().getFirmware()));
        //protocol
        view.getAdapterProtocolComboBox().setItems(getProtocolVersions());
        view.getAdapterProtocolComboBox().getSelectionModel().select(ProtocolFactory.getVersion(adapterController.getAdapter().getProtocol().getVersion()));
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
        //add change listener, if new item is selected in combo box, other fields are changed
        view.getAdapterProtocolComboBox().valueProperty().addListener((observable, oldValue, newValue) -> {
            selectedVersion = (Protocol.Version) newValue;
        });
        //name
        view.getAdapterNameLbl().setText(adapterController.getAdapter().getName());
        //registered
        if(adapterController.getAdapter().getRegistered()){
            view.getAdapterYesRegisteredRadBtn().setSelected(true);
        }else{
            view.getAdapterNoRegisteredRadBtn().setSelected(true);
        }
        //bind field validation support to variable holding completion of protocol version
        adapterValidationSupport.validationResultProperty().addListener((observable, oldValue, newValue) -> {
                setAdapterInfoSet(!adapterValidationSupport.isInvalid());
        });

    }

    public void change(){
        if(getAdapterInfoSet()){
            logger.trace("Finishing");
            changeAdapter();
            close();
        }else {
            logger.trace("Adapter info not filled. Cannot \"Finish\" dialog");
            String message = null;
            for(ValidationMessage msg : adapterValidationSupport.getValidationResult().getErrors()){
                if(message == null){
                    message = msg.getText();
                }else {
                    message = message + "\n"  + msg.getText();
                }
            }
            showWarning("Adapter information","Please correctly fill all necessary information",message);
        }
    }

    public void close(){
        window.hide();
    }

    private boolean changeAdapter(){
        boolean noChange = true;
        if(!adapterController.getAdapter().getName().equals(view.getAdapterNameLbl().getText())){
            logger.debug("Setting adapter name (old/new) -> " + adapterController.getAdapter().getName() + "/" + view.getAdapterNameLbl().getText());
            adapterController.getAdapter().setName(view.getAdapterNameLbl().getText());
            adapterController.setSaved(false);
        }
        if(adapterController.getAdapter().getFirmware() != Double.valueOf(view.getAdapterFirmwareLbl().getText())){
            logger.debug("Setting new adapter firmware (old/new) -> " + adapterController.getAdapter().getFirmware() + "/" + view.getAdapterFirmwareLbl().getText());
            adapterController.getAdapter().setFirmware(Double.valueOf(view.getAdapterFirmwareLbl().getText()));
            adapterController.setSaved(false);
        }
        if(selectedVersion != null && !selectedVersion.equals(ProtocolFactory.getVersion(adapterController.getAdapter().getProtocolVersion()))){
            logger.trace("Setting new protocol version (old/new) -> " + adapterController.getAdapter().getProtocolVersion() + "/" + selectedVersion.getVersion());
            adapterController.getAdapter().setProtocol(ProtocolFactory.buildProtocol(selectedVersion));
            adapterController.setSaved(false);
        }
        boolean yesSelected;
        if(view.getAdapterYesRegisteredRadBtn().isSelected()) {
            yesSelected = true;
        }else {
            yesSelected = false;
        }
        if(adapterController.getAdapter().getRegistered() != yesSelected){
            logger.debug("Setting adapter registered (old/new) -> " + adapterController.getAdapter().getRegistered() + "/" + yesSelected);
            adapterController.getAdapter().setRegistered(yesSelected);
            if(yesSelected){
                adapterController.setRegisterMessageSent(true);
            }else {
                adapterController.setRegisterMessageSent(false);
            }
            adapterController.setSaved(false);
        }
        return true;
    }

    private ObservableList<Protocol.Version> getProtocolVersions(){
        return FXCollections.observableArrayList(Protocol.Version.values());
    }

    @Override
    public Node loadView() throws IOException {
        logger.trace("Loading ChangeAdapterDetailsDialogView from: " + FXML_PATH);
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
        adapterValidationSupport.registerValidator(view.getAdapterNameLbl(), false, Validator.createEmptyValidator("Adapter name is required"));
        adapterValidationSupport.registerValidator(view.getAdapterFirmwareLbl(), false, Validator.createEmptyValidator("Firmware is required"));
        adapterValidationSupport.registerValidator(view.getAdapterFirmwareLbl(), false, (Control c, String newValue) ->
                ValidationResult.fromErrorIf(view.getAdapterFirmwareLbl(), "Firmware must be a number", !Utilities.isNumeric(newValue)));
        adapterValidationSupport.registerValidator(view.getAdapterProtocolComboBox(), false, (Control c, Protocol.Version version) ->
                ValidationResult.fromErrorIf(view.getAdapterProtocolComboBox(), "Protocol version is required", (version == null)));
        //--adapter placeholders
        view.getAdapterNameLbl().setPromptText("Example: EA51914");
        view.getAdapterFirmwareLbl().setPromptText("Example: 0");
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

}
