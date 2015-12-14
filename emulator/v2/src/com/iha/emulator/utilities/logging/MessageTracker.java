package com.iha.emulator.utilities.logging;

import javafx.beans.property.LongProperty;
import javafx.beans.property.SimpleLongProperty;

/**
 * Class providing methods to count and save number unsent and sent messages by adapter.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class MessageTracker {
    /** enabled flag */
    private boolean enabled = false;
    /** sent messages counter property */
    private LongProperty sentMessageCounter;
    /** waiting messages counter property */
    private LongProperty waitingMessageCounter;

    /**
     * Creates and initializes new instance of message tracker
     */
    public MessageTracker() {
        this(true);
    }

    /**
     * Creates and initializes new instance of message tracker. Also sets, if tracker is enabled
     * @param enabled <code>true</code> if tracker is enabled, <code>false</code> otherwise
     */
    public MessageTracker(boolean enabled) {
        this.sentMessageCounter = new SimpleLongProperty(0);
        this.waitingMessageCounter = new SimpleLongProperty(0);
        this.enabled = enabled;
    }

    /**
     * If message tracker is enabled, increments sent messages counter
     */
    public synchronized void incrementSentCounter(){
        if(!isEnabled()) return;
        setSentMessageCounter(getSentMessageCounter()+1);
    }
    /**
     * If message tracker is enabled, increments waiting messages counter
     */
    public synchronized void incrementWaitingCounter(){
        if(!isEnabled()) return;
        setWaitingMessageCounter(getWaitingMessageCounter()+1);
    }
    /**
     * If message tracker is enabled, decrements waiting messages counter
     */
    public synchronized void decrementWaitingCounter(){
        if(!isEnabled()) return;
        setWaitingMessageCounter(getWaitingMessageCounter()-1);
    }

    /**
     * Sets counters to initial value (0)
     */
    public void clearCounters(){
        setSentMessageCounter(0);
        setWaitingMessageCounter(0);
    }

    /**
     * Gets flag, if message tracker is enabled
     * @return <code>true</code> if enabled,<code>false</code> otherwise
     */
    public boolean isEnabled() {
        return enabled;
    }

    /**
     * Sets flag, if message tracker is enabled
     * @param enabled <code>true</code> if enabled,<code>false</code> otherwise
     */
    public void setEnabled(boolean enabled) {
        this.enabled = enabled;
    }

    /**
     * Gets number of sent messages
     * @return number of sent messages
     */
    public long getSentMessageCounter() {
        return sentMessageCounter.get();
    }

    /**
     * Number of sent messages property, can be bound
     * @return number of sent messages property
     */
    public LongProperty sentMessageCounterProperty() {
        return sentMessageCounter;
    }

    /**
     * Sets number of sent messages
     * @param sentMessageCounter number of sent messages
     */
    public void setSentMessageCounter(long sentMessageCounter) {
        this.sentMessageCounter.set(sentMessageCounter);
    }

    /**
     * Gets number of waiting messages
     * @return number of waiting messages
     */
    public long getWaitingMessageCounter() {
        return waitingMessageCounter.get();
    }

    /**
     * Number of waiting messages property, can be bound
     * @return number of waiting messages property
     */
    public LongProperty waitingMessageCounterProperty() {
        return waitingMessageCounter;
    }

    /**
     * Sets number of waiting messages
     * @param waitingMessageCounter number of waiting messages
     */
    public void setWaitingMessageCounter(long waitingMessageCounter) {
        this.waitingMessageCounter.set(waitingMessageCounter);
    }
}
