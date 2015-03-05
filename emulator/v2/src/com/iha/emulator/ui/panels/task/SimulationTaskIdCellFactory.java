package com.iha.emulator.ui.panels.task;

import com.iha.emulator.control.SimulationTask;
import javafx.beans.binding.StringBinding;
import javafx.scene.control.Label;
import javafx.scene.control.TableCell;
import javafx.scene.control.Tooltip;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

/**
 * Created by Shu on 26.2.2015.
 */
public class SimulationTaskIdCellFactory extends TableCell<SimulationTask, SimulationTask> {
    private static final Logger logger = LogManager.getLogger(SimulationTaskIdCellFactory.class);
    @Override
    protected void updateItem(SimulationTask item, boolean empty) {
        super.updateItem(item, empty);
        if (item != null) {
            Label lbl = new Label();
            lbl.textProperty().bind(new StringBinding() {
                {
                    bind(item.idProperty());
                }
                @Override
                protected String computeValue() {
                    return String.valueOf(item.getId());
                }
            });
            Tooltip tp = new Tooltip("Log file location: " + item.getLog().getBufferFile().getAbsolutePath());
            lbl.setTooltip(tp);
            setGraphic(lbl);
        } else {
            setText(null);
            setGraphic(null);
        }
    }
}
