package com.iha.emulator.communication.protocol;

import com.iha.emulator.communication.protocol.implemented.ZeroPointOne;
import com.iha.emulator.control.AdapterController;
import com.iha.emulator.control.SensorController;
import com.iha.emulator.models.Adapter;
import com.iha.emulator.models.Sensor;
import com.iha.emulator.utilities.AdapterLogger;
import javafx.beans.property.DoubleProperty;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.Element;

/**
 * Created by Shu on 26.11.2014.
 */
public interface Protocol {

    public enum Version{
        //1st parameter= double version value, 2nd parameter = Protocol implementation
        ZERO_POINT_ONE(0.1, ZeroPointOne.class);

        private final double version;
        private final Class versionClass;

        Version(double version, Class versionClass) {
            this.version = version;
            this.versionClass = versionClass;
        }

        public double getVersion(){
            return this.version;
        }

        public Class getVersionClass(){
            return this.versionClass;
        }
        public String toString(){
            return String.valueOf(this.version);
        }
    }
    public double getVersion();
    public void setVersion(double version);
    public DoubleProperty versionProperty();
    public Element buildAdapterMessage(Adapter adapter);
    public Document buildSensorMessage(Element rootElement,Sensor sensor);
    public Document buildRegisterMessage(AdapterController adapterController);
    public Document convertInMessageToXML(String inMessage) throws DocumentException;
    public void parseInSensorMessage(Document inDocument,SensorController senderController, AdapterController adapterController) throws NullPointerException;
    public void parseInAdapterMessage(Document inDocument,AdapterController adapterController) throws NullPointerException;
    public int parseAdapterId(Document inDocument) throws NullPointerException;
    public Document checkProtocolVersion(Document inDocument) throws IllegalArgumentException , NullPointerException;
    public String getHexAdapterId(Adapter adapter);
    public String getHexSensorId(Sensor sensor);
    public String getOffsetAsHexString(int offset);
}
