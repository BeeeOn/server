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
public class HumiditySensorValue extends AbstractValue<Integer> implements HasNormalDistribution,HasLinearDistribution{

    private static final int DEFAULT_VALUE = 55;
    private static final double DEFAULT_MAX = 100;
    private static final double DEFAULT_MIN = 0;
    private static final double DEFAULT_NORMAL_DEV = 1;
    private static final double DEFAULT_NORMAL_AVG = 55;
    private static final double DEFAULT_LINEAR_STEP = 5;

    private DoubleProperty dev;
    private DoubleProperty avg;
    private DoubleProperty min;
    private DoubleProperty max;
    private DoubleProperty step;
    private ObjectProperty<Generator> generatorType;

    public HumiditySensorValue(String name, String type,int offset, String unit, boolean generateValue, boolean storeHistory, Random generator, Long generatorSeed) {
        super(Type.SENSOR_HUMIDITY,name,type,offset,unit,generateValue,storeHistory,generator,generatorSeed);
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
        //TODO diverse distributions
        //generate new value
        Double result = Utilities.normalDistribution(getGenerator(), 1, getDev(), getAvg(), getMax(), getMin());
        Integer integer = result == null ? null : (int) Math.round(result);
        this.setValue(integer);
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

    @Override
    public Generator getGeneratorType() {
        return generatorType.get();
    }

    public ObjectProperty<Generator> generatorTypeProperty() {
        return generatorType;
    }

    public void setGeneratorType(Generator generatorType) {
        this.generatorType.set(generatorType);
    }
}
