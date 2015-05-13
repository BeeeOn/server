package com.iha.emulator.control.task;

import com.iha.emulator.communication.eserver.EmulatorServerClient;
import com.iha.emulator.communication.eserver.task.ServerTask;
import com.iha.emulator.communication.eserver.task.TaskParser;
import com.iha.emulator.communication.eserver.task.implemented.DeleteAdapterTask;
import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.communication.server.ssl.ServerController;
import com.iha.emulator.control.AdapterController;
import com.iha.emulator.control.SensorController;
import com.iha.emulator.control.scheduler.Scheduler;
import com.iha.emulator.models.Server;
import com.iha.emulator.models.value.Value;
import com.iha.emulator.utilities.Utilities;
import com.iha.emulator.utilities.logging.AdapterLogger;
import com.iha.emulator.utilities.watchers.QueueWatcher;
import com.iha.emulator.utilities.watchers.ResponseTracker;
import javafx.application.Platform;
import javafx.beans.property.*;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.concurrent.Task;
import javafx.scene.control.TabPane;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.Element;

import java.io.IOException;
import java.net.UnknownHostException;
import java.util.Random;

/**
 * Creates and controls simulation task. Also contains all information gathered while simulation runs. Task can be only in
 * one {@link com.iha.emulator.control.task.SimulationTask.State} at a time. All actions (start, pause, stop, etc.) are
 * dependent on current state of simulation.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class SimulationTask {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(SimulationTask.class);
    /** default adapter firmware version */
    private static final Double DEFAULT_ADAPTER_FIRMWARE = 0.0;
    /** value type min from {@link com.iha.emulator.models.value.Value.Type} */
    public static final int VALUE_TYPE_MIN = Value.Type.SENSOR_HUMIDITY.ordinal();
    /** value type max from {@link com.iha.emulator.models.value.Value.Type} */
    public static final int VALUE_TYPE_MAX = Value.Type.SENSOR_BOILER_STATUS.ordinal();
    /**
     * Enum with simulation task states. Every task must be in one of these states.
     */
    public enum State{
        //ready to run, all task parameters are set
        READY("Ready"),
        //task is running, adapters were created and are sending data to server
        RUNNING("Running"),
        //task's adapters are disabled, no data are sent to server
        PAUSED("Paused"),
        //task finish; stopped manually or one of registered conditions was met
        FINISHED("Finished"),
        //an error occurred, task is stopped
        ERROR("Error");
        /**
         * State name
         */
        private final StringProperty name;

        /**
         * Creates state with given name.
         * @param name name of state
         */
        State(String name){
            this.name = new SimpleStringProperty(name);
        }
        /**
         * Gets state's name
         * @return state's name
         */
        public String getName() {
            return name.get();
        }
        /**
         * Gets state's name property, that can be bound
         * @return state's name property
         */
        public StringProperty nameProperty() {
            return name;
        }
        /**
         * Sets state's name
         * @param name state's name
         */
        public void setName(String name) {
            this.name.set(name);
        }
    }
    /** Instance created in constructor */
    private SimulationTask me;
    /** indicates, if task was saved to XML */
    private boolean saved = false;
    /** indicates, if task is enabled */
    private BooleanProperty enabled;
    /** current task state */
    private ObjectProperty<State> simulationState;
    /** information about server and means to send data throw temporary connection */
    private ServerController serverController;
    /** task's id property */
    private IntegerProperty id;
    /** parameters for task */
    private TaskParameters taskParameters = null;
    /** stop conditions for task */
    private StopCondition stopCondition = null;
    /** response tracker saving response times */
    private ResponseTracker responseTracker = new ResponseTracker(true,false,true);
    /** logger for task */
    private AdapterLogger log;
    /** list of created adapters */
    private ObservableList<AdapterController> adapterControllers = FXCollections.observableArrayList();
    /** list of created adapters as a property, that can be bound */
    private ListProperty<AdapterController> adapterControllersList = new SimpleListProperty<>(adapterControllers);
    //region CONTROL
    /** indicates, if task is currently running */
    private BooleanProperty running;
    /** indicates, if task is currently paused */
    private BooleanProperty finished;
    /** indicates, if sensors are creating new messages for server */
    private BooleanProperty sensorsPaused;
    //endregion
    //region GENERATORS
    /** random generator for number of sensors, that are created for each adapter */
    private Random sensorCountGenerator;
    /** random generator of refresh time for each created sensor */
    private Random refreshTimeGenerator;
    /** random generator of signal value */
    private Random signalGenerator;
    /** random generator of battery value */
    private Random batteryGenerator;
    /** random generator of value type created for each sensor */
    private Random valueTypeGenerator;
    //endregion
    /** class providing continuous running of created tasks */
    private QueueWatcher queueWatcher= null;
    /**
     * Creates new instance with default {@link com.iha.emulator.control.task.SimulationTask.State} (READY)
     */
    public SimulationTask() {
        this.enabled = new SimpleBooleanProperty(true);
        this.simulationState = new SimpleObjectProperty<>(State.READY);
        this.id = new SimpleIntegerProperty();

        this.running = new SimpleBooleanProperty(false);
        this.finished = new SimpleBooleanProperty(false);
        this.sensorsPaused = new SimpleBooleanProperty(false);
        this.me = this;
    }
    /**
     * <p>
     * Depending on {@link com.iha.emulator.control.task.SimulationTask.State}, method either starts task by creating
     * adapters (sensors) and enabling them, or restarts task, so it can be started again. Shows progress dialog.
     * </p>
     * <p>
     *     State: <br>
     *         READY,PAUSED - creates adapter (if they are not already created) and enables them.<br>
     *         FINISHED - restarts task, so it can be started again
     * </p>
     */
    public void start(){
        switch (getSimulationState()){
            case READY:
            case PAUSED:
                logger.trace("Enabling adapters");
                Task<Object> worker = new Task<Object>() {
                    //initialize progress indicator
                    LongProperty taskProgress = new SimpleLongProperty(0);
                    @Override
                    protected Object call() throws Exception {
                        //bind progress indicator
                        taskProgressProperty().addListener(new ChangeListener<Number>() {
                            @Override
                            public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                                if((Long)newValue > 0 && (Long)newValue < 100)
                                    updateProgress((long)newValue,100);
                            }
                        });
                        setTaskProgress(1);
                        //if first start, create adapters
                        if(getSimulationState() == SimulationTask.State.READY){
                            updateMessage("Creating adapters");
                            createAdapters(taskProgressProperty());
                        }
                        //enable all created adapters
                        updateMessage("Enabling adapters");
                        for(AdapterController a :getAdapterControllers()){
                            a.enable();
                        }
                        //log message
                        Platform.runLater(() -> logMessage("Task RUNNING"));
                        //set progress
                        setTaskProgress(100);
                        return null;
                    }
                    public long getTaskProgress() {
                        return taskProgress.get();
                    }

                    public LongProperty taskProgressProperty() {
                        return taskProgress;
                    }

                    public void setTaskProgress(long taskProgress) {
                        this.taskProgress.set(taskProgress);
                    }

                    @Override
                    protected void failed() {
                        //task start failed, show exception dialog
                        super.failed();
                        Utilities.showException(logger,"Start task failed",getException(),false,null);
                    }

                };
                //show progress dialog
                Utilities.showLoadingDialog(worker,"Starting task...");
                //on start success
                worker.setOnSucceeded(event -> {
                    //set task's state
                    setSimulationState(State.RUNNING);
                    //set flag
                    setRunning(true);
                    //start timer
                    getTaskParameters().getStopWatch().start();
                });
                break;
            case FINISHED:
                //if task already finished, restart it
                Task restartTask = restart();
                if(restartTask != null)
                    restartTask.setOnSucceeded(event -> start());
                break;
        }
    }
    /**
     * <p>
     * Depending on {@link com.iha.emulator.control.task.SimulationTask.State}, method disables all created adapters,
     * so they cannot send data to server. Shows progress dialog.
     * </p>
     * <p>
     *     State: <br>
     *         ERROR,RUNNING - disables adapters <br>
     * </p>
     *
     * @return Task task responsible for pausing the simulation
     */
    public Task pause(){
        switch (getSimulationState()){
            case ERROR:
            case RUNNING:
                logger.trace("Disabling adapters");
                Task<Object> worker = new Task<Object>() {
                    //initialize progress indicator
                    LongProperty taskProgress = new SimpleLongProperty(0);
                    @Override
                    protected Object call() throws Exception {
                        //bind progress indicator
                        taskProgressProperty().addListener(new ChangeListener<Number>() {
                            @Override
                            public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                                if ((Long) newValue > 0 && (Long) newValue < 100)
                                    updateProgress((long) newValue, 100);
                            }
                        });
                        //set initial progress
                        setTaskProgress(1);
                        //temp variable to divide parts of progress
                        long progressPerAdapter = 95/getAdapterControllers().size();
                        //diasble adapters
                        for(AdapterController a :getAdapterControllers()){
                            a.disable();
                            setTaskProgress(getTaskProgress() + progressPerAdapter);
                        }
                        //set progress
                        setTaskProgress(100);
                        return null;
                    }

                    public long getTaskProgress() {
                        return taskProgress.get();
                    }

                    public LongProperty taskProgressProperty() {
                        return taskProgress;
                    }

                    public void setTaskProgress(long taskProgress) {
                        this.taskProgress.set(taskProgress);
                    }

                    @Override
                    protected void failed() {
                        //task pausing failed, show exception dialog
                        super.failed();
                        Utilities.showException(logger,"Pause task failed",getException(),false,null);
                    }
                };
                //show progress dialog
                Utilities.showLoadingDialog(worker,"Pausing task...");
                //on pausing success
                worker.setOnSucceeded(event -> {
                    //log message
                    Platform.runLater(() -> logMessage("Task PAUSED"));
                    //change task's state
                    setSimulationState(State.PAUSED);
                    //stop timer
                    getTaskParameters().getStopWatch().pause();
                });
                return worker;
        }
        return null;
    }
    /**
     * <p>
     * Depending on {@link com.iha.emulator.control.task.SimulationTask.State}, method stops task, deletes adapters and
     * finalize task. Shows progress dialog.
     * </p>
     * <p>
     *     State: <br>
     *         RUNNING,PAUSED,ERROR - if not paused, pauses task. Then deletes adapters,saves log file and clear memory
     *         so next task can be started<br>
     * </p>
     *
     * @return Task task responsible for stopping the simulation
     */
    public Task stop(){
        switch (getSimulationState()){
            case RUNNING:
            case PAUSED:
            case ERROR:
                //pause task if necessary -> adapters have to be disabled before they are deleted
                Task pauseTask = pause();
                //delete adapters from emulator and database
                Task<Object> worker = new Task<Object>() {
                    //initialize progress indicator
                    LongProperty taskProgress = new SimpleLongProperty(0);
                    @Override
                    protected Object call() throws Exception {
                        //set progress state
                        setTaskProgress(1);
                        //update progress dialog message
                        updateMessage("Terminating adapters");
                        //divide progress to parts
                        long progressPerAdapter = 35/getAdapterControllers().size();
                        //terminate adapters scheduler's
                        for(AdapterController a: getAdapterControllers()){
                            a.terminateScheduler();
                            setTaskProgress(getTaskProgress()+progressPerAdapter);
                        }
                        //stop task's timer
                        getTaskParameters().getStopWatch().stop();
                        updateMessage("Deleting adapters from database");
                        logger.trace("Deleting adapters from database");
                        //start removing adapter's from database
                        removeAdaptersFromDb();
                        updateMessage("Saving log file : " + getLog().getBufferFile().getAbsolutePath());
                        logger.trace("Saving log file : " + getLog().getBufferFile().getAbsolutePath());
                        //log message
                        logMessage("Task FINISHED");
                        //log final information about task's progress to log file
                        if(getTaskParameters().getStopWatch() != null){
                            logMessage("Task duration: " + getTaskParameters().getStopWatch().getTimeString());
                            logMessage("Max responses per second: " + getResponseTracker().getMaxPerSecond());
                            logMessage("Average sent messages per second: " + (getLog().getMessageTracker().getSentMessageCounter()/getTaskParameters().getStopWatch().getTime()));
                        }
                        updateMessage("Deleting adapters from emulator");
                        /*getAdapterControllers().forEach(a->{
                            a.delete();
                            setTaskProgress(getTaskProgress() + progressPerAdapter);
                        });*/
                        //clear adapter's from memory
                        getAdapterControllers().clear();
                        adapterControllersList.clear();
                        System.gc();
                        logger.trace("Closing buffer");
                        //close log file
                        getLog().closeBuffer();
                        setTaskProgress(95);
                        return null;
                    }

                    public long getTaskProgress() {
                        return taskProgress.get();
                    }

                    public LongProperty taskProgressProperty() {
                        return taskProgress;
                    }

                    public void setTaskProgress(long taskProgress) {
                        this.taskProgress.set(taskProgress);
                    }

                    @Override
                    protected void failed() {
                        //if stopping failed, show exception dialog
                        super.failed();
                        Utilities.showException(logger,"Stop task failed",getException(),false,null);
                    }
                };
                //if task not paused
                if(pauseTask != null){
                    //pause it and afterwards finish stopping
                    pauseTask.setOnSucceeded(e -> finishStop(worker));
                }else{
                    //finish stopping
                    finishStop(worker);
                }
                return worker;
        }
        return null;
    }
    /**
     * <p>
     * Depending on {@link com.iha.emulator.control.task.SimulationTask.State}, method restarts task to it's initial
     * state, so it can be started again
     * </p>
     * <p>
     *     State: <br>
     *         ERROR,FINISHED - clears logs, clears responses, resets timers <br>
     * </p>
     *
     * @return Task task responsible for restarting the simulation
     */
    public Task restart(){
        logger.debug("Restarting task");
        switch (getSimulationState()){
            case ERROR:
            case FINISHED:
                Task<Object> worker = new Task<Object>() {
                    //initialize progress indicator
                    LongProperty taskProgress = new SimpleLongProperty(0);
                    @Override
                    protected Object call() throws Exception {
                        //log message
                        Platform.runLater(()->logMessage("Task STOPPED"));
                        //bind progress indicator
                        taskProgressProperty().addListener(new ChangeListener<Number>() {
                            @Override
                            public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                                if ((Long) newValue > 0 && (Long) newValue < 100)
                                    updateProgress((long) newValue, 100);
                            }
                        });
                        setTaskProgress(1);
                        updateMessage("Clearing logs");
                        //delete log
                        getLog().delete();
                        setTaskProgress(20);
                        updateMessage("Buffering logs");
                        //create new log
                        getLog().createLogs();
                        setTaskProgress(40);
                        try{
                            //open log file
                            getLog().openBufferFile();
                            //write initial info to file
                            getLog().writeTaskLogHeaderToBuffer(me);
                        }catch (IOException e){
                            Platform.runLater(()-> Utilities.showException(logger, "Cannot open buffer file", e, true, event -> stop()));
                        }
                        setTaskProgress(60);
                        updateMessage("Deleting responses");
                        //clearing responses from tracker
                        clearResponseTracker();
                        updateMessage("Reset message tracker");
                        //clearing message counters
                        clearMessageTracker();
                        setTaskProgress(80);
                        updateMessage("Unregister adapters");
                        setSensorsPaused(false);
                        //reset task's timer
                        Platform.runLater(()->getTaskParameters().getStopWatch().reset());
                        //log message
                        logMessage("Task RESTARTED");
                        setTaskProgress(100);
                        return null;
                    }

                    public long getTaskProgress() {
                        return taskProgress.get();
                    }

                    public LongProperty taskProgressProperty() {
                        return taskProgress;
                    }

                    public void setTaskProgress(long taskProgress) {
                        this.taskProgress.set(taskProgress);
                    }

                    @Override
                    protected void failed() {
                        //if restart failed, show exception dialog
                        super.failed();
                        Utilities.showException(logger,"Restart task failed",getException(),false,null);
                    }
                };
                //show progress dialog
                Utilities.showLoadingDialog(worker,"Restarting task...");
                //on restart success
                worker.setOnSucceeded(event -> {
                    //change task's state
                    setSimulationState(State.READY);
                    //set flag
                    setRunning(false);
                });
                return worker;
            default:
                //task is not finish yet, must finish first and then it can be restarted
                Task stopTask = stop();
                if(stopTask != null) stopTask.setOnSucceeded(event -> restart());
        }
        return null;
    }
    /**
     * Method called, when serious error occurred. Changes task {@link com.iha.emulator.control.task.SimulationTask.State}
     * to ERROR and immediately stops task.
     */
    public void criticalStop(){
        if(getSimulationState() == State.ERROR) return;
        setSimulationState(State.ERROR);
        stop();
    }
    /**
     * Deletes log
     */
    public void clear(){
        //clear logs
        getLog().delete();
    }

    /**
     * Method disconnects all created sensors from their adapters, so they cannot generate new messages.
     */
    public void pauseSensors(){
        //create task
        Task<Object> worker = new Task<Object>() {
            @Override
            protected Object call() throws Exception {
                //initiate progress indicator
                LongProperty progress = new SimpleLongProperty(0);
                //bind indicator
                progress.addListener(new ChangeListener<Number>() {
                    @Override
                    public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                        if((Long)newValue > 0 && (Long)newValue < 100)
                            updateProgress((long)newValue,100);
                    }
                });
                //set progress
                progress.set(1);
                //divide progress in parts
                long progressPerAdapter = 95/getAdapterControllers().size();
                //disable sensors
                getAdapterControllers().forEach(a->{
                    a.getSensorControllers().forEach(SensorController::disable);
                    progress.set(progress.get() + progressPerAdapter);
                });
                //log message
                Platform.runLater(() -> logMessage("Task's sensors PAUSED"));
                progress.set(99);
                return null;
            }
        };
        //show progress dialog
        Utilities.showLoadingDialog(worker,"Pausing sensors...");
        //on pausing success set flag
        worker.setOnSucceeded(event -> setSensorsPaused(true));
    }
    /**
     * Method connects all created sensors to their adapters, so they can generate new messages.
     */
    public void resumeSensors(){
        //create task
        Task<Object> worker = new Task<Object>() {
            @Override
            protected Object call() throws Exception {
                //initiate progress indicator
                LongProperty progress = new SimpleLongProperty(0);
                //bind indicator
                progress.addListener(new ChangeListener<Number>() {
                    @Override
                    public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                        if((Long)newValue > 0 && (Long)newValue < 100)
                            updateProgress((long)newValue,100);
                    }
                });
                //set initial progress
                progress.set(1);
                //divide progress to parts
                long progressPerAdapter = 95/getAdapterControllers().size();
                //enable sensors
                getAdapterControllers().forEach(a->{
                    a.getSensorControllers().forEach(SensorController::enable);
                    progress.set(progress.get() + progressPerAdapter);
                });
                //log message
                Platform.runLater(() -> logMessage("Task's sensors RESUMED"));
                progress.set(99);
                return null;
            }
        };
        //show progress dialog
        Utilities.showLoadingDialog(worker,"Resuming sensors...");
        //on resuming success, set flag
        worker.setOnSucceeded(event -> setSensorsPaused(false));
    }

    /**
     * Creates adapters and their sensors according to information in {@link com.iha.emulator.control.task.TaskParameters}.
     * Adapters IDs start from {@link TaskParameters#getStartId()} and increment by 1 for each new adapter. All created adapters
     * are added to {@link #adapterControllers} list.
     * @param progress progress indicator
     */
    public void createAdapters(LongProperty progress){
        if(taskParameters == null){
            logger.error(toString() + " missing task parameters, cannot create adapters!");
            return;
        }
        //loading dialog progress help variable; 95% / number of adapters
        long oneAdapterProgress = (90-progress.get())/getTaskParameters().getAdaptersCount();
        for (int i = 0;i<getTaskParameters().getAdaptersCount();i++){
            int newAdapterId = getTaskParameters().getStartId();
            logger.trace("Creating adapter: " + (newAdapterId+i));
            //create new adapter controller
            AdapterController newAdapterController = new AdapterController();
            //create new adapter
            newAdapterController.createAdapter(false,newAdapterId+i,false, getTaskParameters().getProtocolVersion(),DEFAULT_ADAPTER_FIRMWARE);
            newAdapterController.getAdapter().setName("Adapter");
            //create new server
            newAdapterController.setServerController(getServerController());
            //create logger
            newAdapterController.setLog(getLog());
            //create scheduler
            newAdapterController.createScheduler(Scheduler.Type.PERFORMANCE);
            //set response tracking
            newAdapterController.setServerResponseTracker(getResponseTracker());
            //bind scheduler processing to adapter's status indicator
            newAdapterController.bindSchedulerProcess(newAdapterController.getAdapter(), newAdapterController.getScheduler());
            //bind register message
            newAdapterController.bindRegisterMessage();
            //create sensors for new adapter and save devices count
            int sCount = createSensors(newAdapterController);
            Platform.runLater(()->getTaskParameters().setSensorsCount(getTaskParameters().getSensorsCount() + sCount));
            newAdapterController.setSaved(true);
            //save it to list
            adapterControllersList.add(newAdapterController);
            //set loading dialog progress
            progress.set(progress.get()+oneAdapterProgress);
        }
        logMessage("Created " + getTaskParameters().getSensorsCount() + " sensors.");
    }

    /**
     * Creates sensors for adapter given as parameter. Uses {@link com.iha.emulator.control.task.TaskParameters} for
     * information for random generators (sensors count, refresh time,..). For each sensor is created 1 value.
     * Sensors ignore refresh times given by server, they always use initially generated random refresh time.
     * @param adapterController adapter, for which are new sensors created
     * @return number of successfully created sensors
     */
    public int createSensors(AdapterController adapterController){
        int sensorsCount,sensorsTotal = 0;
        TaskParameters parameters = getTaskParameters();
        try{
            //if sensors count min and max are equal or min is 0, set sensors count to max
            if(getTaskParameters().getSensorsCountMin() == 0 || getTaskParameters().getSensorsCountMin() == getTaskParameters().getSensorsCountMax()){
                sensorsCount = getTaskParameters().getSensorsCountMax();
            }else{
                //else generate random sensors count
                sensorsCount = Utilities.generateIntInRange(
                        sensorCountGenerator,
                        parameters.getSensorsCountMin(),
                        parameters.getSensorsCountMax()
                );
            }
            //inform user
            logMessage("Creating Adapter/" + adapterController.getAdapter().getId() + " with " + sensorsCount + " sensors");
            //generate sensors
            for (int i = 0; i < sensorsCount; i++) {
                /*logMessage("Trying to create sensor: " +
                    Utilities.intToIpString(Integer.valueOf(adapterController.getAdapter().getId() + "" + adapterController.getAdapter().getId() + i))
                );*/
                //create value for sensor
                Value value = null;
                logger.trace("Generating value for new sensor");
                while(value == null){
                    //generate value index (index in enum Value.Type)
                    int valueIndex = Utilities.generateIntInRange(valueTypeGenerator,0,getValueParameters().getEnabledValues().size());
                    if(valueIndex == getValueParameters().getEnabledValues().size()) valueIndex--;
                    //check if should use generated value
                    value = getValueParameters().getEnabledValue(valueIndex);
                    if(value == null)
                        logger.trace("Value not enabled -> " + valueIndex);
                }
                logger.trace("Chosen value: " + value.getName());
                //add value to array
                ObservableList<Value> tmpValues = FXCollections.observableArrayList(value);
                //TODO generate signal and battery
                //generate signal
                int signal = 100;
                //generate battery
                int battery = 100;
                //generate refresh time
                int refreshTime;
                logger.trace("Generating refresh time");
                //if refresh time min and max are equal or min is 0, set refresh time to max
                if(getTaskParameters().getRefreshTimeMin() == 0 || getTaskParameters().getRefreshTimeMin() == getTaskParameters().getRefreshTimeMax()) {
                    refreshTime = getTaskParameters().getRefreshTimeMax();
                }else{
                    //else generate random refresh time
                    refreshTime = Utilities.generateRefreshTime(
                            refreshTimeGenerator,
                            getTaskParameters().getRefreshTimeMin(),
                            getTaskParameters().getRefreshTimeMax()
                    );
                }
                logger.trace("Creating sensor");
                SensorController tmp = adapterController.createSensor(
                        tmpValues, //value for sensor
                        true, //connection
                        Integer.valueOf(adapterController.getAdapter().getId() + "" + (adapterController.getAdapter().getId() + i)), //sensor id
                        "Sensor", // sensor name
                        battery, //battery
                        signal, //signal
                        refreshTime, //refresh time
                        adapterController.getAdapter().getProtocol() //communication protocol
                );
                tmp.setFullMessage(true);
                tmp.setIgnoreRefreshChange(true);
                /*logMessage("Sensor/" + tmp.getSensorIdAsIp() + " created successfully");*/
                sensorsTotal++;
            }
            return sensorsTotal;
        }catch (IllegalArgumentException e ){
            Utilities.showException(logger, "Cannot create sensors for adapter " + adapterController.getAdapter().getId(), e, false, null);
        }
        return 0;
    }

    /**
     * Deletes simulation task from application. Closes logs, clears responses from server and adapters from memory.
     */
    public void delete(){
        logger.debug("Deleting " + toString());
        logger.trace("Dumping logs");
        //close log file
        getLog().closeBuffer();
        logger.trace("Clearing responses");
        //clear responses, so memory will clear
        getResponseTracker().clearResponses();
        logger.trace("Unregistering stop conditions");
        //unregister conditions
        getStopCondition().unregisterAllConditions();
        logger.trace("Clearing adapters");
        //clear adapters
        getAdapterControllers().clear();
    }
    /**
     * Write message to task's log
     * @param message message to be written to log
     */
    public synchronized void logMessage(String message){
        if(this.log != null) log.log(message);
        else logger.error("No logger");
    }
    /**
     * Creates Dom4j XML element in given document. Saves all information about simulation task needed for later
     * loading this task. Creates documents root element.
     *
     * @param doc XML document
     * @return XML document with information about simulation task
     */
    public Document saveToXml(Document doc){
        Element taskElement = doc.getRootElement().addElement("task");
        //server info
        logger.trace("Saving server info");
        getServerController().saveToXml(taskElement);
        //task parameters
        logger.trace("Saving parameters");
        getTaskParameters().saveToXml(taskElement);
        //stop conditions
        logger.trace("Saving stop conditions");
        getStopCondition().saveToXml(taskElement);
        return doc;
    }

    /**
     * Creates new instance of {@link com.iha.emulator.communication.server.ssl.ServerController} containing {@link com.iha.emulator.models.Server}
     * model with information given as parameters.
     * @param conn connection status. <code>True</code> if connected, <code>false</code> otherwise.
     * @param name name of the server
     * @param ip IP address or hostname of server
     * @param port server port
     * @param databaseName name of database storing data
     * @return new instance if server controller
     */
    public ServerController createServer(boolean conn,String name,String ip, int port,String databaseName){
        logger.trace("Creating server model: " + "conn -> "+conn + " name-> "+name + " ip-> "+ip + " port-> "+port + " DB-> "+databaseName);
        setServerController(new ServerController(new Server(conn,name,ip,port,databaseName)));
        logger.trace("OK");
        return this.serverController;
    }

    /**
     * Creates new instance of {@link com.iha.emulator.communication.server.ssl.ServerController} for {@link com.iha.emulator.models.Server}
     * model given as parameter.
     * @param model server model
     * @return new instance of server controller
     */
    public ServerController createServer(Server model){
        logger.trace("Setting server: " + model.getName());
        setServerController(new ServerController(model));
        logger.trace("OK");
        return this.serverController;
    }

    /**
     * Creates and initializes new instance of {@link com.iha.emulator.utilities.logging.AdapterLogger} and assigns it
     * variable {@link #log}. Also assigns {@link #criticalStop()} to logs error handler.
     * @param panel parent GUI component for new logs
     */
    public void createLog(TabPane panel){
        //create log
        this.log = new AdapterLogger(panel);
        logger.trace("Logger created");
        //register error handler
        this.log.getErrorHandler().addErrorListener(this::criticalStop);
        logger.trace("Listener added");
    }

    /**
     * Creates new instance of {@link com.iha.emulator.control.task.TaskParameters} with information given as parameters
     * and creates random generators.
     *
     * @param adaptersCount number of adapters, to be created
     * @param version protocol version, to be used
     * @param startId ID of first adapter, others are assigned ID incremented by 1
     * @param sensorsCountMin minimal number of sensors to be created
     * @param sensorsCountMax maximal number of sensors to be created
     * @param refreshTimeMin minimal refresh time for each sensor
     * @param refreshTimeMax maximal refresh time for each sensor
     * @param saveDir path to directory containing log
     *
     * @return new instance if task parameters
     */
    public TaskParameters createTaskParameters(int adaptersCount,Protocol.Version version, int startId,int sensorsCountMin,int sensorsCountMax,int refreshTimeMin,int refreshTimeMax,String saveDir){
        this.taskParameters = new TaskParameters();
        //set task parameters
        taskParameters.setAdaptersCount(adaptersCount);
        taskParameters.setProtocolVersion(version);
        taskParameters.setStartId(startId);
        taskParameters.setSensorsCountMin(sensorsCountMin);
        taskParameters.setSensorsCountMax(sensorsCountMax);
        taskParameters.setRefreshTimeMin(refreshTimeMin);
        taskParameters.setRefreshTimeMax(refreshTimeMax);
        if(saveDir != null)
            taskParameters.setSaveDir(saveDir);
        //create task's generators
        createGenerators();
        return taskParameters;
    }

    /**
     * Creates new instance of {@link com.iha.emulator.control.task.StopCondition} class and assigns it to {@link #stopCondition}
     * @return new  instance of stop condition
     */
    public StopCondition createStopCondition(){
        this.stopCondition = new StopCondition(this);
        return stopCondition;
    }

    /**
     * <p>
     * Shows progress dialog for given task and changes it's state according to task success. Also if simulation task
     * queue is set to autorun, calls next simulation task.
     * </p>
     * <p>
     *     State: <br>
     *         !ERROR - sets tasks state to FINISHED<br>
     * </p>
     *
     * @param worker task, for which is to be shown progress dialog
     */
    private void finishStop(Task<Object> worker){
        //show progress dialog
        Utilities.showLoadingDialog(worker,"Finishing task...");
        //on success
        worker.setOnSucceeded(event -> {
            //if no error occurred, set state to FINISHED
            if(!getSimulationState().equals(State.ERROR)){
                setSimulationState(State.FINISHED);
            }
            //set running flag
            setRunning(false);
            //if simulation task queue is set to autorun, run next task
            if(getQueueWatcher() != null) getQueueWatcher().next();
        });
    }

    /**
     * Clears all saved responses from {@link com.iha.emulator.utilities.watchers.ResponseTracker}. If no response tracker
     * is created yet, method creates new instance.
     */
    private void clearResponseTracker(){
        if(getResponseTracker() != null){
            getResponseTracker().clearResponses();
        }else{
            setResponseTracker(new ResponseTracker(true,false,true));
        }
    }

    /**
     * Clears all message counters in {@link com.iha.emulator.utilities.logging.MessageTracker}.
     */
    private void clearMessageTracker(){
        if(getLog() == null) return;
        Platform.runLater(()->getLog().getMessageTracker().clearCounters());
    }

    /**
     * Creates random generators with seeds set in previously created{@link com.iha.emulator.control.task.TaskParameters}
     */
    private void createGenerators(){
        sensorCountGenerator = new Random(getTaskParameters().getSensorsCountGeneratorSeed());
        refreshTimeGenerator = new Random(getTaskParameters().getSensorsCountGeneratorSeed());
        batteryGenerator = new Random(getTaskParameters().getSensorsCountGeneratorSeed());
        signalGenerator = new Random(getTaskParameters().getSensorsCountGeneratorSeed());
        valueTypeGenerator = new Random(getTaskParameters().getValueTypeGeneratorSeed());
    }

    /**
     * Sets stop conditions to initial state.
     */
    private void restartStopConditions(){
        if(getStopCondition() != null) getStopCondition().restart();
    }

    /**
     * Creates new {@link com.iha.emulator.communication.eserver.task.implemented.DeleteAdapterTask} and attempts to
     * delete adapters created by simulation task from database. Emulator server must be running on server so this task
     * can be successfull.
     */
    private void removeAdaptersFromDb(){
        EmulatorServerClient server;
        try{
            //create new emulator server task
            server = new EmulatorServerClient(getServerController().getModel().getIp());
            //connect to emulator server
            server.connect();
            //composite message for server
            ServerTask task = new DeleteAdapterTask(
                    getServerController().getModel().getDatabaseName(),
                    String.valueOf(getTaskParameters().getStartId()),
                    getTaskParameters().getAdaptersCount()
            );
            //send message and wait for response
            if(server == null) throw new IllegalStateException("Server is null");
            String messageFromServer = server.sendMessage(task.buildMessage());
            //determine result state (OK/ERROR)
            TaskParser.parseTaskResult(messageFromServer);
            //if ok, parse response
            task.parseResponse(messageFromServer);
            //show if error occurred
            if(!((DeleteAdapterTask)task).getResult()){
                Platform.runLater(()->Utilities.showException(logger,"Error in database delete",null,false,null));
            }
        }catch (UnknownHostException e){
            Platform.runLater(()->Utilities.showException(logger,"Cannot connect to emulator server.Unknown host",e,false,null));
        }catch (IOException e){
            Platform.runLater(()->Utilities.showException(logger,"Cannot connect to emulator server",e,false,null));
        }catch (DocumentException de){
            Platform.runLater(()-> Utilities.showException(logger,"Cannot parse emulator server message",de,false,null));
        }catch (IllegalStateException ie){
            Platform.runLater(()-> Utilities.showException(logger,"Error on emulator server",ie,false,null));
        }
    }

    /**
     * Returns simulation task's name as - Task/(task ID).
     * @return simulation task's name
     */
    public String toString(){
        return "Task/"+getId();
    }

    /**
     * Gets flag, if simulation task is enabled.
     * @return <code>true</code> if task is enabled, <code>false</code> otherwise.
     */
    public boolean getEnabled() {
        return enabled.get();
    }

    /**
     * Gets flag, if simulation task is enabled property, that can be bound
     * @return flag, if simulation task is enabled property
     */
    public BooleanProperty enabledProperty() {
        return enabled;
    }
    /**
     * Sets flag, if simulation task is enabled.
     * @param enabled <code>true</code> if task is enabled, <code>false</code> otherwise.
     */
    public void setEnabled(boolean enabled) {
        this.enabled.set(enabled);
    }

    /**
     * Gets simulation task's current state
     * @return simulation task's current state
     */
    public State getSimulationState() {
        return simulationState.get();
    }

    /**
     * Gets simulation task's current state property, that can be bound
     * @return simulation task's current state property
     */
    public ObjectProperty<State> simulationStateProperty() {
        return simulationState;
    }

    /**
     * Sets simulation task's current state
     * @param simulationState simulation task's current state
     */
    public void setSimulationState(State simulationState) {
        logger.debug("Setting task state -> "  + simulationState.getName());
        this.simulationState.set(simulationState);
    }

    /**
     * Gets server controller containing server model
     * @return server controller containing server model
     */
    public ServerController getServerController() {
        return serverController;
    }

    /**
     * Sets server controller containing server model
     * @param serverController server controller containing server model
     */
    public void setServerController(ServerController serverController) {
        this.serverController = serverController;
    }

    /**
     * Gets simulation task's ID
     * @return simulation task's ID
     */
    public int getId() {
        return id.get();
    }

    /**
     * Gets simulation task's ID property, that can be bound
     * @return simulation task's ID property
     */
    public IntegerProperty idProperty() {
        return id;
    }

    /**
     * Sets simulation task's ID
     * @param id simulation task's ID
     */
    public void setId(int id) {
        this.id.set(id);
    }

    /**
     * Gets simulation is running flag
     * @return <code>true</code> if simulation is running, <code>false</code> otherwise
     */
    public boolean getRunning() {
        return running.get();
    }
    /**
     * Gets simulation is running flag property, that can be bound
     * @return simulation is running flag property
     */
    public BooleanProperty runningProperty() {
        return running;
    }
    /**
     * Sets simulation is running flag
     * @param running <code>true</code> if simulation is running, <code>false</code> otherwise
     */
    public void setRunning(boolean running) {
        logger.debug(toString() + " -> setting running -> " + running);
        this.running.set(running);
    }
    /**
     * Gets sensors paused flag
     * @return <code>true</code> if sensors are paused, <code>false</code> otherwise
     */
    public boolean getSensorsPaused() {
        return sensorsPaused.get();
    }
    /**
     * Gets sensors paused flag property, that can be bound
     * @return sensors paused flag property
     */
    public BooleanProperty sensorsPausedProperty() {
        return sensorsPaused;
    }
    /**
     * Sets sensors paused flag
     * @param sensorsPaused <code>true</code> if sensors are paused, <code>false</code> otherwise
     */
    public void setSensorsPaused(boolean sensorsPaused) {
        this.sensorsPaused.set(sensorsPaused);
    }
    /**
     * Gets task parameters instance
     * @return task parameters instance
     */
    public TaskParameters getTaskParameters() {
        return taskParameters;
    }
    /**
     * Gets value parameters instance
     * @return value parameters instance
     */
    public ValueParameters getValueParameters() {
        return getTaskParameters().getValueParameters();
    }
    /**
     * Sets value parameters instance
     * @param valueParameters value parameters instance
     */
    public void setValueParameters(ValueParameters valueParameters) {
        getTaskParameters().setValueParameters(valueParameters);
    }

    /**
     * Gets simulation task's log
     * @return simulation task's log
     */
    public AdapterLogger getLog() {
        return log;
    }

    /**
     * Sets simulation task's log
     * @param log simulation task's log
     */
    public void setLog(AdapterLogger log) {
        this.log = log;
    }
    /**
     * Gets list of created adapters for task
     * @return list of created adapters for task
     */
    public ObservableList<AdapterController> getAdapterControllers() {
        return adapterControllers;
    }
    /**
     * Gets list of created adapters for task
     * @return list of created adapters for task
     */
    public ObservableList<AdapterController> getAdapterControllersList() {
        return adapterControllersList.get();
    }
    /**
     * Gets list property of created adapters for task, that can be bound
     * @return list property of created adapters for task
     */
    public ListProperty<AdapterController> adapterControllersListProperty() {
        return adapterControllersList;
    }

    /**
     * Sets list of created adapters for task
     * @param adapterControllersList list of created adapters for task
     */
    public void setAdapterControllersList(ObservableList<AdapterController> adapterControllersList) {
        this.adapterControllersList.set(adapterControllersList);
    }

    /**
     * Gets simulation is finished flag
     * @return simulation is finished flag
     */
    public boolean getFinished() {
        return finished.get();
    }
    /**
     * Gets simulation is finished flag property, that can be bound
     * @return simulation is finished flag property
     */
    public BooleanProperty finishedProperty() {
        return finished;
    }
    /**
     * Sets simulation is finished flag
     * @param finished simulation is finished flag
     */
    public void setFinished(boolean finished) {
        logger.debug(toString() + " -> setting finished -> " + finished);
        this.finished.set(finished);
    }

    /**
     * Gets instance of stop condition class
     * @return instance of stop condition class
     */
    public StopCondition getStopCondition() {
        return stopCondition;
    }
    /**
     * Sets instance of stop condition class
     * @param stopCondition instance of stop condition class
     */
    public void setStopCondition(StopCondition stopCondition) {
        this.stopCondition = stopCondition;
    }

    /**
     * Gets instance of queue watcher
     * @return instance of queue watcher
     */
    public QueueWatcher getQueueWatcher() {
        return queueWatcher;
    }
    /**
     * Sets instance of queue watcher
     * @param queueWatcher instance of queue watcher
     */
    public void setQueueWatcher(QueueWatcher queueWatcher) {
        this.queueWatcher = queueWatcher;
    }

    /**
     * Gets instance of response tracker
     * @return instance of response tracker
     */
    public ResponseTracker getResponseTracker() {
        return responseTracker;
    }
    /**
     * Sets instance of response tracker
     * @param responseTracker instance of response tracker
     */
    public void setResponseTracker(ResponseTracker responseTracker) {
        this.responseTracker = responseTracker;
    }

    /**
     * Sets seed for sensors count random generator
     * @param seed seed for sensors count random generator
     */
    public void setSensorCountGeneratorSeed(long seed) {
        getTaskParameters().setSensorsCountGeneratorSeed(seed);
        if(sensorCountGenerator != null) sensorCountGenerator.setSeed(seed);
    }
    /**
     * Sets seed for refresh time random generator
     * @param seed seed for refresh time random generator
     */
    public void setRefreshTimeGeneratorSeed(long seed) {
        getTaskParameters().setRefreshTimeGeneratorSeed(seed);
        if(refreshTimeGenerator != null) refreshTimeGenerator.setSeed(seed);
    }

    /**
     * Sets seed for value type random generator
     * @param seed seed for value type random generator
     */
    public void setValueTypeGeneratorSeed(long seed) {
        getTaskParameters().setValueTypeGeneratorSeed(seed);
        if(valueTypeGenerator != null) valueTypeGenerator.setSeed(seed);
    }

    /**
     * Gets flag, if simulation task is saved to XML
     * @return <code>true</code> if task is saved to XML, <code>false</code> otherwise
     */
    public boolean isSaved() {
        return saved;
    }

    /**
     * Sets flag, if simulation task is saved to XML
     * @param saved <code>true</code> if task is saved to XML, <code>false</code> otherwise
     */
    public void setSaved(boolean saved) {
        this.saved = saved;
    }
}
