package com.iha.emulator.models;

import javafx.beans.property.*;

import java.util.Objects;

/**
 * Created by Shu on 25.11.2014.
 */
public class Server {

    private BooleanProperty conn;
    private StringProperty name;
    private StringProperty ip;
    private IntegerProperty port;
    private StringProperty databaseName;

    public Server(boolean conn,String name,String ip, int port,String databaseName) {
        this.conn = new SimpleBooleanProperty(conn);
        this.name = new SimpleStringProperty(name);
        this.ip = new SimpleStringProperty(ip);
        this.port = new SimpleIntegerProperty(port);
        this.databaseName = new SimpleStringProperty(databaseName);
    }

    @Override
    public String toString(){
        return this.getName();
    }

    public boolean getConn() {
        return conn.get();
    }

    public BooleanProperty connProperty() {
        return conn;
    }

    public void setConn(boolean conn) {
        this.conn.set(conn);
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

    public String getIp() {
        return ip.get();
    }

    public StringProperty ipProperty() {
        return ip;
    }

    public void setIp(String ip) {
        this.ip.set(ip);
    }

    public int getPort() {
        return port.get();
    }

    public IntegerProperty portProperty() {
        return port;
    }

    public void setPort(int port) {
        this.port.set(port);
    }

    public String getDatabaseName() {
        return databaseName.get();
    }

    public StringProperty databaseNameProperty() {
        return databaseName;
    }

    public void setDatabaseName(String databaseName) {
        this.databaseName.set(databaseName);
    }

    @Override
    public int hashCode() {
        return Objects.hash(this.getName(),this.getIp(),this.getPort(),this.getDatabaseName());
    }

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
