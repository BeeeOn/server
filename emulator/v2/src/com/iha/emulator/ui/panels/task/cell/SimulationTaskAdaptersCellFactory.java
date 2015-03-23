package com.iha.emulator.ui.panels.task.cell;

import com.iha.emulator.control.task.SimulationTask;
import com.iha.emulator.control.task.TaskParameters;
import javafx.beans.binding.StringBinding;
import javafx.scene.control.Label;
import javafx.scene.control.TableCell;
import javafx.scene.control.Tooltip;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

/**
 * Created by Shu on 26.2.2015.
 */
public class SimulationTaskAdaptersCellFactory extends TableCell<SimulationTask, SimulationTask> {
    private static final Logger logger = LogManager.getLogger(SimulationTaskAdaptersCellFactory.class);
    @Override
    protected void updateItem(SimulationTask item, boolean empty) {
        super.updateItem(item, empty);
        if (item != null) {
            Label lbl = new Label();
            lbl.textProperty().bind(new StringBinding() {
                @Override
                protected String computeValue() {
                    return "Max " + (item.getTaskParameters().getAdaptersCount() * item.getTaskParameters().getSensorsCountMax()) + " devices";
                }
            });
            TaskParameters params = item.getTaskParameters();
            Tooltip tp = new Tooltip(
                            "Adapters count: " + params.getAdaptersCount() + "\n" +
                            "Protocol: " + params.getProtocolVersion().getVersion() + "\n" +
                            "Start ID: " + params.getStartId() + "\n" +
                            "Sensors count per adapter: " + (params.getSensorsCountMin()!=0 ? params.getSensorsCountMin() + " -> " : "") + params.getSensorsCountMax() + "\n" +
                            "Refresh time range: " + (params.getRefreshTimeMin()!=0 ? params.getRefreshTimeMin() + " -> " : "") + params.getRefreshTimeMax()
            );
            tp.setAutoHide(false);
            lbl.setTooltip(tp);
            setGraphic(lbl);
        } else {
            setText(null);
            setGraphic(null);
        }
    }
}