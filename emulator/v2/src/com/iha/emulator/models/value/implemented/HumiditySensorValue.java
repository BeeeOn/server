package com.iha.emulator.models.value.implemented;

import com.iha.emulator.models.SensorType;
import com.iha.emulator.models.value.AbstractValue;
import com.iha.emulator.utilities.Utilities;

import java.util.Random;

/**
 * Created by Shu on 2.12.2014.
 */
public class HumiditySensorValue extends AbstractValue<Integer> {

    private Double dev;
    private Double avg;
    private Double min;
    private Double max;

    public HumiditySensorValue(String name, String type, String unit, boolean generateValue, boolean storeHistory, Random generator, Long generatorSeed) {
        super(SensorType.SENSOR_HUMIDITY,name,type,unit,generateValue,storeHistory,generator,generatorSeed);
        setValue(0);
    }

    @Override
    public void nextValue() throws NullPointerException{
        //store value history if needed
        if(isStoreHistory()) storeValue(this.getValue());
        //check for variables needed to generate new value
        if(getDev() == null || getAvg() == null || getMax() == null ||getMin() == null){
            throw new UnsupportedOperationException("Humidity generator doesn't have variables needed to generate new value (Dev,Avg,Max,Min)");
        }
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

    public void setGeneratorVariables(double dev,double avg,double max,double min){
        setDev(dev);
        setAvg(avg);
        setMax(max);
        setMin(min);
    }

    public Double getDev() {
        return dev;
    }

    public void setDev(Double dev) {
        this.dev = dev;
    }

    public Double getAvg() {
        return avg;
    }

    public void setAvg(Double avg) {
        this.avg = avg;
    }

    public Double getMin() {
        return min;
    }

    public void setMin(Double min) {
        this.min = min;
    }

    public Double getMax() {
        return max;
    }

    public void setMax(Double max) {
        this.max = max;
    }
}
