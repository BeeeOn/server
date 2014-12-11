package com.iha.emulator.ui.panels.sensor;

import com.iha.emulator.models.value.Value;
import javafx.beans.property.ReadOnlyObjectWrapper;
import javafx.beans.value.ObservableValue;
import javafx.scene.control.TableColumn;
import javafx.util.Callback;

/**
 * Created by Shu on 2.12.2014.
 */
public class SensorValueFactory implements Callback<TableColumn.CellDataFeatures<Value, Object>, ObservableValue<Object>> {
    @SuppressWarnings("unchecked")
    @Override
    public ObservableValue<Object> call(TableColumn.CellDataFeatures<Value, Object> param) {
        Object value = param.getValue();
        return (value instanceof ObservableValue)
                ? (ObservableValue) value
                : new ReadOnlyObjectWrapper<>(value);
    }
}
