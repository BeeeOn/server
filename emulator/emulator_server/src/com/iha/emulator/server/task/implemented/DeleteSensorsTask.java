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
import java.util.ArrayList;

/**
 * Class implementing EmulatorServer task, that deletes given sensor IDs from database.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class DeleteSensorsTask extends AbstractTask {
    /** Log4j2 logger field */
    private Logger logger;
    /** database information */
    private DatabaseInfo dbInfo;
    private ArrayList<String> sensorsIds;
    /** instance of class providing connection to database */
    private Database database;

    /**
     * Creates EmulatorServer task with given information
     *
     * @param logger log4j2 logger
     * @param dbInfo database information
     * @param sensorsIds array of sensors' IDs, that will be deleted
     */
    public DeleteSensorsTask(Logger logger, DatabaseInfo dbInfo, ArrayList<String> sensorsIds) {
        this.logger = logger;
        this.dbInfo = dbInfo;
        this.sensorsIds = sensorsIds;
        this.database = new Database();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Element resolveTask() {
        try {
            Connection conn = database.connect(this.dbInfo);
            String sql = "DELETE FROM facilities WHERE mac IN (" + concatStringsWSep(sensorsIds,",") + ")";
            logger.trace("DeleteSensors SQL: ");
            logger.trace(sql);
            int resultCount = database.getStatement(conn).executeUpdate(sql);
            if(resultCount == 0) throw new SQLException("None of sensors are in database");
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
     * Creates one string of sensor IDs that will be deleted.
     *
     * @param strings array of sensors' IDs
     * @param separator IDs separator
     * @return string containing all adapter IDs separated by given separator
     */
    public static String concatStringsWSep(Iterable<String> strings, String separator) {
        StringBuilder sb = new StringBuilder();
        String sep = "";
        for(String s: strings) {
            sb.append(sep).append(s);
            sep = separator;
        }
        return sb.toString();
    }
}
