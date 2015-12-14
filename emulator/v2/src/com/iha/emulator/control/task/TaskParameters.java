package com.iha.emulator.control.task;

import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.utilities.watchers.StopWatch;
import javafx.beans.property.*;
import org.dom4j.Element;

/**
 * Class providing information about {@link com.iha.emulator.control.task.SimulationTask} needed for creating and
 * running this task.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class TaskParameters {
    /** default path to directory, where log should be created */
    public static final String DEFAULT_SAVE_DIR = "logs/performance";
    /** Number of adapters to be created */
    private IntegerProperty adaptersCount;
    /** protocol version used by simulation task */
    private ObjectProperty<Protocol.Version> protocolVersion;
    /** first adapter ID */
    private IntegerProperty startId;
    /** minimal number of sensors created for each adapter */
    private IntegerProperty sensorsCountMin;
    /** maximal number of sensors created for each adapter */
    private IntegerProperty sensorsCountMax;
    /** actual number of created sensors */
    private IntegerProperty sensorsCount;
    /** minimal refresh time for each created sensor */
    private IntegerProperty refreshTimeMin;
    /** maximal refresh time for each created sensor */
    private IntegerProperty refreshTimeMax;
    /** seed for generator of sensors count */
    private Long sensorsCountGeneratorSeed;
    /** seed for generator of refresh time */
    private Long refreshTimeGeneratorSeed;
    /** seed for generator of signal value */
    private Long signalGeneratorSeed;
    /** seed for generator of battery value */
    private Long batteryGeneratorSeed;
    /** seed for generator of value type */
    private Long valueTypeGeneratorSeed;
    /** path to directory, where log should be created */
    private StringProperty saveDir;
    /** class timing task's running time */
    private StopWatch stopWatch;
    /** values, that can be generated for task */
    private ValueParameters valueParameters = null;
    /**
     * Creates new instance with default values.
     */
    public TaskParameters() {
        this.adaptersCount = new SimpleIntegerProperty(0);
        this.protocolVersion = new SimpleObjectProperty<>(null);
        this.startId = new SimpleIntegerProperty(0);
        this.sensorsCountMin = new SimpleIntegerProperty(0);
        this.sensorsCountMax = new SimpleIntegerProperty(0);
        this.sensorsCount = new SimpleIntegerProperty(0);
        this.refreshTimeMin = new SimpleIntegerProperty(0);
        this.refreshTimeMax = new SimpleIntegerProperty(0);
        this.sensorsCountGeneratorSeed = System.currentTimeMillis();
        this.refreshTimeGeneratorSeed = System.currentTimeMillis();
        this.signalGeneratorSeed = System.currentTimeMillis();
        this.batteryGeneratorSeed = System.currentTimeMillis();
        this.valueTypeGeneratorSeed = System.currentTimeMillis();
        this.saveDir = new SimpleStringProperty(DEFAULT_SAVE_DIR);
        this.stopWatch = new StopWatch();
    }
    /**
     * Creates Dom4j XML element task's parameters.
     * This element is added to root element given as parameter.
     *
     * @param rootElement root Dom4j XML element for server
     * @return XML element
     */
    public Element saveToXml(Element rootElement){
        //parameters element
        Element parametersElement = rootElement.addElement("parameters")
                .addAttribute("ada_count",String.valueOf(getAdaptersCount()))
                .addAttribute("protocol",String.valueOf(getProtocolVersion().getVersion()))
                .addAttribute("start_id",String.valueOf(getStartId()));
        //sensors count element
        parametersElement.addElement("sensors_count")
                .addAttribute("min",String.valueOf(getSensorsCountMin()))
                .addAttribute("max",String.valueOf(getSensorsCountMax()))
                .addAttribute("seed",String.valueOf(getSensorsCountGeneratorSeed()));
        //refresh time element
        parametersElement.addElement("refresh_time")
                .addAttribute("min",String.valueOf(getRefreshTimeMin()))
                .addAttribute("max",String.valueOf(getRefreshTimeMax()))
                .addAttribute("seed",String.valueOf(getRefreshTimeGeneratorSeed()));
        //save dir element
        parametersElement.addElement("save_dir").addText(getSaveDir());
        //enabled values element
        Element enabledValuesElement = getValueParameters().saveToXml(parametersElement);
        enabledValuesElement.addAttribute("seed",String.valueOf(getValueTypeGeneratorSeed()));
        return parametersElement;
    }
    /**
     * Gets number of adapters to be created
     * @return number of adapters to be created
     */
    public int getAdaptersCount() {
        return adaptersCount.get();
    }
    /**
     * Gets number of adapters to be created property, that can be bound
     * @return number of adapters to be created property
     */
    public IntegerProperty adaptersCountProperty() {
        return adaptersCount;
    }
    /**
     * Sets number of adapters to be created
     * @param adaptersCount number of adapters to be created
     */
    public void setAdaptersCount(int adaptersCount) {
        this.adaptersCount.set(adaptersCount);
    }
    /**
     * Gets version of protocol to be used
     * @return version of protocol to be used
     */
    public Protocol.Version getProtocolVersion() {
        return protocolVersion.get();
    }

    /**
     * Gets version of protocol to be used property, that can be bound
     * @return version of protocol to be used property
     */
    public ObjectProperty<Protocol.Version> protocolVersionProperty() {
        return protocolVersion;
    }

    /**
     * Sets version of protocol to be used
     * @param protocolVersion version of protocol to be used
     */
    public void setProtocolVersion(Protocol.Version protocolVersion) {
        this.protocolVersion.set(protocolVersion);
    }

    /**
     * Gets first created adapter ID
     * @return first created adapter ID
     */
    public int getStartId() {
        return startId.get();
    }

    /**
     * Gets first created adapter ID property, that can be bound
     * @return first created adapter ID property
     */
    public IntegerProperty startIdProperty() {
        return startId;
    }

    /**
     * Sets first created adapter ID
     * @param startId first created adapter ID
     */
    public void setStartId(int startId) {
        this.startId.set(startId);
    }

    /**
     * Gets minimal number of sensors to be created for each adapter
     * @return minimal number of sensors to be created for each adapter
     */
    public int getSensorsCountMin() {
        return sensorsCountMin.get();
    }

    /**
     * Gets minimal number of sensors to be created for each adapter property, that can be bound
     * @return minimal number of sensors to be created for each adapter property
     */
    public IntegerProperty sensorsCountMinProperty() {
        return sensorsCountMin;
    }

    /**
     * Sets minimal number of sensors to be created for each adapter
     * @param sensorsCountMin minimal number of sensors to be created for each adapter
     */
    public void setSensorsCountMin(int sensorsCountMin) {
        this.sensorsCountMin.set(sensorsCountMin);
    }
    /**
     * Gets maximal number of sensors to be created for each adapter
     * @return maximal number of sensors to be created for each adapter
     */
    public int getSensorsCountMax() {
        return sensorsCountMax.get();
    }
    /**
     * Gets maximal number of sensors to be created for each adapter property, that can be bound
     * @return maximal number of sensors to be created for each adapter property
     */
    public IntegerProperty sensorsCountMaxProperty() {
        return sensorsCountMax;
    }
    /**
     * Sets maximal number of sensors to be created for each adapter
     * @param sensorsCountMax maximal number of sensors to be created for each adapter
     */
    public void setSensorsCountMax(int sensorsCountMax) {
        this.sensorsCountMax.set(sensorsCountMax);
    }

    /**
     * Gets minimal refresh time for each created sensor
     * @return minimal refresh time for each created sensor
     */
    public int getRefreshTimeMin() {
        return refreshTimeMin.get();
    }

    /**
     * Gets minimal refresh time for each created sensor property, that can be bound
     * @return minimal refresh time for each created sensor property
     */
    public IntegerProperty refreshTimeMinProperty() {
        return refreshTimeMin;
    }

    /**
     * Sets minimal refresh time for each created sensor
     * @param refreshTimeMin minimal refresh time for each created sensor
     */
    public void setRefreshTimeMin(int refreshTimeMin) {
        this.refreshTimeMin.set(refreshTimeMin);
    }
    /**
     * Gets maximal refresh time for each created sensor
     * @return maximal refresh time for each created sensor
     */
    public int getRefreshTimeMax() {
        return refreshTimeMax.get();
    }
    /**
     * Gets maximal refresh time for each created sensor property, that can be bound
     * @return maximal refresh time for each created sensor property
     */
    public IntegerProperty refreshTimeMaxProperty() {
        return refreshTimeMax;
    }
    /**
     * Sets maximal refresh time for each created sensor
     * @param refreshTimeMax minimal refresh time for each created sensor
     */
    public void setRefreshTimeMax(int refreshTimeMax) {
        this.refreshTimeMax.set(refreshTimeMax);
    }

    /**
     * Gets seed for generator of sensors count
     * @return seed for generator of sensors count
     */
    public Long getSensorsCountGeneratorSeed() {
        return sensorsCountGeneratorSeed;
    }

    /**
     * Sets seed for generator of sensors count
     * @param sensorsCountGeneratorSeed seed for generator of sensors count
     */
    public void setSensorsCountGeneratorSeed(Long sensorsCountGeneratorSeed) {
        this.sensorsCountGeneratorSeed = sensorsCountGeneratorSeed;
    }

    /**
     * Gets seed for generator of refresh time
     * @return seed for generator of refresh time
     */
    public Long getRefreshTimeGeneratorSeed() {
        return refreshTimeGeneratorSeed;
    }

    /**
     * Sets seed for generator of refresh time
     * @param refreshTimeGeneratorSeed seed for generator of refresh time
     */
    public void setRefreshTimeGeneratorSeed(Long refreshTimeGeneratorSeed) {
        this.refreshTimeGeneratorSeed = refreshTimeGeneratorSeed;
    }

    /**
     * Gets seed for generator of signal value
     * @return seed for generator of signal value
     */
    public Long getSignalGeneratorSeed() {
        return signalGeneratorSeed;
    }

    /**
     * Sets seed for generator of signal value
     * @param signalGeneratorSeed seed for generator of signal value
     */
    public void setSignalGeneratorSeed(Long signalGeneratorSeed) {
        this.signalGeneratorSeed = signalGeneratorSeed;
    }

    /**
     * Gets seed for generator of battery value
     * @return seed for generator of battery value
     */
    public Long getBatteryGeneratorSeed() {
        return batteryGeneratorSeed;
    }

    /**
     * Sets seed for generator of battery value
     * @param batteryGeneratorSeed seed for generator of battery value
     */
    public void setBatteryGeneratorSeed(Long batteryGeneratorSeed) {
        this.batteryGeneratorSeed = batteryGeneratorSeed;
    }

    /**
     * Gets seed for generator of value type
     * @return seed for generator of value type
     */
    public Long getValueTypeGeneratorSeed() {
        return valueTypeGeneratorSeed;
    }

    /**
     * Sets seed for generator of value type
     * @param valueTypeGeneratorSeed seed for generator of value type
     */
    public void setValueTypeGeneratorSeed(Long valueTypeGeneratorSeed) {
        this.valueTypeGeneratorSeed = valueTypeGeneratorSeed;
    }

    /**
     * Gets path to directory, where log should be or already is created
     * @return path to directory, where log should be or already is created
     */
    public String getSaveDir() {
        return saveDir.get();
    }

    /**
     * Gets path to directory, where log should be or already is created property, that can be bound
     * @return path to directory, where log should be or already is created property
     */
    public StringProperty saveDirProperty() {
        return saveDir;
    }

    /**
     * Sets path to directory, where log should be or already is created
     * @param saveDir path to directory, where log should be or already is created
     */
    public void setSaveDir(String saveDir) {
        this.saveDir.set(saveDir);
    }

    /**
     * Gets actual number of created sensors
     * @return actual number of created sensors
     */
    public int getSensorsCount() {
        return sensorsCount.get();
    }

    /**
     * Gets actual number of created sensors property, that can be bound
     * @return actual number of created sensors property
     */
    public IntegerProperty sensorsCountProperty() {
        return sensorsCount;
    }

    /**
     * Sets actual number of created sensors
     * @param sensorsCount actual number of created sensors
     */
    public void setSensorsCount(int sensorsCount) {
        this.sensorsCount.set(sensorsCount);
    }

    /**
     * Gets class timing task's running time
     * @return class timing task's running time
     */
    public StopWatch getStopWatch() {
        return stopWatch;
    }

    /**
     * Sets class timing task's running time
     * @param stopWatch class timing task's running time
     */
    public void setStopWatch(StopWatch stopWatch) {
        this.stopWatch = stopWatch;
    }

    /**
     * Gets values, that can be generated for task
     * @return values, that can be generated for task
     */
    public ValueParameters getValueParameters() {
        return valueParameters;
    }

    /**
     * Sets values, that can be generated for task
     * @param valueParameters values, that can be generated for task
     */
    public void setValueParameters(ValueParameters valueParameters) {
        this.valueParameters = valueParameters;
    }
}
