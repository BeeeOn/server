package com.iha.emulator.models.value;

import com.iha.emulator.models.value.implemented.HasGenerator;

/**
 * Created by Shu on 20.2.2015.
 */
public interface HasBooleanRandom extends HasGenerator {
    public double getProbability();
    public void setProbability(double probability);
}
