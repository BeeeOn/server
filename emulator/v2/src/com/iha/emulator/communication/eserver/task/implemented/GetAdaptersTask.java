package com.iha.emulator.communication.eserver.task.implemented;

import com.iha.emulator.communication.eserver.model.AdapterInfo;
import com.iha.emulator.communication.eserver.task.AbstractServerTask;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;

import java.util.ArrayList;
import java.util.stream.Collectors;

/**
 * Created by Shu on 8.12.2014.
 */
public class GetAdaptersTask extends AbstractServerTask<ObservableList<AdapterInfo>> {

    private String dbName;
    private String attributesString;

    public GetAdaptersTask(String dbName,String attributes) {
        this.dbName = dbName;
        this.attributesString = attributes;
    }

    /**
     * <emulator_server db="home4>
     *     <task type="getAdapters" attributes="adapter_id,name" />
     * </emulator_server>
     * @return XML message as one string
     */
    @Override
    public String buildMessage() {
        Document doc = createDocument();
        buildMessageRoot(doc,dbName)
                .addElement("task")
                .addAttribute("type","getAdapters")
                .addAttribute("attributes", this.attributesString);
        return doc.asXML();
    }

    /**
     * <server_emulator>
     *     <result state="OK or ERROR">
     *         <adapter adapter_id="111" name="name" /> or error message
     *     </result>
     * </server_emulator>
     * @param serverMessage XML server response as string
     * @throws DocumentException XML parsing errors
     */
    @Override
    public void parseResponse(String serverMessage) throws DocumentException {
        ObservableList<AdapterInfo> data = FXCollections.observableArrayList();
        Document doc = DocumentHelper.parseText(serverMessage);
        Element result = doc.getRootElement().element("result");
        ArrayList<Element> adapters = new ArrayList<>(result.elements("adapter"));
        data.addAll(adapters.stream().map(
                adapter -> new AdapterInfo(adapter.attributeValue("adapter_id"), adapter.attributeValue("name")))
                    .collect(Collectors.toList()));
        this.setResult(data);
    }
}
