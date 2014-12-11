package com.iha.emulator.communication.protocol;

import javafx.beans.property.DoubleProperty;
import javafx.beans.property.FloatProperty;
import javafx.beans.property.SimpleDoubleProperty;
import javafx.beans.property.SimpleFloatProperty;

/**
 * Created by Shu on 26.11.2014.
 */
public abstract class AbstractProtocol implements Protocol{

    private DoubleProperty version;

    public AbstractProtocol(double version) {
        this.version = new SimpleDoubleProperty(version);
    }

    public double getVersion() {
        return version.get();
    }

    public DoubleProperty versionProperty() {
        return version;
    }

    public void setVersion(double version) {
        this.version.set(version);
    }

    public abstract String buildAdapterMessage();
    public abstract String buildSensorMessage();
}
