package com.iha.emulator.communication.protocol;

import com.iha.emulator.control.AdapterController;
import com.iha.emulator.control.SensorController;
import com.iha.emulator.models.Adapter;
import com.iha.emulator.models.Sensor;
import javafx.beans.property.DoubleProperty;
import javafx.beans.property.SimpleDoubleProperty;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.dom4j.Document;
import org.dom4j.Element;

/**
 * Class providing basic methods for interface {@link com.iha.emulator.communication.protocol.Protocol}, that
 * has to be extended when implementing protocol.
 *
 * @see com.iha.emulator.communication.protocol.implemented
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public abstract class AbstractProtocol implements Protocol{
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(AbstractProtocol.class);
    /** protocol version number property */
    private DoubleProperty version;
    /**
     * Creates instance of AbstractProtocol and initializes version property from version number given as parameter
     * @param version initial version property number
     */
    public AbstractProtocol(double version) {
        this.version = new SimpleDoubleProperty(version);
    }
    /**
     * {@inheritDoc}
     */
    public double getVersion() {
        return version.get();
    }
    /**
     * {@inheritDoc}
     */
    public DoubleProperty versionProperty() {
        return version;
    }
    /**
     * {@inheritDoc}
     */
    public void setVersion(double version) {
        this.version.set(version);
    }
    /**
     * {@inheritDoc}
     */
    public String getHexAdapterId(Adapter adapter){
        return "0x" + Integer.toHexString(adapter.getId());
    }
    /**
     * {@inheritDoc}
     */
    public String getHexSensorId(Sensor sensor){
        return "0x" + Integer.toHexString(sensor.getId());
    }
    /**
     * {@inheritDoc}
     */
    public String getOffsetAsHexString(int offset){
        return "0x" + Integer.toHexString(offset);
    }
    /**
     * {@inheritDoc}
     */
    public abstract Element buildAdapterMessage(Adapter adapter);
    /**
     * {@inheritDoc}
     */
    public abstract Document buildSensorMessage(Element rootElement,Sensor sensor);
    /**
     * {@inheritDoc}
     */
    public abstract Document buildRegisterMessage(AdapterController adapterController);
    /**
     * {@inheritDoc}
     */
    public abstract void parseInSensorMessage(Document inDocument, SensorController senderController, AdapterController adapterController) throws IllegalArgumentException,NullPointerException;
    /**
     * {@inheritDoc}
     */
    public abstract void parseInAdapterMessage(Document inDocument, AdapterController adapterController) throws NullPointerException,IllegalArgumentException;
    /**
     * Gets instance of Log4j2 logger for this class
     * @return instance of Log4j2 logger for this class
     */
    public Logger getLogger(){
        return logger;
    }
}
