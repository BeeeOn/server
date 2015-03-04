package com.iha.emulator.ui.dialogs.adapter;

import com.iha.emulator.communication.eserver.EmulatorServerClient;
import com.iha.emulator.communication.eserver.model.AdapterInfo;
import com.iha.emulator.communication.eserver.task.ServerTask;
import com.iha.emulator.communication.eserver.task.TaskParser;
import com.iha.emulator.communication.eserver.task.implemented.GetAdaptersTask;
import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.panels.PanelPresenter;
import com.iha.emulator.utilities.Utilities;
import javafx.application.Platform;
import javafx.collections.ObservableList;
import javafx.concurrent.Task;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.control.Label;
import javafx.scene.control.ProgressIndicator;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.stage.Stage;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.dom4j.DocumentException;

import java.io.IOException;
import java.io.InputStream;

/**
 * Created by Shu on 6.12.2014.
 */
public class ShowAdaptersDialogPresenter implements Presenter,PanelPresenter {

    private static final Logger logger = LogManager.getLogger(ShowAdaptersDialogPresenter.class);
    private static final String FXML_PATH = "ShowAdaptersDialog.fxml";

    public static final String Column1MapKey = "A";
    public static final String Column2MapKey = "B";

    private Display view;
    private Stage window;

    private String databaseName;

    public interface Display {
        public Node getView();
        public void setPresenter(ShowAdaptersDialogPresenter presenter);
        public ProgressIndicator getIndicator();
        public Label getStatus();
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
                    //composite message for server
                    ServerTask task = new GetAdaptersTask(databaseName,"adapter_id,name");
                    //send message and wait for response
                    String messageFromServer = server.sendMessage(task.buildMessage());
                    //determine result state (OK/ERROR)
                    TaskParser.parseTaskResult(messageFromServer);
                    //if ok, parse response
                    task.parseResponse(messageFromServer);
                    //show response in table
                    Platform.runLater(()->populateTable(((GetAdaptersTask)task).getResult()));
                }catch (IOException e){
                    Platform.runLater(()-> Utilities.showException(logger, "Cannot read from socket", e, false, null));
                }catch (DocumentException de){
                    Platform.runLater(()-> Utilities.showException(logger,"Cannot parse server message",de,false,null));
                }catch (IllegalStateException ie){
                    Platform.runLater(()-> Utilities.showException(logger,"Error on server",ie,false,null));
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
        window.hide();
    }

    public void showStatus(String status,boolean indicate){
        view.getStatus().setText(status);
        view.getIndicator().setVisible(indicate);
    }

    @SuppressWarnings("unchecked")
    public void populateTable(ObservableList<AdapterInfo> tableData) {
        logger.trace("Populating table with data from server");
        TableView table = view.getTable();
        TableColumn<AdapterInfo,String> idColumn= (TableColumn<AdapterInfo, String>) table.getColumns().get(0);
        idColumn.setCellValueFactory( new PropertyValueFactory<>("id"));
        TableColumn<AdapterInfo,String> nameColumn= (TableColumn<AdapterInfo, String>) table.getColumns().get(1);
        nameColumn.setCellValueFactory( new PropertyValueFactory<>("name"));
        table.setItems(tableData);
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
    public void clear() {

    }

    @Override
    public void bind() {
        view.setPresenter(this);
    }
}
