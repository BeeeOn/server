package com.iha.emulator.server.task.implemented;

import com.iha.emulator.server.task.AbstractTask;
import com.iha.emulator.server.Database;
import org.dom4j.Element;
import org.apache.logging.log4j.Logger;

import javax.naming.NamingException;
import java.net.ConnectException;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * Created by Shu on 8.12.2014.
 */
public class GetAdaptersTask extends AbstractTask {

    private Logger logger;

    private String dbName;
    private String attributes;
    private Database database;

    public GetAdaptersTask(Logger logger,String dbName,String attributes) {
        this.dbName = dbName;
        this.attributes = attributes;
        this.logger = logger;
        database = new Database();
    }

    @Override
    public Element resolveTask(){
        try {
            Connection conn = database.connect(this.dbName);
            String sql = "SELECT " + attributes + " FROM adapters";
            logger.trace("GetAdapters SQL: ");
            logger.trace(sql);
            ResultSet rs = database.getStatement(conn).executeQuery(sql);
            logger.trace("SQL executed successfully");

            Element result = buildPositiveResultElement();
            while(rs.next()){
                result.addElement("adapter")
                        .addAttribute("adapter_id",rs.getString("adapter_id"))
                        .addAttribute("name",rs.getString("name"));
            }
            database.disconnect(conn);
            return result;
        } catch (ConnectException | ClassNotFoundException | NamingException e) {
            logger.error("Cannot connect to database",e);
            return buildNegativeResultElement("Cannot connect to database");
        } catch (SQLException e) {
            logger.error("Cannot execute SQL query", e);
            return buildNegativeResultElement("Cannot execute SQL query");
        }
    }
}
