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
 * Created by Shu on 11.2.2015.
 */
public class ResponseTracker {

    private boolean enabled = false;
    private boolean dumpResponses = false;
    private boolean trackPerSecond = false;
    private ObservableList<Response> responses = FXCollections.observableArrayList();
    private ListProperty<Response> responsesList = new SimpleListProperty<>(responses);
    private LongProperty currentPerSecond;
    private LongProperty maxPerSecond;
    private long lastChartSecond = 0;
    private int responseIndex = 0;
    private long currentSecond = 0;
    private ResponseChart chart;

    public ResponseTracker(boolean enabled) {
        this(enabled,false,false);
    }

    public ResponseTracker(boolean enabled,boolean dumpResponses){
        this(enabled,dumpResponses,false);
    }

    public ResponseTracker(boolean enabled,boolean dumpResponses,boolean trackPerSecond) {
        this.enabled = enabled;
        this.dumpResponses = dumpResponses;
        this.trackPerSecond = trackPerSecond;

        this.currentPerSecond = new SimpleLongProperty(0);
        this.maxPerSecond = new SimpleLongProperty(0);
    }

    public void registerChart(ResponseChart chart){
        this.chart = chart;
        this.chart.clearChart();
        dumpResponsesToChart();
    }

    private void dumpResponsesToChart(){
        responses.stream().filter(r -> chart != null).forEach(chart::addResponse);
    }

    public void registerSecondCounter(LongProperty seconds){
        seconds.addListener((observable, oldValue, newValue) -> {
            //checkPerSecond();
        });
    }

    private synchronized void checkPerSecond(){
        if(!trackPerSecond) return;
        Platform.runLater(()->{
            setCurrentPerSecond(responses.size() - responseIndex);
            responseIndex = responses.size();
            if(getCurrentPerSecond() > getMaxPerSecond()) setMaxPerSecond(getCurrentPerSecond());
        });
    }

    private synchronized void checkPerSecond(long time){
        if(!trackPerSecond) return;
        if(currentSecond == 0) currentSecond = time/1000;
        if(currentSecond == (time/1000)){
            Platform.runLater(()-> setCurrentPerSecond(getCurrentPerSecond()+1));
            if(getMaxPerSecond() < getCurrentPerSecond()) Platform.runLater(() -> setMaxPerSecond(getCurrentPerSecond()));
        }else{
            currentSecond = time/1000;
            Platform.runLater(()->setCurrentPerSecond(1));
        }
    }
    /**
     * Adds new {@link Response} to {@link ResponseTracker#responses} list.
     * Sets response value as <code>start - end</code> and time of response as <code>end</code>
     * @param start
     * @param end
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
            responseIndex = 0;
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

    public synchronized void dumpLastResponse(){
        if(responses.size() != 0) responses.remove(responses.size()-1);
    }
    /**
     * Returns if response times are saved tracked
     * @return <code>true</code> enable tracking, <code>false</code> otherwise
     */
    public boolean isEnabled(){
        return enabled;
    }
    /**
     * Sets whether responses should be saved or not. Creates list of responses if there is not one.
     * @param b <code>true</code> if save responses, <code>false</code> otherwise
     */
    public void setEnabled(boolean b){
        this.enabled = b;
    }
    /**
     * Returns whether there are responses stored or not.
     * @return <code>false</code> if responses are saved, <code>true</code> otherwise
     */
    public boolean isDumpResponses(){
        return this.dumpResponses;
    }
    /**
     * Sets if responses should be stored or not
     * @param b <code>false</code> if save responses, <code>true</code> otherwise</code>
     */
    public void setDumpResponses(boolean b){
        this.dumpResponses = b;
    }

    public void delete(){
        responses.clear();
    }

    public ObservableList<Response> getResponsesList() {
        return responsesList.get();
    }

    public ListProperty<Response> responsesListProperty() {
        return responsesList;
    }

    public void setResponsesList(ObservableList<Response> responsesList) {
        this.responsesList.set(responsesList);
    }

    public boolean isTrackPerSecond() {
        return trackPerSecond;
    }

    public void setTrackPerSecond(boolean trackPerSecond) {
        this.trackPerSecond = trackPerSecond;
    }

    public long getMaxPerSecond() {
        return maxPerSecond.get();
    }

    public LongProperty maxPerSecondProperty() {
        return maxPerSecond;
    }

    public void setMaxPerSecond(long maxPerSecond) {
        this.maxPerSecond.set(maxPerSecond);
    }

    public long getCurrentPerSecond() {
        return currentPerSecond.get();
    }

    public LongProperty currentPerSecondProperty() {
        return currentPerSecond;
    }

    public void setCurrentPerSecond(long currentPerSecond) {
        this.currentPerSecond.set(currentPerSecond);
    }
}
