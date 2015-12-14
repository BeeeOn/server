package com.iha.emulator.ui.panels.task.cell;

import com.iha.emulator.control.task.SimulationTask;
import javafx.beans.binding.StringBinding;
import javafx.scene.control.Label;
import javafx.scene.control.TableCell;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

/**
 * Class extending {@link javafx.scene.control.TableCell} used to display task's state in tasks table.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class SimulationTaskStateCellFactory extends TableCell<SimulationTask, SimulationTask> {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(SimulationTaskStateCellFactory.class);
    /**
     * {@inheritDoc}
     *
     * @param item simulation task, that needs to display it's state
     */
    @Override
    protected void updateItem(SimulationTask item, boolean empty) {
        super.updateItem(item, empty);
        //if there is item to be displayed
        if (item != null) {
            //create main label with state
            Label lbl = new Label();
            //bind label style to specific state
            lbl.textProperty().bind(new StringBinding() {
                {
                    bind(item.simulationStateProperty());
                }
                @Override
                protected String computeValue() {
                    switch (item.getSimulationState()){
                        case READY:
                            lbl.getStyleClass().clear();
                            lbl.getStyleClass().add("taskStateWaiting");
                            break;
                        case RUNNING:
                            lbl.getStyleClass().clear();
                            lbl.getStyleClass().add("taskStateRunning");
                            break;
                        case PAUSED:
                            lbl.getStyleClass().clear();
                            lbl.getStyleClass().add("taskStatePaused");
                            break;
                        case FINISHED:
                            lbl.getStyleClass().clear();
                            lbl.getStyleClass().add("taskStateFinished");
                            break;
                        case ERROR:
                            lbl.getStyleClass().clear();
                            lbl.getStyleClass().add("taskStateError");
                            break;
                    }
                    return item.getSimulationState().getName();
                }
            });
            setGraphic(lbl);
        } else {
            setText(null);
            setGraphic(null);
        }
    }
}
