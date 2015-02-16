package com.iha.emulator.utilities;

import java.util.ArrayList;

/**
 * Created by Shu on 11.2.2015.
 */
public class ResponseTracker {

    private boolean enabled = false;
    private boolean dumpResponses = false;
    private ArrayList<Response> responses = null;

    public ResponseTracker(boolean enabled) {
        this(enabled,null);
    }

    public ResponseTracker(Boolean enabled,Boolean dumpResponses) {
        if(enabled!=null) this.enabled = enabled;
        if(dumpResponses != null) this.dumpResponses = dumpResponses;
    }
    /**
     * Adds new {@link com.iha.emulator.utilities.Response} to {@link com.iha.emulator.utilities.ResponseTracker#responses} list.
     * Sets response value as <code>start - end</code> and time of response as <code>end</code>
     * @param start
     * @param end
     */
    public synchronized void addResponse(long start,long end){
        if(start!=0 && start < end){
            Response response = new Response(start,end);
            responses.add(response);
        }
    }
    /**
     * If there are any responses in {@link com.iha.emulator.utilities.ResponseTracker#responses}, returns value of last one, else returns <code>0L</code>
     * @return last response value or <code>0L</code>
     */
    public synchronized Long getLastResponseValue(){
        if(responses.size() != 0)
            return responses.get(responses.size()-1).getValue();
        else
            return 0L;
    }
    /**
     * Clears {@link com.iha.emulator.utilities.ResponseTracker#responses} list
     */
    public void dumpResponses(){
        responses.clear();
    }
    /**
     * Returns new {@link java.util.ArrayList} of {@link com.iha.emulator.utilities.Response}s saved since last call of this method,
     * then clears {@link com.iha.emulator.utilities.ResponseTracker#responses} list
     * @return responses saved since last call of this method
     */
    public ArrayList<Response> getNewResponses(){
        ArrayList<Response> newResponses = new ArrayList<>();
        for(int i = 0;i<responses.size();i++){
            newResponses.add(responses.get(i));
        }
        responses.clear();
        return newResponses;
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
        if(b && responses == null)
            responses = new ArrayList<>();
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
        responses = null;
    }

}
