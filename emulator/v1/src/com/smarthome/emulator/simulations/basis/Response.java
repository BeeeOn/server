package com.smarthome.emulator.simulations.basis;

/**
 * Stores server response time and time in which was this response measured
 */
public class Response {
    /** response time */
    private Long value;
    /** time in which was response measured */
    private Long time;

    /**
     * Creates new response. Value sets as <code>start - end</code>, time sets as <code>end</code>
     * @param start time when measuring started
     * @param end time when measuring ended
     */
    public Response(long start,long end) {
        this.value = end - start;
        this.time = end;
    }
    /**
     * Returns response time
     * @return response time
     */
    public Long getValue(){
        return this.value;
    }
    /**
     * Returns time in which was value measured
     * @return time in which was value measured
     */
    public Long getTime(){
        return this.time;
    }
}
