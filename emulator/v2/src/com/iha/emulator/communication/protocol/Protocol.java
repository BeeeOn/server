package com.iha.emulator.communication.protocol;

import javafx.beans.property.DoubleProperty;

/**
 * Created by Shu on 26.11.2014.
 */
public interface Protocol {
    public double getVersion();
    public void setVersion(double version);
    public DoubleProperty versionProperty();
    public String buildAdapterMessage();
    public String buildSensorMessage();
}
