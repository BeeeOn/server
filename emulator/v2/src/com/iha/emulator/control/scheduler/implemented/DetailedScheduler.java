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

import java.io.IOException;
import java.net.ConnectException;
import java.nio.channels.ClosedByInterruptException;

public class DetailedScheduler extends Thread implements Scheduler{

    private static final int SLEEP_INTERVAL = 300;
    private static final Logger logger = LogManager.getLogger(DetailedScheduler.class);
    public static final Type TYPE = Type.DETAILED;


    private boolean terminate = false;
    private boolean continueProcessing = true;

    private ResponseTracker responseTracker  = new ResponseTracker(false);

    private AdapterController adapterController;

    public DetailedScheduler(AdapterController adapterController){
        this.adapterController = adapterController;
        setName("Scheduler/" + adapterController.getAdapter().getId());
    }

    public void run(){
        while(!terminate){
            if(adapterController.getServerController() != null && adapterController.getInternetConnection()){
                OutMessage message = adapterController.getNextMessage();
                if(message != null){
                    logger.trace("Scheduler processing message: " + message.getType().toString());
                    try {
                        String inMessageString;
                        if(message.getType() == OutMessage.Type.REGISTER_ADAPTER && adapterController.getServerReceiver() != null){
                            inMessageString = adapterController.getServerReceiver().sendMessage(message.getSocketMessage(),responseTracker,message.getType());
                        }else{
                            inMessageString = adapterController.getServerController().sendMessage(message.getSocketMessage(),responseTracker,message.getType());
                        }
                        //check if adapter was not already terminated
                        if(adapterController != null && adapterController.getLog() != null && adapterController.getScheduler() != null){
                            logger.trace("Message to server: \n" + message.getSocketMessage().asXML());
                            processTracking(message);
                            //check protocol version
                            Protocol protocol = adapterController.getAdapter().getProtocol();
                            //convert incoming message to XML and check protocol version
                            Document inDocument = protocol.checkProtocolVersion(protocol.convertInMessageToXML(inMessageString));
                            logger.trace("Message from server: \n" + inDocument.asXML());
                            switch (message.getType()){
                                case SENSOR_MESSAGE:
                                    Platform.runLater(() -> adapterController.getLog().log(message.getLogMessage()));
                                    protocol.parseInSensorMessage(inDocument, message.getSenderController(), adapterController);
                                    adapterController.messageSuccessfullySent(message);
                                    break;
                                case REGISTER_ADAPTER:
                                    protocol.parseInAdapterMessage(inDocument,adapterController);
                                    Platform.runLater(() -> adapterController.getLog().log(message.getLogMessage()));
                                    //start listening for messages from server
                                    if(adapterController.getServerReceiver() != null) {
                                        adapterController.getServerReceiver().initialize();
                                    }
                                    adapterController.messageSuccessfullySent(message);
                                    break;
                            }
                        }
                    } catch (IllegalArgumentException ie) {
                        if(message.getSenderController() != null){
                            message.getSenderController().criticalErrorStop(
                                    "Error: " + message.getSenderController().toString() + " -> " + ie.getMessage(),
                                    "Error: " + message.getSenderController().toString() + " -> " + ie.getMessage(),
                                    message
                            );
                        }else{
                            Platform.runLater(()->{
                                adapterController.sendError("Warning: " + adapterController.toString() ,ie,false);
                                adapterController.disable();
                            });
                        }
                    } catch (WrongResponseException we) {
                        if(message.getSenderController() != null){
                            message.getSenderController().criticalErrorStop(
                                    message.getSenderController().toString()+ " --> socket closed or wrong response from server! Stopping sensor",
                                    message.getSenderController().toString() + " --> socket closed or wrong response from server! Stopping sensor",
                                    message
                            );
                        } else {
                            adapterController.sendError(adapterController.toString(),we,false);
                        }
                    } catch (ClosedByInterruptException e){
                        logger.trace(getName() + " closed by interrupt");
                        terminateScheduler(false);
                    } catch (IOException ioe) {
                        Platform.runLater(() -> {
                            //adapter register message
                            try{
                                if(message.getSenderController() == null){
                                    if(ioe instanceof ConnectException){
                                        adapterController.disable();
                                        adapterController.sendError("Error: Cannot connect to server ( " + getName() + " -> server probably not running)!", ioe, false);
                                    }else{
                                        adapterController.sendError("Warning: Cannot connect to server ( " + getName() + " -> register message )!", ioe, false);
                                    }
                                }else{
                                    adapterController.sendError("Warning: Cannot connect to server ( " + message.getSenderController().toString() + ")!", ioe, false);
                                }
                                adapterController.getServerController().getModel().setConn(false);
                            }catch (NullPointerException e){
                                //some threads may not know, they are terminated and their data are cleaned
                            }

                        });
                    } catch (DocumentException e) {
                        if(message.getSenderController() != null) message.getSenderController().criticalErrorStop(
                                "Error: Adapter/" + adapterController.getAdapter().getId() + " -> Sensor/" + message.getSenderController().getSensorIdAsIp() + " --> Cannot parse XML response",
                                "Error: " + message.getSenderController().getModel().getName() + "/" + message.getSenderController().getSensorIdAsIp() + " --> Cannot parse XML response",
                                message
                        );
                    }
                }else {
                    stopProcessing();
                }
            }else{
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
            //thread paused and waiting to be started again
            logger.trace("Scheduler/" + adapterController.getAdapter().getId() + " checking if I should proceed");
            if(!continueProcessing){
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

    public void clearResponseTracker(){
        if(responseTracker == null) return;
        responseTracker.clearResponses();
    }

    public synchronized void processTracking(OutMessage message){
        //if is tracking on, save to end of successful message response time
        if(responseTracker.isEnabled()){
            message.appendToMsg(" -> " + responseTracker.getLastResponseValue() + "ms");
        }
        //if there is no need for responses, dump them
        if(responseTracker.isDumpResponses()) responseTracker.dumpResponses();
    }

    /**
     * Sets process indicator to start and notifies thread if it is waiting
     */
    public synchronized void startProcessing(){
        if(!isContinueProcessing()){
            logger.trace("Scheduler/" + adapterController.getAdapter().getId() + " starting scheduler processing");
            this.continueProcessing = true;
            notify();
        }
    }
    /**
     * Sets process indicator to stop
     */
    public synchronized void stopProcessing(){
        if(isContinueProcessing()){
            logger.trace("Scheduler/" + adapterController.getAdapter().getId() + " stopping scheduler processing");
            this.continueProcessing = false;
        }
    }
    /**
     * Makes scheduler thread to finish
     */
    public void terminateScheduler(boolean clearResponses){
        startProcessing();
        this.terminate = true;
        if(clearResponses && responseTracker != null)
            responseTracker.delete();
    }

    public ResponseTracker getResponseTracker(){
        return responseTracker;
    }

    public void setResponseTracker(ResponseTracker responseTracker){
        this.responseTracker = responseTracker;
    }
    //region getters and setters
    /**
     * Returns <code>true</code> if scheduler is processing messages, <code>false</code> otherwise
     * @return <code>true</code> if scheduler is processing messages, <code>false</code> otherwise
     */
    public boolean isContinueProcessing(){
        return this.continueProcessing;
    }

    @Override
    public Type getType() {
        return TYPE;
    }
}
