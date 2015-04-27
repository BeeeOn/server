package com.iha.emulator.communication.eserver.task;

import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;

/**
 * Class parses response from EmulatorServer sent by {@link com.iha.emulator.communication.eserver.EmulatorServerClient#sendMessage(String)}
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class TaskParser {
    /**
     * Parses response from EmulatorServer sent by {@link com.iha.emulator.communication.eserver.EmulatorServerClient#sendMessage(String)}
     * @param serverMessage EmulatorServer response as string
     * @throws DocumentException if library cannot parse response as XML
     * @throws IllegalStateException if response from server is error, message includes EmulatorServer error message
     */
    public static synchronized void parseTaskResult(String serverMessage) throws DocumentException, IllegalStateException {
        //parse response as XML
        Document doc = DocumentHelper.parseText(serverMessage);
        //get result element
        Element result = doc.getRootElement().element("result");
        //get state element
        String state = result.attributeValue("state");
        //determine response state
        if(!state.toLowerCase().equals("ok")){
            throw new IllegalStateException(result.getText());
        }
    }
}
