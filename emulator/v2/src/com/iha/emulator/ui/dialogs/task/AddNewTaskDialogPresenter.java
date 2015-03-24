package com.iha.emulator.ui.dialogs.task;

import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.communication.protocol.ProtocolFactory;
import com.iha.emulator.communication.server.ServerController;
import com.iha.emulator.control.task.SimulationTask;
import com.iha.emulator.control.task.StopCondition;
import com.iha.emulator.control.task.TaskParameters;
import com.iha.emulator.control.task.ValueParameters;
import com.iha.emulator.models.Server;
import com.iha.emulator.models.value.Value;
import com.iha.emulator.models.value.ValueFactory;
import com.iha.emulator.models.value.implemented.EmptyValue;
import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.dialogs.adapter.ShowAdaptersDialogPresenter;
import com.iha.emulator.ui.panels.PanelPresenter;
import com.iha.emulator.ui.simulations.performance.PerformanceSimulationPresenter;
import com.iha.emulator.utilities.Utilities;
import javafx.beans.binding.BooleanBinding;
import javafx.beans.binding.StringBinding;
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
import javafx.stage.DirectoryChooser;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.util.Callback;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.controlsfx.control.RangeSlider;
import org.controlsfx.validation.ValidationMessage;
import org.controlsfx.validation.ValidationResult;
import org.controlsfx.validation.ValidationSupport;
import org.controlsfx.validation.Validator;
import org.controlsfx.validation.decoration.StyleClassValidationDecoration;
import org.controlsfx.validation.decoration.ValidationDecoration;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.text.DecimalFormat;
import java.util.Iterator;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import static com.iha.emulator.models.value.Value.Type;

/**
 * Created by Shu on 10.12.2014.
 */
public class AddNewTaskDialogPresenter implements Presenter,PanelPresenter{

    private static final Logger logger = LogManager.getLogger(AddNewTaskDialogPresenter.class);
    private static final String FXML_PATH = "AddNewTaskDialog.fxml";
    private static final String CSS_PATH = "/com/iha/emulator/resources/css/theme-light.css";
    private static final String TEMPLATES_DEFAULT_DIR = "templates/tasks";
    private static final double REFRESH_SLIDER_VALUE_COUNT = 16;
    private static final ObservableList<Integer> HOURS = FXCollections.observableArrayList(IntStream.range(0, 24).boxed().collect(Collectors.toList()));
    private static final ObservableList<Integer> MINUTES_SECONDS = FXCollections.observableArrayList(IntStream.range(0, 60).boxed().collect(Collectors.toList()));
    private Display view;
    private Stage window;
    private PerformanceSimulationPresenter simulation;

    private final DecimalFormat sensorCountSliderFormatter = new DecimalFormat( "##0" );
    private ListProperty<Value> values = new SimpleListProperty<>(FXCollections.observableArrayList());
    private ObservableList<Value.Type> enabledValuesTypes = FXCollections.observableArrayList();
    //server
    private ValidationSupport serverValidationSupport = new ValidationSupport();
    private BooleanProperty serverInfoSet;
    private BooleanProperty modifyServer;
    private ObservableList<Server> servers;
    private Server selectedServer;
    //stop conditions
    private ValidationSupport stopConditionsSupport = new ValidationSupport();
    private BooleanProperty stopConditionsSet;
    //task info
    private ValidationSupport taskInfoSupport = new ValidationSupport();
    private BooleanProperty taskInfoSet;
    private Protocol.Version selectedVersion;


    public interface Display{
        public Node getView();
        public void setPresenter(AddNewTaskDialogPresenter presenter);
        public TreeView getValuesTree();
        //server
        public ComboBox getServerComboBox();
        public TextField getServerNameTxtField();
        public TextField getServerIpTxtField();
        public TextField getServerPortTxtField();
        public TextField getServerDbNameTxtField();
        public CheckBox getServerModifyCheckBox();
        //stop conditions
        public RadioButton getTimeDurationYesRadBtn();
        public RadioButton getTimeDurationNoRadBtn();
        public RadioButton getSentMessagesYesRadBtn();
        public RadioButton getSentMessagesNoRadBtn();
        public RadioButton getWaitingMessagesYesRadBtn();
        public RadioButton getWaitingMessagesNoRadBtn();
        public ComboBox getTimeHoursComboBox();
        public ComboBox getTimeMinutesComboBox();
        public ComboBox getTimeSecondsComboBox();
        public TextField getSentMessagesCountTxtField();
        public TextField getWaitingMessagesCountTxtField();
        //task
        public RangeSlider getSensorsCountSlider();
        public RangeSlider getRefreshTimeCountSlider();
        public void setRefreshTimeCountSlider(RangeSlider refreshTimeCountSlider);
        public void setSensorsCountSlider(RangeSlider sensorsCountSlider);
        public Label getSensorCountMinLbl();
        public Label getSensorCountMaxLbl();
        public Label getRefreshTimeMinLbl();
        public Label getRefreshTimeMaxLbl();
        public ComboBox getProtocolComboBox();
        public TextField getSaveDirTextField();
        public TextField getAdaptersCountTxtField();
        public TextField getStartIdTxtField();
    }

    public AddNewTaskDialogPresenter(Stage window,PerformanceSimulationPresenter simulation,ObservableList<Server> servers) {
        this.window = window;
        this.serverInfoSet = new SimpleBooleanProperty(false);
        this.taskInfoSet = new SimpleBooleanProperty(false);
        this.modifyServer = new SimpleBooleanProperty(false);
        this.stopConditionsSet = new SimpleBooleanProperty(true);
        ValidationDecoration cssDecorator = new StyleClassValidationDecoration("validationError", "validationWarn");
        serverValidationSupport.setValidationDecorator(cssDecorator);
        stopConditionsSupport.setValidationDecorator(cssDecorator);
        taskInfoSupport.setValidationDecorator(cssDecorator);
        this.window = window;
        this.servers = servers;
        this.simulation = simulation;
    }

    public void loadTemplate() {
        logger.trace("Trying to load from XML file");
        try {
            String content = Utilities.loadDialogForXML(window, TEMPLATES_DEFAULT_DIR);
            if(content == null) return;
            parseAndShowXml(content);
        } catch (DocumentException | NullPointerException e) {
            Utilities.showException(logger, "Cannot parse loaded file", e, false, null);
        }
    }

    @SuppressWarnings("unchecked")
    private void parseAndShowXml(String content) throws DocumentException,NullPointerException{
        logger.trace("Parsing XML file");
        //---------------------------PARSE XML DOCUMENT--------------------------------
        Document doc = DocumentHelper.parseText(content);
        //<task>
        Element taskElement = doc.getRootElement().element("task");
        //parse server info
        logger.trace("XML -> Parsing server info");
        Element serverElement = taskElement.element("server");
        String serverName = serverElement.attribute("name").getValue();
        String serverIp = serverElement.attribute("ip").getValue();
        Integer serverPort = Integer.valueOf(serverElement.attribute("port").getValue());
        String serverDb = serverElement.attribute("db").getValue();
        //parse parameters
        Element parametersElement = taskElement.element("parameters");
        String adaptersCount = parametersElement.attributeValue("ada_count");
        String start_id = parametersElement.attributeValue("start_id");
        double protocolVersion = Double.valueOf(parametersElement.attributeValue("protocol"));
        Element sensorsCountElement = parametersElement.element("sensors_count");
        double sensorsCountMin = Double.valueOf(sensorsCountElement.attributeValue("min"));
        double sensorsCountMax = Double.valueOf(sensorsCountElement.attributeValue("max"));
        Element refreshTimeElement = parametersElement.element("refresh_time");
        int refreshTimeMin = Integer.valueOf(refreshTimeElement.attributeValue("min"));
        int refreshTimeMax = Integer.valueOf(refreshTimeElement.attributeValue("max"));
        String saveDir = parametersElement.elementText("save_dir");
        String[] enabledValuesNames = parametersElement.elementText("enabled_values").split(",");
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
        String sentMessagesCount = "";
        if(sentEnabled){
            sentMessagesCount = sentMessagesElement.attributeValue("count");
        }
        //-- waiting messages
        Element waitingMessagesElement = stopConditionsElement.element("waiting_messages");
        boolean waitingEnabled = Boolean.valueOf(waitingMessagesElement.attributeValue("enabled"));
        String waitingMessagesCount = "";
        if(waitingEnabled){
            waitingMessagesCount = waitingMessagesElement.attributeValue("count");
        }
        //---------------------------SETGUI ELEMENTS--------------------------------
        //general
        view.getAdaptersCountTxtField().setText(adaptersCount);
        view.getStartIdTxtField().setText(start_id);
        view.getProtocolComboBox().getSelectionModel().select(ProtocolFactory.getVersion(protocolVersion));
        view.getSaveDirTextField().setText(saveDir);
        //server
        Server tmpServer = new Server(false,serverName,serverIp,serverPort,serverDb);
        boolean found = false;
        for(Server s : servers){
            if(s.equals(tmpServer)){
                view.getServerComboBox().getSelectionModel().select(tmpServer);
                view.getServerNameTxtField().setText(tmpServer.getName());
                view.getServerIpTxtField().setText(tmpServer.getIp());
                view.getServerPortTxtField().setText(String.valueOf(tmpServer.getPort()));
                view.getServerDbNameTxtField().setText(tmpServer.getDatabaseName());
                setServerInfoSet(true);
                found = true;
            }
        }
        if(!found){
            servers.add(tmpServer);
            setSelectedServer(tmpServer);
            view.getServerComboBox().getSelectionModel().select(tmpServer);
        }
        //sensor count
        view.getSensorsCountSlider().setLowValue(sensorsCountMin);
        view.getSensorsCountSlider().setHighValue(sensorsCountMax);
        //refresh time
        view.getRefreshTimeCountSlider().setLowValue(Utilities.secondsToRefershSliderScale(refreshTimeMin));
        view.getRefreshTimeCountSlider().setHighValue(Utilities.secondsToRefershSliderScale(refreshTimeMax));
        //enabled values
        for(Iterator it = values.iterator();it.hasNext();){
            Value v = (Value) it.next();
            deleteValue(v,false);
        }
        values.clear();
        ObservableList<Value> tmpValues = FXCollections.observableArrayList();
        for(String s : enabledValuesNames){
            tmpValues.add(ValueFactory.buildValue(s));
        }
        tmpValues.forEach(this::addValueToTree);
        //stop conditions
        if(timeEnabled) {
            view.getTimeDurationYesRadBtn().setSelected(true);
            setTimeDurationFields(timeInSec);
        }else{
            view.getTimeDurationNoRadBtn().setSelected(true);
        }
        if(sentEnabled) {
            view.getSentMessagesYesRadBtn().setSelected(true);
            view.getSentMessagesCountTxtField().setText(sentMessagesCount);
        }else{
            view.getSentMessagesNoRadBtn().setSelected(true);
        }
        if(waitingEnabled){
            view.getWaitingMessagesYesRadBtn().setSelected(true);
            view.getWaitingMessagesCountTxtField().setText(waitingMessagesCount);
        }else{
            view.getWaitingMessagesNoRadBtn().setSelected(true);
        }

    }

    public void saveTemplate() {
        if(!isAllSet()) return;
        String filename = Utilities.saveDialogForXML(window,TEMPLATES_DEFAULT_DIR,buildTaskXML().asXML());
        if(filename != null) showInformation("File saved", "Sensor template successfully saved", "Saved to file \"" + filename + "\"");

    }

    private Document buildTaskXML(){
        Document doc = DocumentHelper.createDocument();
        //root element
        Element taskElement = doc.addElement("tasks").addElement("task");
        //server element
        ServerController serverController = new ServerController(selectedServer);
        serverController.saveToXml(taskElement);
        //parameters element
        //parameters element
        Element parametersElement = taskElement.addElement("parameters")
                .addAttribute("ada_count", view.getAdaptersCountTxtField().getText())
                .addAttribute("protocol",String.valueOf(((Protocol.Version) view.getProtocolComboBox().getValue()).getVersion()))
                .addAttribute("start_id",view.getStartIdTxtField().getText());
        //sensors count element
        parametersElement.addElement("sensors_count")
                .addAttribute("min",String.valueOf(Double.valueOf(view.getSensorsCountSlider().getLowValue()).intValue()))
                .addAttribute("max",String.valueOf(Double.valueOf(view.getSensorsCountSlider().getHighValue()).intValue()));
        //refresh time element
        parametersElement.addElement("refresh_time")
                .addAttribute("min",String.valueOf(Utilities.refreshSliderScaleToSeconds(view.getRefreshTimeCountSlider().getLowValue())))
                .addAttribute("max",String.valueOf(Utilities.refreshSliderScaleToSeconds(view.getRefreshTimeCountSlider().getHighValue())));
        //save dir element
        parametersElement.addElement("save_dir").addText(view.getSaveDirTextField().getText());
        //enabled values element
        enabledValuesToXml(parametersElement);
        //stop conditions
        Element stopConditionElement = taskElement.addElement("stop_conditions");
        //time duration
        stopConditionElement.addElement("time_duration")
                .addAttribute("enabled",String.valueOf(view.getTimeDurationYesRadBtn().isSelected()))
                .addAttribute("sec", String.valueOf(determineTimeDuration()));
        //sent messages
        stopConditionElement.addElement("sent_messages")
                .addAttribute("enabled",String.valueOf(view.getSentMessagesYesRadBtn().isSelected()))
                .addAttribute("count",view.getSentMessagesCountTxtField().getText());
        //sent messages
        stopConditionElement.addElement("waiting_messages")
                .addAttribute("enabled",String.valueOf(view.getWaitingMessagesYesRadBtn().isSelected()))
                .addAttribute("count",view.getWaitingMessagesCountTxtField().getText());
        return doc;
    }

    private Element enabledValuesToXml(Element rootElement){
        Element enabledElement = rootElement.addElement("enabled_values");
        String valueString = "";
        for(Value v : values){
            valueString = valueString + (valueString.equals("")?"":",") + v.getValueType().getName();
        }
        enabledElement.setText(valueString);
        return enabledElement;
    }

    private boolean isAllSet() {
        return checkServerInfo() && checkStopConditions() && checkTaskInfo();
    }

    public void add() {
        if(!isAllSet()) return;
        logger.info("Trying to add new task");
        Task<SimulationTask> worker = new Task<SimulationTask>() {
            @Override
            protected SimulationTask call() throws Exception {
                LongProperty progress = new SimpleLongProperty(0);
                progress.addListener(new ChangeListener<Number>() {
                    @Override
                    public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
                        if((Long)newValue > 0 && (Long)newValue < 100)
                            updateProgress((long)newValue,100);
                    }
                });
                progress.set(1);
                SimulationTask task = new SimulationTask();
                //set id
                if(simulation.getTasks().size() == 0){
                    task.setId(0);
                }else{
                    task.setId(simulation.getTasks().get(simulation.getTasks().size()-1).getId()+1);
                }
                //create server model for task
                logger.trace("Creating new task -> Creating server");
                task.createServer(selectedServer);
                progress.set(10);
                //create task parameters
                logger.trace("Creating new task -> Creating parameters");
                task.createTaskParameters(
                        Integer.valueOf(view.getAdaptersCountTxtField().getText()), //adapters count
                        (Protocol.Version) view.getProtocolComboBox().getValue(), //protocol version
                        Integer.valueOf(view.getStartIdTxtField().getText()), //starting adapter id
                        Double.valueOf(view.getSensorsCountSlider().getLowValue()).intValue(), // sensors count minimum
                        Double.valueOf(view.getSensorsCountSlider().getHighValue()).intValue(), //sensors count maximum
                        Utilities.refreshSliderScaleToSeconds(view.getRefreshTimeCountSlider().getLowValue()), //refresh time minimum
                        Utilities.refreshSliderScaleToSeconds(view.getRefreshTimeCountSlider().getHighValue()), //refresh time maximum
                        view.getSaveDirTextField().getText() // default save dir
                );
                //register tracking responses per second
                task.getResponseTracker().registerSecondCounter(task.getTaskParameters().getStopWatch().timeProperty());
                //update progress
                progress.set(20);
                ValueParameters valueParameters = new ValueParameters();
                valueParameters.setEnabledValues(values);
                task.setValueParameters(valueParameters);
                //create log
                logger.trace("Creating new task -> Creating logs");
                task.createLog(simulation.getView().getLogTabPane());
                task.getLog().createLogs();
                progress.set(30);
                //set message tracking (waiting and sent counter)
                logger.trace("Creating new task -> Creating logs -> Enabling response tracking");
                task.getLog().getMessageTracker().setEnabled(true);
                //DON'T show to be sent messages
                task.getLog().setShowToBeSent(false);
                progress.set(40);
                //set log to buffer
                logger.trace("Creating new task -> Setting logs to buffer");
                try {
                    task.getLog().setBuffered(true,"task_"+task.getId()+"_",task.getTaskParameters().getSaveDir());
                    task.getLog().writeTaskLogHeaderToBuffer(task);
                } catch (IOException e) {
                    Utilities.showException(logger, "Cannot create buffer file for new task's log.", e, true, event -> simulation.quit());
                }
                progress.set(50);
                //register stop conditions
                logger.trace("Creating new task -> Creating stop conditions");
                StopCondition sc = task.createStopCondition();
                if(view.getTimeDurationYesRadBtn().isSelected()){
                    sc.registerTimeDurationWatcher(determineTimeDuration());
                }
                if(view.getSentMessagesYesRadBtn().isSelected()){
                    sc.registerSentMessageWatcher(Long.valueOf(view.getSentMessagesCountTxtField().getText()));
                }
                if(view.getWaitingMessagesYesRadBtn().isSelected()){
                    sc.registerWaitingMessageWatcher(Long.valueOf(view.getWaitingMessagesCountTxtField().getText()));
                }
                progress.set(60);
                //initialize adapters
                task.setSimulationState(SimulationTask.State.READY);
                progress.set(70);
                //add QueueWatcher to task
                task.setQueueWatcher(simulation.getQueueWatcher());
                progress.set(80);
                //add to other tasks
                logger.trace("Creating new task -> Adding task to list");
                progress.set(100);
                task.setSaved(false);
                return task;
            }
        };
        Utilities.showLoadingDialog(worker,"Creating task...");
        worker.setOnSucceeded(e ->{
            simulation.getTasks().add(worker.getValue());
            window.hide();
        });
        worker.setOnFailed(e -> {
            System.gc();
            Utilities.showException(logger,"Task creation failed. Probably too much devices is being created",worker.getException(),true,ea -> simulation.quit());
        });
        logger.info("New task created");
    }

    private long determineTimeDuration(){
        long seconds = 0;
        if(view.getTimeHoursComboBox().getValue() != null){
            seconds = seconds + ((Integer)view.getTimeHoursComboBox().getValue()*3600);
            logger.trace("Time duration seconds after hours: " + seconds);
        }
        if(view.getTimeMinutesComboBox().getValue() != null){
            seconds = seconds + ((Integer)view.getTimeMinutesComboBox().getValue()*60);
            logger.trace("Time duration seconds after minutes: " + seconds);
        }
        if(view.getTimeSecondsComboBox().getValue() != null){
            seconds = seconds + (Integer)view.getTimeSecondsComboBox().getValue();
            logger.trace("Time duration seconds after seconds: " + seconds);
        }
        return seconds;
    }

    private void setTimeDurationFields(long seconds){
        if(seconds < 60) {
            view.getTimeSecondsComboBox().getSelectionModel().select((int)seconds);
        }else if(seconds < 3600){
            long min = (seconds/60);
            long sec = seconds%60;
            view.getTimeSecondsComboBox().getSelectionModel().select((int)sec);
            view.getTimeMinutesComboBox().getSelectionModel().select((int)min);
        }else {
            long hour = seconds/3600;
            long remaining = seconds%3600;
            long min = remaining/60;
            long sec = remaining%60;
            view.getTimeSecondsComboBox().getSelectionModel().select((int)sec);
            view.getTimeMinutesComboBox().getSelectionModel().select((int)min);
            view.getTimeHoursComboBox().getSelectionModel().select((int)hour);
        }
    }

    private boolean checkServerInfo(){
        if(getServerInfoSet() && selectedServer != null){
            updateServerInfo();
            return true;
        }else {
            logger.trace("Server info not filled. Cannot proceed.");
            String message = null;
            for(ValidationMessage msg : serverValidationSupport.getValidationResult().getErrors()){
                if(message == null){
                    message = msg.getText();
                }else {
                    message = message + "\n"  + msg.getText();
                }
            }
            showWarning("Server information","Please fill all necessary information",message);
            return false;
        }
    }

    private boolean checkStopConditions(){
        String message = validateStopConditions();
        if(getStopConditionsSet()){
            return true;
        }else {
            logger.warn("Stop conditions not filled. Cannot proceed.");
            showWarning("Stop conditions","Please fill all necessary information",message);
            return false;
        }
    }

    private boolean checkTaskInfo(){
        if(getTaskInfoSet() && selectedVersion != null){
            return true;
        }else{
            logger.trace("Task info not filled. Cannot \"Finish\" dialog");
            String message = null;
            for(ValidationMessage msg : taskInfoSupport.getValidationResult().getErrors()){
                if(message == null){
                    message = msg.getText();
                }else {
                    message = message + "\n"  + msg.getText();
                }
            }
            showWarning("Task information","Please fill all necessary information",message);
            return false;
        }
    }

    public void cancel() {
        window.hide();
    }

    public void addValue(){
        ChoiceDialog<Type> dlg = new ChoiceDialog<>(enabledValuesTypes.get(0),enabledValuesTypes);
        dlg.setTitle("Choose");
        dlg.getDialogPane().setContentText("Choose value type");
        dlg.show();
        dlg.resultProperty().addListener(new ChangeListener<Type>() {
            @Override
            public void changed(ObservableValue<? extends Type> observable, Type oldValue, Type newValue) {
                addValueToTree(newValue);
            }
        });
    }

    public void deleteValue(){
        logger.trace("Deleting values");
        TreeItem item = (TreeItem) view.getValuesTree().getSelectionModel().getSelectedItem();
        if(item != null){
            values.remove(item.getValue());
            view.getValuesTree().getRoot().getChildren().remove(item);
        }
        if(values.size() > 0){
            view.getValuesTree().getSelectionModel().select(0);
        }
        updateTree();
    }

    public void deleteValue(Value v,boolean removeFromValuesList){
        TreeItem item = null;
        for(TreeItem t : (ObservableList<TreeItem>)view.getValuesTree().getRoot().getChildren()){
            if(t.getValue().equals(v)){
                item = t;
            }
        }
        if(item!= null){
            if(removeFromValuesList) values.remove(v);
            view.getValuesTree().getRoot().getChildren().remove(item);
        }
        updateTree();
    }

    public void openSaveDirDialog(){
        DirectoryChooser directoryChooser = new DirectoryChooser();
        directoryChooser.setTitle("Choose log file location");
        if(view.getSaveDirTextField().getText().equals(TaskParameters.DEFAULT_SAVE_DIR)){
            directoryChooser.setInitialDirectory(new File(TaskParameters.DEFAULT_SAVE_DIR));
        }else{
            directoryChooser.setInitialDirectory(new File(view.getSaveDirTextField().getText()));
        }
        //Show open file dialog
        File file = directoryChooser.showDialog(window);
        if(file!=null){
            view.getSaveDirTextField().setText(file.getPath());
        }
    }

    public void showAdaptersInDB(){
        if(selectedServer == null) {
            showWarning("Choose server","Choose server first","Before connecting to database, you have to choose server.");
            return;
        }
        ShowAdaptersDialogPresenter showAdaptersDialogPresenter;
        try{
            Stage stage = new Stage();
            showAdaptersDialogPresenter = new ShowAdaptersDialogPresenter(stage,selectedServer);
            stage.setTitle("Adapters in database");
            Scene scene = new Scene((Parent) showAdaptersDialogPresenter.loadView());
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            stage.setScene(scene);
            //stage.setResizable(false);
            stage.show();
            showAdaptersDialogPresenter.refresh();
        } catch (IOException e) {
            Utilities.showException(logger, "Cannot load dialog for showing adapters in database!", e, false,null);
        }
    }

    @SuppressWarnings("unchecked")
    protected void addValueToTree(Type type) {
        logger.debug("Chosen value: " + type.getName());
        Value newValue = ValueFactory.buildValue(type);
        for (Value v : values){
            if(v.getName().equals(newValue.getName())){
                logger.trace("Value already in tree");
                return;
            }
        }
        TreeItem newItem = new TreeItem<>(newValue);
        view.getValuesTree().getRoot().getChildren().add(newItem);
        view.getValuesTree().getSelectionModel().select(newItem);
        values.add(newValue);
    }

    protected void addValueToTree(Value newValue) {
        logger.debug("Adding new value to tree: " + newValue.getName());
        for (Value v : values){
            if(v.getName().equals(newValue.getName())){
                logger.trace("Value already in tree");
                return;
            }
        }
        TreeItem newItem = new TreeItem<>(newValue);
        view.getValuesTree().getRoot().getChildren().add(newItem);
        view.getValuesTree().getSelectionModel().select(newItem);
        values.add(newValue);
    }

    private void showWarning(String title,String headerMessage,String message){
        logger.trace("Showing warning");
        Alert dlg = new Alert(Alert.AlertType.WARNING, "");
        dlg.initModality(Modality.WINDOW_MODAL);
        dlg.initOwner(this.window);
        dlg.setTitle(title);
        dlg.getDialogPane().setContentText(message);
        dlg.getDialogPane().setHeaderText(headerMessage);
        dlg.show();
    }

    private void showInformation(String title,String headerMessage,String message){
        logger.trace("Showing information");
        Alert dlg = new Alert(Alert.AlertType.INFORMATION, "");
        dlg.initModality(Modality.WINDOW_MODAL);
        //dlg.initOwner(this.window);
        dlg.setTitle(title);
        dlg.getDialogPane().setContentText(message);
        dlg.getDialogPane().setHeaderText(headerMessage);
        dlg.show();
    }

    @Override
    public Node loadView() throws IOException {
        logger.trace("Loading AddNewTaskView from: " + FXML_PATH);
        InputStream fxmlStream = null;
        try
        {
            //load resource FXML
            fxmlStream = getClass().getResourceAsStream(FXML_PATH);
            FXMLLoader loader = new FXMLLoader();
            loader.load(fxmlStream);
            //remember view
            view = loader.getController();
            // bind view and presenter
            bind();
            initialize();
            logger.trace("OK");
            return view.getView();
        }
        finally
        {
            if (fxmlStream != null) fxmlStream.close();
        }
    }

    @SuppressWarnings("unchecked")
    private void initialize(){
        //----------------SERVER--------------------
        initializeServer();
        //------------END SERVER--------------------
        //----------------VALUES--------------------
        initializeValues();
        //------------END VALUES--------------------
        //----------STOP CONDITION------------------
        initializeStopConditions();
        //--------END STOP CONDITION----------------
        //----------------TASK----------------------
        initializeTaskInfo();
        //--------------END TASK--------------------
    }

    private void initializeServer(){
        BooleanBinding textFieldToModify = new BooleanBinding() {
            {
                bind(modifyServerProperty());
            }
            @Override
            protected boolean computeValue() {
                return !getModifyServer();
            }
        };
        view.getServerModifyCheckBox().selectedProperty().bindBidirectional(modifyServerProperty());
        view.getServerNameTxtField().disableProperty().bind(textFieldToModify);
        view.getServerIpTxtField().disableProperty().bind(textFieldToModify);
        view.getServerPortTxtField().disableProperty().bind(textFieldToModify);
        view.getServerDbNameTxtField().disableProperty().bind(textFieldToModify);
        //--server field validator
        serverValidationSupport.registerValidator(view.getServerNameTxtField(), false, Validator.createEmptyValidator("Name is required"));
        serverValidationSupport.registerValidator(view.getServerIpTxtField(), false, Validator.createEmptyValidator("Ip is required"));
        serverValidationSupport.registerValidator(view.getServerIpTxtField(), false, (Control c, String newValue) ->
                ValidationResult.fromErrorIf(view.getServerIpTxtField(), "Ip must be in IP4 address format (xxx.xxx.xxx.xxx)", !Utilities.isIp(newValue)));
        serverValidationSupport.registerValidator(view.getServerPortTxtField(), false, Validator.createEmptyValidator("Port is required"));
        serverValidationSupport.registerValidator(view.getServerPortTxtField(), false, (Control c, String newValue) ->
                ValidationResult.fromErrorIf(view.getServerPortTxtField(), "Port must be an integer number with maximum of 5 digits", !Utilities.isIntegerNumber(newValue, 1, 5)));
        serverValidationSupport.registerValidator(view.getServerDbNameTxtField(), false, Validator.createEmptyValidator("Database name is required"));
        //--set placeholders
        view.getServerNameTxtField().setPromptText("Example: devel");
        view.getServerIpTxtField().setPromptText("Example: 10.1.0.1");
        view.getServerPortTxtField().setPromptText("Example: 7080");
        view.getServerDbNameTxtField().setPromptText("Example: home4");
        //disable checkbox
        view.getServerModifyCheckBox().setDisable(true);
        //check if we have any servers
        if(this.servers == null){
            this.servers = FXCollections.observableArrayList();
        }else{
            //save default server
            this.servers.add(new Server(false,"New server...","",0,""));
        }
        //populate combo box
        view.getServerComboBox().setItems(servers);
        view.getServerComboBox().setCellFactory(new Callback<ListView<Server>, ListCell<Server>>() {
            @Override
            public ListCell<Server> call(ListView<Server> param) {
                return new ListCell<Server>(){
                    @Override
                    protected void updateItem(Server s,boolean bln){
                        super.updateItem(s,bln);
                        if( s != null ){
                            setText(s.getName());
                        }else{
                            setText(null);
                        }
                    }
                };
            }
        });
        //save change listener, if new item is selected in combo box, other fields are changed
        view.getServerComboBox().valueProperty().addListener((observable, oldValue, newValue) -> {
            if(oldValue == null){
                view.getServerModifyCheckBox().setDisable(false);
            }
            Server tmpServer = (Server) newValue;
            setServerTextFields(tmpServer.getName(), tmpServer.getIp(), String.valueOf(tmpServer.getPort()), tmpServer.getDatabaseName());
            selectedServer = tmpServer;
            view.getServerModifyCheckBox().requestFocus();
        });
        //bind field validation support to variable holding completion of server info
        serverValidationSupport.validationResultProperty().addListener((observable, oldValue, newValue) -> {
            setServerInfoSet(!serverValidationSupport.isInvalid());
        });
    }

    private void initializeValues(){
        //determine enabled value types
        for(Type type : Type.values()){
            if( type.ordinal()>= SimulationTask.VALUE_TYPE_MIN && type.ordinal() <= SimulationTask.VALUE_TYPE_MAX){
                enabledValuesTypes.add(type);
            }
        }
        //populate value tree
        if(view.getValuesTree().getRoot() == null){
            view.getValuesTree().setRoot(new TreeItem<Value>(new EmptyValue("Values")));
            view.getValuesTree().getRoot().setExpanded(true);
            view.getValuesTree().setShowRoot(false);
        }
        // add all values to tree
        enabledValuesTypes.forEach(this::addValueToTree);
        //set value tree cell factory
        view.getValuesTree().setCellFactory(new Callback<TreeView<Value>, TreeCell<Value>>() {
            @Override
            public TreeCell call(TreeView<Value> param) {
                TreeCell<Value> treeItem = new TreeCell<Value>() {
                    @Override
                    protected void updateItem(Value v, boolean bln) {
                        super.updateItem(v, bln);
                        if (v != null) {
                            setText(v.getName());
                        } else {
                            setText(null);
                        }
                    }
                };

                return treeItem;
            }
        });
    }

    @SuppressWarnings("unchecked")
    private void initializeStopConditions(){
        //TIME DURATION
        view.getTimeDurationYesRadBtn().setSelected(false);
        view.getTimeDurationNoRadBtn().setSelected(true);
        //-- set cell factory
        view.getTimeHoursComboBox().setCellFactory(new Callback<ListView<Integer>, ListCell<Integer>>() {
            @Override
            public ListCell call(ListView<Integer> param) {
                return new ListCell<Integer>(){
                    @Override
                    protected void updateItem(Integer s,boolean bln){
                        super.updateItem(s,bln);
                        if( s != null ){
                            setText(s.toString() + " h");
                        }else{
                            setText(null);
                        }
                    }
                };
            }
        });
        view.getTimeHoursComboBox().setButtonCell(new ListCell<Integer>() {
            @Override
            protected void updateItem(Integer t, boolean bln) {
                super.updateItem(t, bln);
                if (bln) {
                    setText("");
                } else {
                    setText(t.toString() + " h");
                }
            }
        });
        view.getTimeMinutesComboBox().setCellFactory(new Callback<ListView<Integer>, ListCell<Integer>>() {
            @Override
            public ListCell call(ListView<Integer> param) {
                return new ListCell<Integer>(){
                    @Override
                    protected void updateItem(Integer s,boolean bln){
                        super.updateItem(s,bln);
                        if( s != null ){
                            setText(s + " m");
                        }else{
                            setText(null);
                        }
                    }
                };
            }
        });
        view.getTimeMinutesComboBox().setButtonCell(new ListCell<Integer>() {
            @Override
            protected void updateItem(Integer t, boolean bln) {
                super.updateItem(t, bln);
                if (bln) {
                    setText("");
                } else {
                    setText(t + " m");
                }
            }
        });
        view.getTimeSecondsComboBox().setCellFactory(new Callback<ListView<Integer>, ListCell<Integer>>() {
            @Override
            public ListCell call(ListView<Integer> param) {
                return new ListCell<Integer>(){
                    @Override
                    protected void updateItem(Integer s,boolean bln){
                        super.updateItem(s,bln);
                        if( s != null ){
                            setText(s + " s");
                        }else{
                            setText(null);
                        }
                    }
                };
            }
        });
        view.getTimeSecondsComboBox().setButtonCell(new ListCell<Integer>() {
            @Override
            protected void updateItem(Integer t, boolean bln) {
                super.updateItem(t, bln);
                if (bln) {
                    setText("");
                } else {
                    setText(t + " s");
                }
            }
        });
        //-- populate combo boxes
        view.getTimeHoursComboBox().setItems(HOURS);
        view.getTimeMinutesComboBox().setItems(MINUTES_SECONDS);
        view.getTimeSecondsComboBox().setItems(MINUTES_SECONDS);
        view.getTimeHoursComboBox().valueProperty().addListener((observable, oldValue, newValue) -> {
            logger.trace("Time duration -> hours -> " + newValue);
        });
        view.getTimeMinutesComboBox().valueProperty().addListener((observable, oldValue, newValue) -> {
            logger.trace("Time duration -> minutes -> " + newValue);
        });
        view.getTimeSecondsComboBox().valueProperty().addListener((observable, oldValue, newValue) -> {
            logger.trace("Time duration -> seconds -> " + newValue);
        });
        //-- bind disable properties
        view.getTimeHoursComboBox().disableProperty().bind(view.getTimeDurationNoRadBtn().selectedProperty());
        view.getTimeMinutesComboBox().disableProperty().bind(view.getTimeDurationNoRadBtn().selectedProperty());
        view.getTimeSecondsComboBox().disableProperty().bind(view.getTimeDurationNoRadBtn().selectedProperty());
        //SENT MESSAGES
        view.getSentMessagesNoRadBtn().setSelected(true);
        view.getSentMessagesYesRadBtn().setSelected(false);
        //-- bind disable properties
        view.getSentMessagesCountTxtField().disableProperty().bind(view.getSentMessagesNoRadBtn().selectedProperty());
        //WAITING MESSAGES
        view.getWaitingMessagesNoRadBtn().setSelected(false);
        view.getWaitingMessagesYesRadBtn().setSelected(true);
        view.getWaitingMessagesCountTxtField().setText("100000");
        //-- bind disable properties
        view.getWaitingMessagesCountTxtField().disableProperty().bind(view.getWaitingMessagesNoRadBtn().selectedProperty());
        //BIND field VALIDATION SUPPORT to variable holding completion of server info
        stopConditionsSupport.validationResultProperty().addListener((observable, oldValue, newValue) -> {
            logger.warn("Setting stop condition set to: " + !stopConditionsSupport.isInvalid());
            setStopConditionsSet(!stopConditionsSupport.isInvalid());
        });
    }

    private String validateStopConditions(){
        if(view.getSentMessagesYesRadBtn().isSelected()){
            if(view.getSentMessagesCountTxtField().getText().isEmpty() || !Utilities.isIntegerNumber(view.getSentMessagesCountTxtField().getText(), 1, 10)){
                setStopConditionsSet(false);
                return "Sent messages count cannot be empty and must be an integer number";
            }else {
                setStopConditionsSet(true);
            }
        }
        if(view.getWaitingMessagesYesRadBtn().isSelected()){
            if(view.getWaitingMessagesCountTxtField().getText().isEmpty() || !Utilities.isIntegerNumber(view.getWaitingMessagesCountTxtField().getText(), 1, 10)){
                setStopConditionsSet(false);
                return "Waiting messages count cannot be empty and must be an integer number";
            }else {
                setStopConditionsSet(true);
            }
        }
        if(view.getTimeDurationYesRadBtn().isSelected()){
            if((view.getTimeHoursComboBox().getValue() == null || (Integer)view.getTimeHoursComboBox().getValue() == 0)
                && (view.getTimeMinutesComboBox().getValue() == null || (Integer)view.getTimeMinutesComboBox().getValue() == 0)
                && (view.getTimeSecondsComboBox().getValue() == null || (Integer)view.getTimeSecondsComboBox().getValue() == 0)
                    ){
                setStopConditionsSet(false);
                return "Time duration cannot be set to 0h:0m:0s";
            }else {
                setStopConditionsSet(true);
            }
        }
        return null;
    }

    @SuppressWarnings("unchecked")
    private void initializeTaskInfo(){
        //SENSORS COUNT
        view.setSensorsCountSlider(createRangeSliders(1,100,1,20));
        view.getSensorCountMinLbl().textProperty().bind(new StringBinding() {
            {
                bind(view.getSensorsCountSlider().lowValueProperty());
            }
            @Override
            protected String computeValue() {
                return sensorCountSliderFormatter.format(view.getSensorsCountSlider().getLowValue());
            }
        });
        view.getSensorCountMaxLbl().textProperty().bind(new StringBinding() {
            {
                bind(view.getSensorsCountSlider().highValueProperty());
            }
            @Override
            protected String computeValue() {
                return sensorCountSliderFormatter.format(view.getSensorsCountSlider().getHighValue());
            }
        });
        //REFRESH TIME
        view.setRefreshTimeCountSlider(createRangeSliders(0,Utilities.REFRESH_SLIDER_VALUE_COUNT,0,2));
        view.getRefreshTimeMinLbl().textProperty().bind(new StringBinding() {
            {
                bind(view.getRefreshTimeCountSlider().lowValueProperty());
            }
            @Override
            protected String computeValue() {
                return Utilities.formatSeconds(Utilities.refreshSliderScaleToSeconds(view.getRefreshTimeCountSlider().getLowValue()));
            }
        });
        view.getRefreshTimeMaxLbl().textProperty().bind(new StringBinding() {
            {
                bind(view.getRefreshTimeCountSlider().highValueProperty());
            }
            @Override
            protected String computeValue() {
                return Utilities.formatSeconds(Utilities.refreshSliderScaleToSeconds(view.getRefreshTimeCountSlider().getHighValue()));
            }
        });
        //PROTOCOL
        view.getProtocolComboBox().setItems(Utilities.getProtocolVersions());
        view.getProtocolComboBox().setCellFactory(new Callback<ListView<Protocol.Version>, ListCell<Protocol.Version>>() {
            @Override
            public ListCell call(ListView<Protocol.Version> param) {
                return new ListCell<Protocol.Version>(){
                    @Override
                    protected void updateItem(Protocol.Version s,boolean bln){
                        super.updateItem(s,bln);
                        if( s != null ){
                            setText(String.valueOf(s.getVersion()));
                        }else{
                            setText(null);
                        }
                    }
                };
            }
        });
        //save change listener, if new item is selected in combo box, other fields are changed
        view.getProtocolComboBox().valueProperty().addListener((observable, oldValue, newValue) -> {
            selectedVersion = (Protocol.Version) newValue;
        });
        taskInfoSupport.registerValidator(view.getProtocolComboBox(), false, (Control c, Protocol.Version version) ->
                ValidationResult.fromErrorIf(view.getProtocolComboBox(), "Protocol version is required", (version == null)));
        //SAVE DIR
        view.getSaveDirTextField().setText(TaskParameters.DEFAULT_SAVE_DIR);
        taskInfoSupport.registerValidator(view.getSaveDirTextField(),false,Validator.createEmptyValidator("You have to choose location for task's log file"));
        //ADAPTERS COUNT
        taskInfoSupport.registerValidator(view.getAdaptersCountTxtField(),false,Validator.createEmptyValidator("Adapters count is empty"));
        taskInfoSupport.registerValidator(view.getAdaptersCountTxtField(), false, (Control c, String s) ->
                ValidationResult.fromErrorIf(view.getAdaptersCountTxtField(), "Adapters count must be an integer number(Max 5 digits)", !Utilities.isIntegerNumber(s,1,5)));
        view.getAdaptersCountTxtField().setPromptText("Example: 100");
        //START ID
        taskInfoSupport.registerValidator(view.getStartIdTxtField(),false,Validator.createEmptyValidator("Starting adapter ID is empty"));
        taskInfoSupport.registerValidator(view.getStartIdTxtField(), false, (Control c, String s) ->
                ValidationResult.fromErrorIf(view.getStartIdTxtField(), "Starting adapter ID must be an integer number(Max 5 digits)", !Utilities.isIntegerNumber(s,1,5)));
        view.getAdaptersCountTxtField().setPromptText("Example: 1000");
        //BIND field VALIDATION SUPPORT to variable holding completion of protocol version
        taskInfoSupport.validationResultProperty().addListener((observable, oldValue, newValue) -> {
            setTaskInfoSet(!taskInfoSupport.isInvalid());
        });
    }

    private RangeSlider createRangeSliders(double min,double max,double lowValue,double highValue){
        final RangeSlider slider = new RangeSlider(min,max,lowValue,highValue);
        slider.setShowTickMarks(false);
        slider.setShowTickLabels(false);
        slider.setBlockIncrement(1);
        slider.setMajorTickUnit(1);
        slider.setMinorTickCount(0);
        slider.setSnapToTicks(true);
        return slider;
    }

    private void setServerTextFields(String name,String ip,String port,String dbName){
        view.getServerNameTxtField().setText(name);
        view.getServerIpTxtField().setText(ip);
        view.getServerPortTxtField().setText(port.equals("0") ? "" : port);
        view.getServerDbNameTxtField().setText(dbName);
    }

    private void updateServerInfo(){
        if(selectedServer != null){
            selectedServer.setIp(view.getServerIpTxtField().getText());
            selectedServer.setPort(Integer.valueOf(view.getServerPortTxtField().getText()));
            selectedServer.setDatabaseName(view.getServerDbNameTxtField().getText());
            selectedServer.setName(view.getServerNameTxtField().getText());
        }
    }

    private void updateTree(){
        TreeItem newItem = new TreeItem();
        view.getValuesTree().getRoot().getChildren().add(newItem);
        view.getValuesTree().getRoot().getChildren().remove(newItem);
    }

    @Override
    public void addModel(Object model) {

    }

    @Override
    public Object getModel() {
        return null;
    }

    @Override
    public Node getView() {
        return view.getView();
    }

    @Override
    public void clear() {

    }

    @SuppressWarnings("unchecked")
    @Override
    public void bind() {
        view.setPresenter(this);
    }

    public Server getSelectedServer() {
        return selectedServer;
    }

    public void setSelectedServer(Server selectedServer) {
        this.selectedServer = selectedServer;
    }

    public ObservableList<Server> getServers() {
        return servers;
    }

    public void setServers(ObservableList<Server> servers) {
        this.servers = servers;
    }

    public boolean getModifyServer() {
        return modifyServer.get();
    }

    public BooleanProperty modifyServerProperty() {
        return modifyServer;
    }

    public void setModifyServer(boolean modifyServer) {
        this.modifyServer.set(modifyServer);
    }

    public boolean getServerInfoSet() {
        return serverInfoSet.get();
    }

    public BooleanProperty serverInfoSetProperty() {
        return serverInfoSet;
    }

    public void setServerInfoSet(boolean serverInfoSet) {
        this.serverInfoSet.set(serverInfoSet);
    }

    public boolean getStopConditionsSet() {
        return stopConditionsSet.get();
    }

    public BooleanProperty stopConditionsSetProperty() {
        return stopConditionsSet;
    }

    public void setStopConditionsSet(boolean stopConditionsSet) {
        this.stopConditionsSet.set(stopConditionsSet);
    }

    public boolean getTaskInfoSet() {
        return taskInfoSet.get();
    }

    public BooleanProperty taskInfoSetProperty() {
        return taskInfoSet;
    }

    public void setTaskInfoSet(boolean taskInfoSet) {
        this.taskInfoSet.set(taskInfoSet);
    }
}
