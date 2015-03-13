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
 * Created by Shu on 9.3.2015.
 */
public class StopWatch {

    private static final long START_TIME = 0;

    private LongProperty time;
    private StringProperty timeString;
    private Timeline timer;

    public StopWatch() {
        time = new SimpleLongProperty(START_TIME);
        timeString = new SimpleStringProperty(timeToString(START_TIME));
        init();
    }

    public void start(){
        if(timer != null) timer.play();
    }

    public void pause(){
        if(timer != null) timer.pause();
    }

    public void stop(){
        if(timer != null) timer.stop();
    }

    public void reset(){
        stop();
        setTime(START_TIME);
    }

    private String timeToString(long time){
        return Utilities.formatSeconds(time);
    }

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

    public long getTime() {
        return time.get();
    }

    public LongProperty timeProperty() {
        return time;
    }

    public void setTime(long time) {
        this.time.set(time);
        setTimeString(timeToString(time));
    }

    public String getTimeString() {
        return timeString.get();
    }

    public StringProperty timeStringProperty() {
        return timeString;
    }

    public void setTimeString(String timeString) {
        this.timeString.set(timeString);
    }
}
