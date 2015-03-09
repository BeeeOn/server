package com.iha.emulator.ui.panels.task;

import com.iha.emulator.control.SimulationTask;
import com.iha.emulator.control.TaskParameters;
import javafx.beans.binding.Bindings;
import javafx.beans.binding.StringBinding;
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
            /*lbl.textProperty().bindBidirectional(item.getServerController().getModel().nameProperty());
            Tooltip tp = new Tooltip(
                    "Server: " + "\n" +
                    "Name: " + item.getServerController().getModel().getName() + "\n" +
                    "IP: " + item.getServerController().getModel().getIp() + "\n" +
                    "Port: " +item.getServerController().getModel().getPort() + "\n" +
                    "Database: " +item.getServerController().getModel().getDatabaseName()
            );*/
            tp.setAutoHide(true);
            lbl.setTooltip(tp);
            setGraphic(lbl);
        } else {
            setText(null);
            setGraphic(null);
        }
    }
}
