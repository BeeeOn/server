package com.iha.emulator.utilities.handlers;

import java.util.EventObject;

/**
 * Event raised when error occurred
 */
public class ErrorEvent extends EventObject {
    /**
     * Creates new error event
     * @param source source of error
     */
    public ErrorEvent(Object source) {
        super(source);
    }
}
