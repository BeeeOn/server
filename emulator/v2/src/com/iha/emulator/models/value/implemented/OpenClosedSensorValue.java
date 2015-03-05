package com.iha.emulator.models.value.implemented;

import com.iha.emulator.models.value.AbstractValue;
import com.iha.emulator.models.value.HasBooleanRandom;
import com.iha.emulator.utilities.Utilities;
import javafx.beans.property.DoubleProperty;
import javafx.beans.property.SimpleDoubleProperty;
import org.dom4j.Element;

import java.util.Random;

/**
 * Created by Shu on 2.12.2014.
 */
public class OpenClosedSensorValue extends AbstractValue<Boolean> implements HasBooleanRandom {

    private static final double DEFAULT_PROBABILITY = 0.25;

    private DoubleProperty probability;

    public OpenClosedSensorValue(String name, String type, int offset, String unit, boolean generateValue, boolean storeHistory, Random generator, Long generatorSeed) {
        super(Type.SENSOR_OPEN_CLOSED,name,type,offset,unit,generateValue,storeHistory,generator,generatorSeed);
        probability = new SimpleDoubleProperty(DEFAULT_PROBABILITY);
        setInitialValue(false);
        setValue(false);
    }

    @Override
    public Boolean nextValue() throws IllegalArgumentException{
        //store value history if needed
        if(isStoreHistory()) storeValue(this.getValue());
        //generate new value
        if(getGeneratorType() == null || !isGenerateValue()) return null;
        switch (getGeneratorType()){
            case BOOLEAN_RANDOM:
                if(probabilityProperty() == null){
                    throw new IllegalArgumentException("Open/Closed sensor generator doesn't have variables needed to generate new value (Probability)");
                }
                return Utilities.booleanRandomGenerate(getValue(),getProbability(),getGenerator());
        }
        return null;
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
            Element generatorElement = valueElement.addElement("generator")
                    .addAttribute("type", getGeneratorType().getType())
                    .addAttribute("seed",String.valueOf(getGeneratorSeed()));
            switch (getGeneratorType()){
                case BOOLEAN_RANDOM:
                    generatorElement.addElement("params")
                            .addAttribute("probability", String.valueOf(getProbability()));
                    break;
            }
        }
    }

    @Override
    public void restartGenerator() {
        setGenerator(new Random());
        setGeneratorSeed(getGeneratorSeed());
    }


    @Override
    public double getProbability() {
        return probability.get();
    }

    public DoubleProperty probabilityProperty() {
        return probability;
    }

    @Override
    public void setProbability(double probability) {
        this.probability.set(probability);
    }
}
