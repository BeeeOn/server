package com.iha.emulator.models.value.implemented;

import com.iha.emulator.models.value.AbstractValue;
import com.iha.emulator.models.value.HasLinearDistribution;
import com.iha.emulator.models.value.HasNormalDistribution;
import com.iha.emulator.utilities.Utilities;
import javafx.beans.property.DoubleProperty;
import javafx.beans.property.SimpleDoubleProperty;
import org.dom4j.Element;

import java.util.Random;

/**
 * Created by Shu on 2.12.2014.
 */
public class TemperatureSensorValue extends AbstractValue<Double> implements HasNormalDistribution,HasLinearDistribution {

    private static final double DEFAULT_VALUE = 25.5;
    private static final double DEFAULT_MAX = 50;
    private static final double DEFAULT_MIN = -50;
    private static final double DEFAULT_NORMAL_DEV = 0.5;
    private static final double DEFAULT_NORMAL_AVG = 25.5;
    private static final double DEFAULT_LINEAR_STEP = 3;

    private DoubleProperty dev;
    private DoubleProperty avg;
    private DoubleProperty min;
    private DoubleProperty max;
    private DoubleProperty step;

    public TemperatureSensorValue(String name, String type,int offset, String unit, boolean generateValue, boolean storeHistory, Random generator, Long generatorSeed) {
        super(Type.SENSOR_TEMPERATURE,name,type,offset,unit,generateValue,storeHistory,generator,generatorSeed);
        this.dev = new SimpleDoubleProperty(DEFAULT_NORMAL_DEV);
        this.avg = new SimpleDoubleProperty(DEFAULT_NORMAL_AVG);
        this.min = new SimpleDoubleProperty(DEFAULT_MIN);
        this.max= new SimpleDoubleProperty(DEFAULT_MAX);
        this.step = new SimpleDoubleProperty(DEFAULT_LINEAR_STEP);
        setInitialValue(DEFAULT_VALUE);
        setValue(DEFAULT_VALUE);
    }

    @Override
    public Double nextValue() throws NullPointerException,IllegalArgumentException{
        //store value history if needed
        if(isStoreHistory()) storeValue(this.getValue());
        /**/
        if(getGeneratorType() == null || !isGenerateValue()) return null;
        switch (getGeneratorType()){
            case NORMAL_DISTRIBUTION:
                if(devProperty() == null || avgProperty() == null || maxProperty() == null || minProperty() == null){
                    throw new IllegalArgumentException("Temperature generator doesn't have variables needed to generate new value (Dev,Avg,Max,Min)");
                }
                return Utilities.normalDistribution(getGenerator(), 100, getDev(), getAvg(), getMax(), getMin());
            case LINEAR_DISTRIBUTION:
                if(stepProperty() == null || maxProperty() == null || minProperty() == null){
                    throw new IllegalArgumentException("Temperature generator doesn't have variables needed to generate new value (Step,Max,Min)");
                }
                return Utilities.linearDistribution(getValue(),getStep(),getMax(),getMin());
        }
        return null;
    }

    @Override
    public String toStringWithUnit() {
        return String.valueOf(getValue()) + " " + getUnit();
    }

    @Override
    public String asMessageString() {
        return String.valueOf(getValue());
    }

    @Override
    public Double fromStringToValueType(String valueString) {
        return Double.valueOf(valueString);
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

    public void setGeneratorVariables(double dev,double avg,double max,double min){
        setDev(dev);
        setAvg(avg);
        setMax(max);
        setMin(min);
    }

    @Override
    public void restartGenerator() {
        setGenerator(new Random());
        setGeneratorSeed(getGeneratorSeed());
    }

    @Override
    public double getDev() {
        return dev.get();
    }

    public DoubleProperty devProperty() {
        return dev;
    }

    public void setDev(double dev) {
        this.dev.set(dev);
    }

    @Override
    public double getAvg() {
        return avg.get();
    }

    public DoubleProperty avgProperty() {
        return avg;
    }

    public void setAvg(double avg) {
        this.avg.set(avg);
    }

    @Override
    public double getMin() {
        return min.get();
    }

    public DoubleProperty minProperty() {
        return min;
    }

    public void setMin(double min) {
        this.min.set(min);
    }

    @Override
    public double getMax() {
        return max.get();
    }

    public DoubleProperty maxProperty() {
        return max;
    }

    public void setMax(double max) {
        this.max.set(max);
    }

    @Override
    public double getStep() {
        return step.get();
    }

    public DoubleProperty stepProperty() {
        return step;
    }

    public void setStep(double step) {
        this.step.set(step);
    }

}
