package com.iha.emulator.ui.panels.task;

import com.iha.emulator.control.SimulationTask;
import javafx.beans.property.ReadOnlyObjectWrapper;
import javafx.beans.value.ObservableValue;
import javafx.scene.control.TableColumn;
import javafx.util.Callback;

/**
 * Created by Shu on 26.2.2015.
 */
public class SimulationTaskFactory implements Callback<TableColumn.CellDataFeatures<SimulationTask, Object>, ObservableValue<Object>> {
    @SuppressWarnings("unchecked")
    @Override
    public ObservableValue<Object> call(TableColumn.CellDataFeatures<SimulationTask, Object> param) {
        Object value = param.getValue();
        return (value instanceof ObservableValue)
                ? (ObservableValue) value
                : new ReadOnlyObjectWrapper<>(value);
    }
}
