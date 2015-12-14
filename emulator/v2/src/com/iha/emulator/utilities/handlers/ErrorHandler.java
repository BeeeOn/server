package com.iha.emulator.utilities.handlers;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 * Class controlling all listeners to raised error events.
 */
public class ErrorHandler {
    /** list of error event listeners */
    private List<ErrorListener> listeners = new ArrayList<>();

    /**
     * Adds new listener to list
     * @param l listener to be added
     */
    public synchronized void addErrorListener(ErrorListener l){
        listeners.add(l);
    }

    /**
     * Removes listener from list
     * @param l listener to be removed
     */
    public synchronized void removeErrorListener(ErrorListener l){
        listeners.remove(l);
    }

    /**
     * Fire new error event and notify listeners.
     */
    public synchronized void fireErrorEvent(){
        ErrorEvent event = new ErrorEvent(this);
        Iterator<ErrorListener> listenerIterator = listeners.iterator();
        while(listenerIterator.hasNext()){
            ((ErrorListener)listenerIterator.next()).handleError();
        }
    }
}
