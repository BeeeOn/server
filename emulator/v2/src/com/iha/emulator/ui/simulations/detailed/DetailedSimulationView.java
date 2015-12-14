package com.iha.emulator.ui.simulations.detailed;

import com.iha.emulator.control.AdapterController;
import com.iha.emulator.ui.panels.adapter.AdapterButton;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.control.*;
import javafx.scene.layout.FlowPane;
import javafx.scene.layout.StackPane;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.net.URL;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.ResourceBundle;

/**
 * Class providing GUI components for "Detailed simulation". Part View of MVP design pattern.
 * Contains methods invoked on user interaction with user interface. These methods call appropriate presenter method
 * to process user interaction.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class DetailedSimulationView implements Initializable,DetailedSimulationPresenter.Display{
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(DetailedSimulationView.class);
    /** presenter */
    private DetailedSimulationPresenter presenter;
    /** GUI components */
    //region MENU
    @FXML private MenuItem newAdapterItem;
    @FXML private MenuItem newSensorItem;
    @FXML private MenuItem enableAdapterItem;
    @FXML private MenuItem disableAdapterItem;
    @FXML private MenuItem disableAllAdaptersItem;
    @FXML private MenuItem deleteAdapterItem;
    @FXML private MenuItem deleteSensorItem;
    @FXML private MenuItem openItem;
    @FXML private MenuItem saveItem;
    @FXML private MenuItem saveAllItem;
    @FXML private MenuItem printItem;
    @FXML private MenuItem preferencesItem;
    @FXML private MenuItem quitItem;
    //endregion
    //region TOOLBAR (TBtn = Toolbar button)
    @FXML private ToolBar tooBar;
    @FXML private Button openTBtn;
    @FXML private Button saveTBtn;
    @FXML private Button saveAllTBtn;
    @FXML private Button enableAdapterTBtn;
    @FXML private Button disableAdapterTBtn;
    @FXML private Button newAdapterTBtn;
    @FXML private Button deleteAdapterTBtn;
    @FXML private Button enableInternetTBtn;
    @FXML private Button disableInternetTBtn;
    @FXML private MenuItem newAdapterSubItem;
    @FXML private MenuItem newSensorSubItem;
    //endregion
    @FXML private Node view;
    //region BOTTOM STATUS LINE
    @FXML private Label statusLineLbl;
    @FXML private ProgressIndicator statusIndicator;
    @FXML private ProgressBar memCheckProgressBar;
    @FXML private Label memCheckStatusLbl;
    //endregion
    @FXML private TextArea appLog;
    //region SERVER DETAILS
    @FXML private FlowPane serverDetailsContainer;
    @FXML private FlowPane adapterDetailsContainer;
    //endregion
    //region ADAPTER
    @FXML private FlowPane adapterBtnsContainer;
    @FXML private Button addAdapterBtn;
    private ArrayList<AdapterButton> adapterBtns;
    private AdapterButtonClickHandler adapterButtonClickHandler = new AdapterButtonClickHandler();
    @FXML private StackPane adapterLogContainer;
    @FXML private ScrollPane toBeSentLogContainer;
    @FXML private TabPane logTabPane;
    @FXML private Tab errorTab;
    @FXML private StackPane errorLogContainer;
    @FXML private ToggleGroup logMessageTypeRadBtnGroup;
    @FXML private RadioButton fullLogMessageRadBtn;
    @FXML private RadioButton partialLogMessageRadBtn;
    @FXML private RadioButton shortLogMessageRadBtn;
    @FXML private Button showFullLogBtn;
    //endregion
    //region SENSORS
    @FXML private Button addNewSensorBtn;
    @FXML private FlowPane sensorPanelContainer;
    //endregion

    @FXML
    public void handleEnableAdapter(ActionEvent event) {
        logger.trace("Enable adapter Clicked!");
        presenter.enableCurrentAdapter();
    }

    @FXML
    public void handleDisableAdapter(ActionEvent event) {
        logger.trace("Disable adapter Clicked!");
        presenter.disableCurrentAdapter();
    }

    @FXML
    public void handleDisableAllAdapters(ActionEvent event) {
        logger.trace("Disable all adapters Clicked!");
        presenter.disableAllAdapters();
    }

    @FXML
    public void handleNewAdapter(ActionEvent event) {
        logger.trace("New adapter Clicked!");
        presenter.addNewAdapter();
    }

    @FXML
    public void handleDeleteAdapter(ActionEvent event) {
        logger.debug("Delete adapter Clicked!");
        presenter.deleteAdapter();
    }

    @FXML
    public void handleDeleteSensor(ActionEvent event) {
        logger.debug("Delete sensor Clicked!");
        presenter.deleteSensor();
    }

    @FXML
    public void handleOpen(ActionEvent event) {
        logger.debug("Open Clicked!");
        presenter.open();
    }

    @FXML
    public void handleSave(ActionEvent event) {
        logger.trace("Save current adapter Clicked!");
        presenter.saveCurrentAdapter(null);
    }

    @FXML
    public void handleSaveAll(ActionEvent event) {
        logger.trace("Save all adapters Clicked!");
        presenter.saveAllAdapters();
    }

    @FXML
    public void handlePrint(ActionEvent event) {
        logger.debug("Print Clicked! -> unimplemented");
    }

    @FXML
    public void handlePreferences(ActionEvent event) {
        logger.debug("Preferences Clicked! -> unimplemented");
    }

    @FXML
    public void handleQuit(ActionEvent event) {
        if(presenter!=null) presenter.quit();
    }

    @FXML
    public void handleEnableInternet(ActionEvent event) {
        logger.trace("Enable internet connection Clicked!");
        presenter.enableInternetConnection();
    }

    @FXML
    public void handleDisableInternet(ActionEvent event) {
        logger.trace("Disable internet connection Clicked!");
        presenter.disableInternetConnection();
    }

    @FXML
    public void handleNewSensor(ActionEvent event) {
        logger.debug("Add new sensor Clicked!");
        presenter.addNewSensor();
    }

    @FXML
    public void handleShowFullLog(ActionEvent event) {
        logger.trace("Show full log Clicked!");
        presenter.showFullLog();
    }

    @FXML
    public void handleChangeServerDetails(ActionEvent event){
        logger.trace("Change server settings Clicked!");
        presenter.changeServerDetails();
    }

    @FXML
    public void handleChangeAdapterDetails(ActionEvent event){
        logger.trace("Change adapter details Clicked!");
        presenter.changeAdapterDetails();
    }

    @Override
    public TabPane getLogTabPane() {
        return logTabPane;
    }

    @Override
    public Tab getErrorTab() {
        return errorTab;
    }

    @Override
    public void setStatusLine(String status){
        statusLineLbl.setText(status);
    }

    @Override
    public void setStatusIndicator(boolean visible) {
        statusIndicator.setVisible(visible);
    }

    @Override
    public ProgressBar getMemCheckProgressBar() {
        return memCheckProgressBar;
    }

    @Override
    public Label getMemCheckStatusLabel() {
        return memCheckStatusLbl;
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        enableAdapterTBtn.setDisable(true);
    }

    @Override
    public void setPresenter(DetailedSimulationPresenter presenter) {
        this.presenter = presenter;
    }

    @Override
    public Node getView(){
        return view;
    }

    @Override
    public void setAdapterBtns(boolean enableBtnDisabled,boolean disableBtnDisabled){
        this.enableAdapterTBtn.setDisable(enableBtnDisabled);
        this.disableAdapterTBtn.setDisable(disableBtnDisabled);
    }
    @Override
    public void setInternetBtns(boolean enableBtnDisabled,boolean disableBtnDisabled){
        this.enableInternetTBtn.setDisable(enableBtnDisabled);
        this.disableInternetTBtn.setDisable(disableBtnDisabled);
    }

    @Override
    public TextArea getApplicationLogTextArea() {
        return appLog;
    }

    @Override
    public void addServerDetailsView(Node serverDetailsView) {
        serverDetailsContainer.getChildren().add(serverDetailsView);
    }

    @Override
    public void addAdapterDetailsView(Node adapterDetailsView) {
        adapterDetailsContainer.getChildren().add(adapterDetailsView);
    }

    @Override
    public void addAdapterBtn(AdapterButton newBtn) {
        //create if doesn't exist
        if(adapterBtns== null)adapterBtns = new ArrayList<>();
        //save to gui
        adapterBtnsContainer.getChildren().add(0,newBtn);
        //save to list
        adapterBtns.add(newBtn);
        newBtn.setOnAction(adapterButtonClickHandler);
    }

    @Override
    public void removeAdapterBtn(AdapterController adapterController){
        if(adapterBtns != null && adapterBtns.size() > 0){
            for(Iterator<AdapterButton> it = adapterBtns.iterator();it.hasNext();){
                AdapterButton btn = it.next();
                if(btn.getController().equals(adapterController)){
                    it.remove();
                    adapterBtnsContainer.getChildren().remove(btn);
                }
            }
        }
    }

    @Override
    public ArrayList<AdapterButton> getAdapterBtns() {
        if(adapterBtns== null)adapterBtns = new ArrayList<>();
        return adapterBtns;
    }

    @Override
    public Button getEnableAdapterBtn() {
        return enableAdapterTBtn;
    }

    @Override
    public Button getDisableAdapterBtn() {
        return disableAdapterTBtn;
    }

    @Override
    public Button getDeleteAdapterBtn() {
        return deleteAdapterTBtn;
    }

    @Override
    public Button getEnableInternetBtn() {
        return enableInternetTBtn;
    }

    @Override
    public Button getDisableInternetBtn() {
        return disableInternetTBtn;
    }

    @Override
    public Button getSaveBtn() {
        return saveTBtn;
    }

    @Override
    public Button getSaveAllBtn() {
        return saveAllTBtn;
    }

    @Override
    public StackPane getErrorLogContainer() {
        return errorLogContainer;
    }

    @Override
    public ScrollPane getToBeSentLogContainer() {
        return toBeSentLogContainer;
    }

    @Override
    public StackPane getAdapterLogContainer() {
        return adapterLogContainer;
    }

    @Override
    public RadioButton getShortLogMessageRadBtn() {
        return shortLogMessageRadBtn;
    }

    @Override
    public RadioButton getPartialLogMessageRadBtn() {
        return partialLogMessageRadBtn;
    }

    @Override
    public Button getAddNewSensorBtn() {
        return addNewSensorBtn;
    }

    @Override
    public FlowPane getSensorPanelContainer() {
        return sensorPanelContainer;
    }

    @Override
    public RadioButton getFullLogMessageRadBtn() {
        return fullLogMessageRadBtn;
    }

    @Override
    public ToggleGroup getLogMessageTypeRadBtnGroup() {
        return logMessageTypeRadBtnGroup;
    }

    @Override
    public Button getShowFullLogBtn() {
        return showFullLogBtn;
    }

    @Override
    public MenuItem getNewSensorItem() {
        return newSensorItem;
    }

    @Override
    public MenuItem getNewSensorSubItem() {
        return newSensorSubItem;
    }

    @Override
    public MenuItem getEnableAdapterItem() {
        return enableAdapterItem;
    }

    @Override
    public MenuItem getDisableAdapterItem() {
        return disableAdapterItem;
    }

    @Override
    public MenuItem getDisableAllAdaptersItem() {
        return disableAllAdaptersItem;
    }

    @Override
    public MenuItem getDeleteAdapterItem() {
        return deleteAdapterItem;
    }

    @Override
    public MenuItem getDeleteSensorItem() {
        return deleteSensorItem;
    }

    @Override
    public MenuItem getNewAdapterSubItem() {
        return newAdapterSubItem;
    }

    private class AdapterButtonClickHandler implements EventHandler<ActionEvent>{
        @Override
        public void handle(ActionEvent event) {
            presenter.setCurrentAdapter(((AdapterButton)event.getSource()).getController());
        }
    }
}
