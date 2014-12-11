package com.iha.emulator.models.value;

import com.iha.emulator.models.SensorType;
import javafx.beans.property.StringProperty;

import java.util.Random;

/**
 * Created by Shu on 2.12.2014.
 */
public interface Value<T> {

    public T getValue();
    public void setValue(T value);
    public void nextValue();
    public void nextValue(T value);
    public Random getGenerator();
    public String toStringWithUnit();
    public StringProperty stringValueProperty();
    public SensorType getValueType();
}
