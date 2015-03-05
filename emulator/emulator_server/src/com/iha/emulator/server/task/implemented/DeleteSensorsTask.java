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
 * Created by Shu on 8.12.2014.
 */
public class DeleteSensorsTask extends AbstractTask {

    private Logger logger;

    private DatabaseInfo dbInfo;
    private ArrayList<String> sensorsIds;
    private Database database;

    public DeleteSensorsTask(Logger logger, DatabaseInfo dbInfo, ArrayList<String> sensorsIds) {
        this.logger = logger;
        this.dbInfo = dbInfo;
        this.sensorsIds = sensorsIds;
        this.database = new Database();
    }

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
