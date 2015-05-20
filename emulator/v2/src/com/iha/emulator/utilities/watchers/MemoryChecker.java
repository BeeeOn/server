package com.iha.emulator.utilities.watchers;

import javafx.animation.Animation;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.scene.control.Label;
import javafx.scene.control.ProgressBar;
import javafx.util.Duration;

import java.text.DecimalFormat;

/**
 * Class implemented by Singleton pattern. Used to continuously check state of operating system memory usage. Uses own timer
 * set to 1 second.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class MemoryChecker {
    /** class instance */
    private static MemoryChecker instance;
    /** timer */
    private Timeline timer;
    /** runtime information */
    public Runtime runtime = Runtime.getRuntime();
    /** current memory usage */
    public Double memory;
    /** total memory allocated for JVM */
    public Double total;
    /** GUI component representing usage */
    private ProgressBar progressBar;
    /** label with usage information */
    private Label statusLbl;

    /**
     * Gets instance of this class
     * @return instance of this class
     */
    public static MemoryChecker getInstance(){
        if (instance == null) {
            instance = new MemoryChecker();
        }
        return instance;
    }

    /**
     * Sets GUI components representing memory usage
     * @param pb progress bar
     * @param lbl label
     */
    public void setGuiElements(ProgressBar pb,Label lbl){
        this.progressBar = pb;
        this.statusLbl = lbl;
    }

    /**
     * Starts timer ( 1 second interval )
     */
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

    /**
     * Stops timer, clears GUI components
     */
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

    /**
     * Checks current memory usage
     */
    private void checkMemory(){
        runtime.gc();
        total =  Double.valueOf(String.valueOf(runtime.totalMemory()));
        memory = Double.valueOf(String.valueOf(total - runtime.freeMemory()));
        progressBar.setProgress(memory/runtime.totalMemory());
        statusLbl.setText("(" + customFormat("###.##", memory/1000000) + "MB" + " of " + customFormat("###.##", total/1000000) + "MB" + ")");
    }

    /**
     * Formats memory usage.
     * @param pattern pattern
     * @param value usage value
     * @return string representation of memory usage
     */
    public static String customFormat(String pattern, double value) {
        DecimalFormat myFormatter = new DecimalFormat(pattern);
        String output = myFormatter.format(value);
        return output;
    }
}
