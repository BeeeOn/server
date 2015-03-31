package com.iha.emulator.ui.dialogs.adapter;

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
public class ChangeAdapterDetailsDialogView implements Initializable,ChangeAdapterDetailsDialogPresenter.Display{

    private static final Logger logger = LogManager.getLogger(ChangeAdapterDetailsDialogView.class);
    private ChangeAdapterDetailsDialogPresenter presenter;

    @FXML private Node view;

    @FXML private Button changeBtn;
    @FXML private Button closeBtn;

    @FXML private VBox adapterContainer;
    //region ADAPTER
    @FXML private TextField adapterNameLbl;
    @FXML private TextField adapterFirmwareLbl;
    @FXML private ComboBox adapterProtocolComboBox;
    //endregion

    @FXML
    public void handleChange(ActionEvent event) {
        logger.trace("Change clicked");
        presenter.change();
    }

    @FXML
    public void handleClose(ActionEvent event) {
        logger.trace("Close clicked");
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
    public VBox getAdapterContainer() {
        return adapterContainer;
    }

    @Override
    public TextField getAdapterNameLbl() {
        return adapterNameLbl;
    }

    @Override
    public TextField getAdapterFirmwareLbl() {
        return adapterFirmwareLbl;
    }

    @Override
    public ComboBox getAdapterProtocolComboBox() {
        return adapterProtocolComboBox;
    }

    @Override
    public Node getView() {
        return view;
    }

    @Override
    public void setPresenter(ChangeAdapterDetailsDialogPresenter presenter) {
        this.presenter = presenter;
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        adapterContainer.setVisible(true);
    }
}
