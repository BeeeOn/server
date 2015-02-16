package com.iha.emulator.ui.dialogs.log;

import com.iha.emulator.models.Adapter;
import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.panels.PanelPresenter;
import javafx.application.Platform;
import javafx.concurrent.Task;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.control.Label;
import javafx.scene.control.ProgressIndicator;
import javafx.stage.Stage;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.io.InputStream;

/**
 * Created by Shu on 3.2.2015.
 */
public class ShowFullLogPresenter implements Presenter,PanelPresenter{

    private static final Logger logger = LogManager.getLogger(ShowFullLogPresenter.class);
    private static final String FXML_PATH = "ShowFullLog.fxml";

    private Display view;
    private Stage window;

    private Adapter adapter;

    public interface Display {
        public Node getView();
        public void setPresenter(ShowFullLogPresenter presenter);
        public ProgressIndicator getIndicator();
        public Label getStatus();
    }

    public ShowFullLogPresenter(Stage window,Adapter adapter) {
        this.window = window;
        this.adapter = adapter;
    }

    public void refresh(){
        showStatus("Fetching tables...",true);
        Task<Object> worker = new Task<Object>() {
            @Override
            protected Object call() throws Exception {

                Platform.runLater(()->showStatus("Table is ready",false));
                return null;
            }
        };
        //create thread for background process
        Thread th = new Thread(worker);
        th.setDaemon(true);
        //run background process
        th.start();
    }

    public void close(){
        window.hide();
    }

    public void showStatus(String status,boolean indicate){
        view.getStatus().setText(status);
        view.getIndicator().setVisible(indicate);
    }

    @Override
    public Node loadView() throws IOException {
        logger.trace("Loading ShowFullLogView from: " + FXML_PATH);
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
            logger.trace("OK");
            return view.getView();
        }
        finally
        {
            if (fxmlStream != null) fxmlStream.close();
        }
    }

    @Override
    public void addModel(Object model) {

    }

    @Override
    public Object getModel() {
        return null;
    }

    @Override
    public Node getView() {
        return view.getView();
    }

    @Override
    public void clear() {

    }

    @Override
    public void bind() {
        view.setPresenter(this);
    }
}
