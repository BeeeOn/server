package com.iha.emulator.ui.dialogs.adapter;

import com.iha.emulator.communication.eserver.EmulatorServerClient;
import com.iha.emulator.communication.eserver.task.ServerTask;
import com.iha.emulator.communication.eserver.task.TaskParser;
import com.iha.emulator.communication.eserver.task.implemented.DeleteAdapterTask;
import com.iha.emulator.control.AdapterController;
import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.panels.PanelPresenter;
import com.iha.emulator.ui.simulations.detailed.DetailedSimulationPresenter;
import com.iha.emulator.utilities.Utilities;
import javafx.application.Platform;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.concurrent.Task;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.control.CheckBox;
import javafx.scene.control.Label;
import javafx.scene.control.ProgressIndicator;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.controlsfx.control.CheckComboBox;
import org.dom4j.DocumentException;

import java.io.IOException;
import java.io.InputStream;

/**
 * Class providing logic to user interactions for "Delete adapter dialog". Part Presenter of MVP design pattern.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class DeleteAdaptersDialogPresenter implements Presenter,PanelPresenter {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(DeleteAdaptersDialogPresenter.class);
    /** path to FXML file */
    private static final String FXML_PATH = "DeleteAdaptersDialog.fxml";
    /** view */
    private Display view;
    /** window */
    private Stage window;
    /** parent presenter */
    private DetailedSimulationPresenter detailedSimulationPresenter;
    /** combo box used to choose adapters to be deleted */
    private CheckComboBox<AdapterController> checkComboBox;
    /**
     * Interface implemented by "Delete adapter dialog" view.
     */
    public interface Display {
        public Node getView();
        public void setPresenter(DeleteAdaptersDialogPresenter presenter);
        public CheckBox getDatabaseCheckBox();
        public CheckBox getSaveCheckBox();
        public ProgressIndicator getIndicator();
        public Label getStatus();
        public StackPane getListContainer();
    }

    /**
     * Creates "Delete adapter dialog" presenter.
     * @param window parent window
     * @param detailedSimulationPresenter parent presenter providing methods to delete adapter
     */
    public DeleteAdaptersDialogPresenter(Stage window, DetailedSimulationPresenter detailedSimulationPresenter) {
        this.window = window;
        this.detailedSimulationPresenter = detailedSimulationPresenter;
    }

    /**
     * All chosen adapters are disabled and deleted from emulator. Also depending on user chosen checkboxes all adapters
     * are saved to XML file and/or deleted from database.
     */
    public void delete(){
        if(checkComboBox == null ) return;
        showStatus("Preparing to delete",true);
        //if any adapter was chosen
        if(checkComboBox.getCheckModel().getCheckedIndices().size() > 0){
            logger.debug("Starting to delete adapters");
            ObservableList<AdapterController> adapterControllers = FXCollections.observableArrayList(checkComboBox.getCheckModel().getCheckedItems());
            //for every chosen adapter
            for(AdapterController adapterController : adapterControllers){
                //disable adapter
                adapterController.disable();
                //if should save to XML, save
                if(view.getSaveCheckBox().isSelected()){
                    showStatus("Saving adapter " + adapterController.toString(),true);
                    saveAdapter(adapterController);
                }
                //if should delete from database, delete
                if(view.getDatabaseCheckBox().isSelected()){
                    showStatus("Deleting from DB "  + adapterController.toString(),true);
                    Task dbTask = deleteFromDatabase(adapterController);
                    //if delete adapters task succeeded, remove from emulator
                    dbTask.setOnSucceeded(event -> {
                        //notify user
                        showStatus("Deleting from emulator",true);
                        //delete from emulator
                        detailedSimulationPresenter.deleteAdapter(adapterController);
                    });
                }else {
                    //notify user
                    showStatus("Deleting from emulator",true);
                    //delete from emulator
                    detailedSimulationPresenter.deleteAdapter(adapterController);
                }
            }
            close();
        }else{
            logger.debug("No adapters chosen to be deleted");
            //notify user to choose adapter/s to delete
            showStatus("Please choose adapters",false);
        }

    }

    /**
     * Invokes save dialog for given adapter.
     * @param adapterController adapter to be saved to XML
     */
    private void saveAdapter(AdapterController adapterController){
        logger.trace("Saving adapter: " + adapterController.toString());
        detailedSimulationPresenter.saveCurrentAdapter(adapterController);
    }

    /**
     * Delete given adapter from database. Uses emulator server. Creates separate {@link javafx.concurrent.Task}
     * @param adapterController adapter to be deleted
     * @return task responsible for deleting adapter
     */
    private Task deleteFromDatabase(AdapterController adapterController){
        logger.trace("Deleting adapter from database: " + adapterController.toString());
        Task<Object> worker = new Task<Object>() {
            @Override
            protected Object call() throws Exception {
                //create emulator server client
                EmulatorServerClient server = new EmulatorServerClient(adapterController.getServerController().getModel().getIp());
                try{
                    //connect to emulator server
                    server.connect();
                }catch (IOException e){
                    Platform.runLater(()->showStatus("Cannot connect to server",false));
                }
                try{
                    //composite message for server
                    ServerTask task = new DeleteAdapterTask(adapterController.getServerController().getModel().getDatabaseName(),String.valueOf(adapterController.getAdapter().getId()));
                    //send message and wait for response
                    String messageFromServer = server.sendMessage(task.buildMessage());
                    //determine result state (OK/ERROR)
                    TaskParser.parseTaskResult(messageFromServer);
                    //if ok, parse response
                    task.parseResponse(messageFromServer);
                    //show response in table
                    String status;
                    if(((DeleteAdapterTask)task).getResult()){
                        status = "Database delete OK";
                    }else{
                        status = "Error in database delete";
                    }
                    Platform.runLater(()->showStatus(status,false));
                }catch (IOException e){
                    Platform.runLater(()-> Utilities.showException(logger, "Cannot read from socket", e, false, null));
                }catch (DocumentException de){
                    Platform.runLater(()-> Utilities.showException(logger,"Cannot parse server message",de,false,null));
                }catch (IllegalStateException ie){
                    Platform.runLater(()-> Utilities.showException(logger,"Error on server",ie,false,null));
                }
                return null;
            }
        };
        //create thread for background process
        Thread th = new Thread(worker);
        th.setDaemon(true);
        //run background process
        th.start();
        return worker;
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
     * Initializes dialog. Fills components with data and sets validation options.
     */
    private void init(){
        if(detailedSimulationPresenter == null || view == null) return;
        checkComboBox = new CheckComboBox<>(detailedSimulationPresenter.getAdapterControllers());
        checkComboBox.setPrefWidth(300);
        checkComboBox.setMaxWidth(300);
        view.getListContainer().getChildren().add(checkComboBox);
        showStatus("",false);
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public Node loadView() throws IOException {
        logger.trace("Loading DeleteAdaptersDialogView from: " + FXML_PATH);
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
            init();
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
