package com.iha.emulator.communication.protocol;

/**
 * Class holding information about {@link com.iha.emulator.models.value.Value}, that should be changed,
 * extracted from message sent by server.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 * @see com.iha.emulator.communication.protocol.implemented.ZeroPointOne#parseInAdapterMessage(org.dom4j.Document, com.iha.emulator.control.AdapterController)
 */
public class SetNewValue{
    /** value type field */
    private String type;
    /** value offset field */
    private String offset;
    /** new value field */
    private String value;
    /**
     * Creates new SetNeValue instance with type,offset and value
     *
     * @param type value type as string representing hexadecimal number type ( Example: 0xA0 )
     * @param offset offset of value in sensor/actuator, needed to identify value in sensor/actuator
     * @param value new value to be set
     */
    public SetNewValue(String type,String offset,String value){
        this.type = type;
        this.offset = offset;
        this.value = value;
    }

    /**
     * Gets value's type as string (Example: 0xA0)
     * @return value's type as string
     */
    public String getType() {
        return type;
    }

    /**
     * Gets value's offset in sensor/actuator (Example: 0x01)
     * @return value's offset
     */
    public String getOffset() {
        return offset;
    }

    /**
     * Gets new value to be applied
     * @return new value to be applied
     */
    public String getValue() {
        return value;
    }
}
