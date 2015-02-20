package com.iha.emulator.models.value;

import com.iha.emulator.models.value.implemented.*;
import javafx.beans.property.StringProperty;
import org.dom4j.Element;

import java.util.Random;

/**
 * Created by Shu on 2.12.2014.
 */
public interface Value<T> {

    public enum Type{
        SENSOR_TEMPERATURE("Temperature", TemperatureSensorValue.class, "°C", "0x0A", new Generator[]{Generator.NORMAL_DISTRIBUTION, Generator.LINEAR_DISTRIBUTION}),
        SENSOR_NOISE("Noise", NoiseSensorValue.class, "dB", "0x06", new Generator[]{Generator.NORMAL_DISTRIBUTION, Generator.LINEAR_DISTRIBUTION}),
        SENSOR_LIGHT("Light", LightSensorValue.class, "lx", "0x05", new Generator[]{Generator.NORMAL_DISTRIBUTION, Generator.LINEAR_DISTRIBUTION}),
        SENSOR_EMISSIONS("Emissions", EmissionsSensorValue.class, "ppm", "0x07", new Generator[]{Generator.NORMAL_DISTRIBUTION, Generator.LINEAR_DISTRIBUTION}),
        SENSOR_PRESSURE("Pressure", PressureSensorValue.class, "hPa", "0x02", new Generator[]{Generator.NORMAL_DISTRIBUTION, Generator.LINEAR_DISTRIBUTION}),
        SENSOR_HUMIDITY("Humidity", HumiditySensorValue.class, "%", "0x01", new Generator[]{Generator.NORMAL_DISTRIBUTION, Generator.LINEAR_DISTRIBUTION}),
        ACTUATOR_ON_OFF("On/Off Actuator", OnOffActuatorValue.class, "", "0xA0", new Generator[]{Generator.BOOLEAN_RANDOM});

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
    public void nextValue();
    public void nextValue(T value);
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
