package com.iha.emulator.ui.panels.task.cell;

import com.iha.emulator.control.task.SimulationTask;
import javafx.beans.binding.StringBinding;
import javafx.scene.control.Label;
import javafx.scene.control.TableCell;
import javafx.scene.control.Tooltip;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

/**
 * Class extending {@link javafx.scene.control.TableCell} used to display task's ID in tasks table.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class SimulationTaskIdCellFactory extends TableCell<SimulationTask, SimulationTask> {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(SimulationTaskIdCellFactory.class);
    /**
     * {@inheritDoc}
     *
     * @param item simulation task, that needs to display it's ID
     */
    @Override
    protected void updateItem(SimulationTask item, boolean empty) {
        super.updateItem(item, empty);
        //if there is item to be displayed
        if (item != null) {
            //create main label showing ID
            Label lbl = new Label();
            //add log file location to it's tool tip
            lbl.textProperty().bind(new StringBinding() {
                {
                    bind(item.idProperty());
                }
                @Override
                protected String computeValue() {
                    Tooltip tp = new Tooltip("Log file location: " + item.getLog().getBufferFile().getAbsolutePath());
                    lbl.setTooltip(tp);
                    return String.valueOf(item.getId());
                }
            });
            setGraphic(lbl);
        } else {
            setText(null);
            setGraphic(null);
        }
    }
}
