package com.iha.emulator.ui.panels.task.cell;

import com.iha.emulator.control.task.SimulationTask;
import javafx.scene.control.CheckBox;
import javafx.scene.control.TableCell;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

/**
 * Class extending {@link javafx.scene.control.TableCell} used to display task's enabled status in tasks table.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class SimulationTaskEnabledCellFactory extends TableCell<SimulationTask, SimulationTask> {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(SimulationTaskEnabledCellFactory.class);
    /**
     * {@inheritDoc}
     *
     * @param item simulation task, that needs to display it's status
     */
    @Override
    protected void updateItem(SimulationTask item, boolean empty) {
        super.updateItem(item, empty);
        //if there is item to be displayed
        if (item != null) {
            //create checkbox and bind it tasks status
            CheckBox checkBox = new CheckBox();
            checkBox.selectedProperty().bindBidirectional(item.enabledProperty());
            setGraphic(checkBox);
        } else {
            setText(null);
            setGraphic(null);
        }
    }
}
