package com.iha.emulator.control.scheduler;

import com.iha.emulator.communication.server.OutMessage;
import com.iha.emulator.utilities.watchers.ResponseTracker;

/**
 * Created by Shu on 18/03/15.
 */
public interface Scheduler {
    public enum Type {
        DETAILED,
        PERFORMANCE
    }
    public void clearResponseTracker();
    public void processTracking(OutMessage message);
    public void startProcessing();
    public void stopProcessing();
    public void terminateScheduler(boolean clearResponses);
    public ResponseTracker getResponseTracker();
    public void setResponseTracker(ResponseTracker responseTracker);
    public boolean isContinueProcessing();
    public void setDaemon(boolean b);
    public void start();
    public boolean isAlive();
    public void interrupt();
    public void join() throws InterruptedException;
    public String getName();
    public Type getType();
}
