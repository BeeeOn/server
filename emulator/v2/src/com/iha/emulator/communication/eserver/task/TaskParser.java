package com.iha.emulator.communication.eserver.task;

import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;

/**
 * Created by Shu on 8.12.2014.
 */
public class TaskParser {

    public static synchronized void parseTaskResult(String serverMessage) throws DocumentException, IllegalStateException {
        Document doc = DocumentHelper.parseText(serverMessage);
        Element result = doc.getRootElement().element("result");

        String state = result.attributeValue("state");
        if(!state.toLowerCase().equals("ok")){
            throw new IllegalStateException(result.getText());
        }
    }
}
