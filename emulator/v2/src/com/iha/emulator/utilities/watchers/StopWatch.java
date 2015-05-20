package com.iha.emulator.utilities.watchers;

import com.iha.emulator.utilities.Utilities;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.beans.property.LongProperty;
import javafx.beans.property.SimpleLongProperty;
import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;
import javafx.util.Duration;

/**
 * Class used to measure duration, for example of {@link com.iha.emulator.control.task.SimulationTask}.
 * Has it's own timer.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class StopWatch {
    /** default duration start time */
    private static final long START_TIME = 0;
    /** measured duration */
    private LongProperty time;
    /** measured duration as string property */
    private StringProperty timeString;
    /** timer for measuring duration */
    private Timeline timer;

    /**
     * Creates and initialises StopWatch. Sets duration time to 0.
     */
    public StopWatch() {
        time = new SimpleLongProperty(START_TIME);
        timeString = new SimpleStringProperty(timeToString(START_TIME));
        init();
    }

    /**
     * Start duration timer
     */
    public void start(){
        if(timer != null) timer.play();
    }

    /**
     * Pauses duration timer
     */
    public void pause(){
        if(timer != null) timer.pause();
    }

    /**
     * Stops duration timer
     */
    public void stop(){
        if(timer != null) timer.stop();
    }

    /**
     * Resets duration timer
     */
    public void reset(){
        stop();
        setTime(START_TIME);
    }

    /**
     * Converts measured time in seconds to formatted string ( 1h : 1m : 1s )
     * @param time measured time
     * @return formatted time as string
     */
    private String timeToString(long time){
        return Utilities.formatSeconds(time);
    }

    /**
     * Initialises timer
     */
    private void init(){
        timer = new Timeline();
        timer.setCycleCount(Timeline.INDEFINITE);
        timer.getKeyFrames().add(
                new KeyFrame(
                        Duration.seconds(1),
                        event -> {
                            setTime(getTime()+1);
                        }
                ));
    }

    /**
     * Gets duration
     * @return duration
     */
    public long getTime() {
        return time.get();
    }

    /**
     * Duration property, that can be bound
     * @return duration property
     */
    public LongProperty timeProperty() {
        return time;
    }

    /**
     * Sets duration. Also sets string duration representation
     * @param time duration
     */
    public void setTime(long time) {
        this.time.set(time);
        setTimeString(timeToString(time));
    }

    /**
     * Gets duration as formatted string
     * @return duration as formatted string
     */
    public String getTimeString() {
        return timeString.get();
    }

    /**
     * Duration as formatted string property, that can be bound
     * @return duration as formatted string property
     */
    public StringProperty timeStringProperty() {
        return timeString;
    }

    /**
     * Sets duration as formatted string
     * @param timeString duration as formatted string
     */
    public void setTimeString(String timeString) {
        this.timeString.set(timeString);
    }
}
