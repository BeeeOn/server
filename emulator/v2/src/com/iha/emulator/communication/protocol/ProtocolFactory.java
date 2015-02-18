package com.iha.emulator.communication.protocol;

/**
 * Created by Shu on 26.11.2014.
 */
public class ProtocolFactory {

    public static Protocol.Version getVersion(double versionNumber){
        for(Protocol.Version version : Protocol.Version.values()){
            if(version.getVersion() == versionNumber)
                return version;
        }
        return null;
    }

    public static Protocol buildProtocol(Double versionNumber) throws IllegalArgumentException{
        for(Protocol.Version version : Protocol.Version.values()){
            if(version.getVersion() == versionNumber){
                try{
                    return buildProtocol(version);
                }catch (IllegalArgumentException e){
                    throw new IllegalArgumentException(e);
                }
            }
        }
        return null;
    }

    public static Protocol buildProtocol(Protocol.Version version) throws IllegalArgumentException{
        Protocol protocol;
        try {
            protocol = (Protocol) version.getVersionClass().newInstance();
        } catch (InstantiationException e) {
            throw new IllegalArgumentException("Cannot initiate protocol class\"" + Protocol.Version.ZERO_POINT_ONE.getVersionClass().getName()+"\"",e);
        } catch (IllegalAccessException e) {
            throw new IllegalArgumentException("Class\"" + Protocol.Version.ZERO_POINT_ONE.getVersionClass().getName() + "\" must have empty constructor",e);
        }
        return protocol;
    }
}
