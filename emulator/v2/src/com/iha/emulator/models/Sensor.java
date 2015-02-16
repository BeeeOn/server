package com.iha.emulator.models;

import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.models.value.Value;
import javafx.beans.property.*;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

/**
 * Created by Shu on 28.11.2014.
 */
public class Sensor {

    private BooleanProperty status;
    private IntegerProperty id;
    private StringProperty name;
    private IntegerProperty battery;
    private IntegerProperty signal;
    private IntegerProperty refreshTime;
    private Protocol protocol;

    private ObservableList<Value> values = FXCollections.observableArrayList();

    public Sensor(boolean status,int id,String name,int battery,int signal,int refreshTime,Protocol protocol) {
        this.status = new SimpleBooleanProperty(status);
        this.id = new SimpleIntegerProperty(id);
        this.name = new SimpleStringProperty(name);
        this.battery = new SimpleIntegerProperty(battery);
        this.signal = new SimpleIntegerProperty(signal);
        this.refreshTime = new SimpleIntegerProperty(refreshTime);
        this.protocol = protocol;
    }

    public ObservableList<Value> getValues() {
        return values;
    }

    public void setValues(ObservableList<Value> values) {
        this.values = values;
    }

    public boolean getStatus() {
        return status.get();
    }

    public BooleanProperty statusProperty() {
        return status;
    }

    public void setStatus(boolean status) {
        this.status.set(status);
    }

    public int getId() {
        return id.get();
    }

    public IntegerProperty idProperty() {
        return id;
    }

    public void setId(int id) {
        this.id.set(id);
    }

    public String getName() {
        return name.get();
    }

    public StringProperty nameProperty() {
        return name;
    }

    public void setName(String name) {
        this.name.set(name);
    }

    public int getBattery() {
        return battery.get();
    }

    public IntegerProperty batteryProperty() {
        return battery;
    }

    public void setBattery(int battery) {
        this.battery.set(battery);
    }

    public int getSignal() {
        return signal.get();
    }

    public IntegerProperty signalProperty() {
        return signal;
    }

    public void setSignal(int signal) {
        this.signal.set(signal);
    }

    public int getRefreshTime() {
        return refreshTime.get();
    }

    public IntegerProperty refreshTimeProperty() {
        return refreshTime;
    }

    public void setRefreshTime(int refreshTime) {
        if(refreshTime != 0) this.refreshTime.set(refreshTime);
    }

    public Protocol getProtocol() {
        return protocol;
    }

    public double getProtocolVersion(){
        return protocol.getVersion();
    }

    public void setProtocol(Protocol protocol) {
        this.protocol = protocol;
    }

    public void setProtocolVersion(double version){
        this.protocol.setVersion(version);
    }
}
