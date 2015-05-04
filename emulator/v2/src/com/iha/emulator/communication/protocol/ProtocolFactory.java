package com.iha.emulator.communication.protocol;

/**
 * Factory creates instance of class implementing interface {@link com.iha.emulator.communication.protocol.Protocol}
 * that is used to process emulated data to XML messages and parse messages from server.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 * @see com.iha.emulator.communication.protocol.Protocol
 * @see com.iha.emulator.communication.protocol.AbstractProtocol
 * @see com.iha.emulator.communication.protocol.implemented
 */
public class ProtocolFactory {
    /**
     * Returns enum element with information about protocol version (number,implementation class located in {@link com.iha.emulator.communication.protocol.implemented})
     * from {@link com.iha.emulator.communication.protocol.Protocol.Version} enum. If no enum element is found, returns <code>null</code>
     * @param versionNumber implemented protocol version number
     * @return {@link com.iha.emulator.communication.protocol.Protocol.Version} enum element
     */
    public static Protocol.Version getVersion(double versionNumber){
        for(Protocol.Version version : Protocol.Version.values()){
            if(version.getVersion() == versionNumber)
                return version;
        }
        return null;
    }

    /**
     * Creates instance of class implementing interface {@link com.iha.emulator.communication.protocol.Protocol} by given
     * protocol version number. If given version number is not found in enum {@link com.iha.emulator.communication.protocol.Protocol.Version},
     *  returns <code>null</code>.
     * @param versionNumber protocol version number contained by element in enum {@link com.iha.emulator.communication.protocol.Protocol.Version}
     * @return instance of class implementing interface {@link com.iha.emulator.communication.protocol.Protocol}
     * @throws IllegalArgumentException if cannot create new instance of implemented protocol class located in {@link com.iha.emulator.communication.protocol.implemented}
     */
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

    /**
     * Creates instance of class implementing interface {@link com.iha.emulator.communication.protocol.Protocol} by given
     * protocol version element from enum {@link com.iha.emulator.communication.protocol.Protocol.Version}.
     * @param version protocol version element from enum {@link com.iha.emulator.communication.protocol.Protocol.Version}
     * @return instance of class implementing interface {@link com.iha.emulator.communication.protocol.Protocol}
     * @throws IllegalArgumentException if cannot create new instance of implemented protocol class located in {@link com.iha.emulator.communication.protocol.implemented}
     */
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
