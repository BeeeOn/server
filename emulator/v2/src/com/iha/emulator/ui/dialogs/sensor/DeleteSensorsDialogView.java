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
 * Class providing GUI components for "Delete sensor dialog". Part View of MVP design pattern.
 * Contains methods invoked on user interaction with user interface. These methods call appropriate presenter method
 * to process user interaction.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class DeleteSensorsDialogView implements Initializable,DeleteSensorsDialogPresenter.Display {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(DeleteSensorsDialogView.class);
    /** presenter */
    private DeleteSensorsDialogPresenter presenter;
    /** GUI components */
    @FXML private Node view;
    @FXML private ProgressIndicator indicator;
    @FXML private Label status;
    @FXML private StackPane listConteiner;
    @FXML private CheckBox databaseCheckBox;

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
