package com.iha.emulator.server.task.implemented;

import com.iha.emulator.server.Database;
import com.iha.emulator.server.DatabaseInfo;
import com.iha.emulator.server.task.AbstractTask;
import org.apache.logging.log4j.Logger;
import org.dom4j.Element;

import javax.naming.NamingException;
import java.net.ConnectException;
import java.sql.Connection;
import java.sql.SQLException;

/**
 * Class implementing EmulatorServer task, that deletes given adapter IDs from database.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class DeleteAdapterTask extends AbstractTask {
    /** Log4j2 logger field */
    private Logger logger;
    /** database information */
    private DatabaseInfo dbInfo;
    /** string containing all adapter IDs separated by comma */
    private String adapterId;
    /** instance of class providing connection to database */
    private Database database;

    /**
     * Creates EmulatorServer task with given information
     *
     * @param logger log4j2 logger
     * @param dbInfo database information
     * @param adapterId ID of first adapter to be deleted
     * @param count number of adapters to be deleted
     */
    public DeleteAdapterTask(Logger logger, DatabaseInfo dbInfo, String adapterId,Integer count) {
        if(count == null) count = 1;
        this.logger = logger;
        this.dbInfo = dbInfo;
        this.adapterId = joinIds(adapterId,count);
        this.database = new Database();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Element resolveTask() {
        try {
            Connection conn = database.connect(this.dbInfo);
            String sql = "DELETE FROM adapters WHERE adapter_id IN ("+this.adapterId +")";
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

    /**
     * Creates one string of adapter IDs that will be deleted.
     *
     * @param adapterId ID of first adapter
     * @param count adapters count
     * @return string containing all adapter IDs separated by comma
     */
    public String joinIds(String adapterId,int count){
        int firstId = Integer.valueOf(adapterId);
        adapterId = "";
        for(int i = 0; i < count; i++){
            adapterId = (i != 0 ? adapterId + "," : "") + (firstId+i);
        }
        return adapterId;
    }
}
