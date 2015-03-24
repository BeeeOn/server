package com.iha.emulator.control;

import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.communication.protocol.SetNewValue;
import com.iha.emulator.communication.server.MessageSender;
import com.iha.emulator.communication.server.OutMessage;
import com.iha.emulator.communication.server.ServerController;
import com.iha.emulator.communication.server.ServerReceiver;
import com.iha.emulator.control.scheduler.Scheduler;
import com.iha.emulator.control.scheduler.implemented.DetailedScheduler;
import com.iha.emulator.control.scheduler.implemented.PerformanceScheduler;
import com.iha.emulator.models.Adapter;
import com.iha.emulator.models.Sensor;
import com.iha.emulator.models.Server;
import com.iha.emulator.models.value.Value;
import com.iha.emulator.resources.images.sensor_types.SensorIcon;
import com.iha.emulator.ui.panels.sensor.SensorPanelPresenter;
import com.iha.emulator.utilities.logging.AdapterLogger;
import com.iha.emulator.utilities.watchers.ResponseTracker;
import javafx.application.Platform;
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
import javafx.scene.control.TabPane;
import javafx.scene.layout.FlowPane;
import javafx.scene.text.Text;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.dom4j.Document;
import org.dom4j.Element;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;

/**
 * Created by Shu on 27.11.2014.
 */
public class AdapterController {

    private static final Logger logger = LogManager.getLogger(AdapterController.class);
    static final int SLEEP_TIME = 500;

    private boolean terminate = false;

    private Adapter adapter;
    private ServerController serverController;
    private Scheduler scheduler;
    private MessageSender messageSender;

    private ServerReceiver serverReceiver = null;

    private ListProperty<SensorController> sensorControllers = new SimpleListProperty<>(FXCollections.observableArrayList());

    private AdapterLogger log;
    private BooleanProperty internetConnection;
    private boolean registerMessageSent = false;
    private boolean saved = false;

    private ArrayList<OutMessage> messages = new ArrayList<>();

    public AdapterController() {
        this.internetConnection = new SimpleBooleanProperty(true);
    }


    public void enable() {
        logger.trace("Enabling adapter -> " + adapter.getId());
        adapter.setStatus(true);
    }

    public void disable() {
        logger.trace("Disabling adapter -> " + adapter.getId());
        adapter.setStatus(false);
    }

    public void sendMessage(String logMessage) {
        log.log(logMessage);
    }

    public void sendMessage(OutMessage message) {
        if (messages != null) {
            switch (message.getType()){
                case REGISTER_ADAPTER:
                    messages.add(0,message);
                    break;
                case SENSOR_MESSAGE:
                    messages.add(message);
                    break;
            }
            startScheduler();
        }
    }

    public void sendMessage(String logMessage, Document message, SensorController senderController, OutMessage.Type type) {
        if (message != null) {
            Text unsent;
            switch (type) {
                case REGISTER_ADAPTER:
                    unsent = log.sent("Adapter/" + getAdapter().getId() + " -> trying to register");
                    messages.add(0, new OutMessage(logMessage, message, senderController, type, unsent));
                    break;
                case SENSOR_MESSAGE:
                    if (senderController.isFullMessage()) {
                        unsent = log.sent("Adapter/" + getAdapter().getId() + " -> Sensor/" + senderController.getModel().getId() + " waiting to send data.");
                    } else {
                        unsent = log.sent("Sensor " + senderController.toString() + " waiting to send data.");
                    }
                    messages.add(new OutMessage(logMessage, message, senderController, type, unsent));
                    break;
            }
            startScheduler();
        }
    }

    public synchronized void sendError(String message, Throwable t, boolean terminate) {
        if (t != null) message = message + " -> " + t.getMessage();
        getLog().error(message, terminate);
    }

    public void changeValueOnSensor(int sensorId, ArrayList<SetNewValue> newValues) throws IllegalArgumentException {
        SensorController sensorController = getSensorById(sensorId);
        if (sensorController == null)
            throw new IllegalArgumentException("Sensor with id \"" + sensorId + "\n doesn't exist");
        try {
            for (SetNewValue newValue : newValues) {
                logger.trace("Trying to change value on sensor: " + sensorController.toString());
                sensorController.changeValue(newValue.getType(), newValue.getOffset(), newValue.getValue());
            }
        } catch (IllegalArgumentException e) {
            Platform.runLater(() -> log.error(e.getMessage(), false));
            logger.error(e.getMessage(), e);
        }
    }

    public void cleanSensor(int sensorId) throws IllegalArgumentException{
        Platform.runLater(()->deleteSensor(getSensorControllerById(sensorId)));
    }

    private SensorController getSensorControllerById(int id)throws IllegalArgumentException{
        for (SensorController s : getSensorControllers()){
            if(s.getModel().getId() == id) return s;
        }
        throw new IllegalArgumentException("Sensor id \"" + id + "\" not found");
    }

    public void deleteSensors(ObservableList<SensorController> sensorControllers) {
        if (sensorControllers == null) return;
        for (Iterator<SensorController> it = sensorControllers.iterator(); it.hasNext(); ) {
            SensorController sensorController = it.next();
            deleteSensor(sensorController);
        }
        getSensorControllers().removeAll(sensorControllers);
        System.gc();
    }

    public void deleteSensor(SensorController sensorController){
        String name;
        name = sensorController.toString();
        logger.debug(name + " -> Deleting");
        if (sensorController.getTimerRunning()) sensorController.setTimerRunning(false);
        if (sensorController.getPanel() != null) {
            logger.trace(name + " -> removing panel from panel container");
            sensorController.getPanel().deletePanel();
        }
        logger.trace(name + " -> removing unsent messages");
        removeUnsentMessages(sensorController);
        logger.trace(name + " -> removing from list");
        sensorController.delete();
        logger.debug(name + " -> OK");
    }



    public void deleteAll() {
        logger.trace("Removing sensors");
        deleteSensors(getSensorControllersList().get());
        logger.trace("Removing logs");
        getLog().delete();
        getLog().deleteBufferFile();
        logger.trace("Removing scheduler");
        getScheduler().terminateScheduler(true);
        messages.clear();
        messages = null;
        logger.trace("Removing server");
        serverController.delete();
        sensorControllers = null;
        log = null;
        serverController = null;
        scheduler = null;
    }

    public void terminateScheduler() {
        logger.trace("Terminating scheduler");
        messages.clear();
        getScheduler().terminateScheduler(false);
        if (getScheduler().isAlive()) getScheduler().interrupt();
        try {
            getScheduler().join();
        } catch (InterruptedException e) {
            logger.error("Cannot join " + getScheduler().getName());
        }
    }

    public void delete() {
        logger.trace("Removing sensors");
        deleteSensors(getSensorControllersList().get());
        messages = null;
        sensorControllers = null;
        log = null;
        serverController = null;
        scheduler = null;
    }

    public void clearResponseTracker() {
        if (scheduler == null) return;
        scheduler.clearResponseTracker();
    }

    private void removeUnsentMessages(SensorController sensorController) {
        if (messages == null || messages.size() == 0) return;
        for (Iterator<OutMessage> it = messages.iterator(); it.hasNext(); ) {
            OutMessage message = it.next();
            if (message.getSenderController() != null && message.getSenderController().equals(sensorController)) {
                it.remove();
            }
        }
        log.removeUnsentMessagesBySensor(sensorController);
    }

    private SensorController getSensorById(int sensorId) {
        for (SensorController sensorController : sensorControllers) {
            if (sensorController.getModel().getId() == sensorId) return sensorController;
        }
        return null;
    }

    public synchronized void messageSuccessfullySent(OutMessage message) {
        if (messages.size() != 0) messages.remove(message);
        getLog().notifyDataSent(message.getUnsent());
    }

    /**
     * Returns next unprocessed message
     *
     * @return message to be processed, null if there are no new messages
     */
    public OutMessage getNextMessage() {
        if (!messages.isEmpty())
            return messages.get(0);
        else
            return null;
    }

    public Adapter createAdapter(boolean status, int id, boolean registered, Protocol.Version protocolVersion, double firmware) {
        logger.trace("Creating adapter model: " + "status->" + status + " id->" + id + " registered->" + registered + " protocol->" + protocolVersion + " firmware->" + firmware);
        this.adapter = new Adapter(status, id, registered, protocolVersion, firmware);
        logger.trace("OK");

        return this.adapter;
    }

    public Adapter createAdapter(String name, boolean status, int id, boolean registered, Protocol.Version protocolVersion, double firmware) {
        logger.trace("Creating adapter model: " + "name->" + name + "status->" + status + " id->" + id + " registered->" + registered + " protocol->" + protocolVersion + " firmware->" + firmware);
        if (name != null && name.equals("")) {
            this.adapter = new Adapter(status, id, registered, protocolVersion, firmware);
        } else {
            this.adapter = new Adapter(name, status, id, registered, protocolVersion, firmware);
        }
        logger.trace("OK");
        return this.adapter;
    }

    public ServerController createServer(boolean conn, String name, String ip, int port, String databaseName) {
        logger.trace("Creating server model: " + "conn->" + conn + "name->" + name + " ip->" + ip + " port->" + port + " DB->" + databaseName);
        this.serverController = new ServerController(new Server(conn, name, ip, port, databaseName));
        setMessageSender(serverController);
        logger.trace("OK");
        return this.serverController;
    }

    public ServerController createServer(Server server) {
        logger.trace("Setting server: " + server.getName());
        if (this.serverController == null) this.serverController = new ServerController(server);
        setMessageSender(serverController);
        logger.trace("OK");
        return this.serverController;
    }

    public ServerReceiver createServerReceiver() {
        logger.trace("Creating server receiver");
        this.serverReceiver = new ServerReceiver(7071, this);
        this.serverReceiver.setDaemon(true);
        setMessageSender(serverReceiver);
        getAdapter().statusProperty().addListener(new ChangeListener<Boolean>() {
            @Override
            public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
                if(newValue){
                    if(getServerReceiver()!= null) getServerReceiver().enable();
                }else{
                    if(getServerReceiver()!= null) {
                        getServerReceiver().disable();
                    }
                }
            }
        });
        return this.serverReceiver;
    }

    public SensorPanelPresenter createSensorPanel(Node container, String headerHexColor, SensorIcon icon, SensorController sensorController) throws LoadException {
        SensorPanelPresenter newPanel;
        try {
            newPanel = new SensorPanelPresenter(container);
            newPanel.loadView();
            if (headerHexColor != null)
                newPanel.setHeaderColor(headerHexColor);
            if (icon != null) {
                newPanel.setIcon(icon);
            } else {
                newPanel.setIcon(SensorIcon.UNKNOWN);
            }
            newPanel.addModel(sensorController);
            sensorController.setPanel(newPanel);
        } catch (IOException e) {
            throw new LoadException("Cannot initiate new sensor panel.Error while loading FXML file", e);
        }
        return newPanel;
    }

    public SensorController createSensor(Node container, String headerHexColor, SensorIcon icon, ObservableList<Value> values, boolean status, int id, String name, int battery, int signal, int refreshTime, Protocol protocol) throws LoadException {
        SensorController controller;
        SensorPanelPresenter newPanel;
        try {
            //create presenter
            newPanel = new SensorPanelPresenter(container);
            ((FlowPane) container).getChildren().add(0, newPanel.loadView());
            if (headerHexColor != null)
                newPanel.setHeaderColor(headerHexColor);
            if (icon != null) {
                newPanel.setIcon(icon);
            } else {
                newPanel.setIcon(SensorIcon.UNKNOWN);
            }
            controller = createSensor(values, status, id, name, battery, signal, refreshTime, protocol);
            newPanel.addModel(controller);
            controller.setPanel(newPanel);
        } catch (IOException e) {
            throw new LoadException("Cannot initiate new sensor panel.Error while loading FXML file", e);
        } catch (NullPointerException ne) {
            throw new LoadException("Cannot initiate new sensor panel.Error while creating SensorController", ne);
        }
        return controller;
    }

    public SensorController createSensor(ObservableList<Value> values, boolean status, int id, String name, int battery, int signal, int refreshTime, Protocol protocol) {
        Sensor newSensor = new Sensor(status, id, name, battery, signal, refreshTime, protocol);
        logger.debug("Adding values");
        newSensor.getValues().addAll(values);
        setValuesOffsets(values);
        newSensor.statusProperty().addListener(new ChangeListener<Boolean>() {
            @Override
            public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
                int activeCount = adapter.getActiveCount();
                if (newValue) {
                    adapter.setActiveCount(++activeCount);
                } else {
                    adapter.setActiveCount(--activeCount);
                }
            }
        });
        SensorController controller = new SensorController(this, newSensor);
        sensorControllers.add(controller);
        //listener for sensor activity; start sensor timer, if adapter is enabled and sensor is enabled
        controller.getModel().statusProperty().addListener(new ChangeListener<Boolean>() {
            @Override
            public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
                if (newValue && adapter.getStatus()) {
                    controller.setTimerRunning(true);
                } else {
                    controller.setTimerRunning(false);
                }
            }
        });
        //listener for adapter activity; start all active sensors timers, if adapter is enabled
        adapter.statusProperty().addListener(new ChangeListener<Boolean>() {
            @Override
            public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
                if (newValue && controller.getModel().getStatus()) {
                    controller.setTimerRunning(true);
                } else {
                    controller.setTimerRunning(false);
                }
            }
        });
        return controller;
    }

    public Scheduler createScheduler(Scheduler.Type type) {
        switch (type) {
            case DETAILED:
                this.scheduler = new DetailedScheduler(this);
                break;
            case PERFORMANCE:
                this.scheduler = new PerformanceScheduler(this);
                break;
        }
        //create scheduler
        this.scheduler.setDaemon(true);
        this.scheduler.start();
        startScheduler();
        logger.trace("Scheduler/" + String.valueOf(adapter.getId()) + " created and started");
        return this.scheduler;
    }

    public Document saveToXml(Document document) {
        //<adapter id="1312">
        Element adapterElement = document.getRootElement()
                .addElement("adapter")
                .addAttribute("id", String.valueOf(getAdapter().getId()))
                .addAttribute("name", getAdapter().getName())
                .addAttribute("protocol", String.valueOf(getAdapter().getProtocolVersion()))
                .addAttribute("registered", String.valueOf(getAdapter().getRegistered()))
                .addAttribute("firmware", String.valueOf(getAdapter().getFirmware()));
        getServerController().saveToXml(adapterElement);
        Element sensorsElement = adapterElement.addElement("sensors");
        for (SensorController sensorController : getSensorControllers()) {
            sensorController.saveToXML(sensorsElement);
        }
        return document;
    }

    public void setTrackServerResponse(boolean b) throws NullPointerException {
        if (getScheduler() == null)
            throw new NullPointerException("Adapter/" + getAdapter().getId() + " cannot set response tracking, scheduler is null");
        getScheduler().getResponseTracker().setEnabled(b);
    }

    public void setServerResponseTracker(ResponseTracker responseTracker) {
        if (getScheduler() == null)
            throw new NullPointerException("Adapter/" + getAdapter().getId() + " cannot set response tracking, scheduler is null");
        getScheduler().setResponseTracker(responseTracker);
    }

    public void setDumpServerResponse(boolean b) throws NullPointerException {
        if (getScheduler() == null)
            throw new NullPointerException("Adapter/" + getAdapter().getId() + " cannot set response dumping, scheduler is null");
        getScheduler().getResponseTracker().setDumpResponses(b);
    }

    public void setTrackMessages(boolean b) throws NullPointerException {
        if (getScheduler() == null)
            throw new NullPointerException("Adapter/" + getAdapter().getId() + " cannot set response dumping, scheduler is null");
        getLog().getMessageTracker().setEnabled(b);
    }

    public void bindSchedulerProcess(Adapter adapter, Scheduler scheduler) {
        adapter.statusProperty().addListener(new ChangeListener<Boolean>() {
            @Override
            public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
                if (newValue && scheduler != null) {
                    scheduler.startProcessing();
                } else if (!newValue && scheduler != null) {
                    scheduler.stopProcessing();
                }
            }
        });
    }

    public void bindRegisterMessage() {
        AdapterController me = this;
        me.getAdapter().statusProperty().addListener(new ChangeListener<Boolean>() {
            @Override
            public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
                if (me.isRegisterMessageSent()) return;
                if (newValue && !me.getAdapter().getRegistered()) {
                    me.sendRegisterMessage(me);
                }
            }
        });
    }

    public void sendRegisterMessage(AdapterController adapterController) {
        //if register message already exists, do nothing
        if(checkIfRegisterMessageExists()) return;
        //else create register message
        logger.trace("Adapter/" + adapterController.getAdapter().getId() + " creating register message.");
        adapterController.sendMessage(adapterController.toString() + " -> Registering adapter", //log message
                adapterController.getAdapter().getProtocol().buildRegisterMessage(adapterController), //socket message
                null, //not null if sensor sends message
                OutMessage.Type.REGISTER_ADAPTER //message type
        );
        adapterController.setRegisterMessageSent(true);
    }

    public OutMessage createRegisterMessage(){
        return new OutMessage(
                toString() + " -> reconnected to server sender", //log message
                getAdapter().getProtocol().buildRegisterMessage(this), //socket message
                null, //not null if sensor sends message
                OutMessage.Type.REGISTER_ADAPTER, //message type
                log.sent("Adapter/" + getAdapter().getId() + " -> trying to reconnect to server sender ")
        );
    }

    private boolean checkIfRegisterMessageExists(){
        if (messages == null || messages.size() == 0) return false;
        boolean found = false;
        for (OutMessage m : messages) {
            if (m.getType() == OutMessage.Type.REGISTER_ADAPTER) {
                found = true;
            }
        }
        return found;
    }

    public void restartValueGenerators(){
        if(getSensorControllers() == null) return;
        getSensorControllers().forEach(SensorController::restartValueGenerators);
    }

    public synchronized void clearScheduler(){
        if(messages != null) messages.clear();
        if(scheduler != null){
            scheduler.stopProcessing();
        }
    }

    public void startScheduler(){
        if(this.scheduler != null) scheduler.startProcessing();
    }

    public void stopScheduler(){
        if(this.scheduler != null) scheduler.stopProcessing();
    }

    public void addValues(Sensor sensor,ObservableList<Value> values){
        sensor.getValues().addAll(values);
    }

    public void createLog(){
        this.log = new AdapterLogger();
    }

    public void createLog(TabPane tabPane){
        this.log = new AdapterLogger(tabPane);
    }

    private void setValuesOffsets(ObservableList<Value> values){
        for (int i = 0; i < values.size(); i++) {
            Value tmpValue = values.get(i);
            int offset = 0;
            for(int j = 0; j < i; j++){
                if(tmpValue.getValueType() == values.get(j).getValueType())
                    offset++;
            }
            logger.trace("Setting offset for value: " + tmpValue.getName() + " -> " + "0x" + Integer.toHexString(offset));
            tmpValue.setOffset(offset);
        }
    }

    public Adapter getAdapter() {
        return adapter;
    }

    public ServerController getServerController() {
        return serverController;
    }

    public void setServerController(ServerController serverController) {
        logger.trace("Setting server: " + serverController.getModel().getName());
        this.serverController = serverController;
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

    public Scheduler getScheduler(){
        return this.scheduler;
    }

    public ListProperty<SensorController> getSensorControllersList() {
        return sensorControllers;
    }

    public ObservableList<SensorController> getSensorControllers(){
        return sensorControllers.get();
    }

    public boolean isRegisterMessageSent() {
        return registerMessageSent;
    }

    public void setRegisterMessageSent(boolean registerMessageSent) {
        this.registerMessageSent = registerMessageSent;
    }

    public boolean isSaved() {
        return saved;
    }

    public void setSaved(boolean saved) {
        this.saved = saved;
    }

    public ServerReceiver getServerReceiver() {
        return serverReceiver;
    }

    public void setServerReceiver(ServerReceiver serverReceiver) {
        this.serverReceiver = serverReceiver;
    }

    public MessageSender getMessageSender() {
        return messageSender;
    }

    public void setMessageSender(MessageSender messageSender) {
        this.messageSender = messageSender;
    }

    public String toString(){
        if(getAdapter() != null)
            return getAdapter().getName() + "/" + getAdapter().getId();
        else
            return "Unknown adapter";
    }
}
