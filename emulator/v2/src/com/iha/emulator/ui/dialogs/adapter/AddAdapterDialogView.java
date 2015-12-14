package com.iha.emulator.ui.dialogs.adapter;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.control.*;
import javafx.scene.layout.VBox;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.net.URL;
import java.util.ResourceBundle;

/**
 * Class providing GUI components for "Add adapter dialog". Part View of MVP design pattern.
 * Contains methods invoked on user interaction with user interface. These methods call appropriate presenter method
 * to process user interaction.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class AddAdapterDialogView implements Initializable,AddAdapterDialogPresenter.Display{
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(AddAdapterDialogView.class);
    /** presenter */
    private AddAdapterDialogPresenter presenter;
    /** GUI components */
    @FXML private Node view;
    @FXML private Button previousBtn;
    @FXML private Button nextBtn;
    @FXML private Button finishBtn;
    @FXML private VBox serverContainer;
    @FXML private VBox adapterContainer;
    //region SERVER
    @FXML private TextField serverNameTxtField;
    @FXML private TextField serverIpTxtField;
    @FXML private TextField serverPortTxtField;
    @FXML private TextField serverDbNameTxtField;
    @FXML private ComboBox serverComboBox;
    @FXML private CheckBox serverModifyCheckBox;
    //endregion
    //region ADAPTER
    @FXML private MenuItem checkIdMenuItem;
    @FXML private MenuItem showAdaptersMenuItem;
    @FXML private TextField adapterIdLbl;
    @FXML private TextField adapterNameLbl;
    @FXML private TextField adapterFirmwareLbl;
    @FXML private ComboBox adapterProtocolComboBox;
    //endregion
    @FXML
    public void handleNext(ActionEvent event) {
        logger.trace("Next panel clicked");
        presenter.next();
    }

    @FXML
    public void handlePrevious(ActionEvent event) {
        logger.trace("Previous panel clicked");
        presenter.previous();
    }

    @FXML
    public void handleFinish(ActionEvent event) {
        logger.trace("Finish clicked");
        presenter.finish();
    }

    @FXML
    public void handleCheckId(ActionEvent event) {
        logger.trace("Checking id");
        presenter.checkId();
    }

    @FXML
    public void handleShowAdapters(ActionEvent event) {
        logger.trace("Showing adapters in database");
        presenter.showAdapters();
    }

    @Override
    public Button getPreviousBtn() {
        return previousBtn;
    }

    @Override
    public Button getNextBtn() {
        return nextBtn;
    }

    @Override
    public Button getFinishBtn() {
        return finishBtn;
    }

    @Override
    public VBox getServerContainer() {
        return serverContainer;
    }

    @Override
    public VBox getAdapterContainer() {
        return adapterContainer;
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
    public CheckBox getServerModifyCheckBox() {
        return serverModifyCheckBox;
    }

    @Override
    public MenuItem getCheckIdMenuItem() {
        return checkIdMenuItem;
    }

    @Override
    public MenuItem getShowAdaptersMenuItem() {
        return showAdaptersMenuItem;
    }

    @Override
    public TextField getAdapterIdLbl() {
        return adapterIdLbl;
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
    public void setPresenter(AddAdapterDialogPresenter presenter) {
        this.presenter = presenter;
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        serverContainer.setVisible(true);
        adapterContainer.setVisible(false);
    }
}
