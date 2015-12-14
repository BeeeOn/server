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
 * Class extending {@link javafx.scene.control.TableCell} used to display task's stop conditions in tasks table.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class SimulationTaskStopCellFactory extends TableCell<SimulationTask, SimulationTask> {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(SimulationTaskStopCellFactory.class);
    /**
     * {@inheritDoc}
     *
     * @param item simulation task, that needs to display it's stop conditions information
     */
    @Override
    protected void updateItem(SimulationTask item, boolean empty) {
        super.updateItem(item, empty);
        //if there is item to be displayed
        if (item != null) {
            //create main label to display number of stop conditions
            Label lbl = new Label();
            //detailed information add to tool tip
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
