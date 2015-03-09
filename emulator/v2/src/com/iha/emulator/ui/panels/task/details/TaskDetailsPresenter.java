package com.iha.emulator.ui.panels.task.details;

import com.iha.emulator.control.SimulationTask;
import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.panels.PanelPresenter;
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
public class TaskDetailsPresenter implements Presenter,PanelPresenter {

    private static final Logger logger = LogManager.getLogger(TaskDetailsPresenter.class);
    private static final String FXML_PATH = "TaskDetails.fxml";

    private Display view;
    private SimulationTask model;

    public interface Display {
        public Node getView();
        public void setPresenter(TaskDetailsPresenter presenter);
        public Label getIdLbl();
        public Label getStateLbl();
        public Label getDevicesCountLbl();
        public Label getAdaptersCountLbl();
        public Label getWaitingMessagesLbl();
        public Label getSentMessagesLbl();
    }

    public TaskDetailsPresenter() {

    }

    @Override
    public void clear(){
        logger.trace("CLEAR!");
        if(this.model != null) {
            logger.trace("Unbinding labels");
            unbindLbl(view.getIdLbl());
            unbindLbl(view.getStateLbl());
            unbindLbl(view.getDevicesCountLbl());
            unbindLbl(view.getAdaptersCountLbl());
            unbindLbl(view.getWaitingMessagesLbl());
            unbindLbl(view.getSentMessagesLbl());
        }
        this.model = null;
        view.getIdLbl();
        view.getStateLbl();
        view.getDevicesCountLbl();
        view.getAdaptersCountLbl();
        view.getWaitingMessagesLbl();
        view.getSentMessagesLbl();
    }

    @Override
    public Node loadView() throws IOException {
        logger.trace("Loading TaskDetailsView from: " + FXML_PATH);
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
        logger.trace("Assigning new model to TaskDetailsPresenter");
        //unbind existing model if exist
        if(this.model!=null){
            logger.trace("Model already exists -> unbinding labels");
            unbindLbl(view.getIdLbl());
            unbindLbl(view.getStateLbl());
            unbindLbl(view.getDevicesCountLbl());
            unbindLbl(view.getAdaptersCountLbl());
            unbindLbl(view.getWaitingMessagesLbl());
            unbindLbl(view.getSentMessagesLbl());
        }else{
            logger.trace("No model = no need to unbind");
        }
        this.model = (SimulationTask)newModel;
        //bind to GUI
        view.getIdLbl().textProperty().bind(new StringBinding() {
            {
                bind(model.idProperty());
            }
            @Override
            protected String computeValue() {
                return String.valueOf(model.getId());
            }
        });
        view.getStateLbl().textProperty().bind(new StringBinding() {
            {
                bind(model.simulationStateProperty());
            }
            @Override
            protected String computeValue() {
                return model.getSimulationState().getName();
            }
        });
        view.getDevicesCountLbl().textProperty().bind(new StringBinding() {
            {
                bind(model.getTaskParameters().sensorsCountProperty());
            }
            @Override
            protected String computeValue() {
                return String.valueOf(model.getTaskParameters().getSensorsCount());
            }
        });
        view.getAdaptersCountLbl().textProperty().bind(new StringBinding() {
            {
                bind(model.getTaskParameters().adaptersCountProperty());
            }
            @Override
            protected String computeValue() {
                return String.valueOf(model.getTaskParameters().getAdaptersCount());
            }
        });
        view.getWaitingMessagesLbl().textProperty().bind(new StringBinding() {
            {
                bind(model.getLog().getMessageTracker().waitingMessageCounterProperty());
            }
            @Override
            protected String computeValue() {
                return String.valueOf(model.getLog().getMessageTracker().getWaitingMessageCounter());
            }
        });
        view.getSentMessagesLbl().textProperty().bind(new StringBinding() {
            {
                bind(model.getLog().getMessageTracker().sentMessageCounterProperty());
            }
            @Override
            protected String computeValue() {
                return String.valueOf(model.getLog().getMessageTracker().getSentMessageCounter());
            }
        });
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

    private void unbindLbl(Label lbl){
        lbl.textProperty().unbind();
    }
}
