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
 * Class used for automatic processing of {@link com.iha.emulator.control.task.SimulationTask}s. Enables to start
 * processing at predefined time.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class QueueWatcher {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(QueueWatcher.class);
    /** default date-time pattern */
    private static final String DATE_TIME_PATTERN = "dd-MM-yyyy HH:mm:ss";
    /** date-time formatter */
    public static final DateTimeFormatter dateTimeFormatter = DateTimeFormatter.ofPattern(DATE_TIME_PATTERN);
    /** default locale */
    private static final Locale DEFAULT_LOCALE = Locale.UK;
    /** watcher enabled property */
    private BooleanProperty enabled;
    /** immediate start  property */
    private BooleanProperty immediateStart;
    /** automatic processing start time property */
    private ObjectProperty<LocalDateTime> startTime;
    /** GUI component for showing start time */
    private LocalDateTimeTextField localDateTimeTextField;
    /** list of simulation tasks */
    private ObservableList<SimulationTask> tasks;
    /** timer */
    private Timer timer;
    /** timer task ran each timer interval */
    private TimerTask timerTask = null;
    /** reference to "Performance simulation" class */
    private PerformanceSimulationPresenter simulation;

    /**
     * Creates instance of QueueWatcher, that is responsible for automatic processing of given tasks. Initiates, but not
     * starts, queue watcher timer.
     * @param simulation "Performance simulation" class
     * @param tasks list of tasks
     */
    public QueueWatcher(PerformanceSimulationPresenter simulation,ObservableList<SimulationTask> tasks) {
        this.enabled = new SimpleBooleanProperty(false);
        this.immediateStart = new SimpleBooleanProperty(true);
        this.startTime = new SimpleObjectProperty<>(null);
        this.tasks = tasks;
        this.simulation = simulation;
        initTimer();
    }

    /**
     * Switches current simulation task to null, so no task is running. Start timer immediately or in set time according
     * to previous configuration.
     * @throws IllegalArgumentException Nor delayed start time, neither immediate start is set
     */
    public void start() throws IllegalArgumentException{
        if(getStartTime() == null && !getImmediateStart()) throw new IllegalArgumentException("No start time selected");
        logger.debug("Clearing task current selection");
        //set current task to null, so if there is task running, it is stopped
        simulation.switchCurrentTask(null);
        setEnabled(true);
        //if start immediately
        if(!getImmediateStart()){
            logger.debug("Queue Start time: " + QueueWatcher.dateTimeFormatter.format(getStartTime()));
            startTimer();
        }else {
            //else process in given time
            logger.debug("Queue Start immediately");
            processQueue();
        }
    }

    /**
     * If queue watcher is enabled, picks and starts next simulation task
     */
    public void next(){
        if(getEnabled()) processQueue();
    }

    /**
     * Disables queue watcher and stops timer if it is running
     */
    public void stop(){
        Platform.runLater(() -> setEnabled(false));
        stopTimer();
        localDateTimeTextField.setLocalDateTime(null);
    }

    /**
     * Picks next simulation task with READY {@link com.iha.emulator.control.task.SimulationTask.State} and starts it.
     */
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

    /**
     * Starts time, that every 2 seconds looks if next task should be started
     */
    public void startTimer(){
        if(timer == null){
            logger.error("QueueWatch timer is null");
            return;
        }
        if(timerTask != null) timerTask.cancel();
        //cancel previous timer
        timer.purge();
        //create new timer
        timerTask = new TimerTask() {
            @Override
            public void run() {
                //if picked time is later then now
                if(getStartTime() != null && getStartTime().isBefore(LocalDateTime.now())){
                    processQueue();
                }
            }
        };
        logger.trace("QueueWatcher timer started");
        //start timer
        timer.schedule(timerTask,1000,1000*2);
    }

    /**
     * Stops queue watcher timer
     */
    public void stopTimer(){
        logger.trace("QueueWatcher timer stopped");
        if(timer != null && timerTask != null){
            timerTask.cancel();
            timer.purge();
            timerTask = null;
        }
    }

    /**
     * Gets flag, if queue watcher is enabled
     * @return <code>true</code> if it is enabled, <code>false</code> otherwise
     */
    public boolean getEnabled() {
        return enabled.get();
    }

    /**
     * Queue watcher enabled property, that can be bound
     * @return watcher enabled property
     */
    public BooleanProperty enabledProperty() {
        return enabled;
    }

    /**
     * Sets flag, if queue watcher is enabled
     * @param enabled <code>true</code> if queue watcher is enabled, <code>false</code> otherwise
     */
    public void setEnabled(boolean enabled) {
        logger.trace("Setting QueueWatcher enabled: " + enabled);
        this.enabled.set(enabled);
    }

    /**
     * Gets flag, if automatic processing should start immediately
     * @return <code>true</code> if automatic processing should start immediately, <code>false</code> otherwise
     */
    public boolean getImmediateStart() {
        return immediateStart.get();
    }

    /**
     * Immediate start flag property, that can be bound
     * @return immediate start flag property
     */
    public BooleanProperty immediateStartProperty() {
        return immediateStart;
    }

    /**
     * Sets flag, if automatic processing should start immediately
     * @param immediateStart <code>true</code> if automatic processing should start immediately, <code>false</code> otherwise
     */
    public void setImmediateStart(boolean immediateStart) {
        logger.trace("Setting QueueWatcher immediate start: " + immediateStart);
        this.immediateStart.set(immediateStart);
    }

    /**
     * Initiates date-time picker. Adds text field change listener, so user couldn't choose start time before now
     * @param localDateTimeTextField text field with start date-time
     */
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

    /**
     * Creates timer
     */
    private void initTimer(){
        this.timer = new Timer(true);
    }

    /**
     * Gets start time
     * @return start time
     */
    public LocalDateTime getStartTime() {
        return startTime.get();
    }

    /**
     * Start time property, that can be bound
     * @return start time property
     */
    public ObjectProperty<LocalDateTime> startTimeProperty() {
        return startTime;
    }

    /**
     * Sets start time
     * @param startTime start time
     */
    public void setStartTime(LocalDateTime startTime) {
        this.startTime.set(startTime);
    }

    /**
     * Gets GUI component with start time
     * @return GUI component with start time
     */
    public LocalDateTimeTextField getLocalDateTimeTextField() {
        return localDateTimeTextField;
    }

    /**
     * Sets GUI component with start time
     * @param localDateTimeTextField GUI component with start time
     */
    public void setLocalDateTimeTextField(LocalDateTimeTextField localDateTimeTextField) {
        this.localDateTimeTextField = localDateTimeTextField;
    }
}
