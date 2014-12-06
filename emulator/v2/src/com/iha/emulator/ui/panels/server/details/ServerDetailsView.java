package com.iha.emulator.ui.panels.server.details;

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
 * Created by Shu on 26.11.2014.
 */
public class ServerDetailsView implements Initializable,ServerDetailsPresenter.Display {

    private static final Logger logger = LogManager.getLogger(ServerDetailsView.class);

    private ServerDetailsPresenter presenter;

    @FXML private Node view;

    @FXML private Label connectionLbl;
    @FXML private Label nameLbl;
    @FXML private Label ipLbl;
    @FXML private Label portLbl;
    @FXML private Label dbNameLbl;

    public ServerDetailsView() {

    }

    @Override
    public void setPresenter(ServerDetailsPresenter presenter) {
        this.presenter = presenter;
    }

    @Override
    public void setConnection(boolean b) {
        if(b){
            this.connectionLbl.setText("Connected");
            this.connectionLbl.getStyleClass().remove("statusInactive");
            this.connectionLbl.getStyleClass().add("statusActive");
        }else {
            this.connectionLbl.setText("Disconnected");
            this.connectionLbl.getStyleClass().remove("statusActive");
            this.connectionLbl.getStyleClass().add("statusInactive");
        }
    }

    @Override
    public void setIp(String ip) {
        this.ipLbl.setText(ip);
    }

    @Override
    public void setPort(String port) {
        this.portLbl.setText(port);
    }

    @Override
    public Label getConnectionLbl() {
        return connectionLbl;
    }

    @Override
    public Label getNameLbl() {
        return nameLbl;
    }

    @Override
    public Label getIpLbl() {
        return ipLbl;
    }

    @Override
    public Label getPortLbl() {
        return portLbl;
    }

    @Override
    public Label getDbNameLbl() {
        return dbNameLbl;
    }

    @Override
    public Node getView() {
        return view;
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        logger.trace("Initializing ServerDetailsView");
        connectionLbl.textProperty().addListener(new ChangeListener<String>() {
            @Override
            public void changed(ObservableValue<? extends String> observable, String oldValue, String newValue) {
                if(newValue.equals("Connected")){
                    connectionLbl.getStyleClass().remove("statusInactive");
                    connectionLbl.getStyleClass().add("statusActive");
                }else{
                    connectionLbl.getStyleClass().remove("statusActive");
                    connectionLbl.getStyleClass().add("statusInactive");
                }
            }
        });
    }
}
