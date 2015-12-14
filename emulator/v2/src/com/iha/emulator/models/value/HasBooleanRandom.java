package com.iha.emulator.models.value;

/**
 * Interface providing methods, that needs to be implemented by value, that want to use boolean generator.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public interface HasBooleanRandom extends HasGenerator {
    /**
     * Gets probability of random boolean change
     * @return probability of random boolean change
     */
    public double getProbability();

    /**
     * Sets probability of random boolean change
     * @param probability probability of random boolean change
     */
    public void setProbability(double probability);
}
