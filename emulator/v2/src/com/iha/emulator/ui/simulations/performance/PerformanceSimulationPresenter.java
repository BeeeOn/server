package com.iha.emulator.ui.simulations.performance;

import com.iha.emulator.communication.protocol.ProtocolFactory;
import com.iha.emulator.control.task.SimulationTask;
import com.iha.emulator.control.task.StopCondition;
import com.iha.emulator.control.task.ValueParameters;
import com.iha.emulator.models.Server;
import com.iha.emulator.models.value.Value;
import com.iha.emulator.models.value.ValueFactory;
import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.dialogs.task.AddNewTaskDialogPresenter;
import com.iha.emulator.ui.dialogs.task.DeleteTasksDialogPresenter;
import com.iha.emulator.ui.panels.chart.ResponseChart;
import com.iha.emulator.ui.panels.server.details.ServerDetailsPresenter;
import com.iha.emulator.ui.panels.task.cell.*;
import com.iha.emulator.ui.panels.task.details.TaskDetailsPresenter;
import com.iha.emulator.utilities.Utilities;
import com.iha.emulator.utilities.logging.AdapterLogger;
import com.iha.emulator.utilities.logging.TextAreaAppender;
import com.iha.emulator.utilities.watchers.MemoryChecker;
import com.iha.emulator.utilities.watchers.QueueWatcher;
import javafx.application.HostServices;
import javafx.application.Platform;
import javafx.beans.binding.BooleanBinding;
import javafx.beans.property.*;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.concurrent.Task;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.FlowPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.StackPane;
import javafx.stage.Modality;
import javafx.stage.Stage;
import jfxtras.scene.control.LocalDateTimeTextField;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.dom4j.*;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Properties;
import java.util.stream.Collectors;

/**
 * Class providing logic to user interactions for "Performance simulation". Part Presenter of MVP design pattern.
 * Also serving main window of "Performance simulation"
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class PerformanceSimulationPresenter implements Presenter{
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(PerformanceSimulationPresenter.class);
    /** default location for saving XML files */
    private static final String SAVES_DEFAULT_DIR = "saved/tasks";
    /** path to FXML file */
    private static final String FXML_PATH = "PerformanceSimulation.fxml";
    /** path to CSS style file */
    private static final String CSS_PATH = "/com/iha/emulator/resources/css/theme-light.css";
    /** view */
    private Display view;
    /** window */
    private Stage window;
    /** host services */
    private HostServices hostServices;
    /** instance of memory checker responsible for checking memory usage */
    private MemoryChecker memoryChecker = MemoryChecker.getInstance();
    /** properties loaded from configuration file */
    private Properties properties;
    /** watcher responsible for automatic task processing */
    private QueueWatcher queueWatcher;

    //region PRESENTERS
    /** server panel */
    private ServerDetailsPresenter serverDetailsPresenter;
    /** task details panel */
    private TaskDetailsPresenter taskDetailsPresenter;
    //endregion
    /** chart showing response times */
    private ResponseChart responseChart;
    //region TASKS
    /** currently selected task */
    private ObjectProperty<SimulationTask> currentTask;
    /** list of all created tasks */
    private ObservableList<SimulationTask> tasks = FXCollections.observableArrayList();
    /** tasks list property */
    private ListProperty<SimulationTask> tasksList = new SimpleListProperty<>(tasks);
    //endregion
    /**
     * Interface implemented by "Performance simulation" view.
     */
    public interface Display {
        public void setPresenter(PerformanceSimulationPresenter presenter);
        public Node getView();
        public TabPane getLogTabPane();
        public Tab getErrorTab();
        public void setStatusLine(String status);
        public void setStatusIndicator(boolean visible);
        public ProgressBar getMemCheckProgressBar();
        public Label getMemCheckStatusLabel();
        public TextArea getApplicationLogTextArea();
        public void addServerDetailsView(Node serverDetailsView);
        public void addTaskDetailsView(Node taskDetailsView);
        public Button getSaveAllBtn();
        public MenuItem getOpenItem();
        public Button getOpenTBtn();
        public FlowPane getTaskDetailsContainer();
        public Node getAdapterLogContainer();
        public Node getToBeSentLogContainer();
        public Node getErrorLogContainer();
        public ToggleGroup getLogMessageTypeRadBtnGroup();
        public RadioButton getFullLogMessageRadBtn();
        public RadioButton getPartialLogMessageRadBtn();
        public RadioButton getShortLogMessageRadBtn();
        public Button getShowFullLogBtn();
        public MenuItem getNewTaskItem();
        public MenuItem getStartTaskItem();
        public MenuItem getPauseTaskItem();
        public MenuItem getStopTaskItem();
        public MenuItem getResumeSensorsItem();
        public MenuItem getPauseSensorsItem();
        public Button getStartTaskTBtn();
        public Button getPauseTaskTBtn();
        public Button getStopTaskTBtn();
        public Button getPauseSensorsTBtn();
        public Button getResumeSensorsTBtn();
        public Button getDeleteTaskTBtn();
        public Button getNewTaskTBtn();
        public TableColumn getIdColumn();
        public TableColumn getServerColumn();
        public TableColumn getStateColumn();
        public TableColumn getEnabledColumn();
        public TableColumn getStopConditionColumn();
        public HBox getQueueProcessContainer();
        public Button getQueueProcessOnBtn();
        public Button getQueueProcessOffBtn();
        public LocalDateTimeTextField getStartDateTimeTextField();
        public void setStartDateTimeTextField(LocalDateTimeTextField startDateTimeTextField);
        public RadioButton getPickTimeStartRadBtn();
        public RadioButton getImmediatelyStartRadBtn();
        public TableView getTasksTable();
        public StackPane getResponseChartContainer();
    }

    //region constructor
    /**
     * Creates new "Performance simulation" presenter. Sets current simulation task to null.
     * @param window parent window
     */
    public PerformanceSimulationPresenter(Stage window) {
        this.window = window;
        this.currentTask = new SimpleObjectProperty<>(null);
    }
    //endregion

    //region public methods

    /**
     * Method start or restarts currently selected simulation task according to it's {@link com.iha.emulator.control.task.SimulationTask.State}
     */
    public void startTask(){
        if(getCurrentTask() == null) return;
        switch (getCurrentTask().getSimulationState()){
            case ERROR:
            case FINISHED:
                setStatus("Restarting task " + getCurrentTask().getId(),true);
                logger.debug(getCurrentTask().toString() + " -> RESTARTING");
                getCurrentTask().restart();
                setStatus(getCurrentTask().toString() + " restarted",false);
                break;
            default:
                setStatus("Starting task " + getCurrentTask().getId(),true);
                logger.debug(getCurrentTask().toString() + " -> STARTING");
                getCurrentTask().start();
                setStatus(getCurrentTask().toString() + " started",false);
                break;
        }
    }

    /**
     * Method stops currently selected simulation task.
     */
    public void stopTask(){
        if(getCurrentTask() == null) return;
        logger.debug(getCurrentTask().toString() + " -> STOPPING");
        setStatus("Stopping task " + getCurrentTask().getId(),true);
        getCurrentTask().stop();
        setStatus(getCurrentTask().toString() + " stopped",false);
    }
    /**
     * Method pauses currently selected simulation task.
     */
    public void pauseTask(){
        if(getCurrentTask() == null) return;
        logger.debug(getCurrentTask().toString() + " -> PAUSING");
        getCurrentTask().pause();
        setStatus(getCurrentTask().toString() + " paused",false);
    }
    /**
     * Method resumes currently selected simulation task sensors.
     */
    public void resumeSensors(){
        if(getCurrentTask() == null) return;
        logger.debug(getCurrentTask().toString() + " -> RESUMING SENSORS");
        getCurrentTask().resumeSensors();
    }
    /**
     * Method pauses currently selected simulation task sensors.
     */
    public void pauseSensors(){
        logger.debug(getCurrentTask().toString() + " -> PAUSING SENSORS");
        getCurrentTask().pauseSensors();
    }
    /**
     * Creates server models from properties and invokes showing of "Add task dialog". Uses separate {@link javafx.concurrent.Task}
     * and shows tasks progress in progress dialog.
     */
    public void addTask(){
        logger.trace("Creating new task");
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
                ObservableList<Server> tmp;
                try{
                    //create server models from properties
                    tmp = Utilities.buildServersFromProperties(properties,progress);
                    final ObservableList<Server> finalTmp = tmp;
                    //show add task dialog with created server models
                    Platform.runLater(() -> showAddTaskDialog(finalTmp));
                }catch (IllegalArgumentException e){
                    //show add adapter dialog without server models
                    Platform.runLater(() -> showAddTaskDialog(null));
                    Platform.runLater(() -> Utilities.showException(logger, "Cannot load settings from properties file", e, false, null));
                }
                return null;
            }
        };
        //show progress dialog
        Utilities.showLoadingDialog(worker, "Loading...");
    }
    /**
     * Shows {@link com.iha.emulator.ui.dialogs.task.DeleteTasksDialogPresenter} in new window, but only if no task
     * is currently selected or currently selected task is not running. Otherwise informs user, that cannot delete
     * tasks while simulation is running.
     */
    public void deleteTask(){
        //if currently selected task is running
        if(getCurrentTask() != null && getCurrentTask().getRunning()){
            //notify user
            logger.trace("Showing delete conformation dialog");
            Alert dlg = new Alert(Alert.AlertType.WARNING,"");
            dlg.initModality(Modality.WINDOW_MODAL);
            dlg.initOwner(window);
            dlg.setTitle("Delete tasks");
            dlg.getDialogPane().setContentText("Current task is running. Please stop every task, before trying to delete.");
            dlg.show();
        }else{
            //no task is running, set current task to null
            switchCurrentTask(null);
            logger.trace("Showing delete dialog");
            DeleteTasksDialogPresenter deleteTasksDialogPresenter;
            try{
                //create window
                Stage stage = new Stage();
                //create presenter
                deleteTasksDialogPresenter = new DeleteTasksDialogPresenter(stage,this);
                stage.setTitle("Delete tasks");
                //create and initiate scene
                Scene scene = new Scene((Parent) deleteTasksDialogPresenter.loadView());
                // set css for view
                logger.trace("Loading CSS from: " + CSS_PATH);
                scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
                stage.setScene(scene);
                stage.setResizable(false);
                //show dialog
                stage.show();
            } catch (IOException e) {
                Utilities.showException(logger, "Cannot load dialog for deleting adapter!", e, false, null);
            }
        }
    }

    /**
     * Removes given simulation task from list of created tasks
     * @param task task to be removed
     */
    public void deleteTask(SimulationTask task){
        //remove all tasks parts
        task.delete();
        //remove it from list
        for(Iterator<SimulationTask> i = getTasks().iterator();i.hasNext();){
            SimulationTask tmp = i.next();
            if(tmp.toString().equals(task.toString())){
                i.remove();
            }
        }
        //getTasks().remove(task);
        logger.debug("Tasks count after delete -> " + getTasks().size());
        getTasks().forEach(t->logger.trace(t.toString()));
        //invoke garbage collector
        System.gc();
    }

    /**
     * Creates XML document for all given simulation tasks in list, that is saved to file chosen by invoked file save dialog.
     * Uses separate {@link javafx.concurrent.Task}. If given list is null, all created tasks are saved.
     *
     * @param tasks list of tasks to be saved, if null all created tasks are saved
     */
    public void saveAll(ObservableList<SimulationTask> tasks){
        ObservableList<SimulationTask> taskToBeSaved;
        //if no tasks are given, save all created tasks
        if(tasks == null){
            taskToBeSaved = getTasks();
        }else{
            taskToBeSaved = tasks;
        }
        if(taskToBeSaved.size() < 1) return;
        logger.trace("Saving tasks");
        //initiate progress task
        Task<Document> worker = new Task<Document>() {
            @Override
            protected Document call() throws Exception {
                logger.trace("Creating XML file");
                //notify user
                updateMessage("Creating XML file");
                updateProgress(20,100);
                //create blank XML document
                Document doc = DocumentFactory.getInstance().createDocument();
                //create root element
                doc.addElement("tasks");
                //save all tasks to root element
                for(SimulationTask task : taskToBeSaved){
                    logger.debug("Saving task -> " + task.getId());
                    doc = task.saveToXml(doc);
                    task.setSaved(true);
                    logger.debug("Done saving task -> " + task.getId());
                }
                return doc;
            }

            @Override
            protected void failed() {
                super.failed();
                Utilities.showException(logger, "Saving to XML failed", getException(), false, null);
            }
        };
        //define action after saving task success
        worker.setOnSucceeded(event -> {
            logger.trace("Trying to save to XML file");
            //show save dialog
            String filename = Utilities.saveDialogForXML(window, SAVES_DEFAULT_DIR, worker.getValue().asXML());
            if(filename != null){
                showInformation("File saved", "Task/s successfully saved", "Saved to file \"" + filename + "\"");
            }
        });
        //create thread for background process
        Thread th = new Thread(worker);
        th.setDaemon(true);
        //run background process
        th.start();
    }
    /**
     * Shows dialog for loading XML file with saved tasks and adds tasks from this XML.
     */
    public void open(){
        logger.trace("Trying to load from XML file");
        try {
            //show open dialog
            String content = Utilities.loadDialogForXML(window, SAVES_DEFAULT_DIR);
            if(content == null) return;
            //parse XML
            parseAndLoadXML(content);
        } catch (DocumentException e) {
            Utilities.showException(logger, "Cannot parse loaded file", e, false, null);
        }
    }
    /**
     * Parses given XML document as string and creates simulation tasks.
     *
     * @param content XML document content containing tasks configuration
     * @throws DocumentException cannot parse read XML document
     */
    private void parseAndLoadXML(String content)throws DocumentException{
        logger.trace("Parsing XML file");
        Document doc = DocumentHelper.parseText(content);
        //root element = <tasks>
        Element rootElement = doc.getRootElement();
        for(Iterator<Element> it = rootElement.elementIterator("task"); it.hasNext();){
            Element taskElement = it.next();
            logger.trace("XML -> Parsing new task");
            SimulationTask tmpTask = null;
            try{
                //-------------------------------- PARSE XML --------------------------
                logger.trace("XML task parse -> Parsing server info");
                Element serverElement = taskElement.element("server");
                String serverName = serverElement.attribute("name").getValue();
                String serverIp = serverElement.attribute("ip").getValue();
                Integer serverPort = Integer.valueOf(serverElement.attribute("port").getValue());
                String serverDb = serverElement.attribute("db").getValue();
                //parse parameters
                Element parametersElement = taskElement.element("parameters");
                int adaptersCount = Integer.valueOf(parametersElement.attributeValue("ada_count"));
                int start_id = Integer.valueOf(parametersElement.attributeValue("start_id"));
                double protocolVersion = Double.valueOf(parametersElement.attributeValue("protocol"));
                Element sensorsCountElement = parametersElement.element("sensors_count");
                long sensorsCountSeed = Long.valueOf(sensorsCountElement.attributeValue("seed"));
                int sensorsCountMin = Integer.valueOf(sensorsCountElement.attributeValue("min"));
                int sensorsCountMax = Integer.valueOf(sensorsCountElement.attributeValue("max"));
                Element refreshTimeElement = parametersElement.element("refresh_time");
                long refreshTimeSeed = Long.valueOf(refreshTimeElement.attributeValue("seed"));
                int refreshTimeMin = Integer.valueOf(refreshTimeElement.attributeValue("min"));
                int refreshTimeMax = Integer.valueOf(refreshTimeElement.attributeValue("max"));
                String saveDir = parametersElement.elementText("save_dir");
                String[] enabledValuesNames = parametersElement.elementText("enabled_values").split(",");
                long enabledValuesSeed = Long.valueOf(parametersElement.element("enabled_values").attributeValue("seed"));
                //stop conditions
                Element stopConditionsElement = taskElement.element("stop_conditions");
                //-- time duration
                Element timeDurationElement = stopConditionsElement.element("time_duration");
                boolean timeEnabled = Boolean.valueOf(timeDurationElement.attributeValue("enabled"));
                long timeInSec = 0;
                if(timeEnabled){
                    timeInSec = Long.valueOf(timeDurationElement.attributeValue("sec"));
                }
                //-- sent messages
                Element sentMessagesElement = stopConditionsElement.element("sent_messages");
                boolean sentEnabled = Boolean.valueOf(sentMessagesElement.attributeValue("enabled"));
                long sentMessagesCount = 0;
                if(sentEnabled){
                    sentMessagesCount = Long.valueOf(sentMessagesElement.attributeValue("count"));
                }
                //-- waiting messages
                Element waitingMessagesElement = stopConditionsElement.element("waiting_messages");
                boolean waitingEnabled = Boolean.valueOf(waitingMessagesElement.attributeValue("enabled"));
                long waitingMessagesCount = 0;
                if(waitingEnabled){
                    waitingMessagesCount = Long.valueOf(waitingMessagesElement.attributeValue("count"));
                }
                //---------------------------CREATE TASK --------------------------------------
                tmpTask = new SimulationTask();
                if(getTasks().size() == 0){
                    tmpTask.setId(0);
                }else{
                    tmpTask.setId(getTasks().get(getTasks().size()-1).getId()+1);
                }
                //create server
                logger.trace("Load task from XML -> creating server");
                tmpTask.createServer(false,serverName,serverIp,serverPort,serverDb);
                logger.trace("Load task from XML -> creating parameters");
                tmpTask.createTaskParameters(
                        adaptersCount,
                        ProtocolFactory.getVersion(protocolVersion),
                        start_id,
                        sensorsCountMin,
                        sensorsCountMax,
                        refreshTimeMin,
                        refreshTimeMax,
                        saveDir
                );
                tmpTask.setSensorCountGeneratorSeed(sensorsCountSeed);
                tmpTask.setRefreshTimeGeneratorSeed(refreshTimeSeed);
                tmpTask.setValueTypeGeneratorSeed(enabledValuesSeed);
                //register tracking responses per second
                //tmpTask.getResponseTracker().registerSecondCounter(tmpTask.getTaskParameters().getStopWatch().timeProperty());
                //value parameters
                ObservableList<Value> tmpValues = FXCollections.observableArrayList();
                for(String s : enabledValuesNames){
                    tmpValues.add(ValueFactory.buildValue(s));
                }
                ValueParameters valueParameters = new ValueParameters();
                valueParameters.setEnabledValues(tmpValues);
                tmpTask.setValueParameters(valueParameters);
                //create logs
                logger.trace("Load task from XML -> Creating logs");
                tmpTask.createLog(getView().getLogTabPane());
                tmpTask.getLog().createLogs();
                //set message tracking (waiting and sent counter)
                logger.trace("Load task from XML -> Creating logs -> Enabling response tracking");
                tmpTask.getLog().getMessageTracker().setEnabled(true);
                //DON'T show to be sent messages
                tmpTask.getLog().setShowToBeSent(false);
                //set log to buffer
                logger.trace("Load task from XML -> Setting logs to buffer");
                try {
                    tmpTask.getLog().setBuffered(true,"task_"+tmpTask.getId()+"_",tmpTask.getTaskParameters().getSaveDir());
                    tmpTask.getLog().writeTaskLogHeaderToBuffer(tmpTask);
                } catch (IOException e) {
                    Utilities.showException(logger, "Cannot create buffer file for new task's log.", e, true, event -> quit());
                }
                //register stop conditions
                logger.trace("Load task from XML -> Creating stop conditions");
                StopCondition sc = tmpTask.createStopCondition();
                if(timeEnabled){
                    sc.registerTimeDurationWatcher(timeInSec);
                }
                if(sentEnabled){
                    sc.registerSentMessageWatcher(sentMessagesCount);
                }
                if(waitingEnabled){
                    sc.registerWaitingMessageWatcher(waitingMessagesCount);
                }
                //set task's state
                tmpTask.setSimulationState(SimulationTask.State.READY);
                //add QueueWatcher to task
                tmpTask.setQueueWatcher(getQueueWatcher());
                logger.trace("Load task from XML -> Adding task to list");
                tmpTask.setSaved(true);
                getTasks().add(tmpTask);
            }catch (NullPointerException e){
                throw new DocumentException("Wrong format of file. Cannot find one or more required elements. Error on in content of tasks ",e);
            }
        }
    }

    /**
     * Starts {@link com.iha.emulator.utilities.watchers.QueueWatcher}, but only if every simulation task has at least
     * one stop condition.
     */
    public void queueProcessStart(){
        //check if queue watcher exists
        if(getQueueWatcher() == null){
            logger.warn("QueueWatcher is null");
            return;
        }
        //check if every task has at least one stop condition
        if(getTasks().size() == 0 || checkMissingStopConditions()) return;
        try{
            //start processing
            getQueueWatcher().start();
        }catch (IllegalArgumentException e){
            logger.debug(e.getMessage());
            //notify user about error
            Utilities.showError("Choose time to start queue auto processing","Auto process queue");
        }
    }

    /**
     * Stops {@link com.iha.emulator.utilities.watchers.QueueWatcher}'s automatic simulation tasks processing
     */
    public void queueProcessStop(){
        if(getQueueWatcher() == null){
            logger.warn("QueueWatcher is null");
            return;
        }
        //stop
        getQueueWatcher().stop();
    }

    /**
     * Checks, if every task has at least one stop condition. If not, tasks with no condition are shown in invoked
     * error dialog.
     * @return <code>true</code> there are tasks missing conditions, <code>false</code> otherwise
     */
    private boolean checkMissingStopConditions(){
        String taskIds = "";
        //check all tasks
        for (SimulationTask t : getTasks()){
            if(t.getStopCondition().getConditionCounter() == 0){
                taskIds = (taskIds.isEmpty() ? "" : " , ") + t.getId();
            }
        }
        //if anything found
        if(!taskIds.isEmpty()){
            //notify user
            Utilities.showError("Tasks " + taskIds + " don't have stop condition. Cannot run queue auto processing.","Auto queue process");
            return true;
        }else{
            return false;
        }
    }

    /**
     * Selects given simulation task as current and displays it's information to user interface. If previous task is still
     * running, it is stopped. If given simulation task is null, control buttons in navigation are disabled.
     * @param newTask task to be set as current
     */
    public void switchCurrentTask(SimulationTask newTask){
        Task<Object> worker = new Task<Object>() {
            @Override
            protected Object call() throws Exception {
                //have previous task and new task
                if(getCurrentTask() != null && newTask != null){
                    //are previous task and new the same?
                    if(newTask.equals(getCurrentTask())) return null;
                    //if previous is running, stop it
                    else if(getCurrentTask().getRunning()) getCurrentTask().stop();
                }
                //if new task is null, disable control buttons in navigation
                if(newTask == null){
                    Platform.runLater(PerformanceSimulationPresenter.this::clearControlsBinding);
                    return null;
                }
                updateProgress(10,100);
                Platform.runLater(()->{
                    setStatus("Setting current task ->" + newTask.getId(),true);
                    //save new task as current
                    setCurrentTask(newTask);
                    //show task's server information
                    updateProgress(20,100);
                    serverDetailsPresenter.addModel(newTask.getServerController().getModel());
                    serverDetailsPresenter.addSenderProperty(null);
                    updateProgress(30,100);
                    Platform.runLater(()->taskDetailsPresenter.addModel(newTask));
                    updateProgress(40,100);
                    newTask.getResponseTracker().registerChart(responseChart);
                    updateProgress(80,100);
                    //bind control buttons to new task
                    bindTaskControlBtns();
                    //bind log areas to new task
                    bindLogs();
                    updateProgress(90,100);
                    //select current task in table
                    selectTaskInTable(newTask);
                    setStatus("Task/" + newTask.getId() + " set as current",false);
                });
                return null;
            }

            @Override
            protected void failed() {
                super.failed();
                Utilities.showException(logger, "Switching current task failed", getException(), false, null);
            }
        };
        //create thread for background process
        Thread th = new Thread(worker);
        th.setDaemon(true);
        //run background process
        th.start();
    }

    /**
     * Shows currently selected task's buffer file in default application used by operating system.
     */
    public void showFullLog(){
        if(hostServices != null && getCurrentTask()!= null && getCurrentTask().getLog().getBufferFile()!= null){
            hostServices.showDocument(getCurrentTask().getLog().getBufferFile().getAbsolutePath());
        }
    }
    /**
     * Initializes window. Fills components with data, defines application onClose actions and initiates details panels.
     * Also starts memory usage checking. Also creates time picker for automatic queue processing.
     *
     * @param properties properties loaded from configuration file
     */
    public void init(Properties properties){
        logger.info("Initialization");
        //application initialisation
        startMemoryCheck();
        //load properties from .properties file
        this.properties = properties;
        //point logger to textArea
        TextAreaAppender.setTextFlow(view.getApplicationLogTextArea());
        //init presenters and save their views to GUI
        initPresentersAndViews();
        //bind task controls disable property
        bindControlBtnsToTasksCount();
        //create date time picker
        createDateTimePicker();
        //create and bind queue watcher
        this.queueWatcher = new QueueWatcher(this,getTasks());
        bindQueueWatcherControls();
        //init response chart
        this.responseChart = new ResponseChart(view.getResponseChartContainer());
        //init task table, set columns cell factories
        initTasksTable();
        view.getNewTaskItem().setDisable(false);
        view.getNewTaskTBtn().setDisable(false);
        view.getOpenTBtn().setDisable(false);
        view.getOpenItem().setDisable(false);
        //on quit
        window.setOnCloseRequest(event -> {
            checkIfSaved();
            if(getUnsavedTasks() == null){
                quit();
            }else {
                event.consume();
            }
        });
        Runtime.getRuntime().addShutdownHook(new Thread(){
            public void run(){
                quit();
                logger.info("Shutting down");
            }
        });
    }

    /**
     * Creates {@link jfxtras.scene.control.LocalDateTimeTextField} with time used to start queue processing.
     */
    private void createDateTimePicker(){
        LocalDateTimeTextField l = new LocalDateTimeTextField();
        l.setPrefHeight(27);
        l.setPrefWidth(189);
        view.setStartDateTimeTextField(l);
        view.getQueueProcessContainer().getChildren().add(l);
    }
    /**
     * Binds application log areas to currently selected task
     */
    private void bindLogs(){
        if(getCurrentTask()!= null && getCurrentTask().getLog() != null){
            AdapterLogger log = getCurrentTask().getLog();
            log.clearContainers();
            log.addAdapterLogTo(view.getAdapterLogContainer());
            log.addSentLogTo(view.getToBeSentLogContainer());
            log.addErrorLogTo(view.getErrorLogContainer());
        }
    }

    /**
     * Binds {@link com.iha.emulator.utilities.watchers.QueueWatcher}'s variables to interface's relevant fields.
     */
    private void bindQueueWatcherControls(){
        if(getQueueWatcher() == null){
            logger.warn("QueueWatcher is null");
            return;
        }
        getQueueWatcher().initDateTimePicker(view.getStartDateTimeTextField());
        view.getQueueProcessOnBtn().disableProperty().bind(getQueueWatcher().enabledProperty());
        view.getQueueProcessOffBtn().disableProperty().bind(new BooleanBinding() {
            {
                bind(getQueueWatcher().enabledProperty());
            }
            @Override
            protected boolean computeValue() {
                return !getQueueWatcher().getEnabled();
            }
        });
        view.getImmediatelyStartRadBtn().selectedProperty().addListener(new ChangeListener<Boolean>() {
            @Override
            public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
                getQueueWatcher().setImmediateStart(newValue);
            }
        });
        view.getStartDateTimeTextField().disableProperty().bind(new BooleanBinding() {
            {
                bind(getQueueWatcher().enabledProperty());
            }
            @Override
            protected boolean computeValue() {
                return getQueueWatcher().getEnabled();
            }
        }.or(getQueueWatcher().immediateStartProperty()));
        view.getImmediatelyStartRadBtn().disableProperty().bind(getQueueWatcher().enabledProperty());
        view.getPickTimeStartRadBtn().disableProperty().bind(getQueueWatcher().enabledProperty());
    }

    /**
     * Initiates simulation task's table. Adds custom display for each table column. Also adds listener for double-click
     * which is used to change current task.
     */
    @SuppressWarnings("unchecked")
    private void initTasksTable(){
        //checkbox column
        view.getEnabledColumn().setCellValueFactory(new SimulationTaskFactory());
        view.getEnabledColumn().setCellFactory(param -> new SimulationTaskEnabledCellFactory());
        //id column
        view.getIdColumn().setCellValueFactory(new SimulationTaskFactory());
        view.getIdColumn().setCellFactory(param -> new SimulationTaskIdCellFactory());
        //state column
        view.getStateColumn().setCellValueFactory(new SimulationTaskFactory());
        view.getStateColumn().setCellFactory(param -> new SimulationTaskStateCellFactory());
        //server column
        view.getServerColumn().setCellValueFactory(new SimulationTaskFactory());
        view.getServerColumn().setCellFactory(param -> new SimulationTaskServerCellFactory());
        //stop condition column
        view.getStopConditionColumn().setCellValueFactory(new SimulationTaskFactory());
        view.getStopConditionColumn().setCellFactory(param -> new SimulationTaskStopCellFactory());
        //set content for tasks table
        view.getTasksTable().setItems(getTasksList());
        //set click action
        view.getTasksTable().setRowFactory(tv->{
            TableRow<SimulationTask> row = new TableRow<>();
            row.setOnMouseClicked(event -> {
                if((!row.isEmpty()) && event.getClickCount() > 1 && !getQueueWatcher().getEnabled()){
                    SimulationTask task = row.getItem();
                    checkIfSwitchTasks(task);
                }
            });
            return row;
        });
    }

    /**
     * Checks if given task can be set as current. New task can be selected only if current task is null or current tusk is
     * not running. If there is a problem, notifies user with confirmation dialog.
     * @param newTask tak to be selected
     */
    private void checkIfSwitchTasks(SimulationTask newTask){
        logger.trace("Trying to switch tasks");
        if(newTask != null){
            //check if current exists or if it is not running
            if(getCurrentTask() == null || !getCurrentTask().getRunning()){
                logger.trace("Trying to switch to task: " + newTask.getId());
                //switch to new task
                switchCurrentTask(newTask);
                return;
            }
            //if current new are the same, nothing happens
            if(newTask.equals(getCurrentTask())){
                logger.trace("Selected task is already current");
            }else {
                //notify user, let user choose, if current should be stopped and new selected or do nothing
                logger.trace("Showing switch conformation dialog");
                Alert dlg = new Alert(Alert.AlertType.CONFIRMATION,"");
                dlg.initModality(Modality.WINDOW_MODAL);
                dlg.initOwner(window);
                dlg.setTitle("Switch task");
                dlg.getDialogPane().setContentText("Current task is running. Do you wish to stop this task and switch to selected task?");
                dlg.showAndWait().ifPresent(result -> {
                    ButtonBar.ButtonData buttonData = result.getButtonData();
                    switch (buttonData){
                        case OK_DONE:
                            stopTask();
                            logger.trace("Trying to switch to task: " + newTask.getId());
                            switchCurrentTask(newTask);
                            break;
                        case CANCEL_CLOSE:
                            logger.trace("Cancel");
                            selectTaskInTable(getCurrentTask());
                            break;
                        }
                    });
            }
        }else {
            logger.error("Selected task is null");
        }
    }

    /**
     * Focuses simulation tasks table selection on given task.
     * @param task task to be focused on
     */
    private void selectTaskInTable(SimulationTask task){
        view.getTasksTable().requestFocus();
        logger.debug("Focusing table on task: " + task.getId());
        int index = view.getTasksTable().getItems().indexOf(task);
        view.getTasksTable().getSelectionModel().select(index);
        view.getTasksTable().getFocusModel().focus(index);
    }

    /**
     * Disable control buttons in navigation, sets current task to null.
     */
    private void clearControlsBinding(){
        if(view.getStartTaskItem().disableProperty().isBound()) view.getStartTaskItem().disableProperty().unbind();
        if(view.getStartTaskTBtn().disableProperty().isBound()) view.getStartTaskTBtn().disableProperty().unbind();

        if(view.getPauseTaskItem().disableProperty().isBound()) view.getPauseTaskItem().disableProperty().unbind();
        if(view.getPauseTaskTBtn().disableProperty().isBound()) view.getPauseTaskTBtn().disableProperty().unbind();

        if(view.getStopTaskItem().disableProperty().isBound()) view.getStopTaskItem().disableProperty().unbind();
        if(view.getStopTaskTBtn().disableProperty().isBound()) view.getStopTaskTBtn().disableProperty().unbind();

        if(view.getPauseSensorsItem().disableProperty().isBound()) view.getPauseSensorsItem().disableProperty().unbind();
        if(view.getPauseSensorsTBtn().disableProperty().isBound()) view.getPauseSensorsTBtn().disableProperty().unbind();

        if(view.getResumeSensorsItem().disableProperty().isBound()) view.getResumeSensorsItem().disableProperty().unbind();
        if(view.getResumeSensorsTBtn().disableProperty().isBound()) view.getResumeSensorsTBtn().disableProperty().unbind();

        view.getStartTaskItem().setDisable(true);
        view.getStartTaskTBtn().setDisable(true);

        view.getPauseTaskItem().setDisable(true);
        view.getPauseTaskTBtn().setDisable(true);

        view.getStopTaskItem().setDisable(true);
        view.getStopTaskTBtn().setDisable(true);

        view.getPauseSensorsItem().setDisable(true);
        view.getPauseSensorsTBtn().setDisable(true);

        view.getResumeSensorsItem().setDisable(true);
        view.getResumeSensorsTBtn().setDisable(true);

        setCurrentTask(null);
    }
    /**
     * Shows {@link com.iha.emulator.ui.dialogs.task.AddNewTaskDialogPresenter} in new window.
     * @param servers default server models list
     */
    private void showAddTaskDialog(ObservableList<Server> servers){
        AddNewTaskDialogPresenter addNewTaskDialogPresenter;
        try{
            //create window
            Stage stage = new Stage();
            //create presenter
            addNewTaskDialogPresenter = new AddNewTaskDialogPresenter(stage,this,servers);
            stage.setTitle("Add new task");
            //create and initiate scene
            Scene scene = new Scene((Parent) addNewTaskDialogPresenter.loadView());
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            stage.setScene(scene);
            stage.setResizable(false);
            //show window
            stage.show();
        } catch (IOException e) {
            Utilities.showException(logger, "Cannot load dialog for task adapter!", e, false, null);
        } catch (IllegalArgumentException ei){
            Utilities.showException(logger, "Cannot create task. Error in properties file. Please review file an start application again.", ei, true, event -> quit());
        }
    }
    /**
     * Binds task's control buttons in navigation to currently selected task.
     */
    private void bindTaskControlBtns(){
        if(getCurrentTask() == null) return;
        //start buttons
        BooleanBinding startBinding = new BooleanBinding() {
            {
                bind(getCurrentTask().simulationStateProperty());
            }
            @Override
            protected boolean computeValue() {
                switch (getCurrentTask().getSimulationState()){
                    case RUNNING:
                        return true;
                    default:
                        return false;
                }
            }
        };
        view.getStartTaskTBtn().disableProperty().bind(startBinding);
        view.getStartTaskItem().disableProperty().bind(startBinding);
        //pause buttons
        BooleanBinding pauseBinding = new BooleanBinding() {
            {
                bind(getCurrentTask().simulationStateProperty());
            }
            @Override
            protected boolean computeValue() {
                switch (getCurrentTask().getSimulationState()){
                    case RUNNING:
                        return false;
                    default:
                        return true;
                }
            }
        };
        view.getPauseTaskTBtn().disableProperty().bind(pauseBinding);
        view.getPauseTaskItem().disableProperty().bind(pauseBinding);
        //start buttons
        BooleanBinding stopBinding = new BooleanBinding() {
            {
                bind(getCurrentTask().simulationStateProperty());
            }
            @Override
            protected boolean computeValue() {
                switch (getCurrentTask().getSimulationState()){
                    case RUNNING:
                    case PAUSED:
                        return false;
                    default:
                        return true;
                }
            }
        };
        view.getStopTaskTBtn().disableProperty().bind(stopBinding);
        view.getStartTaskItem().disableProperty().bind(stopBinding);
        //pause sensors buttons
        BooleanBinding pauseSensorsBinding = new BooleanBinding() {
            {
                bind(getCurrentTask().sensorsPausedProperty());
            }
            @Override
            protected boolean computeValue() {
                return getCurrentTask().getSensorsPaused();
            }
        }.or(new BooleanBinding() {
            {
                bind(getCurrentTask().simulationStateProperty());
            }

            @Override
            protected boolean computeValue() {
                switch (getCurrentTask().getSimulationState()) {
                    case RUNNING:
                    case PAUSED:
                        return false;
                    default:
                        return true;
                }
            }
        });
        view.getPauseSensorsTBtn().disableProperty().bind(pauseSensorsBinding);
        view.getPauseSensorsItem().disableProperty().bind(pauseSensorsBinding);
        //resume sensors buttons
        BooleanBinding resumeSensorsBinding = new BooleanBinding() {
            {
                bind(getCurrentTask().sensorsPausedProperty());
            }
            @Override
            protected boolean computeValue() {
                return !getCurrentTask().getSensorsPaused();
            }
        }.or(new BooleanBinding() {
            {
                bind(getCurrentTask().simulationStateProperty());
            }

            @Override
            protected boolean computeValue() {
                switch (getCurrentTask().getSimulationState()) {
                    case RUNNING:
                    case PAUSED:
                        return false;
                    default:
                        return true;
                }
            }
        });
        view.getResumeSensorsTBtn().disableProperty().bind(resumeSensorsBinding);
        view.getResumeSensorsItem().disableProperty().bind(resumeSensorsBinding);
    }
    /**
     * Binds navigation buttons disable state to number of created tasks. If no task exists, relevant buttons are
     * disabled.
     */
    private void bindControlBtnsToTasksCount(){
        BooleanBinding tasksListZeroReturnTrue = new BooleanBinding() {
            {
                bind(tasksListProperty().sizeProperty());
            }
            @Override
            protected boolean computeValue() {
                return getTasksList().size() <= 0;
            }
        }.or(new BooleanBinding() {
            {
                bind(currentTaskProperty());
            }
            @Override
            protected boolean computeValue() {
                return getCurrentTask() == null;
            }
        });
        BooleanBinding tasksListZero = new BooleanBinding() {
            {
                bind(tasksListProperty().sizeProperty());
            }
            @Override
            protected boolean computeValue() {
                return getTasksList().size() <= 0;
            }
        };
        view.getSaveAllBtn().disableProperty().bind(tasksListZero);
        //sim control buttons
        view.getStartTaskItem().disableProperty().bind(tasksListZeroReturnTrue);
        view.getStartTaskTBtn().disableProperty().bind(tasksListZeroReturnTrue);
        view.getStopTaskItem().disableProperty().bind(tasksListZeroReturnTrue);
        view.getStopTaskTBtn().disableProperty().bind(tasksListZeroReturnTrue);
        view.getPauseTaskItem().disableProperty().bind(tasksListZeroReturnTrue);
        view.getPauseTaskTBtn().disableProperty().bind(tasksListZeroReturnTrue);
        //sim sensors control
        view.getPauseSensorsItem().disableProperty().bind(tasksListZeroReturnTrue);
        view.getPauseSensorsTBtn().disableProperty().bind(tasksListZeroReturnTrue);
        view.getResumeSensorsItem().disableProperty().bind(tasksListZeroReturnTrue);
        view.getResumeSensorsTBtn().disableProperty().bind(tasksListZeroReturnTrue);
        //other
        view.getDeleteTaskTBtn().disableProperty().bind(tasksListZero);

    }
    /**
     * Shows information dialog with given message.
     * @param title dialog title
     * @param headerMessage dialog header message
     * @param message information message
     */
    private void showInformation(String title,String headerMessage,String message){
        logger.trace("Showing information");
        Alert dlg = new Alert(Alert.AlertType.INFORMATION, "");
        dlg.initModality(Modality.WINDOW_MODAL);
        dlg.initOwner(this.window);
        dlg.setTitle(title);
        dlg.getDialogPane().setContentText(message);
        dlg.getDialogPane().setHeaderText(headerMessage);
        dlg.show();
    }
    /**
     * Creates presenter for details panels and initiates their views.
     */
    private void initPresentersAndViews(){
        //load server details
        try {
            serverDetailsPresenter = new ServerDetailsPresenter();
            view.addServerDetailsView(serverDetailsPresenter.loadView());
        } catch (IOException e) {
            Utilities.showException(logger, "Cannot load Server Details", e, true, event -> quit());
        }
        try {
            taskDetailsPresenter = new TaskDetailsPresenter();
            view.addTaskDetailsView(taskDetailsPresenter.loadView());
        } catch (IOException e) {
            Utilities.showException(logger, "Cannot load Server Details", e, true, event -> quit());
        }
    }

    /**
     * Load view from FXML file{@link com.iha.emulator.ui.simulations.performance.PerformanceSimulationPresenter#FXML_PATH} and after that
     * bind the view with a presenter. Also assigns CSS file {@link com.iha.emulator.ui.simulations.performance.PerformanceSimulationPresenter#CSS_PATH}
     * and creates {@link javafx.scene.Scene}, which is returned.
     * @return scene created from loaded view
     * @throws IOException canot load FXML file
     */
    public Scene loadView() throws IOException{
        logger.trace("Loading PerformanceSimulationView from: " + FXML_PATH);
        InputStream fxmlStream = null;
        try {
            //load resource FXML
            fxmlStream = getClass().getResourceAsStream(FXML_PATH);
            FXMLLoader loader = new FXMLLoader();
            loader.load(fxmlStream);
            //remember view
            view = loader.getController();
            //build scene
            Scene scene = new Scene((Parent) view.getView());
            // bind view and presenter
            bind();
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            logger.trace("OK");
            return scene;
        }
        finally
        {
            if (fxmlStream != null) fxmlStream.close();
        }
    }
    /**
     * Shows given message in dialog. Also can start or stop loop indicator
     * @param status message to be shown
     * @param indicate <code>true</code> show indicator, <code>false</code> hide indicator
     */
    public void setStatus(String status,boolean indicate){
        view.setStatusLine(status);
        view.setStatusIndicator(indicate);
        logger.info(status);
    }
    /**
     * Starts memory usage checking.
     */
    public void startMemoryCheck(){
        if(view != null && memoryChecker != null){
            logger.trace("Starting memory check");
            //assign where to show memory usage
            memoryChecker.setGuiElements(view.getMemCheckProgressBar(),view.getMemCheckStatusLabel());
            //start checking
            memoryChecker.start();
        }
    }
    /**
     * Closes buffer files for all tasks in tasks list
     */
    private void dumpLogs(){
        if(getTasks() == null || getTasks().size() == 0) return;
        for(SimulationTask task : getTasks()){
            task.getLog().closeBuffer();
        }
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public void bind() {
        view.setPresenter(this);
    }
    /**
     * {@inheritDoc}
     */
    public Display getView(){
        return this.view;
    }
    /**
     * Gets list of tasks, that have been modified but not saved to XML file
     * @return list of unsaved tasks
     */
    private ObservableList<SimulationTask> getUnsavedTasks(){
        ArrayList<SimulationTask> unsavedTasks = null;
        if(getTasks().size() > 0){
            unsavedTasks =
                    getTasks()
                            .stream()
                            .filter(a -> !a.isSaved())
                            .collect(Collectors.toCollection(ArrayList<SimulationTask>::new));
        }
        if(unsavedTasks == null || unsavedTasks.size() == 0){
            return null;
        }
        else{
            logger.trace("Unsaved adapters count: " + unsavedTasks.size());
            return FXCollections.observableArrayList(unsavedTasks);
        }
    }
    /**
     * Shows {@link javafx.scene.control.ChoiceDialog} with {@link com.iha.emulator.utilities.Utilities.SaveTasksOption}
     * actions, that should be made on list of unsaved adapters.
     */
    private void checkIfSaved(){
        logger.debug("Checking unsaved adapters");
        if(getTasks().size() > 0){
            ObservableList<SimulationTask> unsavedTasks = getUnsavedTasks();
            if(unsavedTasks != null){
                ChoiceDialog<Utilities.SaveTasksOption> dlg = Utilities.saveTasksOnQuitDialog();
                dlg.showAndWait().ifPresent(result -> {
                    switch (result){
                        case SAVE_ALL:
                            saveAll(null);
                            break;
                        case DO_NOTHING:
                            unsavedTasks.forEach(t -> t.setSaved(true));
                            break;
                    }
                });
            }
        }
    }
    /**
     * Stops memory usage checking and closes all tasks' buffer files. Afterwards closes application.
     */
    public void quit(){
        Platform.runLater(memoryChecker::stop);
        dumpLogs();
        Platform.exit();
    }

    /**
     * Gets currently selected task
     * @return currently selected task
     */
    public SimulationTask getCurrentTask() {
        return currentTask.get();
    }

    /**
     * Currently selected task property, that can be bound
     * @return currently selected task property
     */
    public ObjectProperty<SimulationTask> currentTaskProperty() {
        return currentTask;
    }

    /**
     * Sets currently selected task
     * @param currentTask currently selected task
     */
    public void setCurrentTask(SimulationTask currentTask) {
        this.currentTask.set(currentTask);
    }

    /**
     * Gets list of created tasks
     * @return list of created tasks
     */
    public ObservableList<SimulationTask> getTasksList() {
        return tasksList.get();
    }

    /**
     * List of created tasks property, that can be bound
     * @return list of created tasks property
     */
    public ListProperty<SimulationTask> tasksListProperty() {
        return tasksList;
    }
    /**
     * Gets list of created tasks
     * @return list of created tasks
     */
    public ObservableList<SimulationTask> getTasks() {
        return tasksList.get();
    }

    /**
     * Gets instance of queue watcher
     * @return instance of queue watcher
     */
    public QueueWatcher getQueueWatcher() {
        return queueWatcher;
    }

    /**
     * Gets host services
     * @return host services
     */
    public HostServices getHostServices() {
        return hostServices;
    }

    /**
     * Sets host services
     * @param hostServices host services
     */
    public void setHostServices(HostServices hostServices) {
        this.hostServices = hostServices;
    }

    //endregion

}
