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
 * Created by Shu on 6.12.2014.
 */
public class DeleteTasksDialogPresenter implements Presenter,PanelPresenter {

    private static final Logger logger = LogManager.getLogger(DeleteTasksDialogPresenter.class);
    private static final String FXML_PATH = "DeleteTasksDialog.fxml";

    private Display view;
    private Stage window;

    private PerformanceSimulationPresenter performanceSimulationPresenter;
    private CheckComboBox<SimulationTask> checkComboBox;

    public interface Display {
        public Node getView();
        public void setPresenter(DeleteTasksDialogPresenter presenter);
        public CheckBox getSaveCheckBox();
        public ProgressIndicator getIndicator();
        public Label getStatus();
        public StackPane getListContainer();
    }

    public DeleteTasksDialogPresenter(Stage window, PerformanceSimulationPresenter performanceSimulationPresenter) {
        this.window = window;
        this.performanceSimulationPresenter = performanceSimulationPresenter;
    }

    public void delete(){
        if(checkComboBox == null ) return;
        showStatus("Preparing to delete",true);
        if(checkComboBox.getCheckModel().getCheckedIndices().size() > 0){
            logger.debug("Starting to delete tasks");
            ObservableList<SimulationTask> tasks = FXCollections.observableArrayList(checkComboBox.getCheckModel().getCheckedItems());
            if(view.getSaveCheckBox().isSelected()){
                showStatus("Saving chosen tasks",true);
                saveTasks(tasks);
            }
            for(SimulationTask task : tasks){
                performanceSimulationPresenter.deleteTask(task);
                showStatus("Deleting task " + task.toString(), true);
            }
            System.gc();
            close();
        }else{
            logger.debug("No tasks chosen to be deleted");
            showStatus("Please choose tasks",false);
        }

    }

    private void saveTasks(ObservableList<SimulationTask> tasks){
        logger.trace("Saving tasks");
        performanceSimulationPresenter.saveAll(tasks);
    }

    public void close(){
        window.hide();
    }

    public void showStatus(String status,boolean indicate){
        view.getStatus().setText(status);
        view.getIndicator().setVisible(indicate);
    }

    private void init(){
        if(performanceSimulationPresenter == null || view == null) return;
        checkComboBox = new CheckComboBox<>(performanceSimulationPresenter.getTasks());
        checkComboBox.setPrefWidth(300);
        checkComboBox.setMaxWidth(300);
        view.getListContainer().getChildren().add(checkComboBox);
        showStatus("",false);
    }

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
