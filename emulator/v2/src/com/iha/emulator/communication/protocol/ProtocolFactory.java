package com.iha.emulator.communication.protocol;

/**
 * Created by Shu on 26.11.2014.
 */
public class ProtocolFactory {
    public static Protocol buildProtocol(double version) throws IllegalArgumentException{
        Protocol protocol;
        String versionString = String.valueOf(version);
        switch (versionString){
            case "0.1":
                protocol = new ZeroPointOne(version);
                break;
            default:
                throw new IllegalArgumentException("Unknown protocol version - " + versionString);
        }
        return protocol;
    }
}
