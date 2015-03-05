package com.iha.emulator.ui.dialogs.log;

import com.iha.emulator.control.AdapterController;
import com.iha.emulator.control.SimulationTask;
import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.panels.PanelPresenter;
import com.iha.emulator.utilities.AdapterLogger;
import javafx.animation.Animation;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.application.Platform;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.concurrent.Task;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.control.CheckBox;
import javafx.scene.control.Label;
import javafx.scene.control.ProgressIndicator;
import javafx.scene.control.TextArea;
import javafx.stage.Stage;
import javafx.util.Duration;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.io.InputStream;
import java.io.RandomAccessFile;

/**
 * Created by Shu on 3.2.2015.
 */
public class ShowFullLogPresenter implements Presenter,PanelPresenter{

    private static final Logger logger = LogManager.getLogger(ShowFullLogPresenter.class);
    private static final String FXML_PATH = "ShowFullLog.fxml";
    private static final double DEFAULT_REFRESH_MILLIS = 2000;

    private Display view;
    private Stage window;

    private AdapterController adapterController;
    private SimulationTask currentTask;

    private long filePointer = 0;
    private boolean reading = false;
    private Timeline refreshTimer;

    public interface Display {
        public Node getView();
        public void setPresenter(ShowFullLogPresenter presenter);
        public ProgressIndicator getIndicator();
        public Label getStatus();
        public TextArea getTextArea();
        public CheckBox getRefreshAutomaticallyCheckBox();
    }

    public ShowFullLogPresenter(Stage window,AdapterController adapterController) {
        this.window = window;
        this.adapterController = adapterController;
    }

    public ShowFullLogPresenter(Stage window,SimulationTask currentTask) {
        this.window = window;
        this.currentTask = currentTask;
    }

    public void refresh(){
        showStatus("Loading log file...",true);
        Task<Object> worker = new Task<Object>() {
            @Override
            protected Object call() throws Exception {
                //indicate, that file is being read
                reading = true;
                try{
                    AdapterLogger log = null;
                    if(adapterController != null)
                        log = adapterController.getLog();
                    else if(currentTask != null)
                        log = currentTask.getLog();
                    if(log == null) return null;
                    RandomAccessFile file = new RandomAccessFile(log.getBufferFile(),"r");
                    if( file.length() > filePointer){
                        file.seek(filePointer);
                        String line = file.readLine();
                        while( line != null){
                            processLine(line);
                            line = file.readLine();
                        }
                        filePointer = file.getFilePointer();
                    }
                    file.close();
                    Platform.runLater(()->showStatus("Log loaded",false));
                } catch (IOException  e){
                    Platform.runLater(()->view.getTextArea().appendText("ERROR: cannot open or read from buffer file!"));
                }
                return null;
            }
        };
        //create thread for background process
        Thread th = new Thread(worker);
        th.setDaemon(true);
        //run background process
        th.start();
    }

    private void processLine(String line){
        Platform.runLater(()-> view.getTextArea().appendText(line + "\n"));
    }

    public void close(){
        window.hide();
    }

    public void showStatus(String status,boolean indicate){
        view.getStatus().setText(status);
        view.getIndicator().setVisible(indicate);
    }

    private void startRefreshTimer(){
        if(refreshTimer != null) refreshTimer.play();
    }

    private void stopRefreshTimer(){
        if(refreshTimer != null) refreshTimer.stop();
    }

    @Override
    public Node loadView() throws IOException {
        logger.trace("Loading ShowFullLogView from: " + FXML_PATH);
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
            startRefreshTimer();
            logger.trace("OK");
            return view.getView();
        }
        finally
        {
            if (fxmlStream != null) fxmlStream.close();
        }
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

    @Override
    public void bind() {
        view.setPresenter(this);
        refreshTimer = new Timeline(new KeyFrame(
                Duration.millis(DEFAULT_REFRESH_MILLIS),
                ae -> refresh()));
        //set infinite periodical cycle
        refreshTimer.setCycleCount(Animation.INDEFINITE);
        view.getRefreshAutomaticallyCheckBox().selectedProperty().addListener(new ChangeListener<Boolean>() {
            @Override
            public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
                if(newValue){
                    startRefreshTimer();
                }else {
                    stopRefreshTimer();
                }
            }
        });
    }
}
