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
 * Created by Shu on 25.2.2015.
 */
public class SimulationTask {

    private static final Logger logger = LogManager.getLogger(SimulationTask.class);
    private static final Double DEFAULT_ADAPTER_FIRMWARE = 0.0;
    public static final int VALUE_TYPE_MIN = Value.Type.SENSOR_HUMIDITY.ordinal();
    public static final int VALUE_TYPE_MAX = Value.Type.SENSOR_TEMPERATURE.ordinal();

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

    private SimulationTask me;
    private boolean saved = false;
    private BooleanProperty enabled;
    private ObjectProperty<State> simulationState;
    private ServerController serverController;
    private IntegerProperty id;
    private TaskParameters taskParameters = null;
    private StopCondition stopCondition = null;
    private ResponseTracker responseTracker = new ResponseTracker(true,false,true);

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

    private QueueWatcher queueWatcher= null;

    public SimulationTask() {
        this.enabled = new SimpleBooleanProperty(true);
        this.simulationState = new SimpleObjectProperty<>(State.READY);
        this.id = new SimpleIntegerProperty();

        this.running = new SimpleBooleanProperty(false);
        this.finished = new SimpleBooleanProperty(false);
        this.sensorsPaused = new SimpleBooleanProperty(false);
        this.me = this;
    }

    public void start(){
        switch (getSimulationState()){
            case READY:
            case PAUSED:
                logger.trace("Enabling adapters");
                Task<Object> worker = new Task<Object>() {

                    LongProperty taskProgress = new SimpleLongProperty(0);

                    @Override
                    protected Object call() throws Exception {
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
                        //set task to running simulationState
                        updateMessage("Enabling adapters");
                        for(AdapterController a :getAdapterControllers()){
                            a.enable();
                        }
                        Platform.runLater(() -> logMessage("Task RUNNING"));
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
                        super.failed();
                        Utilities.showException(logger,"Start task failed",getException(),false,null);
                    }

                };
                Utilities.showLoadingDialog(worker,"Starting task...");
                worker.setOnSucceeded(event -> {
                    setSimulationState(State.RUNNING);
                    setRunning(true);
                    getTaskParameters().getStopWatch().start();
                });
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

                    LongProperty taskProgress = new SimpleLongProperty(0);

                    @Override
                    protected Object call() throws Exception {

                        taskProgressProperty().addListener(new ChangeListener<Number>() {
                            @Override
                            public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                                if ((Long) newValue > 0 && (Long) newValue < 100)
                                    updateProgress((long) newValue, 100);
                            }
                        });
                        setTaskProgress(1);
                        long progressPerAdapter = 95/getAdapterControllers().size();
                        for(AdapterController a :getAdapterControllers()){
                            a.disable();
                            setTaskProgress(getTaskProgress() + progressPerAdapter);
                        }
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
                        super.failed();
                        Utilities.showException(logger,"Pause task failed",getException(),false,null);
                    }
                };
                Utilities.showLoadingDialog(worker,"Pausing task...");
                worker.setOnSucceeded(event -> {
                    Platform.runLater(() -> logMessage("Task PAUSED"));
                    setSimulationState(State.PAUSED);
                    getTaskParameters().getStopWatch().pause();
                });
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

                    LongProperty taskProgress = new SimpleLongProperty(0);

                    @Override
                    protected Object call() throws Exception {
                        setTaskProgress(1);
                        updateMessage("Terminating adapters");
                        long progressPerAdapter = 35/getAdapterControllers().size();
                        for(AdapterController a: getAdapterControllers()){
                            a.terminateScheduler();
                            setTaskProgress(getTaskProgress()+progressPerAdapter);
                        }
                        getTaskParameters().getStopWatch().stop();
                        updateMessage("Deleting adapters from database");
                        logger.trace("Deleting adapters from database");
                        //TODO find a way to remove adapters from DB
                        //removeAdaptersFromDb();
                        updateMessage("Saving log file : " + getLog().getBufferFile().getAbsolutePath());
                        logger.trace("Saving log file : " + getLog().getBufferFile().getAbsolutePath());
                        logMessage("Task FINISHED");
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
                        getAdapterControllers().clear();
                        adapterControllersList.clear();
                        System.gc();
                        logger.trace("Closing buffer");
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
                        super.failed();
                        Utilities.showException(logger,"Stop task failed",getException(),false,null);
                    }
                };
                if(pauseTask != null){
                    pauseTask.setOnSucceeded(e -> finishStop(worker));
                }else{
                    finishStop(worker);
                }
                return worker;
        }
        return null;
    }

    private void finishStop(Task<Object> worker){
        Utilities.showLoadingDialog(worker,"Finishing task...");
        worker.setOnSucceeded(event -> {
            if(!getSimulationState().equals(State.ERROR)){
                setSimulationState(State.FINISHED);
            }
            //getAdapterControllers().forEach(AdapterController::delete);
            setRunning(false);
            if(getQueueWatcher() != null) getQueueWatcher().next();
        });
    }

    public Task restart(){
        logger.debug("Restarting task");
        switch (getSimulationState()){
            case ERROR:
            case FINISHED:
                Task<Object> worker = new Task<Object>() {

                    LongProperty taskProgress = new SimpleLongProperty(0);

                    @Override
                    protected Object call() throws Exception {
                        Platform.runLater(()->logMessage("Task STOPPED"));
                        taskProgressProperty().addListener(new ChangeListener<Number>() {
                            @Override
                            public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                                if ((Long) newValue > 0 && (Long) newValue < 100)
                                    updateProgress((long) newValue, 100);
                            }
                        });
                        setTaskProgress(1);
                        updateMessage("Clearing logs");
                        getLog().delete();
                        setTaskProgress(20);
                        updateMessage("Buffering logs");
                        getLog().createLogs();
                        setTaskProgress(40);
                        try{
                            getLog().openBufferFile();
                            getLog().writeTaskLogHeaderToBuffer(me);
                        }catch (IOException e){
                            Platform.runLater(()-> Utilities.showException(logger, "Cannot open buffer file", e, true, event -> stop()));
                        }
                        setTaskProgress(60);
                        updateMessage("Deleting responses");
                        clearResponseTracker();
                        updateMessage("Reset message tracker");
                        clearMessageTracker();
                        setTaskProgress(80);
                        updateMessage("Unregister adapters");
                        setSensorsPaused(false);
                        //restartStopConditions();
                        Platform.runLater(()->getTaskParameters().getStopWatch().reset());
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
                        super.failed();
                        Utilities.showException(logger,"Restart task failed",getException(),false,null);
                    }
                };
                Utilities.showLoadingDialog(worker,"Restarting task...");
                worker.setOnSucceeded(event -> {
                    setSimulationState(State.READY);
                    setRunning(false);
                });
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
        stop();
        /*//disable adapters
        getAdapterControllers().forEach(a->{
            a.disable();
            a.clearScheduler();
        });
        getAdapterControllers().forEach(AdapterController::delete);
        getAdapterControllers().clear();
        if(getTaskParameters().getStopWatch() != null){
            logMessage("Task duration: " + getTaskParameters().getStopWatch().getTimeString());
            getTaskParameters().getStopWatch().stop();
        }
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
        if(getQueueWatcher() != null) getQueueWatcher().next();
        setRunning(false);*/
    }

    private void clearResponseTracker(){
        if(getResponseTracker() != null){
            getResponseTracker().clearResponses();
        }else{
            setResponseTracker(new ResponseTracker(true,false,true));
        }
    }

    private void clearMessageTracker(){
        if(getLog() == null) return;
        Platform.runLater(()->getLog().getMessageTracker().clearCounters());
    }

    private void restartStopConditions(){
        if(getStopCondition() != null) getStopCondition().restart();
    }

    private void removeAdaptersFromDb(){
        EmulatorServerClient server;
        try{
            server = new EmulatorServerClient(getServerController().getModel().getIp());
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

    public void delete(){
        logger.debug("Deleting " + toString());
        logger.trace("Dumping logs");
        getLog().closeBuffer();
        logger.trace("Clearing responses");
        getResponseTracker().clearResponses();
        logger.trace("Unregistering stop conditions");
        getStopCondition().unregisterAllConditions();
        logger.trace("Clearing adapters");
        getAdapterControllers().clear();
    }

    protected void finalize(){
        logger.warn("Finalizing " + toString());
    }

    public synchronized void logMessage(String message){
        if(this.log != null) log.log(message);
        else logger.error("No logger");
    }

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

    public ServerController createServer(boolean conn,String name,String ip, int port,String databaseName){
        logger.trace("Creating server model: " + "conn -> "+conn + " name-> "+name + " ip-> "+ip + " port-> "+port + " DB-> "+databaseName);
        setServerController(new ServerController(new Server(conn,name,ip,port,databaseName)));
        logger.trace("OK");
        return this.serverController;
    }

    public ServerController createServer(Server model){
        logger.trace("Setting server: " + model.getName());
        setServerController(new ServerController(model));
        logger.trace("OK");
        return this.serverController;
    }

    public void createLog(TabPane panel){
        //create log
        this.log = new AdapterLogger(panel);
        logger.trace("Logger created");
        //register error handler
        this.log.getErrorHandler().addErrorListener(this::criticalStop);
        logger.trace("Listener added");
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

    public StopCondition createStopCondition(){
        this.stopCondition = new StopCondition(this);
        return stopCondition;
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
        return getTaskParameters().getValueParameters();
    }

    public void setValueParameters(ValueParameters valueParameters) {
        getTaskParameters().setValueParameters(valueParameters);
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

    public StopCondition getStopCondition() {
        return stopCondition;
    }

    public void setStopCondition(StopCondition stopCondition) {
        this.stopCondition = stopCondition;
    }

    public QueueWatcher getQueueWatcher() {
        return queueWatcher;
    }

    public void setQueueWatcher(QueueWatcher queueWatcher) {
        this.queueWatcher = queueWatcher;
    }

    public ResponseTracker getResponseTracker() {
        return responseTracker;
    }

    public void setResponseTracker(ResponseTracker responseTracker) {
        this.responseTracker = responseTracker;
    }

    public void setSensorCountGeneratorSeed(long seed) {
        getTaskParameters().setSensorsCountGeneratorSeed(seed);
        if(sensorCountGenerator != null) sensorCountGenerator.setSeed(seed);
    }

    public void setRefreshTimeGeneratorSeed(long seed) {
        getTaskParameters().setRefreshTimeGeneratorSeed(seed);
        if(refreshTimeGenerator != null) refreshTimeGenerator.setSeed(seed);
    }

    public void setValueTypeGeneratorSeed(long seed) {
        getTaskParameters().setValueTypeGeneratorSeed(seed);
        if(valueTypeGenerator != null) valueTypeGenerator.setSeed(seed);
    }

    public boolean isSaved() {
        return saved;
    }

    public void setSaved(boolean saved) {
        this.saved = saved;
    }
}
