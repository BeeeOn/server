package com.iha.emulator.models.value;

/**
 * Interface providing methods, that needs to be implemented by value, that want to use linear distribution generator.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public interface HasLinearDistribution extends HasGenerator {
    /**
     * Gets generator step of linear distribution
     * @return generator step of linear distribution
     */
    public double getStep();

    /**
     * Sets generator step of linear distribution
     * @param avg generator step of linear distribution
     */
    public void setStep(double avg);

    /**
     * Gets minimal value of distribution
     * @return minimal value of distribution
     */
    public double getMin();

    /**
     * Sets minimal value of distribution
     * @param min minimal value of distribution
     */
    public void setMin(double min);

    /**
     * Gets maximal value of distribution
     * @return maximal value of distribution
     */
    public double getMax();

    /**
     * Sets maximal value of distribution
     * @param max maximal value of distribution
     */
    public void setMax(double max);
}
