package com.iha.emulator.utilities.watchers;

import com.iha.emulator.control.task.SimulationTask;
import com.iha.emulator.ui.simulations.performance.PerformanceSimulationPresenter;
import com.iha.emulator.utilities.Utilities;
import javafx.application.Platform;
import javafx.beans.property.BooleanProperty;
import javafx.beans.property.ObjectProperty;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.beans.property.SimpleObjectProperty;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.ObservableList;
import jfxtras.scene.control.LocalDateTimeTextField;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Locale;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * Created by Shu on 10.3.2015.
 */
public class QueueWatcher {

    private static final Logger logger = LogManager.getLogger(QueueWatcher.class);
    private static final String DATE_TIME_PATTERN = "dd-MM-yyyy HH:mm:ss";
    public static final DateTimeFormatter dateTimeFormatter = DateTimeFormatter.ofPattern(DATE_TIME_PATTERN);
    private static final Locale DEFAULT_LOCALE = Locale.UK;

    private BooleanProperty enabled;
    private BooleanProperty immediateStart;
    private ObjectProperty<LocalDateTime> startTime;
    private LocalDateTimeTextField localDateTimeTextField;
    private ObservableList<SimulationTask> tasks;
    private Timer timer;
    private TimerTask timerTask = null;
    private PerformanceSimulationPresenter simulation;

    public QueueWatcher(PerformanceSimulationPresenter simulation,ObservableList<SimulationTask> tasks) {
        this.enabled = new SimpleBooleanProperty(false);
        this.immediateStart = new SimpleBooleanProperty(true);
        this.startTime = new SimpleObjectProperty<>(null);
        this.tasks = tasks;
        this.simulation = simulation;
        initTimer();
    }

    public void start() throws IllegalArgumentException{
        if(getStartTime() == null && !getImmediateStart()) throw new IllegalArgumentException("No start time selected");
        logger.debug("Clearing task current selection");
        simulation.switchCurrentTask(null);
        setEnabled(true);
        if(!getImmediateStart()){
            logger.debug("Queue Start time: " + QueueWatcher.dateTimeFormatter.format(getStartTime()));
            startTimer();
        }else {
            logger.debug("Queue Start immediately");
            processQueue();
        }
    }

    public void next(){
        if(getEnabled()) processQueue();
    }

    public void stop(){
        Platform.runLater(() -> setEnabled(false));
        stopTimer();
        localDateTimeTextField.setLocalDateTime(null);
    }

    public void processQueue(){
        if(timerTask != null) stopTimer();
        AtomicInteger readyStateCount = new AtomicInteger(0);
        for(SimulationTask t : tasks){
            if(t.getSimulationState() == SimulationTask.State.READY && t.getEnabled()){
                readyStateCount.addAndGet(1);
                Platform.runLater(()->{
                    simulation.switchCurrentTask(t);
                    t.start();
                });

                break;
            }
        }
        if(readyStateCount.get() == 0) stop();
    }

    public void startTimer(){
        if(timer == null){
            logger.error("QueueWatch timer is null");
            return;
        }
        if(timerTask != null) timerTask.cancel();
        timer.purge();
        timerTask = new TimerTask() {
            @Override
            public void run() {
                if(getStartTime() != null && getStartTime().isBefore(LocalDateTime.now())){
                    processQueue();
                }
            }
        };
        logger.trace("QueueWatcher timer started");
        timer.schedule(timerTask,1000,1000*2);
    }

    public void stopTimer(){
        logger.trace("QueueWatcher timer stopped");
        if(timer != null && timerTask != null){
            timerTask.cancel();
            timer.purge();
            timerTask = null;
        }
    }

    public boolean getEnabled() {
        return enabled.get();
    }

    public BooleanProperty enabledProperty() {
        return enabled;
    }

    public void setEnabled(boolean enabled) {
        logger.trace("Setting QueueWatcher enabled: " + enabled);
        this.enabled.set(enabled);
    }

    public boolean getImmediateStart() {
        return immediateStart.get();
    }

    public BooleanProperty immediateStartProperty() {
        return immediateStart;
    }

    public void setImmediateStart(boolean immediateStart) {
        logger.trace("Setting QueueWatcher immediate start: " + immediateStart);
        this.immediateStart.set(immediateStart);
    }

    public void initDateTimePicker(LocalDateTimeTextField localDateTimeTextField){
        this.localDateTimeTextField = localDateTimeTextField;
        this.localDateTimeTextField.setDateTimeFormatter(dateTimeFormatter);
        this.localDateTimeTextField.setLocale(DEFAULT_LOCALE);
        this.startTime.bind(localDateTimeTextField.localDateTimeProperty());
        this.localDateTimeTextField.localDateTimeProperty().addListener(new ChangeListener<LocalDateTime>() {
            @Override
            public void changed(ObservableValue<? extends LocalDateTime> observable, LocalDateTime oldValue, LocalDateTime newValue) {
                if(newValue != null && newValue.isBefore(LocalDateTime.now())){
                    Utilities.showError("Cannot set start time before now!","Wrong queue start time!");
                    localDateTimeTextField.setLocalDateTime(null);
                }
            }
        });
    }

    private void initTimer(){
        this.timer = new Timer(true);
    }

    public LocalDateTime getStartTime() {
        return startTime.get();
    }

    public ObjectProperty<LocalDateTime> startTimeProperty() {
        return startTime;
    }

    public void setStartTime(LocalDateTime startTime) {
        this.startTime.set(startTime);
    }

    public LocalDateTimeTextField getLocalDateTimeTextField() {
        return localDateTimeTextField;
    }

    public void setLocalDateTimeTextField(LocalDateTimeTextField localDateTimeTextField) {
        this.localDateTimeTextField = localDateTimeTextField;
    }
}
