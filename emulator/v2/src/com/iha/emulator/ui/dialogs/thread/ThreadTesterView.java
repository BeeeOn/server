package com.iha.emulator.ui.dialogs.thread;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.control.Label;
import javafx.scene.control.ProgressIndicator;
import javafx.scene.control.TextArea;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.net.URL;
import java.util.ResourceBundle;

/**
 * Created by Shu on 3.2.2015.
 */
public class ThreadTesterView implements Initializable,ThreadTesterPresenter.Display{

    private static final Logger logger = LogManager.getLogger(ThreadTesterView.class);
    private ThreadTesterPresenter presenter;

    @FXML
    private Node view;

    @FXML private ProgressIndicator indicator;
    @FXML private Label status;
    @FXML private TextArea textArea;

    public ThreadTesterView() {

    }

    @FXML
    public void handleStart(){
        presenter.start();
    }

    @FXML
    public void handleClose(ActionEvent event){
        logger.trace("Close button clicked");
        presenter.close();
    }

    @Override
    public Node getView() {
        return view;
    }

    @Override
    public void setPresenter(ThreadTesterPresenter presenter) {
        this.presenter = presenter;
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
    public TextArea getTextArea(){
        return textArea;
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {

    }
}
