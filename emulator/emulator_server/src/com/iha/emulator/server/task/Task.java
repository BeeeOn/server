package com.iha.emulator.server.task;

import org.dom4j.Element;

/**
 * Interface for tasks, which are received by EmulatorServer. Provides methods to resolve received tasks.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public interface Task {
    /**
     * Executes database statement and build response for client
     * @return statement response to be sent to client as XML
     */
    public Element resolveTask();
}
