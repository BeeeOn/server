package com.iha.emulator.communication.eserver.task.implemented;

import com.iha.emulator.communication.eserver.task.AbstractServerTask;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;

/**
 * Class implementing task for EmulatorServer, that deletes given adapter IDs
 * from database.
 *
 * @see com.iha.emulator.communication.eserver.task.implemented
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class DeleteAdapterTask extends AbstractServerTask<Boolean> {
    /** database name */
    private String dbName;
    /** first adapter ID, to be deleted */
    private String adapterId;
    /** number of adapters, to be deleted */
    private int count;

    /**
     * Creates DeleteAdapterTask for single given adapter ID in given database
     * @param dbName database name
     * @param id adapter ID, to be deleted from database
     */
    public DeleteAdapterTask(String dbName, String id) {
        this(dbName,id,1);
    }
    /**
     * Creates DeleteAdapterTask for multiple adapters. ( Used only by Performance Simulation, where adapters IDs are
     * are created automatically).
     * First adapter ID is given as parameter, others are incremented from this ID to given count.
     * Example (id = 1000,count = 10): deleting adapters 1000,1001,1002,..,1009
     * @param dbName database name
     * @param id first adapter ID, to be deleted
     * @param count number of adapters, to be deleted
     */
    public DeleteAdapterTask(String dbName, String id,int count) {
        this.dbName = dbName;
        this.adapterId = id;
        this.count = count;
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
                .addAttribute("type", "deleteAdapter")
                .addElement("adapter")
                .addAttribute("adapter_id", adapterId)
                .addAttribute("count", String.valueOf(count));
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
