package com.iha.emulator.models.value;

import com.iha.emulator.models.value.implemented.*;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.Random;

import static com.iha.emulator.models.value.Value.Type;

/**
 * Class used to create new instances of values.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class ValueFactory {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(ValueFactory.class);

    /**
     * Returns new instance of value from given value type name in {@link com.iha.emulator.models.value.Value.Type}
     * @param valueTypeName value type name
     * @param <T> value used in application compilation
     * @return new instance of class extending {@link com.iha.emulator.models.value.Value}, null if type not found
     */
    public static <T extends Value> T  buildValue(String valueTypeName){
        for(Type name : Value.Type.values()){
            if(name.getName().equals(valueTypeName)){
                return buildValue(name,false,false,null);
            }
        }
        return null;
    }

    /**
     * Returns new instance of value from given value {@link com.iha.emulator.models.value.Value.Type}
     * @param valueType type of value
     * @param <T> value used in application compilation
     * @return new instance of class extending {@link com.iha.emulator.models.value.Value}, null if type not found
     */
    public static <T extends Value> T  buildValue(Type valueType){
        return buildValue(valueType,false,false,null);
    }

    /**
     * Returns new instance of value from given value {@link com.iha.emulator.models.value.Value.Type} and sets it's
     * variables from given parameters
     * @param valueType type of value
     * @param generateValue generate value flag
     * @param storeHistory store history flag
     * @param generatorSeed seed for new value generator
     * @param <T> value used in application compilation
     * @return new instance of class extending {@link com.iha.emulator.models.value.Value}, null if type not found
     */
    public static <T extends Value> T  buildValue(Type valueType, boolean generateValue, boolean storeHistory, Long generatorSeed){
        Value value = null;
        switch (valueType){
            case SENSOR_TEMPERATURE:
                value = new TemperatureSensorValue(valueType.getName(),valueType.getType(),0,valueType.getUnit(),generateValue,storeHistory,new Random(),generatorSeed);
                break;
            case SENSOR_NOISE:
                value = new NoiseSensorValue(valueType.getName(),valueType.getType(),0,valueType.getUnit(),generateValue,storeHistory,new Random(),generatorSeed);
                break;
            case SENSOR_LIGHT:
                value = new LightSensorValue(valueType.getName(),valueType.getType(),0,valueType.getUnit(),generateValue,storeHistory,new Random(),generatorSeed);
                break;
            case SENSOR_HUMIDITY:
                value = new HumiditySensorValue(valueType.getName(),valueType.getType(),0,valueType.getUnit(),generateValue,storeHistory,new Random(),generatorSeed);
                break;
            case SENSOR_PRESSURE:
                value = new PressureSensorValue(valueType.getName(),valueType.getType(),0,valueType.getUnit(),generateValue,storeHistory,new Random(),generatorSeed);
                break;
            case SENSOR_EMISSIONS:
                value = new EmissionsSensorValue(valueType.getName(),valueType.getType(),0,valueType.getUnit(),generateValue,storeHistory,new Random(),generatorSeed);
                break;
            case SENSOR_ON_OFF:
                value = new OnOffSensorValue(valueType.getName(),valueType.getType(),0,valueType.getUnit(),generateValue,storeHistory,new Random(),generatorSeed);
                break;
            case SENSOR_OPEN_CLOSED:
                value = new OpenClosedSensorValue(valueType.getName(),valueType.getType(),0,valueType.getUnit(),generateValue,storeHistory,new Random(),generatorSeed);
                break;
            case SENSOR_BOILER_STATUS:
                logger.debug("Creating new Sensor Boiler Status -> type: " + valueType.getType());
                value = new BoilerStatusSensorValue(valueType.getName(),valueType.getType(),0,valueType.getUnit(),generateValue,storeHistory,new Random(),generatorSeed);
                break;
            case ACTUATOR_ON_OFF:
                value = new OnOffActuatorValue(valueType.getName(),valueType.getType(),0,valueType.getUnit(),generateValue,storeHistory,new Random(),generatorSeed);
                break;
            case ACTUATOR_TEMPERATURE:
                logger.debug("Creating new Actuator Temperature -> type: " + valueType.getType());
                value = new TemperatureActuatorValue(valueType.getName(),valueType.getType(),0,valueType.getUnit(),generateValue,storeHistory,new Random(),generatorSeed);
                break;
            case ACTUATOR_BOILER_TYPE:
                logger.debug("Creating new Actuator Boiler Type -> type: " + valueType.getType());
                value = new BoilerTypeActuatorValue(valueType.getName(),valueType.getType(),0,valueType.getUnit(),generateValue,storeHistory,new Random(),generatorSeed);
                break;
            case ACTUATOR_BOILER_MODE:
                logger.debug("Creating new Actuator Boiler Mode -> type: " + valueType.getType());
                value = new BoilerModeActuatorValue(valueType.getName(),valueType.getType(),0,valueType.getUnit(),generateValue,storeHistory,new Random(),generatorSeed);
                break;
        }
        return (T)value;
    }

    /**
     * Returns {@link com.iha.emulator.models.value.Value.Generator} type found by name given as parameter.
     * @param generatorName name of generator from {@link com.iha.emulator.models.value.Value.Generator}
     * @return {@link com.iha.emulator.models.value.Value.Generator} type found by name given as parameter, null
     * if generator wasn't found
     */
    public static Value.Generator generatorByName(String generatorName){
        for(Value.Generator gen : Value.Generator.values()){
            if(gen.getType().equals(generatorName))
                return gen;
        }
        return null;
    }
}
