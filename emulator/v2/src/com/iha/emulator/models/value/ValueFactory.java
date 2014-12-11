package com.iha.emulator.models.value;

import com.iha.emulator.models.SensorType;
import com.iha.emulator.models.value.implemented.HumiditySensorValue;
import com.iha.emulator.models.value.implemented.OnOffActuatorValue;
import com.iha.emulator.models.value.implemented.TemperatureSensorValue;

import java.util.Random;

/**
 * Created by Shu on 2.12.2014.
 */
public class ValueFactory {

    public static <T extends Value> T  buildValue(SensorType valueType, String name, boolean generateValue, boolean storeHistory, Long generatorSeed){
        Value value = null;
        switch (valueType){
            case SENSOR_TEMPERATURE:
                value = new TemperatureSensorValue(name,"0x0A","°C",generateValue,storeHistory,new Random(),generatorSeed);
                break;
            case SENSOR_HUMIDITY:
                value = new HumiditySensorValue(name,"0x01","%",generateValue,storeHistory,new Random(),generatorSeed);
                break;
            case ACTUATOR_ON_OFF:
                value = new OnOffActuatorValue(name,"0xA0","",generateValue,storeHistory,new Random(),generatorSeed);
                break;
        }
        return (T)value;
    }
}
