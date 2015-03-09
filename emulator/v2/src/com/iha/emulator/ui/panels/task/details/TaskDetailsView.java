package com.iha.emulator.ui.panels.task.details;

import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.control.Label;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.net.URL;
import java.util.ResourceBundle;

/**
 * Created by Shu on 26.11.2014.
 */
public class TaskDetailsView implements Initializable,TaskDetailsPresenter.Display {

    private static final Logger logger = LogManager.getLogger(TaskDetailsView.class);

    private TaskDetailsPresenter presenter;

    @FXML private Node view;

    @FXML private Label idLbl;
    @FXML private Label stateLbl;
    @FXML private Label devicesCountLbl;
    @FXML private Label adaptersCountLbl;
    @FXML private Label waitingMessagesLbl;
    @FXML private Label sentMessagesLbl;

    public TaskDetailsView() {

    }

    @Override
    public void setPresenter(TaskDetailsPresenter presenter) {
        this.presenter = presenter;
    }

    @Override
    public Label getIdLbl() {
        return idLbl;
    }

    @Override
    public Label getStateLbl() {
        return stateLbl;
    }

    @Override
    public Label getDevicesCountLbl() {
        return devicesCountLbl;
    }

    @Override
    public Label getAdaptersCountLbl() {
        return adaptersCountLbl;
    }

    @Override
    public Label getWaitingMessagesLbl() {
        return waitingMessagesLbl;
    }

    @Override
    public Label getSentMessagesLbl() {
        return sentMessagesLbl;
    }

    @Override
    public Node getView() {
        return view;
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {

    }
}
