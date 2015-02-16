package com.iha.emulator.server.task.implemented;

import com.iha.emulator.server.Database;
import com.iha.emulator.server.task.AbstractTask;
import org.apache.logging.log4j.Logger;
import org.dom4j.Element;

import javax.naming.NamingException;
import java.net.ConnectException;
import java.sql.Connection;
import java.sql.SQLException;

/**
 * Created by Shu on 8.12.2014.
 */
public class DeleteAdapterTask extends AbstractTask {

    private Logger logger;

    private String dbName;
    private String adapterId;
    private Database database;

    public DeleteAdapterTask(Logger logger, String dbName, String adapterId) {
        this.logger = logger;
        this.dbName = dbName;
        this.adapterId = adapterId;
        this.database = new Database();
    }

    @Override
    public Element resolveTask() {
        try {
            Connection conn = database.connect(this.dbName);
            String sql = "DELETE FROM adapters WHERE adapter_id="+this.adapterId;
            logger.trace("DeleteAdapter SQL: ");
            logger.trace(sql);
            int resultCount = database.getStatement(conn).executeUpdate(sql);
            if(resultCount == 0) throw new SQLException("No adapter with id " + adapterId + " in database");
            logger.trace("SQL executed successfully");
            Element result = buildPositiveResultElement();
            database.disconnect(conn);
            return result;
        } catch (ConnectException | ClassNotFoundException | NamingException e) {
            logger.error("Cannot connect to database",e);
            return buildNegativeResultElement("Cannot connect to database");
        } catch (SQLException e) {
            logger.error("Cannot execute SQL query", e);
            return buildNegativeResultElement("Cannot execute SQL query.\n " + e.getMessage());
        }
    }
}
