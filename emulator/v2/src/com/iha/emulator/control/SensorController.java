package com.iha.emulator.control;

import com.iha.emulator.communication.server.OutMessage;
import com.iha.emulator.models.Sensor;
import com.iha.emulator.models.value.HasGenerator;
import com.iha.emulator.models.value.Value;
import com.iha.emulator.ui.panels.sensor.SensorPanelPresenter;
import com.iha.emulator.utilities.Utilities;
import javafx.application.Platform;
import javafx.beans.property.BooleanProperty;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.dom4j.Document;
import org.dom4j.Element;

import java.util.Timer;
import java.util.TimerTask;

/**
 * Creates and controls sensor. Also contains all list of emulated values, time for sensor and logic to generate new
 * values.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class SensorController {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(SensorController.class);
    /** adapter controller containing this sensor */
    private AdapterController adapterController;
    /** GUI representation of sensor */
    private SensorPanelPresenter panel;
    /** sensor model containing list of valus*/
    private Sensor model;
    /** timer running status property */
    private BooleanProperty timerRunning;
    /** help variable to determine, if sensor should generate new values or send message */
    private boolean runValueGenerator = true;
    /** timer for sensor */
    private Timer newTimer;
    /** task ran by timer */
    private TimerTask timerTask;
    /** type of message flag */
    private boolean fullMessage = false;
    /** ignore refresh time change from server flag */
    private BooleanProperty ignoreRefreshChange;
    /** XML message created by sensor */
    private Document XMLmessage = null;
    /** critical stop flag */
    private boolean criticalStop = false;
    /** action ran when adapter's status changes */
    private ChangeListener<Boolean> adapterStatusChangeListener = null;

    /**
     * Creates new instance of sensor's controller for adapter given as parameter and with model given as parameter
     * @param adapterController adapter containing new instance of sensor
     * @param model model of new sensor
     * @throws NullPointerException if model is null
     */
    public SensorController(AdapterController adapterController,Sensor model) throws NullPointerException{
        this(adapterController,null,model);
        initializeTimer();
    }

    /**
     * Creates new instance of sensor's controller with sensor GUI representation for adapter given as parameter
     * and with model given as parameter. Bind timerRunningProperty with methods used to start and stop timer.
     *
     * @param adapterController adapter containing new instance of sensor
     * @param panel GUI representation of sensor
     * @param model model of new sensor
     * @throws NullPointerException if model is null
     */
    public SensorController(AdapterController adapterController,SensorPanelPresenter panel, Sensor model) throws NullPointerException{
        if(model == null) throw new NullPointerException("New sensor model is null. Cannot create SensorController!");
        this.adapterController = adapterController;
        this.panel = panel;
        this.model = model;
        this.timerRunning = new SimpleBooleanProperty(false);
        this.ignoreRefreshChange = new SimpleBooleanProperty(false);
        this.timerRunningProperty().addListener(new ChangeListener<Boolean>() {
            @Override
            public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
                if(newValue){
                    startTimer();
                }else{
                    stopTimer();
                }
            }
        });
    }

    /**
     * If no critical error occurred previously, enables sensor.
     */
    public void enable(){
        if(criticalStop) return;
        getModel().setStatus(true);
    }
    /**
     * If no critical error occurred previously, disables sensor.
     */
    public void disable(){
        if(criticalStop) return;
        getModel().setStatus(false);
    }

    /**
     * Starts sensor's timer every half of sensor's refresh time.
     */
    public void startTimer(){
        logger.trace("Sensor/" + getModel().getId() + " timer started");
        if(newTimer != null){
            //newTimer = new Timer();
            if(timerTask != null) timerTask.cancel();
            newTimer.purge();
            timerTask = new TimerTask() {
                @Override
                public void run() {
                    timerEngine();
                }
            };
            newTimer.schedule(timerTask,(getModel().getRefreshTime()*1000),(getModel().getRefreshTime()*1000)/2);
        }
    }

    /**
     * Stops timer, if running.
     */
    public void stopTimer(){
        logger.trace("Sensor/" + getModel().getId() + " timer stopped");
        if(newTimer != null && timerTask != null){
            timerTask.cancel();
            newTimer.purge();
            timerTask = null;
        }
    }

    /**
     * Deletes model.
     */
    public void delete(){
        adapterController = null;
        model = null;
    }

    /**
     * Initializes new timer for sensor.
     */
    private void initializeTimer(){
        logger.trace("Sensor/" + getModel().getId() + " initialising timer");
        //create timer
        newTimer = new Timer(true);
    }

    /**
     * Method providing logic for generating and saving message to adapter's unsent list. In half of sensor's refresh
     * time method decides, if new values should be generated or message should be saved.
     */
    private void timerEngine(){
        SensorController me = this;
        //generate new value or send existing to server
        if(isValueGeneratorRunning()){
            logger.trace("Generating new sensor value");
            //create new values
            try{
                getModel().getValues().stream()
                        .filter(Value::isGenerateValue) //if new value should be generated
                        .forEach(v -> {
                            Object next = v.nextValue();
                            if (next != null) {
                                Platform.runLater(() -> v.setValue(next));
                            }
                        }); //do it
            }catch (IllegalArgumentException e){
                adapterController.sendError(toString() + " -> cannot generate new value!",e,false);
            }
            logger.trace("Building sensor message");
            //create XML message with new values
            XMLmessage = adapterController.getAdapter().getProtocol()
                    .buildSensorMessage(adapterController.getAdapter().getProtocol().buildAdapterMessage(adapterController.getAdapter()), getModel());
            //change engine to send message mod
            setRunValueGenerator(false);
        }else if(getModel().getStatus()){
            //if sensor's message doesn't exist, error is showed
            if(XMLmessage == null){
                adapterController.sendError(toString() + " doesn't have XML message to send",null,false);
                return;
            }
            //if we have means to send message and adapter is registered, send message to adapter
            if(adapterController.getServerReceiver() != null || adapterController.getAdapter().getRegistered()){
                //full message contains information about adapter
                if(isFullMessage()){
                    //used in "Performance simulation"
                    adapterController.sendMessage(me.toString() + " --> data sent",XMLmessage,me, OutMessage.Type.SENSOR_MESSAGE);
                }else{
                    //short message contains information only about sensor
                    //used in "Detailed simulation"
                    adapterController.sendMessage("Sensor " + me.toString() + " trying to send message.");
                    adapterController.sendMessage("Sensor " + me.toString() + " --> data sent",XMLmessage,me, OutMessage.Type.SENSOR_MESSAGE);
                }
                //change engine to generate value mod
                setRunValueGenerator(true);
            }
        }
    }

    /**
     * Method used to change actuator value by server.
     *
     * @param typeString type of value from server message
     * @param offsetString offset of value in sensor from server message
     * @param valueString new value from server message
     * @throws IllegalArgumentException cannot change given value, more info in exception message
     */
    @SuppressWarnings("unchecked")
    public void changeValue(String typeString,String offsetString,String valueString) throws IllegalArgumentException{
        try{
            //modify type string
            if(!typeString.startsWith("0x")) typeString = "0x" + typeString;
            //modify offset string
            if(!offsetString.startsWith("0x")) offsetString = "0x" + offsetString;
            boolean valueChanged = false;
            for(Value value : getModel().getValues()){
                String currentValueTypeString = value.getValueType().getType();
                String currentOffsetString = "0x" + Integer.toHexString(value.getOffset());
                if(currentValueTypeString.equalsIgnoreCase(typeString) && currentOffsetString.equalsIgnoreCase(offsetString)){
                    logger.debug("Sensor/" + getModel().getId() + " changing value set by server (from->to): " + value.asMessageString() + " -> " + valueString);
                    String oldValue = value.asMessageString();
                    Platform.runLater(() -> {
                        value.setValue(value.fromStringToValueType(valueString));
                        if(isFullMessage()){
                            adapterController.sendMessage(
                                    "Adapter/" + adapterController.getAdapter().getId() + " -> " +
                                            "Sensor "
                                            + getModel().getName() + "/" + getModel().getId()
                                            + " changed value of " + value.getName() + " (from->to): "
                                            + oldValue + " -> " + value.asMessageString());
                        }else {
                            adapterController.sendMessage("Sensor "
                                    + getModel().getName() + "/" + getModel().getId()
                                    + " changed value of " + value.getName() + " (from->to): "
                                    + oldValue + " -> " + value.asMessageString());
                        }
                    });
                    valueChanged = true;
                    break;
                }
            }
            if(!valueChanged) throw new IllegalArgumentException(toString() + " -> message \"set\" -> Cannot find value with type-> " + typeString + " offset -> " + offsetString);
        }catch (NumberFormatException e){
            throw new IllegalArgumentException("Cannot set value to -> " + valueString + " on sensor " + getModel().getId());
        }

    }

    /**
     * When critical error occurred on sensor, it is disabled and cannot run anymore. This prevents more errors when
     * for example message is wrong, or there is server error and messages from it are damaged.
     *
     * @param fullMessage message containing adapter information to be shown to user
     * @param shortMessage message containing only sensor information to be shown to user
     * @param message sent message that created error
     */
    public void criticalErrorStop(String fullMessage,String shortMessage,OutMessage message){
        //stop sensor
        Platform.runLater(this::disable);
        //set critical stop flag
        setCriticalStop(true);
        //pause adapter
        adapterController.stopScheduler();
        adapterController.messageSuccessfullySent(message);
        if(isFullMessage()){
            //"Performance simulation" message
            Platform.runLater(() -> adapterController.getLog().error(fullMessage,false));
        }else{
            //"Detailed simulation" message
            Platform.runLater(() -> adapterController.getLog().error(shortMessage,false));
        }
        //resume scheduler
        adapterController.startScheduler();
    }

    /**
     * Sets new refresh time for sensor, also restarts timer with new time.
     * @param refreshTime new refresh time
     */
    public void setNewRefreshTime(int refreshTime){
        resetTimer(refreshTime);
    }

    /**
     * Resets timer and sets new wake up time given as parameter.
     * @param refreshTime new refresh time
     */
    private void resetTimer(int refreshTime){
        //if timer is running
        if(getTimerRunning()){
            //stop timer
            setTimerRunning(false);
            //set new refresh time
            Platform.runLater(() -> {
                getModel().setRefreshTime(refreshTime);
                //run timer
                setTimerRunning(true);
            });
        }else{
            //set new refresh time
            Platform.runLater(() -> getModel().setRefreshTime(refreshTime));
        }
    }
    /**
     * Creates Dom4j XML element in given document. Saves all information sensor and it's values needed for later
     * loading of this sensor.
     *
     * @param rootElement root element for sensor's element
     */
    public void saveToXML(Element rootElement){
        Element sensorElement = rootElement.addElement("sensor")
                .addAttribute("id",String.valueOf(getModel().getId()));
        sensorElement.addElement("name").addText(getModel().getName());
        sensorElement.addElement("refresh").addText(String.valueOf(getModel().getRefreshTime()));
        sensorElement.addElement("signal").addText(String.valueOf(getModel().getSignal()));
        sensorElement.addElement("battery").addText(String.valueOf(getModel().getBattery()));
        if(getPanel() != null){
            sensorElement.addElement("icon").addText(getPanel().getIconType().getName());
            sensorElement.addElement("header_color").addText(getPanel().getHexHeaderColor());
        }
        Element valuesElement = sensorElement.addElement("values");
        for(Value value : getModel().getValues()){
            value.saveToXML(valuesElement);
        }
    }

    /**
     * Gets GUI representation of sensor.
     * @return GUI representation of sensor, <code>null</code> if panel doesn't exist.
     */
    public SensorPanelPresenter getPanel(){
        return this.panel;
    }

    /**
     * Sets GUI representation of sensor.
     * @param panel GUI representation of sensor.
     */
    public void setPanel(SensorPanelPresenter panel) {
        this.panel = panel;
    }

    /**
     * Gets sensor's model.
     * @return sensor's model.
     */
    public Sensor getModel(){
        return this.model;
    }

    /**
     * Sets sensor's model.
     * @param model sensor's model.
     */
    public void setModel(Sensor model) {
        this.model = model;
    }

    /**
     * Gets timer status
     * @return <code>true</code> if timer is running, <code>false</code> otherwise
     */
    public boolean getTimerRunning() {
        return timerRunning.get();
    }

    /**
     * Gets timer status property, that can be bound
     * @return timer status property
     */
    public BooleanProperty timerRunningProperty() {
        return timerRunning;
    }

    /**
     * Sets timer status
     * @param timerRunning timer status
     */
    public void setTimerRunning(boolean timerRunning) {
        this.timerRunning.set(timerRunning);
    }

    /**
     * Gets value generation mode
     * @return <code>true</code> if new value should be generated, <code>false</code> if message should be sent to adapter
     */
    public boolean isValueGeneratorRunning() {
        return runValueGenerator;
    }

    /**
     * Sets value generation mode
     * @param runValueGenerator <code>true</code> if new value should be generated, <code>false</code> if message should be sent to adapter
     */
    public void setRunValueGenerator(boolean runValueGenerator) {
        this.runValueGenerator = runValueGenerator;
    }

    /**
     * Gets sensor's log message mode
     * @return <code>true</code> if message should contain information about adapter, <code>false</code> if message should
     * contain only information about sensor
     */
    public boolean isFullMessage() {
        return fullMessage;
    }

    /**
     * Sets sensor's log message mode
     * @param fullMessage <code>true</code> if message should contain information about adapter, <code>false</code> if message should
     * contain only information about sensor
     */
    public void setFullMessage(boolean fullMessage) {
        this.fullMessage = fullMessage;
    }

    /**
     * Gets flag, if refresh time set by server should be ignored ( used in "Performance simulation")
     * @return <code>true</code> refresh time from server is ignored, <code>false</code> otherwise
     */
    public boolean getIgnoreRefreshChange() {
        return ignoreRefreshChange.get();
    }

    /**
     * Gets flag, if refresh time set by server should be ignored ( used in "Performance simulation") property, that can be
     * bound
     * @return Gets flag, if refresh time set by server should be ignored ( used in "Performance simulation") property
     */
    public BooleanProperty ignoreRefreshChangeProperty() {
        return ignoreRefreshChange;
    }

    /**
     * Sets flag, if refresh time set by server should be ignored ( used in "Performance simulation")
     * @param ignoreRefreshChange <code>true</code> refresh time from server is ignored, <code>false</code> otherwise
     */
    public void setIgnoreRefreshChange(boolean ignoreRefreshChange) {
        this.ignoreRefreshChange.set(ignoreRefreshChange);
    }

    /**
     * Gets flag, if critical error occurred previously
     * @return <code>true</code> critical error occurred previously, <code>false</code> otherwise
     */
    public boolean isCriticalStop() {
        return criticalStop;
    }

    /**
     * Sets flag, if critical error occurred previously
     * @param criticalStop <code>true</code> critical error occurred, <code>false</code> otherwise
     */
    public void setCriticalStop(boolean criticalStop) {
        this.criticalStop = criticalStop;
    }

    /**
     * Gets action ran when adapter status changes
     * @return action ran when adapter status changes
     */
    public ChangeListener<Boolean> getAdapterStatusChangeListener() {
        return adapterStatusChangeListener;
    }

    /**
     * Sets action ran when adapter status changes
     * @param adapterStatusChangeListener action ran when adapter status changes
     */
    public void setAdapterStatusChangeListener(ChangeListener<Boolean> adapterStatusChangeListener) {
        this.adapterStatusChangeListener = adapterStatusChangeListener;
    }

    /**
     * Method generates unique name for sensor
     * @return unique name for sensor.
     */
    public String toString(){
        if(isFullMessage()){
            return  "Adapter/" + adapterController.getAdapter().getId() + " -> " + "Sensor/" + (getModel() != null ? getModel().getId() : "Unknown sensor");
        }else {
            return getModel() != null ? getModel().getName() + "/" + getModel().getId() : "Unknown sensor";
        }
    }
}
