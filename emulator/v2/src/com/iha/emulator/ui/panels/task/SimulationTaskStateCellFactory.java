package com.iha.emulator.ui.panels.task;

import com.iha.emulator.control.SimulationTask;
import javafx.beans.binding.StringBinding;
import javafx.scene.control.Label;
import javafx.scene.control.TableCell;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

/**
 * Created by Shu on 26.2.2015.
 */
public class SimulationTaskStateCellFactory extends TableCell<SimulationTask, SimulationTask> {
    private static final Logger logger = LogManager.getLogger(SimulationTaskStateCellFactory.class);
    @Override
    protected void updateItem(SimulationTask item, boolean empty) {
        super.updateItem(item, empty);
        if (item != null) {
            Label lbl = new Label();
            lbl.textProperty().bind(new StringBinding() {
                {
                    bind(item.simulationStateProperty());
                }
                @Override
                protected String computeValue() {
                    switch (item.getSimulationState()){
                        case RUNNING:
                            lbl.getStyleClass().clear();
                            lbl.getStyleClass().add("taskStateRunning");
                            break;
                        case PAUSED:
                            lbl.getStyleClass().clear();
                            lbl.getStyleClass().add("taskStatePaused");
                            break;
                        case RESUMED:
                            lbl.getStyleClass().clear();
                            lbl.getStyleClass().add("taskStateResumed");
                            break;
                        case STOPPED:
                            lbl.getStyleClass().clear();
                            lbl.getStyleClass().add("taskStateStopped");
                            break;
                        case PAUSED_SENSORS:
                            lbl.getStyleClass().clear();
                            lbl.getStyleClass().add("taskStatePaused");
                            break;
                        case RESUMED_SENSORS:
                            lbl.getStyleClass().clear();
                            lbl.getStyleClass().add("taskStateResumed");
                            break;
                        case WAITING:
                            lbl.getStyleClass().clear();
                            lbl.getStyleClass().add("taskStateWaiting");
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
