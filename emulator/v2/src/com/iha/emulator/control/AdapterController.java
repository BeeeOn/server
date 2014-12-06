package com.iha.emulator.control;

import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.models.Adapter;
import com.iha.emulator.models.Sensor;
import com.iha.emulator.models.SensorType;
import com.iha.emulator.models.Server;
import com.iha.emulator.models.value.implemented.HumiditySensorValue;
import com.iha.emulator.models.value.implemented.OnOffActuatorValue;
import com.iha.emulator.models.value.implemented.TemperatureSensorValue;
import com.iha.emulator.models.value.ValueFactory;
import com.iha.emulator.ui.panels.sensor.SensorPanelPresenter;
import com.iha.emulator.utilities.AdapterLogger;
import javafx.beans.property.BooleanProperty;
import javafx.beans.property.ListProperty;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.beans.property.SimpleListProperty;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.LoadException;
import javafx.scene.Node;
import javafx.scene.layout.FlowPane;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.util.ArrayList;
/**
 * Created by Shu on 27.11.2014.
 */
public class AdapterController {

    private static final Logger logger = LogManager.getLogger(AdapterController.class);

    private Adapter adapter;
    private Server server;
    private ObservableList<Sensor> oSensors = FXCollections.observableArrayList();
    private ListProperty<Sensor> sensors = new SimpleListProperty<>(oSensors);
    private AdapterLogger log;
    private BooleanProperty internetConnection;

    private ArrayList<SensorPanelPresenter> sensorPanelPresenters;

    public AdapterController() {
        this.internetConnection = new SimpleBooleanProperty(true);
        this.log = new AdapterLogger();
    }

    public void start(){
        logger.debug("Starting adapter -> " + adapter.getId());
        adapter.setStatus(true);
    }

    public void stop(){
        logger.debug("Stopping adapter -> " + adapter.getId());
        adapter.setStatus(false);
    }

    public Adapter createAdapter(boolean status,int id,boolean registered,double protocolVersion,double firmware){
        logger.trace("Creating adapter model: " + "status->"+status + " id->"+id + " registered->"+registered + " protocol->" + protocolVersion + " firmware->" + firmware);
        this.adapter = new Adapter(status,id,registered,protocolVersion,firmware);
        logger.trace("OK");
        return this.adapter;
    }

    public Server createServer(boolean conn,String name,String ip, int port,String databaseName){
        logger.trace("Creating server model: " + "conn->"+conn + "name->"+name + " ip->"+ip + " port->"+port + " DB->"+databaseName);
        this.server = new Server(conn,name,ip,port,databaseName);
        logger.trace("OK");
        return this.server;
    }

    public Sensor createSensor(Node container,boolean status,int id,String name,int battery,int signal,int refreshTime,Protocol protocol) throws LoadException {
        if(sensorPanelPresenters == null) sensorPanelPresenters = new ArrayList<>();
        SensorPanelPresenter newPanel;
        try {
            //create presenter
            newPanel = new SensorPanelPresenter();
            //create model
            Sensor newSensor = new Sensor(status,id,name,battery,signal,refreshTime,protocol);
            //add values
            TemperatureSensorValue tmp = ValueFactory.buildValue(
                    SensorType.SENSOR_TEMPERATURE,
                    "Temperature",
                    false,
                    false,
                    null);
            tmp.setGeneratorVariables(5,34.5,50,-50);
            newSensor.getValues().add(tmp);
            //add values
            HumiditySensorValue tmp1 = ValueFactory.buildValue(
                    SensorType.SENSOR_HUMIDITY,
                    "Humidity",
                    false,
                    false,
                    null);
            tmp1.setGeneratorVariables(1,55,100,0);
            newSensor.getValues().add(tmp1);
            OnOffActuatorValue tmp2 = ValueFactory.buildValue(
                    SensorType.ACTUATOR_ON_OFF,
                    "On/Off Actuator",
                    false,
                    false,
                    null);
            newSensor.getValues().add(tmp2);
            /*for (int i = 0; i < 100; i++) {
                try {
                    tmp.nextValue();
                    logger.debug("New value -> " + tmp.getStringValue());
                }catch (NullPointerException e){
                    logger.error(e.getMessage());
                }
            }*/

                    ((FlowPane) container).getChildren().add(0,newPanel.loadView());
            newPanel.addModel(newSensor);
            newSensor.statusProperty().addListener(new ChangeListener<Boolean>() {
                @Override
                public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
                    int activeCount = adapter.getActiveCount();
                    logger.trace("Active sensors before change: " + activeCount);
                    if(newValue){
                        adapter.setActiveCount(++activeCount);
                    }else{
                        adapter.setActiveCount(--activeCount);
                    }
                    logger.trace("Active sensors after change: " + adapter.getActiveCount());
                }
            });
            sensors.add(newSensor);
            sensorPanelPresenters.add(newPanel);
        } catch (IOException e) {
            throw new LoadException("Cannot initiate new sensor panel.Error while loading FXML file",e);
        }
        return (Sensor)newPanel.getModel();
    }

    public void createLog(){
        this.log = new AdapterLogger();
    }

    public ListProperty<Sensor> getSensors(){
        return this.sensors;
    }

    public Adapter getAdapter() {
        return adapter;
    }

    public Server getServer() {
        return server;
    }

    public boolean getInternetConnection() {
        return internetConnection.get();
    }

    public BooleanProperty internetConnectionProperty() {
        return internetConnection;
    }

    public void setInternetConnection(boolean internetConnection) {
        this.internetConnection.set(internetConnection);
    }

    public AdapterLogger getLog() {
        return log;
    }

    public void setLog(AdapterLogger log) {
        this.log = log;
    }

    public ArrayList<SensorPanelPresenter> getSensorPanelPresenters() {
        return sensorPanelPresenters;
    }
}
