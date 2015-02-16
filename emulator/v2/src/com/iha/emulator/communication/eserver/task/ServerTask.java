package com.iha.emulator.communication.eserver.task;

import org.dom4j.DocumentException;

/**
 * Created by Shu on 8.12.2014.
 */
public interface ServerTask<T> {
    public String buildMessage();
    public void parseResponse(String serverMessage) throws DocumentException;
    public T getResult();
    public void setResult(T result);
}
