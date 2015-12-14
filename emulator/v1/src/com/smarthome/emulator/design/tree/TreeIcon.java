package com.smarthome.emulator.design.tree;

/**
 * Created by Shu on 28.5.2014.
 */
public enum TreeIcon {
    //TODO add all values
    //measured values icons
    TEMPERATURE(0,"/img/tree/temperature.png"),
    HUMIDITY(1,"/img/tree/humidity.png"),
    PRESSURE(2,"/img/tree/pressure.png"),
    SENSOR_ACTUATOR(3,"/img/tree/actuator.png"),
    ACTUATOR(4,"/img/tree/actuator.png"),
    LIGHT_INTENSITY(5,"/img/tree/temperature.png"),
    NOISE_INTENSITY(6,"/img/tree/temperature.png"),
    EMISSIONS(7,"/img/tree/temperature.png"),
    //other icons
    PARENT(100,"/img/tree/parent.png"),
    FOLDER(101,"/img/tree/folder.png"),
    SETTINGS(102,"/img/tree/settings.png"),
    SENSOR(103,"/img/tree/sensor.png"),
    UNKNOWN(104,"/img/tree/unknown.png");

    private final int id;
    private final String path;

    TreeIcon(int id, String path){
        this.id = id;
        this.path = path;
    }

    public String getPath(){
        return this.path;
    }

    public int getId(){
        return this.id;
    }
}
