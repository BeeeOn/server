package com.iha.emulator.communication.server;

/**
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class WrongResponseException extends Exception{

    public WrongResponseException() {
        super();
    }
    /**
     * @param message explanation
     */
    public WrongResponseException(String message) {
        super(message);
    }
    /**
     * @param message explanation
     * @param t cause
     */
    public WrongResponseException(String message,Throwable t) {
        super(message,t);
    }
}
