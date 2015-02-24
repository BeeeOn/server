package com.iha.emulator.server;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import javax.naming.NamingException;
import java.net.ConnectException;
import java.sql.*;

/**
 * Created by Shu on 6.12.2014.
 */
public class Database {

    private static final Logger logger = LogManager.getLogger(Database.class);


    public Connection connect(DatabaseInfo dbInfo) throws SQLException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver");
        return DriverManager.getConnection("jdbc:postgresql://"+dbInfo.getHost()+":"+dbInfo.getPort()+"/" + dbInfo.getDbName(), dbInfo.getUser(), dbInfo.getPass());
    }

    public Statement getStatement(Connection c) throws ConnectException, SQLException, NamingException {
        return c.createStatement();
    }

    public void disconnect(Connection c) throws SQLException {
        c.close();
    }

    public void disconnectWithStatement(Statement stmt) throws SQLException {
        logger.debug("Disconnecting from database");
        Connection conn = null;
        conn = stmt.getConnection();
        stmt.close();
        conn.close();
    }

    public void disconnectWithResultSet(ResultSet set) {
        logger.debug("Disconnecting from database");
        Statement stmt = null;
        Connection conn = null;
        try {
            stmt = set.getStatement();
            conn = stmt.getConnection();
        } catch (SQLException e) {
            logger.error("Cannot get statement or connection from resultSet",e);
        }

        try {
            set.close();
            stmt.close();
            conn.close();
        } catch (SQLException e) {
            logger.error("Cannot close resultSet,Statement or Connection",e);
        }

    }
}
