package com.iha.emulator.communication.protocol;

import com.iha.emulator.communication.protocol.implemented.ZeroPointOne;
import com.iha.emulator.control.AdapterController;
import com.iha.emulator.control.SensorController;
import com.iha.emulator.models.Adapter;
import com.iha.emulator.models.Sensor;
import javafx.beans.property.DoubleProperty;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.Element;

/**
 * Interface used for creating messages for server and parsing messages from server. Contains enum {@link com.iha.emulator.communication.protocol.Protocol.Version}
 * with already implemented protocol versions. New enum element with protocol version and protocol class has to be added
 * to {@link com.iha.emulator.communication.protocol.Protocol.Version}, when new protocol version is implemented.
 *
 * @see com.iha.emulator.communication.protocol.implemented
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public interface Protocol {
    /**
     * Enum with implemented protocol versions.
     */
    public enum Version{
        //1st parameter= double version value, 2nd parameter = Protocol implementation
        ZERO_POINT_ONE(0.1, ZeroPointOne.class);
        /** protocol version number field */
        private final double version;
        /** protocol implementation class field */
        private final Class versionClass;
        /**
         * Creates new protocol version with given version number and implementation class
         * @param version protocol version number
         * @param versionClass protocol implementation class
         */
        Version(double version, Class versionClass) {
            this.version = version;
            this.versionClass = versionClass;
        }
        /**
         * Gets protocol version number of enum element
         * @return protocol version number
         */
        public double getVersion(){
            return this.version;
        }
        /**
         * Gets protocol implementation class of enum element
         * @return protocol implementation class
         */
        public Class getVersionClass(){
            return this.versionClass;
        }

        /**
         * Protocol version number as string
         * @return protocol version number as string
         */
        public String toString(){
            return String.valueOf(this.version);
        }
    }
    /**
     * Gets protocol version number
     * @return protocol version number
     */
    public double getVersion();
    /**
     * Sets protocol version number
     * @param version protocol version number
     */
    public void setVersion(double version);
    /**
     * Gets protocol version number property, that can be bound
     * @return protocol version number property
     */
    public DoubleProperty versionProperty();
    /**
     * Creates Dom4j XML document and element representing {@link com.iha.emulator.models.Adapter} according to protocol version
     * implemented in {@link com.iha.emulator.communication.protocol.implemented}
     * @param adapter adapter model with adapter information needed in protocol
     * @return Dom4j XML element with adapter information
     */
    public Element buildAdapterMessage(Adapter adapter);
    /**
     * Creates Dom4j XML element representing {@link com.iha.emulator.models.Sensor} according to protocol version
     * implementation in {@link com.iha.emulator.communication.protocol.implemented}. This element is attached to element
     * given as parameter.
     * @param rootElement root element for sensor element
     * @param sensor sensor model with sensor information needed in protocol
     * @return Dom4j XML document with sensor information
     */
    public Document buildSensorMessage(Element rootElement,Sensor sensor);
    /**
     * Creates Dom4j XML document and element representing register message according to protocol version
     * implementation in {@link com.iha.emulator.communication.protocol.implemented}
     * @param adapterController class with information about {@link com.iha.emulator.models.Adapter} and it's state needed
     *                          for creating register message
     * @return Dom4j XML document with register message
     */
    public Document buildRegisterMessage(AdapterController adapterController);
    /**
     * Converts incoming message string to Dom4j document
     * @param inMessage incoming message from server
     * @return Dom4j document with parsed message
     * @throws DocumentException if message cannot be parsed as XML
     */
    public Document convertInMessageToXML(String inMessage) throws DocumentException;
    /**
     * Parses message from server to sensor/actuator according to protocol version implementation
     * in {@link com.iha.emulator.communication.protocol.implemented}. Used for parsing messages
     * in temporary communication thread {@link com.iha.emulator.communication.server.ssl.ServerController}
     * @param inDocument Dom4j XML document containing response message from server
     * @param senderController controller of sensor, for which was message sent
     * @param adapterController controller of adapter, which own sensor
     * @throws IllegalArgumentException defined in implementation class
     * @throws NullPointerException if trying to parse XML element that should be in message, but is not.
     */
    public void parseInSensorMessage(Document inDocument,SensorController senderController, AdapterController adapterController) throws NullPointerException;
    /**
     * Parses message from server to adapter according to protocol version implementation
     * in {@link com.iha.emulator.communication.protocol.implemented}.
     * Used for parsing messages in permanent communication thread {@link com.iha.emulator.communication.server.ssl.ServerReceiver}
     * @param inDocument Dom4j XML document containing message from server
     * @param adapterController controller of adapter, which received message
     * @throws IllegalArgumentException defined in implementation class
     * @throws NullPointerException if trying to parse XML element that should be in message, but is not.
     */
    public void parseInAdapterMessage(Document inDocument,AdapterController adapterController) throws NullPointerException,IllegalArgumentException;
    /**
     * Parses message represented in Dom4j document and compares version number from message with
     * version number of instance used to call this method
     * @param inDocument Dom4j XML document containing message from server
     * @return Dom4j document given as parameter
     * @throws IllegalArgumentException if version numbers are not equal
     * @throws NullPointerException if trying to parse XML element that should be in message, but is not.
     */
    public Document checkProtocolVersion(Document inDocument) throws IllegalArgumentException , NullPointerException;
    /**
     * Returns string containing hexadecimal representation of adapter's ID set in
     * {@link com.iha.emulator.models.Adapter} model. Used in communication with server.
     * @param adapter adapter model containing adapter ID
     * @return string containing hexadecimal representation of adapter's ID
     */
    public String getHexAdapterId(Adapter adapter);
    /**
     * Returns string containing hexadecimal representation of sensor's ID set in
     * {@link com.iha.emulator.models.Sensor} model. Used in communication with server.
     * @param sensor sensor model containing sensor ID
     * @return string containing hexadecimal representation of sensor's ID
     */
    public String getHexSensorId(Sensor sensor);
    /**
     * Returns string containing hexadecimal representation of value's offset. Used in communication with server.
     * @param offset value's offset as integer number
     * @return string containing hexadecimal representation of value's offset
     */
    public String getOffsetAsHexString(int offset);
}
