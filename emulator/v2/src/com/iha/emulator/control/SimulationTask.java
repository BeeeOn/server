package com.iha.emulator.control;

import com.iha.emulator.communication.eserver.EmulatorServerClient;
import com.iha.emulator.communication.eserver.task.ServerTask;
import com.iha.emulator.communication.eserver.task.TaskParser;
import com.iha.emulator.communication.eserver.task.implemented.DeleteAdapterTask;
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
import org.dom4j.DocumentException;

import java.io.IOException;
import java.net.UnknownHostException;
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
        READY("Ready"),
        RUNNING("Running"),
        PAUSED("Paused"),
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
    private BooleanProperty running;
    private BooleanProperty finished;
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
        this.simulationState = new SimpleObjectProperty<>(State.READY);
        this.id = new SimpleIntegerProperty();

        this.running = new SimpleBooleanProperty(false);
        this.finished = new SimpleBooleanProperty(false);
        this.sensorsPaused = new SimpleBooleanProperty(false);
    }

    public void start(){
        switch (getSimulationState()){
            case READY:
            case PAUSED:
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
                        //set task to running simulationState
                        long progressPerAdapter = 95/getAdapterControllers().size();
                        getAdapterControllers().forEach(a->{
                            a.enable();
                            progress.set(progress.get()+progressPerAdapter);
                        });
                        Platform.runLater(() -> logMessage("Task RUNNING"));
                        progress.set(100);
                        return null;
                    }
                };
                Utilities.showLoadingDialog(worker,"Starting task...");
                worker.setOnSucceeded(event -> setSimulationState(State.RUNNING));
                break;
            case FINISHED:
                Task restartTask = restart();
                if(restartTask != null)
                    restartTask.setOnSucceeded(event -> start());
                break;
        }
    }

    public Task pause(){
        switch (getSimulationState()){
            case ERROR:
            case RUNNING:
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
                        Platform.runLater(() -> logMessage("Task PAUSED"));
                        progress.set(99);
                        return null;
                    }
                };
                Utilities.showLoadingDialog(worker,"Pausing task...");
                worker.setOnSucceeded(event -> setSimulationState(State.PAUSED));
                return worker;
        }
        return null;
    }

    public Task stop(){
        switch (getSimulationState()){
            case RUNNING:
            case PAUSED:
            case ERROR:
                //pause task if necessary
                Task pauseTask = pause();
                //delete adapters from emulator and database
                Task<Object> worker = new Task<Object>() {
                    @Override
                    protected Object call() throws Exception {
                        Platform.runLater(()->logMessage("Task STOPPED"));
                        LongProperty progress = new SimpleLongProperty(0);
                        progress.addListener(new ChangeListener<Number>() {
                            @Override
                            public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                                if ((Long) newValue > 0 && (Long) newValue < 100)
                                    updateProgress((long) newValue, 100);
                            }
                        });
                        progress.set(1);
                        getAdapterControllers().forEach(AdapterController::clearScheduler);
                        updateMessage("Deleting adapters from database");
                        logger.trace("Deleting adapters from database");
                        removeAdaptersFromDb();
                        progress.set(30);
                        updateMessage("Saving log file : " + getLog().getBufferFile().getAbsolutePath());
                        logger.trace("Saving log file : " + getLog().getBufferFile().getAbsolutePath());
                        logMessage("Task FINISHED");
                        logger.trace("Closing buffer");
                        getLog().closeBuffer();
                        progress.set(40);
                        progress.set(99);
                        return null;
                    }
                };
                if(pauseTask != null){
                    pauseTask.setOnSucceeded(e -> {
                        Utilities.showLoadingDialog(worker,"Finishing task...");
                        worker.setOnSucceeded(event -> {
                            if(!getSimulationState().equals(State.ERROR)){
                                setSimulationState(State.FINISHED);
                            }
                        });
                    });
                }else{
                    Utilities.showLoadingDialog(worker, "Finishing task...");
                    worker.setOnSucceeded(event -> {
                        if(!getSimulationState().equals(State.ERROR)){
                            setSimulationState(State.FINISHED);
                        }
                    });
                }

                return worker;
        }
        return null;
    }

    public Task restart(){
        logger.debug("Restarting task");
        switch (getSimulationState()){
            case ERROR:
            case FINISHED:
                Task<Object> worker = new Task<Object>() {
                    @Override
                    protected Object call() throws Exception {
                        Platform.runLater(()->logMessage("Task STOPPED"));
                        LongProperty progress = new SimpleLongProperty(0);
                        progress.addListener(new ChangeListener<Number>() {
                            @Override
                            public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                                if ((Long) newValue > 0 && (Long) newValue < 100)
                                    updateProgress((long) newValue, 100);
                            }
                        });
                        progress.set(1);
                        updateMessage("Clearing logs");
                        getLog().delete();
                        updateMessage("Buffering logs");
                        getLog().createLogs();
                        try{
                            getLog().openBufferFile();
                        }catch (IOException e){
                            Platform.runLater(()-> Utilities.showException(logger, "Cannot open buffer file", e, true, event -> stop()));
                        }
                        updateMessage("Deleting responses");
                        clearResponseTrackers();
                        clearMessageTracker();
                        updateMessage("Unregister adapters");
                        getAdapterControllers().forEach(a->{
                            a.setRegisterMessageSent(false);
                            a.getAdapter().setRegistered(false);
                            a.restartValueGenerators();
                        });
                        setSensorsPaused(false);
                        logMessage("Task RESTARTED");
                        progress.set(99);
                        return null;
                    }
                };
                Utilities.showLoadingDialog(worker,"Restarting task...");
                worker.setOnSucceeded(event -> setSimulationState(State.READY));
                return worker;
            default:
                Task stopTask = stop();
                if(stopTask != null) stopTask.setOnSucceeded(event -> restart());
        }
        return null;
    }

    public void criticalStop(){
        if(getSimulationState() == State.ERROR) return;
        setSimulationState(State.ERROR);
        //stop();
        //disable adapters
        getAdapterControllers().forEach(a->{
            a.disable();
            a.clearScheduler();
        });
        getLog().closeBuffer();
        Task<Object> worker = new Task<Object>() {
            @Override
            protected Object call() throws Exception {
                updateProgress(10,100);
                updateMessage("Removing adapters");
                removeAdaptersFromDb();
                updateProgress(100,100);
                return null;
            }
        };
        Utilities.showLoadingDialog(worker,"Removing adapters from database");
    }

    private void clearResponseTrackers(){
        if(getAdapterControllers() == null) return;
        getAdapterControllers().forEach(AdapterController::clearResponseTracker);
    }

    private void clearMessageTracker(){
        if(getLog() == null) return;
        Platform.runLater(()->getLog().getMessageTracker().clearCounters());
    }

    private void removeAdaptersFromDb(){
        EmulatorServerClient server;
        try{
            server = new EmulatorServerClient();
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

    public void clear(){
        //clear logs
        getLog().delete();
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
                    a.getSensorControllers().forEach(SensorController::disable);
                    progress.set(progress.get() + progressPerAdapter);
                });
                Platform.runLater(() -> logMessage("Task's sensors PAUSED"));
                progress.set(99);
                return null;
            }
        };
        Utilities.showLoadingDialog(worker,"Pausing sensors...");
        worker.setOnSucceeded(event -> setSensorsPaused(true));
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
                    a.getSensorControllers().forEach(SensorController::enable);
                    progress.set(progress.get() + progressPerAdapter);
                });
                Platform.runLater(() -> logMessage("Task's sensors RESUMED"));
                progress.set(99);
                return null;
            }
        };
        Utilities.showLoadingDialog(worker,"Resuming sensors...");
        worker.setOnSucceeded(event -> setSensorsPaused(false));
    }

    public void createAdapters(LongProperty progress){
        if(taskParameters == null){
            logger.error(toString() + " missing task parameters, cannot create adapters!");
            return;
        }
        //loading dialog progress help variable; 95% / number of adapters
        long oneAdapterProgress = (90-progress.get())/getTaskParameters().getAdaptersCount();
        int sensorsTotal = 0;
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
            //create sensors for new adapter and save devices count
            getTaskParameters().setSensorsCount(getTaskParameters().getSensorsCount() + createSensors(newAdapterController));
            //set loading dialog progress
            progress.set(progress.get()+oneAdapterProgress);
        }
        logMessage("Created " + sensorsTotal + " sensors.");
    }

    public int createSensors(AdapterController adapterController){
        int sensorsCount,sensorsTotal = 0;
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
                sensorsTotal++;
            }
            return sensorsTotal;
        }catch (IllegalArgumentException e ){
            Utilities.showException(logger, "Cannot create sensors for adapter " + adapterController.getAdapter().getId(), e, false, null);
        }
        return 0;
    }

    public synchronized void logMessage(String message){
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
        //create log
        this.log = new AdapterLogger(panel);
        //register error handler
        this.log.getErrorHandler().addErrorListener(this::criticalStop);
    }

    public TaskParameters createTaskParameters(int adaptersCount,Protocol.Version version, int startId,int sensorsCountMin,int sensorsCountMax,int refreshTimeMin,int refreshTimeMax,String saveDir) throws IllegalArgumentException{
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
        logger.debug("Setting task state -> "  + simulationState.getName());
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

    public boolean getRunning() {
        return running.get();
    }

    public BooleanProperty runningProperty() {
        return running;
    }

    public void setRunning(boolean running) {
        logger.debug(toString() + " -> setting running -> " + running);
        this.running.set(running);
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

    public boolean getFinished() {
        return finished.get();
    }

    public BooleanProperty finishedProperty() {
        return finished;
    }

    public void setFinished(boolean finished) {
        logger.debug(toString() + " -> setting finished -> " + finished);
        this.finished.set(finished);
    }
}
