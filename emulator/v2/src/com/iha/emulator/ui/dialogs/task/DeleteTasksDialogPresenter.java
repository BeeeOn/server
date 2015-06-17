package com.iha.emulator.ui.dialogs.task;

import com.iha.emulator.control.task.SimulationTask;
import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.panels.PanelPresenter;
import com.iha.emulator.ui.simulations.performance.PerformanceSimulationPresenter;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
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

import java.io.IOException;
import java.io.InputStream;

/**
 * Class providing logic to user interactions for "Delete simulation tasks dialog". Part Presenter of MVP design pattern.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class DeleteTasksDialogPresenter implements Presenter,PanelPresenter {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(DeleteTasksDialogPresenter.class);
    /** path to FXML file */
    private static final String FXML_PATH = "DeleteTasksDialog.fxml";
    /** view */
    private Display view;
    /** window */
    private Stage window;
    /** parent presenter */
    private PerformanceSimulationPresenter performanceSimulationPresenter;
    /** combo box with task choice */
    private CheckComboBox<SimulationTask> checkComboBox;
    /**
     * Interface implemented by "Delete simulation tasks dialog" view.
     */
    public interface Display {
        public Node getView();
        public void setPresenter(DeleteTasksDialogPresenter presenter);
        public CheckBox getSaveCheckBox();
        public ProgressIndicator getIndicator();
        public Label getStatus();
        public StackPane getListContainer();
    }

    /**
     * Creates new "Delete simulation tasks dialog" presenter.
     * @param window parent window
     * @param performanceSimulationPresenter parent presenter holding tasks
     */
    public DeleteTasksDialogPresenter(Stage window, PerformanceSimulationPresenter performanceSimulationPresenter) {
        this.window = window;
        this.performanceSimulationPresenter = performanceSimulationPresenter;
    }

    /**
     * All chosen tasks deleted from emulator. Also depending on user chosen checkboxes all tasks
     * are saved to XML file. Afterwards closes window.
     */
    public void delete(){
        if(checkComboBox == null ) return;
        //notify user
        showStatus("Preparing to delete",true);
        //if any task was chosen
        if(checkComboBox.getCheckModel().getCheckedIndices().size() > 0){
            logger.debug("Starting to delete tasks");
            ObservableList<SimulationTask> tasks = FXCollections.observableArrayList(checkComboBox.getCheckModel().getCheckedItems());
            //if should save to XML, save
            if(view.getSaveCheckBox().isSelected()){
                showStatus("Saving chosen tasks",true);
                saveTasks(tasks);
            }
            //for every chosen task
            for(SimulationTask task : tasks){
                //delete task from emulator
                performanceSimulationPresenter.deleteTask(task);
                //notify user
                showStatus("Deleting task " + task.toString(), true);
            }
            System.gc();
            //close window
            close();
        }else{
            logger.debug("No tasks chosen to be deleted");
            //notify user to choose task/s to delete
            showStatus("Please choose tasks",false);
        }

    }

    /**
     * Invokes save to XML dialog for given list of tasks
     * @param tasks list of tasks to be saved
     */
    private void saveTasks(ObservableList<SimulationTask> tasks){
        logger.trace("Saving tasks");
        performanceSimulationPresenter.saveAll(tasks);
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
        if(performanceSimulationPresenter == null || view == null) return;
        checkComboBox = new CheckComboBox<>(performanceSimulationPresenter.getTasks());
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
        logger.trace("Loading DeleteTasksDialogView from: " + FXML_PATH);
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
