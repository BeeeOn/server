package com.iha.emulator.server.task.implemented;

import com.iha.emulator.server.DatabaseInfo;
import com.iha.emulator.server.task.TaskParser;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

public class DeleteAdapterTaskTest {

    private static final Logger logger = LogManager.getLogger(TaskParser.class);

    private DatabaseInfo dbInfo;
    private String adapterId;
    private int count;

    @Before
    public void setUp() throws Exception {
        adapterId = "1000";
        count = 20;
        dbInfo = new DatabaseInfo("localhost","5432","postgres","");
    }

    @After
    public void tearDown() throws Exception {

    }

    @Test
    public void testJoinIds() throws Exception {
        DeleteAdapterTask task = new DeleteAdapterTask(null,null,adapterId,count);
        assertEquals("Count = 1, id string should be \"1000\"","1000",task.joinIds(adapterId,1));
        assertEquals("Count = 5, id string should be 1000,..,..,1004","1000,1001,1002,1003,1004",task.joinIds(adapterId,5));
    }

    @Test
    public void textResolveTask() throws Exception {
        DeleteAdapterTask task = new DeleteAdapterTask(logger,dbInfo,adapterId,count);
        assertNotNull("Task should return result",task.resolveTask());
    }


}