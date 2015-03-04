package com.iha.emulator.models.value;

/**
 * Created by Shu on 20.2.2015.
 */
public interface HasBooleanRandom extends HasGenerator {
    public double getProbability();
    public void setProbability(double probability);
}
