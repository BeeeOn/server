package com.iha.emulator.utilities.logging;

import javafx.beans.property.LongProperty;
import javafx.beans.property.SimpleLongProperty;

/**
 * Created by Shu on 6.3.2015.
 */
public class MessageTracker {

    private boolean enabled = false;
    private LongProperty sentMessageCounter;
    private LongProperty waitingMessageCounter;

    public MessageTracker() {
        this(true);
    }

    public MessageTracker(boolean enabled) {
        this.sentMessageCounter = new SimpleLongProperty(0);
        this.waitingMessageCounter = new SimpleLongProperty(0);
        this.enabled = enabled;
    }

    public synchronized void incrementSentCounter(){
        if(!isEnabled()) return;
        setSentMessageCounter(getSentMessageCounter()+1);
    }

    public synchronized void incrementWaitingCounter(){
        if(!isEnabled()) return;
        setWaitingMessageCounter(getWaitingMessageCounter()+1);
    }

    public synchronized void decrementWaitingCounter(){
        if(!isEnabled()) return;
        setWaitingMessageCounter(getWaitingMessageCounter()-1);
    }

    public void clearCounters(){
        setSentMessageCounter(0);
        setWaitingMessageCounter(0);
    }

    public boolean isEnabled() {
        return enabled;
    }

    public void setEnabled(boolean enabled) {
        this.enabled = enabled;
    }

    public long getSentMessageCounter() {
        return sentMessageCounter.get();
    }

    public LongProperty sentMessageCounterProperty() {
        return sentMessageCounter;
    }

    public void setSentMessageCounter(long sentMessageCounter) {
        this.sentMessageCounter.set(sentMessageCounter);
    }

    public long getWaitingMessageCounter() {
        return waitingMessageCounter.get();
    }

    public LongProperty waitingMessageCounterProperty() {
        return waitingMessageCounter;
    }

    public void setWaitingMessageCounter(long waitingMessageCounter) {
        this.waitingMessageCounter.set(waitingMessageCounter);
    }
}
