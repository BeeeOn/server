package com.iha.emulator.communication.protocol;

/**
 * Created by Shu on 12.2.2015.
 */
public class SetNewValue{

    private String type;
    private String offset;
    private String value;

    public SetNewValue(String type,String offset,String value){
        this.type = type;
        this.offset = offset;
        this.value = value;
    }

    public String getType() {
        return type;
    }

    public String getOffset() {
        return offset;
    }

    public String getValue() {
        return value;
    }
}
