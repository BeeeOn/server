package com.iha.emulator.models.value;

import javafx.beans.property.*;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.ArrayList;
import java.util.Random;

/**
 * Abstract class providing implementation of {@link com.iha.emulator.models.value.Value} interface methods used by
 * all specific value implementations.
 * @param <T> data type set by class extending this abstract class, used in compilation
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public abstract class AbstractValue<T> implements Value<T>,HasGenerator{
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(Value.class);
    /** value type */
    private Type valueType;
    /** value property */
    private ObjectProperty<T> value;
    /** initial value */
    private T initialValue;
    /** string representation of value */
    private StringProperty stringValue;
    /** name of value */
    private StringProperty name;
    /** type of value used in XML communication */
    private String type;
    /** value's offset in sensor */
    private int offset;
    /** value's unit */
    private String unit;
    /** value generation flag */
    private boolean generateValue;
    /** store old value flag */
    private boolean storeHistory;
    /** generator for new value */
    private Random generator;
    /** seed for new value generator */
    private Long generatorSeed;
    /** currently used generator type */
    private ObjectProperty<Generator> generatorType;
    /** list of old values */
    private ArrayList<T> valueHistory;

    /**
     * Initializes value information given as parameters.
     * @param valueType value type
     * @param name value name
     * @param type value type used in XML communication
     * @param unit value unit
     */
    public AbstractValue(Type valueType,String name,String type,String unit){
        this.name = new SimpleStringProperty(name);
        this.valueType = valueType;
        this.type = type;
        this.unit = unit;
    }

    /**
     * Initializes value information given as parameters.
     * @param valueType value type
     * @param name value name
     * @param type value type used in XML communication
     * @param offset value's offset in sensor
     * @param unit value's unit
     * @param generateValue should be new value generated?
     * @param storeHistory should be value history stored?
     * @param generator generator for new values
     * @param generatorSeed seed for new values generator
     */
    public AbstractValue(Type valueType,String name,String type,int offset,String unit,boolean generateValue,boolean storeHistory,Random generator,Long generatorSeed) {
        this.valueType = valueType;
        this.name = new SimpleStringProperty(name);
        this.type = type;
        this.offset = offset;
        this.unit = unit;
        this.generateValue = generateValue;
        this.storeHistory = storeHistory;
        this.generator = generator;
        //if seed is not given, make seed current time in milliseconds
        if(generatorSeed == null){
            setGeneratorSeed(System.currentTimeMillis());
        }else{
            setGeneratorSeed(generatorSeed);
        }
        //initialize string value property
        if(value!= null){
            this.stringValue = new SimpleStringProperty(getStringValue());
        }else{
            this.stringValue = new SimpleStringProperty();
        }
        this.value = new SimpleObjectProperty<>();
        this.generatorType = new SimpleObjectProperty<>(null);
    }

    /**
     * Store value given as parameter in history list
     * @param value value to be stored
     */
    public void storeValue(T value){
        if(valueHistory == null) valueHistory = new ArrayList<>();
        valueHistory.add(value);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public T getInitialValue() {
        return initialValue;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setInitialValue(T initialValue) {
        this.initialValue = initialValue;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public T getValue() {
        return value.get();
    }

    /**
     * Gets value property, that can be bound
     * @return value property
     */
    public Property<T> valueProperty() {
        return value;
    }

    /**
     * {@inheritDoc}
     *
     * Also sets string value.
     *
     * @param value value with data type set by class implementing {@link com.iha.emulator.models.value.Value} interface
     */
    @Override
    public void setValue(T value) {
        this.value.set(value);
        setStringValue(toStringWithUnit());
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getStringValue() {
        return stringValue.get();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public StringProperty stringValueProperty() {
        return stringValue;
    }

    /**
     * {@inheritDoc}
     * @param stringValue value from string
     */
    @Override
    public void setStringValue(String stringValue) {
        this.stringValue.set(stringValue);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public abstract T nextValue();

    /**
     * {@inheritDoc}
     */
    @Override
    public Type getValueType() {
        return valueType;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setValueType(Type valueType) {
        this.valueType = valueType;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getName() {
        return name.get();
    }

    /**
     * {@inheritDoc}
     * @param name value's name
     */
    @Override
    public void setName(String name) {
        this.name.set(name);
    }

    /**
     * Gets value type used in XML communication
     * @return value type used in XML communication
     */
    public String getType() {
        return type;
    }

    /**
     * Sets value type used in XML communication
     * @param type value type used in XML communication
     */
    public void setType(String type) {
        this.type = type;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getOffset(){
        return this.offset;
    }

    /**
     * {@inheritDoc}
     * @param offset value's offset in sensor
     */
    @Override
    public void setOffset(int offset){
        this.offset = offset;
    }

    /**
     * Gets value's unit
     * @return value's unit
     */
    public String getUnit() {
        return unit;
    }

    /**
     * Sets value's unit
     * @param unit value's unit
     */
    public void setUnit(String unit) {
        this.unit = unit;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isGenerateValue() {
        return generateValue;
    }

    /**
     * {@inheritDoc}
     * @param generateValue <code>true</code> if new value should be generated, <code>false</code> otherwise
     */
    @Override
    public void setGenerateValue(boolean generateValue) {
        this.generateValue = generateValue;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isStoreHistory() {
        return storeHistory;
    }

    /**
     * {@inheritDoc}
     * @param storeHistory <code>true</code> if history should be stored, <code>false</code> otherwise
     */
    @Override
    public void setStoreHistory(boolean storeHistory) {
        this.storeHistory = storeHistory;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Random getGenerator() {
        return generator;
    }

    /**
     * Sets generator for new values
     * @param generator generator for new values
     */
    public void setGenerator(Random generator) {
        this.generator = generator;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public long getGeneratorSeed() {
        return generatorSeed;
    }

    /**
     * {@inheritDoc}
     * @param generatorSeed seed of value generator
     */
    @Override
    public void setGeneratorSeed(long generatorSeed) {
        this.generatorSeed = generatorSeed;
        if(this.generator != null){
            this.generator.setSeed(this.generatorSeed);

        }
    }

    /**
     * Gets list of old values
     * @return list of old values
     */
    public ArrayList<T> getValueHistory() {
        return valueHistory;
    }

    /**
     * Sets list of old values
     * @param valueHistory list of old values
     */
    public void setValueHistory(ArrayList<T> valueHistory) {
        this.valueHistory = valueHistory;
    }

    /**
     * Gets log4j2 logger
     * @return log4j2 logger
     */
    public Logger getLogger(){
        return logger;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Generator getGeneratorType() {
        return generatorType.get();
    }

    /**
     * Gets currently used generator type property
     * @return currently used generator type property
     */
    public ObjectProperty<Generator> generatorTypeProperty() {
        return generatorType;
    }

    /**
     * {@inheritDoc}
     * @param generatorType generator type
     */
    @Override
    public void setGeneratorType(Generator generatorType) {
        this.generatorType.set(generatorType);
    }
}
