package com.iha.emulator.ui.panels.adapter.details;

import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.control.Label;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.net.URL;
import java.util.ResourceBundle;

/**
 * Class providing GUI components for "Adapter details panel". Part View of MVP design pattern.
 * Contains methods invoked on user interaction with user interface. These methods call appropriate presenter method
 * to process user interaction.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class AdapterDetailsView implements Initializable,AdapterDetailsPresenter.Display {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(AdapterDetailsView.class);
    /** presenter */
    private AdapterDetailsPresenter presenter;
    /** GUI components */
    @FXML private Node view;
    @FXML private Label statusLbl;
    @FXML private Label nameLbl;
    @FXML private Label idLbl;
    @FXML private Label registeredLbl;
    @FXML private Label activeCountLbl;
    @FXML private Label registeredCountLbl;
    @FXML private Label protocolLbl;
    @FXML private Label firmwareLbl;

    @Override
    public Label getStatusLbl() {
        return statusLbl;
    }

    @Override
    public Label getNameLbl() {
        return nameLbl;
    }

    @Override
    public Label getIdLbl() {
        return idLbl;
    }

    @Override
    public Label getRegisteredLbl() {
        return registeredLbl;
    }

    @Override
    public Label getActiveCountLbl() {
        return activeCountLbl;
    }

    @Override
    public Label getRegisteredCountLbl() {
        return registeredCountLbl;
    }

    @Override
    public Label getProtocolLbl() {
        return protocolLbl;
    }

    @Override
    public Label getFirmwareLbl() {
        return firmwareLbl;
    }

    @Override
    public Node getView() {
        return view;
    }

    @Override
    public void setPresenter(AdapterDetailsPresenter presenter) {
        this.presenter = presenter;
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        logger.trace("Initializing AdapterDetailsView");
        //if status label text changes, change it's color appropriately
        statusLbl.textProperty().addListener(new ChangeListener<String>() {
            @Override
            public void changed(ObservableValue<? extends String> observable, String oldValue, String newValue) {
                if(!newValue.equals("Inactive")){
                    statusLbl.getStyleClass().remove("statusInactive");
                    statusLbl.getStyleClass().add("statusActive");
                }else{
                    statusLbl.getStyleClass().remove("statusActive");
                    statusLbl.getStyleClass().add("statusInactive");
                }
            }
        });
    }
}
