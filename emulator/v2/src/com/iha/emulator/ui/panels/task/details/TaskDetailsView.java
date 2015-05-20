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
 * Class providing GUI components for "Task details panel". Part View of MVP design pattern.
 * Contains methods invoked on user interaction with user interface. These methods call appropriate presenter method
 * to process user interaction.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class TaskDetailsView implements Initializable,TaskDetailsPresenter.Display {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(TaskDetailsView.class);
    /** presenter */
    private TaskDetailsPresenter presenter;
    /** GUI components */
    @FXML private Node view;
    @FXML private Label idLbl;
    @FXML private Label stateLbl;
    @FXML private Label devicesCountLbl;
    @FXML private Label adaptersCountLbl;
    @FXML private Label waitingMessagesLbl;
    @FXML private Label sentMessagesLbl;
    @FXML private Label stopWatchLbl;
    @FXML private Label respPerSecondLbl;
    @FXML private Label maxRespPerSecondLbl;

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
    public Label getStopWatchLbl() {
        return stopWatchLbl;
    }

    @Override
    public Label getRespPerSecondLbl() {
        return respPerSecondLbl;
    }

    @Override
    public Label getMaxRespPerSecondLbl() {
        return maxRespPerSecondLbl;
    }

    @Override
    public Node getView() {
        return view;
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {

    }
}
