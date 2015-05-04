package com.iha.emulator.models.value;

import com.iha.emulator.models.value.implemented.*;
import javafx.beans.property.StringProperty;
import org.dom4j.Element;

import java.util.Random;

/**
 * Created by Shu on 2.12.2014.
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

        final private String name;
        final private Class typeClass;
        final private String unit;
        final private String type;
        final private Generator[] generators;

        Type(String name, Class typeClass, String unit, String type, Generator[] generators) {
            this.name = name;
            this.typeClass = typeClass;
            this.unit = unit;
            this.type = type;
            this.generators = generators;
        }

        public String getName() {
            return name;
        }

        public Class getTypeClass() {
            return typeClass;
        }

        public String getUnit() {
            return unit;
        }

        public String getType() {
            return type;
        }

        public Generator[] getGenerators() {
            return generators;
        }

        public String toString(){
            return this.name;
        }
    }

    public enum Generator {
        NORMAL_DISTRIBUTION("ND","Normal distribution"),
        LINEAR_DISTRIBUTION("LD","Linear distribution"),
        BOOLEAN_RANDOM("BR","Boolean random");

        final private String type;
        final private String name;

        Generator(String type,String name) {
            this.type = type;
            this.name = name;
        }

        public String getType(){
            return this.type;
        }

        public String getName() {
            return name;
        }

        public String toString(){
            return name;
        }
    }

    public String getName();
    public void setName(String name);
    public T getValue();
    public T getInitialValue();
    public void setInitialValue(T value);
    public void setValue(T value);
    public T nextValue();
    public Random getGenerator();
    public String toStringWithUnit();
    public String asMessageString();
    public String getStringValue();
    public StringProperty stringValueProperty();
    public void setStringValue(String stringValue);
    public Type getValueType();
    public void setValueType(Type type);
    public boolean isStoreHistory();
    public void setStoreHistory(boolean storeHistory);
    public boolean isGenerateValue();
    public void setGenerateValue(boolean generateValue);
    public T fromStringToValueType(String valueString) throws NumberFormatException;
    public int getOffset();
    public void setOffset(int offset);
    public void saveToXML(Element rootElement);
    public long getGeneratorSeed();
    public void setGeneratorSeed(long generatorSeed);
}
