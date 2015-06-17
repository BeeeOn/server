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
 * Class providing GUI components for "Application simulation chooser". Part View of MVP design pattern.
 * Contains methods invoked on user interaction with user interface. These methods call appropriate presenter method
 * to process user interaction.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class ChooserView implements Initializable,ChooserPresenter.Display {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(ChooserView.class);
    /** presenter */
    private ChooserPresenter presenter;
    /** GUI components */
    @FXML private Node view;

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

    @FXML
    public void handleMaxThreadTest(ActionEvent event){
        presenter.runMaxThreadTest();
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
