package com.iha.emulator.ui.dialogs.sensor;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.control.CheckBox;
import javafx.scene.control.Label;
import javafx.scene.control.ProgressIndicator;
import javafx.scene.layout.StackPane;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.net.URL;
import java.util.ResourceBundle;

/**
 * Created by Shu on 6.12.2014.
 */
public class DeleteSensorsDialogView implements Initializable,DeleteSensorsDialogPresenter.Display {

    private static final Logger logger = LogManager.getLogger(DeleteSensorsDialogView.class);
    private DeleteSensorsDialogPresenter presenter;

    @FXML private Node view;

    @FXML private ProgressIndicator indicator;
    @FXML private Label status;
    @FXML private StackPane listConteiner;

    @FXML private CheckBox databaseCheckBox;

    public DeleteSensorsDialogView() {

    }

    @FXML
    public void handleDelete(ActionEvent event){
        logger.trace("Delete button clicked");
        presenter.delete();
    }

    @FXML
    public void handleClose(ActionEvent event){
        logger.trace("Close button clicked");
        presenter.close();
    }

    @Override
    public CheckBox getDatabaseCheckBox() {
        return databaseCheckBox;
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
    public StackPane getListContainer() {
        return listConteiner;
    }

    @Override
    public Node getView() {
        return view;
    }

    @Override
    public void setPresenter(DeleteSensorsDialogPresenter presenter) {
        this.presenter = presenter;
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {

    }
}
