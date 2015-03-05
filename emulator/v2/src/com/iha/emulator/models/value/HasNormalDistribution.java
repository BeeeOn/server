package com.iha.emulator.models.value;

/**
 * Created by Shu on 12.12.2014.
 */
public interface HasNormalDistribution extends HasGenerator{
    public double getDev();
    public void setDev(double dev);
    public double getAvg();
    public void setAvg(double avg);
    public double getMin();
    public void setMin(double min);
    public double getMax();
    public void setMax(double max);
}
