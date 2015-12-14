package com.iha.emulator.communication.eserver.model;

import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;

/**
 * Class holding simple information about adapter (id, name, version) retrieved from emulator server
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class AdapterInfo {
    /** Adapter id */
    private StringProperty id;
    /** Adapter name */
    private StringProperty name;
    /** Adapter firmware version */
    private StringProperty version;

    /**
     * Creates new AdapterInfo with given id and name
     * @param id adapter id
     * @param name adapter name
     */
    public AdapterInfo(String id,String name) {
        this.id = new SimpleStringProperty(id);
        this.name = new SimpleStringProperty(name);
    }
    /**
     * Creates new AdapterInfo with given id and name
     * @param id adapter id
     * @param name adapter name
     * @param version adapter firmware version
     */
    public AdapterInfo(String id,String name,String version){
        this(id,name);
        this.version = new SimpleStringProperty(version);
    }

    /**
     * Gets adapter ID
     * @return adapter ID
     */
    public String getId() {
        return id.get();
    }
    /**
     * Gets adapter ID property, that can be bound
     * @return adapter ID property
     */
    public StringProperty idProperty() {
        return id;
    }
    /**
     * Sets adapter ID
     * @param id adapter ID
     */
    public void setId(String id) {
        this.id.set(id);
    }
    /**
     * Gets adapter name
     * @return adapter name
     */
    public String getName() {
        return name.get();
    }
    /**
     * Gets adapter name property, that can be bound
     * @return adapter name property
     */
    public StringProperty nameProperty() {
        return name;
    }
    /**
     * Sets adapter name
     * @param name adapter name
     */
    public void setName(String name) {
        this.name.set(name);
    }
    /**
     * Gets adapter firmware version
     * @return adapter firmware version
     */
    public String getVersion() {
        return version.get();
    }
    /**
     * Gets adapter firmware version property, that can be bound
     * @return adapter firmware version property
     */
    public StringProperty versionProperty() {
        return version;
    }
    /**
     * Sets adapter firmware version
     * @param version adapter firmware version
     */
    public void setVersion(String version) {
        this.version.set(version);
    }
}
