package com.iha.emulator.ui.panels.server.details;

import javafx.application.Platform;
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
 * Class providing GUI components for "Server details panel". Part View of MVP design pattern.
 * Contains methods invoked on user interaction with user interface. These methods call appropriate presenter method
 * to process user interaction.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class ServerDetailsView implements Initializable,ServerDetailsPresenter.Display {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(ServerDetailsView.class);
    /** presenter */
    private ServerDetailsPresenter presenter;
    /** GUI components */
    @FXML private Node view;
    @FXML private Label connectionLbl;
    @FXML private Label senderConnectionLbl;
    @FXML private Label nameLbl;
    @FXML private Label ipLbl;
    @FXML private Label portLbl;
    @FXML private Label dbNameLbl;

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
    public Label getSenderConnectionLbl() {
        return senderConnectionLbl;
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
                Platform.runLater(()-> {
                    if(newValue.equals("Connected")){
                        connectionLbl.getStyleClass().remove("statusInactive");
                        connectionLbl.getStyleClass().add("statusActive");
                    }else{
                        connectionLbl.getStyleClass().remove("statusActive");
                        connectionLbl.getStyleClass().add("statusInactive");
                    }
                });
            }
        });
        senderConnectionLbl.textProperty().addListener(new ChangeListener<String>() {
            @Override
            public void changed(ObservableValue<? extends String> observable, String oldValue, String newValue) {
                Platform.runLater(()-> {
                    if(newValue.equals("Connected")){
                        senderConnectionLbl.getStyleClass().remove("statusInactive");
                        senderConnectionLbl.getStyleClass().add("statusActive");
                    }else if(newValue.equals("Disconnected")){
                        senderConnectionLbl.getStyleClass().remove("statusActive");
                        senderConnectionLbl.getStyleClass().add("statusInactive");
                    }else {
                        senderConnectionLbl.getStyleClass().remove("statusActive");
                        senderConnectionLbl.getStyleClass().remove("statusInactive");
                        senderConnectionLbl.getStyleClass().add("serverDetailsText");
                    }
                });
            }
        });

    }
}
