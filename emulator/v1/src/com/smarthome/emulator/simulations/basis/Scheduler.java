package com.smarthome.emulator.simulations.basis;

import com.smarthome.emulator.Emulator;
import com.smarthome.emulator.simulations.Error;
import com.smarthome.emulator.simulations.panels.ServerPanel;
import java.nio.ByteBuffer;

/**
 * Takes messages stored in {@link com.smarthome.emulator.simulations.basis.Adapter} instance and sends them to server.
 * Extends {@link java.lang.Thread}
 */
public class Scheduler extends Thread {
    /** indicator of scheduler processing */
    private boolean proceed = true;
    /** indicator of scheduler termination */
    private boolean terminate = false;
    /** {@link com.smarthome.emulator.simulations.basis.Adapter} that created this scheduler*/
    private Adapter adapter;
    /** {@link com.smarthome.emulator.simulations.basis.Logger} logger for scheduler */
    private Logger logger;
    /** {@link com.smarthome.emulator.simulations.basis.Server} instance */
    private Server server = null;
    /**  Initial response start time */
    private long responseStart = 0L;
    /**  Thread sleep time when cannot connect to server */
    private static final int SLEEP_INTERVAL = 300;
    /**
     * Assign {@link com.smarthome.emulator.simulations.basis.Adapter} that created scheduler and
     * {@link com.smarthome.emulator.simulations.basis.Logger} in which should scheduler write messages
     * @param logger
     * @param adapter
     */
    public Scheduler(Logger logger,Adapter adapter) {
        this.logger = logger;
        this.adapter = adapter;
    }
    /**
     * Ancestor class {@link java.lang.Thread} implementation. Runs until <code>terminate</code> is set to <code>true</code>.
     * If there is no internet connection or server is unreachable, thread sleeps for {@link com.smarthome.emulator.simulations.basis.Scheduler#SLEEP_INTERVAL}
     * before trying to send message. If there are no messages to send, thread is put to wait.
     * <p>
     *     Shows errors:
     *     <ul>
     *         <li>SCHEDULER_SLEEP if failed to put thread to sleep</li>
     *         <li>SCHEDULER_WAIT if failed to put thread to wait</li>
     *     </ul>
     * </p>
     */
    public void run(){
        while(!terminate){
                if((server != null && adapter.getInternetConnection())){
                    //get next message in list
                    Message m = adapter.getNextMessage();
                    if(m != null){
                        //if tracking responses, set response start time
                        //if(logger.isTrackResponse()) responseStart = System.currentTimeMillis();
                        boolean wait = true;
                        if(m.getSender() == null && m.getLogMessage() != null && m.getSocketMessage() != null){
                            wait = false;
                        }
                        //send message
                        ByteBuffer inMessage = server.sendMessage(m.socketMessage,wait);
                        //set response end time
                        //if(logger.isTrackResponse()) logger.addResponse(responseStart,System.currentTimeMillis());
                        //if there is message from server
                        if(m.getSender() != null && inMessage != null){
                            //if is tracking on, add to end of successful message response time
                            if(logger.isTrackResponse()){
                                m.appendToMsg(" -> " +String.valueOf(logger.getLastResponseValue()) + "ms");
                            }
                            //if there is no need for responses, dump them
                            if(logger.isDumpResponses()) logger.dumpResponses();
                            //message sent and received response successfully
                            logger.writeLogMessage(m.getLogMessage(),Logger.BOLD_STYLE);
                            //parse message from server
                            m.sender.parseInMessage(inMessage);
                            //delete message from list
                            adapter.messageSuccessfullySent();
                        }else if(m.getSender() == null && m.getLogMessage() != null && m.getSocketMessage() != null){
                            adapter.messageSuccessfullySent();
                            logger.writeLogMessage(m.getLogMessage(),Logger.BOLD_STYLE);
                        }else {
                            //bad response from server, stopping sensor
                            if(server.isWrongResponse()){
                                m.sender.criticalErrorStop();
                                //tells server that wrong response was dealt with
                                server.setWrongResponse(false);
                            }
                        }
                    }else{
                        //when there are no messages in list
                        stopProcessing();
                    }
                }else{
                    //when internet connection is off
                    server.setConnection(false);
                    try {
                        Thread.sleep(SLEEP_INTERVAL);
                    } catch (InterruptedException e) {
                        Emulator.showError(Error.SCHEDULER_SLEEP.getMessage());
                    }
                }
            //thread paused and waiting to be started again
            if(!proceed){
                synchronized (this){
                    while(!proceed)
                        try {
                            wait();
                        } catch (InterruptedException e) {
                            Emulator.showError(Error.SCHEDULER_WAIT.getMessage());
                        }
                }
            }
        }

    }
    /**
     * Creates {@link com.smarthome.emulator.simulations.basis.Server} instance from given parameters
     * @param panel server GUI panel,not null
     * @param givenName server hostname, null if <code>givenIp</code> is not null
     * @param givenIp server ip, null if <code>givenName</code> is not null
     * @param givenPort server port, not null
     */
    public void setServerParameters(ServerPanel panel,String givenName,String givenIp,int givenPort){
        server = new Server(panel, logger,givenName,givenIp,givenPort);
    }
    /**
     * Sets process indicator to start and notifies thread if it is waiting
     */
    public synchronized void startProcessing(){
        this.proceed = true;
        notify();
    }
    /**
     * Sets process indicator to stop
     */
    public synchronized void stopProcessing(){
        this.proceed = false;
    }
    /**
     * Makes scheduler thread to finish
     */
    public void terminateScheduler(){
        this.terminate = true;
        //if(Emulator.writeConsole) System.out.println("trying to terminate scheduler: " + adapter.getAdapterId());
    }
    //region getters and setters
    /**
     * Returns {@link com.smarthome.emulator.simulations.basis.Server} created in <code>setServerParameters</code> method
     * @return {@link com.smarthome.emulator.simulations.basis.Server} created in <code>setServerParameters</code> method
     */
    public Server getServer() {
        return server;
    }
    /**
     * Returns <code>true</code> if scheduler is processing messages, <code>false</code> otherwise
     * @return <code>true</code> if scheduler is processing messages, <code>false</code> otherwise
     */
    public boolean isProcessing(){
        return this.proceed;
    }
    //endregion
}
