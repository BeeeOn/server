package com.iha.emulator.models.value;

import com.iha.emulator.models.value.implemented.*;
import javafx.beans.property.StringProperty;
import org.dom4j.Element;

import java.util.Random;

/**
 * Interface providing method declarations needed to implemented by every value. Also contains enum of existing
 * sensors and actuators, as well as enum with possible generator types.
 * @param <T> type of value needed for compilation
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public interface Value<T> {
    /*
    * <?xml version="1.0" encoding="UTF-8"?>
        <server_adapter adapter_id="0x00cafe" fw_version="0.11" protocol_version="1.0" state="table" time="0">
          <table version="1.1" timestamp="1415602654">
            <sensors>
              <value type="0x01" name="humidity"    data_type="int"       unit="%"    transform=""    ></value> OK
              <value type="0x02" name="pressure"    data_type="int"       unit="hPa"  transform=""    ></value> OK
              <value type="0x03" name="open/closed" data_type="bool"      unit=""     transform=""    ></value> OK
              <value type="0x04" name="on/off"      data_type="bool"      unit=""     transform=""    ></value> OK
              <value type="0x05" name="light"       data_type="float"     unit="lx"   transform=""    ></value> OK
              <value type="0x06" name="noise"       data_type="float"     unit="dB"   transform=""    ></value> OK
              <value type="0x07" name="CO2"         data_type="int"       unit="ppm"  transform=""    ></value> OK
              <value type="0x08" name="position"    data_type="bit array" unit=""     transform=""    ></value>
              <value type="0x0A" name="temperature" data_type="float"     unit="°C"   transform="/100"></value> OK
              <value type="0x0B" name="boiler status" data_type="int"       unit=""     transform="">   </value> OK
            </sensors>
            <actors>
              <value type="0xA0" name="on/off"   data_type="bool" unit="" transform="" size="1b"></value>  OK
              <value type="0xA1" name="on-only"  data_type="bool" unit="" transform="" size="1b"></value>
              <value type="0xA2" name="toggle"   data_type="bool" unit="" transform="" size="1b"></value>
              <value type="0xA3" name="range"    data_type="int"  unit="" transform="" size="1B"></value>
              <value type="0xA4" name="RGB"      data_type="int"  unit="" transform="" size="3B"></value>
              <value type="0xA6" name="boiler operation type" data_type="int" unit="" transform="" size="1B"></value> OK
              <value type="0xA7" name="boiler operation mode" data_type="int" unit="" transform="" size="1B"></value> OK
            </actors>
          </table>
        <server_adapter>
    * */

    /**
     * Enum containing all types of implemented values with their default information.
     */
     public enum Type{
        SENSOR_HUMIDITY("Humidity", HumiditySensorValue.class, "%", "0x01", new Generator[]{Generator.NORMAL_DISTRIBUTION, Generator.LINEAR_DISTRIBUTION}),
        SENSOR_PRESSURE("Pressure", PressureSensorValue.class, "hPa", "0x02", new Generator[]{Generator.NORMAL_DISTRIBUTION, Generator.LINEAR_DISTRIBUTION}),
        SENSOR_OPEN_CLOSED("Open/Closed Sensor", OpenClosedSensorValue.class, "", "0x03", new Generator[]{Generator.BOOLEAN_RANDOM}),
        SENSOR_ON_OFF("On/Off Sensor", OnOffActuatorValue.class, "", "0x04", new Generator[]{Generator.BOOLEAN_RANDOM}),
        SENSOR_LIGHT("Light", LightSensorValue.class, "lx", "0x05", new Generator[]{Generator.NORMAL_DISTRIBUTION, Generator.LINEAR_DISTRIBUTION}),
        SENSOR_NOISE("Noise", NoiseSensorValue.class, "dB", "0x06", new Generator[]{Generator.NORMAL_DISTRIBUTION, Generator.LINEAR_DISTRIBUTION}),
        SENSOR_EMISSIONS("Emissions", EmissionsSensorValue.class, "ppm", "0x07", new Generator[]{Generator.NORMAL_DISTRIBUTION, Generator.LINEAR_DISTRIBUTION}),
        SENSOR_TEMPERATURE("Temperature", TemperatureSensorValue.class, "°C", "0x0A", new Generator[]{Generator.NORMAL_DISTRIBUTION, Generator.LINEAR_DISTRIBUTION}),
        SENSOR_BOILER_STATUS("Boiler status", BoilerStatusSensorValue.class, "", "0x0B", new Generator[]{Generator.NORMAL_DISTRIBUTION, Generator.LINEAR_DISTRIBUTION}),
        ACTUATOR_ON_OFF("On/Off Actuator", OnOffActuatorValue.class, "", "0xA0", new Generator[]{Generator.BOOLEAN_RANDOM}),
        ACTUATOR_TEMPERATURE("B.O. temperature", TemperatureActuatorValue.class, "°C", "0xA5", new Generator[]{Generator.NORMAL_DISTRIBUTION, Generator.LINEAR_DISTRIBUTION}),
        ACTUATOR_BOILER_TYPE("B.O. type", BoilerTypeActuatorValue.class, "", "0xA6", new Generator[]{Generator.NORMAL_DISTRIBUTION, Generator.LINEAR_DISTRIBUTION}),
        ACTUATOR_BOILER_MODE("B.O. mode", BoilerModeActuatorValue.class, "", "0xA7", new Generator[]{Generator.NORMAL_DISTRIBUTION, Generator.LINEAR_DISTRIBUTION});
        /** value type name */
        final private String name;
        /** value type class */
        final private Class typeClass;
        /** value type unit */
        final private String unit;
        /** value type used for communication */
        final private String type;
        /** array of generators, that can be used */
        final private Generator[] generators;

        /**
         * Value type with given information as parameters.
         * @param name value type name
         * @param typeClass value type class
         * @param unit value type unit
         * @param type value type used in communication
         * @param generators array of generators, that can be used
         */
        Type(String name, Class typeClass, String unit, String type, Generator[] generators) {
            this.name = name;
            this.typeClass = typeClass;
            this.unit = unit;
            this.type = type;
            this.generators = generators;
        }

        /**
         * Gets value type name
         * @return value type name
         */
        public String getName() {
            return name;
        }

        /**
         * Gets value type class
         * @return value type class
         */
        public Class getTypeClass() {
            return typeClass;
        }

        /**
         * Gets value type unit
         * @return value type unit
         */
        public String getUnit() {
            return unit;
        }

        /**
         * Gets value type used in communication
         * @return value type used in communication
         */
        public String getType() {
            return type;
        }

        /**
         * Gets array of generators, that can be used
         * @return array of generators, that can be used
         */
        public Generator[] getGenerators() {
            return generators;
        }

        /**
         * Returns name of value type
         * @return name of value type
         */
        public String toString(){
            return this.name;
        }
    }

    /**
     * Enum containing description of generators for new values.
     */
    public enum Generator {
        NORMAL_DISTRIBUTION("ND","Normal distribution"),
        LINEAR_DISTRIBUTION("LD","Linear distribution"),
        BOOLEAN_RANDOM("BR","Boolean random");
        /** generator type */
        final private String type;
        /** generator name */
        final private String name;

        /**
         * Generator with type and name given as parameters.
         * @param type generator type
         * @param name generator name
         */
        Generator(String type,String name) {
            this.type = type;
            this.name = name;
        }

        /**
         * Gets generator type
         * @return generator type
         */
        public String getType(){
            return this.type;
        }

        /**
         * Gets generator name
         * @return generator name
         */
        public String getName() {
            return name;
        }

        /**
         * Gets generator name
         * @return generator name
         */
        public String toString(){
            return name;
        }
    }

    /**
     * Gets value's name
     * @return value's name
     */
    public String getName();

    /**
     * Sets value's name
     * @param name value's name
     */
    public void setName(String name);

    /**
     * Gets value with data type set by class implementing {@link com.iha.emulator.models.value.Value} interface
     * @return value with data type set by class implementing {@link com.iha.emulator.models.value.Value} interface
     */
    public T getValue();
    /**
     * Gets initial value with data type set by class implementing {@link com.iha.emulator.models.value.Value} interface
     * @return initial value with data type set by class implementing {@link com.iha.emulator.models.value.Value} interface
     */
    public T getInitialValue();

    /**
     * Sets initial value with data type set by class implementing {@link com.iha.emulator.models.value.Value} interface
     * @param value initial value with data type set by class implementing {@link com.iha.emulator.models.value.Value} interface
     */
    public void setInitialValue(T value);

    /**
     * Sets value with data type set by class implementing {@link com.iha.emulator.models.value.Value} interface
     * @param value value with data type set by class implementing {@link com.iha.emulator.models.value.Value} interface
     */
    public void setValue(T value);

    /**
     * Method generates new value and returns it. If store history flag is <code>true</code>, old value is saved.
     * @return new generated value
     */
    public T nextValue();

    /**
     * Gets generator instance
     * @return generator instance
     */
    public Random getGenerator();

    /**
     * Returns string composed of value and it's unit
     * @return string composed of value and it's unit
     */
    public String toStringWithUnit();

    /**
     * Gets value as string used in XML communication
     * @return value as string used in XML communication
     */
    public String asMessageString();

    /**
     * Gets value as string
     * @return value as string
     */
    public String getStringValue();

    /**
     * Gets value as string property, that can be bound
     * @return value as string property
     */
    public StringProperty stringValueProperty();

    /**
     * Sets value from string
     * @param stringValue value from string
     */
    public void setStringValue(String stringValue);

    /**
     * Gets value type from enum {@link com.iha.emulator.models.value.Value.Type}
     * @return value type from enum {@link com.iha.emulator.models.value.Value.Type}
     */
    public Type getValueType();

    /**
     * Sets value type from enum {@link com.iha.emulator.models.value.Value.Type}
     * @param type value type from enum {@link com.iha.emulator.models.value.Value.Type}
     */
    public void setValueType(Type type);

    /**
     * Gets store history flag
     * @return <code>true</code> if history is stored, <code>false</code> otherwise
     */
    public boolean isStoreHistory();

    /**
     * Sets store history flag
     * @param storeHistory <code>true</code> if history should be stored, <code>false</code> otherwise
     */
    public void setStoreHistory(boolean storeHistory);

    /**
     * Gets flag, if new value should is generated
     * @return <code>true</code> if new value is generated, <code>false</code> otherwise
     */
    public boolean isGenerateValue();

    /**
     * Sets flag, if new value should be generated
     * @param generateValue <code>true</code> if new value should be generated, <code>false</code> otherwise
     */
    public void setGenerateValue(boolean generateValue);

    /**
     * Method converts string to value data type and returns it
     * @param valueString string containing value
     * @return value
     * @throws NumberFormatException cannot convert string to number
     */
    public T fromStringToValueType(String valueString) throws NumberFormatException;

    /**
     * Gets value's offset in sensor
     * @return value's offset in sensor
     */
    public int getOffset();

    /**
     * Sets value's offset in sensor
     * @param offset value's offset in sensor
     */
    public void setOffset(int offset);

    /**
     * Creates Dom4j XML element, which is added to rootElement given as parameter. Saves all information about value
     * needed for later loading of this value
     * @param rootElement root element for value's element
     */
    public void saveToXML(Element rootElement);

    /**
     * Gets seed of value generator
     * @return seed of value generator
     */
    public long getGeneratorSeed();

    /**
     * Sets seed of value generator
     * @param generatorSeed seed of value generator
     */
    public void setGeneratorSeed(long generatorSeed);
}
