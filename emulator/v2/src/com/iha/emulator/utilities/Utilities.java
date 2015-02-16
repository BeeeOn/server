package com.iha.emulator.utilities;

import com.iha.emulator.Main;
import com.iha.emulator.control.AdapterController;
import com.iha.emulator.control.SensorController;
import com.iha.emulator.models.Server;
import javafx.beans.property.LongProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.scene.paint.Color;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.*;
import java.nio.ByteBuffer;
import java.nio.channels.Channels;
import java.nio.channels.ReadableByteChannel;
import java.nio.channels.WritableByteChannel;
import java.util.Properties;
import java.util.Random;
import java.util.prefs.Preferences;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created by Shu on 24.11.2014.
 */
public class Utilities {
    private static final Logger logger = LogManager.getLogger(Utilities.class);

    private static final String PATH_TO_RESOURCES = "/com/iha/emulator/resources/";
    private static final String PROPERTIES_FILENAME = "emu.properties";
    private static Utilities instance;
    Preferences prefs = Preferences.userNodeForPackage(Main.class);

    private Pattern pattern;
    private Matcher matcher;

    private static final String IPADDRESS_PATTERN =
            "^([01]?\\d\\d?|2[0-4]\\d|25[0-5])\\." +
                    "([01]?\\d\\d?|2[0-4]\\d|25[0-5])\\." +
                    "([01]?\\d\\d?|2[0-4]\\d|25[0-5])\\." +
                    "([01]?\\d\\d?|2[0-4]\\d|25[0-5])$";

    public static Utilities getInstance(){
        if(instance==null){
            instance = new Utilities();
        }
        return instance;
    }

    public Properties loadDefaultProperties(String propertiesFileName) throws IOException {
        logger.trace("Loading default settings from .properties");
        InputStream input;
        Properties prop = new Properties();
        if(!checkIfExists(PROPERTIES_FILENAME)){
            logger.debug("File {} doesn't exist, creating new from default",PROPERTIES_FILENAME);
            copyFromResourceToFile(PATH_TO_RESOURCES + propertiesFileName, PROPERTIES_FILENAME);
        }
        input = new FileInputStream(PROPERTIES_FILENAME);
        prop.load(input);
        input.close();
        logger.trace("OK");
        logger.trace("Properties sample: " + " version->" + prop.getProperty("version"));
        return prop;
    }

    public static ObservableList<Server> buildServersFromProperties(Properties properties,LongProperty progress) throws IllegalArgumentException{
        ObservableList<Server> servers = FXCollections.observableArrayList();
        String serversProperty = properties.getProperty("servers", null);
        progress.set(10);
        if(serversProperty == null) throw new IllegalArgumentException("Cannot find property \"servers\" in properties file");
        String[] serversArray = serversProperty.split(",");
        for (String serverProperty : serversArray){
            String tmpServer = properties.getProperty(serverProperty + "_server_ip", null);
            if(tmpServer == null || tmpServer.equals("")) throw new IllegalArgumentException("Cannot find property \"" + serverProperty + "_server_ip\" in properties file");
            String tmpPort = properties.getProperty(serverProperty + "_server_port", null);
            if(tmpPort == null || tmpPort.equals("")) throw new IllegalArgumentException("Cannot find property \"" + serverProperty + "_server_port\" in properties file");
            String tmpDbName = properties.getProperty(serverProperty + "_server_db", null);
            if(tmpDbName == null || tmpDbName.equals("")) throw new IllegalArgumentException("Cannot find property \"" + serverProperty + "_server_db\" in properties file");
            try {
                Server server = new Server(false,serverProperty,tmpServer,Integer.valueOf(tmpPort),tmpDbName);
                servers.add(server);
                progress.set(progress.get()+(90/serversArray.length));
            } catch (NumberFormatException en){
                throw new IllegalArgumentException("Cannot parse port to integer on property \"" + serverProperty +"_server_port\" in properties file",en);
            }
        }
        return servers;
    }

    public Properties loadUserPrefs(Properties defaultProperties){
        if(defaultProperties == null){
            return null;
        }
        //last opened file location
        logger.trace("Loading user settings from Preferences");
        String lastOpenedFile = prefs.get("lastOpenedFile",null);
        if(lastOpenedFile != null) {
            defaultProperties.setProperty("lastOpenedFile",lastOpenedFile);
        }
        String servers = prefs.get("servers",null);
        if(servers != null) {
            defaultProperties.setProperty("servers",servers);
        }
        String[] serversArray = defaultProperties.getProperty("servers").split(",");
        for (String server : serversArray){
            String tmpServer = prefs.get(server + "_server_ip",null);
            if(tmpServer != null){
                defaultProperties.setProperty(server + "_server_ip",tmpServer);
            }
            String tmpPort = prefs.get(server + "_server_port",null);
            if(tmpPort != null){
                defaultProperties.setProperty(server + "_server_port",tmpPort);
            }
        }
        logger.trace("OK");
        return defaultProperties;
    }

    public void setLastOpenedFile(String path) {
        logger.trace("Setting last opened file: " + path);
        if (path != null) {
            prefs.put("lastOpenedFile",path);
        }else{
            prefs.remove("lastOpenedFile");
        }
    }

    public static synchronized String intToIpString(int i){
        return ((i >> 24 ) & 0xFF) + "." +

                ((i >> 16 ) & 0xFF) + "." +

                ((i >> 8 ) & 0xFF) + "." +

                ( i & 0xFF);
    }

    public static synchronized int ipStringToInt(String ip){
        String[] ipArray = ip.split("\\.");
        int num = 0;

        for (int i=0;i<ipArray.length;i++) {
            int power = 3-i;
            num += ((Integer.parseInt(ipArray[i])%256 * Math.pow(256,power)));
        }
        return num;
    }

    public static boolean containsIgnoreCase( String haystack, String needle ) {
        if(haystack == null || needle == null || haystack .equals(""))
            return false;
        if(needle.equals(""))
            return true;
        Pattern p = Pattern.compile(needle,Pattern.CASE_INSENSITIVE+Pattern.LITERAL);
        Matcher m = p.matcher(haystack);
        return m.find();
    }

    public static boolean isSensorIdTaken(AdapterController controller,String sensorId){
        if(sensorId.equals("")) return false;
        int id = Utilities.ipStringToInt(sensorId);
        for(SensorController sController : controller.getSensorControllersList()){
            if(sController.getModel().getId() == id){
                return true;
            }
        }
        return false;
    }

    public static boolean isIp(String ipString){
        Pattern pattern = Pattern.compile(IPADDRESS_PATTERN);
        Matcher matcher = pattern.matcher(ipString);
        return matcher.matches();
    }

    public static boolean isIntegerNumber(String number,int minDigits,int maxDigits){
        Pattern pattern = Pattern.compile("\\d{"+minDigits+","+maxDigits+"}");
        Matcher matcher = pattern.matcher(number);
        return matcher.matches();
    }

    public static String formatSeconds(int seconds){
        if(seconds < 60) {
            return seconds + " s";
        }else if(seconds < 3600){
            return (seconds/60) + " m";
        }else {
            return (seconds/3600) + " h";
        }
    }

    public static int refreshSliderScaleToSeconds(double scaleNo) {
        Double scaleDouble = scaleNo;
        int seconds;
        switch (scaleDouble.intValue()) {
            case 0:
                seconds = 1;
                break;
            case 1:
                seconds = 5;
                break;
            case 2:
                seconds = 10;
                break;
            case 3:
                seconds = 20;
                break;
            case 4:
                seconds = 30;
                break;
            case 5:
                seconds = 60;
                break;
            case 6:
                seconds = 300;
                break;
            case 7:
                seconds = 600;
                break;
            case 8:
                seconds = 900;
                break;
            case 9:
                seconds = 1800;
                break;
            case 10:
                seconds = 3600;
                break;
            case 11:
                seconds = 7200;
                break;
            case 12:
                seconds = 10800;
                break;
            case 13:
                seconds = 14400;
                break;
            case 14:
                seconds = 28800;
                break;
            case 15:
                seconds = 43200;
                break;
            case 16:
                seconds = 86400;
                break;
            default:
                seconds = 0;
                break;
        }
        return seconds;
    }

    public static double secondsToRefershSliderScale(int seconds) {
        int scaleNo;
        switch (seconds) {
            case 1:
                scaleNo = 0;
                break;
            case 5:
                scaleNo = 1;
                break;
            case 10:
                scaleNo = 2;
                break;
            case 20:
                scaleNo = 3;
                break;
            case 30:
                scaleNo = 4;
                break;
            case 60:
                scaleNo = 5;
                break;
            case 300:
                scaleNo = 6;
                break;
            case 600:
                scaleNo = 7;
                break;
            case 900:
                scaleNo = 8;
                break;
            case 1800:
                scaleNo = 9;
                break;
            case 3600:
                scaleNo = 10;
                break;
            case 7200:
                scaleNo = 11;
                break;
            case 10800:
                scaleNo = 12;
                break;
            case 14400:
                scaleNo = 13;
                break;
            case 28800:
                scaleNo = 14;
                break;
            case 43200:
                scaleNo = 15;
                break;
            case 86400:
                scaleNo = 16;
                break;
            default:
                scaleNo = 1;
                break;
        }
        Integer i = new Integer(scaleNo);
        return i.doubleValue();
    }

    public static String toRGBCode( Color color )
    {
        return String.format( "#%02X%02X%02X",
                (int)( color.getRed() * 255 ),
                (int)( color.getGreen() * 255 ),
                (int)( color.getBlue() * 255 ) );
    }
    /**
     * Generates new value from current one with accuracy given as parameters.
     * Accuracy = 10 for 1 decimal, 100 for to decimals, ...
     */
    public static double normalDistribution(Random generator,int accuracy,double dev,double avg,double max, double min){
        if(generator == null) throw new NullPointerException("Generator is null. Cannot generate new value");
        double newValue;
        do{
            double val = generator.nextGaussian() * dev + avg;
            newValue = Math.round(val * accuracy) / accuracy;
        }while(newValue > max || newValue < min);
        logger.trace("Generating new value with normal distribution: newVal -> " + newValue);
        return newValue;
    }


    private boolean checkIfExists(String filename){
        File file = new File(filename);
        return file.exists();
    }

    private void copyFromResourceToFile(String fromResource,String toFile) throws IOException{
        logger.trace("Copying properties from: " + fromResource);
        logger.trace("Copying properties to: " + toFile);
        InputStream input;
        input = getClass().getResourceAsStream(fromResource);
        OutputStream out = new FileOutputStream(toFile);
        final ReadableByteChannel inByteChannel = Channels.newChannel(input);
        final WritableByteChannel outByteChannel = Channels.newChannel(out);
        fastChannelCopy(inByteChannel,outByteChannel);
        inByteChannel.close();
        outByteChannel.close();
    }

    private static void fastChannelCopy(final ReadableByteChannel src, final WritableByteChannel dest) throws IOException {
        final ByteBuffer buffer = ByteBuffer.allocateDirect(16 * 1024);
        while (src.read(buffer) != -1) {
            // prepare the buffer to be drained
            buffer.flip();
            // write to the channel, may block
            dest.write(buffer);
            // If partial transfer, shift remainder down
            // If buffer is empty, same as doing clear()
            buffer.compact();
        }
        // EOF will leave buffer in fill state
        buffer.flip();
        // make sure the buffer is fully drained.
        while (buffer.hasRemaining()) {
            dest.write(buffer);
        }
    }
}
