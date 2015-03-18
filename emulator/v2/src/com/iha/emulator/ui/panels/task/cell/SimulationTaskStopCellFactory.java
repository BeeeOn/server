package com.iha.emulator.ui.panels.task.cell;

import com.iha.emulator.control.task.SimulationTask;
import com.iha.emulator.control.task.StopCondition;
import com.iha.emulator.utilities.Utilities;
import javafx.beans.binding.StringBinding;
import javafx.scene.control.Label;
import javafx.scene.control.TableCell;
import javafx.scene.control.Tooltip;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

/**
 * Created by Shu on 26.2.2015.
 */
public class SimulationTaskStopCellFactory extends TableCell<SimulationTask, SimulationTask> {
    private static final Logger logger = LogManager.getLogger(SimulationTaskStopCellFactory.class);
    @Override
    protected void updateItem(SimulationTask item, boolean empty) {
        super.updateItem(item, empty);
        if (item != null) {
            Label lbl = new Label();
            lbl.textProperty().bind(new StringBinding() {
                {
                    bind(item.getStopCondition().conditionCounterProperty());
                }

                @Override
                protected String computeValue() {
                    StopCondition con = item.getStopCondition();
                    Tooltip tp = new Tooltip(
                            "Conditions: " + "\n" +
                                    (con.isTimerEnabled() ? "\t Task duration: " + Utilities.formatSeconds(con.getSecondsToTrigger()) + "\n" : "") +
                                    (con.isSentMessageEnabled() ? "\t Sent messages count: " + con.getMessageCountToTrigger()  + "\n" : "") +
                                    (con.isWaitingMessageEnabled() ? "\t Waiting messages count: " + con.getWaitingMessagesToTrigger() : "")
                    );
                    tp.setAutoHide(true);
                    lbl.setTooltip(tp);
                    return item.getStopCondition().getConditionCounter() + " condition/s";
                }
            });
            setGraphic(lbl);
        } else {
            setText(null);
            setGraphic(null);
        }
    }
}
