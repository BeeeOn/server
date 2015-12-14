package com.iha.emulator.utilities;

import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.control.AdapterController;
import com.iha.emulator.control.SensorController;
import com.iha.emulator.models.Server;
import javafx.application.Platform;
import javafx.beans.property.LongProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.concurrent.Task;
import javafx.event.EventHandler;
import javafx.scene.control.Alert;
import javafx.scene.control.ChoiceDialog;
import javafx.scene.control.DialogEvent;
import javafx.scene.paint.Color;
import javafx.stage.FileChooser;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.stage.StageStyle;
import org.apache.logging.log4j.Level;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.core.LoggerContext;
import org.apache.logging.log4j.core.config.Configuration;
import org.controlsfx.dialog.ExceptionDialog;
import org.controlsfx.dialog.ProgressDialog;

import java.io.*;
import java.nio.ByteBuffer;
import java.nio.channels.Channels;
import java.nio.channels.ReadableByteChannel;
import java.nio.channels.WritableByteChannel;
import java.util.Properties;
import java.util.Random;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Class implemented by Singleton pattern. Provides method used across all of application, for example converting,
 * distribution generators, loading of properties, showing dialogs, etc.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class Utilities {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(Utilities.class);
    /** refresh time in seconds representing mobile application refresh time options: 1s, 5s, 10s, 20, 30s, 1m, 5m, 10m, ... */
    private static final int[] REFERSH_TIME_OPTIONS = {1,5,10,20,30,60,300,600,900,1800,3600,7200,10800,14400,28800,43200,86400};
    /** number of GUI slider for refresh time values */
    public static final double REFRESH_SLIDER_VALUE_COUNT = 16;
    /** path to directory with application resources */
    private static final String PATH_TO_RESOURCES = "/com/iha/emulator/resources/";
    /** default application properties filename */
    private static final String PROPERTIES_FILENAME = "emu.properties";
    /** singleton instance */
    private static Utilities instance;

    /**
     * Get singleton instance of this class
     * @return instance of this class
     */
    public static Utilities getInstance(){
        if(instance==null){
            instance = new Utilities();
        }
        return instance;
    }

    /**
     * Gets list of all implemented protocol versions
     * @return list of all implemented protocol versions
     */
    public static ObservableList<Protocol.Version> getProtocolVersions(){
        return FXCollections.observableArrayList(Protocol.Version.values());
    }

    /**
     * Method loads .properties file, with filename given as parameter, and loads it's properties. If filename cannot be
     * found, one is created from resources.
     * @param propertiesFileName .properties file location
     * @return properties loaded from .properties file
     * @throws IOException cannot copy properties file from resources
     */
    public Properties loadDefaultProperties(String propertiesFileName) throws IOException {
        logger.trace("Loading default settings from .properties");
        InputStream input;
        Properties prop = new Properties();
        //check if given file exists
        if(!checkIfExists(PROPERTIES_FILENAME)){
            //if not, copy default properties file from resources
            logger.debug("File {} doesn't exist, creating new from default",PROPERTIES_FILENAME);
            copyFromResourceToFile(PATH_TO_RESOURCES + propertiesFileName, PROPERTIES_FILENAME);
        }
        //load file
        input = new FileInputStream(PROPERTIES_FILENAME);
        prop.load(input);
        input.close();
        logger.trace("OK");
        logger.trace("Properties sample: " + " version->" + prop.getProperty("version"));
        return prop;
    }

    /**
     * Build {@link com.iha.emulator.models.Server} models from properties file.
     * @param properties properties with server information
     * @param progress loading dialog progress property
     * @return list of server models created from properties
     * @throws IllegalArgumentException cannot process information from properties to server information
     */
    public static ObservableList<Server> buildServersFromProperties(Properties properties,LongProperty progress) throws IllegalArgumentException{
        //create list
        ObservableList<Server> servers = FXCollections.observableArrayList();
        //find property with list of servers
        String serversProperty = properties.getProperty("servers", null);
        if(progress != null ) progress.set(10);
        //if property with list of servers doesn't exist, notify
        if(serversProperty == null) throw new IllegalArgumentException("Cannot find property \"servers\" in properties file");
        String[] serversArray = serversProperty.split(",");
        for (String serverProperty : serversArray){
            //take server name and find it's parameters in properties file
            String tmpServer = properties.getProperty(serverProperty + "_server_ip", null);
            if(tmpServer == null || tmpServer.equals("")) throw new IllegalArgumentException("Cannot find property \"" + serverProperty + "_server_ip\" in properties file");
            String tmpPort = properties.getProperty(serverProperty + "_server_port", null);
            if(tmpPort == null || tmpPort.equals("")) throw new IllegalArgumentException("Cannot find property \"" + serverProperty + "_server_port\" in properties file");
            String tmpDbName = properties.getProperty(serverProperty + "_server_db", null);
            if(tmpDbName == null || tmpDbName.equals("")) throw new IllegalArgumentException("Cannot find property \"" + serverProperty + "_server_db\" in properties file");
            try {
                //create model
                Server server = new Server(false,serverProperty,tmpServer,Integer.valueOf(tmpPort),tmpDbName);
                //add model to list
                servers.add(server);
                //change dialog progress
                if(progress != null ) progress.set(progress.get()+(90/serversArray.length));
            } catch (NumberFormatException en){
                throw new IllegalArgumentException("Cannot parse port to integer on property \"" + serverProperty +"_server_port\" in properties file",en);
            }
        }
        return servers;
    }

    /**
     * Get specific property from loaded properties.
     * @param properties loaded properties
     * @param property wanted property
     * @return value of wanted property
     * @throws IllegalArgumentException property was not found
     */
    public static String getProperty(Properties properties, String property) throws IllegalArgumentException{
        if(properties == null) throw new IllegalArgumentException("Properties NULL");
        String tmp = properties.getProperty(property);
        if( tmp == null){
            throw new IllegalArgumentException("Missing property \"" + property + "\" in properties file");
        }else{
            return tmp;
        }
    }

    /**
     * Shows {@link org.controlsfx.dialog.ExceptionDialog} with given exception.
     * @param log logger from class, that caught exception
     * @param header dialog header
     * @param e exception
     * @param closeApp <code>true</code> if application should be closed after showing exception, <code>false</code> otherwise
     * @param onCloseEvent action ran, if before application closes, null if closeApp parameter is <code>false</code>
     */
    public static void showException(Logger log,String header,Throwable e,boolean closeApp,EventHandler<DialogEvent> onCloseEvent){
        if(e == null){
            e = new IllegalArgumentException(header);
        }
        log.error(e.getMessage(),e);
        //create exception dialog
        ExceptionDialog dlg = new ExceptionDialog(e);
        //define default header
        if (header == null) header = "Ooops. Something went wrong!.";
        dlg.getDialogPane().setHeaderText(header);
        dlg.initStyle(StageStyle.DECORATED);
        if(closeApp && onCloseEvent != null){
            dlg.setOnCloseRequest(onCloseEvent);
        }
        //show exception dialog
        Platform.runLater(dlg::show);
    }

    /**
     * Show error dialog with given message.
     * @param message error message
     * @param title dialog title
     */
    public static void showError(String message,String title) {
        Alert dlg = new Alert(Alert.AlertType.ERROR, message);
        dlg.initModality(Modality.WINDOW_MODAL);
        dlg.setTitle(title);
        Platform.runLater(dlg::show);
    }

    /**
     * Loads properties from given properties filename. Creates separate {@link javafx.concurrent.Task}, so GUI thread
     * is not stopped.
     * @param defaultPropertiesFileName properties filename
     * @return {@link javafx.concurrent.Task} responsible for loading properties
     */
    public static Task<Properties> loadProperties(String defaultPropertiesFileName){
        logger.trace("loading");
        //define background process
        Task<Properties> worker = new Task<Properties>() {
            @Override
            protected Properties call() throws Exception {
                Properties properties = null;
                //update progress bar
                updateProgress(1,99);
                //update progress message
                updateMessage("Loading application settings...");
                //load properties
                try{
                    properties = Utilities.getInstance().loadDefaultProperties(defaultPropertiesFileName);
                    updateProgress(50, 99);
                    //unlikely to happen, just to be sure
                    if(properties == null){
                        throw new IOException("Properties is null");
                    }
                    updateProgress(99, 99);
                }catch (IOException e){
                    showException(logger, "Cannot load properties. PLease review properties file and run application again", e, true, null);
                    logger.trace("success");
                }
                return properties;
            }
        };
        return worker;
    }

    /**
     * Sets log4j2 logger level for application.
     * @param levelString level as string
     */
    public static void setLoggerLevel(String levelString){
        Level level;
        switch (levelString.toLowerCase()){
            case "all":
                level = Level.ALL;
                break;
            case "trace":
                level = Level.TRACE;
                break;
            case "debug":
                level = Level.DEBUG;
                break;
            case "info":
                level = Level.INFO;
                break;
            case "warn":
                level = Level.WARN;
                break;
            case "error":
                level = Level.ERROR;
                break;
            case "fatal":
                level = Level.FATAL;
                break;
            case "off":
                level = Level.OFF;
                break;
            default:
                level = Level.INFO;
                break;
        }
        logger.info("Setting logger level: " + level.toString());
        if(level != null){
            LoggerContext ctx = (LoggerContext) LogManager.getContext(false);
            Configuration conf = ctx.getConfiguration();
            conf.getLoggerConfig(LogManager.ROOT_LOGGER_NAME).setLevel(level);
            ctx.updateLoggers(conf);
        }
    }

    /**
     * Checks, if string contains other string.
     * @param haystack source string
     * @param needle searched for string
     * @return <code>true</code> if found, <code>false</code> otherwise
     */
    public static boolean containsIgnoreCase( String haystack, String needle ) {
        if(haystack == null || needle == null || haystack .equals(""))
            return false;
        if(needle.equals(""))
            return true;
        Pattern p = Pattern.compile(needle,Pattern.CASE_INSENSITIVE+Pattern.LITERAL);
        Matcher m = p.matcher(haystack);
        return m.find();
    }

    /**
     * Show {@link org.controlsfx.dialog.ProgressDialog} for given {@link javafx.concurrent.Task}.
     * @param worker task, for which is dialog created
     * @param title dialog title
     */
    public static void showLoadingDialog(Task worker,String title){
        //create progress dialog
        ProgressDialog dlg = new ProgressDialog(worker);
        dlg.setTitle(title);
        dlg.getDialogPane().setHeaderText("");
        dlg.initStyle(StageStyle.UNIFIED);
        //create thread for background process
        Thread th = new Thread(worker);
        th.setDaemon(true);
        //run background process
        th.start();
    }

    /**
     * Shows {@link javafx.scene.control.ChoiceDialog} with options to save adapters, when application is closing.
     * @return dialog instance
     */
    public static ChoiceDialog<SaveAdaptersOption> saveAdaptersOnQuitDialog(){
        ChoiceDialog<SaveAdaptersOption> dlg = new ChoiceDialog<>(SaveAdaptersOption.SAVE_ALL, SaveAdaptersOption.values());
        dlg.setTitle("Unsaved adapters");
        dlg.getDialogPane().setContentText("There are unsaved adapters. Choose option");
        return dlg;
    }
    /**
     * Shows {@link javafx.scene.control.ChoiceDialog} with options to save {@link com.iha.emulator.control.task.SimulationTask}s,
     * when application is closing.
     * @return dialog instance
     */
    public static ChoiceDialog<SaveTasksOption> saveTasksOnQuitDialog(){
        ChoiceDialog<SaveTasksOption> dlg = new ChoiceDialog<>(SaveTasksOption.SAVE_ALL, SaveTasksOption.values());
        dlg.setTitle("Unsaved tasks");
        dlg.getDialogPane().setContentText("There are unsaved tasks. Choose option");
        return dlg;
    }

    /**
     * Enum with options to save adapters presented when application is closing
     */
    public enum SaveAdaptersOption {
        SAVE_ALL(0,"Save all adapters"),
        SAVE_CURRENT(1,"Save currently selected adapter"),
        DO_NOTHING(2,"Do nothing");
        /** option ID */
        final private int id;
        /** option text*/
        final private String text;

        /**
         * Option with given ID and text
         * @param id option ID
         * @param text option text
         */
        SaveAdaptersOption(int id, String text){
            this.id = id;
            this.text = text;
        }

        /**
         * Gets option ID
         * @return option ID
         */
        public int getId(){
            return this.id;
        }

        /**
         * Gets option text
         * @return option text
         */
        public String getText(){
            return this.text;
        }
        /**
         * Gets option text
         * @return option text
         */
        public String toString(){
            return this.text;
        }
    }
    /**
     * Enum with options to save simulation tasks presented when application is closing
     */
    public enum SaveTasksOption {
        SAVE_ALL(0,"Save tasks"),
        DO_NOTHING(2,"Do nothing");
        /** option ID */
        final private int id;
        /** option text*/
        final private String text;
        /**
         * Option with given ID and text
         * @param id option ID
         * @param text option text
         */
        SaveTasksOption(int id, String text){
            this.id = id;
            this.text = text;
        }
        /**
         * Gets option ID
         * @return option ID
         */
        public int getId(){
            return this.id;
        }
        /**
         * Gets option text
         * @return option text
         */
        public String getText(){
            return this.text;
        }
        /**
         * Gets option text
         * @return option text
         */
        public String toString(){
            return this.text;
        }
    }

    /**
     * Shows {@link javafx.stage.FileChooser} dialog for saving XML file. User chooses filename and XML content given
     * as parameter is saved to file.
     * @param parentWindow parent window of dialog
     * @param dirPath default directory location
     * @param XMLContent XML content to be saved
     * @return name of XML file, null if saving was unsuccessful
     */
    public static String saveDialogForXML(Stage parentWindow,String dirPath,String XMLContent){
        FileChooser fileChooser = new FileChooser();
        //Set extension filter
        FileChooser.ExtensionFilter extFilter = new FileChooser.ExtensionFilter("XML files (*.xml)", "*.xml");
        fileChooser.getExtensionFilters().add(extFilter);
        //directory
        File dir = new File(dirPath);
        if(!dir.exists())
            dir.mkdirs();
        fileChooser.setInitialDirectory(dir);
        //Show save file dialog
        File file = fileChooser.showSaveDialog(parentWindow);
        try {
            if(file != null){
                if (!file.getPath().endsWith(".xml")) {
                    file = new File(file.getPath() + ".xml");
                }
                FileWriter fileWriter;
                fileWriter = new FileWriter(file);
                fileWriter.write(XMLContent);
                fileWriter.close();
                logger.trace("OK");
            }else {
                return null;
            }
        }catch (IOException e){
            Utilities.showException(logger, "Cannot save file", e, false, null);
        }
        return file.getName();
    }
    /**
     * Shows {@link javafx.stage.FileChooser} dialog for loading XML file. User chooses file and XML content is returned.
     * @param window parent window of dialog
     * @param dirPath default directory location
     * @return content of XML file, null if loading was unsuccessful
     */
    public static String loadDialogForXML(Stage window,String dirPath){
        FileChooser fileChooser = new FileChooser();
        //Set extension filter
        FileChooser.ExtensionFilter extFilter = new FileChooser.ExtensionFilter("XML files (*.xml)", "*.xml");
        fileChooser.getExtensionFilters().add(extFilter);
        //directory
        File dir = new File(dirPath);
        if(!dir.exists()) dir.mkdirs();
        fileChooser.setInitialDirectory(dir);
        //Show save file dialog
        File file = fileChooser.showOpenDialog(window);
        try{
            if(file != null){
                return readFile(file);
            }
        }catch (IOException e){
            Utilities.showException(logger,"Cannot load file",e,false,null);
            return null;
        }catch (NullPointerException ex) {
            Utilities.showException(logger, "Cannot parse loaded file. Incorrect or damaged file content format.", ex, false, null);
            return null;
        }
        return null;
    }

    /**
     * Reads content of given file and saves it to single string.
     * @param file file to be read
     * @return single string with file content.
     * @throws IOException cannot read line in file
     */
    public static String readFile(File file) throws IOException {
        logger.trace("reading XML file");
        FileReader fileReader = new FileReader(file);
        BufferedReader bufferedReader = new BufferedReader(fileReader);
        StringBuilder stringBuffer = new StringBuilder();
        String line;
        while ((line = bufferedReader.readLine()) != null) {
            stringBuffer.append(line);
        }
        fileReader.close();
        return stringBuffer.toString();
    }

    /**
     * Checks, if ID given as parameter already exists in list of adapters also given as parameter
     * @param adapterControllers list of adapters
     * @param newId ID to be checked
     * @return <code>true</code> if ID exists, <code>false</code> otherwise
     */
    public static boolean isAdapterIdTaken(ObservableList<AdapterController> adapterControllers,int newId){
        if(adapterControllers.size() == 0) return false;
        for(AdapterController adapterController : adapterControllers){
            if(adapterController.getAdapter().getId() == newId)
                return true;
        }
        return false;
    }
    /**
     * Checks, if ID given as parameter already exists in list of sensors in adapter also given as parameter
     * @param controller adapter containing list of sensors
     * @param sensorId ID to be checked
     * @return <code>true</code> if ID exists, <code>false</code> otherwise
     */
    public static boolean isSensorIdTaken(AdapterController controller,String sensorId){
        if(sensorId.equals("")) return false;
        int id = Integer.valueOf(sensorId);
        for(SensorController sController : controller.getSensorControllersList()){
            if(sController.getModel() != null && sController.getModel().getId() == id){
                return true;
            }
        }
        return false;
    }

    /**
     * Method checks, if given string contains Integer number in given digit range
     * @param number string containing number
     * @param minDigits minimum number digits
     * @param maxDigits maximum number digits
     * @return <code>true</code> if string contains Integer number in given range, <code>false</code> otherwise
     */
    public static boolean isIntegerNumber(String number,int minDigits,int maxDigits){
        if(number == null) return false;
        Pattern pattern = Pattern.compile("\\d{"+minDigits+","+maxDigits+"}");
        Matcher matcher = pattern.matcher(number);
        return matcher.matches();
    }

    /**
     * Method checks, if given string contains Double number.
     * @param str string to be checked
     * @return <code>true</code> if string contains Double number, <code>false</code> otherwise
     */
    public static boolean isNumeric(String str)
    {
        try
        {
            double d = Double.parseDouble(str);
        }
        catch(NumberFormatException nfe)
        {
            return false;
        }
        return true;
    }

    /**
     * Method format seconds given as parameter to string representation containing time unit (s, m, h)
     * @param seconds time to be formatted
     * @return formatted time
     */
    public static String formatSeconds(int seconds){
        if(seconds < 60) {
            return seconds + " s";
        }else if(seconds < 3600){
            return (seconds/60) + " m";
        }else {
            return (seconds/3600) + " h";
        }
    }
    /**
     * Method format seconds given as parameter to string representation containing time unit (s, m, h)
     * @param seconds time to be formatted
     * @return formatted time
     */
    public static String formatSeconds(long seconds){
        if(seconds < 60) {
            return seconds + " s";
        }else if(seconds < 3600){
            long min = (seconds/60);
            long sec = seconds%60;
            return min + "m : " + sec + "s";
        }else {
            long hour = seconds/3600;
            long remaining = seconds%3600;
            long min = remaining/60;
            long sec = remaining%60;
            return hour + "h : " + min + "m : " + sec + "s";
        }
    }

    /**
     * Method, with help of given generator, generates refresh time in given range
     * @param generator generator to be used
     * @param min minimal refresh time, represented as index of option in {@link #REFERSH_TIME_OPTIONS} array
     * @param max maximal refresh time, represented as index of option in {@link #REFERSH_TIME_OPTIONS} array
     * @return generated refresh time
     * @throws IllegalArgumentException generator is null or max is less then min
     */
    public static int generateRefreshTime(Random generator,int min,int max) throws IllegalArgumentException{
        if(generator == null || max < min) throw new IllegalArgumentException("Generator is null or max is less then min.");
        int minIndex = getIndexOf(min,REFERSH_TIME_OPTIONS);
        int maxIndex = getIndexOf(max,REFERSH_TIME_OPTIONS);
        int newIndex;
        newIndex = generator.nextInt((maxIndex - minIndex)+1) + minIndex;
        logger.trace("Generating refresh time (index/value): " + newIndex + "/" + REFERSH_TIME_OPTIONS[newIndex]);
        return REFERSH_TIME_OPTIONS[newIndex];
    }

    /**
     * Gets index of searched option in given array
     * @param toSearch option to be searched for
     * @param tab array of options
     * @return index of searched option in given array
     * @throws IllegalArgumentException if option doesn't exist
     */
    public static int getIndexOf( int toSearch, int[] tab )throws IllegalArgumentException{
        int i = 0;
        while(!(tab[i] == toSearch) ){
            i++;
            if(i == tab.length) throw new IllegalArgumentException("Cannot find index of second value: " + toSearch);
        }
        return i; // or return tab[i];
    }

    /**
     * GUI refresh slider scale number ( 0 - {@link #REFRESH_SLIDER_VALUE_COUNT} to seconds. Each scale index represents
     * number from {@link #REFERSH_TIME_OPTIONS} array.
     * @param scaleNo scale index
     * @return seconds from slider
     */
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

    /**
     * Seconds to GUI refresh slider scale number ( 0 - {@link #REFRESH_SLIDER_VALUE_COUNT}. Each scale index represents
     * number from {@link #REFERSH_TIME_OPTIONS} array.
     * @param seconds seconds to be converted
     * @return scale index
     */
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

    /**
     * Gets hex representation of given color
     * @param color color to be converted
     * @return hex representation of given color
     */
    public static String toRGBCode( Color color )
    {
        return String.format( "#%02X%02X%02X",
                (int)( color.getRed() * 255 ),
                (int)( color.getGreen() * 255 ),
                (int)( color.getBlue() * 255 ) );
    }

    /**
     * Generates new value from current one using normal distribution with accuracy given as parameters.
     * Accuracy = 10 for 1 decimal, 100 for to decimals, ...
     *
     * @param generator generator to be used
     * @param accuracy accuracy of new number
     * @param dev normal distribution deviation
     * @param avg normal distribution average
     * @param max normal distribution max
     * @param min normal distribution min
     * @return new value
     * @throws IllegalArgumentException if generator is null or max is less then min
     */
    public static double normalDistribution(Random generator,int accuracy,double dev,double avg,double max, double min) throws IllegalArgumentException{
        if(generator == null) throw new IllegalArgumentException("Generator is null. Cannot generate new value");
        if(max < min) throw new IllegalArgumentException("Max value is smaller than min value");
        double newValue;
        do{
            double val = generator.nextGaussian() * dev + avg;
            newValue = Math.round(val * accuracy) / accuracy;
        }while(newValue > max || newValue < min);
        logger.trace("Generating new value with normal distribution: newVal -> " + newValue);
        return newValue;
    }

    /**
     * Generates new value from current one using linear distribution. Step is added to current value, if new value is
     * bigger then maximal value, maximal value is returned. If new value is lower then minimal value, minimal value
     * is returned.
     *
     * @param value current value
     * @param step linear distribution step
     * @param max maximal value
     * @param min minimal value
     * @return new value
     * @throws IllegalArgumentException max is less then min
     */
    public static double linearDistribution(double value,double step,double max,double min) throws IllegalArgumentException{
        if(max < min) throw new IllegalArgumentException("Max value is smaller than min value");
        double newValue = value + step;
        if(newValue > max){
            logger.trace("Linear distribution reached max, returning max -> " + max);
            return max;
        }
        if(newValue < min) {
            logger.trace("Linear distribution smaller than minimum, returning min -> " + min);
            return min;
        }
        logger.trace("Generating new value with linear distribution: newVal -> " + newValue);
        return newValue;
    }

    /**
     * According to probability, boolean value is switched to opposite value. If randomly generated number between 0.0 and
     * 1.0 is lower or equal to given probability, value is switched. Otherwise value stays same.
     * @param value current value
     * @param probability switch probability
     * @param generator generator used to generate random number
     * @return new value
     * @throws IllegalArgumentException if generator is null
     */
    public static boolean booleanRandomGenerate(boolean value,double probability,Random generator)throws IllegalArgumentException{
        if(generator == null) throw new IllegalArgumentException("Generator is null. Cannot generate new value");
        boolean newValue =generator.nextDouble() <= probability ? !value : value;
        logger.trace("Generating new boolean random value: newVal -> " + newValue);
        return newValue;
    }

    /**
     * Generates random number in given range.
     * @param generator used generator
     * @param min range minimum
     * @param max range maximum
     * @return random number in given range
     * @throws IllegalArgumentException if generator is null or max is less then min
     */
    public static int generateIntInRange(Random generator,int min,int max)throws IllegalArgumentException{
        if(generator == null || max < min) throw new IllegalArgumentException("Generator is null or max is less then min.");
        int newValue;
        if(min == max){
            newValue = max;
        }else {
            newValue = generator.nextInt((max - min) + 1) + min;
        }
        logger.trace("Generating integer in range(min/max) -> " + newValue + "(" + min + "/" + max + ")");
        return  newValue;
    }

    /**
     * Checks, if given filename exists
     * @param filename filename to be checked
     * @return <code>true</code> if file exists, <code>false</code> otherwise
     */
    private boolean checkIfExists(String filename){
        File file = new File(filename);
        return file.exists();
    }

    /**
     * Copies resource to given filename
     * @param fromResource resource to be copied
     * @param toFile destination filename
     * @throws IOException cannot copy files
     */
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

    /**
     * Deep channel copy.
     * @param src source channel
     * @param dest destination cannel
     * @throws IOException cannot read or write channels
     */
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
