package com.iha.emulator.models.value;

import com.iha.emulator.models.value.implemented.*;

import java.util.Random;

import static com.iha.emulator.models.value.Value.Type;

/**
 * Created by Shu on 2.12.2014.
 */
public class ValueFactory {

    public static <T extends Value> T  buildValue(String valueTypeName){
        for(Type name : Value.Type.values()){
            if(name.getName().equals(valueTypeName)){
                return buildValue(name,false,false,null);
            }
        }
        return null;
    }

    public static <T extends Value> T  buildValue(Type valueType){
        return buildValue(valueType,false,false,null);
    }

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
            case ACTUATOR_ON_OFF:
                value = new OnOffActuatorValue(valueType.getName(),valueType.getType(),0,valueType.getUnit(),generateValue,storeHistory,new Random(),generatorSeed);
                break;
        }
        return (T)value;
    }

    public static Value.Generator generatorByName(String generatorName){
        for(Value.Generator gen : Value.Generator.values()){
            if(gen.getType().equals(generatorName))
                return gen;
        }
        return null;
    }
}
