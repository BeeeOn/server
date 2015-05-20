package com.iha.emulator.ui.panels.task.cell;

import com.iha.emulator.control.task.SimulationTask;
import com.iha.emulator.control.task.TaskParameters;
import javafx.beans.binding.Bindings;
import javafx.beans.binding.StringBinding;
import javafx.scene.control.Label;
import javafx.scene.control.TableCell;
import javafx.scene.control.Tooltip;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

/**
 * Class extending {@link javafx.scene.control.TableCell} used to display information about adapters and server in tasks table.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class SimulationTaskServerCellFactory extends TableCell<SimulationTask, SimulationTask> {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(SimulationTaskServerCellFactory.class);
    /**
     * {@inheritDoc}
     *
     * @param item simulation task, that needs to display it's server information
     */
    @Override
    protected void updateItem(SimulationTask item, boolean empty) {
        //if there is item to be displayed
        super.updateItem(item, empty);
        if (item != null) {
            //create main label showing server name and max devices, that can be created
            Label lbl = new Label();
            lbl.textProperty().bind(Bindings.concat(new StringBinding() {
                {
                    bind(item.getServerController().getModel().nameProperty());
                }

                @Override
                protected String computeValue() {
                    return "Server: " +  "\t" + item.getServerController().getModel().getName();
                }
            }, "\n", new StringBinding() {
                {
                    bind(item.getTaskParameters().adaptersCountProperty().multiply(item.getTaskParameters().sensorsCountMaxProperty()));
                }
                @Override
                protected String computeValue() {
                    return "Max: "  +  "\t" +(item.getTaskParameters().getAdaptersCount() * item.getTaskParameters().getSensorsCountMax()) + " devices";
                }
            }));
            //detailed information about server and adapters add to tool tip
            TaskParameters params = item.getTaskParameters();
            Tooltip tp = new Tooltip(
                    "Server: " + "\n" +
                            "\t" + "Name: " + item.getServerController().getModel().getName() + "\n" +
                            "\t" + "IP: " + item.getServerController().getModel().getIp() + "\n" +
                            "\t" + "Port: " +item.getServerController().getModel().getPort() + "\n" +
                            "\t" + "Database: " +item.getServerController().getModel().getDatabaseName() + "\n" +
                    "Adapters: " + "\n" +
                            "\t" + "Adapters count: " + params.getAdaptersCount() + "\n" +
                            "\t" + "Protocol: " + params.getProtocolVersion().getVersion() + "\n" +
                            "\t" + "Start ID: " + params.getStartId() + "\n" +
                            "\t" + "Sensors count per adapter: " + (params.getSensorsCountMin()!=0 ? params.getSensorsCountMin() + " -> " : "") + params.getSensorsCountMax() + "\n" +
                            "\t" + "Refresh time range: " + (params.getRefreshTimeMin()!=0 ? params.getRefreshTimeMin() + " -> " : "") + params.getRefreshTimeMax()
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
