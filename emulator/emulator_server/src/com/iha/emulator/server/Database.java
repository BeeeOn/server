package com.iha.emulator.server;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import javax.naming.NamingException;
import java.net.ConnectException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;

/**
 * Class providing connection to database and sending database statements.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class Database {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(Database.class);

    /**
     * Connects to database with given information.
     * @param dbInfo information about database
     * @return established database connection
     * @throws SQLException cannot connect to database
     * @throws ClassNotFoundException cannot find Driver class
     */
    public Connection connect(DatabaseInfo dbInfo) throws SQLException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver");
        return DriverManager.getConnection("jdbc:postgresql://"+dbInfo.getHost()+":"+dbInfo.getPort()+"/" + dbInfo.getDbName(), dbInfo.getUser(), dbInfo.getPass());
    }

    /**
     * Creates and returns new statement on given connection.
     * @param c established connection
     * @return new statement
     * @throws ConnectException connection not established
     * @throws SQLException cannot create statement
     * @throws javax.naming.NamingException error in database context
     */
    public Statement getStatement(Connection c) throws ConnectException, SQLException, NamingException {
        return c.createStatement();
    }

    /**
     * Disconnects given connection
     * @param c connection to be closed
     * @throws SQLException error while closing connection
     */
    public void disconnect(Connection c) throws SQLException {
        c.close();
    }
}
