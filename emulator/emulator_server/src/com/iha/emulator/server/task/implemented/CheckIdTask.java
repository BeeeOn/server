package com.iha.emulator.server.task.implemented;

import com.iha.emulator.server.Database;
import com.iha.emulator.server.DatabaseInfo;
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
public class CheckIdTask extends AbstractTask {

    private Logger logger;

    private DatabaseInfo dbInfo;
    private String adapterId;
    private Database database;

    public CheckIdTask(Logger logger,DatabaseInfo dbInfo,String adapterId) {
        this.logger = logger;
        this.dbInfo = dbInfo;
        this.adapterId = adapterId;
        this.database = new Database();
    }

    @Override
    public Element resolveTask() {
        try {
            Connection conn = database.connect(this.dbInfo);
            String sql = "SELECT adapter_id,name,version FROM adapters WHERE adapter_id="+this.adapterId;
            logger.trace("CheckId SQL: ");
            logger.trace(sql);
            ResultSet rs = database.getStatement(conn).executeQuery(sql);
            logger.trace("SQL executed successfully");
            Element result = buildPositiveResultElement();
            Element adapter = result.addElement("adapter");
            while(rs.next()){
                adapter.addAttribute("registered","true")
                            .addAttribute("adapter_id", rs.getString("adapter_id"))
                            .addAttribute("name",rs.getString("name"))
                            .addAttribute("version",rs.getString("version"));
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
