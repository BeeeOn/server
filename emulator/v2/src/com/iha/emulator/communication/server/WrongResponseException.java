package com.iha.emulator.communication.server;

/**
 * Created by Shu on 9.2.2015.
 */
public class WrongResponseException extends Exception{

    public WrongResponseException() {
        super();
    }

    public WrongResponseException(String message) {
        super(message);
    }

    public WrongResponseException(String message,Throwable t) {
        super(message,t);
    }
}
