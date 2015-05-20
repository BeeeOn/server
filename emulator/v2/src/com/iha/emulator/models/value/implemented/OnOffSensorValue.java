package com.iha.emulator.models.value.implemented;

import com.iha.emulator.models.value.AbstractValue;
import com.iha.emulator.models.value.HasBooleanRandom;
import com.iha.emulator.utilities.Utilities;
import javafx.beans.property.DoubleProperty;
import javafx.beans.property.SimpleDoubleProperty;
import org.dom4j.Element;

import java.util.Random;

/**
 * Implementation of {@link com.iha.emulator.models.value.Value} interface, extending {@link com.iha.emulator.models.value.AbstractValue}
 * representing On/Off sensor. Uses Boolean as value data type. Provides methods for storing, generating and saving value.
 * For generating new values uses Boolean random generator.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class OnOffSensorValue extends AbstractValue<Boolean> implements HasBooleanRandom {
    /** default probability for boolean random generator */
    private static final double DEFAULT_PROBABILITY = 0.25;
    /** boolean random generator probability property */
    private DoubleProperty probability;
    /**
     * Creates and initializes new instance of On/Off sensor from given information as parameter.
     * @param name value name
     * @param type value type used in XML communication
     * @param offset value offset in sensor
     * @param unit value unit
     * @param generateValue should be new value generated?
     * @param storeHistory should be value history stored?
     * @param generator generator for new values
     * @param generatorSeed seed for new values generator
     */
    public OnOffSensorValue(String name, String type, int offset, String unit, boolean generateValue, boolean storeHistory, Random generator, Long generatorSeed) {
        super(Type.SENSOR_ON_OFF,name,type,offset,unit,generateValue,storeHistory,generator,generatorSeed);
        probability = new SimpleDoubleProperty(DEFAULT_PROBABILITY);
        setInitialValue(false);
        setValue(false);
    }
    /**
     * {@inheritDoc}
     *
     * Returns boolean. Uses boolean random generator.
     *
     * @throws IllegalArgumentException if variables needed for generator are missing or generator cannot generate
     * new value. Check exception message for details.
     */
    @Override
    public Boolean nextValue() throws IllegalArgumentException{
        //store value history if needed
        if(isStoreHistory()) storeValue(this.getValue());
        //generate new value
        if(getGeneratorType() == null || !isGenerateValue()) return null;
        switch (getGeneratorType()){
            case BOOLEAN_RANDOM:
                if(probabilityProperty() == null){
                    throw new IllegalArgumentException("On/Off Sensor generator doesn't have variables needed to generate new value (Probability)");
                }
                return Utilities.booleanRandomGenerate(getValue(),getProbability(),getGenerator());
        }
        return null;
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public String toStringWithUnit() {
        return String.valueOf(getValue()) + " " + getUnit();
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public String asMessageString() {
        return getValue() ? "1" : "0";
    }
    /**
     * {@inheritDoc}
     *
     * Value string must be one of these options: 1,1.0,true,TRUE,0,0.0,false,FALSE
     * Returns boolean.
     */
    @Override
    public Boolean fromStringToValueType(String valueString) throws NumberFormatException {
        if(valueString.equals("1") || valueString.equals("1.0"))
            return true;
        else if (valueString.equals("0") || valueString.equals("0.0"))
            return false;
        else return Boolean.valueOf(valueString);
    }
    /**
     * {@inheritDoc}
     * @param rootElement root element for value's element
     */
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
    /**
     * {@inheritDoc}
     */
    @Override
    public void restartGenerator() {
        setGenerator(new Random());
        setGeneratorSeed(getGeneratorSeed());
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public double getProbability() {
        return probability.get();
    }
    /**
     * Gets probability of random boolean change property,that can be bound
     * @return probability of random boolean change property
     */
    public DoubleProperty probabilityProperty() {
        return probability;
    }
    /**
     * {@inheritDoc}
     * @param probability probability of random boolean change
     */
    @Override
    public void setProbability(double probability) {
        this.probability.set(probability);
    }
}
