package com.iha.emulator.control.scheduler;

import com.iha.emulator.communication.server.OutMessage;
import com.iha.emulator.utilities.watchers.ResponseTracker;

/**
 * Interface used for scheduling sending messages to server.
 *
 * @see com.iha.emulator.control.scheduler.implemented
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public interface Scheduler {
    /**
     * Enum with scheduler types
     */
    public enum Type {
        DETAILED,
        PERFORMANCE
    }
    /**
     * Method called after scheduler's thread is started. Providing logic to send and process messages between server
     * and emulator.
     */
    public void run();
    /**
     * Clears saved responses
     */
    public void clearResponseTracker();
    /**
     * If response tracking is enabled, saves response time to adapter's log
     * message. Also if response dumping is enabled, clears this response.
     * @param message message to server containing adapter's log message
     */
    public void processTracking(OutMessage message);
    /**
     * Sets process indicator to start and notifies thread if it is waiting
     */
    public void startProcessing();
    /**
     * Sets process indicator to stop. Because of this indicator scheduler puts
     * it's thread to waiting state.
     */
    public void stopProcessing();
    /**
     * Clears all saved responses in assigned {@link com.iha.emulator.utilities.watchers.ResponseTracker} and terminates
     * scheduler's thread.
     * @param clearResponses <code>true</code> if responses should be cleared, <code>false</code> otherwise
     */
    public void terminateScheduler(boolean clearResponses);
    /**
     * Gets assigned response tracker class.
     * @return response tracker
     */
    public ResponseTracker getResponseTracker();
    /**
     * Sets response tracker for scheduler
     * @param responseTracker response tracker
     */
    public void setResponseTracker(ResponseTracker responseTracker);
    /**
     * Indicates, whether scheduler is currently processing messages.
     * @return <code>true</code> if processing, <code>false</code> otherwise
     */
    public boolean isContinueProcessing();
    /**
     * Sets scheduler's thread as daemon thread
     * @param b <code>true</code> if wish to set scheduler's thread as daemon, <code>false</code> otherwise
     */
    public void setDaemon(boolean b);
    /**
     * Starts scheduler's thread
     */
    public void start();
    /**
     * Checks, if scheduler's thread is alive
     * @return <code>true</code> if is alive, <code>false</code> otherwise
     */
    public boolean isAlive();
    /**
     * Interrupts scheduler's thread
     */
    public void interrupt();
    /**
     * Joins scheduler's thread with thread, that created it
     * @throws InterruptedException interrupted while waiting on parent thread
     */
    public void join() throws InterruptedException;
    /**
     * Gets scheduler's thread name
     * @return scheduler's thread name
     */
    public String getName();
    /**
     * Gets scheduler's type
     * @return scheduler's type
     */
    public Type getType();
}
