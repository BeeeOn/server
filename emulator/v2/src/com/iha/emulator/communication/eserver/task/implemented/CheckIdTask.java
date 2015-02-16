package com.iha.emulator.communication.eserver.task.implemented;

import com.iha.emulator.communication.eserver.model.AdapterInfo;
import com.iha.emulator.communication.eserver.task.AbstractServerTask;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;

/**
 * Created by Shu on 8.12.2014.
 */
public class CheckIdTask extends AbstractServerTask<AdapterInfo> {

    private String dbName;
    private String checkedId;

    public CheckIdTask(String dbName,String id) {
        this.dbName = dbName;
        this.checkedId = id;
    }

    /**
     * <emulator_server db="home4>
     *     <task type="checkId">
     *         <adapter adapter_id="111" />
     *     </task>
     * </emulator_server>
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
     * <server_emulator>
     *     <result state="OK or ERROR">
     *         <adapter registered="true" adapter_id="111" name="name" version="0" />
     *         or
     *         <adapter registered="false" />
     *         or
     *         error message
     *     </result>
     * </server_emulator>
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
