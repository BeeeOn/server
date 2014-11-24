package com.iha.emulator.ui.simulations.detailed;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.control.*;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.net.URL;
import java.util.ResourceBundle;

/**
 * Created by Shu on 23/11/14.
 */
public class DetailedSimulationView implements Initializable,DetailedSimulationPresenter.Display{

    private static final Logger logger = LogManager.getLogger(DetailedSimulationView.class);
    DetailedSimulationPresenter presenter;

    //region MENU
    @FXML private MenuItem newAdapterItem;
    @FXML private MenuItem openItem;
    @FXML private MenuItem saveItem;
    @FXML private MenuItem saveAllItem;
    @FXML private MenuItem printItem;
    @FXML private MenuItem preferencesItem;
    @FXML private MenuItem quitItem;
    //endregion
    //region TOOLBAR (TBtn = Toolbar button)
    @FXML private Button openTBtn;
    @FXML private Button saveTBtn;
    @FXML private Button saveAllTBtn;
    @FXML private Button printTBtn;
    @FXML private Button enableAdapterTBtn;
    @FXML private Button disableAdapterTBtn;
    @FXML private Button newAdapterTBtn;
    @FXML private Button deleteAdapterTBtn;
    @FXML private Button enableInternetTBtn;
    @FXML private Button disableInternetTBtn;
    //endregion



    @FXML private Node view;
    @FXML private Label statusLineLbl;
    @FXML private ProgressIndicator statusIndicator;
    @FXML private ProgressBar memCheckProgressBar;
    @FXML private Label memCheckStatusLbl;

    public DetailedSimulationView() {
        logger.trace("Trace message");
    }

    @FXML
    public void handleEnableAdapter(ActionEvent event) {
        logger.debug("Enable adapter Clicked! -> unimplemented");
    }

    @FXML
    public void handleDisableAdapter(ActionEvent event) {
        logger.debug("Disable adapter Clicked! -> unimplemented");
    }

    @FXML
    public void handleNewAdapter(ActionEvent event) {
        logger.debug("New adapter Clicked! -> unimplemented");
    }

    @FXML
    public void handleDeleteAdapter(ActionEvent event) {
        logger.debug("Delete adapter Clicked! -> unimplemented");
    }

    @FXML
    public void handleOpen(ActionEvent event) {
        logger.debug("Open Clicked! -> unimplemented");
    }


    @FXML
    public void handleSave(ActionEvent event) {
        logger.debug("Save Clicked! -> unimplemented");
    }

    @FXML
    public void handleSaveAll(ActionEvent event) {
        logger.debug("SaveAll Clicked! -> unimplemented");
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
        logger.debug("Enable Internet Clicked! -> unimplemented");
    }

    @FXML
    public void handleDisableInternet(ActionEvent event) {
        logger.debug("Disable internet Clicked! -> unimplemented");
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


}
