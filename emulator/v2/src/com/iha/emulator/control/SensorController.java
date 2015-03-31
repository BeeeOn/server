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
 * Created by Shu on 4.2.2015.
 */
public class SensorController {

    private static final Logger logger = LogManager.getLogger(SensorController.class);

    private AdapterController adapterController;
    private SensorPanelPresenter panel;
    private Sensor model;
    private BooleanProperty timerRunning;
    private boolean runValueGenerator = true;
    private Timer newTimer;
    private TimerTask timerTask;
    private boolean fullMessage = false;
    private BooleanProperty ignoreRefreshChange;
    private Document XMLmessage = null;
    private boolean criticalStop = false;
    private ChangeListener<Boolean> adapterStatusChangeListener = null;

    public SensorController(AdapterController adapterController,Sensor model) throws NullPointerException{
        this(adapterController,null,model);
        initializeTimer();
    }

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

    public void enable(){
        if(criticalStop) return;
        getModel().setStatus(true);
    }

    public void disable(){
        if(criticalStop) return;
        getModel().setStatus(false);
    }

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

    public void stopTimer(){
        logger.trace("Sensor/" + getModel().getId() + " timer stopped");
        if(newTimer != null && timerTask != null){
            timerTask.cancel();
            newTimer.purge();
            timerTask = null;
        }
    }

    public void delete(){
        adapterController = null;
        model = null;
    }

    private void initializeTimer(){
        logger.trace("Sensor/" + getModel().getId() + " initialising timer");
        //create timer
        newTimer = new Timer(true);
    }

    private void timerEngine(){
        SensorController me = this;
        //generate new value or send existing to server
        if(isValueGeneratorRunning()){
            logger.trace("Generating new sensor value");
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
            XMLmessage = adapterController.getAdapter().getProtocol()
                    .buildSensorMessage(adapterController.getAdapter().getProtocol().buildAdapterMessage(adapterController.getAdapter()), getModel());
            //change engine to send message mod
            setRunValueGenerator(false);
        }else if(getModel().getStatus()){
            //should message include adapter id and sensor id?
            if(XMLmessage == null){
                adapterController.sendError(toString() + " doesn't have XML message to send",null,false);
                return;
            }
            if(adapterController.getServerReceiver() != null || adapterController.getAdapter().getRegistered()){
                if(isFullMessage()){
                    //used in "Performance simulation"
                    adapterController.sendMessage(me.toString() + " --> data sent",XMLmessage,me, OutMessage.Type.SENSOR_MESSAGE);
                }else{
                    //used in "Detailed simulation"
                    adapterController.sendMessage("Sensor " + me.toString() + " trying to send message.");
                    adapterController.sendMessage("Sensor " + me.toString() + " --> data sent",XMLmessage,me, OutMessage.Type.SENSOR_MESSAGE);
                }
                //change engine to generate value mod
                setRunValueGenerator(true);
            }
        }
    }

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

    public void restartValueGenerators(){
        if(getModel().getValues() == null ) return;
        getModel().getValues().stream()
                .filter(v -> v.isGenerateValue() && ((HasGenerator) v).getGeneratorType() != null)
                .forEach(v -> ((HasGenerator) v).restartGenerator());

    }

    public void criticalErrorStop(String fullMessage,String shortMessage,OutMessage message){
        //stop sensor
        Platform.runLater(this::disable);
        //set critical stop flag
        setCriticalStop(true);
        //pause adapter
        adapterController.stopScheduler();
        adapterController.messageSuccessfullySent(message);
        if(isFullMessage()){
            //if Performance simulation, stop task
            Platform.runLater(() -> adapterController.getLog().error(fullMessage,false));
        }else{
            Platform.runLater(() -> adapterController.getLog().error(shortMessage,false));
        }
        adapterController.startScheduler();
    }

    public void setNewRefreshTime(int refreshTime){
        resetTimer(refreshTime);
    }

    private void resetTimer(int refreshTime){
        if(getTimerRunning()){
            setTimerRunning(false);
            Platform.runLater(() -> {
                getModel().setRefreshTime(refreshTime);
                setTimerRunning(true);
            });

        }else{
            Platform.runLater(() -> getModel().setRefreshTime(refreshTime));
        }
    }

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

    public String getSensorIdAsIp(){
        return Utilities.intToIpString(getModel().getId());
    }

    public SensorPanelPresenter getPanel(){
        return this.panel;
    }

    public void setPanel(SensorPanelPresenter panel) {
        this.panel = panel;
    }

    public Sensor getModel(){
        return this.model;
    }

    public void setModel(Sensor model) {
        this.model = model;
    }

    public boolean getTimerRunning() {
        return timerRunning.get();
    }

    public BooleanProperty timerRunningProperty() {
        return timerRunning;
    }

    public void setTimerRunning(boolean timerRunning) {
        this.timerRunning.set(timerRunning);
    }

    public boolean isValueGeneratorRunning() {
        return runValueGenerator;
    }

    public void setRunValueGenerator(boolean runValueGenerator) {
        this.runValueGenerator = runValueGenerator;
    }

    public boolean isFullMessage() {
        return fullMessage;
    }

    public void setFullMessage(boolean fullMessage) {
        this.fullMessage = fullMessage;
    }

    public boolean getIgnoreRefreshChange() {
        return ignoreRefreshChange.get();
    }

    public BooleanProperty ignoreRefreshChangeProperty() {
        return ignoreRefreshChange;
    }

    public void setIgnoreRefreshChange(boolean ignoreRefreshChange) {
        this.ignoreRefreshChange.set(ignoreRefreshChange);
    }

    public boolean isCriticalStop() {
        return criticalStop;
    }

    public void setCriticalStop(boolean criticalStop) {
        this.criticalStop = criticalStop;
    }

    public ChangeListener<Boolean> getAdapterStatusChangeListener() {
        return adapterStatusChangeListener;
    }

    public void setAdapterStatusChangeListener(ChangeListener<Boolean> adapterStatusChangeListener) {
        this.adapterStatusChangeListener = adapterStatusChangeListener;
    }

    public String toString(){
        if(isFullMessage()){
            return  "Adapter/" + adapterController.getAdapter().getId() + " -> " + "Sensor/" + (getModel() != null ? getModel().getId() : "Unknown sensor");
        }else {
            return getModel() != null ? getModel().getName() + "/" + getModel().getId() : "Unknown sensor";
        }
    }
}
