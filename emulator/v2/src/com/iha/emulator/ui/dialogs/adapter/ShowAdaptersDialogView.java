package com.iha.emulator.ui.dialogs.adapter;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.ProgressIndicator;
import javafx.scene.control.TableView;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.net.URL;
import java.util.ResourceBundle;

/**
 * Created by Shu on 6.12.2014.
 */
public class ShowAdaptersDialogView implements Initializable,ShowAdaptersDialogPresenter.Display {

    private static final Logger logger = LogManager.getLogger(ShowAdaptersDialogView.class);
    private ShowAdaptersDialogPresenter presenter;

    @FXML private Node view;

    @FXML private ProgressIndicator indicator;
    @FXML private Label status;
    @FXML private Button refreshBtn;
    @FXML private Button closeBtn;
    @FXML private TableView table;

    public ShowAdaptersDialogView() {

    }

    @FXML
    public void handleRefresh(ActionEvent event){
        logger.trace("Refresh button clicked");
        presenter.refresh();
    }

    @FXML
    public void handleClose(ActionEvent event){
        logger.trace("Close button clicked");
        presenter.close();
    }

    @Override
    public ProgressIndicator getIndicator() {
        return indicator;
    }

    @Override
    public Label getStatus() {
        return status;
    }

    @Override
    public Button getRefreshBtn() {
        return refreshBtn;
    }

    @Override
    public Button getCloseBtn() {
        return closeBtn;
    }

    @Override
    public TableView getTable() {
        return table;
    }

    @Override
    public Node getView() {
        return view;
    }

    @Override
    public void setPresenter(ShowAdaptersDialogPresenter presenter) {
        this.presenter = presenter;
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {

    }
}
