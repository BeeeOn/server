package com.iha.emulator.control;

import com.iha.emulator.communication.protocol.Protocol;
import javafx.beans.property.*;

/**
 * Created by Shu on 26.2.2015.
 */
public class TaskParameters {

    private static final String DEFAULT_SAVE_DIR = "logs/performance";

    private IntegerProperty adaptersCount;
    private ObjectProperty<Protocol.Version> protocolVersion;
    private IntegerProperty startId;
    private IntegerProperty sensorsCountMin;
    private IntegerProperty sensorsCountMax;
    private IntegerProperty refreshTimeMin;
    private IntegerProperty refreshTimeMax;
    private Long sensorsCountGeneratorSeed;
    private Long refreshTimeGeneratorSeed;
    private Long signalGeneratorSeed;
    private Long batteryGeneratorSeed;
    private Long valueTypeGeneratorSeed;
    private StringProperty saveDir;

    public TaskParameters() {
        this.adaptersCount = new SimpleIntegerProperty(0);
        this.protocolVersion = new SimpleObjectProperty<>(null);
        this.startId = new SimpleIntegerProperty(0);
        this.sensorsCountMin = new SimpleIntegerProperty(0);
        this.sensorsCountMax = new SimpleIntegerProperty(0);
        this.refreshTimeMin = new SimpleIntegerProperty(0);
        this.refreshTimeMax = new SimpleIntegerProperty(0);
        this.sensorsCountGeneratorSeed = System.currentTimeMillis();
        this.refreshTimeGeneratorSeed = System.currentTimeMillis();
        this.signalGeneratorSeed = System.currentTimeMillis();
        this.batteryGeneratorSeed = System.currentTimeMillis();
        this.valueTypeGeneratorSeed = System.currentTimeMillis();
        this.saveDir = new SimpleStringProperty(DEFAULT_SAVE_DIR);
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
}
