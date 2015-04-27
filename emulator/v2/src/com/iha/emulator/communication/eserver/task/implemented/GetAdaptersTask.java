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
 * Class implementing task for EmulatorServer, that gets adapters
 * detail information from database.
 *
 * @see com.iha.emulator.communication.eserver.task.implemented
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class GetAdaptersTask extends AbstractServerTask<ObservableList<AdapterInfo>> {
    /** database name */
    private String dbName;
    /** attributes containing which information about adapters is needed */
    private String attributesString;
    /**
     * Creates DeleteAdapterTask for single given adapter ID in given database
     * @param dbName database name
     * @param attributes one or more attributes for SQL query, separated by comma ( adapter_id,name,version )
     */
    public GetAdaptersTask(String dbName,String attributes) {
        this.dbName = dbName;
        this.attributesString = attributes;
    }

    /**
     * Creates message for EmulatorServer in XML format. (see <a href="https://ant-2.fit.vutbr.cz/projects/server/wiki/Emulator_server">Protocol</a> )
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
     * Parses response from server, creates {@link com.iha.emulator.communication.eserver.model.AdapterInfo} with
     * result from server and sets this class as this task's result. (see <a href="https://ant-2.fit.vutbr.cz/projects/server/wiki/Emulator_server">Protocol</a>)
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
