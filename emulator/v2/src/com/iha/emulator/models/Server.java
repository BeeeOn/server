package com.iha.emulator.models;

import javafx.beans.property.*;

import java.util.Objects;

/**
 * Model of server holding information needed to communicate with server and EmulatorServer
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class Server {
    /** connection status property */
    private BooleanProperty conn;
    /** name property */
    private StringProperty name;
    /** ip/hostname property */
    private StringProperty ip;
    /** port property */
    private IntegerProperty port;
    /** database name property */
    private StringProperty databaseName;

    /**
     * Creates Server model with given connection status, name, ip/hostname, port and database name.
     *
     * @param conn initial connection status
     * @param name name
     * @param ip ip or hostname
     * @param port port
     * @param databaseName database name
     */
    public Server(boolean conn,String name,String ip, int port,String databaseName) {
        this.conn = new SimpleBooleanProperty(conn);
        this.name = new SimpleStringProperty(name);
        this.ip = new SimpleStringProperty(ip);
        this.port = new SimpleIntegerProperty(port);
        this.databaseName = new SimpleStringProperty(databaseName);
    }

    /**
     * Returns server name
     * @return server name
     */
    @Override
    public String toString(){
        return this.getName();
    }

    /**
     * Gets connection status
     * @return connection status
     */
    public boolean getConn() {
        return conn.get();
    }

    /**
     * Gets connection status property, can be bound
     * @return connection status property
     */
    public BooleanProperty connProperty() {
        return conn;
    }

    /**
     * Sets connection status
     * @param conn connection status
     */
    public void setConn(boolean conn) {
        this.conn.set(conn);
    }

    /**
     * Gets server name
     * @return server name
     */
    public String getName() {
        return name.get();
    }

    /**
     * Gets server name property, can be bound
     * @return server name property
     */
    public StringProperty nameProperty() {
        return name;
    }

    /**
     * Sets server name
     * @param name server name
     */
    public void setName(String name) {
        this.name.set(name);
    }

    /**
     * Gets server ip/hostname
     * @return server ip/hostname
     */
    public String getIp() {
        return ip.get();
    }

    /**
     * Gets server ip/hostname property, can be bound
     * @return server ip/hostname property
     */
    public StringProperty ipProperty() {
        return ip;
    }

    /**
     * Sets server ip/hostname
     * @param ip server ip/hostname
     */
    public void setIp(String ip) {
        this.ip.set(ip);
    }

    /**
     * Gets server port
     * @return server port
     */
    public int getPort() {
        return port.get();
    }

    /**
     * Gets server port property, can be bound
     * @return server port property
     */
    public IntegerProperty portProperty() {
        return port;
    }

    /**
     * Sets server port
     * @param port server port
     */
    public void setPort(int port) {
        this.port.set(port);
    }

    /**
     * Gets database name
     * @return database name
     */
    public String getDatabaseName() {
        return databaseName.get();
    }

    /**
     * Gets database name property, can be bound
     * @return database name property
     */
    public StringProperty databaseNameProperty() {
        return databaseName;
    }

    /**
     * Sets database name
     * @param databaseName database name
     */
    public void setDatabaseName(String databaseName) {
        this.databaseName.set(databaseName);
    }

    /**
     * Generates hash code consisting of combination of all models variables
     * @return Hash code consisting of combination of all models variables
     */
    @Override
    public int hashCode() {
        return Objects.hash(this.getName(),this.getIp(),this.getPort(),this.getDatabaseName());
    }

    /**
     * If each model's variable equals to given object variables
     * @param o instance of {@link com.iha.emulator.models.Server}
     * @return true, if all variables equals to given object variables
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Server server = (Server) o;
        return getDatabaseName().equals(server.getDatabaseName()) &&
                getIp().equals(server.getIp()) &&
                getName().equals(server.getName()) &&
                getPort() == server.getPort();
    }
}
