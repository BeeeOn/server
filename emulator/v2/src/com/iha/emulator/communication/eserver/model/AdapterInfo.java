package com.iha.emulator.communication.eserver.model;

import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;

/**
 * Created by Shu on 8.12.2014.
 */
public class AdapterInfo {

    private StringProperty id;
    private StringProperty name;
    private StringProperty version;

    public AdapterInfo(String id,String name) {
        this.id = new SimpleStringProperty(id);
        this.name = new SimpleStringProperty(name);
    }

    public AdapterInfo(String id,String name,String version){
        this(id,name);
        this.version = new SimpleStringProperty(version);
    }

    public String getId() {
        return id.get();
    }

    public StringProperty idProperty() {
        return id;
    }

    public void setId(String id) {
        this.id.set(id);
    }

    public String getName() {
        return name.get();
    }

    public StringProperty nameProperty() {
        return name;
    }

    public void setName(String name) {
        this.name.set(name);
    }

    public String getVersion() {
        return version.get();
    }

    public StringProperty versionProperty() {
        return version;
    }

    public void setVersion(String version) {
        this.version.set(version);
    }
}
