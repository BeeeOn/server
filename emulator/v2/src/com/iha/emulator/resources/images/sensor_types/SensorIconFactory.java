package com.iha.emulator.resources.images.sensor_types;

/**
 * Created by Shu on 16.2.2015.
 */
public class SensorIconFactory {

    public static SensorIcon getByName(String name){
        for(SensorIcon icon : SensorIcon.values()){
            if(icon.getName().equalsIgnoreCase(name))
                return icon;
        }
        return SensorIcon.UNKNOWN;
    }
}
