package com.iha.emulator.control;

import com.iha.emulator.communication.server.OutMessage;
import com.iha.emulator.models.Sensor;
import com.iha.emulator.models.value.Value;
import com.iha.emulator.ui.panels.sensor.SensorPanelPresenter;
import com.iha.emulator.utilities.Utilities;
import javafx.animation.Animation;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.application.Platform;
import javafx.beans.property.BooleanProperty;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.util.Duration;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.dom4j.Document;

/**
 * Created by Shu on 4.2.2015.
 */
public class SensorController {

    private static final Logger logger = LogManager.getLogger(SensorController.class);

    private AdapterController adapterController;
    private SensorPanelPresenter panel;
    private Sensor model;
    private BooleanProperty timerRunning;
    private boolean runValueGenerator = false;
    private Timeline timer;
    private boolean fullMessage = false;

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

    public void startTimer(){
        logger.trace("Sensor/" + getSensorIdAsIp() + " timer started");
        if(timer != null) timer.play();
    }

    public void stopTimer(){
        logger.trace("Sensor/" + getSensorIdAsIp() + " timer stopped");
        timer.stop();
    }

    public void delete(){
        adapterController = null;
        timer = null;
        model = null;
    }

    private void initializeTimer(){
        logger.trace("Sensor/" + getSensorIdAsIp() + " initialising timer");
        //create timer
        timer = new Timeline(new KeyFrame(
                Duration.millis((model.getRefreshTime()*1000)/2),
                ae -> timerEngine()));
        //set infinite periodical cycle
        timer.setCycleCount(Animation.INDEFINITE);
    }

    private void timerEngine(){
        //generate new value or send existing to server
        if(isValueGeneratorRunning()){
            logger.trace("Generating new sensor value");
            getModel().getValues().stream()
                    .filter(Value::isGenerateValue) //if new value should be generated
                    .forEach(Value::nextValue); //do it
            //change engine to send message mod
            setRunValueGenerator(false);
        }else if(getModel().getStatus()){
            logger.trace("Building and sending sensor message");
            Document message = adapterController.getAdapter().getProtocol()
                    .buildSensorMessage(adapterController.getAdapter().getProtocol().buildAdapterMessage(adapterController.getAdapter()), getModel());
            //should message include adapter id and sensor id?
            if(isFullMessage()){
                //used in "Performance simulation"
                adapterController.sendMessage("Adapter/" + adapterController.getAdapter().getId() + " -> Sensor/" + getSensorIdAsIp() + " --> data sent",message,this, OutMessage.Type.SENSOR_MESSAGE);
            }else{
                //used in "Detailed simulation"
                adapterController.sendMessage("Sensor " + getModel().getName() + "/" + getSensorIdAsIp() + " trying to send message.");
                adapterController.sendMessage("Sensor " + getModel().getName() + "/" + getSensorIdAsIp() + " --> data sent",message,this, OutMessage.Type.SENSOR_MESSAGE);
                //adapterController.sendMessage("Sensor " + getModel().getName() + "/" + getSensorIdAsIp() + " falling asleep for " + (timer.getDelay().toMillis()*2) / 1000 + " second/s");
            }
            //change engine to generate value mod
            setRunValueGenerator(true);
        }
    }

    @SuppressWarnings("unchecked")
    public void changeValue(String typeString,String offsetString,String valueString) throws IllegalArgumentException{
        try{
            //modify type string
            if(!typeString.startsWith("0x")) typeString = "0x" + typeString;
            //modify offset string
            if(!offsetString.startsWith("0x")) offsetString = "0x" + offsetString;
            for(Value value : getModel().getValues()){
                String currentValueTypeString = value.getValueType().getType();
                String currentOffsetString = "0x" + Integer.toHexString(value.getOffset());
                if(currentValueTypeString.equalsIgnoreCase(typeString) && currentOffsetString.equalsIgnoreCase(offsetString)){
                    logger.debug("Sensor/" + getSensorIdAsIp() + " changing value set by server (from->to): " + value.asMessageString() + " -> " + valueString);
                    String oldValue = value.asMessageString();
                    Platform.runLater(() -> {
                        value.setValue(value.fromStringToValueType(valueString));
                        if(isFullMessage()){
                            adapterController.sendMessage(
                                    "Adapter/" + adapterController.getAdapter().getId() + " -> " +
                                            "Sensor "
                                            + getModel().getName() + "/" + getSensorIdAsIp()
                                            + " changed value of " + value.getName() + " (from->to): "
                                            + oldValue + " -> " + value.asMessageString());
                        }else {
                            adapterController.sendMessage("Sensor "
                                    + getModel().getName() + "/" + getSensorIdAsIp()
                                    + " changed value of " + value.getName() + " (from->to): "
                                    + oldValue + " -> " + value.asMessageString());
                        }
                    });
                    return;
                }
            }
        }catch (NumberFormatException e){
            throw new IllegalArgumentException("Cannot set value to -> " + valueString + " on sensor " + getSensorIdAsIp());
        }

    }

    public void criticalErrorStop(String fullMessage,String shortMessage){
        //stop sensor
        Platform.runLater(() -> getModel().setStatus(false));
        //pause adapter
        adapterController.stopScheduler();
        adapterController.messageSuccessfullySent();
        if(isFullMessage()){
            Platform.runLater(() -> adapterController.getLog().error(fullMessage));
        }else{
            Platform.runLater(() -> adapterController.getLog().error(shortMessage));
        }
        adapterController.startScheduler();
    }

    public void setNewRefreshTime(int refreshTime){
        resetTimer(refreshTime);
        Platform.runLater(() -> getModel().setRefreshTime(refreshTime));
    }

    private void resetTimer(int refreshTime){
        KeyFrame keyFrame = new KeyFrame(
                Duration.millis((refreshTime*1000)/2),
                ae -> timerEngine()
        );
        if(getTimerRunning()){
            timer.stop();
            timer.getKeyFrames().setAll(keyFrame);
            timer.play();
        }else{
            timer.getKeyFrames().setAll(keyFrame);
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

    public String toString(){
        if(getModel() != null) return getModel().getName() + "/" + getSensorIdAsIp();
        else return "Unknown sensor";
    }
}
