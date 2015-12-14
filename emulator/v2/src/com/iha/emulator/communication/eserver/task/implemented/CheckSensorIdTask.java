package com.iha.emulator.communication.eserver.task.implemented;

import com.iha.emulator.communication.eserver.task.AbstractServerTask;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;

/**
 * Class implementing task for EmulatorServer, that checks, if given
 * sensor ID exists in database
 *
 * @see com.iha.emulator.communication.eserver.task.implemented
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class CheckSensorIdTask extends AbstractServerTask<String> {
    /** database name */
    private String dbName;
    /** sensor ID as string */
    private String checkedId;
    /**
     * Creates CheckSensorIdTask for given sensor ID and in given database
     * @param dbName database name
     * @param id adapter ID as String
     */
    public CheckSensorIdTask(String dbName, String id) {
        this.dbName = dbName;
        this.checkedId = id;
    }

    /**
     * Creates message for EmulatorServer in XML format. (see <a href="https://ant-2.fit.vutbr.cz/projects/server/wiki/Emulator_server">Protocol</a> )
     * @return XML message as one string
     */
    @Override
    public String buildMessage() {
        Document doc = createDocument();
        Element task = buildMessageRoot(doc,dbName)
                .addElement("task")
                .addAttribute("type", "checkSensorId")
                .addElement("sensor")
                .addAttribute("sensor_id", checkedId);
        return doc.asXML();
    }

    /**
     * Parses response from server and sets sensor ID, which owns given sensor as result. (see <a href="https://ant-2.fit.vutbr.cz/projects/server/wiki/Emulator_server">Protocol</a> )
     * @param serverMessage XML server response as string
     * @throws DocumentException XML parsing errors
     */
    @Override
    public void parseResponse(String serverMessage) throws DocumentException {
        Document doc = DocumentHelper.parseText(serverMessage);
        Element result = doc.getRootElement().element("result");
        Element adapter = result.element("sensor");
        if(adapter.attributeValue("registered").equals("true")) {
            setResult(adapter.attributeValue("adapter_id"));
        }else {
            setResult(null);
        }
    }
}
