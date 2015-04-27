package com.iha.emulator.models;

import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.communication.protocol.ProtocolFactory;
import javafx.beans.property.*;

/**
 * Model of adapter holding information needed to emulate adapter's behaviour.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class Adapter {
    /** status property */
    private BooleanProperty status;
    /** name property */
    private StringProperty name;
    /** id property */
    private IntegerProperty id;
    /** registered indicator property */
    private BooleanProperty registered;
    /** number of currently connected sensor's property */
    private IntegerProperty activeCount;
    /** number of currently created sensor's property */
    private IntegerProperty registeredCount;
    /** instance of class implementing {@link com.iha.emulator.communication.protocol.Protocol} */
    private Protocol protocol;
    /** firmware version property */
    private DoubleProperty firmware;

    /**
     * Creates Adapter model with name "EA"+ given adapter id (Example: EA1111), id, registered indicator, protocol version, firmware version.
     *
     * @param status adapter status (true=Active/false=Inactive)
     * @param id adapter's id
     * @param registered adapter's registered indicator (is adapter registered on server?)
     * @param protocolVersion {@link com.iha.emulator.communication.protocol.Protocol.Version} enum's constant indicating used protocol version
     * @param firmware adapter's firmware version
     */
    public Adapter(boolean status,int id,boolean registered,Protocol.Version protocolVersion,double firmware) {
        this("EA" + id,status,id,registered,protocolVersion,firmware);

    }
    /**
     * Creates Adapter model with given name, id, registered indicator, protocol version, firmware version.
     *
     * @param name adapter's name in emulator context
     * @param status adapter's status (true=Active/false=Inactive)
     * @param id adapter's id
     * @param registered adapter's registered indicator (is adapter registered on server?)
     * @param protocolVersion {@link com.iha.emulator.communication.protocol.Protocol.Version} enum's constant indicating used protocol version
     * @param firmware adapter's firmware version
     */
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

    /**
     * Gets adapter's status (true=Active/false=Inactive)
     * @return adapter's status
     */
    public boolean getStatus() {
        return status.get();
    }

    /**
     * Gets adapter's status property, can be bound
     * @return adapter's status property
     */
    public BooleanProperty statusProperty() {
        return status;
    }

    /**
     * Sets adapter's status (true=Active/false=Inactive)
     * @param status adapter's status
     */
    public void setStatus(boolean status) {
        this.status.set(status);
    }

    /**
     * Gets adapter's name
     * @return adapter's name
     */
    public String getName() {
        return name.get();
    }

    /**
     * Gets adapter's name property, can be bound
     * @return adapter's name property
     */
    public StringProperty nameProperty() {
        return name;
    }

    /**
     * Sets adapter's name
     * @param name adapter's name
     */
    public void setName(String name) {
        this.name.set(name);
    }

    /**
     * Gets adapter's ID
     * @return adapter's ID
     */
    public int getId() {
        return id.get();
    }

    /**
     * Gets adapter's ID property, can be bound
     * @return adapter's ID property
     */
    public IntegerProperty idProperty() {
        return id;
    }

    /**
     * Sets adapter's ID
     * @param id adapter's ID
     */
    public void setId(int id) {
        this.id.set(id);
    }

    /**
     * Gets adapter's registered indicator
     * @return adapter's registered indicator
     */
    public boolean getRegistered() {
        return registered.get();
    }

    /**
     * Gets adapter's registered indicator property, can be found
     * @return adapter's registered indicator property
     */
    public BooleanProperty registeredProperty() {
        return registered;
    }

    /**
     * Sets adapter's registered indicator
     * @param registered adapter's registered indicator
     */
    public void setRegistered(boolean registered) {
        this.registered.set(registered);
    }

    /**
     * Gets number of currently connected sensors to adapter
     * @return number of currently connected sensors to adapter
     */
    public int getActiveCount() {
        return activeCount.get();
    }

    /**
     * Gets number of currently connected sensors to adapter property, can be bound
     * @return number of currently connected sensors to adapter property
     */
    public IntegerProperty activeCountProperty() {
        return activeCount;
    }

    /**
     * Sets number of currently connected sensors to adapter
     * @param activeCount number of currently connected sensors to adapter
     */
    public void setActiveCount(int activeCount) {
        this.activeCount.set(activeCount);
    }

    /**
     * Gets number of currently created sensors on adapter
     * @return number of currently created sensors on adapter
     */
    public int getRegisteredCount() {
        return registeredCount.get();
    }

    /**
     * Gets number of currently created sensors on adapter property, can be bound
     * @return number of currently created sensors on adapter
     */
    public IntegerProperty registeredCountProperty() {
        return registeredCount;
    }

    /**
     * Sets number of currently created sensors on adapter
     * @param registeredCount number of currently created sensors on adapter
     */
    public void setRegisteredCount(int registeredCount) {
        this.registeredCount.set(registeredCount);
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

    /**
     * Gets adapter's firmware version
     * @return adapter's firmware version
     */
    public double getFirmware() {
        return firmware.get();
    }

    /**
     * Gets adapter's firmware version property, can be bound
     * @return adapter's firmware version
     */
    public DoubleProperty firmwareProperty() {
        return firmware;
    }

    /**
     * Sets adapter's firmware version
     * @param firmware adapter's firmware version
     */
    public void setFirmware(double firmware) {
        this.firmware.set(firmware);
    }


}
