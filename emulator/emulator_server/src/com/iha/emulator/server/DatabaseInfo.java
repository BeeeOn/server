package com.iha.emulator.server;

/**
 * Class holding information needed for connection to database. All information are set by EmulatorServer from
 * configuration file, except database name, which is included in every message from client.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class DatabaseInfo {
    /** database name */
    private String dbName;
    /** database hostname */
    private String host;
    /** database port */
    private String port;
    /** username */
    private String user;
    /** password */
    private String pass;

    /**
     * Creates instance with given information.
     * @param host database hostname
     * @param port database port
     * @param user username
     * @param pass port
     */
    public DatabaseInfo(String host,String port,String user,String pass) {
        this.host = host;
        this.port = port;
        this.user = user;
        this.pass = pass;
    }

    /**
     * Sets database name
     * @param dbName database name
     */
    public void setDbName(String dbName) {
        this.dbName = dbName;
    }

    /**
     * Gets database name
     * @return database name
     */
    public String getDbName() {
        return dbName;
    }

    /**
     * Gets database hostname
     * @return database hostname
     */
    public String getHost() {
        return host;
    }

    /**
     * Gets database port
     * @return database port
     */
    public String getPort() {
        return port;
    }

    /**
     * Gets username
     * @return username
     */
    public String getUser() {
        return user;
    }

    /**
     * Gets user password
     * @return user password
     */
    public String getPass() {
        return pass;
    }
}
