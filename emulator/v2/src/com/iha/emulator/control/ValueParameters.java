package com.iha.emulator.control;

import com.iha.emulator.models.value.*;
import com.iha.emulator.models.value.implemented.HasGenerator;
import javafx.collections.ObservableList;

/**
 * Created by Shu on 3.3.2015.
 */
public class ValueParameters {

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

    private Value copyDeepFoundValue(Value foundValue){
        Value newValue = ValueFactory.buildValue(foundValue.getValueType());
        if(((HasGenerator)foundValue).getGeneratorType() == null)
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
        }
        //set other value parameters
        newValue.setValue(foundValue.getValue());
        newValue.setInitialValue(foundValue.getInitialValue());
        newValue.setGeneratorSeed(foundValue.getGeneratorSeed());
        newValue.setGenerateValue(foundValue.isGenerateValue());
        newValue.setStoreHistory(foundValue.isStoreHistory());
        newValue.setName(foundValue.getName());
        return newValue;
    }

    public ObservableList<Value> getEnabledValues() {
        return enabledValues;
    }

    public void setEnabledValues(ObservableList<Value> enabledValues) {
        this.enabledValues = enabledValues;
    }
}
