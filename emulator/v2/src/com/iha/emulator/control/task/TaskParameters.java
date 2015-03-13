package com.iha.emulator.control.task;

import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.utilities.watchers.StopWatch;
import javafx.beans.property.*;
import org.dom4j.Element;

/**
 * Created by Shu on 26.2.2015.
 */
public class TaskParameters {

    public static final String DEFAULT_SAVE_DIR = "logs/performance";

    private IntegerProperty adaptersCount;
    private ObjectProperty<Protocol.Version> protocolVersion;
    private IntegerProperty startId;
    private IntegerProperty sensorsCountMin;
    private IntegerProperty sensorsCountMax;
    private IntegerProperty sensorsCount;
    private IntegerProperty refreshTimeMin;
    private IntegerProperty refreshTimeMax;
    private Long sensorsCountGeneratorSeed;
    private Long refreshTimeGeneratorSeed;
    private Long signalGeneratorSeed;
    private Long batteryGeneratorSeed;
    private Long valueTypeGeneratorSeed;
    private StringProperty saveDir;
    private StopWatch stopWatch;
    private ValueParameters valueParameters = null;

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

    public int getAdaptersCount() {
        return adaptersCount.get();
    }

    public IntegerProperty adaptersCountProperty() {
        return adaptersCount;
    }

    public void setAdaptersCount(int adaptersCount) {
        this.adaptersCount.set(adaptersCount);
    }

    public Protocol.Version getProtocolVersion() {
        return protocolVersion.get();
    }

    public ObjectProperty<Protocol.Version> protocolVersionProperty() {
        return protocolVersion;
    }

    public void setProtocolVersion(Protocol.Version protocolVersion) {
        this.protocolVersion.set(protocolVersion);
    }

    public int getStartId() {
        return startId.get();
    }

    public IntegerProperty startIdProperty() {
        return startId;
    }

    public void setStartId(int startId) {
        this.startId.set(startId);
    }

    public int getSensorsCountMin() {
        return sensorsCountMin.get();
    }

    public IntegerProperty sensorsCountMinProperty() {
        return sensorsCountMin;
    }

    public void setSensorsCountMin(int sensorsCountMin) {
        this.sensorsCountMin.set(sensorsCountMin);
    }

    public int getSensorsCountMax() {
        return sensorsCountMax.get();
    }

    public IntegerProperty sensorsCountMaxProperty() {
        return sensorsCountMax;
    }

    public void setSensorsCountMax(int sensorsCountMax) {
        this.sensorsCountMax.set(sensorsCountMax);
    }

    public int getRefreshTimeMin() {
        return refreshTimeMin.get();
    }

    public IntegerProperty refreshTimeMinProperty() {
        return refreshTimeMin;
    }

    public void setRefreshTimeMin(int refreshTimeMin) {
        this.refreshTimeMin.set(refreshTimeMin);
    }

    public int getRefreshTimeMax() {
        return refreshTimeMax.get();
    }

    public IntegerProperty refreshTimeMaxProperty() {
        return refreshTimeMax;
    }

    public void setRefreshTimeMax(int refreshTimeMax) {
        this.refreshTimeMax.set(refreshTimeMax);
    }


    public Long getSensorsCountGeneratorSeed() {
        return sensorsCountGeneratorSeed;
    }

    public void setSensorsCountGeneratorSeed(Long sensorsCountGeneratorSeed) {
        this.sensorsCountGeneratorSeed = sensorsCountGeneratorSeed;
    }

    public Long getRefreshTimeGeneratorSeed() {
        return refreshTimeGeneratorSeed;
    }

    public void setRefreshTimeGeneratorSeed(Long refreshTimeGeneratorSeed) {
        this.refreshTimeGeneratorSeed = refreshTimeGeneratorSeed;
    }

    public Long getSignalGeneratorSeed() {
        return signalGeneratorSeed;
    }

    public void setSignalGeneratorSeed(Long signalGeneratorSeed) {
        this.signalGeneratorSeed = signalGeneratorSeed;
    }

    public Long getBatteryGeneratorSeed() {
        return batteryGeneratorSeed;
    }

    public void setBatteryGeneratorSeed(Long batteryGeneratorSeed) {
        this.batteryGeneratorSeed = batteryGeneratorSeed;
    }

    public Long getValueTypeGeneratorSeed() {
        return valueTypeGeneratorSeed;
    }

    public void setValueTypeGeneratorSeed(Long valueTypeGeneratorSeed) {
        this.valueTypeGeneratorSeed = valueTypeGeneratorSeed;
    }

    public String getSaveDir() {
        return saveDir.get();
    }

    public StringProperty saveDirProperty() {
        return saveDir;
    }

    public void setSaveDir(String saveDir) {
        this.saveDir.set(saveDir);
    }

    public int getSensorsCount() {
        return sensorsCount.get();
    }

    public IntegerProperty sensorsCountProperty() {
        return sensorsCount;
    }

    public void setSensorsCount(int sensorsCount) {
        this.sensorsCount.set(sensorsCount);
    }

    public StopWatch getStopWatch() {
        return stopWatch;
    }

    public void setStopWatch(StopWatch stopWatch) {
        this.stopWatch = stopWatch;
    }

    public ValueParameters getValueParameters() {
        return valueParameters;
    }

    public void setValueParameters(ValueParameters valueParameters) {
        this.valueParameters = valueParameters;
    }
}
