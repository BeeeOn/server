package com.iha.emulator.ui.dialogs.log;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.control.CheckBox;
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
public class ShowFullLogView implements Initializable,ShowFullLogPresenter.Display{

    private static final Logger logger = LogManager.getLogger(ShowFullLogView.class);
    private ShowFullLogPresenter presenter;

    @FXML
    private Node view;

    @FXML private ProgressIndicator indicator;
    @FXML private Label status;
    @FXML private TextArea textArea;
    @FXML private CheckBox refreshAutomaticallyCheckBox;

    public ShowFullLogView() {

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
    public Node getView() {
        return view;
    }

    @Override
    public void setPresenter(ShowFullLogPresenter presenter) {
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
    public CheckBox getRefreshAutomaticallyCheckBox() {
        return refreshAutomaticallyCheckBox;
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {

    }
}
