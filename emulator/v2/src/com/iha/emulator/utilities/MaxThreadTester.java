package com.iha.emulator.utilities;

import javafx.application.Platform;
import javafx.scene.control.TextArea;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Shu on 13.3.2015.
 */
public class MaxThreadTester {

    public static final int DEFAULT_BATCH_SIZE = 4000;
    private int bachSize;
    private TextArea logArea;

    public MaxThreadTester(TextArea logArea) {
        this(0,logArea);
    }

    public MaxThreadTester(int batchSize,TextArea logArea) {
        this.bachSize = batchSize;
        this.logArea = logArea;
    }

    public void startTest(){
        List<Thread> threads = new ArrayList<>();
        int currentBachSize = bachSize == 0 ? DEFAULT_BATCH_SIZE : bachSize;
        try {
            for (int i = 0; i <= 100 * 1000; i += currentBachSize) {
                long start = System.currentTimeMillis();
                addThread(threads, currentBachSize);
                long end = System.currentTimeMillis();
                Thread.sleep(1000);
                long delay = end - start;
                Platform.runLater(() -> logArea.appendText(threads.size() + " threads: Time to create "+currentBachSize+" threads was "+ delay / 1e3 +" seconds \n"));
            }
        } catch (Throwable e) {
            Platform.runLater(() -> logArea.appendText("After creating " + threads.size() +" threads, " + e.getMessage()));
            e.printStackTrace();
        }
    }

    private static void addThread(List<Thread> threads, int num) {
        for (int i = 0; i < num; i++) {
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
            threads.add(t);
            t.start();
        }
    }
}
