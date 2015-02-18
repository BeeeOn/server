package com.iha.emulator.models.value;

import com.iha.emulator.models.value.implemented.HasGenerator;
import javafx.beans.property.*;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.ArrayList;
import java.util.Random;

/**
 * Created by Shu on 2.12.2014.
 */
public abstract class AbstractValue<T> implements Value<T>,HasGenerator{

    private static final Logger logger = LogManager.getLogger(Value.class);

    private Type valueType;
    private ObjectProperty<T> value;
    private T initialValue;
    private StringProperty stringValue;
    private StringProperty name;
    private String type;
    private int offset;
    private String unit;
    private boolean generateValue;
    private boolean storeHistory;
    private Random generator;
    private Long generatorSeed;

    private ArrayList<T> valueHistory;

    public AbstractValue(Type valueType,String name,String type,String unit){
        this.name = new SimpleStringProperty(name);
        this.valueType = valueType;
        this.type = type;
        this.unit = unit;
    }

    public AbstractValue(Type valueType,String name,String type,int offset,String unit,boolean generateValue,boolean storeHistory,Random generator,Long generatorSeed) {
        this.valueType = valueType;
        this.name = new SimpleStringProperty(name);
        this.type = type;
        this.offset = offset;
        this.unit = unit;
        this.generateValue = generateValue;
        this.storeHistory = storeHistory;
        this.generator = generator;
        if(generatorSeed == null){
            setGeneratorSeed(System.currentTimeMillis());
        }else{
            setGeneratorSeed(generatorSeed);
        }
        if(value!= null){
            this.stringValue = new SimpleStringProperty(getStringValue());
        }else{
            this.stringValue = new SimpleStringProperty();
        }
        this.value = new SimpleObjectProperty<>();
    }

    public void storeValue(T value){
        if(valueHistory == null) valueHistory = new ArrayList<>();
        valueHistory.add(value);
    }

    @Override
    public T getInitialValue() {
        return initialValue;
    }

    @Override
    public void setInitialValue(T initialValue) {
        this.initialValue = initialValue;
    }

    @Override
    public T getValue() {
        return value.get();
    }

    public Property<T> valueProperty() {
        return value;
    }

    public void setValue(T value) {
        this.value.set(value);
        setStringValue(toStringWithUnit());
    }

    public String getStringValue() {
        return stringValue.get();
    }

    public StringProperty stringValueProperty() {
        return stringValue;
    }

    public void setStringValue(String stringValue) {
        this.stringValue.set(stringValue);
    }

    public abstract void nextValue();

    public Type getValueType() {
        return valueType;
    }

    @Override
    public void setValueType(Type valueType) {
        this.valueType = valueType;
    }

    public String getName() {
        return name.get();
    }

    public StringProperty nameProperty() {
        return name;
    }

    public void setName(String name) {
        this.name.set(name);
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public int getOffset(){
        return this.offset;
    }

    public void setOffset(int offset){
        this.offset = offset;
    }

    public String getUnit() {
        return unit;
    }

    public void setUnit(String unit) {
        this.unit = unit;
    }

    public boolean isGenerateValue() {
        return generateValue;
    }

    public void setGenerateValue(boolean generateValue) {
        this.generateValue = generateValue;
    }

    public boolean isStoreHistory() {
        return storeHistory;
    }

    public void setStoreHistory(boolean storeHistory) {
        this.storeHistory = storeHistory;
    }

    public Random getGenerator() {
        return generator;
    }

    public void setGenerator(Random generator) {
        this.generator = generator;
    }

    public long getGeneratorSeed() {
        return generatorSeed;
    }

    public void setGeneratorSeed(long generatorSeed) {
        this.generatorSeed = generatorSeed;
        if(this.generator != null){
            this.generator.setSeed(this.generatorSeed);
        }
    }

    public ArrayList<T> getValueHistory() {
        return valueHistory;
    }

    public void setValueHistory(ArrayList<T> valueHistory) {
        this.valueHistory = valueHistory;
    }
}
