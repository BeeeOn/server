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
 * Class providing logic to user interactions for "Change adapter settings dialog". Part Presenter of MVP design pattern.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class ChangeAdapterDetailsDialogPresenter implements Presenter,PanelPresenter{
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(ChangeAdapterDetailsDialogPresenter.class);
    /** path to FXML file */
    private static final String FXML_PATH = "ChangeAdapterDetailsDialog.fxml";
    /** adapter information validator */
    private ValidationSupport adapterValidationSupport = new ValidationSupport();
    /** view */
    private Display view;
    /** window */
    private Stage window;
    /** adapter controller */
    private AdapterController adapterController;
    /** all needed adapter information set property */
    private BooleanProperty adapterInfoSet;
    /** selected protocol version */
    private Protocol.Version selectedVersion;
    /**
     * Interface implemented by "Change adapter settings dialog" view.
     */
    public interface Display {
        public Node getView();
        public void setPresenter(ChangeAdapterDetailsDialogPresenter presenter);
        public Button getChangeBtn();
        public Button getCloseBtn();
        public VBox getAdapterContainer();
        public TextField getAdapterNameLbl();
        public TextField getAdapterFirmwareLbl();
        public ComboBox getAdapterProtocolComboBox();
    }

    /**
     * Creates "Change adapter settings dialog" presenter for given adapter.
     * @param stage parent window
     * @param adapterController adapter controller
     */
    public ChangeAdapterDetailsDialogPresenter(Stage stage, AdapterController adapterController) {
        this.adapterInfoSet = new SimpleBooleanProperty(false);
        this.window = stage;
        this.adapterController = adapterController;
    }
    /**
     * Initializes dialog. Fills components with data and sets validation options.
     */
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
        //save change listener, if new item is selected in combo box, other fields are changed
        view.getAdapterProtocolComboBox().valueProperty().addListener((observable, oldValue, newValue) -> {
            selectedVersion = (Protocol.Version) newValue;
        });
        //name
        view.getAdapterNameLbl().setText(adapterController.getAdapter().getName());
        //bind field validation support to variable holding completion of protocol version
        adapterValidationSupport.validationResultProperty().addListener((observable, oldValue, newValue) -> {
                setAdapterInfoSet(!adapterValidationSupport.isInvalid());
        });

    }

    /**
     * If all needed information about adapter is set, applies changes and closes dialog. Otherwise notifies user.
     */
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

    /**
     * Closes dialog
     */
    public void close(){
        window.hide();
    }

    /**
     * Checks if any information was changed, if it was, applies changes to adapter's model.
     */
    private void changeAdapter(){
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
    }
    /**
     * Gets list of all implemented protocol version
     * @return list of all implemented protocol version
     */
    private ObservableList<Protocol.Version> getProtocolVersions(){
        return FXCollections.observableArrayList(Protocol.Version.values());
    }

    /**
     * {@inheritDoc}
     */
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

}
