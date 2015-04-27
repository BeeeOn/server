package com.iha.emulator.communication.eserver.task.implemented;

import com.iha.emulator.communication.eserver.task.AbstractServerTask;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;

import java.util.ArrayList;

/**
 * Class implementing task for EmulatorServer, that deletes given sensors IDs
 * from database.
 *
 * @see com.iha.emulator.communication.eserver.task.implemented
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class DeleteSensorsTask extends AbstractServerTask<Boolean> {
    /** database name */
    private String dbName;
    /** array of sensors IDs to be deleted */
    private ArrayList<Integer> sensors;
    /**
     * Creates DeleteSensorsTask for given array of sensor IDs in given database
     * @param dbName database name
     * @param sensors array of sensors IDs, to be deleted from database
     */
    public DeleteSensorsTask(String dbName,ArrayList<Integer> sensors) {
        this.dbName = dbName;
        this.sensors = sensors;
        getLogger().trace("DeleteSensorsTask -> initializing -> " + dbName + " sensors: " + sensors);
    }
    /**
     * Creates message for EmulatorServer in XML format. (see <a href="https://ant-2.fit.vutbr.cz/projects/server/wiki/Emulator_server">Protocol</a> )
     * @return XML message as one string
     */
    @Override
    public String buildMessage() {
        getLogger().warn("DeleteSensorsTask -> building message");
        Document doc = createDocument();
        getLogger().warn("DeleteSensorsTask -> document created");
        Element task = buildMessageRoot(doc, dbName)
                .addElement("task")
                .addAttribute("type", "deleteSensors");
        getLogger().warn("DeleteSensorsTask -> task element created");
        Element sensorsElement = task.addElement("sensors");
        for(Integer sensor : sensors){
            sensorsElement.addElement("sensor")
                    .addAttribute("id",String.valueOf(sensor));
        }
        getLogger().warn("DELETE message: " + doc.asXML());
        return doc.asXML();
    }

    /**
     * Parses response from server, set result as true ( everything went OK) or
     * false if an error occurred during adapter delete. (see <a href="https://ant-2.fit.vutbr.cz/projects/server/wiki/Emulator_server">Protocol</a> )
     * @param serverMessage XML server response as string
     * @throws org.dom4j.DocumentException XML parsing errors
     */
    @Override
    public void parseResponse(String serverMessage) throws DocumentException {
        Document doc = DocumentHelper.parseText(serverMessage);
        Element result = doc.getRootElement().element("result");
        String stateString = result.attribute("state").getValue();
        if(stateString.equalsIgnoreCase("ok")){
            setResult(true);
        }else{
            setResult(false);
        }
    }
}
