package com.iha.emulator.communication.protocol;

/**
 * Created by Shu on 26.11.2014.
 */
public class ZeroPointOne extends AbstractProtocol {

    public ZeroPointOne(double version) {
        super(version);
    }

    @Override
    public String buildAdapterMessage() {
        return "adapter message";
    }

    @Override
    public String buildSensorMessage() {
        return "return sensor message";
    }
}
