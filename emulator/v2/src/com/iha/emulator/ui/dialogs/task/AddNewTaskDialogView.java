package com.iha.emulator.ui.dialogs.task;

import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.control.*;
import javafx.scene.layout.GridPane;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.controlsfx.control.RangeSlider;

import java.net.URL;
import java.util.ResourceBundle;

/**
 * Created by Shu on 10.12.2014.
 */
public class AddNewTaskDialogView implements Initializable,AddNewTaskDialogPresenter.Display{

    private static final Logger logger = LogManager.getLogger(AddNewTaskDialogView.class);

    private AddNewTaskDialogPresenter presenter;

    @FXML private Node view;
    @FXML private Button loadTemplateBtn;
    @FXML private Button saveTemplateBtn;
    @FXML private Button addBtn;
    @FXML private Button cancelBtn;
    //SERVER INFORMATION
    @FXML private ComboBox serverComboBox;
    @FXML private TextField serverNameTxtField;
    @FXML private TextField serverIpTxtField;
    @FXML private TextField serverPortTxtField;
    @FXML private TextField serverDbNameTxtField;
    @FXML private CheckBox serverModifyCheckBox;
    //STOP CONDITIONS
    @FXML private RadioButton timeDurationYesRadBtn;
    @FXML private RadioButton timeDurationNoRadBtn;
    @FXML private RadioButton sentMessagesYesRadBtn;
    @FXML private RadioButton sentMessagesNoRadBtn;
    @FXML private RadioButton waitingMessagesYesRadBtn;
    @FXML private RadioButton waitingMessagesNoRadBtn;
    @FXML private ComboBox timeHoursComboBox;
    @FXML private ComboBox timeMinutesComboBox;
    @FXML private ComboBox timeSecondsComboBox;
    @FXML private TextField sentMessagesCountTxtField;
    @FXML private TextField waitingMessagesCountTxtField;
    //task
    @FXML private GridPane taskInformationContainer;
    private RangeSlider sensorsCountSlider;
    @FXML private Label sensorCountMinLbl;
    @FXML private Label sensorCountMaxLbl;
    private RangeSlider refreshTimeCountSlider;
    @FXML private Label refreshTimeMinLbl;
    @FXML private Label refreshTimeMaxLbl;
    @FXML private ComboBox protocolComboBox;
    @FXML private TextField saveDirTextField;
    @FXML private TextField adaptersCountTxtField;
    @FXML private TextField startIdTxtField;
    //values general
    @FXML private Button addValueBtn;
    @FXML private Button deleteValueBtn;
    @FXML private TreeView valuesTree;

    public AddNewTaskDialogView() {

    }

    @FXML
    public void handleLoadTemplate(){
        logger.trace("Load template clicked");
        presenter.loadTemplate();
    }

    @FXML
    public void handleSaveTemplate(){
        logger.trace("Save template clicked");
        presenter.saveTemplate();
    }

    @FXML
    public void handleAdd(){
        logger.trace("Add button clicked");
        presenter.add();
    }

    @FXML
    public void handleCancel(){
        logger.trace("Cancel button clicked");
        presenter.cancel();
    }

    @FXML
     public void handleAddValue(){
        logger.trace("Add value button clicked");
        presenter.addValue();
    }

    @FXML
    public void handleDeleteValue(){
        logger.trace("Delete value button clicked");
        presenter.deleteValue();
    }

    @FXML
    public void handleOpenSaveDir(){
        logger.trace("Open save dir clicked");
        presenter.openSaveDirDialog();
    }

    @FXML
    public void handleShowAdapters(){
        logger.trace("Show adapters clicked");
        presenter.showAdaptersInDB();
    }

    //region SERVER
    @Override
    public ComboBox getServerComboBox() {
        return serverComboBox;
    }
    @Override
    public TextField getServerNameTxtField() {
        return serverNameTxtField;
    }
    @Override
    public TextField getServerIpTxtField() {
        return serverIpTxtField;
    }
    @Override
    public TextField getServerPortTxtField() {
        return serverPortTxtField;
    }
    @Override
    public TextField getServerDbNameTxtField() {
        return serverDbNameTxtField;
    }
    @Override
    public CheckBox getServerModifyCheckBox() {
        return serverModifyCheckBox;
    }
    //endregion

    //region STOP CONDITIONS

    public RadioButton getTimeDurationYesRadBtn() {
        return timeDurationYesRadBtn;
    }

    public RadioButton getTimeDurationNoRadBtn() {
        return timeDurationNoRadBtn;
    }

    public RadioButton getSentMessagesYesRadBtn() {
        return sentMessagesYesRadBtn;
    }

    public RadioButton getSentMessagesNoRadBtn() {
        return sentMessagesNoRadBtn;
    }

    public RadioButton getWaitingMessagesYesRadBtn() {
        return waitingMessagesYesRadBtn;
    }

    public RadioButton getWaitingMessagesNoRadBtn() {
        return waitingMessagesNoRadBtn;
    }

    public ComboBox getTimeHoursComboBox() {
        return timeHoursComboBox;
    }

    public ComboBox getTimeMinutesComboBox() {
        return timeMinutesComboBox;
    }

    public ComboBox getTimeSecondsComboBox() {
        return timeSecondsComboBox;
    }

    public TextField getSentMessagesCountTxtField() {
        return sentMessagesCountTxtField;
    }

    public TextField getWaitingMessagesCountTxtField() {
        return waitingMessagesCountTxtField;
    }

    public RangeSlider getSensorsCountSlider() {
        return sensorsCountSlider;
    }

    public RangeSlider getRefreshTimeCountSlider() {
        return refreshTimeCountSlider;
    }

    public void setRefreshTimeCountSlider(RangeSlider refreshTimeCountSlider) {
        taskInformationContainer.add(refreshTimeCountSlider,2,3,2,1);
        this.refreshTimeCountSlider = refreshTimeCountSlider;
    }

    public void setSensorsCountSlider(RangeSlider sensorsCountSlider) {
        taskInformationContainer.add(sensorsCountSlider,2,2,2,1);
        this.sensorsCountSlider = sensorsCountSlider;
    }

    public Label getSensorCountMinLbl() {
        return sensorCountMinLbl;
    }

    public Label getSensorCountMaxLbl() {
        return sensorCountMaxLbl;
    }

    public Label getRefreshTimeMinLbl() {
        return refreshTimeMinLbl;
    }

    public Label getRefreshTimeMaxLbl() {
        return refreshTimeMaxLbl;
    }

    public ComboBox getProtocolComboBox() {
        return protocolComboBox;
    }

    public TextField getSaveDirTextField() {
        return saveDirTextField;
    }

    public TextField getAdaptersCountTxtField() {
        return adaptersCountTxtField;
    }

    public TextField getStartIdTxtField() {
        return startIdTxtField;
    }
    //endregion

    @Override
    public TreeView getValuesTree() {
        return valuesTree;
    }

    @Override
    public Node getView() {
        return view;
    }

    @Override
    public void setPresenter(AddNewTaskDialogPresenter presenter) {
        this.presenter = presenter;
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {

    }
}
