package com.smarthome.emulator.simulations.basis;

import com.smarthome.emulator.simulations.*;
import com.smarthome.emulator.simulations.Error;
import com.smarthome.emulator.simulations.panels.AdapterPanel;
import com.smarthome.emulator.simulations.panels.SensorPanel;
import com.smarthome.emulator.simulations.panels.ServerPanel;
import org.dom4j.Document;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;

import javax.swing.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

/**
 * Stores information about adapter. Sets message for server. Stores messages waiting to be sent for
 * {@link com.smarthome.emulator.simulations.basis.Scheduler}, controls scheduler, controls {@link Sensor}'s
 * timers, sets up server and holds list of {@link Sensor}s.
 * Extends {@link java.lang.Thread}.
 */
public class Adapter extends Thread{
    /** Termination indicator */
    private boolean terminate = false;
    private boolean registered = false;
    /** Activity indicator */
    private boolean active;
    /** Adapter name */
    private String name;
    /** Adapter Id */
    private long id;
    /** Adapter firmware */
    private short firmware;
    /** Internet connection indicator */
    private boolean internetConnection = true;
    /** Number of active(connected) sensors */
    private int activeSensors = 0;
    /** Number of adapter's registered sensors */
    private int registeredSensors = 0;
    /** List of sensors. Instances of class {@link Sensor} */
    private ArrayList<Sensor> sensors = new ArrayList<Sensor>();
    /** Communication protocol between server and adapter */
    private float serverProtocol;
    /** Instance od class {@link com.smarthome.emulator.simulations.basis.Server} with server information */
    private Server server;
    /** Instance of class {@link com.smarthome.emulator.simulations.panels.AdapterPanel},null if no panel */
    private AdapterPanel panel = null;
    /** Instance of class {@link com.smarthome.emulator.simulations.basis.Scheduler} */
    private Scheduler scheduler;
    /** Instance of class {@link com.smarthome.emulator.simulations.basis.Logger} */
    private Logger logger;
    /** Unsent messages list. Each message is class {@link com.smarthome.emulator.simulations.basis.Message} */
    private List<Message> messages = new ArrayList<Message>();
    private int sensorsCount = 0;
    /** Time for thread sleep, when adapter thread is not active */
    static final int SLEEP_TIME = 500;
    /** Size of protocol message static fields in bytes */
    static final int PROTOCOL_STATIC_SIZE = 39;
    /** Adapter firmware version, for emulator always 0 */
    static final short FIRMWARE_VERSION = 0;
    /**
     * Creates instance of this class and class {@link com.smarthome.emulator.simulations.basis.Scheduler}. Used when all adapters have one <code>logger</code>.
     * @param logger adapter's logger, not null
     */
    public Adapter(Logger logger){
        this.firmware = FIRMWARE_VERSION;
        this.logger = logger;
        scheduler = new Scheduler(logger,this);
    }
    /**
     * Assigns panel and logger to new instance of this class, creates instance of class {@link com.smarthome.emulator.simulations.basis.Scheduler}.
     * Used when every adapter has own  <code>panel</code> and  <code>logger</code>.
     * @param panel adapter's panel,not null
     * @param logger adapter's logger, not null
     */
    public Adapter(AdapterPanel panel,Logger logger) {
        this(logger);
        this.panel = panel;
    }
    /**
     * Used when all adapters share one {@link com.smarthome.emulator.simulations.panels.ServerPanel}.
     * Sends server parameters to <code>scheduler</code>.
     * @param panel panel showing server information, not null
     * @param host server connection hostname, null when <code>ip</code> is not null
     * @param ip server connection ip address, null when <code>host</code> is not null
     * @param port server connection port, not null
     */
    public void setServerParameters(ServerPanel panel,String host,String ip,int port){
        scheduler.setServerParameters(panel,host,ip,port);
    }
    /**
     * Used when every <code>adapter</code> has own {@link com.smarthome.emulator.simulations.panels.ServerPanel}
     * Sends server parameters to <code>scheduler</code>.
     * @param host server connection hostname, null when <code>ip</code> is not null
     * @param ip server connection ip address, null when <code>host</code> is not null
     * @param port server connection port, not null
     */
    public void setServerParameters(String host,String ip,int port){
        scheduler.setServerParameters(panel.getServerPanel(),host,ip,port);
    }
    /**
     * Starts <code>scheduler</code>'s thread and starts processing messages.
     * <p>
     * Shows errors:
     * <ul>
     *     <li><code>SERVER_NOT_CREATED</code> when server is <code>null</code></li>
     * </ul>
     * </p>
     */
    public void initScheduler(){
        if(scheduler.getServer() != null){
            server = scheduler.getServer();
            scheduler.start();
            scheduler.startProcessing();
        }else{
            logger.writeErrorMessage(Error.SERVER_NOT_CREATED.getMessage());
        }

    }
    /**
     * Ancestor class {@link java.lang.Thread} implementation. Runs until <code>terminate</code> is set to <code>true</code>.
     * When adapter is not active, sets thread to sleep for {@link com.smarthome.emulator.simulations.basis.Adapter#SLEEP_TIME}.
     */
    public void run(){
        while (!terminate){
            if(!this.isActive()){
                stopSensorsTimers();
                try {
                    Thread.sleep(SLEEP_TIME);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }
    /**
     * Adds new instance of class {@link Sensor} to list of sensors. Registers sensor for this adapter.
     * Used when sensor has GUI own GUI panel
     * @param panel sensor's panel for displaying information about sensor
     */
    public void addSensor(int id,SensorPanel panel){
        sensors.add(new Sensor(this,panel,id));
        registerSensor();
    }
    /**
     * Adds new instance of class {@link Sensor} to list of {@link com.smarthome.emulator.simulations.basis.Adapter#sensors}.
     * Registers sensor for adapter. Sets <code>Sensor</code>'s {@link Sensor#fullMessage} to write full messages.
     * If sensor initialization is successful, activates this sensor.
     * @param properties
     */
    public void addSensor(Properties properties){
        Sensor newSensor = new Sensor(this, PerformanceSimulation.getSensorsCount(),properties);
        newSensor.setFullMessage(true);
        sensors.add(newSensor);
        if(newSensor.isInitSuccess()){
            PerformanceSimulation.incrementSensorsCount();
            registerSensor();
            //connect sensor to adapter
            newSensor.setActivity(true);
            sendMessage("Sensor: " + newSensor.getSensorIdAsIp() + " init successful!");
        }else{
            sensors.remove(newSensor);
            sendMessage("Sensor: " + newSensor.getSensorIdAsIp() + " init unsuccessful");
        }

    }
    /**
     * Creates {@link java.nio.ByteBuffer}, writes adapter's information to it according to adapter -> server protocol and returns it
     * to sensor.
     * @param valuesSize (number of sensors * (long size in bytes + 2 bytes for sensor type)) - maximum possible sensor values message size.
     * @return buffer filled with adapter information
     */
    public Element createAdapterHeader(int valuesSize){
        //----------------- Byte version
        /*ByteBuffer messageHeader = ByteBuffer.allocate(PROTOCOL_STATIC_SIZE + valuesSize);
        //message length - only header for now +2B
        messageHeader.putShort((short) 0);
        //adapter ID +8B
        messageHeader.putLong(getAdapterId());
        //firmware version +2B
        messageHeader.putShort(getFirmware());
        //server communication protocol +2B
        messageHeader.putShort(getServerProtocol());
        //unix timestamp +8B
        messageHeader.putLong((System.currentTimeMillis()/1000L));
        return messageHeader;*/
        //----------------- END Byte version
        Document document = DocumentHelper.createDocument();
        Element root = document.addElement("adapter_server")
                .addAttribute("protocol_version",String.valueOf(getServerProtocol()))
                .addAttribute("state","data")
                .addAttribute("adapter_id","0x" + Long.toHexString(getAdapterId()))
                .addAttribute("fw_version",String.valueOf(getFirmware()))
                .addAttribute("time",String.valueOf((System.currentTimeMillis()/1000L)));
        return root;
    }

    public static int safeLongToInt(long l) {
        if (l < Integer.MIN_VALUE || l > Integer.MAX_VALUE) {
            throw new IllegalArgumentException
                    (l + " cannot be cast to int without changing its value.");
        }
        return (int) l;
    }
    
    /**
     * Stops all timers for sensor's in {@link com.smarthome.emulator.simulations.basis.Adapter#sensors} list.
     */
    public void stopSensorsTimers(){
        Timer timer;
        for(int i = 0; i < sensors.size(); i++ ){
            timer = sensors.get(i).getTimer();
            if(timer.isRunning()){
                sensors.get(i).getTimer().stop();
            }
        }
    }
    /**
     * Starts all timers for sensor's in {@link com.smarthome.emulator.simulations.basis.Adapter#sensors} list.
     */
    public void startSensorTimers(){
        for(int i = 0; i < sensors.size(); i++ ){
            if(sensors.get(i).isActive())
                sensors.get(i).getTimer().start();
        }
    }
    /**
     * sets if history of sensor's values should be stored
     */
    public void setStoreValuesHistory(){
        if(sensors.size()!= 0){
            for(Sensor sensor : sensors){
                for(Value value : sensor.getValues()){
                    value.setStoreHistory(false);
                }
            }
        }
    }
    /**
     * If adapter has {@link com.smarthome.emulator.simulations.basis.Adapter#panel}, refreshes adapter's information.
     */
    public void updateDetails(){
        if(panel!= null) panel.getDetailsPanel().refreshTable(this);
    }
    /**
     * When message is successfully sent, it is removed from {@link com.smarthome.emulator.simulations.basis.Adapter#messages} list
     * and line in "To be sent" log is removed.
     */
    public void messageSuccessfullySent(){
        if(messages.size() != 0){
            messages.remove(0);
        }
        logger.removeFirstLineFromToBeSent();
    }
    /**
     * Writes message to logger, doesn't sent any messages to server.
     * @param logMsg message to be written to log
     */
    public synchronized void sendMessage(String logMsg){
        logger.writeLogMessage(logMsg,0);
    }
    /**
     * Adds new {@link com.smarthome.emulator.simulations.basis.Message} to {@link com.smarthome.emulator.simulations.basis.Adapter#messages} list for scheduler to process.
     * Writes <code>logMsg</code> to adapter's log.
     * @param logMsg message to be written to log
     * @param msg protocol correct message, that will be sent to server
     * @param sender sensor, that sent the message
     */
    public synchronized void sendMessage(String logMsg,Document msg,Sensor sender){
        if(msg != null){
            messages.add(new Message(logMsg,msg,sender));
            if(sender != null){
                if(!sender.isFullMessage()){
                    logger.writeToBeSentMessage("Sensor " + sender.getName()+ " " + sender.getSensorIdAsIp() + " waiting to send data.");
                }else{
                    logger.writeToBeSentMessage("Adapter/" + getAdapterId() + " -> Sensor/" + sender.getSensorIdAsIp() + " waiting to send data.");
                }
            }else{
                logger.writeToBeSentMessage("Adapter/" + getAdapterId() + " -> trying to register");
            }
            scheduler.startProcessing();
        }
    }
    /**
     * Stops {@link com.smarthome.emulator.simulations.basis.Adapter#scheduler} from processing more messages
     */
    public void stopScheduler(){
        scheduler.stopProcessing();
    }
    /**
     * Starts {@link com.smarthome.emulator.simulations.basis.Adapter#scheduler}'s message processing
     */
    public void startScheduler(){
        scheduler.startProcessing();
    }
    /**
     * When sensor has unsent messages, removes them.
     * @param sensor sensor, which messages should be removed
     */
    public void deleteMessagesFromSensor(Sensor sensor){
        for(Message message : messages){
            if(message.sender.equals(sensor)){
                messages.remove(message);
            }
        }
    }
    /**
     * Delete adapter and all it's components
     */
    public void delete(){
        //stop timers
        stopSensorsTimers();
        //delete messages
        messages.clear();
        //end scheduler thread
        setInternetConnection(false);
        scheduler.terminateScheduler();
        scheduler.startProcessing();
        /*try {
            scheduler.join();
        } catch (InterruptedException e) {
            logger.writeErrorMessage(Error.SCHEDULER_TERMINATE.getMessage() + "/" + getAdapterId());
        }*/
        //System.out.println("TERMINATED scheduler: " + getAdapterId());
        //delete all sensors
        sensors.clear();
        logger.clearAllLogs();
        terminateAdapter();
    }
    /**
     * Deletes sensor of given id. If this sensor has {@link com.smarthome.emulator.simulations.panels.SensorPanel}, returns it.
     * @return sensor's panel, or null if sensor doesn't have panel.
     */
    public SensorPanel deleteSensor(int id){
        scheduler.stopProcessing();
        for(int i = 0;i <sensors.size();i++){
            Sensor sensor = sensors.get(i);
            if(sensor.getSensorId() == id){
                if(sensor.isActive()){
                    deactivateSensor();
                }
                //deactivate sensor
                sensor.setActivity(false);
                //delete unsent messages from this sensor
                for( int j = 0; j < messages.size();j++){
                    if(Integer.valueOf(messages.get(j).sender.getSensorId()).equals(Integer.valueOf(sensor.getSensorId()))){
                        messages.remove(j);
                    }
                }
                //stop sensor timer
                sensor.getTimer().stop();
                //remove from sensor list
                sensors.remove(sensor);
                scheduler.startProcessing();
                return sensor.getPanel();
            }
        }
        scheduler.startProcessing();
        return null;
    }
    public boolean equalsId(long id){
        return getAdapterId() == id;
    }
    public Sensor findSensor(int id){
        for(Sensor sensor : sensors){
            if(sensor.equalsId(id)){
                return sensor;
            }
        }
        return null;
    }
    /**
     * Terminates adapter's thread
     */
    public void terminateAdapter(){
        this.terminate = true;
    }
    /**
     * Register new sensor for adapter
     */
    public void registerSensor(){
        this.setRegisteredSensors(getRegisteredSensors()+1);
    }
    /**
     * Unregister new sensor from adapter
     */
    public void unregisterSensor(){
        this.setRegisteredSensors(getRegisteredSensors()-1);
    }
    /**
     * Increase number of active sensors
     */
    public void activateSensor(){
        this.setActiveSensors(getActiveSensors()+1);
    }
    /**
     * Decrease number of active sensors
     */
    public void deactivateSensor(){
        this.setActiveSensors(getActiveSensors()-1);
    }
    /**
     * Returns next unprocessed message
     * @return message to be processed
     */
    public Message getNextMessage(){
        if(!messages.isEmpty())
            return messages.get(0);
        else
            return null;
    }

    private Document createRegisterMessage(){
        Document document = DocumentHelper.createDocument();
        Element root = document.addElement("adapter_server")
                .addAttribute("protocol_version",String.valueOf(getServerProtocol()))
                .addAttribute("state","register")
                .addAttribute("adapter_id","0x" + Long.toHexString(getAdapterId()))
                .addAttribute("fw_version",String.valueOf(getFirmware()))
                .addAttribute("time",String.valueOf((System.currentTimeMillis()/1000L)));
        root.addElement("tmp");
        return document;
    }
    //region Getters and Setters
    /**
     * Returns adapter's activity status
     * @return <code>true</code> if adapter is active, else <code>false</code>
     */
    public boolean isActive() {
        return active;
    }
    /**
     * Sets adapter's activity to given value and updates adapter's detail panel. According to value starts or stops
     * {@link com.smarthome.emulator.simulations.basis.Adapter#scheduler}'s processing
     * @param b value of adapter's activity
     */
    public void setActivity(boolean b){
        this.active = b;
        updateDetails();
        if(this.active){
            if(!isRegistered()){
                System.out.println("Adding register message to queue");
                sendMessage("Registering adapter",createRegisterMessage(),null);
                registered = true;
            }
            startSensorTimers();
            scheduler.startProcessing();
        }else {
            scheduler.stopProcessing();
        }
    }
    /**
     * Returns adapter's id
     * @return adapter's id
     */
    public boolean isRegistered(){
        return this.registered;
    }

    public void setRegistered(boolean b){
        this.registered = b;
    }

    public int getMessagesCount(){
        return this.messages.size();
    }
    public long getAdapterId() {
        return id;
    }
    /**
     * Sets adapter's id
     * @param id adapter's id
     */
    public void setAdapterId(long id) {
        this.id = id;
        updateDetails();
    }
    /**
     * Returns number of active sensors
     * @return number of active sensors
     */
    public int getActiveSensors() {
        return activeSensors;
    }

    /**
     * Sets number of active sensors
     * @param activeSensors number of active sensors
     */
    public void setActiveSensors(int activeSensors) {
        this.activeSensors = activeSensors;
        updateDetails();
    }

    /**
     * Returns number of registered sensors
     * @return number of registered sensors
     */
    public int getRegisteredSensors() {
        return registeredSensors;
    }

    /**
     * Sets number of registered sensors. If there exists adapter's details panel, updates it.
     * @param registeredSensors
     */
    public void setRegisteredSensors(int registeredSensors) {
        this.registeredSensors = registeredSensors;
        updateDetails();
    }

    /**
     * Returns version of protocol between adapter and server
     * @return protocol version
     */
    public float getServerProtocol() {
        return serverProtocol;
    }

    /**
     * Sets version of protocol between adapter and server
     * @param serverProtocol
     */
    public void setServerProtocol(float serverProtocol) {
        this.serverProtocol = serverProtocol;
        updateDetails();
    }

    /**
     * Returns internet connection indicator
     * @return <code>true</code> if there is internet connection, <code>false</code> otherwise
     */
    public boolean getInternetConnection(){
        return internetConnection;
    }

    /**
     * Sets internet connection indicator
     * @param b
     */
    public void setInternetConnection(boolean b){
        this.internetConnection = b;
    }

    /**
     * Returns adapter's name
     * @return adapter's name
     */
    public String getAdapterName(){
        return this.name;
    }

    /**
     * Sets adapter's name
     * @param name name of adapter
     */
    public void setAdapterName(String name){
        this.name = name;
        panel.setAdapterName(this.name);
    }

    /**
     * Returns number of sensors registered on adapter
     * @return number of sensors registered on adapter
     */
    public int getSensorsCount(){
        return sensorsCount;
    }

    /**
     * Returns adapter's logger
     * @return adapter's logger
     */
    public Logger getLogger(){
        return this.logger;
    }

    /**
     * Returns list of adapter's sensors
     * @return list of adapter's sensors
     */
    public ArrayList<Sensor> getSensors(){
        return this.sensors;
    }

    /**
     * Returns server instance
     * @return server instance
     */
    public Server getServer(){
        return this.server;
    }

    /**
     * Returns firmware version
     * @return firmware version
     */
    public short getFirmware(){
        return this.firmware;
    }
    //endregion
}
