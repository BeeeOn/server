package com.iha.emulator.resources.images.sensor_types;

/**
 * Class providing method to get {@link com.iha.emulator.resources.images.sensor_types.SensorIcon} members, that hold
 * information about icons, that can be used.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class SensorIconFactory {
    /**
     * Returns {@link com.iha.emulator.resources.images.sensor_types.SensorIcon} enum member by icon name.
     * @param name name of icon
     * @return member of {@link com.iha.emulator.resources.images.sensor_types.SensorIcon} enum
     */
    public static SensorIcon getByName(String name){
        for(SensorIcon icon : SensorIcon.values()){
            if(icon.getName().equalsIgnoreCase(name))
                return icon;
        }
        return SensorIcon.UNKNOWN;
    }
}
