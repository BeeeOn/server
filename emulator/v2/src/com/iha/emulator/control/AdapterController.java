package com.iha.emulator.control;

import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.communication.protocol.SetNewValue;
import com.iha.emulator.communication.server.MessageSender;
import com.iha.emulator.communication.server.OutMessage;
import com.iha.emulator.communication.server.ssl.ServerController;
import com.iha.emulator.communication.server.ssl.ServerReceiver;
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
 * Creates and controls adapter. Also contains all information gathered while simulation runs like unsent messages, scheduler
 * for processing these messages, server with which should adapter communicate, list of created sensors, etc.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class AdapterController {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(AdapterController.class);
    /** adapter model */
    private Adapter adapter;
    /** server controller containing server model */
    private ServerController serverController;
    /** scheduler processing messages for server*/
    private Scheduler scheduler;
    /** class providing connection to server */
    private MessageSender messageSender;
    /** class providing stable connection to server */
    private ServerReceiver serverReceiver = null;
    /** list created of sensors */
    private ListProperty<SensorController> sensorControllers = new SimpleListProperty<>(FXCollections.observableArrayList());
    /** logger */
    private AdapterLogger log;
    /** internet connection property */
    private BooleanProperty internetConnection;
    /** helper variable for determining, if register message was send */
    private boolean registerMessageSent = false;
    /** adapter saved to XML flag */
    private boolean saved = false;
    /** list of unsent messages for server */
    private ArrayList<OutMessage> messages = new ArrayList<>();

    /**
     * Creates new instance of adapter's controller. Initializes internet connection property.
     */
    public AdapterController() {
        this.internetConnection = new SimpleBooleanProperty(true);
    }

    /**
     * Enables adapter, so {@link com.iha.emulator.control.scheduler.Scheduler} can start to send messages to server.
     */
    public void enable() {
        logger.trace("Enabling adapter -> " + adapter.getId());
        adapter.setStatus(true);
    }

    /**
     * Disables adapter, so {@link com.iha.emulator.control.scheduler.Scheduler} is paused and no messages are sent
     * to server.
     */
    public void disable() {
        logger.trace("Disabling adapter -> " + adapter.getId());
        adapter.setStatus(false);
    }

    /**
     * Writes simple message to adapter's log.
     * @param logMessage message to be written in log
     */
    public void sendMessage(String logMessage) {
        log.log(logMessage);
    }

    /**
     * Adds new message for server to list of messages, so it can be sent to server. Also starts scheduler, if not running.
     * @param message new message for server
     */
    public void sendMessage(OutMessage message) {
        if (messages != null) {
            switch (message.getType()){
                case REGISTER_ADAPTER:
                    //if register message, put on top of the messages queue
                    messages.add(0,message);
                    break;
                case SENSOR_MESSAGE:
                    //if common sensor message, put at the end of the queue
                    messages.add(message);
                    break;
            }
            //if scheduler is not processing messages, start
            startScheduler();
        }
    }

    /**
     * Creates new instance of {@link com.iha.emulator.communication.server.OutMessage} for server and adds it to list
     * of messages ready to be sent. Also logs message to "To Be Sent" log and starts scheduler, if not running.
     * @param logMessage message to be written to adapter's log
     * @param message message for server in Dom4j XML format
     * @param senderController sensor, that is sending this message
     * @param type message type
     */
    public void sendMessage(String logMessage, Document message, SensorController senderController, OutMessage.Type type) {
        if (message != null) {
            Text unsent;
            switch (type) {
                case REGISTER_ADAPTER:
                    //log message to "To Be Sent" log
                    unsent = log.sent("Adapter/" + getAdapter().getId() + " -> trying to register");
                    //if register message, put on top of the messages queue
                    messages.add(0, new OutMessage(logMessage, message, senderController, type, unsent));
                    break;
                case SENSOR_MESSAGE:
                    //log message to "To Be Sent" log
                    if (senderController.isFullMessage()) {
                        unsent = log.sent("Adapter/" + getAdapter().getId() + " -> Sensor/" + senderController.getModel().getId() + " waiting to send data.");
                    } else {
                        unsent = log.sent("Sensor " + senderController.toString() + " waiting to send data.");
                    }
                    //if common sensor message, put at the end of the queue
                    messages.add(new OutMessage(logMessage, message, senderController, type, unsent));
                    break;
            }
            //if scheduler is not processing messages, start
            startScheduler();
        }
    }

    /**
     * Writes message to "Error" log.
     * @param message message to be written to "Error" log
     * @param t exception that caused this error
     * @param terminate <code>true</code> if {@link com.iha.emulator.utilities.logging.AdapterLogger} should notify
     *                  error handlers, <code>false</code> otherwise
     */
    public synchronized void sendError(String message, Throwable t, boolean terminate) {
        if (t != null) message = message + " -> " + t.getMessage();
        getLog().error(message, terminate);
    }

    /**
     * Find sensor by it's ID and tries to change it's value/s. If sensor is not found or cannot apply new value, exception
     * is thrown.
     * @param sensorId ID of a sensor, that is to change its value/s
     * @param newValues list of new values
     * @throws IllegalArgumentException sensor ID not found or cannot apply new value/s
     */
    public void changeValueOnSensor(int sensorId, ArrayList<SetNewValue> newValues) throws IllegalArgumentException {
        //find sensor controller for given ID
        SensorController sensorController = getSensorById(sensorId);
        if (sensorController == null)
            //sensor not found
            throw new IllegalArgumentException("Sensor with id \"" + sensorId + "\n doesn't exist");
        try {
            //iterate through new values and apply them
            for (SetNewValue newValue : newValues) {
                logger.trace("Trying to change value on sensor: " + sensorController.toString());
                sensorController.changeValue(newValue.getType(), newValue.getOffset(), newValue.getValue());
            }
        } catch (IllegalArgumentException e) {
            //cannot apply new value
            Platform.runLater(() -> log.error(e.getMessage(), false));
            logger.error(e.getMessage(), e);
        }
    }

    /**
     * Deletes sensor (given by ID) from this adapter
     * @param sensorId sensor to be deleted
     * @throws IllegalArgumentException sensor ID not found or cannot delete it
     */
    public void cleanSensor(int sensorId) throws IllegalArgumentException{
        Platform.runLater(()->deleteSensor(getSensorControllerById(sensorId)));
    }

    /**
     * Deletes sensors (given as list of sensor controllers) from this adapter.
     * @param sensorControllers list of sensors to be deleted
     */
    public void deleteSensors(ObservableList<SensorController> sensorControllers) {
        if (sensorControllers == null) return;
        for (Iterator<SensorController> it = sensorControllers.iterator(); it.hasNext(); ) {
            SensorController sensorController = it.next();
            deleteSensor(sensorController);
        }
        getSensorControllers().removeAll(sensorControllers);
        System.gc();
    }

    /**
     * Deletes sensor (given by sensor controller) from this adapter.
     * @param sensorController sensor to be deleted
     */
    public void deleteSensor(SensorController sensorController){
        String name;
        name = sensorController.toString();
        //clean after sensor
        removeSensorsAdaptersStatusListener(sensorController);
        logger.debug(name + " -> Deleting");
        //stop sensors timer
        if (sensorController.getTimerRunning()) sensorController.setTimerRunning(false);
        //delete GUI panel
        if (sensorController.getPanel() != null) {
            logger.trace(name + " -> removing panel from panel container");
            sensorController.getPanel().deletePanel();
        }
        //delete sensors unsent messages
        logger.trace(name + " -> removing unsent messages");
        removeUnsentMessages(sensorController);
        logger.trace(name + " -> removing from list");
        //delete sensor itself
        sensorController.delete();
        logger.debug(name + " -> OK");
    }

    /**
     * Deletes all sensors on this adapter and after adapter itself is terminated.
     */
    public void deleteAll() {
        logger.trace("Removing sensors");
        //delete sensors
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

    /**
     * Terminate adapter's scheduler
     */
    public void terminateScheduler() {
        logger.trace("Terminating scheduler");
        //clean unsent messages
        messages.clear();
        //terminate scheduler, but do not clean responses
        getScheduler().terminateScheduler(false);
        //interrupt scheduler's thread if needed
        if (getScheduler().isAlive()) getScheduler().interrupt();
        try {
            //wait for the scheduler to finish
            getScheduler().join();
        } catch (InterruptedException e) {
            logger.error("Cannot join " + getScheduler().getName());
        }
    }

    /**
     * Deletes adapter.
     */
    public void delete() {
        logger.trace("Removing sensors");
        deleteSensors(getSensorControllersList().get());
        messages = null;
        sensorControllers = null;
        log = null;
        serverController = null;
        scheduler = null;
    }

    /**
     * Clear responses from server
     */
    public void clearResponseTracker() {
        if (scheduler == null) return;
        scheduler.clearResponseTracker();
    }

    /**
     * Notifies adapter, that message given as parameter was successfully sent. Adapter will remove this message
     * from list of unsent messages and line in "To Be Sent" log is deleted.
     * @param message message that was successfully sent
     */
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

    /**
     * Creates new instance of {@link com.iha.emulator.models.Adapter} model from given parameters.
     * @param status <code>true</code> if adapter is enabled, <code>false</code> otherwise
     * @param id adapter's ID
     * @param registered <code>true</code> if adapter is registered on server, <code>false</code> otherwise
     * @param protocolVersion version of protocol used to communicate with server
     * @param firmware adapter's firmware version
     * @return new instance of adapter model
     */
    public Adapter createAdapter(boolean status, int id, boolean registered, Protocol.Version protocolVersion, double firmware) {
        logger.trace("Creating adapter model: " + "status->" + status + " id->" + id + " registered->" + registered + " protocol->" + protocolVersion + " firmware->" + firmware);
        this.adapter = new Adapter(status, id, registered, protocolVersion, firmware);
        logger.trace("OK");
        return this.adapter;
    }
    /**
     * Creates new instance of {@link com.iha.emulator.models.Adapter} model from given parameters.
     * @param name adapter's name in emulator
     * @param status <code>true</code> if adapter is enabled, <code>false</code> otherwise
     * @param id adapter's ID
     * @param registered <code>true</code> if adapter is registered on server, <code>false</code> otherwise
     * @param protocolVersion version of protocol used to communicate with server
     * @param firmware adapter's firmware version
     * @return new instance of adapter model
     */
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
    public ServerController createServer(boolean conn, String name, String ip, int port, String databaseName) {
        logger.trace("Creating server model: " + "conn->" + conn + "name->" + name + " ip->" + ip + " port->" + port + " DB->" + databaseName);
        this.serverController = new ServerController(new Server(conn, name, ip, port, databaseName));
        setMessageSender(serverController);
        logger.trace("OK");
        return this.serverController;
    }
    /**
     * Creates new instance of {@link com.iha.emulator.communication.server.ssl.ServerController} for {@link com.iha.emulator.models.Server}
     * model given as parameter.
     * @param server server model
     * @return new instance of server controller
     */
    public ServerController createServer(Server server) {
        logger.trace("Setting server: " + server.getName());
        if (this.serverController == null) this.serverController = new ServerController(server);
        setMessageSender(serverController);
        logger.trace("OK");
        return this.serverController;
    }

    /**
     * Creates new instance of {@link com.iha.emulator.communication.server.ssl.ServerReceiver} used to communicate with
     * server by stable connection. Also bind {@link com.iha.emulator.communication.server.ssl.ServerReceiver} to
     * adapter's status.
     * @return new instance of server receiver
     */
    public ServerReceiver createServerReceiver() {
        logger.trace("Creating server receiver");
        //create new instance
        this.serverReceiver = new ServerReceiver(this);
        //set as daemon
        this.serverReceiver.setDaemon(true);
        //set as primary message sender for this adapter
        setMessageSender(serverReceiver);
        //bind server receiver to adapter's status property
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
    /**
     * Creates new instance of {@link com.iha.emulator.ui.panels.sensor.SensorPanelPresenter} as a GUI representation
     * of sensor.
     * @param container container for sensor panel
     * @param headerHexColor sensor's panel color
     * @param icon sensor's panel icon
     * @param sensorController sensor, for which is panel created
     * @return new instance of panel for sensor
     * @throws LoadException cannot load FXML file for {@link com.iha.emulator.ui.panels.sensor.SensorPanelPresenter}
     */
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

    /**
     * Creates new instance of {@link com.iha.emulator.control.SensorController} from information given as parameters.
     * Also creates GUI representation {@link com.iha.emulator.ui.panels.sensor.SensorPanelPresenter} and model
     * {@link com.iha.emulator.models.Sensor} for new sensor.
     *
     * @param container container for sensor panel
     * @param headerHexColor sensor panel header color
     * @param icon sensor panel icon
     * @param values list of emulated values by new sensor
     * @param status connection to adapter status
     * @param id sensor ID
     * @param name sensor name
     * @param battery battery value
     * @param signal signal value
     * @param refreshTime refresh time in seconds
     * @param protocol instance of used protocol
     * @return new instance of sensor controller with created panel placed in given container
     * @throws LoadException cannot load sensor panel FXML file
     */
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
    /**
     * Creates new instance of {@link com.iha.emulator.control.SensorController} from information given as parameters and
     * creates model {@link com.iha.emulator.models.Sensor} for new sensor. Also sets offsets for given values, binds
     * sensor's status to adapter's active sensors count, binds sensor's status to sensor's timer and binds sensor's
     * status to adapter's status.
     *
     * @param values list of emulated values by new sensor
     * @param status connection to adapter status
     * @param id sensor ID
     * @param name sensor name
     * @param battery battery value
     * @param signal signal value
     * @param refreshTime refresh time in seconds
     * @param protocol instance of used protocol
     * @return new instance of sensor controller
     */
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
        controller.setAdapterStatusChangeListener((observable, oldValue, newValue) -> {
            if(controller == null || controller.getModel() == null) return;
            if (newValue && controller.getModel().getStatus()) {
                controller.setTimerRunning(true);
            } else {
                controller.setTimerRunning(false);
            }
        });
        adapter.statusProperty().addListener(controller.getAdapterStatusChangeListener());
        return controller;
    }

    /**
     * Creates new instance of class implementing {@link com.iha.emulator.control.scheduler.Scheduler} from given type.
     * Sets it's thread as daemon and starts it.
     * @param type type of scheduler
     * @return new instance of class implementing interface {@link com.iha.emulator.control.scheduler.Scheduler}
     */
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
    /**
     * Creates Dom4j XML element in given document. Saves all information about adapter and its sensors needed for later
     * loading of this adapter.
     *
     * @param document dom4j XML document
     * @return XML document with information about adapter and its sensors
     */
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

    /**
     * Bind adapter status to scheduler's processing. If adapter is disabled, scheduler stops processing.
     * @param adapter adapter to be bound
     * @param scheduler scheduler to be bound
     */
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
    /**
     * Binds sending of register message to adapter's status. If message hasn't been already sent when adapter is enabled,
     * message is created and send for processing.
     */
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
    /**
     * If register message doesn't exist, creates register message and adds it to adapter's unsent messages queue
     * @param adapterController adapter, for which is register message created
     */
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

    /**
     * Creates and returns register message
     * @return register message
     */
    public OutMessage createRegisterMessage(){
        return new OutMessage(
                toString() + " -> reconnected to server sender", //log message
                getAdapter().getProtocol().buildRegisterMessage(this), //socket message
                null, //not null if sensor sends message
                OutMessage.Type.REGISTER_ADAPTER, //message type
                log.sent("Adapter/" + getAdapter().getId() + " -> trying to reconnect to server sender ")
        );
    }

    /**
     * If scheduler instance exists, it is set to start processing unsent messages.
     */
    public void startScheduler(){
        if(this.scheduler != null) scheduler.startProcessing();
    }

    /**
     * If scheduler instance exists, it is set to stop processing unsent messages.
     */
    public void stopScheduler(){
        if(this.scheduler != null) scheduler.stopProcessing();
    }

    /**
     * Crates new instance of {@link com.iha.emulator.utilities.logging.AdapterLogger} in container given as parameter.
     * @param tabPane container for logger
     */
    public void createLog(TabPane tabPane){
        this.log = new AdapterLogger(tabPane);
    }

    /**
     * Removes already created status listener from given sensor. Used when removing sensor, so application won't hold
     * unnecessary information.
     * @param sensorController sensor, for which should be status listener removed.
     */
    private void removeSensorsAdaptersStatusListener(SensorController sensorController){
        if(sensorController.getAdapterStatusChangeListener() == null) return;
        adapter.statusProperty().removeListener(sensorController.getAdapterStatusChangeListener());
    }

    /**
     * Removes all unsent messages by given sensor from adapter's unsent messages list. Used when removing sensor.
     * @param sensorController sensor, for which should be unsent messages removed from list
     */
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

    /**
     * Returns instance of {@link com.iha.emulator.control.SensorController} with given sensor ID.
     * @param sensorId ID of sensor to be found
     * @return found sensor controller or null
     */
    private SensorController getSensorById(int sensorId) {
        for (SensorController sensorController : sensorControllers) {
            if (sensorController.getModel().getId() == sensorId) return sensorController;
        }
        return null;
    }
    /**
     * Returns instance of {@link com.iha.emulator.control.SensorController} with given sensor ID.
     * @param id ID of sensor to be found
     * @return found sensor controller or null
     */
    private SensorController getSensorControllerById(int id)throws IllegalArgumentException{
        for (SensorController s : getSensorControllers()){
            if(s.getModel().getId() == id) return s;
        }
        throw new IllegalArgumentException("Sensor id \"" + id + "\" not found");
    }

    /**
     * Method checks, if register message for this adapter was already created and added to unsent messages list
     * @return <code>true</code> if register message already exists,<code>false</code> otherwise
     */
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

    /**
     * Method sets offset for values given in list as parameter.
     * @param values list of values, for which should be offsets set
     */
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

    /**
     * Method sets, if {@link com.iha.emulator.utilities.watchers.ResponseTracker} is enabled
     * @param b <code>true</code> if response tracker should be enabled, <code>false</code> otheriwse
     * @throws NullPointerException if no scheduler with response tracker exists
     */
    public void setTrackServerResponse(boolean b) throws NullPointerException {
        if (getScheduler() == null)
            throw new NullPointerException("Adapter/" + getAdapter().getId() + " cannot set response tracking, scheduler is null");
        getScheduler().getResponseTracker().setEnabled(b);
    }

    /**
     * Sets response tracker for scheduler
     * @param responseTracker instance of response tracker
     */
    public void setServerResponseTracker(ResponseTracker responseTracker) {
        if (getScheduler() == null)
            throw new NullPointerException("Adapter/" + getAdapter().getId() + " cannot set response tracking, scheduler is null");
        getScheduler().setResponseTracker(responseTracker);
    }

    /**
     * Sets if response tracker should remove response after it is logged by adapter's logger
     * @param b <code>true</code> if responses should be dumped, <code>false</code> otheriwse
     * @throws NullPointerException if no scheduler with response tracker exists
     */
    public void setDumpServerResponse(boolean b) throws NullPointerException {
        if (getScheduler() == null)
            throw new NullPointerException("Adapter/" + getAdapter().getId() + " cannot set response dumping, scheduler is null");
        getScheduler().getResponseTracker().setDumpResponses(b);
    }

    /**
     * Gets model of adapter
     * @return model of adapter
     */
    public Adapter getAdapter() {
        return adapter;
    }

    /**
     * Gets server controller
     * @return server controller
     */
    public ServerController getServerController() {
        return serverController;
    }

    /**
     * Sets server controller
     * @param serverController server controller
     */
    public void setServerController(ServerController serverController) {
        logger.trace("Setting server: " + serverController.getModel().getName());
        this.serverController = serverController;
    }

    /**
     * Gets state of internet connection
     * @return state of internet connection
     */
    public boolean getInternetConnection() {
        return internetConnection.get();
    }

    /**
     * Gets state of internet connection property, that can be bound
     * @return state of internet connection property
     */
    public BooleanProperty internetConnectionProperty() {
        return internetConnection;
    }

    /**
     * Sets state of internet connection
     * @param internetConnection state of internet connection
     */
    public void setInternetConnection(boolean internetConnection) {
        this.internetConnection.set(internetConnection);
    }

    /**
     * Gets adapter's logger
     * @return adapter's logger
     */
    public AdapterLogger getLog() {
        return log;
    }

    /**
     * Sets adapter's logger
     * @param log adapter's logger
     */
    public void setLog(AdapterLogger log) {
        this.log = log;
    }

    /**
     * Gets adapter's scheduler
     * @return adapter's scheduler
     */
    public Scheduler getScheduler(){
        return this.scheduler;
    }

    /**
     * Gets property list of sensors controllers
     * @return property list of sensors controllers
     */
    public ListProperty<SensorController> getSensorControllersList() {
        return sensorControllers;
    }

    /**
     * Gets list of sensors controllers
     * @return list of sensors controllers
     */
    public ObservableList<SensorController> getSensorControllers(){
        return sensorControllers.get();
    }

    /**
     * Gets flag, if register message has been sent
     * @return flag, if register message has been sent
     */
    public boolean isRegisterMessageSent() {
        return registerMessageSent;
    }

    /**
     * Sets flag, if register message has been sent
     * @param registerMessageSent flag, if register message has been sent
     */
    public void setRegisterMessageSent(boolean registerMessageSent) {
        this.registerMessageSent = registerMessageSent;
    }

    /**
     * Gets flag, if adapter's information has been saved
     * @return flag, if adapter's information has been saved
     */
    public boolean isSaved() {
        return saved;
    }

    /**
     * Sets flag, if adapter's information has been saved
     * @param saved flag, if adapter's information has been saved
     */
    public void setSaved(boolean saved) {
        this.saved = saved;
    }

    /**
     * Sets adapter's server receiver
     * @return adapter's server receiver
     */
    public ServerReceiver getServerReceiver() {
        return serverReceiver;
    }

    /**
     * Sets adapter's message sender
     * @param messageSender adapter's message sender
     */
    public void setMessageSender(MessageSender messageSender) {
        this.messageSender = messageSender;
    }

    /**
     * Method generates unique name from adapter's name and ID
     * @return unique name from adapter's name and ID
     */
    public String toString(){
        if(getAdapter() != null)
            return getAdapter().getName() + "/" + getAdapter().getId();
        else
            return "Unknown adapter";
    }
}
