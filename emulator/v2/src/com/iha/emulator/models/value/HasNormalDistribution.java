package com.iha.emulator.models.value;

/**
 * Interface providing methods, that needs to be implemented by value, that want to use normal distribution generator.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public interface HasNormalDistribution extends HasGenerator{
    /**
     * Gets deviation of normal distribution
     * @return deviation of normal distribution
     */
    public double getDev();

    /**
     * Sets deviation of normal distribution
     * @param dev deviation of normal distribution
     */
    public void setDev(double dev);

    /**
     * Gets average of normal distribution
     * @return average of normal distribution
     */
    public double getAvg();

    /**
     * Sets average of normal distribution
     * @param avg average of normal distribution
     */
    public void setAvg(double avg);

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
