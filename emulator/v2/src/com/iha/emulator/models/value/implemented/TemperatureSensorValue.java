package com.iha.emulator.models.value.implemented;

import com.iha.emulator.models.value.AbstractValue;
import com.iha.emulator.models.value.HasLinearDistribution;
import com.iha.emulator.models.value.HasNormalDistribution;
import com.iha.emulator.utilities.Utilities;
import javafx.beans.property.DoubleProperty;
import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleDoubleProperty;
import javafx.beans.property.SimpleObjectProperty;

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
    private ObjectProperty<Generator> generatorType;

    public TemperatureSensorValue(Type valueType,String name, String type,int offset, String unit, boolean generateValue, boolean storeHistory, Random generator, Long generatorSeed) {
        super(valueType,name,type,offset,unit,generateValue,storeHistory,generator,generatorSeed);
        this.dev = new SimpleDoubleProperty(DEFAULT_NORMAL_DEV);
        this.avg = new SimpleDoubleProperty(DEFAULT_NORMAL_AVG);
        this.min = new SimpleDoubleProperty(DEFAULT_MIN);
        this.max= new SimpleDoubleProperty(DEFAULT_MAX);
        this.step = new SimpleDoubleProperty(DEFAULT_LINEAR_STEP);
        this.generatorType = new SimpleObjectProperty<>(null);
        setValue(DEFAULT_VALUE);
    }

    @Override
    public void nextValue() throws NullPointerException{
        //store value history if needed
        if(isStoreHistory()) storeValue(this.getValue());
        //check for variables needed to generate new value
        /*if(devProperty() == null || avgProperty() == null || maxProperty() == null || minProperty() == null){
            throw new UnsupportedOperationException("Temperature generator doesn't have variables needed to generate new value (Dev,Avg,Max,Min)");
        }*/
        this.setValue(Utilities.normalDistribution(getGenerator(), 100, getDev(), getAvg(), getMax(), getMin()));
    }

    @Override
    public void nextValue(Double value) {
        //check value
        if(value == null) throw new NullPointerException("Trying to set value to null");
        //store value history if needed
        if(isStoreHistory()) storeValue(this.getValue());
        //set value
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
    public Double fromStringToValueType(String valueString) {
        return Double.valueOf(valueString);
    }

    public void setGeneratorVariables(double dev,double avg,double max,double min){
        setDev(dev);
        setAvg(avg);
        setMax(max);
        setMin(min);
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

    @Override
    public Generator getGeneratorType() {
        return generatorType.get();
    }

    public ObjectProperty<Generator> generatorTypeProperty(){
        return generatorType;
    }

    public void setGeneratorType(Generator generatorType){
        this.generatorType.set(generatorType);
    }
}
