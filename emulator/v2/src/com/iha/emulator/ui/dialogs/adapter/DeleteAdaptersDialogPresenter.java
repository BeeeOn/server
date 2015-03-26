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
 * Created by Shu on 6.12.2014.
 */
public class DeleteAdaptersDialogPresenter implements Presenter,PanelPresenter {

    private static final Logger logger = LogManager.getLogger(DeleteAdaptersDialogPresenter.class);
    private static final String FXML_PATH = "DeleteAdaptersDialog.fxml";

    private Display view;
    private Stage window;

    private DetailedSimulationPresenter detailedSimulationPresenter;
    private CheckComboBox<AdapterController> checkComboBox;

    public interface Display {
        public Node getView();
        public void setPresenter(DeleteAdaptersDialogPresenter presenter);
        public CheckBox getDatabaseCheckBox();
        public CheckBox getSaveCheckBox();
        public ProgressIndicator getIndicator();
        public Label getStatus();
        public StackPane getListContainer();
    }

    public DeleteAdaptersDialogPresenter(Stage window, DetailedSimulationPresenter detailedSimulationPresenter) {
        this.window = window;
        this.detailedSimulationPresenter = detailedSimulationPresenter;
    }

    public void delete(){
        if(checkComboBox == null ) return;
        showStatus("Preparing to delete",true);
        if(checkComboBox.getCheckModel().getCheckedIndices().size() > 0){
            logger.debug("Starting to delete adapters");
            ObservableList<AdapterController> adapterControllers = FXCollections.observableArrayList(checkComboBox.getCheckModel().getCheckedItems());
            for(AdapterController adapterController : adapterControllers){
                adapterController.disable();
                if(view.getSaveCheckBox().isSelected()){
                    showStatus("Saving adapter " + adapterController.toString(),true);
                    saveAdapter(adapterController);
                }
                if(view.getDatabaseCheckBox().isSelected()){
                    showStatus("Deleting from DB "  + adapterController.toString(),true);
                    Task dbTask = deleteFromDatabase(adapterController);
                    dbTask.setOnSucceeded(event -> {
                        showStatus("Deleting from emulator",true);
                        detailedSimulationPresenter.deleteAdapter(adapterController);
                    });
                }else {
                    showStatus("Deleting from emulator",true);
                    detailedSimulationPresenter.deleteAdapter(adapterController);
                }
            }
            close();
        }else{
            logger.debug("No adapters chosen to be deleted");
            showStatus("Please choose adapters",false);
        }

    }

    private void printAdaptersLogs(AdapterController adapterController){
        logger.trace("Printing logs for adapter: " + adapterController.toString());
    }

    private void saveAdapter(AdapterController adapterController){
        logger.trace("Saving adapter: " + adapterController.toString());
        detailedSimulationPresenter.saveCurrentAdapter(adapterController);
    }

    private Task deleteFromDatabase(AdapterController adapterController){
        logger.trace("Deleting adapter from database: " + adapterController.toString());
        Task<Object> worker = new Task<Object>() {
            @Override
            protected Object call() throws Exception {
                EmulatorServerClient server = new EmulatorServerClient(adapterController.getServerController().getModel().getIp());
                try{
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

    public void close(){
        window.hide();
    }

    public void showStatus(String status,boolean indicate){
        view.getStatus().setText(status);
        view.getIndicator().setVisible(indicate);
    }

    private void init(){
        if(detailedSimulationPresenter == null || view == null) return;
        checkComboBox = new CheckComboBox<>(detailedSimulationPresenter.getAdapterControllers());
        checkComboBox.setPrefWidth(300);
        checkComboBox.setMaxWidth(300);
        view.getListContainer().getChildren().add(checkComboBox);
        showStatus("",false);
    }

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
