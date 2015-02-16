package com.iha.emulator.models;

import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.communication.protocol.ProtocolFactory;
import javafx.beans.property.*;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

/**
 * Created by Shu on 26.11.2014.
 */
public class Adapter {

    private static final Logger logger = LogManager.getLogger(Adapter.class);

    private BooleanProperty status;
    private StringProperty name;
    private IntegerProperty id;
    private BooleanProperty registered;
    private IntegerProperty activeCount;
    private IntegerProperty registeredCount;
    private Protocol protocol;
    private DoubleProperty firmware;

    //TODO change firmware to version (integer)
    public Adapter(boolean status,int id,boolean registered,Protocol.Version protocolVersion,double firmware) {
        this("EA" + id,status,id,registered,protocolVersion,firmware);

    }

    public Adapter(String name,boolean status,int id,boolean registered,Protocol.Version protocolVersion,double firmware){
        //default status must be false
        this.status = new SimpleBooleanProperty(status);
        //set name
        this.name = new SimpleStringProperty(name);
        //assign id
        this.id = new SimpleIntegerProperty(id);
        //is adapter registered on server/is in database?
        this.registered = new SimpleBooleanProperty(registered);
        //default number of active sensors is 0
        this.activeCount = new SimpleIntegerProperty(0);
        //default number of registered sensors is 0
        this.registeredCount = new SimpleIntegerProperty(0);
        //build protocol controller
        this.protocol = ProtocolFactory.buildProtocol(protocolVersion);
        //set firmware
        this.firmware = new SimpleDoubleProperty(firmware);
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

    public String getName() {
        return name.get();
    }

    public StringProperty nameProperty() {
        return name;
    }

    public void setName(String name) {
        this.name.set(name);
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

    public boolean getRegistered() {
        return registered.get();
    }

    public BooleanProperty registeredProperty() {
        return registered;
    }

    public void setRegistered(boolean registered) {
        logger.trace("Registered set to -> " + registered);
        this.registered.set(registered);
    }

    public int getActiveCount() {
        return activeCount.get();
    }

    public IntegerProperty activeCountProperty() {
        return activeCount;
    }

    public void setActiveCount(int activeCount) {
        this.activeCount.set(activeCount);
    }

    public int getRegisteredCount() {
        return registeredCount.get();
    }

    public IntegerProperty registeredCountProperty() {
        return registeredCount;
    }

    public void setRegisteredCount(int registeredCount) {
        this.registeredCount.set(registeredCount);
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

    public double getFirmware() {
        return firmware.get();
    }

    public DoubleProperty firmwareProperty() {
        return firmware;
    }

    public void setFirmware(double firmware) {
        this.firmware.set(firmware);
    }


}
