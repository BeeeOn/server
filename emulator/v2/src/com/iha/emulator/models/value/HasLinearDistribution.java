package com.iha.emulator.models.value;

/**
 * Created by Shu on 12.12.2014.
 */
public interface HasLinearDistribution extends HasGenerator {
    public double getStep();
    public void setStep(double avg);
    public double getMin();
    public void setMin(double min);
    public double getMax();
    public void setMax(double max);
}
