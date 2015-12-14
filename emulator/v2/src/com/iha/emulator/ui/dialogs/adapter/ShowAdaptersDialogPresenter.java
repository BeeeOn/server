package com.iha.emulator.ui.dialogs.adapter;

import com.iha.emulator.communication.eserver.EmulatorServerClient;
import com.iha.emulator.communication.eserver.model.AdapterInfo;
import com.iha.emulator.communication.eserver.task.ServerTask;
import com.iha.emulator.communication.eserver.task.TaskParser;
import com.iha.emulator.communication.eserver.task.implemented.GetAdaptersTask;
import com.iha.emulator.models.Server;
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
 * Class providing logic to user interactions for "Show adapters in database dialog". Part Presenter of MVP design pattern.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class ShowAdaptersDialogPresenter implements Presenter,PanelPresenter {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(ShowAdaptersDialogPresenter.class);
    /** path to FXML file */
    private static final String FXML_PATH = "ShowAdaptersDialog.fxml";
    /** view */
    private Display view;
    /** window */
    private Stage window;
    /** server model with information about server */
    private Server server;
    /**
     * Interface implemented by "Show adapters in database dialog" view.
     */
    public interface Display {
        public Node getView();
        public void setPresenter(ShowAdaptersDialogPresenter presenter);
        public ProgressIndicator getIndicator();
        public Label getStatus();
        public TableView getTable();
    }

    /**
     * Creates "Show adapters in database dialog" presenter. Dialog used to show existing adapter's in database.
     * @param window parent window
     * @param server server model
     */
    public ShowAdaptersDialogPresenter(Stage window,Server server) {
        this.window = window;
        this.server = server;
    }

    /**
     * Get all adapters from database. Uses emulator server. Creates separate {@link javafx.concurrent.Task}
     */
    public void refresh(){
        showStatus("Fetching tables...",true);
        final Server server1 = server;
        Task<Object> worker = new Task<Object>() {
            @Override
            protected Object call() throws Exception {
                //create emulator server client
                EmulatorServerClient server = new EmulatorServerClient(server1.getIp());
                try{
                    //connect to emulator server
                    server.connect();
                }catch (IOException e){
                    Platform.runLater(()->showStatus("Cannot connect to server",false));
                }
                try{
                    //composite message for server
                    ServerTask task = new GetAdaptersTask(server1.getDatabaseName(),"adapter_id,name");
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
    /**
     * Closes dialog
     */
    public void close(){
        window.hide();
    }
    /**
     * Shows given message in dialog. Also can start or stop loop indicator
     * @param status message to be shown
     * @param indicate <code>true</code> show indicator, <code>false</code> hide indicator
     */
    public void showStatus(String status,boolean indicate){
        view.getStatus().setText(status);
        view.getIndicator().setVisible(indicate);
    }

    /**
     * Populates table with adapters' information given as parameter.
     * @param tableData list of adapters' information
     */
    @SuppressWarnings("unchecked")
    public void populateTable(ObservableList<AdapterInfo> tableData) {
        logger.trace("Populating table with data from server");
        TableView table = view.getTable();
        //set ID column
        TableColumn<AdapterInfo,String> idColumn= (TableColumn<AdapterInfo, String>) table.getColumns().get(0);
        idColumn.setCellValueFactory( new PropertyValueFactory<>("id"));
        //set name column
        TableColumn<AdapterInfo,String> nameColumn= (TableColumn<AdapterInfo, String>) table.getColumns().get(1);
        nameColumn.setCellValueFactory(new PropertyValueFactory<>("name"));
        //show data
        table.setItems(tableData);
    }
    /**
     * {@inheritDoc}
     */
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

    /**
     * {@inheritDoc}
     *
     * Empty
     */
    @Override
    public void addModel(Object model) {

    }

    /**
     * {@inheritDoc}
     *
     * Empty
     * @return null
     */
    @Override
    public Object getModel() {
        return null;
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
     *
     * Empty
     */
    @Override
    public void clear() {

    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void bind() {
        view.setPresenter(this);
    }
}
