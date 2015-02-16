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
public class DeleteAdapterTask extends AbstractServerTask<Boolean> {

    private String dbName;
    private String adapterId;

    public DeleteAdapterTask(String dbName, String id) {
        this.dbName = dbName;
        this.adapterId = id;
    }

    /**
     * <emulator_server db="home4>
     *     <task type="deleteAdapter">
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
                .addAttribute("type", "deleteAdapter")
                .addElement("adapter")
                .addAttribute("adapter_id", adapterId);
        return doc.asXML();
    }

    /**
     * <server_emulator>
     *     <result state="OK or ERROR">
     *         error message
     *     </result>
     * </server_emulator>
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
