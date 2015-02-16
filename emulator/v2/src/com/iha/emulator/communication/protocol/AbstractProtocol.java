package com.iha.emulator.communication.protocol;

import com.iha.emulator.control.AdapterController;
import com.iha.emulator.control.SensorController;
import com.iha.emulator.models.Adapter;
import com.iha.emulator.models.Sensor;
import com.iha.emulator.utilities.AdapterLogger;
import javafx.beans.property.DoubleProperty;
import javafx.beans.property.SimpleDoubleProperty;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.dom4j.Document;
import org.dom4j.Element;

/**
 * Created by Shu on 26.11.2014.
 */
public abstract class AbstractProtocol implements Protocol{

    private static final Logger logger = LogManager.getLogger(AbstractProtocol.class);

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

    public String getHexAdapterId(Adapter adapter){
        return "0x" + Integer.toHexString(adapter.getId());
    }

    public String getHexSensorId(Sensor sensor){
        return "0x" + Integer.toHexString(sensor.getId());
    }

    public String getOffsetAsHexString(int offset){
        return "0x" + Integer.toHexString(offset);
    }

    public Logger getLogger(){
        return logger;
    }

    public abstract Element buildAdapterMessage(Adapter adapter);
    public abstract Document buildSensorMessage(Element rootElement,Sensor sensor);
    public abstract Document buildRegisterMessage(AdapterController adapterController);

    public abstract void parseInSensorMessage(Document inDocument, SensorController senderController, AdapterController adapterController) throws NullPointerException;
    public abstract int parseAdapterId(Document inDocument) throws NullPointerException;
}
