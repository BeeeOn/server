package com.iha.emulator.ui.simulations.chooser;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.net.URL;
import java.util.ResourceBundle;

/**
 * Created by Shu on 25.2.2015.
 */
public class ChooserView implements Initializable,ChooserPresenter.Display {

    private static final Logger logger = LogManager.getLogger(ChooserView.class);

    private ChooserPresenter presenter;

    @FXML private Node view;

    public ChooserView() {

    }

    @FXML
    public void handleDetailedSimulation(ActionEvent event){
        logger.trace("Detailed simulation Clicked!");
        presenter.runDetailedSimulation();
    }

    @FXML
    public void handlePerformanceSimulation(ActionEvent event){
        logger.trace("Performance simulation Clicked!");
        presenter.runPerformanceSimulation();
    }

    @Override
    public void setPresenter(ChooserPresenter presenter) {
        this.presenter = presenter;
    }

    @Override
    public Node getView() {
        return view;
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {

    }
}
