package com.iha.emulator.communication.eserver.task;

import org.dom4j.DocumentException;

/**
 * Interface for tasks, which are sent to EmulatorServer via {@link com.iha.emulator.communication.eserver.EmulatorServerClient}
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public interface ServerTask<T> {
    /**
     * Creates task in XML format
     * @return task in XML format as string
     */
    public String buildMessage();
    /**
     * Parses response from server
     * @param serverMessage EmulatorServer response as string
     * @throws DocumentException if response is not parsable as XML
     */
    public void parseResponse(String serverMessage) throws DocumentException;
    /**
     * Gets task's result
     * @return task's result
     */
    public T getResult();
    /**
     * Sets task's result
     * @param result task's result to be set
     */
    public void setResult(T result);
}
