package com.iha.emulator.models.value.implemented;

import com.iha.emulator.models.value.Value;

/**
 * Created by Shu on 12.12.2014.
 */
public interface HasGenerator {
    public Value.Generator getGeneratorType();
    public void setGeneratorType(Value.Generator generatorType);
}
