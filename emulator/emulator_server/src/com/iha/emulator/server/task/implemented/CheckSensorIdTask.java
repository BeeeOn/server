package com.iha.emulator.server.task.implemented;

import com.iha.emulator.server.Database;
import com.iha.emulator.server.task.AbstractTask;
import org.apache.logging.log4j.Logger;
import org.dom4j.Element;

import javax.naming.NamingException;
import java.net.ConnectException;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * Created by Shu on 8.12.2014.
 */
public class CheckSensorIdTask extends AbstractTask {

    private Logger logger;

    private String dbName;
    private String sensorId;
    private Database database;

    public CheckSensorIdTask(Logger logger, String dbName, String sensorId) {
        this.logger = logger;
        this.dbName = dbName;
        this.sensorId = sensorId;
        this.database = new Database();
    }

    @Override
    public Element resolveTask() {
        try {
            Connection conn = database.connect(this.dbName);
            String sql = "SELECT fk_adapter_id FROM facilities WHERE mac=\'"+this.sensorId + "\'";
            logger.trace("CheckId SQL: ");
            logger.trace(sql);
            ResultSet rs = database.getStatement(conn).executeQuery(sql);
            logger.trace("SQL executed successfully");
            Element result = buildPositiveResultElement();
            Element adapter = result.addElement("sensor");
            while(rs.next()){
                adapter.addAttribute("registered", "true")
                            .addAttribute("adapter_id", rs.getString("fk_adapter_id"));
            }
            if(adapter.attributes().size()== 0){
                adapter.addAttribute("registered","false");
            }
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
