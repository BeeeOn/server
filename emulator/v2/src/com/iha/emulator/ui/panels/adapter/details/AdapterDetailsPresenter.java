package com.iha.emulator.ui.panels.adapter.details;

import com.iha.emulator.models.Adapter;
import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.panels.PanelPresenter;
import javafx.beans.binding.Bindings;
import javafx.beans.binding.StringBinding;
import javafx.beans.property.Property;
import javafx.beans.property.ReadOnlyIntegerProperty;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.control.Label;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.io.InputStream;

/**
 * Created by Shu on 26.11.2014.
 */
public class AdapterDetailsPresenter implements Presenter,PanelPresenter {

    private static final Logger logger = LogManager.getLogger(AdapterDetailsPresenter.class);
    private static final String FXML_PATH = "AdapterDetails.fxml";

    public interface Display {
        public Node getView();
        public void setPresenter(AdapterDetailsPresenter presenter);
        public Label getStatusLbl();
        public Label getNameLbl();
        public Label getIdLbl();
        public Label getRegisteredLbl();
        public Label getActiveCountLbl();
        public Label getRegisteredCountLbl();
        public Label getProtocolLbl();
        public Label getFirmwareLbl();
    }

    private Display view;
    private Adapter model;

    public AdapterDetailsPresenter() {

    }

    @Override
    public Node loadView() throws IOException {
        logger.trace("Loading AdapterDetailsView from: " + FXML_PATH);
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
    public Object getModel(){
        return model;
    }

    @Override
    public Node getView() {
        return view.getView();
    }

    @Override
    public void addModel(Object newModel) {
        logger.trace("Assigning new model to AdapterDetailsPresenter");
        //unbind existing model if exist
        if(this.model!=null){
            logger.trace("Model already exists -> unbinding labels");
            unbindLbl(view.getStatusLbl());
            unbindLbl(view.getNameLbl());
            unbindLbl(view.getIdLbl());
            unbindLbl(view.getRegisteredLbl());
            unbindLbl(view.getActiveCountLbl());
            unbindLbl(view.getRegisteredCountLbl());
            unbindLbl(view.getProtocolLbl());
            unbindLbl(view.getFirmwareLbl());
        }else{
            logger.trace("No model = no need to unbind");
        }
        this.model = (Adapter)newModel;
        logger.trace("New adapter model:");
        if (newModel != null) {
            logger.trace("      Status: " + this.model.getStatus());
            logger.trace("      Name: " + this.model.getName());
            logger.trace("      Id: " + this.model.getId());
            logger.trace("      Registered: " + this.model.getRegistered());
            logger.trace("      ActiveCount: " + this.model.getActiveCount());
            logger.trace("      RegisteredCount: " + this.model.getRegisteredCount());
            logger.trace("      Protocol: " + this.model.getProtocol().getVersion());
            logger.trace("      Firmware: " + this.model.getFirmware());
        }else{
            logger.trace("      Model is null");
        }
        //bind to GUI
        view.getStatusLbl().textProperty().bind(new StringBinding() {
            {
                bind(model.statusProperty());
            }
            @Override
            protected String computeValue() {
                if(model.statusProperty().get())
                    return "Active";
                else
                    return "Inactive";
            }
        });
        view.getNameLbl().textProperty().bind(this.model.nameProperty());
        view.getIdLbl().textProperty().bind(Bindings.convert(this.model.idProperty()));
        view.getRegisteredLbl().textProperty().bind(new StringBinding() {
            {
                bind(model.registeredProperty());
            }
            @Override
            protected String computeValue() {
                if(model.registeredProperty().get())
                    return "Yes";
                else
                    return "No";
            }
        });
        view.getActiveCountLbl().textProperty().bind(Bindings.convert(this.model.activeCountProperty()));
        view.getRegisteredCountLbl().textProperty().bind(Bindings.convert(this.model.registeredCountProperty()));
        view.getProtocolLbl().textProperty().bind(Bindings.convert(this.model.getProtocol().versionProperty()));
        view.getFirmwareLbl().textProperty().bind(Bindings.convert(this.model.firmwareProperty()));
        logger.trace("OK");
    }

    public void bindRegisteredCount(ReadOnlyIntegerProperty property){
        view.getRegisteredCountLbl().textProperty().bind(Bindings.convert(property));
    }

    private void unbindLbl(Label lbl){
        lbl.textProperty().unbind();
    }

    @Override
    public void bind() {
        view.setPresenter(this);
    }
}
