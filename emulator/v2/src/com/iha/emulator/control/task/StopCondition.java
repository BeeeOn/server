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
 * Class providing stop conditions for {@link com.iha.emulator.control.task.SimulationTask}. If any of registered
 * condition is met, task is stopped. Currently implemented 3 conditions: time duration condition, number of sent
 * messages condition and number of messages waiting to be sent condition.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class StopCondition {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(StopCondition.class);
    /** task for which are conditions registered */
    private SimulationTask task;
    /** time duration condition flag */
    private boolean timerEnabled = false;
    /** sent messages condition flag */
    private boolean sentMessageEnabled = false;
    /** waiting messages condition flag */
    private boolean waitingMessageEnabled = false;
    /** time duration condition action called after condition is met */
    private ChangeListener<Number> timerListener;
    /** sent messages condition action called after condition is met */
    private ChangeListener<Number> sentMessageListener;
    /** waiting messages condition action called after condition is met */
    private ChangeListener<Number> waitingMessageListener;
    /** time in seconds, after which should time condition stop the task */
    private long secondsToTrigger = 0;
    /** number of sent messages, after which should sent messages condition stop the task */
    private long messageCountToTrigger = 0;
    /** number of waiting messages, after which should waiting messages condition stop the task */
    private long waitingMessagesToTrigger = 0;
    /** assisting flag, so the action after condition is met will be called only once */
    private AtomicBoolean atomicTimerTriggered = new AtomicBoolean(false);
    /** assisting flag, so the action after condition is met will be called only once */
    private AtomicBoolean atomicSentMessageTriggered = new AtomicBoolean(false);
    /** assisting flag, so the action after condition is met will be called only once */
    private AtomicBoolean atomicWaitingMessageTriggered = new AtomicBoolean(false);
    /** number of registered conditions property */
    private IntegerProperty conditionCounter;

    /**
     * Creates new instance for {@link com.iha.emulator.control.task.SimulationTask} given as parameter. Conditions
     * are initiated. No condition is registered by default.
     * @param task task for which are stop conditions created
     */
    public StopCondition(SimulationTask task){
        this.task = task;
        this.conditionCounter = new SimpleIntegerProperty(0);
        initWatchers();
    }
    /**
     * Registers time duration condition. Needs task with existing {@link com.iha.emulator.control.task.TaskParameters} and
     * {@link com.iha.emulator.utilities.watchers.StopWatch} instances to properly register condition. Time to stop given
     * as parameter is bound to {@link com.iha.emulator.utilities.watchers.StopWatch#timeProperty()}.
     * @param secondsToTrigger time in seconds, after which should be task stopped
     */
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
        //set flag
        this.timerEnabled = true;
        //set time to trigger
        this.secondsToTrigger = secondsToTrigger;
        //log message
        task.logMessage("Estimated task duration: " + Utilities.formatSeconds(secondsToTrigger) + "\n");
        //bind to timer
        task.getTaskParameters().getStopWatch().timeProperty().addListener(timerListener);
        incrementConditionCounter();
    }

    /**
     * Registers sent messages condition. Needs task with existing {@link com.iha.emulator.utilities.logging.AdapterLogger} and
     * {@link com.iha.emulator.utilities.logging.MessageTracker}. Number of messages, after which task should stopped, given as
     * parameter, is compared to {@link com.iha.emulator.utilities.logging.MessageTracker#sentMessageCounterProperty()}.
     * @param messageCountToTrigger number of messages, after which task should stopped
     */
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
        //set flag
        this.sentMessageEnabled = true;
        //set number of messages to trigger
        this.messageCountToTrigger = messageCountToTrigger;
        //log message
        task.logMessage("Estimated task's sent message count: " + messageCountToTrigger + "\n");
        //bind to message counter
        task.getLog().getMessageTracker().sentMessageCounterProperty().addListener(sentMessageListener);
        incrementConditionCounter();
    }
    /**
     * Registers waiting messages condition. Needs task with existing {@link com.iha.emulator.utilities.logging.AdapterLogger} and
     * {@link com.iha.emulator.utilities.logging.MessageTracker}. Number of messages, after which task should stopped, given as
     * parameter, is compared to {@link com.iha.emulator.utilities.logging.MessageTracker#waitingMessageCounterProperty()}.
     * @param waitingMessagesToTrigger number of messages, after which task should stopped
     */
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
        //set flag
        this.waitingMessageEnabled = true;
        //set number of messages to trigger
        this.waitingMessagesToTrigger = waitingMessagesToTrigger;
        //log message
        task.logMessage("Estimated task's waiting messages count: " + waitingMessagesToTrigger + "\n");
        //bind to waiting messages counter
        task.getLog().getMessageTracker().waitingMessageCounterProperty().addListener(waitingMessageListener);
        incrementConditionCounter();
    }

    /**
     * Unregisters time duration condition.
     */
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
        //set flag
        this.timerEnabled = false;
        try{
            //unbind
            task.getTaskParameters().getStopWatch().timeProperty().removeListener(timerListener);
        }catch (NullPointerException e){

        }
        decrementConditionCounter();
    }
    /**
     * Unregisters sent messages condition.
     */
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
        //set flag
        this.sentMessageEnabled = false;
        try{
            //unbind
            task.getLog().getMessageTracker().sentMessageCounterProperty().removeListener(sentMessageListener);
        }catch (NullPointerException e){

        }
        decrementConditionCounter();
    }
    /**
     * Unregisters waiting messages condition.
     */
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
        //set flag
        this.waitingMessageEnabled = false;
        try{
            //unbind
            task.getLog().getMessageTracker().waitingMessageCounterProperty().removeListener(waitingMessageListener);
        }catch (NullPointerException e){

        }
        decrementConditionCounter();
    }

    /**
     * Initializes actions, that are called, after conditions are met.
     */
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
    /**
     * Restarts conditions checking
     */
    public void restart(){
        if(timerEnabled) atomicTimerTriggered.set(false);
        if(sentMessageEnabled) atomicSentMessageTriggered.set(false);
        if(waitingMessageEnabled) atomicWaitingMessageTriggered.set(false);
    }

    /**
     * Unregisters all registered conditions.
     */
    public void unregisterAllConditions(){
        unregisterTimeDurationWatcher();
        unregisterSentMessageWatcher();
        unregisterWaitingMessageWatcher();
    }
    /**
     * Creates Dom4j XML element for condition parameters
     * This element is added to root element given as parameter.
     *
     * @param rootElement root Dom4j XML element for server
     * @return XML element
     */
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
    /**
     * Increments number of registered conditions.
     */
    private synchronized void incrementConditionCounter(){
        setConditionCounter(getConditionCounter()+1);
    }
    /**
     * Decrements number of registered conditions.
     */
    private synchronized void decrementConditionCounter(){
        setConditionCounter(getConditionCounter()-1);
    }
    /**
     * Gets time duration condition flag.
     * @return time duration condition flag.
     */
    public boolean isTimerEnabled() {
        return timerEnabled;
    }
    /**
     * Gets sent messages condition flag.
     * @return sent messages condition flag.
     */
    public boolean isSentMessageEnabled() {
        return sentMessageEnabled;
    }
    /**
     * Gets waiting messages condition flag.
     * @return waiting messages condition flag.
     */
    public boolean isWaitingMessageEnabled() {
        return waitingMessageEnabled;
    }

    /**
     * Gets number of registered conditions.
     * @return number of registered conditions.
     */
    public int getConditionCounter() {
        return conditionCounter.get();
    }

    /**
     * Gets number of registered conditions property, that can be bound
     * @return number of registered conditions property
     */
    public IntegerProperty conditionCounterProperty() {
        return conditionCounter;
    }

    /**
     * Sets number of registered conditions
     * @param conditionCounter number of registered conditions
     */
    public void setConditionCounter(int conditionCounter) {
        this.conditionCounter.set(conditionCounter);
    }

    /**
     * Gets time in seconds, after which should be task stopped
     * @return time in seconds, after which should be task stopped
     */
    public long getSecondsToTrigger() {
        return secondsToTrigger;
    }

    /**
     * Gets number of sent messages, after which should be task stopped
     * @return number of sent messages, after which should be task stopped
     */
    public long getMessageCountToTrigger() {
        return messageCountToTrigger;
    }
    /**
     * Gets number of waiting messages, after which should be task stopped
     * @return number of waiting messages, after which should be task stopped
     */
    public long getWaitingMessagesToTrigger() {
        return waitingMessagesToTrigger;
    }
}
