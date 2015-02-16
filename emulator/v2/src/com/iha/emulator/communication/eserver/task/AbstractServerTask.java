package com.iha.emulator.communication.eserver.task;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.dom4j.Document;
import org.dom4j.DocumentFactory;
import org.dom4j.Element;

/**
 * Created by Shu on 8.12.2014.
 */
public abstract class AbstractServerTask<T> implements ServerTask<T> {

    private static final Logger logger = LogManager.getLogger(AbstractServerTask.class);

    private static DocumentFactory factory = DocumentFactory.getInstance();
    private T result;

    public AbstractServerTask() {

    }

    public void setResult(T result) {
        this.result = result;
    }

    public T getResult() {
        return result;
    }

    public Document createDocument(){
        return factory.createDocument();
    }

    /**
     * <emulator_server db="home4">
     * </emulator_server>
     */
    public Element buildMessageRoot(Document doc,String dbName){
        return doc.addElement("emulator_server")
                .addAttribute("db", dbName);
    }

    public Logger getLogger(){
        return logger;
    }


}
