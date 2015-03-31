package com.iha.emulator.communication.eserver.task.implemented;

import com.iha.emulator.communication.eserver.task.AbstractServerTask;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;

import java.util.ArrayList;

/**
 * Created by Shu on 8.12.2014.
 */
public class DeleteSensorsTask extends AbstractServerTask<Boolean> {

    private String dbName;
    private ArrayList<Integer> sensors;


    public DeleteSensorsTask(String dbName,ArrayList<Integer> sensors) {
        this.dbName = dbName;
        this.sensors = sensors;
        getLogger().warn("DeleteSensorsTask -> initializing -> " + dbName + " sensors: " + sensors);
    }

    /**
     * <emulator_server db="home4">
     *     <task type="deleteSensors">
     *         <sensors>
     *             <sensor id="132123"/>
     *             <sensor id="132123"/>
     *             <sensor id="132123"/>
     *             .
     *             .
     *             .
     *         </sensors>
     *     </task>
     * </emulator_server>
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
