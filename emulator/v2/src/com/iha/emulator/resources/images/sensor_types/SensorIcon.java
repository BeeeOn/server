package com.iha.emulator.resources.images.sensor_types;

/**
 * Created by Shu on 11.12.2014.
 */
public enum SensorIcon {
    UNKNOWN("Unknown","unknown.png"),
    MULTI_SENSOR("Multisensor","multisensor.png"),
    TEMPERATURE("Temperature","temperature.png"),
    EMISSIONS("Emissions","emissions.png"),
    HUMIDITY("Humidity","humidity.png"),
    LIGHT("Light","light_on.png"),
    LUX("Light intensity","lux.png"),
    NOISE("Noise intensity","noise.png"),
    PRESSURE("Pressure","pressure.png"),
    ACTUATOR("Actuator","actuator.png"),
    WINDOW("Window","win_closed.png");

    final private String name;
    final private String file;

    SensorIcon(String name,String file) {
        this.name = name;
        this.file = file;
    }

    public String getName() {
        return name;
    }

    public String getFile() {
        return file;
    }

    public String toString(){
        return getName();
    }
}
