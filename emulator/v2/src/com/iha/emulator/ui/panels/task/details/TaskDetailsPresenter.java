package com.iha.emulator.ui.panels.task.details;

import com.iha.emulator.control.task.SimulationTask;
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
 * Class providing logic to user interactions for "Task details panel". Part Presenter of MVP design pattern.
 * Also responsible for model change.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class TaskDetailsPresenter implements Presenter,PanelPresenter {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(TaskDetailsPresenter.class);
    /** path to FXML file */
    private static final String FXML_PATH = "TaskDetails.fxml";
    /** view */
    private Display view;
    /** model */
    private SimulationTask model;
    /**
     * Interface implemented by "Task details panel" view.
     */
    public interface Display {
        public Node getView();
        public void setPresenter(TaskDetailsPresenter presenter);
        public Label getIdLbl();
        public Label getStateLbl();
        public Label getDevicesCountLbl();
        public Label getAdaptersCountLbl();
        public Label getWaitingMessagesLbl();
        public Label getSentMessagesLbl();
        public Label getStopWatchLbl();
        public Label getRespPerSecondLbl();
        public Label getMaxRespPerSecondLbl();
    }
    /**
     * Unbinds model, if it exists and clears all panel's fields.
     */
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
            unbindLbl(view.getStopWatchLbl());
            unbindLbl(view.getRespPerSecondLbl());
            unbindLbl(view.getMaxRespPerSecondLbl());
        }
        this.model = null;
        view.getIdLbl().setText("");
        view.getStateLbl().setText("");
        view.getDevicesCountLbl().setText("");
        view.getAdaptersCountLbl().setText("");
        view.getWaitingMessagesLbl().setText("");
        view.getSentMessagesLbl().setText("");
        view.getStopWatchLbl().setText("");
        view.getRespPerSecondLbl().setText("");
        view.getMaxRespPerSecondLbl().setText("");
    }
    /**
     * {@inheritDoc}
     */
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
    /**
     * Assign model to panel and binds it's variables to panel fields. If panel has already assigned model, it is unbound.
     * @param newModel model to be bound with panel's fields
     */
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
            unbindLbl(view.getStopWatchLbl());
            unbindLbl(view.getRespPerSecondLbl());
            unbindLbl(view.getMaxRespPerSecondLbl());
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
        view.getRespPerSecondLbl().textProperty().bind(new StringBinding() {
            {
                bind(model.getResponseTracker().currentPerSecondProperty());
            }
            @Override
            protected String computeValue() {
                return String.valueOf(model.getResponseTracker().getCurrentPerSecond());
            }
        });
        view.getMaxRespPerSecondLbl().textProperty().bind(new StringBinding() {
            {
                bind(model.getResponseTracker().maxPerSecondProperty());
            }
            @Override
            protected String computeValue() {
                return String.valueOf(model.getResponseTracker().getMaxPerSecond());
            }
        });
        view.getStopWatchLbl().textProperty().bind(model.getTaskParameters().getStopWatch().timeStringProperty());
        logger.trace("OK");
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
    /**
     * Unbinds given label text property
     * @param lbl label for which should be text property unbound
     */
    private void unbindLbl(Label lbl){
        lbl.textProperty().unbind();
    }
}
