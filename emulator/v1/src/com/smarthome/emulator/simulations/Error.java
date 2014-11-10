package com.smarthome.emulator.simulations;

/**
 * {@link java.lang.Enum} holding all possible errors for application.
 */
public enum Error {
    OPEN_DETAILED_SIMULATION(0,"Cannot start Detailed Simulation"),
    OPEN_PERFORMANCE_SIMULATION(1,"Cannot start Performance Simulation"),
    SET_LOOK_AND_FEEL(2,"Cannot set look and feel of application"),
    FONT_OPEN(3,"Cannot open font file"),
    FONT_REGISTER(4,"Cannot register font"),
    //Detailed simulation
    ADAPTER_ID_TAKEN(50,"Adapter Id is taken"),
    LOAD_DOC_CREATE(60,"Cannot create new document when loading file"),
    LOAD_DOC_READ(61,"Cannot read loaded file"),
    LOAD_DOC_PARSE(62,"Cannot parse loaded file"),
    FILE_NOT_FOUND(63,"File not found"),
    SAVE_DOC_CREATE(70,"Cannot create new document while saving"),
    //Performance simulation
    PRINT_CLEAR_DOC(100,"Cannot clear document after printing"),
    //server
    SERVER_IP_FROM_HOST(150,"Cannot get ip from given host name"),
    SERVER_HOST_FROM_IP(151,"Cannot get hostname from given IP address"),
    SERVER_NO_PORT(152,"No port added"),
    SERVER_NOT_RESPONDING(153,"Server not responding for over 10 seconds.Connection closed"),
    SERVER_CONNECT(154,"Warning: Cannot connect to server"),
    SERVER_NOT_CREATED(155,"No server created"),
    SERVER_UNREACHABLE(156,"Server unreachable!!!"),
    SERVER_FAIL_UNREACHABLE(157,"Cannot check if server is reachable"),
    //scheduler
    SCHEDULER_SLEEP(160,"Cannot put scheduler to sleep while waiting on server"),
    SCHEDULER_WAIT(161,"Cannot put scheduler to wait while waiting for new messages"),
    SCHEDULER_TERMINATE(162,"Failed to terminate scheduler thread"),
    //logger
    LOGGER_CLEAR(170,"Cannot clear logs"),
    LOGGER_SENT_CLEAR(171,"Cannot clear \"to be sent\" log"),
    LOGGER_SENT_FIRST_LINE(172,"Cannot remove \"To Be Sent\" log first line"),
    LOGGER_WRITE_LOG(173,"Cannot write to log"),
    LOGGER_WRITE_SENT(174,"Cannot write to \"to be sent\" log"),
    LOGGER_WRITE_ERROR(175,"Cannot write to \"error\" log"),
    LOGGER_PRINT(176,"Cannot create document for printing"),
    //properties
    PROPERTIES_NOT_FOUND(200,"Cannot find properties file"),
    PROPERTIES_LOAD(201,"Cannot load sensor types property file"),
    PROPERTIES_LOAD_EXTENSION(202,"Cannot load sensor types property file - wrong extension"),
    PROPERTIES_TYPES_INVALID_FOLDER(203,"Sensor types property file is invalid - folder property"),
    PROPERTIES_TYPES_INVALID_FILES(204,"Sensor types property file is invalid - files property"),
    PROPERTIES_TYPES_NOT_FOUND(205,"Sensor types property file not found"),
    PROPERTIES_TYPES_LOAD(206,"Cannot load sensor types property file"),
    PROPERTIES_ICON_INVALID_FOLDER(207,"Icons property file is invalid - folder property"),
    PROPERTIES_ICON_INVALID_FILES(208,"Icons property file is invalid - files property"),
    PROPERTIES_ICON_NOT_FOUND(209,"Icons types property file not found"),
    PROPERTIES_ICON_LOAD(210,"Cannot load icons property file"),
    PROPERTIES_NAME(211,"Wrong properties file format - missing property \"name\""),
    PROPERTIES_BCOLOR(212,"Wrong properties file format - invalid property \"backgroundColor\""),
    PROPERTIES_FCOLOR(213,"Wrong properties file format - invalid property \"fontColor\""),
    PROPERTIES_LCOLOR(214,"Wrong properties file format - invalid property \"lblFontColor\""),
    PROPERTIES_REFRESH_RATE(215,"Wrong properties file format - invalid property \"refreshRate\""),
    PROPERTIES_SIGNAL(216,"Wrong properties file format - invalid property \"signal\""),
    PROPERTIES_BATTERY(217,"Wrong properties file format - invalid property \"battery\""),
    PROPERTIES_PROTOCOL(218,"Wrong properties file format - invalid property \"protocol\""),
    PROPERTIES_MISSING(219,"Wrong properties file format - missing one or more properties"),
    PROPERTIES_MISSING_WRONG(220,"Wrong properties file format - wrong format of one or more properties"),
    PROPERTIES_INTEGER(221,"Wrong properties file format - property \"refreshRate\",\"signal\",\"battery\" not integer number or \"protocol\" not float number"),
    PROPERTIES_LOAD_MULTISET(222,"Wrong properties file format - multiset field do not have same number of values"),
    //other errors
    SENSOR_ID_FORMAT(223,"Wrong sensor id format - must be number or IP"),
    SENSOR_ID_IP_CONVERT(224,"Cannot convert IP to integer format"),
    INTERNAL_ERROR(255,"Unknown error");
    /** error number */
    private final int id;
    /** error message */
    private final String message;
    /**
     * New error with given number and message
     * @param id error number
     * @param message error message
     */
    Error(int id,String message){
        this.id = id;
        this.message = message;
    }
    /**
     * Returns error number
     * @return error number
     */
    public int getId(){
        return id;
    }
    /**
     * Returns error message
     * @return error message
     */
    public String getMessage(){
        return message;
    }
}
