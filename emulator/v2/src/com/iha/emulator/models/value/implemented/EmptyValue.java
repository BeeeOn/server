package com.iha.emulator.models.value.implemented;

import com.iha.emulator.models.value.AbstractValue;
import org.dom4j.Element;

/**
 * Created by Shu on 12.12.2014.
 */
public class EmptyValue extends AbstractValue {
    public EmptyValue(String name) {
        super(null,name,"","");
    }

    @Override
    public void restartGenerator() {
    }

    @Override
    public Object nextValue() {
        return null;
    }

    @Override
    public String toStringWithUnit() {
        return null;
    }

    @Override
    public String asMessageString() {
        return null;
    }

    @Override
    public Object fromStringToValueType(String valueString) throws NumberFormatException {
        return null;
    }

    @Override
    public void saveToXML(Element rootElement) {

    }

    @Override
    public Generator getGeneratorType() {
        return null;
    }

    @Override
    public void setGeneratorType(Generator generatorType) {

    }
}
