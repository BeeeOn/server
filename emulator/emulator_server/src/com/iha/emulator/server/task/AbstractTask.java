package com.iha.emulator.server.task;

import org.dom4j.DocumentHelper;
import org.dom4j.Element;

/**
 * Class providing basic methods EmulatorServer tasks.
 *
 * @see com.iha.emulator.server.task.implemented
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public abstract class AbstractTask implements Task {
    /**
     * Builds skeleton of positive response as XML message, that will be sent to client
     * @return skeleton of positive XML message
     */
    public Element buildPositiveResultElement(){
        return  DocumentHelper.createElement("result")
                .addAttribute("state","OK");
    }
    /**
     * Builds negative response as XML message, that will be sent to client
     * @param message error message
     * @return negative response as XML message
     */
    public Element buildNegativeResultElement(String message){
        return DocumentHelper.createElement("result")
                .addAttribute("state","ERROR")
                .addText(message);
    }

}
