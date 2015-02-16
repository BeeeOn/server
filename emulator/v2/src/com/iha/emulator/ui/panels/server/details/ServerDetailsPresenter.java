package com.iha.emulator.ui.panels.server.details;

import com.iha.emulator.models.Server;
import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.panels.PanelPresenter;
import javafx.beans.binding.Bindings;
import javafx.beans.binding.StringBinding;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.control.Label;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.io.InputStream;

/**
 * Created by Shu on 25.11.2014.
 */
public class ServerDetailsPresenter implements Presenter,PanelPresenter {

    private static final Logger logger = LogManager.getLogger(ServerDetailsPresenter.class);
    private static final String FXML_PATH = "ServerDetails.fxml";

    private Display view;
    private Server model;

    public interface Display {
        public Node getView();
        public void setPresenter(ServerDetailsPresenter presenter);
        public void setConnection(boolean b);
        public void setIp(String ip);
        public void setPort(String port);
        public Label getConnectionLbl();
        public Label getNameLbl();
        public Label getIpLbl();
        public Label getPortLbl();
        public Label getDbNameLbl();
    }

    public ServerDetailsPresenter() {

    }

    @Override
    public void clear(){
        logger.trace("CLEAR!");
        if(this.model != null) {
            logger.trace("Unbinding labels");
            view.getConnectionLbl().textProperty().unbind();
            view.getNameLbl().textProperty().unbind();
            view.getIpLbl().textProperty().unbind();
            view.getPortLbl().textProperty().unbind();
            view.getDbNameLbl().textProperty().unbind();
        }
        this.model = null;
        view.getConnectionLbl().setText("");
        view.getNameLbl().setText("");
        view.getIpLbl().setText("");
        view.getPortLbl().setText("");
        view.getDbNameLbl().setText("");
    }

    @Override
    public Node loadView() throws IOException {
        logger.trace("Loading ServerDetailsView from: " + FXML_PATH);
        InputStream fxmlStream = null;
        try
        {
            //load resource FXML
            fxmlStream = getClass().getResourceAsStream(FXML_PATH);
            FXMLLoader loader = new FXMLLoader();
            loader.load(fxmlStream);
            //remember view
            view = loader.getController();
            // bind view and presenter
            bind();
            // set css for view
            logger.trace("OK");
            return view.getView();
        }
        finally
        {
            if (fxmlStream != null) fxmlStream.close();
        }
    }

    @Override
    public void addModel(Object newModel){
        logger.trace("Assigning new model to ServerDetailsPresenter");
        //unbind existing model if exist
        if(view.getConnectionLbl().textProperty().isBound()){
            logger.trace("Connection label is bound-> unbind()");
            view.getConnectionLbl().textProperty().unbind();
        }
        if(view.getNameLbl().textProperty().isBound()){
            logger.trace("Name label is bound-> unbind()");
            view.getNameLbl().textProperty().unbind();
        }
        if(view.getIpLbl().textProperty().isBound()) {
            logger.trace("ip label is bound-> unbind()");
            view.getIpLbl().textProperty().unbind();
        }
        if(view.getPortLbl().textProperty().isBound()){
            logger.trace("port label is bound-> unbind()");
            view.getPortLbl().textProperty().unbind();
        }
        if(view.getDbNameLbl().textProperty().isBound()){
            logger.trace("db name label is bound-> unbind()");
            view.getDbNameLbl().textProperty().unbind();
        }
        //set new model
        this.model = (Server)newModel;
        logger.trace("New server model:");{
            if (newModel != null) {
                logger.trace("      Connection: " + this.model.getConn());
                logger.trace("      Name: " + this.model.getName());
                logger.trace("      Ip: " + this.model.getIp());
                logger.trace("      Port: " + this.model.getPort());
                logger.trace("      DB name: " + this.model.getDatabaseName());
            }else{
                logger.trace("      Model is null");
            }
        }
        //bind to GUI
        view.getConnectionLbl().textProperty().bind(new StringBinding() {
            {
                bind(model.connProperty());
            }
            @Override
            protected String computeValue() {
                if(model.connProperty().get())
                    return "Connected";
                else
                    return "Disconnected";
            }
        });
        view.getNameLbl().textProperty().bind(this.model.nameProperty());
        view.getIpLbl().textProperty().bind(this.model.ipProperty());
        view.getPortLbl().textProperty().bind(Bindings.convert(this.model.portProperty()));
        view.getDbNameLbl().textProperty().bind(this.model.databaseNameProperty());
        logger.trace("OK");
    }

    @Override
    public Object getModel() {
        return model;
    }

    @Override
    public Node getView() {
        return view.getView();
    }

    @Override
    public void bind() {
        view.setPresenter(this);
    }
}
