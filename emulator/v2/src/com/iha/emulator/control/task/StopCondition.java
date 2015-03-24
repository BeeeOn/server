package com.iha.emulator.control.task;

import com.iha.emulator.utilities.Utilities;
import javafx.beans.property.IntegerProperty;
import javafx.beans.property.SimpleIntegerProperty;
import javafx.beans.value.ChangeListener;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.dom4j.Element;

import java.util.concurrent.atomic.AtomicBoolean;

/**
 * Created by Shu on 9.3.2015.
 */
public class StopCondition {

    private static final Logger logger = LogManager.getLogger(StopCondition.class);

    private SimulationTask task;

    private boolean timerEnabled = false;
    private boolean sentMessageEnabled = false;
    private boolean waitingMessageEnabled = false;

    private ChangeListener<Number> timerListener;
    private ChangeListener<Number> sentMessageListener;
    private ChangeListener<Number> waitingMessageListener;

    private long secondsToTrigger = 0;
    private long messageCountToTrigger = 0;
    private long waitingMessagesToTrigger = 0;

    private AtomicBoolean atomicTimerTriggered = new AtomicBoolean(false);
    private AtomicBoolean atomicSentMessageTriggered = new AtomicBoolean(false);
    private AtomicBoolean atomicWaitingMessageTriggered = new AtomicBoolean(false);

    private IntegerProperty conditionCounter;

    public StopCondition(SimulationTask task){
        this.task = task;
        this.conditionCounter = new SimpleIntegerProperty(0);
        initWatchers();
    }

    public void registerTimeDurationWatcher(long secondsToTrigger){
        if(task == null){
            logger.warn("No task assigned to StopCondition(null). Cannot register timer watcher!");
            return;
        }else if(task.getTaskParameters() == null){
            logger.warn("No task parameters(null). Cannot register timer watcher!");
            return;
        }else if(task.getTaskParameters().getStopWatch() == null){
            logger.warn("No stop watcher(null). Cannot register timer watcher!");
            return;
        }
        logger.trace("Registering timer stop condition: " + Utilities.formatSeconds(secondsToTrigger));
        this.timerEnabled = true;
        this.secondsToTrigger = secondsToTrigger;
        task.logMessage("Estimated task duration: " + Utilities.formatSeconds(secondsToTrigger) + "\n");
        task.getTaskParameters().getStopWatch().timeProperty().addListener(timerListener);
        incrementConditionCounter();
    }

    public void registerSentMessageWatcher(long messageCountToTrigger){
        if(task == null){
            logger.warn("No task assigned to StopCondition. Cannot register sent message watcher!");
            return;
        }else if(task.getLog() == null){
            logger.warn("No log(null). Cannot register sent message watcher!");
            return;
        }else if(task.getLog().getMessageTracker() == null){
            logger.warn("No message tracker(null). Cannot register sent message watcher!");
            return;
        }
        logger.trace("Registering sent message stop condition: " + messageCountToTrigger);
        this.sentMessageEnabled = true;
        this.messageCountToTrigger = messageCountToTrigger;
        task.logMessage("Estimated task's sent message count: " + messageCountToTrigger + "\n");
        task.getLog().getMessageTracker().sentMessageCounterProperty().addListener(sentMessageListener);
        incrementConditionCounter();
    }

    public void registerWaitingMessageWatcher(long waitingMessagesToTrigger){
        if(task == null){
            logger.warn("No task assigned to StopCondition. Cannot register waiting message watcher!");
            return;
        }else if(task.getLog() == null){
            logger.warn("No log(null). Cannot register waiting message watcher!");
            return;
        }else if(task.getLog().getMessageTracker() == null){
            logger.warn("No message tracker(null). Cannot register waiting message watcher!");
            return;
        }
        logger.trace("Registering waiting message stop condition: " + waitingMessagesToTrigger);
        this.waitingMessageEnabled = true;
        this.waitingMessagesToTrigger = waitingMessagesToTrigger;
        task.logMessage("Estimated task's waiting messages count: " + waitingMessagesToTrigger + "\n");
        task.getLog().getMessageTracker().waitingMessageCounterProperty().addListener(waitingMessageListener);
        incrementConditionCounter();
    }

    public void unregisterTimeDurationWatcher(){
        if(task == null){
            logger.warn("No task assigned to StopCondition. Cannot unregister timer watcher!");
            return;
        }else if(task.getTaskParameters() == null){
            logger.warn("No task parameters(null). Cannot unregister timer watcher!");
            return;
        }else if(task.getTaskParameters().getStopWatch() == null){
            logger.warn("No stop watcher(null). Cannot unregister timer watcher!");
            return;
        }
        logger.trace("Unregistering timer stop condition");
        this.timerEnabled = false;
        try{
            task.getTaskParameters().getStopWatch().timeProperty().removeListener(timerListener);
        }catch (NullPointerException e){

        }

        decrementConditionCounter();
    }

    public void unregisterSentMessageWatcher(){
        if(task == null){
            logger.warn("No task assigned to StopCondition. Cannot unregister sent message watcher!");
            return;
        }else if(task.getLog() == null){
            logger.warn("No log(null). Cannot unregister sent message watcher!");
            return;
        }else if(task.getLog().getMessageTracker() == null){
            logger.warn("No message tracker(null). Cannot unregister sent message watcher!");
            return;
        }
        logger.trace("Unregistering sent message stop condition");
        this.sentMessageEnabled = false;
        try{
            task.getLog().getMessageTracker().sentMessageCounterProperty().removeListener(sentMessageListener);
        }catch (NullPointerException e){

        }
        decrementConditionCounter();
    }

    public void unregisterWaitingMessageWatcher(){
        if(task == null){
            logger.warn("No task assigned to StopCondition. Cannot unregister waiting message watcher!");
            return;
        }else if(task.getLog() == null){
            logger.warn("No log(null). Cannot unregister waiting message watcher!");
            return;
        }else if(task.getLog().getMessageTracker() == null){
            logger.warn("No message tracker(null). Cannot unregister waiting message watcher!");
            return;
        }
        logger.trace("Unregistering waiting message stop condition");
        this.waitingMessageEnabled = false;
        try{
            task.getLog().getMessageTracker().waitingMessageCounterProperty().removeListener(waitingMessageListener);
        }catch (NullPointerException e){

        }
        decrementConditionCounter();
    }

    private void initWatchers(){
        this.timerListener = (observable, oldValue, newValue) -> {
            if(!atomicTimerTriggered.get() && secondsToTrigger <=(long)newValue){
                atomicTimerTriggered.set(true);
                task.logMessage("Duration stop condition met. Stopping task");
                task.stop();
            }
        };
        this.sentMessageListener = (observable, oldValue, newValue) -> {
            if(!atomicSentMessageTriggered.get() && messageCountToTrigger <= (long)newValue){
                atomicSentMessageTriggered.set(true);
                task.logMessage("Sent messages count stop condition met. Stopping task");
                task.stop();
            }
        };
        this.waitingMessageListener = (observable, oldValue, newValue) -> {
            if(!atomicWaitingMessageTriggered.get() && waitingMessagesToTrigger <= (long)newValue){
                atomicWaitingMessageTriggered.set(true);
                task.logMessage("Waiting messages count stop condition met. Stopping task");
                task.stop();
            }
        };
    }

    public void restart(){
        if(timerEnabled) atomicTimerTriggered.set(false);
        if(sentMessageEnabled) atomicSentMessageTriggered.set(false);
        if(waitingMessageEnabled) atomicWaitingMessageTriggered.set(false);
    }

    public void unregisterAllConditions(){
        unregisterTimeDurationWatcher();
        unregisterSentMessageWatcher();
        unregisterWaitingMessageWatcher();
    }

    public Element saveToXml(Element rootElement){
        Element stopConditionElement = rootElement.addElement("stop_conditions");
        //time duration
        stopConditionElement.addElement("time_duration")
                .addAttribute("enabled",String.valueOf(isTimerEnabled()))
                .addAttribute("sec", String.valueOf(getSecondsToTrigger()));
        //sent messages
        stopConditionElement.addElement("sent_messages")
                .addAttribute("enabled",String.valueOf(isSentMessageEnabled()))
                .addAttribute("count",String.valueOf(getMessageCountToTrigger()));
        //sent messages
        stopConditionElement.addElement("waiting_messages")
                .addAttribute("enabled",String.valueOf(isWaitingMessageEnabled()))
                .addAttribute("count",String.valueOf(getWaitingMessagesToTrigger()));
        return stopConditionElement;
    }

    private synchronized void incrementConditionCounter(){
        setConditionCounter(getConditionCounter()+1);
    }

    private synchronized void decrementConditionCounter(){
        setConditionCounter(getConditionCounter()-1);
    }

    public boolean isTimerEnabled() {
        return timerEnabled;
    }

    public boolean isSentMessageEnabled() {
        return sentMessageEnabled;
    }

    public boolean isWaitingMessageEnabled() {
        return waitingMessageEnabled;
    }

    public int getConditionCounter() {
        return conditionCounter.get();
    }

    public IntegerProperty conditionCounterProperty() {
        return conditionCounter;
    }

    public void setConditionCounter(int conditionCounter) {
        this.conditionCounter.set(conditionCounter);
    }

    public long getSecondsToTrigger() {
        return secondsToTrigger;
    }

    public long getMessageCountToTrigger() {
        return messageCountToTrigger;
    }

    public long getWaitingMessagesToTrigger() {
        return waitingMessagesToTrigger;
    }
}
