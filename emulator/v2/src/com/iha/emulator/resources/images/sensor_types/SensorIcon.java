package com.iha.emulator.resources.images.sensor_types;

/**
 * Enum providing sensor icons by their name and filename.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
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
    /** icon name */
    final private String name;
    /** icon filename */
    final private String file;

    /**
     * SensorIcon with given name and file
     * @param name icon name
     * @param file icon filename
     */
    SensorIcon(String name,String file) {
        this.name = name;
        this.file = file;
    }

    /**
     * Gets icon name
     * @return icon name
     */
    public String getName() {
        return name;
    }

    /**
     * Gets icon filename
     * @return icon filename
     */
    public String getFile() {
        return file;
    }
    /**
     * Gets icon name
     * @return icon name
     */
    public String toString(){
        return getName();
    }
}
