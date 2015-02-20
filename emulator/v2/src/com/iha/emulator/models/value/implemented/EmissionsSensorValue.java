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
public class EmissionsSensorValue extends AbstractValue<Integer> implements HasNormalDistribution,HasLinearDistribution{

    private static final int DEFAULT_VALUE = 320;
    private static final double DEFAULT_MAX = 2000;
    private static final double DEFAULT_MIN = 0;
    private static final double DEFAULT_NORMAL_DEV = 5;
    private static final double DEFAULT_NORMAL_AVG = 320;
    private static final double DEFAULT_LINEAR_STEP = 20;

    private DoubleProperty dev;
    private DoubleProperty avg;
    private DoubleProperty min;
    private DoubleProperty max;
    private DoubleProperty step;

    public EmissionsSensorValue(String name, String type, int offset, String unit, boolean generateValue, boolean storeHistory, Random generator, Long generatorSeed) {
        super(Type.SENSOR_EMISSIONS,name,type,offset,unit,generateValue,storeHistory,generator,generatorSeed);
        this.dev = new SimpleDoubleProperty(DEFAULT_NORMAL_DEV);
        this.avg = new SimpleDoubleProperty(DEFAULT_NORMAL_AVG);
        this.min = new SimpleDoubleProperty(DEFAULT_MIN);
        this.max= new SimpleDoubleProperty(DEFAULT_MAX);
        this.step = new SimpleDoubleProperty(DEFAULT_LINEAR_STEP);
        setInitialValue(DEFAULT_VALUE);
        setValue(DEFAULT_VALUE);
    }

    @Override
    public void nextValue() throws NullPointerException{
        //store value history if needed
        if(isStoreHistory()) storeValue(this.getValue());
        if(getGeneratorType() == null || !isGenerateValue()) return;
        Double result = null;
        switch (getGeneratorType()){
            case NORMAL_DISTRIBUTION:
                if(devProperty() == null || avgProperty() == null || maxProperty() == null || minProperty() == null){
                    throw new IllegalArgumentException("Emissions generator doesn't have variables needed to generate new value (Dev,Avg,Max,Min)");
                }
                 result = Utilities.normalDistribution(getGenerator(), 1, getDev(), getAvg(), getMax(), getMin());

                break;
            case LINEAR_DISTRIBUTION:
                if(stepProperty() == null || maxProperty() == null || minProperty() == null){
                    throw new IllegalArgumentException("Emissions generator doesn't have variables needed to generate new value (Step,Max,Min)");
                }
                result = Utilities.linearDistribution(getValue(),getStep(),getMax(),getMin());
                break;
        }
        this.setValue(result == null ? null : (int) Math.round(result));
    }

    @Override
    public void nextValue(Integer value) {
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
        return String.valueOf(getValue());
    }

    @Override
    public Integer fromStringToValueType(String valueString) throws NumberFormatException {
        return Integer.valueOf(valueString);
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

    public double getDev() {
        return dev.get();
    }

    public DoubleProperty devProperty() {
        return dev;
    }

    public void setDev(double dev) {
        this.dev.set(dev);
    }

    public double getAvg() {
        return avg.get();
    }

    public DoubleProperty avgProperty() {
        return avg;
    }

    public void setAvg(double avg) {
        this.avg.set(avg);
    }

    public double getMin() {
        return min.get();
    }

    public DoubleProperty minProperty() {
        return min;
    }

    public void setMin(double min) {
        this.min.set(min);
    }

    public double getMax() {
        return max.get();
    }

    public DoubleProperty maxProperty() {
        return max;
    }

    public void setMax(double max) {
        this.max.set(max);
    }

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
