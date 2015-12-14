package com.iha.emulator.control.scheduler.implemented;

import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.communication.server.OutMessage;
import com.iha.emulator.communication.server.WrongResponseException;
import com.iha.emulator.control.AdapterController;
import com.iha.emulator.control.scheduler.Scheduler;
import com.iha.emulator.utilities.watchers.ResponseTracker;
import javafx.application.Platform;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.dom4j.Document;
import org.dom4j.DocumentException;

import javax.net.ssl.SSLHandshakeException;
import java.io.IOException;
import java.net.ConnectException;
import java.net.UnknownHostException;
import java.nio.channels.ClosedByInterruptException;
/**
 * Implementation of interface {@link com.iha.emulator.control.scheduler.Scheduler} providing means to send and process
 * messages between server and emulator. Uses temporary and static connection to server provided by
 * {@link com.iha.emulator.communication.server.ssl.ServerReceiver} and {@link com.iha.emulator.communication.server.ssl.ServerController}.
 *
 * @see com.iha.emulator.control.scheduler.Scheduler
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class DetailedScheduler extends Thread implements Scheduler{
    /** default sleep interval when internet connection is disabled */
    private static final int SLEEP_INTERVAL = 300;
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(DetailedScheduler.class);
    /** default scheduler type */
    public static final Type TYPE = Type.DETAILED;
    /** termination indicator */
    private boolean terminate = false;
    /** processing indicator */
    private boolean continueProcessing = true;
    /** response tracker instance */
    private ResponseTracker responseTracker  = new ResponseTracker(false,true,false);
    /** adapter controller, that uses this scheduler instance */
    private AdapterController adapterController;
    /**
     * Creates instance of scheduler, that uses both stable and temporary connection to server.
     *
     * @param adapterController adapter controller, that will scheduler
     */
    public DetailedScheduler(AdapterController adapterController){
        this.adapterController = adapterController;
        setName("Scheduler/" + adapterController.getAdapter().getId());
    }

    /**
     * <p>{@inheritDoc}</p>
     *
     * <p>
     * Scheduler processes messages saved in {@link #adapterController}. <br>
     * If register message is to be sent, uses stable connection provided by
     * {@link com.iha.emulator.communication.server.ssl.ServerReceiver}. If this message is successfully
     * sent, initiates listening on stable connection. <br>
     * If common sensor message is to be sent, uses temporary connection provided by
     * {@link com.iha.emulator.communication.server.ssl.ServerController}.
     * </p>
     */
    public void run(){
        while(!terminate){
            //check if server is created and internet connection is enabled
            if(adapterController.getServerController() != null && adapterController.getInternetConnection()){
                //get next message for server
                OutMessage message = adapterController.getNextMessage();
                //if message exists
                if(message != null){
                    logger.trace("Scheduler processing message: " + message.getType().toString());
                    try {
                        String inMessageString;
                        //if register message, send throw stable connection
                        if(message.getType() == OutMessage.Type.REGISTER_ADAPTER && adapterController.getServerReceiver() != null){
                            inMessageString = adapterController.getServerReceiver().sendMessage(message.getSocketMessage(),responseTracker,message.getType());
                        }else{
                            //else send throw temporary connection
                            inMessageString = adapterController.getServerController().sendMessage(message.getSocketMessage(),responseTracker,message.getType());
                        }
                        //check if adapter was not already terminated
                        if(adapterController != null && adapterController.getLog() != null && adapterController.getScheduler() != null){
                            logger.trace("Message to server: \n" + message.getSocketMessage().asXML());
                            //process response tracking for current message
                            processTracking(message);
                            //check protocol version
                            Protocol protocol = adapterController.getAdapter().getProtocol();
                            //convert incoming message to XML and check protocol version
                            Document inDocument = protocol.checkProtocolVersion(protocol.convertInMessageToXML(inMessageString));
                            logger.trace("Message from server: \n" + inDocument.asXML());
                            //depending on message type process server's response
                            switch (message.getType()){
                                case SENSOR_MESSAGE:
                                    //write message to adapter's log
                                    Platform.runLater(() -> adapterController.getLog().log(message.getLogMessage()));
                                    //parse message from server
                                    protocol.parseInSensorMessage(inDocument, message.getSenderController(), adapterController);
                                    //notify adapter, that message was successfully sent
                                    adapterController.messageSuccessfullySent(message);
                                    break;
                                case REGISTER_ADAPTER:
                                    //parse response from server
                                    protocol.parseInAdapterMessage(inDocument,adapterController);
                                    //write message to adapter's log
                                    Platform.runLater(() -> adapterController.getLog().log(message.getLogMessage()));
                                    //start listening for messages from server on stable connection
                                    if(adapterController.getServerReceiver() != null) {
                                        adapterController.getServerReceiver().initialize();
                                    }
                                    //notify adapter, that message was successfully sent
                                    adapterController.messageSuccessfullySent(message);
                                    break;
                            }
                        }
                    } catch (IllegalArgumentException ie) {
                        switch (message.getType()){
                            case SENSOR_MESSAGE:
                                //critical error occurred while sending message. Sensor, which tried to send this message
                                //will be stopped as a precaution of next failure
                                message.getSenderController().criticalErrorStop(
                                        "Error: " + message.getSenderController().toString() + " -> " + ie.getMessage(),
                                        "Error: " + message.getSenderController().toString() + " -> " + ie.getMessage(),
                                        message
                                );
                                break;
                            case REGISTER_ADAPTER:
                                //cannot register adapter, so it will be disabled
                                adapterController.sendError("Warning: " + adapterController.toString() ,ie,false);
                                Platform.runLater(adapterController::disable);
                                break;
                            default:
                                logger.fatal("Cannot determine message type");
                                break;
                        }
                    } catch (WrongResponseException we) {
                        switch (message.getType()){
                            case SENSOR_MESSAGE:
                                //usually socket stream was unexpectedly ended
                                message.getSenderController().criticalErrorStop(
                                        message.getSenderController().toString() + " --> " + we.getMessage() + "! Stopping sensor",
                                        message.getSenderController().toString() + " --> " + we.getMessage() + "! Stopping sensor",
                                        message
                                );
                                break;
                            case REGISTER_ADAPTER:
                                //usually socket stream was unexpectedly ended
                                adapterController.sendError(adapterController.toString(), we, false);
                                break;
                            default:
                                logger.fatal("Cannot determine message type");
                                break;
                        }
                    } catch (ClosedByInterruptException e){
                        //scheduler's thread was interrupted while waiting on socket or while waiting, terminating scheduler
                        logger.trace(getName() + " closed by interrupt");
                        terminateScheduler(false);
                    } catch (SSLHandshakeException e){
                        Platform.runLater(()->{
                            adapterController.disable();
                            adapterController.sendError(getName(),
                                    e,
                                    false);
                        });
                    }catch (UnknownHostException e){
                        //cannot connect to given hostname or IP
                        adapterController.sendError("Unknown host for " + adapterController.getServerController().getModel().getIp() + ". Please check IP address in server details.", e, false);
                        Platform.runLater(adapterController::disable);
                    } catch (IOException ioe) {
                        try{
                            switch (message.getType()){
                                case SENSOR_MESSAGE:
                                    //cannot create connection, maybe server is down?
                                    adapterController.sendError("Warning: Cannot connect to server ( " + message.getSenderController().toString() + ")!", ioe, false);
                                    break;
                                case REGISTER_ADAPTER:
                                    if(ioe instanceof ConnectException){
                                        //server probably not running
                                        Platform.runLater(adapterController::disable);
                                        adapterController.sendError("Error: Cannot connect to server ( " + getName() + " -> server probably not running)!", ioe, false);
                                    }else{
                                        //server rejected new message, probably busy, will try to send message next time
                                        adapterController.sendError("Warning: Cannot connect to server ( " + getName() + " -> server is probably busy )!", ioe, false);
                                    }
                                    break;
                                default:
                                    logger.fatal("Cannot determine message type");
                                    break;
                            }
                            Platform.runLater(() -> adapterController.getServerController().getModel().setConn(false));
                        }catch (NullPointerException e){
                            //some threads may not know, they are terminated and their data are cleaned
                        }
                    } catch (DocumentException e) {
                        //cannot parse response from server, not in XML format
                        switch (message.getType()){
                            case SENSOR_MESSAGE:
                                message.getSenderController().criticalErrorStop(
                                        "Error: Adapter/" + adapterController.getAdapter().getId() + " -> Sensor/" + message.getSenderController().getModel().getId() + " --> Cannot parse XML response",
                                        "Error: " + message.getSenderController().getModel().getName() + "/" + message.getSenderController().getModel().getId() + " --> Cannot parse XML response",
                                        message
                                );
                                break;
                            case REGISTER_ADAPTER:
                                Platform.runLater(adapterController::disable);
                                adapterController.sendError("Cannot parse server XML response (REGISTER MESSAGE)",e,false);
                                break;
                            default:
                                logger.fatal("Cannot determine message type");
                                break;
                        }
                    }
                }else {
                    //if there are no new messages, to be sent, scheduler is paused
                    stopProcessing();
                }
            }else{
                //internet connection is disabled, sleep for a while and check again
                if(adapterController.getServerController() != null){
                    Platform.runLater(() -> adapterController.getServerController().getModel().setConn(false));
                }
                try{
                    logger.trace("Scheduler sleeping...");
                    Thread.sleep(SLEEP_INTERVAL);
                }catch (InterruptedException e){
                    Platform.runLater(() -> adapterController.getLog().error("Cannot put scheduler to sleep, while waiting for server!",true));
                }
            }
            logger.trace("Scheduler/" + adapterController.getAdapter().getId() + " checking if I should proceed");
            if(!continueProcessing){
                //thread paused and waiting to be started again
                logger.trace(getName()+" -> Don't proceed. Start to wait for new messages");
                synchronized (this){
                    while(!continueProcessing)
                        try {
                            wait();
                        } catch (InterruptedException e) {
                            terminateScheduler(false);
                            break;
                            /*Platform.runLater(() -> adapterController.getLog().error("Cannot put scheduler to wait while waiting for new messages",true));*/
                        }
                }
            }
            logger.trace("Proceed");
        }
        logger.trace(getName() + " terminated.");
    }
    /**
     * {@inheritDoc}
     */
    public void clearResponseTracker(){
        if(responseTracker == null) return;
        responseTracker.clearResponses();
    }
    /**
     * {@inheritDoc}
     */
    public synchronized void processTracking(OutMessage message){
        //if is tracking on, save to end of successful message response time
        if(responseTracker.isEnabled()){
            message.appendToMsg(" -> " + responseTracker.getLastResponseValue() + "ms");
        }
        //if there is no need for responses, dump them
        if(responseTracker.isDumpResponses()) responseTracker.dumpResponses();
    }
    /**
     * {@inheritDoc}
     */
    public synchronized void startProcessing(){
        //if scheduler is paused
        if(!isContinueProcessing()){
            logger.trace("Scheduler/" + adapterController.getAdapter().getId() + " starting scheduler processing");
            //indicate processing start
            this.continueProcessing = true;
            //notify thread
            notify();
        }
    }
    /**
     * {@inheritDoc}
     */
    public synchronized void stopProcessing(){
        //if scheduler is running
        if(isContinueProcessing()){
            logger.trace("Scheduler/" + adapterController.getAdapter().getId() + " stopping scheduler processing");
            //indicate processing stop
            this.continueProcessing = false;
        }
    }
    /**
     * {@inheritDoc}
     */
    public void terminateScheduler(boolean clearResponses){
        startProcessing();
        this.terminate = true;
        if(clearResponses && responseTracker != null)
            responseTracker.delete();
    }
    /**
     * {@inheritDoc}
     */
    public ResponseTracker getResponseTracker(){
        return responseTracker;
    }

    /**
     * {@inheritDoc}
     */
    public void setResponseTracker(ResponseTracker responseTracker){
        this.responseTracker = responseTracker;
    }
    /**
     * {@inheritDoc}
     */
    public boolean isContinueProcessing(){
        return this.continueProcessing;
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public Type getType() {
        return TYPE;
    }
}
