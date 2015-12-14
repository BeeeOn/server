package com.iha.emulator.communication.eserver.task.implemented;

import com.iha.emulator.communication.eserver.model.AdapterInfo;
import com.iha.emulator.communication.eserver.task.AbstractServerTask;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;

/**
 * Class implementing task for EmulatorServer, that checks, if given
 * adapter ID exists in database
 *
 * @see com.iha.emulator.communication.eserver.task.implemented
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class CheckIdTask extends AbstractServerTask<AdapterInfo> {
    /** database name */
    private String dbName;
    /** adapter ID as string */
    private String checkedId;

    /**
     * Creates CheckIdTask for given adapter ID and in given database
     * @param dbName database name
     * @param id adapter ID as String
     */
    public CheckIdTask(String dbName,String id) {
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
                .addAttribute("type", "checkId")
                .addElement("adapter")
                .addAttribute("adapter_id", checkedId);
        return doc.asXML();
    }
    /**
     * Parses response from server, creates {@link com.iha.emulator.communication.eserver.model.AdapterInfo} with
     * result from server and sets this class as this task's result. (see <a href="https://ant-2.fit.vutbr.cz/projects/server/wiki/Emulator_server">Protocol</a> )
     * @param serverMessage XML server response as string
     * @throws DocumentException XML parsing errors
     */
    @Override
    public void parseResponse(String serverMessage) throws DocumentException {
        Document doc = DocumentHelper.parseText(serverMessage);
        Element result = doc.getRootElement().element("result");
        Element adapter = result.element("adapter");
        if(adapter.attributeValue("registered").equals("true")) {
            setResult(new AdapterInfo(adapter.attributeValue("adapter_id"), adapter.attributeValue("name"), adapter.attributeValue("version")));
        }else {
            setResult(null);
        }
    }
}
