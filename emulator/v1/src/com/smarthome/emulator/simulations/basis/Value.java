package com.smarthome.emulator.simulations.basis;

import java.util.ArrayList;
import java.util.Random;

/**
 * Stores information about {@link Sensor}'s value.
 * Generates new value for {@link java.lang.Float} type values with normal distribution.
 */
public class Value {
    private String name = "Sensor/actuator";
    /** generator for random values */
    private Random generator = null;
    /** indicator of generating new value */
    private boolean generateValue = true;
    /** seed for generator */
    private long generatorSeed;
    /** list of value's history */
    private ArrayList<Object> history;
    /** value type according to protocol */
    private Short type;
    /** actual value object */
    private Object value;
    /** first value inserted */
    private Object initialValue = null;
    /** max generated value */
    private Float max = null;
    /** min generated value */
    private Float min = null;
    /** average for generated values */
    private Float avg = null;
    /** deviation for generating value */
    private Float dev = null;
    private String valueClass = null;
    /** indicator of saving history */
    private boolean storeHistory = true;
    public Value(){

    }
    public Value(String name) {
        this.name = name;
    }
    /**
     * Stores current value and generates new value (currently only for Float values) with normal distribution function
     */
    public void nextValue(){
        if(isStoreHistory()){
            storeValue(this.getValue());
        }
        if(this.getValue() instanceof Float){
            this.setValue(normalDistribution(this,100));
        }
        if(this.getValue() instanceof  Boolean){
            this.setValue(generator.nextBoolean());
        }
    }
    public void nextValue(Object newValue){
        if(newValue == null){
            System.err.println("Trying to set null value to value type: " + getType());
            return;
        }
        if(isStoreHistory()){
            storeValue(this.getValue());
        }
        this.setValue(newValue);
    }
    /**
     * Generates new value from current one with accuracy given as parameters.
     * Accuracy = 10 for 1 decimal, 100 for to decimals, ...
     * @param value current value
     * @param accuracy 10 for 1 decimal, 100 for to decimals, ...
     * @return newly generated value
     */
    public static float normalDistribution(Value value,int accuracy){
        float newValue;
        do{
            double val = value.getGenerator().nextGaussian() * value.getDev() + value.getAvg();
            newValue = (float) Math.round(val * accuracy) / accuracy;
        }while(newValue > value.getMax() || newValue < value.getMin());
        return newValue;
    }
    /**
     * Saves given value to history
     * @param o value to be saved
     */
    public void storeValue(Object o){
        history.add(o);
    }
    /**
     * Checks, if value is of type of actuator
     * @return <code>true</code> if is actuator,<code>false</code> otherwise
     */
    public boolean isActuator(){
        if(this.type == (short)4)
            return true;
        else
            return false;
    }
    public void switchActuatorState(){
        if(this.type == (short)4 || this.type == (short)3){
            Boolean currentValue = (Boolean)getValue();
            nextValue(!currentValue);
        }
    }
    //region getters and setters
    /**
     * Returns value's generator
     * @return value's generator
     */
    public Random getGenerator() {
        return generator;
    }
    /**
     * Sets value's generator
     * @param generator generator for new values
     */
    public void setGenerator(Random generator) {
        this.generator = generator;
    }
    /**
     * Gets new value generator seed
     * @return new value generator seed
     */
    public long getGeneratorSeed(){
        return this.generatorSeed;
    }
    /**
     * Sets new value generator seed
     * @param l seed for generator
     */
    public void setGeneratorSeed(long l){
        this.generatorSeed = l;
        if(this.generator != null){
            this.generator.setSeed(this.generatorSeed);
        }
    }
    /**
     * Returns <code>true</code> if generating of new value is accepted, <code>false</code> otherwise
     * @return <code>true</code> if generating of new value is accepted, <code>false</code> otherwise
     */
    public boolean isGenerateValue(){
        return this.generateValue;
    }
    /**
     * Sets if should be generated new value
     * @param b <code>true</code> if generating new value is accepted, <code>false</code> otherwise
     */
    public void setGenerateValue(boolean b){
        this.generateValue = b;
    }
    /**
     * Returns list of value's history
     * @return list of value's history
     */
    public ArrayList<Object> getHistory() {
        return history;
    }

    /**
     * Sets value history list
     * @param history list of value history
     */
    public void setHistory(ArrayList<Object> history) {
        this.history = history;
    }
    /**
     * Returns value's type according to protocol
     * @return value's type according to protocol
     */
    public Short getType() {
        return type;
    }
    /**
     * Sets value's type according to protocol
     * @param type value's type according to protocol
     */
    public void setType(Short type) {
        this.type = type;
    }
    /**
     * Gets first set value
     * @return first set value
     */
    public Object getInitialValue() {
        return initialValue;
    }
    /**
     * Sets first value
     * @param initialValue first value
     */
    public void setInitialValue(Object initialValue) {
        this.initialValue = initialValue;
    }
    /**
     * Returns current value
     * @return current value
     */
    public Object getValue() {
        return value;
    }
    /**
     * Sets current value
     * @param value current value
     */
    public void setValue(Object value) {
        this.value = value;
    }
    /**
     * Returns maximum for generated values
     * @return maximum for generated values
     */
    public Float getMax() {
        return max;
    }

    /**
     * Sets maximum for generated values
     * @param max maximum for generated values
     */
    public void setMax(Float max) {
        this.max = max;
    }
    /**
     * Returns minimum for generated values
     * @return minimum for generated values
     */
    public Float getMin() {
        return min;
    }
    /**
     * Sets minimum for generated values
     * @param min minimum for generated values
     */
    public void setMin(Float min) {
        this.min = min;
    }

    /**
     * Returns average for generated value
     * @return average for generated value
     */
    public Float getAvg() {
        return avg;
    }
    /**
     * Sets average for generated values
     * @param avg average for generated values
     */
    public void setAvg(Float avg) {
        this.avg = avg;
    }
    /**
     * Returns deviation for generated values
     * @return deviation for generated values
     */
    public Float getDev() {
        return dev;
    }
    /**
     * Sets deviation for generated values
     * @param dev deviation for generated values
     */
    public void setDev(Float dev) {
        this.dev = dev;
    }
    public String getValueClass() {
        return valueClass;
    }

    public void setValueClass(String valueClass) {
        this.valueClass = valueClass;
    }
    /**
     * If history of value is accepted
     * @return <code>true</code> if saving is accepted, <code>false</code> otherwise
     */
    public boolean isStoreHistory(){
        return this.storeHistory;
    }

    /**
     * Sets if history of value should save
     * @param b <code>true</code> if history should be saved, <code>false</code> otherwise
     */
    public void setStoreHistory(boolean b){
        this.storeHistory = b;
    }
    public String getName(){
        return this.name;
    }
    public void setName(String name){
        this.name = name;
    }
    //endregion
}
