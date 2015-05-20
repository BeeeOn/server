package com.iha.emulator.utilities.watchers;

import com.iha.emulator.ui.panels.chart.ResponseChart;
import javafx.application.Platform;
import javafx.beans.property.ListProperty;
import javafx.beans.property.LongProperty;
import javafx.beans.property.SimpleListProperty;
import javafx.beans.property.SimpleLongProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

import java.util.ArrayList;

/**
 * Class responsible for storing server responses time. Also create statistical information about responses times as
 * maximal responses per second or number of responses for current second.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class ResponseTracker {
    /** response tracker enabled flag */
    private boolean enabled = false;
    /** dump responses after logging them flag */
    private boolean dumpResponses = false;
    /** statistical information enabled flag */
    private boolean trackPerSecond = false;
    /** list of stored responses */
    private ObservableList<Response> responses = FXCollections.observableArrayList();
    /** list property of stored responses */
    private ListProperty<Response> responsesList = new SimpleListProperty<>(responses);
    /** number of responses in last second */
    private LongProperty currentPerSecond;
    /** maximal responses per second */
    private LongProperty maxPerSecond;
    /** current second in milliseconds */
    private long currentSecond = 0;
    /** GUI chart instance, used in "Performance simulation" */
    private ResponseChart chart;

    /**
     * Creates response tracker and sets if it is enabled according to parameter. Responses are stored and statistical
     * information gathering is disabled.
     *
     * @param enabled <code>true</code> if response tracking is enabled, <code>false</code> otherwise
     */
    public ResponseTracker(boolean enabled) {
        this(enabled,false,false);
    }

    /**
     * Creates response tracker, sets if it is enabled and if responses are stored according to parameters. Statistical
     * information gathering is disabled.
     *
     * @param enabled <code>true</code> if response tracking is enabled, <code>false</code> otherwise
     * @param dumpResponses <code>true</code> if responses shouldn't be stored, <code>false</code> otherwise
     */
    public ResponseTracker(boolean enabled,boolean dumpResponses){
        this(enabled,dumpResponses,false);
    }

    /**
     * Creates response tracker, sets if it is enabled, if responses are stored and if statistical
     * information are gathered according to parameters.
     *
     * @param enabled <code>true</code> if response tracking is enabled, <code>false</code> otherwise
     * @param dumpResponses <code>true</code> if responses shouldn't be stored, <code>false</code> otherwise
     * @param trackPerSecond <code>true</code> if statistical information should be gathered, <code>false</code> otherwise
     */
    public ResponseTracker(boolean enabled,boolean dumpResponses,boolean trackPerSecond) {
        this.enabled = enabled;
        this.dumpResponses = dumpResponses;
        this.trackPerSecond = trackPerSecond;

        this.currentPerSecond = new SimpleLongProperty(0);
        this.maxPerSecond = new SimpleLongProperty(0);
    }

    /**
     * Sets GUI representation of responses as chart. Charts data are cleared and all stored responses are added afterwards.
     * @param chart GUI representation of responses
     */
    public void registerChart(ResponseChart chart){
        this.chart = chart;
        this.chart.clearChart();
        dumpResponsesToChart();
    }
    /**
     * Adds all stored responses to previously given chart.
     */
    private void dumpResponsesToChart(){
        responses.stream().filter(r -> chart != null).forEach(chart::addResponse);
    }

    /**
     * If gathering statistical information is enabled, measures count of responses in current second and stores maximal
     * responses per second
     * @param time current time
     */
    private synchronized void checkPerSecond(long time){
        if(!trackPerSecond) return;
        //if measured second is not set, set it to current
        if(currentSecond == 0) currentSecond = time/1000;
        //measured second equals to current second
        if(currentSecond == (time/1000)){
            //increment number of responses per current second
            Platform.runLater(()-> setCurrentPerSecond(getCurrentPerSecond()+1));
            //check if number of responses in current second is not greater than max responses per second
            if(getMaxPerSecond() < getCurrentPerSecond()) Platform.runLater(() -> setMaxPerSecond(getCurrentPerSecond()));
        }else{
            //set current measured second
            currentSecond = time/1000;
            Platform.runLater(()->setCurrentPerSecond(1));
        }
    }
    /**
     * Adds new {@link Response} to {@link ResponseTracker#responses} list. If chart is assigned, adds response to chart
     * as well. Also if statistical information gathering is enabled, measuring method is invoked
     * Sets response value as <code>start - end</code> and time of response as <code>end</code>
     * @param start response start time in milliseconds
     * @param end response end time in milliseconds
     */
    public synchronized void addResponse(long start,long end){
        if(responses != null && start!=0 && start < end){
            Response response = new Response(start,end);
            responses.add(response);
            if(chart!=null) chart.addResponse(response);
        }
        checkPerSecond(end);
    }
    /**
     * If there are any responses in {@link ResponseTracker#responses}, returns value of last one, else returns <code>0L</code>
     * @return last response value or <code>0L</code>
     */
    public synchronized Long getLastResponseValue(){
        if(responses.size() != 0)
            return responses.get(responses.size()-1).getValue();
        else
            return 0L;
    }

    /**
     * Clears all stored responses from list and from chart as well. If statistical information gathering is enabled,
     * resets information counters.
     */
    public synchronized void clearResponses(){
        if(responses != null) responses.clear();
        if(chart != null){
            chart.clearChart();
            chart = null;
        }
        if(isTrackPerSecond()) {
            Platform.runLater(()->{
                setCurrentPerSecond(0);
                setMaxPerSecond(0);
            });
            currentSecond = 0;
        }
    }
    /**
     * Clears {@link ResponseTracker#responses} list
     */
    public synchronized void dumpResponses(){
        responses.clear();
    }
    /**
     * Returns new {@link java.util.ArrayList} of {@link Response}s saved since last call of this method,
     * then clears {@link ResponseTracker#responses} list
     * @return responses saved since last call of this method
     */
    public synchronized ArrayList<Response> getNewResponses(){
        ArrayList<Response> newResponses = new ArrayList<>();
        for(int i = 0;i<responses.size();i++){
            newResponses.add(responses.get(i));
        }
        responses.clear();
        return newResponses;
    }

    /**
     * Removes last response from list.
     */
    public synchronized void dumpLastResponse(){
        if(responses.size() != 0) responses.remove(responses.size()-1);
    }
    /**
     * Returns if response times are stored
     * @return <code>true</code> enable tracking, <code>false</code> otherwise
     */
    public boolean isEnabled(){
        return enabled;
    }
    /**
     * Sets whether responses should be stored or not
     * @param b <code>true</code> if save responses, <code>false</code> otherwise
     */
    public void setEnabled(boolean b){
        this.enabled = b;
    }
    /**
     * Returns whether responses are being stored or not.
     * @return <code>false</code> if responses are saved, <code>true</code> otherwise
     */
    public boolean isDumpResponses(){
        return this.dumpResponses;
    }
    /**
     * Sets if responses should be stored or not
     * @param b <code>false</code> if save responses, <code>true</code> otherwise
     */
    public void setDumpResponses(boolean b){
        this.dumpResponses = b;
    }

    /**
     * Clears responses list
     */
    public void delete(){
        responses.clear();
    }

    /**
     * Gets list of stored responses
     * @return list of stored responses
     */
    public ObservableList<Response> getResponsesList() {
        return responsesList.get();
    }

    /**
     * Stored responses list property, that can be bound
     * @return stored responses list property
     */
    public ListProperty<Response> responsesListProperty() {
        return responsesList;
    }

    /**
     * Sets list of stored responses
     * @param responsesList list of stored responses
     */
    public void setResponsesList(ObservableList<Response> responsesList) {
        this.responsesList.set(responsesList);
    }

    /**
     * Gets flag, if gathering of statistical information is enabled
     * @return <code>true</code> if enabled, <code>false</code> otherwise
     */
    public boolean isTrackPerSecond() {
        return trackPerSecond;
    }

    /**
     * Sets flag, if gathering of statistical information is enabled
     * @param trackPerSecond <code>true</code> if enabled, <code>false</code> otherwise
     */
    public void setTrackPerSecond(boolean trackPerSecond) {
        this.trackPerSecond = trackPerSecond;
    }

    /**
     * Gets maximal responses per second
     * @return maximal responses per second
     */
    public long getMaxPerSecond() {
        return maxPerSecond.get();
    }

    /**
     * Maximal responses per second property, that can be bound
     * @return maximal responses per second property
     */
    public LongProperty maxPerSecondProperty() {
        return maxPerSecond;
    }

    /**
     * Sets maximal responses per second
     * @param maxPerSecond maximal responses per second
     */
    public void setMaxPerSecond(long maxPerSecond) {
        this.maxPerSecond.set(maxPerSecond);
    }

    /**
     * Gets number of responses in current second
     * @return number of responses in current second
     */
    public long getCurrentPerSecond() {
        return currentPerSecond.get();
    }

    /**
     * Number of responses in current second property, can be bound
     * @return number of responses in current second property
     */
    public LongProperty currentPerSecondProperty() {
        return currentPerSecond;
    }

    /**
     * Sets number of responses in current second
     * @param currentPerSecond number of responses in current second
     */
    public void setCurrentPerSecond(long currentPerSecond) {
        this.currentPerSecond.set(currentPerSecond);
    }
}
