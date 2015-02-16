package com.iha.emulator.communication.eserver.task.implemented;

import com.iha.emulator.communication.eserver.task.AbstractServerTask;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;

/**
 * Created by Shu on 8.12.2014.
 */
public class CheckSensorIdTask extends AbstractServerTask<String> {

    private String dbName;
    private String checkedId;

    public CheckSensorIdTask(String dbName, String id) {
        this.dbName = dbName;
        this.checkedId = id;
    }

    /**
     * <emulator_server db="home4>
     *     <task type="checkSensorId">
     *         <sensor sensor_id="111.111.111.111" />
     *     </task>
     * </emulator_server>
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
     * <server_emulator>
     *     <result state="OK or ERROR">
     *         <sensor registered="true" adapter_id="111" />
     *         or
     *         <sensor registered="false" />
     *         or
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
        Element adapter = result.element("sensor");
        if(adapter.attributeValue("registered").equals("true")) {
            setResult(adapter.attributeValue("adapter_id"));
        }else {
            setResult(null);
        }
    }
}
