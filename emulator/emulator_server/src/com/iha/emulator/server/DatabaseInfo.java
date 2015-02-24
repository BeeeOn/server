package com.iha.emulator.server;

/**
 * Created by Shu on 24.2.2015.
 */
public class DatabaseInfo {

    private String dbName;
    private String host;
    private String port;
    private String user;
    private String pass;

    public DatabaseInfo(String host,String port,String user,String pass) {
        this.host = host;
        this.port = port;
        this.user = user;
        this.pass = pass;
    }

    public void setDbName(String dbName) {
        this.dbName = dbName;
    }

    public String getDbName() {
        return dbName;
    }

    public String getHost() {
        return host;
    }

    public String getPort() {
        return port;
    }

    public String getUser() {
        return user;
    }

    public String getPass() {
        return pass;
    }
}
