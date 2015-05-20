package com.iha.emulator.ui.panels.sensor;

import com.iha.emulator.models.value.Value;
import javafx.beans.property.ReadOnlyObjectWrapper;
import javafx.beans.value.ObservableValue;
import javafx.scene.control.TableColumn;
import javafx.util.Callback;

/**
 * Class used to display a value name in values table.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class SensorValueFactory implements Callback<TableColumn.CellDataFeatures<Value, Object>, ObservableValue<Object>> {
    /**
     * {@inheritDoc}
     */
    @SuppressWarnings("unchecked")
    @Override
    public ObservableValue<Object> call(TableColumn.CellDataFeatures<Value, Object> param) {
        Object value = param.getValue();
        return (value instanceof ObservableValue)
                ? (ObservableValue) value
                : new ReadOnlyObjectWrapper<>(value);
    }
}
