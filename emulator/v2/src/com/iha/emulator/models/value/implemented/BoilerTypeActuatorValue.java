package com.iha.emulator.models.value.implemented;

import com.iha.emulator.models.value.*;
import com.iha.emulator.utilities.Utilities;
import javafx.beans.property.DoubleProperty;
import javafx.beans.property.SimpleDoubleProperty;
import org.dom4j.Element;

import java.util.Random;

/**
 * Implementation of {@link com.iha.emulator.models.value.Value} interface, extending {@link com.iha.emulator.models.value.AbstractValue}
 * representing Boiler Type actuator. Uses Integer as value data type. Provides methods for storing, generating and saving value.
 * For generating new values uses both Normal and Linear distribution. All possible value provided by
 * {@link com.iha.emulator.models.value.implemented.BoilerTypeActuatorValue.BoilerOperationType} enum.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class BoilerTypeActuatorValue extends AbstractValue<Integer> implements HasNormalDistribution,HasLinearDistribution,VPTRegulatorValue {
    /**
     * Enum implementing {@link com.iha.emulator.models.value.Status} interface. Provides all possible values with
     * their description.
     */
    public enum BoilerOperationType implements Status {
        OFF(0,"Off"),
        ROOM_CONTROLLER(1,"Room controller"),
        EQUITHERMIC_REGULATION(2,"Equithermic regulation"),
        CONSTANT_WATER_TEMP(3,"Constant water temperature regulation"),
        SHUT_DOWN(4,"Heating TUV");
        /** value code */
        private int code;
        /** value description */
        private String description;
        /**
         * Value with given code and description
         * @param code value code
         * @param description value description
         */
        BoilerOperationType(int code,String description){
            this.code = code;
            this.description = description;
        }
        /**
         * Gets value code
         * @return value code
         */
        public int getCode() {
            return code;
        }
        /**
         * Gets value description
         * @return value description
         */
        public String getDescription() {
            return description;
        }
    }
    /** default value */
    private static final int DEFAULT_VALUE = 1;
    /** default distribution maximal value */
    private static final double DEFAULT_MAX = 4;
    /** default distribution minimal value */
    private static final double DEFAULT_MIN = 0;
    /** default normal distribution deviation */
    private static final double DEFAULT_NORMAL_DEV = 1;
    /** default normal distribution average */
    private static final double DEFAULT_NORMAL_AVG = 2;
    /** default linear distribution step */
    private static final double DEFAULT_LINEAR_STEP = 1;
    /** normal distribution deviation property */
    private DoubleProperty dev;
    /** normal distribution average property */
    private DoubleProperty avg;
    /** distribution minimal value property */
    private DoubleProperty min;
    /** distribution maximal value property */
    private DoubleProperty max;
    /** linear distribution step property */
    private DoubleProperty step;
    /**
     * Creates and initializes new instance of Boiler Type actuator from given information as parameter.
     * @param name value name
     * @param type value type used in XML communication
     * @param offset value offset in sensor
     * @param unit value unit
     * @param generateValue should be new value generated?
     * @param storeHistory should be value history stored?
     * @param generator generator for new values
     * @param generatorSeed seed for new values generator
     */
    public BoilerTypeActuatorValue(String name, String type, int offset, String unit, boolean generateValue, boolean storeHistory, Random generator, Long generatorSeed) {
        super(Type.ACTUATOR_BOILER_TYPE,name,type,offset,unit,generateValue,storeHistory,generator,generatorSeed);
        this.dev = new SimpleDoubleProperty(DEFAULT_NORMAL_DEV);
        this.avg = new SimpleDoubleProperty(DEFAULT_NORMAL_AVG);
        this.min = new SimpleDoubleProperty(DEFAULT_MIN);
        this.max= new SimpleDoubleProperty(DEFAULT_MAX);
        this.step = new SimpleDoubleProperty(DEFAULT_LINEAR_STEP);
        setInitialValue(DEFAULT_VALUE);
        setValue(DEFAULT_VALUE);
    }
    /**
     * {@inheritDoc}
     *
     * Returns integer. Uses normal and linear distribution.
     *
     * @throws IllegalArgumentException if variables needed for generator are missing or generator cannot generate
     * new value. Check exception message for details.
     */
    @Override
    public Integer nextValue() throws NullPointerException,IllegalArgumentException{
        //store value history if needed
        if(isStoreHistory()) storeValue(this.getValue());
        if(getGeneratorType() == null || !isGenerateValue()) return null;
        Double result = null;
        switch (getGeneratorType()){
            case NORMAL_DISTRIBUTION:
                if(devProperty() == null || avgProperty() == null || maxProperty() == null || minProperty() == null){
                    throw new IllegalArgumentException("Boiler operation type generator doesn't have variables needed to generate new value (Dev,Avg,Max,Min)");
                }
                 result = Utilities.normalDistribution(getGenerator(), 1, getDev(), getAvg(), getMax(), getMin());

                break;
            case LINEAR_DISTRIBUTION:
                if(stepProperty() == null || maxProperty() == null || minProperty() == null){
                    throw new IllegalArgumentException("Boiler operation type generator doesn't have variables needed to generate new value (Step,Max,Min)");
                }
                result = Utilities.linearDistribution(getValue(),getStep(),getMax(),getMin());
                break;
        }
        return result == null ? null : (int) Math.round(result);
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public String toStringWithUnit() {
        for(BoilerOperationType tmp : BoilerOperationType.values()){
            if(getValue() == tmp.getCode())
                return String.valueOf(tmp.getCode() + "-" + tmp.getDescription());
        }
        return "No type";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String asMessageString() {
        return String.valueOf(getValue());
    }

    /**
     * {@inheritDoc}
     *
     * Returns integer.
     */
    @Override
    public Integer fromStringToValueType(String valueString) throws NumberFormatException {
        return Integer.valueOf(valueString);
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
                case NORMAL_DISTRIBUTION:
                    generatorElement.addElement("params")
                            .addAttribute("min", String.valueOf(getMin()))
                            .addAttribute("max",String.valueOf(getMax()))
                            .addAttribute("dev",String.valueOf(getDev()))
                            .addAttribute("avg",String.valueOf(getAvg()));
                    break;
                case LINEAR_DISTRIBUTION:
                    generatorElement.addElement("params")
                            .addAttribute("min", String.valueOf(getMin()))
                            .addAttribute("max",String.valueOf(getMax()))
                            .addAttribute("step",String.valueOf(getStep()));
                    break;
            }
        }
    }

    /**
     * Sets variables for normal distribution
     * @param dev deviation
     * @param avg average
     * @param max maximal value
     * @param min minimal value
     */
    public void setGeneratorVariables(double dev,double avg,double max,double min){
        setDev(dev);
        setAvg(avg);
        setMax(max);
        setMin(min);
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
    public double getDev() {
        return dev.get();
    }

    /**
     * Gets normal distribution deviation property, that can be bound
     * @return normal distribution deviation property
     */
    public DoubleProperty devProperty() {
        return dev;
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public void setDev(double dev) {
        this.dev.set(dev);
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public double getAvg() {
        return avg.get();
    }

    /**
     * Gets normal distribution average property,that can be bound
     * @return normal distribution average property
     */
    public DoubleProperty avgProperty() {
        return avg;
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public void setAvg(double avg) {
        this.avg.set(avg);
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public double getMin() {
        return min.get();
    }

    /**
     * Gets distribution minimal value property,that can be bound
     * @return distribution minimal value property
     */
    public DoubleProperty minProperty() {
        return min;
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public void setMin(double min) {
        this.min.set(min);
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public double getMax() {
        return max.get();
    }

    /**
     * Gets distribution maximal value property,that can be bound
     * @return distribution maximal value property
     */
    public DoubleProperty maxProperty() {
        return max;
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public void setMax(double max) {
        this.max.set(max);
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public double getStep() {
        return step.get();
    }

    /**
     * Gets linear distribution step property,that can be bound
     * @return linear distribution step property
     */
    public DoubleProperty stepProperty() {
        return step;
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public void setStep(double step) {
        this.step.set(step);
    }
}
