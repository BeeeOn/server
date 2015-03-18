package com.iha.emulator.utilities.watchers;

import javafx.animation.Animation;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.scene.control.Label;
import javafx.scene.control.ProgressBar;
import javafx.util.Duration;

import java.text.DecimalFormat;

/**
 * Created by Shu on 24.11.2014.
 */
public class MemoryChecker {

    private static MemoryChecker instance;
    private Timeline timer;
    public Runtime runtime = Runtime.getRuntime();
    public Double memory,total;

    private ProgressBar progressBar;
    private Label statusLbl;

    public static MemoryChecker getInstance(){
        if (instance == null) {
            instance = new MemoryChecker();
        }
        return instance;
    }

    public void setGuiElements(ProgressBar pb,Label lbl){
        this.progressBar = pb;
        this.statusLbl = lbl;
    }

    public void start(){
        if (timer == null) {
            if(progressBar == null || statusLbl == null ){
                throw new NullPointerException("Missing progressBar or status label in MemoryChecker");
            }
            timer = new Timeline(new KeyFrame(
                    Duration.millis(1000),
                    ae -> checkMemory()));
            timer.setCycleCount(Animation.INDEFINITE);
        }
        timer.play();
    }

    public void stop(){
        if(timer != null){
            timer.stop();
        }
        if(progressBar!=null){
            progressBar.setProgress(0);
        }
        if(statusLbl!=null){
            statusLbl.setText("(0MB of 00MB)");
        }
    }

    private void checkMemory(){
        runtime.gc();
        total =  Double.valueOf(String.valueOf(runtime.totalMemory()));
        memory = Double.valueOf(String.valueOf(total - runtime.freeMemory()));
        progressBar.setProgress(memory/runtime.totalMemory());
        statusLbl.setText("(" + customFormat("###.##", memory/1000000) + "MB" + " of " + customFormat("###.##", total/1000000) + "MB" + ")");
    }

    public static String customFormat(String pattern, double value) {
        DecimalFormat myFormatter = new DecimalFormat(pattern);
        String output = myFormatter.format(value);
        return output;
    }
}
