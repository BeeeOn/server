package com.iha.emulator.models.value.implemented;

import com.iha.emulator.models.value.AbstractValue;
import org.dom4j.Element;

/**
 * Dummy class used as representation of Empty value
 */
public class EmptyValue extends AbstractValue {
    /**
     * Creates instance with no value
     * @param name name of value
     */
    public EmptyValue(String name) {
        super(null,name,"","");
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void restartGenerator() {
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public Object nextValue() {
        return null;
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public String toStringWithUnit() {
        return null;
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public String asMessageString() {
        return null;
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public Object fromStringToValueType(String valueString) throws NumberFormatException {
        return null;
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public void saveToXML(Element rootElement) {

    }
    /**
     * {@inheritDoc}
     */
    @Override
    public Generator getGeneratorType() {
        return null;
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public void setGeneratorType(Generator generatorType) {

    }
}
