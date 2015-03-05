package com.iha.emulator.utilities.handler;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 * Created by Shu on 5.3.2015.
 */
public class ErrorHandler {

    private List<ErrorListener> listeners = new ArrayList<>();

    public synchronized void addErrorListener(ErrorListener l){
        listeners.add(l);
    }

    public synchronized void removeErrorListener(ErrorListener l){
        listeners.remove(l);
    }

    public synchronized void fireErrorEvent(){
        ErrorEvent event = new ErrorEvent(this);
        Iterator<ErrorListener> listenerIterator = listeners.iterator();
        while(listenerIterator.hasNext()){
            ((ErrorListener)listenerIterator.next()).handleError();
        }
    }
}
