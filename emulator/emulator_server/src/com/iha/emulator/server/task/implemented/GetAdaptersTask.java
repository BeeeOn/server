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
 * Class implementing EmulatorServer task, that retrieves ID and name from all adapters in database.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class GetAdaptersTask extends AbstractTask {
    /** Log4j2 logger field */
    private Logger logger;
    /** database information */
    private DatabaseInfo dbInfo;
    private String attributes;
    /** instance of class providing connection to database */
    private Database database;

    /**
     * Creates EmulatorServer task with given information
     *
     * @param logger log4j2 logger
     * @param dbInfo database information
     * @param attributes needed database column name separated by comma (adapter_id,name)
     */
    public GetAdaptersTask(Logger logger,DatabaseInfo dbInfo,String attributes) {
        this.dbInfo = dbInfo;
        this.attributes = attributes;
        this.logger = logger;
        database = new Database();
    }
    /**
     * {@inheritDoc}
     */
    @Override
    public Element resolveTask(){
        try {
            Connection conn = database.connect(this.dbInfo);
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
