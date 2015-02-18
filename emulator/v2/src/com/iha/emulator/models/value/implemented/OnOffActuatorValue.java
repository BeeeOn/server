package com.iha.emulator.models.value.implemented;

import com.iha.emulator.models.value.AbstractValue;
import org.dom4j.Element;

import java.util.Random;

/**
 * Created by Shu on 2.12.2014.
 */
public class OnOffActuatorValue extends AbstractValue<Boolean> {


    public OnOffActuatorValue(String name, String type,int offset, String unit, boolean generateValue, boolean storeHistory, Random generator, Long generatorSeed) {
        super(Type.ACTUATOR_ON_OFF,name,type,offset,unit,generateValue,storeHistory,generator,generatorSeed);
        setInitialValue(false);
        setValue(false);
    }

    @Override
    public void nextValue() throws NullPointerException{
        //store value history if needed
        if(isStoreHistory()) storeValue(this.getValue());
        //generate new value
        setValue(!getValue());
    }

    @Override
    public void nextValue(Boolean value) {
        //check value
        if(value == null) throw new NullPointerException("Trying to set value to null");
        //store value history if needed
        if(isStoreHistory()) storeValue(this.getValue());
        this.setValue(value);
    }

    @Override
    public String toStringWithUnit() {
        return String.valueOf(getValue()) + " " + getUnit();
    }

    @Override
    public String asMessageString() {
        return getValue() ? "1" : "0";
    }

    @Override
    public Boolean fromStringToValueType(String valueString) throws NumberFormatException {
        if(valueString.equals("1") || valueString.equals("1.0"))
            return true;
        else if (valueString.equals("0") || valueString.equals("0.0"))
            return false;
        else return Boolean.valueOf(valueString);
    }

    @Override
    public void saveToXML(Element rootElement) {
        Element valueElement = rootElement.addElement("value")
                .addAttribute("type",getValueType().getName())
                .addAttribute("name",getName())
                .addAttribute("store_history",String.valueOf(isStoreHistory()))
                .addAttribute("generate_value",String.valueOf(isGenerateValue()));
        valueElement.addElement("initial_value").addText(String.valueOf(getInitialValue()));
        if(getGeneratorType() != null){
            valueElement.addElement("generator")
                    .addAttribute("type", getGeneratorType().getName())
                    .addAttribute("seed",String.valueOf(getGeneratorSeed()));
        }
    }

    @Override
    public Generator getGeneratorType() {
        return null;
    }

    @Override
    public void setGeneratorType(Generator generatorType) {

    }
}
