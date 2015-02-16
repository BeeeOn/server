package com.iha.emulator.server.task;

import org.dom4j.DocumentHelper;
import org.dom4j.Element;

/**
 * Created by Shu on 8.12.2014.
 */
public abstract class AbstractTask implements Task {

    public AbstractTask() {

    }

    public Element buildPositiveResultElement(){
        return  DocumentHelper.createElement("result")
                .addAttribute("state","OK");
    }

    public Element buildNegativeResultElement(String message){
        return DocumentHelper.createElement("result")
                .addAttribute("state","ERROR")
                .addText(message);
    }

}
