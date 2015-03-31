package com.iha.emulator.control.scheduler.implemented;

import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.communication.server.OutMessage;
import com.iha.emulator.communication.server.WrongResponseException;
import com.iha.emulator.communication.server.ssl.ServerController;
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
import java.net.UnknownHostException;
import java.nio.channels.ClosedByInterruptException;

public class PerformanceScheduler extends Thread implements Scheduler{

    private static final int SLEEP_INTERVAL = 300;
    private static final Logger logger = LogManager.getLogger(PerformanceScheduler.class);
    public static final Type TYPE = Type.PERFORMANCE;

    private boolean terminate = false;
    private boolean continueProcessing = true;

    private ResponseTracker responseTracker  = new ResponseTracker(false);

    private AdapterController adapterController;

    public PerformanceScheduler(AdapterController adapterController){
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
                        String inMessageString = adapterController.getServerController().sendMessage(message.getSocketMessage(),responseTracker,message.getType());
                        //check if adapter was not already terminated
                        if(adapterController != null && adapterController.getLog() != null && adapterController.getScheduler() != null){
                            if(ServerController.DEBUG){
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
                                        Platform.runLater(() -> {
                                            adapterController.getLog().log(message.getLogMessage());
                                        });
                                        adapterController.messageSuccessfullySent(message);
                                        break;
                                }
                            }else{
                                switch (message.getType()){
                                    case SENSOR_MESSAGE:
                                        logger.trace("Message to server: \n" + message.getSocketMessage().asXML());
                                        processTracking(message);
                                        Platform.runLater(() -> adapterController.getLog().log(message.getLogMessage()));
                                        //check protocol version
                                        Protocol protocol = adapterController.getAdapter().getProtocol();
                                        //convert incoming message to XML and check protocol version
                                        Document inDocument = protocol.checkProtocolVersion(protocol.convertInMessageToXML(inMessageString));
                                        logger.trace("Message from server: \n" + inDocument.asXML());
                                        protocol.parseInSensorMessage(inDocument, message.getSenderController(), adapterController);
                                        adapterController.messageSuccessfullySent(message);
                                        break;
                                    case REGISTER_ADAPTER:
                                        processTracking(message);
                                        Platform.runLater(() -> {
                                            adapterController.getLog().log(message.getLogMessage());
                                            adapterController.getAdapter().setRegistered(true);
                                        });
                                        adapterController.messageSuccessfullySent(message);
                                        break;
                                }
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
                    } catch (SSLHandshakeException e){
                        Platform.runLater(()-> adapterController.sendError(getName(),
                                e,
                                true));
                        terminateScheduler(false);
                    } catch (UnknownHostException e){
                        logger.trace(getName() + " unknown host for -> " + adapterController.getServerController().getModel().getIp());
                        Platform.runLater(()-> adapterController.sendError(
                                "Unknown host for" + adapterController.getServerController().getModel().getIp() + ". Please check IP address in server details.",
                                e,
                                false));
                        terminateScheduler(false);
                    }catch (IOException ioe) {
                        Platform.runLater(() -> {
                            //adapter register message
                            try{
                                if(message.getSenderController() == null){
                                    adapterController.sendError("Warning: Cannot connect to server ( " + getName() + " -> register message )!", ioe, false);
                                }else{
                                    adapterController.sendError("Warning: Cannot connect to server ( " + message.getSenderController().toString() + ")!", ioe, false);
                                }
                                adapterController.getServerController().getModel().setConn(false);
                            }catch (NullPointerException e){
                                //some threads may not know, they are terminated and their data are cleaned
                            }

                        });
                    } catch (DocumentException e) {
                        if(message.getSenderController() != null){
                            message.getSenderController().criticalErrorStop(
                                    "Error: Adapter/" + adapterController.getAdapter().getId() + " -> Sensor/" + message.getSenderController().getModel().getId() + " --> Cannot parse XML response",
                                    "Error: " + message.getSenderController().getModel().getName() + "/" + message.getSenderController().getModel().getId() + " --> Cannot parse XML response",
                                    message
                            );
                        }else{
                            Platform.runLater(() -> {
                                adapterController.disable();
                                adapterController.sendError("Cannot parse server XML response (REGISTER MESSAGE)", e, false);
                            });
                        }
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
