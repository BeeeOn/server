package com.iha.emulator.ui.simulations.performance;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.control.*;
import javafx.scene.layout.FlowPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.StackPane;
import jfxtras.scene.control.LocalDateTimeTextField;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.net.URL;
import java.util.ResourceBundle;

/**
 * Created by Shu on 23/11/14.
 */
public class PerformanceSimulationView implements Initializable,PerformanceSimulationPresenter.Display{

    private static final Logger logger = LogManager.getLogger(PerformanceSimulationView.class);
    private PerformanceSimulationPresenter presenter;

    //region MENU
    @FXML private MenuItem newTaskItem;
    @FXML private MenuItem startTaskItem;
    @FXML private MenuItem pauseTaskItem;
    @FXML private MenuItem stopTaskItem;
    @FXML private MenuItem resumeSensorsItem;
    @FXML private MenuItem pauseSensorsItem;
    @FXML private MenuItem openItem;
    @FXML private MenuItem saveAllItem;
    @FXML private MenuItem quitItem;
    //endregion
    //region TOOLBAR (TBtn = Toolbar button)
    @FXML private ToolBar tooBar;
    @FXML private Button openTBtn;
    @FXML private Button saveAllTBtn;
    @FXML private Button startTaskTBtn;
    @FXML private Button pauseTaskTBtn;
    @FXML private Button stopTaskTBtn;
    @FXML private Button pauseSensorsTBtn;
    @FXML private Button resumeSensorsTBtn;
    @FXML private Button newTaskTBtn;
    @FXML private Button deleteTaskTBtn;
    //endregion
    @FXML private Node view;
    //region BOTTOM STATUS LINE
    @FXML private Label statusLineLbl;
    @FXML private ProgressIndicator statusIndicator;
    @FXML private ProgressBar memCheckProgressBar;
    @FXML private Label memCheckStatusLbl;
    //endregion
    @FXML private TextArea appLog;
    //region DETAILS
    @FXML private FlowPane serverDetailsContainer;
    @FXML private FlowPane taskDetailsContainer;
    //endregion
    //region ADAPTER
    @FXML private StackPane adapterLogContainer;
    @FXML private ScrollPane toBeSentLogContainer;
    @FXML private TabPane logTabPane;
    @FXML private Tab errorTab;
    @FXML private StackPane errorLogContainer;
    @FXML private ToggleGroup logMessageTypeRadBtnGroup;
    @FXML private RadioButton fullLogMessageRadBtn;
    @FXML private RadioButton partialLogMessageRadBtn;
    @FXML private RadioButton shortLogMessageRadBtn;
    @FXML private Button showFullLogBtn;
    //endregion
    //region TASKS
    @FXML private TableView tasksTable;
    @FXML private TableColumn enabledColumn;
    @FXML private TableColumn idColumn;
    @FXML private TableColumn stateColumn;
    @FXML private TableColumn serverColumn;
    @FXML private TableColumn stopConditionColumn;
    @FXML private HBox queueProcessContainer;
    @FXML private Button queueProcessOnBtn;
    @FXML private Button queueProcessOffBtn;
    @FXML private RadioButton immediatelyStartRadBtn;
    @FXML private RadioButton pickTimeStartRadBtn;
    private LocalDateTimeTextField startDateTimeTextField;
    //endregion
    //region SERVER RESPONSE
    @FXML private StackPane responseChartContainer;
    //endregion

    public PerformanceSimulationView(){
    }

    @FXML
    public void handleStartTask(ActionEvent event){
        logger.trace("Start Task Clicked!");
        presenter.startTask();
    }

    @FXML
    public void handlePauseTask(ActionEvent event){
        logger.trace("Pause task Clicked!");
        presenter.pauseTask();
    }

    @FXML
    public void handleStopTask(ActionEvent event){
        logger.trace("Stop task Clicked!");
        presenter.stopTask();
    }

    @FXML
    public void handleResumeSensors(ActionEvent event){
        logger.trace("Resume Sensors Clicked!");
        presenter.resumeSensors();
    }

    @FXML
    public void handlePauseSensors(ActionEvent event){
        logger.trace("Pause sensors Clicked!");
        presenter.pauseSensors();
    }

    @FXML
    public void handleAddTask(ActionEvent event){
        logger.trace("Add task Clicked!");
        presenter.addTask();
    }

    @FXML
    public void handleDeleteTask(ActionEvent event){
        logger.trace("Delete task Clicked!");
        presenter.deleteTask();
    }

    @FXML
    public void handleSaveAll(ActionEvent event) {
        logger.trace("Save all Clicked!");
        presenter.saveAll(null);
    }

    @FXML
    public void handleOpen(ActionEvent event) {
        logger.trace("Open Clicked!");
        presenter.open();
    }

    @FXML
    public void handlePrint(ActionEvent event) {
        logger.debug("Print Clicked! -> unimplemented");
    }

    @FXML
    public void handleQuit(ActionEvent event) {
        if(presenter!=null) presenter.quit();
    }


    @FXML
    public void handleShowFullLog(ActionEvent event) {
        logger.trace("Show full log Clicked!");
        presenter.showFullLog();
    }

    @FXML
    public void handleQueueProcessOn(){
        logger.trace("Queue process ON Clicked!");
        presenter.queueProcessStart();
    }

    @FXML
    public void handleQueueProcessOff(){
        logger.trace("Queue process OFF Clicked!");
        presenter.queueProcessStop();
    }

    @Override
    public MenuItem getOpenItem() {
        return openItem;
    }

    @Override
    public Button getOpenTBtn() {
        return openTBtn;
    }

    @Override
    public TabPane getLogTabPane() {
        return logTabPane;
    }

    @Override
    public Tab getErrorTab() {
        return errorTab;
    }

    @Override
    public void setStatusLine(String status){
        statusLineLbl.setText(status);
    }

    @Override
    public void setStatusIndicator(boolean visible) {
        statusIndicator.setVisible(visible);
    }

    @Override
    public ProgressBar getMemCheckProgressBar() {
        return memCheckProgressBar;
    }

    @Override
    public Label getMemCheckStatusLabel() {
        return memCheckStatusLbl;
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {

    }

    @Override
    public void setPresenter(PerformanceSimulationPresenter presenter) {
        this.presenter = presenter;
    }

    @Override
    public Node getView(){
        return view;
    }

    @Override
    public TextArea getApplicationLogTextArea() {
        return appLog;
    }

    @Override
    public void addServerDetailsView(Node serverDetailsView) {
        serverDetailsContainer.getChildren().add(serverDetailsView);
    }

    @Override
    public void addTaskDetailsView(Node taskDetailsView) {
        taskDetailsContainer.getChildren().add(taskDetailsView);
    }

    @Override
    public FlowPane getTaskDetailsContainer() {
        return taskDetailsContainer;
    }

    @Override
    public Button getSaveAllBtn() {
        return saveAllTBtn;
    }

    @Override
    public StackPane getErrorLogContainer() {
        return errorLogContainer;
    }

    @Override
    public ScrollPane getToBeSentLogContainer() {
        return toBeSentLogContainer;
    }

    @Override
    public StackPane getAdapterLogContainer() {
        return adapterLogContainer;
    }

    @Override
    public RadioButton getShortLogMessageRadBtn() {
        return shortLogMessageRadBtn;
    }

    @Override
    public RadioButton getPartialLogMessageRadBtn() {
        return partialLogMessageRadBtn;
    }

    @Override
    public RadioButton getFullLogMessageRadBtn() {
        return fullLogMessageRadBtn;
    }

    @Override
    public ToggleGroup getLogMessageTypeRadBtnGroup() {
        return logMessageTypeRadBtnGroup;
    }

    @Override
    public Button getShowFullLogBtn() {
        return showFullLogBtn;
    }

    @Override
    public MenuItem getNewTaskItem() {
        return newTaskItem;
    }

    @Override
    public MenuItem getStartTaskItem() {
        return startTaskItem;
    }

    @Override
    public MenuItem getPauseTaskItem() {
        return pauseTaskItem;
    }

    @Override
    public MenuItem getStopTaskItem() {
        return stopTaskItem;
    }

    @Override
    public MenuItem getResumeSensorsItem() {
        return resumeSensorsItem;
    }

    @Override
    public MenuItem getPauseSensorsItem() {
        return pauseSensorsItem;
    }

    @Override
    public Button getStartTaskTBtn() {
        return startTaskTBtn;
    }

    @Override
    public Button getPauseTaskTBtn() {
        return pauseTaskTBtn;
    }

    @Override
    public Button getStopTaskTBtn() {
        return stopTaskTBtn;
    }

    @Override
    public Button getPauseSensorsTBtn() {
        return pauseSensorsTBtn;
    }

    @Override
    public Button getResumeSensorsTBtn() {
        return resumeSensorsTBtn;
    }

    @Override
    public Button getDeleteTaskTBtn() {
        return deleteTaskTBtn;
    }

    @Override
    public Button getNewTaskTBtn() {
        return newTaskTBtn;
    }

    @Override
    public TableColumn getServerColumn() {
        return serverColumn;
    }

    @Override
    public TableColumn getStateColumn() {
        return stateColumn;
    }

    @Override
    public TableColumn getEnabledColumn() {
        return enabledColumn;
    }

    @Override
    public TableColumn getIdColumn() {
        return idColumn;
    }

    @Override
    public TableColumn getStopConditionColumn() {
        return stopConditionColumn;
    }

    @Override
    public HBox getQueueProcessContainer() {
        return queueProcessContainer;
    }

    @Override
    public Button getQueueProcessOnBtn() {
        return queueProcessOnBtn;
    }

    @Override
    public Button getQueueProcessOffBtn() {
        return queueProcessOffBtn;
    }

    @Override
    public LocalDateTimeTextField getStartDateTimeTextField() {
        return startDateTimeTextField;
    }

    @Override
    public void setStartDateTimeTextField(LocalDateTimeTextField startDateTimeTextField) {
        this.startDateTimeTextField = startDateTimeTextField;
    }

    @Override
    public RadioButton getPickTimeStartRadBtn() {
        return pickTimeStartRadBtn;
    }

    @Override
    public RadioButton getImmediatelyStartRadBtn() {
        return immediatelyStartRadBtn;
    }

    @Override
    public TableView getTasksTable() {
        return tasksTable;
    }

    @Override
    public StackPane getResponseChartContainer() {
        return responseChartContainer;
    }
}
