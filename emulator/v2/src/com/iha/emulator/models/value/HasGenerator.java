package com.iha.emulator.models.value;

/**
 * Created by Shu on 12.12.2014.
 */
public interface HasGenerator {
    public Value.Generator getGeneratorType();
    public void setGeneratorType(Value.Generator generatorType);
    public void restartGenerator();
}
