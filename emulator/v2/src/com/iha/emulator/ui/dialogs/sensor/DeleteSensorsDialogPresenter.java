package com.iha.emulator.ui.dialogs.sensor;

import com.iha.emulator.communication.eserver.EmulatorServerClient;
import com.iha.emulator.communication.eserver.task.ServerTask;
import com.iha.emulator.communication.eserver.task.TaskParser;
import com.iha.emulator.communication.eserver.task.implemented.DeleteAdapterTask;
import com.iha.emulator.communication.eserver.task.implemented.DeleteSensorsTask;
import com.iha.emulator.control.AdapterController;
import com.iha.emulator.control.SensorController;
import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.panels.PanelPresenter;
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
import java.util.ArrayList;

/**
 * Class providing logic to user interactions for "Delete sensor dialog". Part Presenter of MVP design pattern.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class DeleteSensorsDialogPresenter implements Presenter,PanelPresenter {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(DeleteSensorsDialogPresenter.class);
    /** path to FXML file */
    private static final String FXML_PATH = "DeleteSensorsDialog.fxml";
    /** view */
    private Display view;
    /** window */
    private Stage window;
    /** adapter containing sensors */
    private AdapterController adapterController;
    /** combo box used to choose sensors to be deleted */
    private CheckComboBox<SensorController> checkComboBox;
    /**
     * Interface implemented by "Delete sensor dialog" view.
     */
    public interface Display {
        public Node getView();
        public void setPresenter(DeleteSensorsDialogPresenter presenter);
        public CheckBox getDatabaseCheckBox();
        public ProgressIndicator getIndicator();
        public Label getStatus();
        public StackPane getListContainer();
    }

    /**
     * Creates "Delete sensor dialog" presenter.
     * @param window parent window
     * @param adapterController adapter with sensors
     */
    public DeleteSensorsDialogPresenter(Stage window, AdapterController adapterController) {
        this.window = window;
        this.adapterController = adapterController;
    }
    /**
     * All chosen sensors are disabled and deleted from emulator. Also depending on user chosen checkbox all sensors
     * are deleted from database.
     */
    public void delete(){
        if(checkComboBox == null ) return;
        //temporary save adapter's status
        boolean adapterStatus = adapterController.getAdapter().getStatus();
        showStatus("Preparing to delete", true);
        //if any sensor was chosen
        if(checkComboBox.getCheckModel().getCheckedIndices().size() > 0){
            //disable adapter
            adapterController.disable();
            logger.debug("Starting to delete sensors");
            ObservableList<SensorController> sensorControllers = FXCollections.observableArrayList(checkComboBox.getCheckModel().getCheckedItems());
            //if should delete from database, delete
            if(view.getDatabaseCheckBox().isSelected()){
                showStatus("Deleting from DB ",true);
                deleteFromDatabase(sensorControllers);
            }
            //notify user
            showStatus("Deleting from emulator",true);
            //delete sensors from emulator
            adapterController.deleteSensors(sensorControllers);
            sensorControllers.clear();
            //return adapter to its previous status state
            adapterController.getAdapter().setStatus(adapterStatus);
            adapterController.setSaved(false);
            close();
        }else{
            logger.debug("No sensors chosen to be deleted");
            showStatus("Please choose sensors",false);
        }
    }
    /**
     * Delete given list of sensors from database. Uses emulator server. Creates separate {@link javafx.concurrent.Task}
     * @param sensorControllers list of sensors to be deleted
     * @return task responsible for deleting sensors
     */
    private Thread deleteFromDatabase(ObservableList<SensorController> sensorControllers){
        ArrayList<Integer> ids = new ArrayList<>();
        sensorControllers.forEach(s->ids.add(s.getModel().getId()));
        logger.trace("Deleting sensors from database");
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
                    //compose message for server
                    ServerTask task = new DeleteSensorsTask(adapterController.getServerController().getModel().getDatabaseName(),ids);
                    //send message and wait for response
                    logger.trace("Creating and sending message");
                    String messageFromServer = server.sendMessage(task.buildMessage());
                    //determine result state (OK/ERROR)
                    TaskParser.parseTaskResult(messageFromServer);
                    //if ok, parse response
                    task.parseResponse(messageFromServer);
                    //show response in table
                    if(!((DeleteAdapterTask)task).getResult()){
                        Platform.runLater(()->{
                            showStatus("Error in database delete",false);
                            Utilities.showException(logger,"Error in database delete",null,false,null);
                        });
                    }
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
        return th;
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
        if(adapterController == null || view == null) return;
        checkComboBox = new CheckComboBox<>(adapterController.getSensorControllers());
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
        logger.trace("Loading DeleteSensorsDialogView from: " + FXML_PATH);
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
