package com.iha.emulator.server.task;

import com.iha.emulator.server.DatabaseInfo;
import com.iha.emulator.server.task.implemented.*;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.dom4j.Element;

import java.util.ArrayList;
import java.util.Iterator;

/**
 * Created by Shu on 8.12.2014.
 */
public class TaskParser {

    private static final Logger logger = LogManager.getLogger(TaskParser.class);

    public static synchronized Task parseTask(DatabaseInfo dbInfo,Element taskElement){
        //extract type from task element
        String type = taskElement.attributeValue("type");
        logger.trace("Found task -> " + type);
        Task task;
        switch (type.toLowerCase()){
            case "getadapters":
                task = new GetAdaptersTask(
                        logger,
                        dbInfo,
                        taskElement.attributeValue("attributes")
                );
                break;
            case "checkid":
                task = new CheckIdTask(
                        logger,
                        dbInfo,
                        taskElement.element("adapter").attributeValue("adapter_id")
                );
                break;
            case "checksensorid":
                task = new CheckSensorIdTask(
                        logger,
                        dbInfo,
                        taskElement.element("sensor").attributeValue("sensor_id")
                );
                break;
            case "deleteadapter":
                Integer count;
                try{
                    String countString = taskElement.element("adapter").attributeValue("count");
                    count = Integer.valueOf(countString);
                }catch (NullPointerException e){
                    logger.warn("No adapter count found, setting to 1.");
                    count = 1;
                }
                task = new DeleteAdapterTask(
                        logger,
                        dbInfo,
                        taskElement.element("adapter").attributeValue("adapter_id"),
                        count
                );
                break;
            case "deletesensors":
                ArrayList<String> sensorsIds = new ArrayList<>();
                for(Iterator i = taskElement.element("sensors").elementIterator();i.hasNext();){
                    sensorsIds.add("\'" + ((Element)i.next()).attribute("id").getValue() + "\'");
                }
                task = new DeleteSensorsTask(
                        logger,
                        dbInfo,
                        sensorsIds
                );
                break;
            default:
                logger.error("Task -> " + type + " NOT found" );
                task = null;
                break;
        }
        return task;
    }
}
