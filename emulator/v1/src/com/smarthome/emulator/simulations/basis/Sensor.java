package com.smarthome.emulator.simulations.basis;

import com.smarthome.emulator.Emulator;
import com.smarthome.emulator.simulations.Error;
import com.smarthome.emulator.simulations.forms.AddNewSensor;
import com.smarthome.emulator.simulations.panels.SensorPanel;
import org.dom4j.*;
import org.dom4j.io.OutputFormat;
import org.dom4j.io.XMLWriter;
import org.xml.sax.EntityResolver;

import javax.swing.*;
import javax.swing.Timer;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.io.Writer;
import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;
import java.nio.charset.Charset;
import java.text.DecimalFormat;
import java.util.*;

/**
 * Stores information about sensor and it's values, values history, creates messages for server
 * and sends them to {@link com.smarthome.emulator.simulations.basis.Adapter}, generates new values
 */
public class Sensor {
    /** instance of this class */
    private Sensor me;
    /** max number of values for one sensor */
    private static final int MAX_VALUES_NUMBER = 5;
    /** sensor activity indicator */
    private boolean active = false;
    /** sensor id */
    private int id;
    /** sensor  name */
    private String name;
    /** list of values */
    private ArrayList<Value> values = new ArrayList<Value>();
    /** list of last values */
    private ArrayList<Object> lastValues = new ArrayList<>();
    /** sensor's signal */
    private short signal;
    /** interval for sending new messages */
    private int refreshRate;
    /** sensor's battery */
    private short battery;
    /** GUI panel showing sensor information */
    private SensorPanel panel;
    /** communication protocol between sensor and adapter */
    private float protocol;
    /** initializing value */
    private byte initializeValue = (byte)0xFF;
    /** information about sensor used for creating sensor */
    private Properties defaultProperties;
    /** timer used for generating new values and sending messages to adapter*/
    private Timer sendMessageTimer;
    /** adapter owning this sensor */
    private Adapter adapter;
    /** if should be generated new value */
    private boolean runValueGenerator = false;
    /** for GUI sensor version resource image path */
    private String iconPath;
    /** for GUI sensor version, sensor head label color */
    private String backgroundColorString;
    /** for GUI sensor version, text color */
    private String fontColorString;
    /** for GUI sensor version, sensor head label text color */
    private String lblFontColorString;
    /** if initialising of sensor successful or not */
    private boolean initSuccess = true;
    private boolean pureActuator = false;
    /** full message indicator;full message contains adapter id, sensor id;
     * short message contains sensor name and id
     */
    private boolean fullMessage = false;
    /** help variable */
    public static final boolean DEBUG =true;
    /** actuator number according to protocol */
    private static final short ACTUATOR_TYPE_NUMBER = 4;
    private static final short ACTUATORS_SENSOR_TYPE_NUMBER = 3;

    private static final int MESSAGE_POS_BEFORE_SENSOR_ID = 22;
    private static final int MESSAGE_POS_BEFORE_VALUE_COUNT = 37;

    private static final String PROTOCOL = "0.1";
    /**
     * Creates new sensor by parsing information about sensor from <code>properties</code> given as parameter.
     * Also initializes, not starts, timer.
     * Constructor used for non-GUI purposes, ignores all sensors GUI properties.
     * @param adapter adapter that owns this sensor
     * @param id new sensor's id
     * @param properties information about sensor
     */
    public Sensor(Adapter adapter,int id,Properties properties){
        adapter.sendMessage("Trying to create sensor: " + id);
        this.me = this;
        this.adapter = adapter;
        this.defaultProperties = properties;
        this.name = String.valueOf(id);
        this.id = id;
        this.panel = null;
        //parse properties
        this.refreshRate = Integer.valueOf(defaultProperties.getProperty("refreshRate", "7"));
        //for safety reasons, so timer wouldn't be set to 0
        if(this.refreshRate == 0){
            this.refreshRate = 5;
        }
        this.signal = Short.valueOf(defaultProperties.getProperty("signal","100"));
        this.battery = Short.valueOf(defaultProperties.getProperty("battery","100"));
        this.protocol = Short.valueOf(defaultProperties.getProperty("protocol","0.1"));
        this.iconPath = null;
        this.backgroundColorString = null;
        this.fontColorString = null;
        this.lblFontColorString = null;
        parseValues();
        initTimer();
    }
    /**
     * Creates new sensor by parsing information about sensor from <code>properties</code> stored in <code>panel</code>.
     * parameter. Also initializes, not starts, timer.
     * Constructor used for GUI purposes.
     * @param adapter adapter that owns this sensor
     * @param panel GUI panel for showing sensor information
     * @param id new sensor's id
     */
    public Sensor(Adapter adapter,SensorPanel panel,int id) {
        this.me = this;
        this.adapter = adapter;
        this.defaultProperties = panel.getDefaultProperties();
        this.name = defaultProperties.getProperty("name","sensor/actor");
        this.id = id;
        this.panel = panel;
        this.panel.setSensor(this);
        //parse properties
        this.refreshRate = Integer.valueOf(defaultProperties.getProperty("refreshRate", "7"));
        this.signal = Short.valueOf(defaultProperties.getProperty("signal","100"));
        this.battery = Short.valueOf(defaultProperties.getProperty("battery","100"));
        this.protocol = Float.valueOf(defaultProperties.getProperty("protocol","0.1"));
        this.iconPath = defaultProperties.getProperty("icon","/img/lighbulb.png");
        //TODO add Alpha to colors
        this.backgroundColorString = defaultProperties.getProperty("backgroundColor","255,224,55");
        this.fontColorString = defaultProperties.getProperty("fontColor","130,130,130");
        this.lblFontColorString = defaultProperties.getProperty("lblFontColor","68,68,68");
        parseValues();
        initTimer();
    }

    public static long getUnsignedInt(int x) {
        return x & 0x00000000ffffffffL;
    }
    /**
     * Appends sensor's information to message according to adapter <-> server communication
     * protocol after adapter information given as parameter
     * @return full message containing adapter and sensor information
     */
    public Document buildSensorMessage(Element rootElement){
        //add device tag
        Element deviceElement = rootElement.addElement("device")
                .addAttribute("id", getHexaSensorId());
        //add debug tag
        deviceElement.addElement("debug")
                .addAttribute("protocol_version",String.valueOf(getProtocol()))
                .addAttribute("fw_version","0.1");
        //add battery tag
        deviceElement.addElement("battery")
                .addAttribute("value",String.valueOf(getBattery()));
        //add signal tag
        deviceElement.addElement("rssi")
                .addAttribute("value",String.valueOf(getSignal()));
        //add values tag
        Element valuesElement = deviceElement.addElement("values")
                .addAttribute("count",String.valueOf(values.size()));
        //add values
        for(Value sensorValue : values){
            //insert value
            Object value = sensorValue.getValue();
            if(value instanceof Float){
                //message.putInt(Integer.valueOf(Math.round((Float)value * 100)));
                DecimalFormat format = new DecimalFormat("0.##");
                Double newValue = Double.valueOf((Float) value);
                valuesElement.addElement("value")
                        .addAttribute("type","0x" + String.valueOf(Integer.toHexString(sensorValue.getType().intValue())))
                        .addText(String.valueOf((Float)value));

            }
        }
        if(DEBUG){
            /*XMLWriter writer;
            OutputFormat format = OutputFormat.createPrettyPrint();
            try {
                writer = new XMLWriter( System.out, format );
                writer.write( rootElement.getDocument() );
            } catch (UnsupportedEncodingException e) {
                System.err.println("Cannot create XML for output message debug");
            } catch (IOException e) {
                System.err.println("Cannot write to debug output XML");
            }*/

        }
        return rootElement.getDocument();


        //----------------- Byte Version ----------------------
        /*//move after adapter header
        message.position(MESSAGE_POS_BEFORE_SENSOR_ID);
        //----------------- 18B -----------------
        //sensor id +4B
        message.putInt(getSensorId());
        //adapter id in int +4B
        message.putInt(Adapter.safeLongToInt(adapter.getAdapterId()));
        //initializing value +1B
        message.put(getInitializeValue());
        //communication protocol +2B
        message.putShort(getProtocol());
        //battery +2B
        message.putShort(getBattery());
        //signal +2B
        message.putShort(getSignal());
        //values count +1B
        message.put((byte)0);
        //pad byte + 1B
        message.put((byte)0);
        //---------------- 39B ------------------
        //values
        int valueCount = 0;
        for(Value sensorValue : values){
            //insert value type, only if not actuator
            if(sensorValue.getType() != ACTUATOR_TYPE_NUMBER){
                message.putShort(sensorValue.getType());
                valueCount++;
            }
            //insert value
            Object value = sensorValue.getValue();
            if(value instanceof Float){
                //according to protocol, sent value should be 4Bytes, that equals to Java Integer
                message.putInt(Integer.valueOf(Math.round((Float)value * 100)));
            }else if(value instanceof Boolean){
                if(sensorValue.getType() == ACTUATOR_TYPE_NUMBER && getInitializeValue() == (byte)0xFF){
                    message.putShort(sensorValue.getType());
                    valueCount++;
                    if(((Boolean) value).booleanValue()){
                        message.put((byte) 1);
                    }else{
                        message.put((byte) 0);
                    }
                }else if( sensorValue.getType() == ACTUATORS_SENSOR_TYPE_NUMBER){
                    if(((Boolean) value).booleanValue()){
                        message.put((byte) 1);
                    }else{
                        message.put((byte) 0);
                    }
                }
            }
        }
        message.flip();
        message.position(MESSAGE_POS_BEFORE_VALUE_COUNT);
        message.put((byte)valueCount);
        message.position(0);
        //set the message length
        message.putShort((short) message.limit());
        message.position(0);
        if (DEBUG){
            System.out.println("ADAPTER HEADER------------------------");
            System.out.println("Message length: " + message.getShort());
            System.out.println("Adapter ID: " + message.getLong());
            System.out.println("Firmware: " + message.getShort());
            System.out.println("Protocol: " + message.getShort());
            Long time = message.getLong();
            System.out.println("Raw timestamp: " + time);
            System.out.println("Converted timestamp: " + new Date(time*1000));
            System.out.println("END ADAPTER HEADER------------------------");
            System.out.println("SENSOR ------------------------");
            System.out.println("Refresh rate !! not part of message: " + getRefreshRate());
            System.out.println("Sensor ip: " + message.getInt());
            System.out.println("Sensor port: " + message.getInt());
            System.out.println("Init value: " + message.get());
            System.out.println("Comm protocol: " + message.getShort());
            System.out.println("Battery: " + message.getShort());
            System.out.println("Signal: " + message.getShort());
            System.out.println("Values count: " + message.get());
            System.out.println("Pad byte: " + message.get());
            for(Value value : values){
                if(value.getType() == ACTUATOR_TYPE_NUMBER){
                    if(initializeValue != (byte)0xFF){
                        continue;
                    }else{
                        System.out.println("Value type: " + message.getShort());
                        System.out.println("Value: " + message.get());
                    }
                }else{
                    if(value.getValue() instanceof Boolean){
                        System.out.println("Value type: " + message.getShort());
                        System.out.println("Value: " + message.get());
                    }else{
                        System.out.println("Value type: " + message.getShort());
                        System.out.println("Value: " + message.getInt());
                    }
                }

            }
            System.out.println("END SENSOR------------------------");
            message.position(0);
        }
        return message;*/
        //----------------- END Byte Version ----------------------
    }

    /**
     * Parses message from server. Sets new refresh rate
     * @param message
     */
    public void parseInMessage(ByteBuffer message){
        try{
            message.position(0);
            String fromBytes = new String(message.array());
            int lastBeak = fromBytes.lastIndexOf(">");
            fromBytes = fromBytes.substring(0,lastBeak+1);
            Document document = null;
            try {
                document = DocumentHelper.parseText(fromBytes);
            } catch (DocumentException e) {
                if(!isFullMessage()){
                    adapter.getLogger().writeErrorMessage("Error: " + getName() + "/" + getSensorIdAsIp() + " --> Cannot parse XML response");
                }else{
                    adapter.getLogger().writeErrorMessage("Error: Adapter/" + adapter.getAdapterId() + " -> Sensor/" + getSensorIdAsIp() + " --> Cannot parse XML response");
                }
                adapter.getLogger().writeErrorMessage(e.getMessage());
            }
            try{
            if(document != null){
                Element rootElement = document.getRootElement();
                String protocol = rootElement.attribute("protocol_version").getValue();
                if(protocol.equals(String.valueOf(getProtocol()))){
                    String state = rootElement.attribute("state").getValue();
                    String id = null,time = null;
                    switch (state){
                        case "update":
                        case "set":
                            id = rootElement.attribute("id").getValue();
                            time = rootElement.attribute("time").getValue();
                            break;
                    }
                    if(id != null && time != null){
                        if(getRefreshRate() != Short.valueOf(time)){
                            //greater than -1 means message is ok
                            if(Short.valueOf(time) > -1){
                                if(!isFullMessage()){
                                    adapter.sendMessage("Sensor " + getName() + "/" + getSensorIdAsIp() + " new Refresh rate --> " + time);
                                }else{
                                    adapter.sendMessage("Adapter/" + adapter.getAdapterId() + " -> Sensor/" + getSensorIdAsIp() + " new Refresh rate --> " + time);
                                }
                                //refresh rate 0, do not update refresh rate
                                if(Short.valueOf(time) != 0){
                                    this.setRefreshRate(Short.valueOf(time));
                                }
                            }else{
                                //less than 0 means something went wrong on server side
                                if(!isFullMessage()){
                                    adapter.getLogger().writeErrorMessage("Error: " + getName() + "/" + getSensorIdAsIp() + " --> server trying to set refresh rate " + (int) refreshRate);
                                }else{
                                    adapter.getLogger().writeErrorMessage("Error: Adapter/" + adapter.getAdapterId() + " -> Sensor/" + getSensorIdAsIp() + " --> server trying to set refresh rate " + (int) refreshRate);
                                }
                                disconnect();
                            }
                        }
                    }else{
                        if(!isFullMessage()){
                            adapter.getLogger().writeErrorMessage("Error: " + getName() + "/" + getSensorIdAsIp() + " --> Id or time in server response doesn't exist");
                        }else{
                            adapter.getLogger().writeErrorMessage("Error: Adapter/" + adapter.getAdapterId() + " -> Sensor/" + getSensorIdAsIp() + " -->  Id or time in server response doesn't exist");
                        }
                    }
                }else{
                    if(!isFullMessage()){
                        adapter.getLogger().writeErrorMessage("Error: " + getName() + "/" + getSensorIdAsIp() + " --> Protocols doesn't match");
                    }else{
                        adapter.getLogger().writeErrorMessage("Error: Adapter/" + adapter.getAdapterId() + " -> Sensor/" + getSensorIdAsIp() + " -->  Protocols doesn't match");
                    }
                }

            }
            }catch(Exception e){
                adapter.getLogger().writeErrorMessage("Unknown error(Sensor->parseInMessage): " + e.getMessage());
            }
            /*short protocol = message.getShort();
            if(protocol == getProtocol()){
                int ip = message.getInt();
                int port = message.getInt();
                short refreshRate = message.getShort();
                if(DEBUG){
                    System.out.println("Protocol: " + protocol);
                    System.out.println("SensorIp: " + ip);
                    System.out.println("AdapterId: " + port);
                    System.out.println("Refresh rate: " + refreshRate);
                }
                this.setProtocol(protocol);
                if(getRefreshRate() != (int)refreshRate){
                    //greater than -1 means message is ok
                    if((int)refreshRate > -1){
                        if(!isFullMessage()){
                            adapter.sendMessage("Sensor " + getName() + "/" + getSensorId() + " new Refresh rate --> " + (int) refreshRate);
                        }else{
                            adapter.sendMessage("Adapter/" + adapter.getAdapterId() + " -> Sensor/" + getSensorId() + " new Refresh rate --> " + (int) refreshRate);
                        }
                        //refresh rate 0, do not update refresh rate
                        if((int)refreshRate != 0){
                            this.setRefreshRate((int)refreshRate);
                        }
                    }else{
                        //less than 0 means something went wrong on server side
                        if(!isFullMessage()){
                            adapter.getLogger().writeErrorMessage("Error: " + getName() + "/" + getSensorId() + " --> server trying to set refresh rate " + (int) refreshRate);
                        }else{
                            adapter.getLogger().writeErrorMessage("Error: Adapter/" + adapter.getAdapterId() + " -> Sensor/" + getSensorId() + " --> server trying to set refresh rate " + (int) refreshRate);
                        }
                        disconnect();
                    }
                }
                if(DEBUG){
                    System.out.println("END PARSING MESSAGE FROM SERVER----------------------------------");
                }
            }else{
                if(!isFullMessage()){
                    adapter.getLogger().writeErrorMessage("Message from server to " + getName() + "/" + getSensorId() + " has different protocol -> " + protocol);
                }else{
                    adapter.getLogger().writeErrorMessage("Message from server to : Adapter/"  + adapter.getAdapterId() + " -> Sensor/" + getSensorId() + " has different protocol -> " + protocol);
                }
            }*/
        }catch (BufferUnderflowException bue){
            if(!isFullMessage()){
                adapter.getLogger().writeErrorMessage("Unable to parse message (BufferUnderFlow) from server on :" + getName() + "/" + getSensorIdAsIp());
            }else{
                adapter.getLogger().writeErrorMessage("Unable to parse message (BufferUnderFlow) from server on : Adapter/" + adapter.getAdapterId() + " -> Sensor/" + getSensorIdAsIp());
            }
            disconnect();
        }
    }

    public void connect(){
        setActivity(true);
        adapter.activateSensor();
    }

    public void disconnect(){
        setActivity(false);
        adapter.deactivateSensor();
    }

    public void switchActuator(){
        for(Value value : values){
            value.switchActuatorState();
        }
        updateDetails();
    }

    public void criticalErrorStop(){
        disconnect();
        adapter.stopScheduler();
        adapter.messageSuccessfullySent();
        if(!isFullMessage()){
            adapter.getLogger().writeErrorMessage(getName() + "/" + getSensorIdAsIp() + " --> socket closed or wrong response from server!" +
                    " Current data set by server: Protocol ->" + getProtocol() + " Refresh rate: " + getRefreshRate());
        }else{
            adapter.getLogger().writeErrorMessage("Adapter/" + adapter.getAdapterId() + " -> Sensor/" + getSensorIdAsIp() + " --> socket closed or wrong response from server!" +
            " Current data set by server: Protocol ->" + getProtocol() + " Refresh rate: " + getRefreshRate());
        }
        adapter.startScheduler();
    }

    public void updateDetails(){
        if(this.panel != null)
            this.panel.refreshPropertyTbl();
    }

    public boolean equalsId(int id){
        return getSensorId() == id;
    }

    private void initTimer(){
        sendMessageTimer = new Timer((refreshRate*1000)/2, new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if(isValueGeneratorRunning()){
                    //before generating new value, store old one into history list
                    //afterwards generate new value
                    /*Value actuator = null,actuatorsSensor = null;*/
                    for(Value value:values){
                        /*if(value.getType().shortValue() == ACTUATORS_SENSOR_TYPE_NUMBER)
                            actuatorsSensor = value;
                        if(value.getType().shortValue() == ACTUATOR_TYPE_NUMBER)
                            actuator = value;*/
                        if(value.isGenerateValue())
                            value.nextValue();
                    }
                    /*if(actuator!=null && actuatorsSensor!=null){
                        actuatorsSensor.nextValue(actuator.getValue());
                    }*/
                    //show new value in GUI
                    updateDetails();
                    //prepare for next timer cycle to send message
                    setRunValueGenerator(false);
                }else if(isActive()){
                    if( pureActuator && getInitializeValue()!=(byte)0xFF){
                        setRunValueGenerator(true);
                        return;
                    }
                    int size = 0;
                    //increasing ByteBuffer size, for every measured value add 10B
                    for(Value value : values){
                        if(value.getType() == ACTUATOR_TYPE_NUMBER && initializeValue != (byte)0xFF){
                                continue;
                        }
                        size = size + 10;
                    }
                    Document message = buildSensorMessage(adapter.createAdapterHeader(size));
                    String tryingMsg,dataSentMsg,fallingAsleepMsg;
                    if(!isFullMessage()){
                        tryingMsg = "Sensor " + getName() + "/" + getSensorIdAsIp() + " trying to send message.";
                        dataSentMsg = "Sensor " + getName() + "/" + getSensorIdAsIp() + " --> data sent";
                        fallingAsleepMsg = "Sensor " + getName() + "/" + getSensorIdAsIp() + " falling asleep for " + (sendMessageTimer.getDelay()*2) / 1000 + " second/s";
                    }else{
                        tryingMsg = null;
                        dataSentMsg = "Adapter/" + adapter.getAdapterId() + " -> Sensor/" + getSensorIdAsIp() + " --> data sent";
                        fallingAsleepMsg = null;
                    }
                    if(tryingMsg != null)
                        adapter.sendMessage(tryingMsg);
                    if(dataSentMsg != null)
                        adapter.sendMessage(dataSentMsg,message,me);
                    if(fallingAsleepMsg != null)
                        adapter.sendMessage(fallingAsleepMsg);
                    if(getInitializeValue() == (byte)0xFF){
                        setInitializeValue((byte)0);
                    }
                    setRunValueGenerator(true);
                }
            }
        });
    }

    private void parseValues(){
        String[] vt = defaultProperties.getProperty("valueTypes").split(",");
        String[] vc = defaultProperties.getProperty("valueClasses").split(",");
        String[] v = defaultProperties.getProperty("values").split(",");
        String[] max = defaultProperties.getProperty("max").split(",");
        String[] min = defaultProperties.getProperty("min").split(",");
        String[] dev = defaultProperties.getProperty("dev").split(",");
        String[] avg = defaultProperties.getProperty("avg").split(",");
        String seeds = defaultProperties.getProperty("seed");
        String n = defaultProperties.getProperty("propertyNames",null);
        String[] names = null;
        if(n != null){
            names = n.split(",");
        }
        String[] seed = null;
        if(seeds!=null){
            seed = seeds.split(",");
        }
        if(vt.length == vc.length && vc.length == v.length
                && max.length == v.length && min.length == max.length
                && dev.length == min.length && avg.length == dev.length){
            int maxValues;
            if(vt.length < MAX_VALUES_NUMBER)
                maxValues = vt.length;
            else
                maxValues = MAX_VALUES_NUMBER;
            for(int j = 0;j < maxValues; j++){
                Value value = new Value();
                if(names != null){
                    value.setName(names[j+6]);
                }
                value.setGenerateValue(true);
                value.setType(new Short(Short.parseShort(vt[j].replace("0x",""),16)));
                if(vt.length == 1 && vt[j].equals("4"))
                    pureActuator = true;
                switch(vc[j]){
                    case "Float":
                    case "float":
                    case "F":
                    case "f":
                        value.setValue(new Float(v[j]));
                        value.setInitialValue(new Float(v[j]));
                        break;
                    case "Bool":
                    case "bool":
                    case "Boolean":
                    case "boolean":
                        value.setValue(new Boolean(v[j]));
                        value.setInitialValue(new Boolean(v[j]));
                        value.setGenerateValue(false);
                }
                value.setValueClass(vc[j]);
                value.setGenerator(new Random());
                if(seed != null){
                    value.setGeneratorSeed(Long.valueOf(seed[j]));
                }else {
                    value.setGeneratorSeed(System.currentTimeMillis());
                }
                value.setHistory(new ArrayList<Object>());
                if(!max[j].equals("none")){
                    value.setMax(Float.valueOf(max[j]));
                }
                if(!min[j].equals("none")){
                    value.setMin(Float.valueOf(min[j]));
                }
                if(!dev[j].equals("none")){
                    value.setDev(Float.valueOf(dev[j]));
                }
                if(!avg[j].equals("none")){
                    value.setAvg(Float.valueOf(avg[j]));
                }
                values.add(value);
            }
            updateDetails();
        }else{
            Emulator.showError(Error.PROPERTIES_LOAD_MULTISET.getMessage());
            this.initSuccess = false;
        }
    }

    public void setProtocol(short s){
        this.protocol = s;
        updateDetails();
    }
    public void setRefreshRate(int newRate){
        this.refreshRate = newRate;
        sendMessageTimer.setDelay((this.refreshRate * 1000) / 2);
        updateDetails();
    }
    public void setActivity(boolean b){
        this.active = b;
        updateDetails();
        if(b && adapter.isActive()){
            sendMessageTimer.start();
        }
        else{
            sendMessageTimer.stop();
            setInitializeValue((byte)0xFF);
        }
    }
    private void setTimer(Timer t){
        this.sendMessageTimer = t;
    }
    public ArrayList<Object> getLastValues(){
        if(values.size() > 0){
            lastValues.clear();
            for(int i = 0; i < values.size(); i++){
                lastValues.add(values.get(i).getValue());
            }
        }else {
            lastValues = null;
        }
        return lastValues;
    }
    public Timer getTimer(){
        return this.sendMessageTimer;
    }
    public int getRefreshRate(){
        return this.refreshRate;
    }
    public short getSignal(){
        return this.signal;
    }
    public void setSignal(short s){
        this.signal = s;
        updateDetails();
    }
    public short getBattery(){
        return this.battery;
    }
    public void setBattery(short s){
        this.battery = s;
        updateDetails();
    }
    public boolean isActive(){
        return this.active;
    }
    public String getSensorIdAsIp(){
        return SensorPanel.intToIp(getSensorId());
    }
    public int getSensorId(){
        return this.id;
    }
    public String getHexaSensorId(){
        return "0x" + Integer.toHexString(getSensorId());
    }
    public String getPropertyNames(){
        return panel.getPropertyNamesString();
    }
    public String getName(){
        return this.name;
    }
    public void setName(String name){
        this.name = name;
        updateDetails();
    }
    public byte getInitializeValue(){
        return this.initializeValue;
    }
    public void setInitializeValue(byte b){
        this.initializeValue = b;
    }
    public float getProtocol(){
        return this.protocol;
    }
    public boolean isValueGeneratorRunning(){
        return this.runValueGenerator;
    }
    public void setRunValueGenerator(boolean b){
        this.runValueGenerator = b;
    }
    public String getIconPath(){
        return this.iconPath;
    }
    public void setIconPath(String s){
        this.iconPath = s;
    }
    public String getBackgroundColorString() {
        return backgroundColorString;
    }
    public void setBackgroundColorString(String s){
        this.backgroundColorString = s;
    }
    public String getFontColorString() {
        return fontColorString;
    }
    public void setFontColorString(String s){
        this.fontColorString = s;
    }
    public String getLblFontColorString() {
        return lblFontColorString;
    }
    public void setLblFontColorString(String s){
        this.lblFontColorString = s;
    }
    public ArrayList<Value> getValues() {
        return values;
    }
    public SensorPanel getPanel(){
        return this.panel;
    }
    public boolean isFullMessage(){
        return this.fullMessage;
    }
    public void setFullMessage(boolean b){
        this.fullMessage = b;
    }
    public boolean isInitSuccess(){
        return this.initSuccess;
    }
}
