package com.iha.emulator.control.task;

import com.iha.emulator.models.value.Value;
import com.iha.emulator.models.value.ValueFactory;
import javafx.collections.ObservableList;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.dom4j.Element;

/**
 * Created by Shu on 3.3.2015.
 */
public class ValueParameters {

    private static final Logger logger = LogManager.getLogger(ValueParameters.class);
    private ObservableList<Value> enabledValues;

    public Value isValueEnabled(int valueIndex) throws IllegalArgumentException{
        if(enabledValues == null || enabledValues.size() == 0)
            throw new IllegalArgumentException("No enabled values(possible null).");
        for(Value value : enabledValues){
            if(valueIndex == value.getValueType().ordinal()){
                return copyDeepFoundValue(value);
            }
        }
        return null;
    }

    public Value getEnabledValue(int valueIndex){
        return copyDeepFoundValue(enabledValues.get(valueIndex));
    }

    private Value copyDeepFoundValue(Value foundValue){
        Value newValue = ValueFactory.buildValue(foundValue.getValueType());
        /*if(((HasGenerator)foundValue).getGeneratorType() == null)
            return newValue;
        if(foundValue.isGenerateValue() && ((HasGenerator)foundValue).getGeneratorType() != null){
            //set generator type
            ((HasGenerator)newValue).setGeneratorType(((HasGenerator)foundValue).getGeneratorType());
            //set parameters for generator
            switch (((HasGenerator)foundValue).getGeneratorType()){
                case NORMAL_DISTRIBUTION:
                    ((HasNormalDistribution)newValue).setMin(((HasNormalDistribution) foundValue).getMin());
                    ((HasNormalDistribution)newValue).setMax(((HasNormalDistribution) foundValue).getMax());
                    ((HasNormalDistribution)newValue).setDev(((HasNormalDistribution) foundValue).getDev());
                    ((HasNormalDistribution)newValue).setAvg(((HasNormalDistribution) foundValue).getAvg());
                    break;
                case LINEAR_DISTRIBUTION:
                    ((HasLinearDistribution)newValue).setMin(((HasLinearDistribution) foundValue).getMin());
                    ((HasLinearDistribution)newValue).setMax(((HasLinearDistribution) foundValue).getMax());
                    ((HasLinearDistribution)newValue).setStep(((HasLinearDistribution) foundValue).getStep());
                    break;
                case BOOLEAN_RANDOM:
                    ((HasBooleanRandom)newValue).setProbability(((HasBooleanRandom) foundValue).getProbability());
                    break;
            }
        }*/
        //set other value parameters
        newValue.setValue(foundValue.getValue());
        newValue.setInitialValue(foundValue.getInitialValue());
        //newValue.setGeneratorSeed(foundValue.getGeneratorSeed());
        newValue.setGenerateValue(false);
        newValue.setStoreHistory(false);
        newValue.setName(foundValue.getName());
        return newValue;
    }

    public Element saveToXml(Element rootElement){
        Element enabledElement = rootElement.addElement("enabled_values");
        String valueString = "";
        for(Value v : enabledValues){
            valueString = valueString + (valueString.equals("")?"":",") + v.getValueType().getName();
        }
        enabledElement.setText(valueString);
        return enabledElement;
    }

    public ObservableList<Value> getEnabledValues() {
        return enabledValues;
    }

    public void setEnabledValues(ObservableList<Value> enabledValues) {
        this.enabledValues = enabledValues;
    }
}
