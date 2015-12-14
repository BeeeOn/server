package com.iha.emulator.control.task;

import com.iha.emulator.models.value.Value;
import com.iha.emulator.models.value.ValueFactory;
import javafx.collections.ObservableList;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.dom4j.Element;

/**
 * Information about values, that are enabled. Used in "Performance Simulation". When creating {@link com.iha.emulator.control.task.SimulationTask}
 * random values are created. Each simulation task can have own enabled values.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class ValueParameters {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(ValueParameters.class);
    /** list of enabled values */
    private ObservableList<Value> enabledValues;

    /**
     * Returns new instance of value, if given value index is in enabled values list.
     * @param valueIndex index of value, that is to be created. This index is number of value in {@link com.iha.emulator.models.value.Value.Type}
     * @return new instance of value or null if given value index is not enabled.
     */
    public Value getEnabledValue(int valueIndex){
        return copyDeepFoundValue(enabledValues.get(valueIndex));
    }

    /**
     * Creates new instance of same type as value given as parameter. New value will have same value,initial value and name
     * as given value. Store history and generate value flags are automatically set to <code>false</code>.
     * @param foundValue value to be copied
     * @return deep copy of given value
     */
    private Value copyDeepFoundValue(Value foundValue){
        //create new value of same type as given value
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
        //copy value
        newValue.setValue(foundValue.getValue());
        //copy initial value
        newValue.setInitialValue(foundValue.getInitialValue());
        //set flags
        newValue.setGenerateValue(false);
        newValue.setStoreHistory(false);
        //copy name
        newValue.setName(foundValue.getName());
        return newValue;
    }
    /**
     * Creates Dom4j XML element with information about enabled values.
     * This element is added to root element given as parameter.
     *
     * @param rootElement root Dom4j XML element for server
     * @return XML element
     */
    public Element saveToXml(Element rootElement){
        Element enabledElement = rootElement.addElement("enabled_values");
        String valueString = "";
        for(Value v : enabledValues){
            valueString = valueString + (valueString.equals("")?"":",") + v.getValueType().getName();
        }
        enabledElement.setText(valueString);
        return enabledElement;
    }
    /**
     * Gets list of enabled values
     * @return list of enabled values
     */
    public ObservableList<Value> getEnabledValues() {
        return enabledValues;
    }

    /**
     * Sets list of enabled values
     * @param enabledValues list of enabled values
     */
    public void setEnabledValues(ObservableList<Value> enabledValues) {
        this.enabledValues = enabledValues;
    }
}
