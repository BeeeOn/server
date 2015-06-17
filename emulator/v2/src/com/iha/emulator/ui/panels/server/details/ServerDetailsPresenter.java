package com.iha.emulator.ui.panels.server.details;

import com.iha.emulator.models.Server;
import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.panels.PanelPresenter;
import javafx.beans.binding.Bindings;
import javafx.beans.binding.StringBinding;
import javafx.beans.property.BooleanProperty;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.control.Label;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.io.InputStream;

/**
 * Class providing logic to user interactions for "Server details panel". Part Presenter of MVP design pattern.
 * Also responsible for model change.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class ServerDetailsPresenter implements Presenter,PanelPresenter {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(ServerDetailsPresenter.class);
    /** path to FXML file */
    private static final String FXML_PATH = "ServerDetails.fxml";
    /** view */
    private Display view;
    /** model */
    private Server model;
    private BooleanProperty senderConnectionProp;
    /**
     * Interface implemented by "Server details panel" view.
     */
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
        public Label getSenderConnectionLbl();
    }
    /**
     * Unbinds model, if it exists and clears all panel's fields.
     */
    @Override
    public void clear(){
        logger.trace("CLEAR!");
        if(this.model != null) {
            logger.trace("Unbinding labels");
            view.getConnectionLbl().textProperty().unbind();
            view.getSenderConnectionLbl().textProperty().unbind();
            view.getNameLbl().textProperty().unbind();
            view.getIpLbl().textProperty().unbind();
            view.getPortLbl().textProperty().unbind();
            view.getDbNameLbl().textProperty().unbind();
        }
        this.model = null;
        view.getConnectionLbl().setText("");
        view.getSenderConnectionLbl().setText("");
        view.getNameLbl().setText("");
        view.getIpLbl().setText("");
        view.getPortLbl().setText("");
        view.getDbNameLbl().setText("");
    }
    /**
     * {@inheritDoc}
     */
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
    /**
     * Assign model to panel and binds it's variables to panel fields. If panel has already assigned model, it is unbound.
     * @param newModel model to be bound with panel's fields
     */
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

    /**
     * Bind {@link com.iha.emulator.communication.server.ssl.ServerReceiver}'s status property to panel's field.
     * If ServerReceiver doesn't exist, "Disabled" is shown.
     * @param senderConnection {@link com.iha.emulator.communication.server.ssl.ServerReceiver}'s status property
     */
    public void addSenderProperty(BooleanProperty senderConnection){
        if(senderConnection == null) {
            view.getSenderConnectionLbl().setText("Disabled");
            return;
        }
        if(senderConnection != null){
            view.getSenderConnectionLbl().textProperty().unbind();
        }
        this.senderConnectionProp = senderConnection;
        view.getSenderConnectionLbl().textProperty().bind(new StringBinding() {
            {
                bind(senderConnectionProp);
            }
            @Override
            protected String computeValue() {
                if(senderConnectionProp == null ) return "Disabled";
                if(senderConnectionProp.get())
                    return "Connected";
                else
                    return "Disconnected";
            }
        });
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public Object getModel() {
        return model;
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public Node getView() {
        return view.getView();
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public void bind() {
        view.setPresenter(this);
    }
}
