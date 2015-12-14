package com.iha.emulator.models;

import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.models.value.Value;
import javafx.beans.property.*;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

/**
 * Model of sensor holding information needed to emulate sensor's/actuator's behaviour.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class Sensor {
    /** connection to adapter status property */
    private BooleanProperty status;
    /** id property */
    private IntegerProperty id;
    /** name property */
    private StringProperty name;
    /** battery value property */
    private IntegerProperty battery;
    /** signal value property */
    private IntegerProperty signal;
    /** refresh time property */
    private IntegerProperty refreshTime;
    /** instance of class implementing {@link com.iha.emulator.communication.protocol.Protocol} */
    private Protocol protocol;
    /** list of sensor's/actuator's values */
    private ObservableList<Value> values = FXCollections.observableArrayList();

    /**
     * Creates Sensor model with given connection status, id, name, battery,...
     * @param status connection to adapter status
     * @param id sensor's ID
     * @param name sensor's name
     * @param battery sensor's battery value
     * @param signal sensor's signal value
     * @param refreshTime sensor's refresh time
     * @param protocol instance of class implementing {@link com.iha.emulator.communication.protocol.Protocol}
     */
    public Sensor(boolean status,int id,String name,int battery,int signal,int refreshTime,Protocol protocol) {
        this.status = new SimpleBooleanProperty(status);
        this.id = new SimpleIntegerProperty(id);
        this.name = new SimpleStringProperty(name);
        this.battery = new SimpleIntegerProperty(battery);
        this.signal = new SimpleIntegerProperty(signal);
        this.refreshTime = new SimpleIntegerProperty(refreshTime);
        this.protocol = protocol;
    }

    /**
     * Gets list of sensor's values
     * @return list of sensor's values
     */
    public ObservableList<Value> getValues() {
        return values;
    }

    /**
     * Sets list of sensor's values
     * @param values list of sensor's values
     */
    public void setValues(ObservableList<Value> values) {
        this.values = values;
    }

    /**
     * Gets connection to adapter status
     * @return connection to adapter status
     */
    public boolean getStatus() {
        return status.get();
    }

    /**
     * connection to adapter status property, can be bound
     * @return connection to adapter status property
     */
    public BooleanProperty statusProperty() {
        return status;
    }

    /**
     * Sets connection to adapter status
     * @param status connection to adapter status
     */
    public void setStatus(boolean status) {
        this.status.set(status);
    }

    /**
     * Gets sensor's ID
     * @return sensor's ID
     */
    public int getId() {
        return id.get();
    }

    /**
     * Gets sensor's ID property, can be bound
     * @return sensor's ID property
     */
    public IntegerProperty idProperty() {
        return id;
    }

    /**
     * Sets sensor's ID
     * @param id sensor's ID
     */
    public void setId(int id) {
        this.id.set(id);
    }

    /**
     * Gets sensor's name
     * @return sensor's name
     */
    public String getName() {
        return name.get();
    }

    /**
     * Gets sensor's name property, can be bound
     * @return sensor's name property
     */
    public StringProperty nameProperty() {
        return name;
    }

    /**
     * Sets sensor's name
     * @param name sensor's name
     */
    public void setName(String name) {
        this.name.set(name);
    }

    /**
     * Gets sensor's battery value
     * @return sensor's battery value
     */
    public int getBattery() {
        return battery.get();
    }

    /**
     * Gets sensor's battery value property, can be bound
     * @return sensor's battery value property
     */
    public IntegerProperty batteryProperty() {
        return battery;
    }

    /**
     * Sets sensor's battery value
     * @param battery sensor's battery value
     */
    public void setBattery(int battery) {
        this.battery.set(battery);
    }

    /**
     * Gets sensor's signal value
     * @return sensor's signal value
     */
    public int getSignal() {
        return signal.get();
    }

    /**
     * Gets sensor's signal value property, can be bound
     * @return sensor's signal value property
     */
    public IntegerProperty signalProperty() {
        return signal;
    }

    /**
     * Sets sensor's signal value
     * @param signal sensor's signal value
     */
    public void setSignal(int signal) {
        this.signal.set(signal);
    }

    /**
     * Gets sensor's refresh time
     * @return sensor's refresh time
     */
    public int getRefreshTime() {
        return refreshTime.get();
    }

    /**
     * Gets sensor's refresh time property, can be bound
     * @return sensor's refresh time property
     */
    public IntegerProperty refreshTimeProperty() {
        return refreshTime;
    }

    /**
     * Sets sensor's refresh time
     * @param refreshTime sensor's refresh time
     */
    public void setRefreshTime(int refreshTime) {
        if(refreshTime != 0) this.refreshTime.set(refreshTime);
    }

    /**
     * Gets protocol version implementation instance
     * @return protocol version implementation instance
     */
    public Protocol getProtocol() {
        return protocol;
    }

    /**
     * Gets protocol version number
     * @return protocol version number
     */
    public double getProtocolVersion(){
        return protocol.getVersion();
    }

    /**
     * Sets protocol version implementation instance
     * @param protocol protocol version implementation instance
     */
    public void setProtocol(Protocol protocol) {
        this.protocol = protocol;
    }

    /**
     * Sets protocol version number
     * @param version protocol version number
     */
    public void setProtocolVersion(double version){
        this.protocol.setVersion(version);
    }
}
