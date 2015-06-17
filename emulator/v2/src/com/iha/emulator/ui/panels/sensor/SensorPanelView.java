package com.iha.emulator.ui.panels.sensor;

import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TableView;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.net.URL;
import java.util.ResourceBundle;

/**
 * Class providing GUI components for "Sensor panel". Part View of MVP design pattern.
 * Contains methods invoked on user interaction with user interface. These methods call appropriate presenter method
 * to process user interaction.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class SensorPanelView implements Initializable,SensorPanelPresenter.Display {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(SensorPanelView.class);
    /** presenter */
    private SensorPanelPresenter presenter;
    /** GUI components */
    @FXML private Node view;
    @FXML private Label nameLbl;
    @FXML private Label batteryLbl;
    @FXML private Label signalLbl;
    @FXML private Label refreshTimeLbl;
    @FXML private Label statusLbl;
    @FXML private Label idLbl;
    @FXML private Button settingsBtn;
    @FXML private Button connectionBtn;
    @FXML private TableView valueTable;

    @FXML
    public void handleConnection(ActionEvent event) {
        logger.trace("Connect/Disconnect sensor button Clicked!");
        presenter.connection();
    }

    @FXML
    public void handleSettings(ActionEvent event) {
        logger.trace("Sensor settings Clicked!");
        presenter.settings();
    }

    @Override
    public Label getNameLbl() {
        return nameLbl;
    }

    @Override
    public Label getBatteryLbl() {
        return batteryLbl;
    }

    @Override
    public Label getSignalLbl() {
        return signalLbl;
    }

    @Override
    public Label getRefreshTimeLbl() {
        return refreshTimeLbl;
    }

    @Override
    public Label getStatusLbl() {
        return statusLbl;
    }

    @Override
    public Label getIdLbl() {
        return idLbl;
    }

    @Override
    public Button getSettingsBtn() {
        return settingsBtn;
    }

    @Override
    public Button getConnectionBtn() {
        return connectionBtn;
    }

    @Override
    public TableView getValueTable() {
        return valueTable;
    }

    @Override
    public Node getView() {
        return view;
    }

    @Override
    public void setPresenter(SensorPanelPresenter presenter) {
        this.presenter = presenter;
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        statusLbl.textProperty().addListener(new ChangeListener<String>() {
            @Override
            public void changed(ObservableValue<? extends String> observable, String oldValue, String newValue) {
                if(newValue.equals("Connected")){
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
