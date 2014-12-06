package com.iha.emulator.ui.dialogs.adapter;

import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.panels.PanelPresenter;
import com.iha.emulator.utilities.server.EmulatorServerClient;
import com.iha.emulator.utilities.server.MessageFactory;
import javafx.application.Platform;
import javafx.concurrent.Task;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.ProgressIndicator;
import javafx.scene.control.TableView;
import javafx.stage.Stage;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.io.InputStream;

/**
 * Created by Shu on 6.12.2014.
 */
public class ShowAdaptersDialogPresenter implements Presenter,PanelPresenter {

    private static final Logger logger = LogManager.getLogger(ShowAdaptersDialogPresenter.class);
    private static final String FXML_PATH = "ShowAdaptersDialog.fxml";

    private Display view;
    private Stage window;

    private String databaseName;

    public interface Display {
        public Node getView();
        public void setPresenter(ShowAdaptersDialogPresenter presenter);
        public ProgressIndicator getIndicator();
        public Label getStatus();
        public Button getRefreshBtn();
        public Button getCloseBtn();
        public TableView getTable();
    }

    public ShowAdaptersDialogPresenter(Stage window,String databaseName) {
        this.window = window;
        this.databaseName = databaseName;
    }

    public void refresh(){
        showStatus("Fetching tables...",true);
        Task<Object> worker = new Task<Object>() {
            @Override
            protected Object call() throws Exception {
                EmulatorServerClient server = new EmulatorServerClient();
                try{
                    server.connect();
                }catch (IOException e){
                    Platform.runLater(()->showStatus("Cannot connect to server",false));
                }
                try{
                    server.sendMessage(MessageFactory.buildGetAdaptersMessage(databaseName,"id,name"));
                }catch (IOException e){
                    Platform.runLater(()->showStatus("Cannot read from socket",false));
                }

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
        showStatus("Table is ready",false);
    }

    public void showStatus(String status,boolean inidicate){
        view.getStatus().setText(status);
        view.getIndicator().setVisible(inidicate);
    }

    @Override
    public Node loadView() throws IOException {
        logger.trace("Loading ShowAdaptersDialogView from: " + FXML_PATH);
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
    public void bind() {
        view.setPresenter(this);
    }
}
