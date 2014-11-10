package com.smarthome.emulator.simulations.basis;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by Shu on 3.6.2014.
 */
public class ActuatorMessage {

    private Sensor sensor = null;
    private short protocol;
    private String sensorId;
    private String adapterId;
    private short refreshRate;
    private Map<Short,String> values = new HashMap<>();

    public ActuatorMessage(short protocol,String sensorId,String adapterId,short refreshRate) {
        setProtocol(protocol);
        setSensorId(sensorId);
        setAdapterId(adapterId);
        setRefreshRate(refreshRate);
    }
    public ActuatorMessage(short protocol,String sensorId,String adapterId,short refreshRate,Map<Short,String> values) {
        this(protocol, sensorId, adapterId, refreshRate);
        this.values.putAll(values);

    }

    public ActuatorMessage(short protocol,String sensorId,String adapterId,short refreshRate,Map<Short,String> values,Sensor sensor) {
        this(protocol, sensorId, adapterId, refreshRate,values);
        setSensor(sensor);
    }

    public void addValue(Short type,String value){
        this.values.put(type,value);
    }

    public Map<Short,String> getValues(){
        return this.values;
    }

    public short getProtocol() {
        return protocol;
    }

    public void setProtocol(short protocol) {
        this.protocol = protocol;
    }

    public String getSensorId() {
        return sensorId;
    }

    public void setSensorId(String sensorId) {
        this.sensorId = sensorId;
    }

    public String getAdapterId() {
        return adapterId;
    }

    public void setAdapterId(String adapterId) {
        this.adapterId = adapterId;
    }

    public short getRefreshRate() {
        return refreshRate;
    }

    public void setRefreshRate(short refreshRate) {
        this.refreshRate = refreshRate;
    }

    public Sensor getSensor() {
        return sensor;
    }

    public void setSensor(Sensor sensor) {
        this.sensor = sensor;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        ActuatorMessage that = (ActuatorMessage) o;

        if (protocol != that.protocol) return false;
        if (adapterId != null ? !adapterId.equals(that.adapterId) : that.adapterId != null) return false;
        if (sensorId != null ? !sensorId.equals(that.sensorId) : that.sensorId != null) return false;

        return true;
    }

    @Override
    public int hashCode() {
        int result = (int) protocol;
        result = 31 * result + (sensorId != null ? sensorId.hashCode() : 0);
        result = 31 * result + (adapterId != null ? adapterId.hashCode() : 0);
        return result;
    }
}
