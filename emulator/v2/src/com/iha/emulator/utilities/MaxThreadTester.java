package com.iha.emulator.utilities;

import javafx.application.Platform;
import javafx.scene.control.TextArea;

import java.util.ArrayList;
import java.util.List;

/**
 * Class creates test of maximal threads, that can be created.
 */
public class MaxThreadTester {
    /** default thread batch size */
    public static final int DEFAULT_BATCH_SIZE = 4000;
    /** batch size*/
    private int bachSize;
    /** GUI component for notifying user of results */
    private TextArea logArea;

    /**
     * Creates instance of tester with given GUI component for notifying user of results
     * @param logArea component for notifying user of results
     */
    public MaxThreadTester(TextArea logArea) {
        this(0,logArea);
    }

    /**
     *  Creates instance of tester with given GUI component for notifying user of results and given thread batch size
     * @param batchSize thread batch size
     * @param logArea component for notifying user of results
     */
    public MaxThreadTester(int batchSize,TextArea logArea) {
        this.bachSize = batchSize;
        this.logArea = logArea;
    }

    /**
     * Method creates threads in batches and adds them to list, recording time to create each batch. Also notifies user
     * about results.
     */
    public void startTest(){
        //create threads list
        List<Thread> threads = new ArrayList<>();
        //set bach size
        int currentBachSize = bachSize == 0 ? DEFAULT_BATCH_SIZE : bachSize;
        try {
            //create batches of threads
            for (int i = 0; i <= 100 * 1000; i += currentBachSize) {
                //record time to create batch
                long start = System.currentTimeMillis();
                //start threads batch
                addThread(threads, currentBachSize);
                long end = System.currentTimeMillis();
                Thread.sleep(1000);
                long delay = end - start;
                //notify user
                Platform.runLater(() -> logArea.appendText(threads.size() + " threads: Time to create "+currentBachSize+" threads was "+ delay / 1e3 +" seconds \n"));
            }
        } catch (Throwable e) {
            //when cannot create more threads
            Platform.runLater(() -> logArea.appendText("After creating " + threads.size() +" threads, " + e.getMessage()));
            e.printStackTrace();
        }
    }

    /**
     * Create given number of threads, run them and add them to list.
     * @param threads list of threads
     * @param num number of threads to be created
     */
    private static void addThread(List<Thread> threads, int num) {
        for (int i = 0; i < num; i++) {
            //create thread
            Thread t = new Thread(() -> {
                try {
                    while (!Thread.interrupted()) {
                        Thread.sleep(1000);
                    }
                } catch (InterruptedException ignored) {
                    //
                }
            });
            t.setDaemon(true);
            t.setPriority(Thread.MIN_PRIORITY);
            //add to list
            threads.add(t);
            //run thread
            t.start();
        }
    }
}
