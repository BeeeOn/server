package com.iha.emulator.ui.dialogs.server;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.control.Button;
import javafx.scene.control.ComboBox;
import javafx.scene.control.TextField;
import javafx.scene.layout.VBox;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.net.URL;
import java.util.ResourceBundle;

/**
 * Created by Shu on 5.12.2014.
 */
public class ChangeServerDetailsDialogView implements Initializable,ChangeServerDetailsDialogPresenter.Display{

    private static final Logger logger = LogManager.getLogger(ChangeServerDetailsDialogView.class);
    private ChangeServerDetailsDialogPresenter presenter;

    @FXML private Node view;

    @FXML private Button changeBtn;
    @FXML private Button closeBtn;

    @FXML private VBox serverContainer;
    //region SERVER
    @FXML private TextField serverNameTxtField;
    @FXML private TextField serverIpTxtField;
    @FXML private TextField serverPortTxtField;
    @FXML private TextField serverDbNameTxtField;
    @FXML private ComboBox serverComboBox;
    //endregion
    @FXML
    public void handleChange(ActionEvent event) {
        logger.trace("Change button clicked");
        presenter.change();
    }

    @FXML
    public void handleClose(ActionEvent event) {
        logger.trace("Close button clicked");
        presenter.close();
    }

    @Override
    public Button getChangeBtn() {
        return changeBtn;
    }

    @Override
    public Button getCloseBtn() {
        return closeBtn;
    }

    @Override
    public VBox getServerContainer() {
        return serverContainer;
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
    public ComboBox getServerComboBox() {
        return serverComboBox;
    }


    @Override
    public Node getView() {
        return view;
    }

    @Override
    public void setPresenter(ChangeServerDetailsDialogPresenter presenter) {
        this.presenter = presenter;
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        serverContainer.setVisible(true);
    }
}