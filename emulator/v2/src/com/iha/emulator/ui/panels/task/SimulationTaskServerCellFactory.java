package com.iha.emulator.ui.panels.task;

import com.iha.emulator.control.SimulationTask;
import javafx.scene.control.Label;
import javafx.scene.control.TableCell;
import javafx.scene.control.Tooltip;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

/**
 * Created by Shu on 26.2.2015.
 */
public class SimulationTaskServerCellFactory extends TableCell<SimulationTask, SimulationTask> {
    private static final Logger logger = LogManager.getLogger(SimulationTaskServerCellFactory.class);
    @Override
    protected void updateItem(SimulationTask item, boolean empty) {
        super.updateItem(item, empty);
        if (item != null) {
            Label lbl = new Label();
            lbl.textProperty().bindBidirectional(item.getServerController().getModel().nameProperty());
            Tooltip tp = new Tooltip(
                    "Name: " + item.getServerController().getModel().getName() + "\n" +
                    "IP: " + item.getServerController().getModel().getIp() + "\n" +
                    "Port: " +item.getServerController().getModel().getPort() + "\n" +
                    "Database: " +item.getServerController().getModel().getDatabaseName()
            );
            tp.setAutoHide(true);
            lbl.setTooltip(tp);
            setGraphic(lbl);
        } else {
            setText(null);
            setGraphic(null);
        }
    }
}
