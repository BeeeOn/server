package com.iha.emulator.ui.panels.task.cell;

import com.iha.emulator.control.task.SimulationTask;
import javafx.scene.control.CheckBox;
import javafx.scene.control.TableCell;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

/**
 * Created by Shu on 26.2.2015.
 */
public class SimulationTaskEnabledCellFactory extends TableCell<SimulationTask, SimulationTask> {
    private static final Logger logger = LogManager.getLogger(SimulationTaskEnabledCellFactory.class);
    @Override
    protected void updateItem(SimulationTask item, boolean empty) {
        super.updateItem(item, empty);
        if (item != null) {
            CheckBox checkBox = new CheckBox();
            checkBox.selectedProperty().bindBidirectional(item.enabledProperty());
            setGraphic(checkBox);
        } else {
            setText(null);
            setGraphic(null);
        }
    }
}
