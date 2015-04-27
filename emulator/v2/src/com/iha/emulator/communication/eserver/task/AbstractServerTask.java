package com.iha.emulator.communication.eserver.task;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.dom4j.Document;
import org.dom4j.DocumentFactory;
import org.dom4j.Element;

/**
 * Class providing basic methods for interface {@link com.iha.emulator.communication.eserver.task.ServerTask}, that
 * has to be extended when implementing task for EmulatorServer.
 *
 * @see com.iha.emulator.communication.eserver.task.implemented
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public abstract class AbstractServerTask<T> implements ServerTask<T> {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(AbstractServerTask.class);
    /** XML document factory */
    private static DocumentFactory factory = DocumentFactory.getInstance();
    /** EmulatorServer task result */
    private T result;
    /**
     * Sets task's result
     * @param result result set after parsing server response
     */
    public void setResult(T result) {
        this.result = result;
    }
    /**
     * Gets task's result
     * @return result set after parsing server response
     */
    public T getResult() {
        return result;
    }
    /**
     * Creates XML document structure
     * @return XML document structure
     */
    public Document createDocument(){
        return factory.createDocument();
    }
    /**
     * Creates and adds to given XML document structure protocol root element ( <code>emulator_server</code> )
     * and return this element.  (see <a href="https://ant-2.fit.vutbr.cz/projects/server/wiki/Emulator_server">Protocol</a> )
     * @param doc XML document, in which should be root element created
     * @param dbName attribute of root element according to protocol
     * @return created root element
     */
    public Element buildMessageRoot(Document doc, String dbName) {
        getLogger().trace("DeleteSensorsTask -> root message generating");
        return doc.addElement("emulator_server")
                .addAttribute("db", dbName);
    }
    /**
     * Gets instance of Log4j2 logger for this class
     * @return instance of Log4j2 logger for this class
     */
    public Logger getLogger(){
        return logger;
    }
}
