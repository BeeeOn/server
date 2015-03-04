package com.iha.emulator.control;

import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.communication.server.ServerController;
import com.iha.emulator.models.Server;
import com.iha.emulator.models.value.Value;
import com.iha.emulator.utilities.AdapterLogger;
import com.iha.emulator.utilities.Utilities;
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

import java.util.Random;

/**
 * Created by Shu on 25.2.2015.
 */
public class SimulationTask {

    private static final Logger logger = LogManager.getLogger(SimulationTask.class);
    private static final Double DEFAULT_ADAPTER_FIRMWARE = 0.0;
    private static final int VALUE_TYPE_MIN = Value.Type.SENSOR_HUMIDITY.ordinal();
    private static final int VALUE_TYPE_MAX = Value.Type.SENSOR_TEMPERATURE.ordinal();

    public enum State{
        RUNNING("Running"),
        PAUSED("Paused"),
        RESUMED("Resumed"),
        STOPPED("Stopped"),
        PAUSED_SENSORS("Paused sensors"),
        RESUMED_SENSORS("Resumed sensors"),
        WAITING("Waiting"),
        FINISHED("Finished"),
        ERROR("Error");

        private final StringProperty name;

        State(String name){
            this.name = new SimpleStringProperty(name);
        }

        public String getName() {
            return name.get();
        }

        public StringProperty nameProperty() {
            return name;
        }

        public void setName(String name) {
            this.name.set(name);
        }
    }

    private BooleanProperty enabled;
    private ObjectProperty<State> simulationState;
    private ServerController serverController;
    private IntegerProperty id;
    private TaskParameters taskParameters = null;
    private ValueParameters valueParameters = null;

    private AdapterLogger log;
    private ObservableList<AdapterController> adapterControllers = FXCollections.observableArrayList();
    private ListProperty<AdapterController> adapterControllersList = new SimpleListProperty<>(adapterControllers);

    //region CONTROL
    private BooleanProperty started;
    private BooleanProperty paused;
    private BooleanProperty sensorsPaused;
    //endregion
    //region GENERATORS
    private Random sensorCountGenerator;
    private Random refreshTimeGenerator;
    private Random signalGenerator;
    private Random batteryGenerator;
    private Random valueTypeGenerator;
    //endregion


    public SimulationTask() {
        this.enabled = new SimpleBooleanProperty(true);
        this.simulationState = new SimpleObjectProperty<>(State.WAITING);
        this.id = new SimpleIntegerProperty();

        this.started = new SimpleBooleanProperty(false);
        this.paused = new SimpleBooleanProperty(true);
        this.sensorsPaused = new SimpleBooleanProperty(false);
    }

    public void start(){
        Task<Object> worker = new Task<Object>() {
            @Override
            protected Object call() throws Exception {
                LongProperty progress = new SimpleLongProperty(0);
                progress.addListener(new ChangeListener<Number>() {
                    @Override
                    public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                        if((Long)newValue > 0 && (Long)newValue < 100)
                            updateProgress((long)newValue,100);
                    }
                });
                progress.set(1);
                //set task to running simulationState
                Platform.runLater(() -> {
                    setSimulationState(SimulationTask.State.RUNNING);
                    setStarted(true);
                    setPaused(false);
                    progress.set(97);
                    logMessage("Task STARTED");
                });
                progress.set(100);
                return null;
            }
        };
        Utilities.showLoadingDialog(worker,"Starting task...");
        worker.setOnSucceeded(event -> resume());
    }

    public void pause(){
        logger.trace("Disabling adapters");
        Task<Object> worker = new Task<Object>() {
            @Override
            protected Object call() throws Exception {
                LongProperty progress = new SimpleLongProperty(0);
                progress.addListener(new ChangeListener<Number>() {
                    @Override
                    public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                        if((Long)newValue > 0 && (Long)newValue < 100)
                            updateProgress((long)newValue,100);
                    }
                });
                progress.set(1);
                long progressPerAdapter = 95/getAdapterControllers().size();
                getAdapterControllers().forEach(a->{
                    a.disable();
                    progress.set(progress.get() + progressPerAdapter);
                });
                Platform.runLater(() -> {
                    setSimulationState(SimulationTask.State.PAUSED);
                    setPaused(true);
                    logMessage("Task PAUSED");
                });
                progress.set(99);
                return null;
            }
        };
        Utilities.showLoadingDialog(worker,"Pausing task...");
    }

    public void stop(){
        pause();
        Task<Object> worker = new Task<Object>() {
            @Override
            protected Object call() throws Exception {
                Platform.runLater(()->logMessage("Task STOPPED"));
                LongProperty progress = new SimpleLongProperty(0);
                progress.addListener(new ChangeListener<Number>() {
                    @Override
                    public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                        if((Long)newValue > 0 && (Long)newValue < 100)
                            updateProgress((long)newValue,100);
                    }
                });
                progress.set(1);
                updateMessage("Deleting adapters");
                long progressPerAdapter = 95/getAdapterControllers().size();
                getAdapterControllers().forEach(a->{
                    //TODO save logs
                    //delete adapter data
                    a.delete();
                    //update process
                    progress.set(progress.get() + progressPerAdapter);
                });
                //clear adapters list
                if(getAdapterControllers().size() > 0){
                    getAdapterControllers().clear();
                }
                Platform.runLater(() -> {
                    setSimulationState(SimulationTask.State.STOPPED);
                    setStarted(false);
                    setPaused(true);
                });
                progress.set(99);
                return null;
            }
        };
        Utilities.showLoadingDialog(worker,"Stopping task...");
    }

    public void clear(){
        //clear logs
        getLog().delete();
    }

    public void resume(){
        logger.trace("Enabling adapters");
        Task<Object> worker = new Task<Object>() {
            @Override
            protected Object call() throws Exception {
                LongProperty progress = new SimpleLongProperty(0);
                progress.addListener(new ChangeListener<Number>() {
                    @Override
                    public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                        if((Long)newValue > 0 && (Long)newValue < 100)
                            updateProgress((long)newValue,100);
                    }
                });
                progress.set(1);
                long progressPerAdapter = 95/getAdapterControllers().size();
                getAdapterControllers().forEach(a->{
                    a.enable();
                    progress.set(progress.get()+progressPerAdapter);
                });
                Platform.runLater(() -> {
                    setSimulationState(SimulationTask.State.RUNNING);
                    setPaused(false);
                    logMessage("Task RESUMED");
                });
                progress.set(99);
                return null;
            }
        };
        Utilities.showLoadingDialog(worker,"Resuming task...");
    }

    public void pauseSensors(){
        Task<Object> worker = new Task<Object>() {
            @Override
            protected Object call() throws Exception {
                LongProperty progress = new SimpleLongProperty(0);
                progress.addListener(new ChangeListener<Number>() {
                    @Override
                    public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                        if((Long)newValue > 0 && (Long)newValue < 100)
                            updateProgress((long)newValue,100);
                    }
                });
                progress.set(1);
                long progressPerAdapter = 95/getAdapterControllers().size();
                getAdapterControllers().forEach(a->{
                    a.getSensorControllers().forEach(s->s.setTimerRunning(false));
                    progress.set(progress.get() + progressPerAdapter);
                });
                Platform.runLater(() -> {
                    setSimulationState(SimulationTask.State.PAUSED_SENSORS);
                    setSensorsPaused(true);
                    logMessage("Task's sensors PAUSED");
                });
                progress.set(99);
                return null;
            }
        };
        Utilities.showLoadingDialog(worker,"Pausing sensors...");
    }

    public void resumeSensors(){
        Task<Object> worker = new Task<Object>() {
            @Override
            protected Object call() throws Exception {
                LongProperty progress = new SimpleLongProperty(0);
                progress.addListener(new ChangeListener<Number>() {
                    @Override
                    public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                        if((Long)newValue > 0 && (Long)newValue < 100)
                            updateProgress((long)newValue,100);
                    }
                });
                progress.set(1);
                long progressPerAdapter = 95/getAdapterControllers().size();
                getAdapterControllers().forEach(a->{
                    a.getSensorControllers().forEach(s->s.setTimerRunning(true));
                    progress.set(progress.get() + progressPerAdapter);
                });
                Platform.runLater(() -> {
                    if(getPaused())
                        setSimulationState(SimulationTask.State.PAUSED);
                    else
                        setSimulationState(SimulationTask.State.RUNNING);
                    setSensorsPaused(false);
                    logMessage("Task's sensors RESUMED");
                });
                progress.set(99);
                return null;
            }
        };
        Utilities.showLoadingDialog(worker,"Resuming sensors...");
    }

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
            //create new server
            newAdapterController.setServerController(getServerController());
            //create logger
            newAdapterController.setLog(getLog());
            //create scheduler
            newAdapterController.createScheduler();
            //set response tracking
            newAdapterController.setTrackServerResponse(true);
            newAdapterController.setDumpServerResponse(false);
            //bind scheduler processing to adapter's status indicator
            newAdapterController.bindSchedulerProcess(newAdapterController.getAdapter(), newAdapterController.getScheduler());
            //bind register message
            newAdapterController.bindRegisterMessage(newAdapterController);
            //save it to list
            adapterControllersList.add(newAdapterController);
            //create sensors for new adapter
            createSensors(newAdapterController);
            //set loading dialog progress
            progress.set(progress.get()+oneAdapterProgress);
        }
    }

    public void createSensors(AdapterController adapterController){
        int sensorsCount;
        TaskParameters parameters = getTaskParameters();
        try{
            //determine sensors count
            if(getTaskParameters().getSensorsCountMin() == 0 || getTaskParameters().getSensorsCountMin() == getTaskParameters().getSensorsCountMax()){
                sensorsCount = getTaskParameters().getSensorsCountMax();
            }else{
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
                logMessage("Trying to create sensor: " +
                    Utilities.intToIpString(Integer.valueOf(adapterController.getAdapter().getId() + "" + adapterController.getAdapter().getId() + i))
                );
                //create value for sensor
                Value value = null;
                logger.trace("Generating value for new sensor");
                while(value == null){
                    //generate value index (index in enum Value.Type)
                    int valueIndex = Utilities.generateIntInRange(valueTypeGenerator,VALUE_TYPE_MIN,VALUE_TYPE_MAX);
                    //check if should use generated value
                    value = getValueParameters().isValueEnabled(valueIndex);
                    if(value == null)
                        logger.trace("Value not enabled -> " + valueIndex);
                }
                logger.trace("Chosen value: " + value.getName());
                //add value to array
                ObservableList<Value> tmpValues = FXCollections.observableArrayList(value);
                //TODO generate signal and battery
                //generate signal
                int signal = 90;
                //generate battery
                int battery = 80;
                //generate refresh time
                int refreshTime;
                logger.trace("Generating refresh time");
                if(getTaskParameters().getRefreshTimeMin() == 0 || getTaskParameters().getRefreshTimeMin() == getTaskParameters().getRefreshTimeMax()) {
                    refreshTime = getTaskParameters().getRefreshTimeMax();
                }
                else{
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
                        Integer.valueOf(adapterController.getAdapter().getId() + "" + adapterController.getAdapter().getId() + i), //sensor id
                        "Sensor", // sensor name
                        battery, //battery
                        signal, //signal
                        refreshTime, //refresh time
                        adapterController.getAdapter().getProtocol() //communication protocol
                );
                tmp.setFullMessage(true);
                tmp.setIgnoreRefreshChange(true);
                adapterController.setSaved(true);
                logMessage("Sensor/" + tmp.getSensorIdAsIp() + " created successfully");
            }
        }catch (IllegalArgumentException e ){
            Utilities.showException(logger, "Cannot create sensors for adapter " + adapterController.getAdapter().getId(), e, false, null);
        }

    }

    public void logMessage(String message){
        if(this.log != null) log.log(message);
        else logger.error("No logger");
    }

    public ServerController createServer(boolean conn,String name,String ip, int port,String databaseName){
        logger.trace("Creating server model: " + "conn -> "+conn + " name-> "+name + " ip-> "+ip + " port-> "+port + " DB-> "+databaseName);
        setServerController(new ServerController(new Server(conn,name,ip,port,databaseName)));
        logger.trace("OK");
        return this.serverController;
    }

    public void createLog(TabPane panel){
        this.log = new AdapterLogger(panel);
    }

    public TaskParameters createTaskParameters(int adaptersCount,Protocol.Version version, int startId,int sensorsCountMin,int sensorsCountMax,int refreshTimeMin,int refreshTimeMax) throws IllegalArgumentException{
        this.taskParameters = new TaskParameters();
        //set task parameters
        taskParameters.setAdaptersCount(adaptersCount);
        taskParameters.setProtocolVersion(version);
        taskParameters.setStartId(startId);
        taskParameters.setSensorsCountMin(sensorsCountMin);
        taskParameters.setSensorsCountMax(sensorsCountMax);
        taskParameters.setRefreshTimeMin(refreshTimeMin);
        taskParameters.setRefreshTimeMax(refreshTimeMax);
        //create task's generators
        createGenerators();
        return taskParameters;
    }

    private void createGenerators(){
        sensorCountGenerator = new Random(getTaskParameters().getSensorsCountGeneratorSeed());
        refreshTimeGenerator = new Random(getTaskParameters().getSensorsCountGeneratorSeed());
        batteryGenerator = new Random(getTaskParameters().getSensorsCountGeneratorSeed());
        signalGenerator = new Random(getTaskParameters().getSensorsCountGeneratorSeed());
        valueTypeGenerator = new Random(getTaskParameters().getValueTypeGeneratorSeed());
    }

    public String toString(){
        return "Task/"+getId();
    }

    public boolean getEnabled() {
        return enabled.get();
    }

    public BooleanProperty enabledProperty() {
        return enabled;
    }

    public void setEnabled(boolean enabled) {
        this.enabled.set(enabled);
    }

    public State getSimulationState() {
        return simulationState.get();
    }

    public ObjectProperty<State> simulationStateProperty() {
        return simulationState;
    }

    public void setSimulationState(State simulationState) {
        this.simulationState.set(simulationState);
    }

    public ServerController getServerController() {
        return serverController;
    }

    public void setServerController(ServerController serverController) {
        this.serverController = serverController;
    }

    public int getId() {
        return id.get();
    }

    public IntegerProperty idProperty() {
        return id;
    }

    public void setId(int id) {
        this.id.set(id);
    }

    public boolean getStarted() {
        return started.get();
    }

    public BooleanProperty startedProperty() {
        return started;
    }

    public void setStarted(boolean started) {
        logger.debug(toString() + " -> setting started -> " + started);
        this.started.set(started);
    }

    public boolean getPaused() {
        return paused.get();
    }

    public BooleanProperty pausedProperty() {
        return paused;
    }

    public void setPaused(boolean paused) {
        logger.debug(toString() + " -> setting paused -> " + paused);
        this.paused.set(paused);
    }

    public boolean getSensorsPaused() {
        return sensorsPaused.get();
    }

    public BooleanProperty sensorsPausedProperty() {
        return sensorsPaused;
    }

    public void setSensorsPaused(boolean sensorsPaused) {
        this.sensorsPaused.set(sensorsPaused);
    }

    public TaskParameters getTaskParameters() {
        return taskParameters;
    }

    public ValueParameters getValueParameters() {
        return valueParameters;
    }

    public void setValueParameters(ValueParameters valueParameters) {
        this.valueParameters = valueParameters;
    }

    public AdapterLogger getLog() {
        return log;
    }

    public void setLog(AdapterLogger log) {
        this.log = log;
    }

    public ObservableList<AdapterController> getAdapterControllers() {
        return adapterControllers;
    }

    public ObservableList<AdapterController> getAdapterControllersList() {
        return adapterControllersList.get();
    }

    public ListProperty<AdapterController> adapterControllersListProperty() {
        return adapterControllersList;
    }

    public void setAdapterControllersList(ObservableList<AdapterController> adapterControllersList) {
        this.adapterControllersList.set(adapterControllersList);
    }
}
