package com.smarthome.emulator.simulations;

import com.intellij.uiDesigner.core.GridConstraints;
import com.intellij.uiDesigner.core.GridLayoutManager;
import com.intellij.uiDesigner.core.Spacer;
import com.smarthome.emulator.Emulator;
import com.smarthome.emulator.design.JCustomTabbedPaneUI;
import com.smarthome.emulator.design.JDetailsTable;
import com.smarthome.emulator.design.JIOTScrollBarUI;
import com.smarthome.emulator.design.JNumberTextField;
import com.smarthome.emulator.simulations.forms.LogFileViewer;
import com.smarthome.emulator.simulations.panels.ServerPanel;
import com.smarthome.emulator.simulations.utilities.StyledDocumentPrinter;
import com.smarthome.emulator.simulations.basis.*;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.DateAxis;
import org.jfree.chart.axis.ValueAxis;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.renderer.xy.XYItemRenderer;
import org.jfree.chart.renderer.xy.XYLineAndShapeRenderer;
import org.jfree.data.time.Second;
import org.jfree.data.time.TimeSeries;
import org.jfree.data.time.TimeSeriesCollection;
import org.jfree.data.xy.XYDataset;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.xml.sax.SAXException;

import javax.print.attribute.HashPrintRequestAttributeSet;
import javax.print.attribute.PrintRequestAttributeSet;
import javax.print.attribute.standard.JobName;
import javax.swing.*;
import javax.swing.Timer;
import javax.swing.border.LineBorder;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.swing.table.DefaultTableModel;
import javax.swing.text.BadLocationException;
import javax.swing.text.Style;
import javax.swing.text.StyleConstants;
import javax.swing.text.StyledDocument;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import java.awt.*;
import java.awt.event.*;
import java.io.File;
import java.io.IOException;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.*;

/**
 * Creates environment for server performance testing. Provides creation of large number of adapters
 * with randomized number of sensors and there values. Also provides possibility to save, load simulation,
 * print simulation logs and display server's responses in chart.
 */
public class PerformanceSimulation extends JFrame {
    /**
     * GUI variables
     */
    private JPanel mainPanel;
    private JLabel performanceSimulationLabel;
    private JPanel menuPanel;
    private JButton btnMainSave;
    private JButton btnMainLoad;
    private JButton btnMainPrintLogs;
    private JLabel configurationLbl;
    private JScrollPane configurationScroller;
    private JPanel configurationPanel;
    private JPanel serverResponsePanel;
    private JLabel serverResponseLbl;
    private JTabbedPane logTabPanel;
    private JScrollPane simulationLogScroller;
    private JTextPane simulationLogText;
    private JScrollPane toBeSentScroller;
    private JTextPane toBeSentText;
    private JScrollPane errorLogScroller;
    private JTextPane errorLogText;
    private JLabel currentMem;
    private JProgressBar memProgressBar;
    private JLabel totalMem;
    private JButton memoryCheckBtn;
    private JButton startSimulationBtn;
    private JButton stopSimulationBtn;
    private JPanel confPanel;
    private JNumberTextField adaptersNumberTextField;
    private JNumberTextField sensorsPerAdapterFromTextField;
    private JNumberTextField sensorsPerAdapterToTextField;
    private JNumberTextField refreshRateFromTextField;
    private JNumberTextField refreshRateToTextField;
    private JNumberTextField adapterSensorProtocolTextField;
    private JNumberTextField adapterServerProtocolTextField;
    private JNumberTextField temperatureMaxTextField;
    private JNumberTextField temperatureMinTextField;
    private JNumberTextField temperatureDevTextField;
    private JNumberTextField temperatureAvgTextField;
    private JNumberTextField humidityMaxTextField;
    private JNumberTextField humidityMinTextField;
    private JNumberTextField humidityDevTextField;
    private JNumberTextField humidityAvgTextField;
    private JNumberTextField pressureMaxTextField;
    private JNumberTextField pressureMinTextField;
    private JNumberTextField pressureDevTextField;
    private JNumberTextField pressureAvgTextField;
    private JNumberTextField lightIntensityMaxTextField;
    private JNumberTextField lightIntensityMinTextField;
    private JNumberTextField lightIntensityDevTextField;
    private JNumberTextField lightIntensityAvgTextField;
    private JNumberTextField noiseIntensityMaxTextField;
    private JNumberTextField noiseIntensityMinTextField;
    private JNumberTextField noiseIntensityDevTextField;
    private JNumberTextField noiseIntensityAvgTextField;
    private JNumberTextField emissionsMaxTextField;
    private JNumberTextField emissionsMinTextField;
    private JNumberTextField emissionsDevTextField;
    private JNumberTextField emissionsAvgTextField;
    private JCheckBox temperatureCheckBox;
    private JCheckBox humidityCheckBox;
    private JCheckBox pressureCheckBox;
    private JCheckBox lightIntensityCheckBox;
    private JCheckBox noiseIntensityCheckBox;
    private JCheckBox emissionsCheckBox;
    private JPanel sensorValuesPanel;
    private JLabel sensorValuesLbl;
    private JLabel serverDetailsLbl;
    private JDetailsTable serverTable;
    private JLabel internetConnectionLbl;
    private JButton connectBtn;
    private JButton disconnectBtn;
    private JNumberTextField startFromIdTextField;
    private JLabel adaptersLbl;
    private JLabel senPerAdaLbl;
    private JLabel warningLbl;
    private JRadioButton hostRadBtn;
    private JRadioButton ipRadBtn;
    private JTextField addressTextField;
    private JNumberTextField portTextField;
    private JPanel warningPanel;
    private JButton resumeSimulationBtn;
    private JButton pauseSimulationBtn;
    private JButton pauseSensorsBtn;
    private JButton resumeSensorsBtn;
    private JCheckBox sensorOfActuatorCheckBox;
    private JRadioButton sensorOfActuatorTrueRadBtn;
    private JRadioButton sensorOfActuatorFalseRadBtn;
    /**
     * list of adapters
     */
    private ArrayList<Adapter> adapters = new ArrayList<>();
    /**
     * internet connection indicator
     */
    private boolean internetConnection = true;
    /**
     * simulation logger
     */
    private Logger logger;
    /**
     * server panel for displaying server information
     */
    private ServerPanel serverPanel;
    /**
     * data model for server table
     */
    private DefaultTableModel serverTableModel;
    /**
     * simulation running indicator
     */
    private boolean running = false;
    /**
     * simulation paused indicator
     */
    private boolean paused = false;
    /**
     * simulation sensors activity indicator
     */
    private boolean sensorsPaused = false;
    /**
     * simulation saving indicator
     */
    private boolean saved = true;
    /**
     * number of sensors generator
     */
    private Random sensorCountGenerator;
    /**
     * number of sensors generator seed
     */
    private Long sensorCountGeneratorSeed = null;
    /**
     * sensor refresh rate generator
     */
    private Random refreshRateGenerator;
    /**
     * sensor refresh rate generator seed
     */
    private Long refreshRateGeneratorSeed = null;
    /**
     * sensor signal generator
     */
    private Random signalGenerator;
    /**
     * sensor signal generator seed
     */
    private Long signalGeneratorSeed = null;
    /**
     * sensor battery generator generator
     */
    private Random batteryGenerator;
    /**
     * sensor battery generator generator seed
     */
    private Long batteryGeneratorSeed = null;
    /**
     * sensors types generator
     */
    private Random valueTypeGenerator;
    /**
     * sensors types generator seed
     */
    private Long valueTypeGeneratorSeed = null;
    /**
     * value generator - type "temperature" - seed
     */
    private Long temperatureSeed = null;
    /**
     * value generator - type "humidity" - seed
     */
    private Long humiditySeed = null;
    /**
     * value generator - type "pressure" - seed
     */
    private Long pressureSeed = null;
    /**
     * value generator - type "light intensity" - seed
     */
    private Long lightIntensitySeed = null;
    /**
     * value generator - type "noise intensity" - seed
     */
    private Long noiseIntensitySeed = null;
    /**
     * value generator - type "emmisions" - seed
     */
    private Long emissionsSeed = null;
    /**
     * maximum value of battery
     */
    private static final int BATTERY_MAX = 100;
    /**
     * minimum value of battery
     */
    private static final int BATTERY_MIN = 40;
    /**
     * maximum value of siganl
     */
    private static final int SIGNAL_MAX = 100;
    /**
     * minimum value of signal
     */
    private static final int SIGNAL_MIN = 60;
    /**
     * maximum value of type
     */
    private static final int VALUE_TYPE_MAX = 7;
    /**
     * minimum value of type
     */
    private static final int VALUE_TYPE_MIN = 0;
    /**
     * number of created sensors, also used to assign new sensor's id
     */
    private static int sensorsCount = 0;
    /**
     * timer for updating chart
     */
    private Timer responseChartTimer;
    /**
     * server responses chart
     */
    private JFreeChart chart;
    /**
     * chart timer refresh time
     */
    private static final int REFRESH_TIME = 1500;
    /**
     * chart dataset
     */
    private TimeSeriesCollection dataset = null;
    /**
     * chart series - used to update chart data
     */
    private TimeSeries chartSeries = null;
    /**
     * chart line color
     */
    private static final Color CHART_COLOR = new Color(28,207,28);
    /**
     * chart labels color
     */
    private static final Color LABEL_COLOR = new Color(130,130,130);
    /**
     * chart moves every 60s
     */
    private static final double CHART_MOVING_BASE = 60000.0;
    /**
     * Maximum count of created threads
     */
    private static final int MAX_THREADS = 2500;
    /**
     * Creates and initializes all visual components needed to perform performance simulation
     */
    public PerformanceSimulation() {
        $$$setupUI$$$();
        initComponents();
        startMemoryCheck();
        resizeConfigurationPanel();
        setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                onCancel();
            }
        });
        pack();
        setLocationRelativeTo(null);
        setVisible(true);
    }
    /**
     * Checks if configuration fields are correctly filled, validates filled fields.
     * Then creates given number of adapters with randomized number of sensors and
     * activates all adapters by calling {@link com.smarthome.emulator.simulations.PerformanceSimulation#resumeSimulation()}.
     * Also disables configuration change
     */
    public void startSimulation() {
        //if any warning is set, remove it
        warningPanel.setVisible(false);
        warningLbl.setText("");
        //set indicator
        setRunning(true);
        //change buttons backgrounds
        switchButtonsLook(stopSimulationBtn, startSimulationBtn);
        setActiveBtnBackground(pauseSensorsBtn);
        //disable configuration change
        disableConfigurationChange();
        //checks and validates fields
        if (!checkConfigurationWhiteFields()) {
            stopSimulation();
            return;
        }
        if(!validateConfiguration()){
            stopSimulation();
            return;
        }
        //adding adapters
        for(int i = 0; i < adaptersNumberTextField.getInt();i++){
            Adapter newAdapter = new Adapter(logger);
            newAdapter.setServerParameters(serverPanel,hostRadBtn.isSelected()?addressTextField.getText():null,ipRadBtn.isSelected()?addressTextField.getText():null,portTextField.getInt());
            newAdapter.setAdapterId(startFromIdTextField.getLong() + i);
            newAdapter.setServerProtocol(Short.valueOf(adapterServerProtocolTextField.getText()));
            newAdapter.initScheduler();
            newAdapter.setActivity(false);
            newAdapter.setInternetConnection(isInternetConnection());
            serverPanel.refreshTable(newAdapter.getServer());
            adapters.add(newAdapter);
            randomizeSensors(newAdapter);
            newAdapter.setStoreValuesHistory();
        }
        //notifies user
        logger.writeLogMessage("SIMULATION STARTED",logger.BOLD_STYLE);
        setSaved(false);
        //start simulation by activating adapters
        resumeSimulation();
    }
    /**
     * Stops simulation, removes all data generated by simulation, resets chart. Also enables configuration change
     */
    public void stopSimulation() {
        pauseSimulation();
        //set indicator
        setRunning(false);
        //change buttons
        switchButtonsLook(startSimulationBtn, stopSimulationBtn);
        enableConfigurationChange();
        //notifies user
        logger.writeLogMessage("SIMULATION STOPPED",logger.BOLD_STYLE);
        //remove adapters
        deleteAdapters();
        //set default configuration
        setDefault();
        //reset chart
        resetChartDataset();
        //remove server information
        //serverPanel.delete();
        //set resume button inactive
        setInactiveBtnBackground(resumeSimulationBtn);
        setInactiveBtnBackground(resumeSensorsBtn);
        setInactiveBtnBackground(pauseSensorsBtn);
        logger.clearAllLogs();
        logger.clearBuffer();
        //resets sensor count
        restartSensorsCount();
    }
    /**
     * Sets all adapters to inactive state, stops response chart painting.
     */
    public void pauseSimulation(){
        //set indicator
        setPaused(true);
        //stops response chart timer
        responseChartTimer.stop();
        //change button looks
        switchButtonsLook(resumeSimulationBtn, pauseSimulationBtn);
        //deactivate adapters
        for(Adapter adapter : adapters){
            adapter.setActivity(false);
            adapter.stopSensorsTimers();
        }
        //notify user
        logger.writeLogMessage("SIMULATION PAUSED",logger.BOLD_STYLE);
    }
    /**
     * Sets all adapters to active state,starts response chart painting
     */
    public void resumeSimulation(){
        //set indicator
        setPaused(false);
        //change buttons
        switchButtonsLook(pauseSimulationBtn,resumeSimulationBtn);
        //set indicator
        setSensorsPaused(false);
        //change buttons
        switchButtonsLook(pauseSensorsBtn,resumeSensorsBtn);
        //starts response chart timer
        responseChartTimer.start();
        //activate all adapters
        for(Adapter adapter : adapters){
            adapter.setActivity(true);
        }
        //notify user
        logger.writeLogMessage("SIMULATION RESUMED",logger.BOLD_STYLE);
    }
    /**
     * Stops all sensors on all adapters from sending new messages
     */
    public void pauseSensors(){
        //set indicator
        setSensorsPaused(true);
        //change buttons
        switchButtonsLook(resumeSensorsBtn,pauseSensorsBtn);
        //stop sensors timers
        for(Adapter adapter : adapters){
            adapter.stopSensorsTimers();
        }
        //notify user
        logger.writeLogMessage("SENSORS ACTIVITY PAUSED",logger.BOLD_STYLE);
    }
    /**
     * Allow all sensors on all adapters to send new messages
     */
    public void resumeSensors(){
        //set indicator
        setSensorsPaused(false);
        //change buttons
        switchButtonsLook(pauseSensorsBtn,resumeSensorsBtn);
        //start sensors timers
        for(Adapter adapter : adapters){
            adapter.startSensorTimers();
        }
        //notify user
        logger.writeLogMessage("SENSORS ACTIVITY RESUMED",logger.BOLD_STYLE);
    }
    /**
     * Increase number of sensors
     */
    public static void incrementSensorsCount(){
        sensorsCount++;
    }
    /**
     * Decrease number of sensors
     */
    public static void decrementSensorsCount(){
        sensorsCount--;
    }
    /**
     * Show either save of load dialog returning file name of file to be saved/loaded.
     *
     * @param title dialog title
     * @param save  <code>true</code> for save dialog, <code>false</code> for load dialog
     * @return file name on success, null if no file selected
     */
    public String showDialog(String title, boolean save) {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setDialogTitle(title);
        FileNameExtensionFilter xml = new FileNameExtensionFilter("XML Files", "xml");
        fileChooser.addChoosableFileFilter(xml);
        fileChooser.setFileFilter(xml);
        int returnVal;
        //open either save or load dialog according to parameter
        if (save) {
            returnVal = fileChooser.showSaveDialog(this);
        } else {
            returnVal = fileChooser.showOpenDialog(this);
        }
        if (returnVal == fileChooser.APPROVE_OPTION) {
            //return filename
            return fileChooser.getSelectedFile().getAbsolutePath();
        }
        //if no file selected
        return null;
    }
    /**
     * Show dialog with options to save and print logs, do nothing or cancel dialog.
     * If save and print action is selected, save and print dialogs are shown.
     */
    public void showSavePrintDialog(){
        pauseSimulation();
        Object[] options = { "Save simulation and print logs",  "No" ,  "Cancel"};
        int dialogResult = JOptionPane.showOptionDialog(null, "Would you like to save simulation and print dialog?", "Warning",
                JOptionPane.DEFAULT_OPTION, JOptionPane.WARNING_MESSAGE,
                null, options, options[0]);
        if (dialogResult == 0) {
            saveSimulationToXML();
            logger.printLogs("Performance Simulation -> adapters count="+ adapters.size());
            stopSimulation();
        }else if (dialogResult == 1) {
            stopSimulation();
        }
    }
    //region memory check
    /**
     * memory check indicator
     */
    public boolean isMemoryChecked = true;
    /**
     * current runtime
     */
    public Runtime runtime = Runtime.getRuntime();
    /**
     * variable to store memory usage
     */
    public long memory;
    /**
     * timer for memory check
     */
    public Timer memoryCheckTimer = null;
    /**
     * Variable of type double returns as string in given format
     *
     * @param pattern string format (example ###,###.###)
     * @param value   value to be formatted
     * @return string representation of <code>value</code>
     */
    public static String customFormat(String pattern, double value) {
        DecimalFormat myFormatter = new DecimalFormat(pattern);
        String output = myFormatter.format(value);
        return output;
    }
    /**
     * Stops checking for memory usage, stops memory check timer
     */
    public void stopMemoryCheck() {
        isMemoryChecked = false;
        memoryCheckTimer.stop();
        memProgressBar.setValue(0);
        currentMem.setText("0");
        totalMem.setText(String.valueOf(memProgressBar.getMaximum()));
        memoryCheckBtn.setText("Start memory check");
    }
    /**
     * Calculates current memory usage and maximum memory offered by VM and
     * displays these information.
     */
    public void checkMemory() {
        memProgressBar.setMaximum((int) runtime.totalMemory());
        memProgressBar.setMinimum(0);
        runtime.gc();
        memory = runtime.totalMemory() - runtime.freeMemory();
        memProgressBar.setValue((int) memory);
        currentMem.setText(customFormat("###,###.###", (double) memory) + "B");
        totalMem.setText(customFormat("###,###.###", (double) runtime.totalMemory()) + "B");
    }
    /**
     * Starts checking the memory usage. If no timer is set, creates one
     */
    public void startMemoryCheck() {
        isMemoryChecked = true;
        memoryCheckBtn.setText("Stop memory check");
        if (memoryCheckTimer == null) {
            memoryCheckTimer = new Timer(1000, new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    checkMemory();
                }
            });
            memoryCheckTimer.setRepeats(true);
        }
        memoryCheckTimer.start();
    }
    //endregion
    //region getters and setters
    /**
     * Returns if simulation is paused
     *
     * @return <code>true</code> if paused, <code>false</code> otherwise
     */
    public boolean isPaused() {
        return paused;
    }
    /**
     * Sets simulation paused indicator
     *
     * @param paused <code>true</code> if paused, <code>false</code> otherwise
     */
    public void setPaused(boolean paused) {
        this.paused = paused;
    }
    /**
     * Returns <code>JScrollPane</code> for simulation log
     *
     * @return <code>JScrollPane</code> for simulation log
     */
    public JScrollPane getSimulationLogScroller(){
        return this.simulationLogScroller;
    }
    /**
     * Returns <code>JScrollPane</code> for to be sent log
     *
     * @return <code>JScrollPane</code> for to be sent log
     */
    public JScrollPane getToBeSentScroller(){
        return this.toBeSentScroller;
    }
    /**
     * Returns <code>JScrollPane</code> for error log
     *
     * @return <code>JScrollPane</code> for error log
     */
    public JScrollPane getErrorLogScroller(){
        return this.errorLogScroller;
    }
    /**
     * Returns server information table
     *
     * @return server information table
     */
    public JDetailsTable getServerTable(){
        return serverTable;
    }
    /**
     * Returns internet connection indicator
     *
     * @return internet connection indicator
     */
    public boolean isInternetConnection() {
        return this.internetConnection;
    }
    /**
     * Sets internet connection indicator
     *
     * @param b <code>true</code> if internet connection is on, <code>false</code> otherwise
     */
    public void setInternetConnection(boolean b) {
        this.internetConnection = b;
    }
    /**
     * Returns simulation running indicator
     *
     * @return <code>true</code> if simulation is running, <code>false</code> otherwise
     */
    public boolean isRunning() {
        return running;
    }
    /**
     * Sets whether simulation is running
     *
     * @param b <code>true</code> to set simulation as running, <code>false</code> otherwise
     */
    public void setRunning(boolean b) {
        this.running = b;
    }
    /**
     * Returns whether sensors generate new messages
     *
     * @return <code>true</code> if generate, <code>false</code> otherwise
     */
    public boolean isSensorsPaused(){
        return this.sensorsPaused;
    }
    /**
     * Sets whether sensors should generate new messages or not
     *
     * @param b <code>true</code> if generate, <code>false</code> otherwise
     */
    public void setSensorsPaused(boolean b){
        this.sensorsPaused = b;
    }
    /**
     * Returns number of created sensors
     *
     * @return number of created sensors
     */
    public static int getSensorsCount(){
        return sensorsCount;
    }
    /**
     * Returns whether simulation is saved
     *
     * @return <code>true</code> if is saved, <code>false</code> otherwise
     */
    public boolean isSaved(){
        return this.saved;
    }
    /**
     * Sets whether simulation is saved
     *
     * @param b <code>true</code> if is saved, <code>false</code> otherwise
     */
    public void setSaved(Boolean b){
        this.saved = b;
    }
    //endregion
    /**
     * Called when user tries to close frame. If there is unsaved simulation, offers user dialog to
     * save it
     */
    private void onCancel(){
        if(!isSaved() && adapters.size() != 0){
            Object[] options = { "Yes",  "No"};
            int dialogResult = JOptionPane.showOptionDialog(null, "Simulation is unsaved. Do you want to save?", "Warning",
                    JOptionPane.DEFAULT_OPTION, JOptionPane.WARNING_MESSAGE,
                    null, options, options[0]);
            if (dialogResult == 0) {
                //print logs and save simulation
                logger.printLogs("Performance Simulation -> adapters count="+ adapters.size());
                saveSimulationToXML();
                //if simulation is still running, stop it
                if(isRunning())
                    stopSimulation();
            }else if (dialogResult == 1){
                //if user doesn't want to save, stop simulation if running and dispose frame
                if(isRunning())
                    stopSimulation();
                dispose();
            }
        }else{
            logger.deleteBuffer();
        }
    }
    /**
     * Clears adapters list
     */
    private void deleteAdapters(){
        for(Adapter adapter : adapters){
            adapter.delete();
        }
        adapters.clear();
    }
    /**
     * Sets default values of configuration text fields, creates new {@link com.smarthome.emulator.simulations.panels.ServerPanel}
     * , resets generators seeds to <code>null</code> and creates new generators
     */
    private void setDefault(){
        //fillDefaultData();
        resetSeeds();
        createServerPanel();
        createGenerators();
    }
    /**
     * Sets all generators seeds to <code>null</code>
     */
    private void resetSeeds(){
        sensorCountGeneratorSeed = null;
        refreshRateGeneratorSeed = null;
        signalGeneratorSeed = null;
        batteryGeneratorSeed = null;
        valueTypeGeneratorSeed = null;
        temperatureSeed = null;
        humiditySeed = null;
        pressureSeed = null;
        lightIntensitySeed = null;
        noiseIntensitySeed = null;
        emissionsSeed = null;
    }
    /**
     * Opens load dialog. From given XML file loads and sets configuration text fields. Also creates new generators
     * from seed loaded from file.
     * <p>
     * Shows errors:
     * <ul>
     * <li>LOAD_DOC_CREATE if cannot create document from given file</li>
     * <li>LOAD_DOC_READ if cannot read given file</li>
     * <li>LOAD_DOC_PARSE if cannot parse given file</li>
     * <li>FILE_NOT_FOUND if given file was not found</li>
     * </ul>
     * </p>
     */
    private void loadSimulationFromXML(){
        try {
            String xmlLoadFile = showDialog("Specify file to load", false);
            if (xmlLoadFile != null) {
                File fXmlFile = new File(xmlLoadFile);
                DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
                DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
                Document doc = dBuilder.parse(fXmlFile);
                //get all adapters
                Element rootElement = (Element) doc.getElementsByTagName("simulation").item(0);
                adaptersNumberTextField.setText(rootElement.getElementsByTagName("adaptersCount").item(0).getTextContent());
                adapterSensorProtocolTextField.setText(rootElement.getElementsByTagName("adapterSensorProtocol").item(0).getTextContent());
                adapterServerProtocolTextField.setText(rootElement.getElementsByTagName("adapterServerProtocol").item(0).getTextContent());
                startFromIdTextField.setText(rootElement.getElementsByTagName("startFromId").item(0).getTextContent());
                Element sensorsPerAdapterElement = (Element) rootElement.getElementsByTagName("sensorsPerAdapter").item(0);
                sensorsPerAdapterFromTextField.setText(sensorsPerAdapterElement.getAttribute("from"));
                sensorsPerAdapterToTextField.setText(sensorsPerAdapterElement.getAttribute("to"));
                Element refreshRateElement = (Element) rootElement.getElementsByTagName("refreshRate").item(0);
                refreshRateFromTextField.setText(refreshRateElement.getAttribute("from"));
                refreshRateToTextField.setText(refreshRateElement.getAttribute("to"));
                //seeds section
                Element seedsElement = (Element) rootElement.getElementsByTagName("seeds").item(0);
                sensorCountGeneratorSeed = Long.valueOf(seedsElement.getElementsByTagName("sensorCount").item(0).getTextContent());
                refreshRateGeneratorSeed = Long.valueOf(seedsElement.getElementsByTagName("refreshRate").item(0).getTextContent());
                signalGeneratorSeed = Long.valueOf(seedsElement.getElementsByTagName("signal").item(0).getTextContent());
                batteryGeneratorSeed = Long.valueOf(seedsElement.getElementsByTagName("battery").item(0).getTextContent());
                valueTypeGeneratorSeed = Long.valueOf(seedsElement.getElementsByTagName("valueType").item(0).getTextContent());
                //server section
                Element serverElement = (Element) rootElement.getElementsByTagName("server").item(0);
                String hostname = serverElement.getElementsByTagName("hostname").item(0).getTextContent();
                if(!hostname.trim().equals("")){
                    addressTextField.setText(hostname);
                    hostRadBtn.setSelected(true);
                    ipRadBtn.setSelected(false);
                }else{
                    addressTextField.setText(serverElement.getElementsByTagName("ip").item(0).getTextContent());
                    hostRadBtn.setSelected(false);
                    ipRadBtn.setSelected(true);
                }
                portTextField.setText(serverElement.getElementsByTagName("port").item(0).getTextContent());
                //values section
                Element sensorValuesElement = (Element) rootElement.getElementsByTagName("sensorValues").item(0);
                // ---- temperature
                Element temperatureElement = (Element) sensorValuesElement.getElementsByTagName("temperature").item(0);
                temperatureCheckBox.setSelected(temperatureElement.getAttribute("checked").equals("1"));
                temperatureMaxTextField.setText(temperatureElement.getAttribute("max"));
                temperatureMinTextField.setText(temperatureElement.getAttribute("min"));
                temperatureDevTextField.setText(temperatureElement.getAttribute("dev"));
                temperatureAvgTextField.setText(temperatureElement.getAttribute("avg"));
                temperatureSeed = Long.valueOf(temperatureElement.getAttribute("seed"));
                // ---- humidity
                Element humidityElement = (Element) sensorValuesElement.getElementsByTagName("humidity").item(0);
                humidityCheckBox.setSelected(humidityElement.getAttribute("checked").equals("1"));
                humidityMaxTextField.setText(humidityElement.getAttribute("max"));
                humidityMinTextField.setText(humidityElement.getAttribute("min"));
                humidityDevTextField.setText(humidityElement.getAttribute("dev"));
                humidityAvgTextField.setText(humidityElement.getAttribute("avg"));
                humiditySeed = Long.valueOf(humidityElement.getAttribute("seed"));
                // ---- pressure
                Element pressureElement = (Element) sensorValuesElement.getElementsByTagName("pressure").item(0);
                pressureCheckBox.setSelected(pressureElement.getAttribute("checked").equals("1"));
                pressureMaxTextField.setText(pressureElement.getAttribute("max"));
                pressureMinTextField.setText(pressureElement.getAttribute("min"));
                pressureDevTextField.setText(pressureElement.getAttribute("dev"));
                pressureAvgTextField.setText(pressureElement.getAttribute("avg"));
                pressureSeed = Long.valueOf(pressureElement.getAttribute("seed"));
                // ---- sensorOfActuator
                Element sensorOfActuatorElement = (Element) sensorValuesElement.getElementsByTagName("sensorOfActuator").item(0);
                sensorOfActuatorCheckBox.setSelected(sensorOfActuatorElement.getAttribute("checked").equals("1"));
                String sensorOfActuatorValue = sensorOfActuatorElement.getAttribute("value");
                if(sensorOfActuatorValue.equals("true")){
                    sensorOfActuatorFalseRadBtn.setSelected(false);
                    sensorOfActuatorTrueRadBtn.setSelected(true);
                }else{
                    sensorOfActuatorFalseRadBtn.setSelected(true);
                    sensorOfActuatorTrueRadBtn.setSelected(false);
                }
                // ---- lightIntensity
                Element lightIntensityElement = (Element) sensorValuesElement.getElementsByTagName("lightIntensity").item(0);
                lightIntensityCheckBox.setSelected(lightIntensityElement.getAttribute("checked").equals("1"));
                lightIntensityMaxTextField.setText(lightIntensityElement.getAttribute("max"));
                lightIntensityMinTextField.setText(lightIntensityElement.getAttribute("min"));
                lightIntensityDevTextField.setText(lightIntensityElement.getAttribute("dev"));
                lightIntensityAvgTextField.setText(lightIntensityElement.getAttribute("avg"));
                lightIntensitySeed = Long.valueOf(lightIntensityElement.getAttribute("seed"));
                // ---- noise intensity
                Element noiseIntensityElement = (Element) sensorValuesElement.getElementsByTagName("noiseIntensity").item(0);
                noiseIntensityCheckBox.setSelected(noiseIntensityElement.getAttribute("checked").equals("1"));
                noiseIntensityMaxTextField.setText(noiseIntensityElement.getAttribute("max"));
                noiseIntensityMinTextField.setText(noiseIntensityElement.getAttribute("min"));
                noiseIntensityDevTextField.setText(noiseIntensityElement.getAttribute("dev"));
                noiseIntensityAvgTextField.setText(noiseIntensityElement.getAttribute("avg"));
                noiseIntensitySeed = Long.valueOf(noiseIntensityElement.getAttribute("seed"));
                // ---- emissions
                Element emissionsElement = (Element) sensorValuesElement.getElementsByTagName("emissions").item(0);
                emissionsCheckBox.setSelected(emissionsElement.getAttribute("checked").equals("1"));
                emissionsMaxTextField.setText(emissionsElement.getAttribute("max"));
                emissionsMinTextField.setText(emissionsElement.getAttribute("min"));
                emissionsDevTextField.setText(emissionsElement.getAttribute("dev"));
                emissionsAvgTextField.setText(emissionsElement.getAttribute("avg"));
                emissionsSeed = Long.valueOf(emissionsElement.getAttribute("seed"));
                createGenerators();
                setSaved(true);
            }
        } catch (ParserConfigurationException pce) {
            Emulator.showError(Error.LOAD_DOC_CREATE.getMessage());
        } catch (IOException ioe) {
            Emulator.showError(Error.LOAD_DOC_READ.getMessage());
        } catch (SAXException sae) {
            Emulator.showError(Error.LOAD_DOC_PARSE.getMessage());
        } catch (IllegalArgumentException iae) {
            Emulator.showError(Error.FILE_NOT_FOUND.getMessage());
        }
    }
    /**
     * Opens save dialog. To given file saves values from configuration text fields and generators seeds
     * <p>
     * Shows errors:
     * <ul>
     * <li>SAVE_DOC_CREATE if cannot create document for given file</li>
     * </ul>
     * </p>
     */
    private void saveSimulationToXML(){
        try {
            String xmlSaveFile = showDialog("Specify file to save", true);
            if (xmlSaveFile != null) {
                DocumentBuilderFactory docFactory = DocumentBuilderFactory.newInstance();
                DocumentBuilder docBuilder = docFactory.newDocumentBuilder();
                // create document
                Document doc = docBuilder.newDocument();
                //insert root element
                Element rootElement = doc.createElement("simulation");
                doc.appendChild(rootElement);

                //rootElement adapters count
                Element adaptersCountElement = doc.createElement("adaptersCount");
                adaptersCountElement.appendChild(doc.createTextNode(adaptersNumberTextField.getText()));
                rootElement.appendChild(adaptersCountElement);
                //rootElement adapter sensor protocol
                Element adapterSensorProtocolElement = doc.createElement("adapterSensorProtocol");
                adapterSensorProtocolElement.appendChild(doc.createTextNode(adapterSensorProtocolTextField.getText()));
                rootElement.appendChild(adapterSensorProtocolElement);
                //rootElement adapter server protocol
                Element adapterServerProtocolElement = doc.createElement("adapterServerProtocol");
                adapterServerProtocolElement.appendChild(doc.createTextNode(adapterServerProtocolTextField.getText()));
                rootElement.appendChild(adapterServerProtocolElement);
                //rootElement start from id
                Element startFromIdElement = doc.createElement("startFromId");
                startFromIdElement.appendChild(doc.createTextNode(startFromIdTextField.getText()));
                rootElement.appendChild(startFromIdElement);
                //rootElement sensors per adapter
                Element sensorsPerAdapterElement = doc.createElement("sensorsPerAdapter");
                sensorsPerAdapterElement.setAttribute("from",sensorsPerAdapterFromTextField.getText());
                sensorsPerAdapterElement.setAttribute("to",sensorsPerAdapterToTextField.getText());
                rootElement.appendChild(sensorsPerAdapterElement);
                //rootElement refresh rate
                Element refreshRateElement = doc.createElement("refreshRate");
                refreshRateElement.setAttribute("from",refreshRateFromTextField.getText());
                refreshRateElement.setAttribute("to",refreshRateToTextField.getText());
                rootElement.appendChild(refreshRateElement);

                //rootElement generators seeds
                Element generatorsSeedsElement = doc.createElement("seeds");
                rootElement.appendChild(generatorsSeedsElement);
                //generatorsSeedsElement sensors count seed
                Element sensorCountSeedElement = doc.createElement("sensorCount");
                sensorCountSeedElement.appendChild(doc.createTextNode(String.valueOf(sensorCountGeneratorSeed)));
                generatorsSeedsElement.appendChild(sensorCountSeedElement);
                //generatorsSeedsElement refresh rate
                Element refreshRateSeedElement = doc.createElement("refreshRate");
                refreshRateSeedElement.appendChild(doc.createTextNode(String.valueOf(refreshRateGeneratorSeed)));
                generatorsSeedsElement.appendChild(refreshRateSeedElement);
                //generatorsSeedsElement signal seed
                Element signalSeedElement = doc.createElement("signal");
                signalSeedElement.appendChild(doc.createTextNode(String.valueOf(refreshRateGeneratorSeed)));
                generatorsSeedsElement.appendChild(signalSeedElement);
                //generatorsSeedsElement battery seed
                Element batterySeedElement = doc.createElement("battery");
                batterySeedElement.appendChild(doc.createTextNode(String.valueOf(batteryGeneratorSeed)));
                generatorsSeedsElement.appendChild(batterySeedElement);
                //generatorsSeedsElement value type
                Element valueTypeSeedElement = doc.createElement("valueType");
                valueTypeSeedElement.appendChild(doc.createTextNode(String.valueOf(valueTypeGeneratorSeed)));
                generatorsSeedsElement.appendChild(valueTypeSeedElement);

                //rootElement server
                Element serverElement = doc.createElement("server");
                rootElement.appendChild(serverElement);
                //serverElement hostname
                Element hostname = doc.createElement("hostname");
                hostname.appendChild(doc.createTextNode(hostRadBtn.isSelected()?addressTextField.getText():""));
                serverElement.appendChild(hostname);
                //serverElement ip
                Element ip = doc.createElement("ip");
                ip.appendChild(doc.createTextNode(ipRadBtn.isSelected()?addressTextField.getText():""));
                serverElement.appendChild(ip);
                //serverElement port
                Element port = doc.createElement("port");
                port.appendChild(doc.createTextNode(portTextField.getText()));
                serverElement.appendChild(port);

                //rootElement sensor values
                Element sensorValuesElement = doc.createElement("sensorValues");
                rootElement.appendChild(sensorValuesElement);
                //sensorValuesElement temperature
                Element temperatureElement = doc.createElement("temperature");
                temperatureElement.setAttribute("checked",temperatureCheckBox.isSelected()?"1":"0");
                temperatureElement.setAttribute("max",temperatureMaxTextField.getText());
                temperatureElement.setAttribute("min",temperatureMinTextField.getText());
                temperatureElement.setAttribute("dev",temperatureDevTextField.getText());
                temperatureElement.setAttribute("avg",temperatureAvgTextField.getText());
                temperatureElement.setAttribute("seed",String.valueOf(temperatureSeed));
                sensorValuesElement.appendChild(temperatureElement);
                //sensorValuesElement humidity
                Element humidityElement = doc.createElement("humidity");
                humidityElement.setAttribute("checked",humidityCheckBox.isSelected()?"1":"0");
                humidityElement.setAttribute("max",humidityMaxTextField.getText());
                humidityElement.setAttribute("min", humidityMinTextField.getText());
                humidityElement.setAttribute("dev",humidityDevTextField.getText());
                humidityElement.setAttribute("avg",humidityAvgTextField.getText());
                humidityElement.setAttribute("seed",String.valueOf(humiditySeed));
                sensorValuesElement.appendChild(humidityElement);
                //sensorValuesElement pressure
                Element pressureElement = doc.createElement("pressure");
                pressureElement.setAttribute("checked",pressureCheckBox.isSelected()?"1":"0");
                pressureElement.setAttribute("max",pressureMaxTextField.getText());
                pressureElement.setAttribute("min",pressureMinTextField.getText());
                pressureElement.setAttribute("dev",pressureDevTextField.getText());
                pressureElement.setAttribute("avg",pressureAvgTextField.getText());
                pressureElement.setAttribute("seed",String.valueOf(pressureSeed));
                sensorValuesElement.appendChild(pressureElement);
                //sensorValuesElement sensor of actuator
                Element sensorOfActuatorElement = doc.createElement("sensorOfActuator");
                sensorOfActuatorElement.setAttribute("checked",sensorOfActuatorCheckBox.isSelected()?"1":"0");
                sensorOfActuatorElement.setAttribute("value",sensorOfActuatorTrueRadBtn.isSelected()?"true":"false");
                sensorValuesElement.appendChild(sensorOfActuatorElement);
                //sensorValuesElement light intensity
                Element lightIntensityElement = doc.createElement("lightIntensity");
                lightIntensityElement.setAttribute("checked",lightIntensityCheckBox.isSelected()?"1":"0");
                lightIntensityElement.setAttribute("max",lightIntensityMaxTextField.getText());
                lightIntensityElement.setAttribute("min",lightIntensityMinTextField.getText());
                lightIntensityElement.setAttribute("dev",lightIntensityDevTextField.getText());
                lightIntensityElement.setAttribute("avg",lightIntensityAvgTextField.getText());
                lightIntensityElement.setAttribute("seed",String.valueOf(lightIntensitySeed));
                sensorValuesElement.appendChild(lightIntensityElement);
                //sensorValuesElement noise intensity
                Element noiseIntensityElement = doc.createElement("noiseIntensity");
                noiseIntensityElement.setAttribute("checked",noiseIntensityCheckBox.isSelected()?"1":"0");
                noiseIntensityElement.setAttribute("max",noiseIntensityMaxTextField.getText());
                noiseIntensityElement.setAttribute("min",noiseIntensityMinTextField.getText());
                noiseIntensityElement.setAttribute("dev",noiseIntensityDevTextField.getText());
                noiseIntensityElement.setAttribute("avg",noiseIntensityAvgTextField.getText());
                noiseIntensityElement.setAttribute("seed",String.valueOf(noiseIntensitySeed));
                sensorValuesElement.appendChild(noiseIntensityElement);
                //sensorValuesElement noise intensity
                Element emissionsElement = doc.createElement("emissions");
                emissionsElement.setAttribute("checked",emissionsCheckBox.isSelected()?"1":"0");
                emissionsElement.setAttribute("max",emissionsMaxTextField.getText());
                emissionsElement.setAttribute("min",emissionsMinTextField.getText());
                emissionsElement.setAttribute("dev",emissionsDevTextField.getText());
                emissionsElement.setAttribute("avg",emissionsAvgTextField.getText());
                emissionsElement.setAttribute("seed",String.valueOf(emissionsSeed));
                sensorValuesElement.appendChild(emissionsElement);

                TransformerFactory transformerFactory = TransformerFactory.newInstance();
                Transformer transformer = transformerFactory.newTransformer();
                DOMSource source = new DOMSource(doc);

                StreamResult result = new StreamResult(new File(xmlSaveFile));
                transformer.transform(source, result);
                Emulator.showMessage("File saved");
                setSaved(true);
            }
        } catch (ParserConfigurationException | TransformerException pce) {
            Emulator.showError(Error.SAVE_DOC_CREATE.getMessage());
        }
    }
    /**
     * Creates randomize amount of sensors for {@link com.smarthome.emulator.simulations.basis.Adapter} given as parameter.
     * Also randomizes refresh rate value,signal value, battery value and sensor type value.
     *
     * @param adapter adapter for which should be created sensors
     */
    private void randomizeSensors(Adapter adapter){
        Properties properties = new Properties();
        int sensorsCount;
        if(sensorsPerAdapterFromTextField.getText().trim().equals("") || sensorsPerAdapterFromTextField.getInt() == 0){
            sensorsCount = sensorsPerAdapterToTextField.getInt();
        }else{
            sensorsCount = sensorCountGenerator.nextInt((sensorsPerAdapterToTextField.getInt() - sensorsPerAdapterFromTextField.getInt()) + 1) + sensorsPerAdapterFromTextField.getInt();
        }
        logger.writeLogMessage("Creating Adapter/" + adapter.getAdapterId() + " with " + sensorsCount + " sensor/s.", logger.BOLD_STYLE);
        for(int i = 0; i < sensorsCount;i++){
            int refreshRate;
            if(refreshRateFromTextField.getText().trim().equals("") || refreshRateFromTextField.getInt() == 0){
                refreshRate = refreshRateToTextField.getInt();
            }else{
                refreshRate = sensorCountGenerator.nextInt((refreshRateToTextField.getInt() - refreshRateFromTextField.getInt()) + 1) + refreshRateFromTextField.getInt();
            }
            properties.setProperty("refreshRate",String.valueOf(refreshRate));
            int signal = signalGenerator.nextInt((SIGNAL_MAX - SIGNAL_MIN) + 1) + SIGNAL_MIN;
            properties.setProperty("signal",String.valueOf(signal));
            int battery = batteryGenerator.nextInt((BATTERY_MAX - BATTERY_MIN) + 1) + BATTERY_MIN;
            properties.setProperty("battery",String.valueOf(battery));
            properties.setProperty("protocol",adapterSensorProtocolTextField.getText());
            int valueType;
            boolean repeat = true;
            while(repeat){
                valueType = valueTypeGenerator.nextInt((VALUE_TYPE_MAX - VALUE_TYPE_MIN) + 1) + VALUE_TYPE_MIN;
                switch (valueType){
                    case 0:
                        if(temperatureCheckBox.isSelected()){
                            properties.setProperty("valueTypes","0");
                            properties.setProperty("valueClasses","Float");
                            properties.setProperty("values",temperatureAvgTextField.getText());
                            properties.setProperty("max",temperatureMaxTextField.getText());
                            properties.setProperty("min",temperatureMinTextField.getText());
                            properties.setProperty("dev",temperatureDevTextField.getText());
                            properties.setProperty("avg",temperatureAvgTextField.getText());
                            properties.setProperty("seed",String.valueOf(temperatureSeed));
                            repeat = false;
                        }
                        break;
                    case 1:
                        if(humidityCheckBox.isSelected()){
                            properties.setProperty("valueTypes","1");
                            properties.setProperty("valueClasses","Float");
                            properties.setProperty("values",humidityAvgTextField.getText());
                            properties.setProperty("max",humidityMaxTextField.getText());
                            properties.setProperty("min",humidityMinTextField.getText());
                            properties.setProperty("dev",humidityDevTextField.getText());
                            properties.setProperty("avg",humidityAvgTextField.getText());
                            properties.setProperty("seed",String.valueOf(humiditySeed));
                            repeat = false;
                        }
                        break;
                    case 2:
                        if(pressureCheckBox.isSelected()){
                            properties.setProperty("valueTypes","2");
                            properties.setProperty("valueClasses","Float");
                            properties.setProperty("values",pressureAvgTextField.getText());
                            properties.setProperty("max",pressureMaxTextField.getText());
                            properties.setProperty("min",pressureMinTextField.getText());
                            properties.setProperty("dev", pressureDevTextField.getText());
                            properties.setProperty("avg", pressureAvgTextField.getText());
                            properties.setProperty("seed",String.valueOf(pressureSeed));
                            repeat = false;
                        }
                        break;
                    case 3:
                        if(sensorOfActuatorCheckBox.isSelected()){
                            properties.setProperty("valueTypes","3");
                            properties.setProperty("valueClasses","Boolean");
                            properties.setProperty("values",sensorOfActuatorTrueRadBtn.isSelected()?"true":"false");
                            properties.setProperty("max","none");
                            properties.setProperty("min","none");
                            properties.setProperty("dev","none");
                            properties.setProperty("avg","none");
                            repeat = false;
                        }
                        break;
                    case 5:
                        if(lightIntensityCheckBox.isSelected()){
                            properties.setProperty("valueTypes","5");
                            properties.setProperty("valueClasses","Float");
                            properties.setProperty("values",lightIntensityAvgTextField.getText());
                            properties.setProperty("max",lightIntensityMaxTextField.getText());
                            properties.setProperty("min",lightIntensityMinTextField.getText());
                            properties.setProperty("dev",lightIntensityDevTextField.getText());
                            properties.setProperty("avg",lightIntensityAvgTextField.getText());
                            properties.setProperty("seed",String.valueOf(lightIntensitySeed));
                            repeat = false;
                        }
                        break;
                    case 6:
                        if(noiseIntensityCheckBox.isSelected()){
                            properties.setProperty("valueTypes","6");
                            properties.setProperty("valueClasses","Float");
                            properties.setProperty("values",noiseIntensityAvgTextField.getText());
                            properties.setProperty("max",noiseIntensityMaxTextField.getText());
                            properties.setProperty("min",noiseIntensityMinTextField.getText());
                            properties.setProperty("dev",noiseIntensityDevTextField.getText());
                            properties.setProperty("avg",noiseIntensityAvgTextField.getText());
                            properties.setProperty("seed",String.valueOf(noiseIntensitySeed));
                            repeat = false;
                        }
                        break;
                    case 7:
                        if(emissionsCheckBox.isSelected()){
                            properties.setProperty("valueTypes","7");
                            properties.setProperty("valueClasses","Float");
                            properties.setProperty("values",emissionsAvgTextField.getText());
                            properties.setProperty("max",emissionsMaxTextField.getText());
                            properties.setProperty("min",emissionsMinTextField.getText());
                            properties.setProperty("dev",emissionsDevTextField.getText());
                            properties.setProperty("avg",emissionsAvgTextField.getText());
                            properties.setProperty("seed",String.valueOf(emissionsSeed));
                            repeat = false;
                        }
                        break;
                    default:
                        break;
                }
            }
            adapter.addSensor(properties);
        }

    }
    /**
     * Creates {@link com.smarthome.emulator.simulations.basis.Logger} for simulation
     */
    private void createLogger(){
        Boolean bufferedLog = true;
        this.logger = new Logger(getSimulationLogScroller(),getToBeSentScroller(),getErrorLogScroller(),bufferedLog);
        this.logger.setActiveViewPorts();
        this.logger.setScrollLog(false);
        this.logger.setTrackResponse(true);
        if(bufferedLog){
            getSimulationLogScroller().setComponentPopupMenu(new JIOTLogPopUpMenu());
        }
    }
    /**
     * Creates {@link com.smarthome.emulator.simulations.panels.ServerPanel} and it's
     * table model for simulation
     */
    private void createServerPanel(){
        serverTableModel = ServerPanel.createModel();
        this.serverPanel = new ServerPanel(getServerTable(),serverTableModel);
    }
    /**
     * Creates new generators
     */
    private void createGenerators(){
        createGeneratorsSeeds();
        sensorCountGenerator = new Random(sensorCountGeneratorSeed);
        refreshRateGenerator = new Random(refreshRateGeneratorSeed);
        signalGenerator = new Random(signalGeneratorSeed);
        batteryGenerator = new Random(batteryGeneratorSeed);
        valueTypeGenerator = new Random(valueTypeGeneratorSeed);
    }
    /**
     * Determines, if there exists generators seeds, if not creates ones
     */
    private void createGeneratorsSeeds(){
        if(sensorCountGeneratorSeed == null){
            sensorCountGeneratorSeed = System.currentTimeMillis();
        }
        if(refreshRateGeneratorSeed == null){
            refreshRateGeneratorSeed = System.currentTimeMillis();
        }
        if(signalGeneratorSeed == null){
            signalGeneratorSeed = System.currentTimeMillis();
        }
        if(batteryGeneratorSeed == null){
            batteryGeneratorSeed = System.currentTimeMillis();
        }
        if(valueTypeGeneratorSeed == null){
            valueTypeGeneratorSeed = System.currentTimeMillis();
        }
        if(temperatureSeed == null){
            temperatureSeed = System.currentTimeMillis();
        }
        if(humiditySeed == null){
            humiditySeed = System.currentTimeMillis();
        }
        if(pressureSeed == null){
            pressureSeed = System.currentTimeMillis();
        }
        if(lightIntensitySeed == null){
            lightIntensitySeed = System.currentTimeMillis();
        }
        if(noiseIntensitySeed == null){
            noiseIntensitySeed = System.currentTimeMillis();
        }
        if(emissionsSeed == null){
            emissionsSeed = System.currentTimeMillis();
        }
    }
    /**
     * Sets number of sensors to <code>0</code>
     */
    private static void restartSensorsCount(){
        sensorsCount = 0;
    }
    /**
     * Fill configuration fields with default data
     */
    private void fillDefaultData(){
        adaptersNumberTextField.setText("10");
        adapterSensorProtocolTextField.setText("1");
        adapterServerProtocolTextField.setText("1");
        startFromIdTextField.setText("1000");
        sensorsPerAdapterFromTextField.setText("");
        sensorsPerAdapterToTextField.setText("5");
        refreshRateFromTextField.setText("1");
        refreshRateToTextField.setText("20");
        hostRadBtn.setSelected(true);
        ipRadBtn.setSelected(false);
        addressTextField.setText("ant-2.fit.vutbr.cz");
        portTextField.setText("9091");
        temperatureMaxTextField.setText("50");
        temperatureMinTextField.setText("-50");
        temperatureAvgTextField.setText("34.5");
        temperatureDevTextField.setText("0.5");
        humidityMaxTextField.setText("100");
        humidityMinTextField.setText("0");
        humidityAvgTextField.setText("60.2");
        humidityDevTextField.setText("0.8");
    }
    /**
     * Show warning sign when configuratio of simulation is not correct
     *
     * @param warning message
     */
    private void fireWarning(String warning) {
        warningPanel.setVisible(true);
        warningLbl.setText(warning);
    }
    /**
     * Validates filled configuration fields
     *
     * @return <code>true</code> if all set correctly, <code>false</code> otherwise
     */
    private boolean validateConfiguration(){
        int possibleThreads = adaptersNumberTextField.getInt() * (sensorsPerAdapterToTextField.getInt() + 3);
        if(possibleThreads > MAX_THREADS){
            fireWarning("<html>Lower number of adapters or sensors per adapter!<br> (Adapters count * (Sensors per adapter + 3) < " + MAX_THREADS + "</html>");
            return false;
        }

        if(!sensorsPerAdapterFromTextField.getText().trim().equals("") && sensorsPerAdapterFromTextField.getInt() < 0){
            fireWarning("<html>Adjust sensors per adapter \"from\" field.<br> Minimum sensors count is 0</html>");
            return false;
        }
        if(!sensorsPerAdapterFromTextField.getText().trim().equals("") && sensorsPerAdapterToTextField.getInt() < sensorsPerAdapterFromTextField.getInt()){
            fireWarning("<html>Adjust sensors per adapter range.<br> \"from\" field is higher than \"to\" field</html>");
            return false;
        }
        if(refreshRateFromTextField.getInt() <= 0){
            fireWarning("<html>Adjust sensors per adapter \"from\" field.<br> Minimum sensors count is 1</html>");
            return false;
        }
        if(refreshRateToTextField.getInt() < refreshRateFromTextField.getInt()){
            fireWarning("<html>Adjust refresh rate range.<br> \"from\" field is higher than \"to\" field</html>");
            return false;
        }
        return true;
    }
    /**
     * Checks if all needed configuration fields are set
     *
     * @return <code>true</code> if all set correctly, <code>false</code> otherwise
     */
    private boolean checkConfigurationWhiteFields() {
        if (adaptersNumberTextField.getText().trim().equals("")) {
            fireWarning("Adapters count field is empty!");
            return false;
        }
        if (adapterSensorProtocolTextField.getText().trim().equals("")) {
            fireWarning("Adapter -> Sensor protocol field is empty!");
            return false;
        }
        if (adapterServerProtocolTextField.getText().trim().equals("")) {
            fireWarning("Adapter -> Server protocol field is empty!");
            return false;
        }
        if (startFromIdTextField.getText().trim().equals("")) {
            fireWarning("Start from Id field is empty!");
            return false;
        }
        /*if (sensorsPerAdapterFromTextField.getText().trim().equals("")) {
            fireWarning("Sensors per adapter \"from\" field is empty!");
            return false;
        }*/
        if (sensorsPerAdapterToTextField.getText().trim().equals("")) {
            fireWarning("Sensors per adapter \"to\" field is empty!");
            return false;
        }if (refreshRateFromTextField.getText().trim().equals("")) {
            fireWarning("Refresh rate \"from\" field is empty!");
            return false;
        }
        if (refreshRateToTextField.getText().trim().equals("")) {
            fireWarning("Refresh rate \"to\" field is empty!");
            return false;
        }
        if (portTextField.getText().trim().equals("")) {
            fireWarning("Port field is empty!");
            return false;
        }
        if (addressTextField.getText().trim().equals("")) {
            fireWarning("Address field is empty!");
            return false;
        }
        if(temperatureCheckBox.isSelected()){
            if(temperatureMaxTextField.getText().trim().equals("") ||
                    temperatureMinTextField.getText().trim().equals("") ||
                    temperatureDevTextField.getText().trim().equals("") ||
                    temperatureAvgTextField.getText().trim().equals("")){
                fireWarning("Fill all temperature fields!");
                return false;
            }
        }
        if(humidityCheckBox.isSelected()){
            if(humidityMaxTextField.getText().trim().equals("") ||
                    humidityMinTextField.getText().trim().equals("") ||
                    humidityDevTextField.getText().trim().equals("") ||
                    humidityAvgTextField.getText().trim().equals("")){
                fireWarning("Fill all humidity fields!");
                return false;
            }
        }
        if(lightIntensityCheckBox.isSelected()){
            if(lightIntensityMaxTextField.getText().trim().equals("") ||
                    lightIntensityMinTextField.getText().trim().equals("") ||
                    lightIntensityDevTextField.getText().trim().equals("") ||
                    lightIntensityAvgTextField.getText().trim().equals("")){
                fireWarning("Fill all humidity fields!");
                return false;
            }
        }
        if(pressureCheckBox.isSelected()){
            if(pressureMaxTextField.getText().trim().equals("") ||
                    pressureMinTextField.getText().trim().equals("") ||
                    pressureDevTextField.getText().trim().equals("") ||
                    pressureAvgTextField.getText().trim().equals("")){
                fireWarning("Fill all pressure fields!");
                return false;
            }
        }
        if(noiseIntensityCheckBox.isSelected()){
            if(noiseIntensityMaxTextField.getText().trim().equals("") ||
                    noiseIntensityMinTextField.getText().trim().equals("") ||
                    noiseIntensityDevTextField.getText().trim().equals("") ||
                    noiseIntensityAvgTextField.getText().trim().equals("")){
                fireWarning("Fill all noise intensity fields!");
                return false;
            }
        }
        if(emissionsCheckBox.isSelected()){
            if(emissionsMaxTextField.getText().trim().equals("") ||
                    emissionsMinTextField.getText().trim().equals("") ||
                    emissionsDevTextField.getText().trim().equals("") ||
                    emissionsAvgTextField.getText().trim().equals("")){
                fireWarning("Fill all emissions fields!");
                return false;
            }
        }
        return true;
    }
    /**
     * Enables all configuration fields for editing
     */
    private void enableConfigurationChange() {
        startFromIdTextField.setEnabled(true);
        adaptersNumberTextField.setEnabled(true);
        sensorsPerAdapterFromTextField.setEnabled(true);
        sensorsPerAdapterToTextField.setEnabled(true);
        refreshRateFromTextField.setEnabled(true);
        refreshRateToTextField.setEnabled(true);
        adapterSensorProtocolTextField.setEnabled(true);
        adapterServerProtocolTextField.setEnabled(true);

        temperatureMaxTextField.setEnabled(true);
        temperatureMinTextField.setEnabled(true);
        temperatureDevTextField.setEnabled(true);
        temperatureAvgTextField.setEnabled(true);

        humidityMaxTextField.setEnabled(true);
        humidityMinTextField.setEnabled(true);
        humidityDevTextField.setEnabled(true);
        humidityAvgTextField.setEnabled(true);

        pressureMaxTextField.setEnabled(true);
        pressureMinTextField.setEnabled(true);
        pressureDevTextField.setEnabled(true);
        pressureAvgTextField.setEnabled(true);

        sensorOfActuatorTrueRadBtn.setEnabled(true);
        sensorOfActuatorFalseRadBtn.setEnabled(true);

        lightIntensityMaxTextField.setEnabled(true);
        lightIntensityMinTextField.setEnabled(true);
        lightIntensityDevTextField.setEnabled(true);
        lightIntensityAvgTextField.setEnabled(true);

        noiseIntensityMaxTextField.setEnabled(true);
        noiseIntensityMinTextField.setEnabled(true);
        noiseIntensityDevTextField.setEnabled(true);
        noiseIntensityAvgTextField.setEnabled(true);

        emissionsMaxTextField.setEnabled(true);
        emissionsMinTextField.setEnabled(true);
        emissionsDevTextField.setEnabled(true);
        emissionsAvgTextField.setEnabled(true);

        temperatureCheckBox.setEnabled(true);
        humidityCheckBox.setEnabled(true);
        pressureCheckBox.setEnabled(true);
        lightIntensityCheckBox.setEnabled(true);
        noiseIntensityCheckBox.setEnabled(true);
        emissionsCheckBox.setEnabled(true);
        sensorOfActuatorCheckBox.setEnabled(true);

        hostRadBtn.setEnabled(true);
        ipRadBtn.setEnabled(true);
        addressTextField.setEnabled(true);
        portTextField.setEnabled(true);
    }
    /**
     * Disables all configuration fields
     */
    private void disableConfigurationChange() {
        startFromIdTextField.setEnabled(false);
        adaptersNumberTextField.setEnabled(false);
        sensorsPerAdapterFromTextField.setEnabled(false);
        sensorsPerAdapterToTextField.setEnabled(false);
        refreshRateFromTextField.setEnabled(false);
        refreshRateToTextField.setEnabled(false);
        adapterSensorProtocolTextField.setEnabled(false);
        adapterServerProtocolTextField.setEnabled(false);

        temperatureMaxTextField.setEnabled(false);
        temperatureMinTextField.setEnabled(false);
        temperatureDevTextField.setEnabled(false);
        temperatureAvgTextField.setEnabled(false);

        humidityMaxTextField.setEnabled(false);
        humidityMinTextField.setEnabled(false);
        humidityDevTextField.setEnabled(false);
        humidityAvgTextField.setEnabled(false);

        pressureMaxTextField.setEnabled(false);
        pressureMinTextField.setEnabled(false);
        pressureDevTextField.setEnabled(false);
        pressureAvgTextField.setEnabled(false);

        sensorOfActuatorTrueRadBtn.setEnabled(false);
        sensorOfActuatorFalseRadBtn.setEnabled(false);

        lightIntensityMaxTextField.setEnabled(false);
        lightIntensityMinTextField.setEnabled(false);
        lightIntensityDevTextField.setEnabled(false);
        lightIntensityAvgTextField.setEnabled(false);

        noiseIntensityMaxTextField.setEnabled(false);
        noiseIntensityMinTextField.setEnabled(false);
        noiseIntensityDevTextField.setEnabled(false);
        noiseIntensityAvgTextField.setEnabled(false);

        emissionsMaxTextField.setEnabled(false);
        emissionsMinTextField.setEnabled(false);
        emissionsDevTextField.setEnabled(false);
        emissionsAvgTextField.setEnabled(false);

        temperatureCheckBox.setEnabled(false);
        humidityCheckBox.setEnabled(false);
        pressureCheckBox.setEnabled(false);
        sensorOfActuatorCheckBox.setEnabled(false);
        lightIntensityCheckBox.setEnabled(false);
        noiseIntensityCheckBox.setEnabled(false);
        emissionsCheckBox.setEnabled(false);

        hostRadBtn.setEnabled(false);
        ipRadBtn.setEnabled(false);
        addressTextField.setEnabled(false);
        portTextField.setEnabled(false);
    }
    /**
     * Sets configuration panel to needed size so {@link javax.swing.JScrollPane} would show scrollbar
     */
    private void resizeConfigurationPanel() {
        configurationPanel.setPreferredSize(new Dimension(configurationPanel.getPreferredSize().width, configurationPanel.getPreferredSize().height + 670));
        configurationPanel.validate();
    }
    /**
     * Initializes all visual components, creates {@link com.smarthome.emulator.simulations.basis.Logger},
     * {@link com.smarthome.emulator.simulations.panels.ServerPanel}, all generators, print area and
     * server response chart
     */
    private void initComponents() {
        setTitle("Performance Simulation - SmartHome Emulator");
        UIModifications();
        fillDefaultData();
        setContentPane(mainPanel);
        setBounds(100, 100, 1316, 730);
        addMainBtnMouseListeners();
        mouseListeners();
        createLogger();
        createServerPanel();
        createGenerators();
        createResponseChart();
    }
    /**
     * Assign mouse listeners for main buttons
     */
    private void addMainBtnMouseListeners() {
        btnMainSave.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                saveSimulationToXML();
            }

            @Override
            public void mouseEntered(MouseEvent e) {
                e.getComponent().setBackground(Emulator.simColorBlue);
                e.getComponent().setForeground(Color.WHITE);
            }

            @Override
            public void mouseExited(MouseEvent e) {
                e.getComponent().setForeground(DetailedSimulation.simColorMenuItemFont);
                e.getComponent().setBackground(DetailedSimulation.simColorMenuItem);

            }
        });
        btnMainLoad.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if(adapters.size() > 0)
                    showSavePrintDialog();
                if(isRunning())
                    stopSimulation();
                loadSimulationFromXML();

            }

            @Override
            public void mouseEntered(MouseEvent e) {
                e.getComponent().setBackground(Emulator.simColorBlue);
                e.getComponent().setForeground(Color.WHITE);
            }

            @Override
            public void mouseExited(MouseEvent e) {
                e.getComponent().setForeground(DetailedSimulation.simColorMenuItemFont);
                e.getComponent().setBackground(DetailedSimulation.simColorMenuItem);

            }
        });

        btnMainPrintLogs.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                logger.printLogs("Performance Simulation -> adapters count="+ adapters.size());
            }

            @Override
            public void mouseEntered(MouseEvent e) {
                e.getComponent().setBackground(Emulator.simColorBlue);
                e.getComponent().setForeground(Color.WHITE);
            }

            @Override
            public void mouseExited(MouseEvent e) {
                e.getComponent().setForeground(DetailedSimulation.simColorMenuItemFont);
                e.getComponent().setBackground(DetailedSimulation.simColorMenuItem);

            }
        });
    }
    /**
     * Assigns mouse listeners for buttons
     */
    private void mouseListeners() {
        memoryCheckBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (isMemoryChecked)
                    stopMemoryCheck();
                else
                    startMemoryCheck();

            }
        });
        startSimulationBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (!isRunning()) {
                    startSimulation();
                }
            }
        });
        stopSimulationBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (isRunning()) {
                    showSavePrintDialog();
                }
            }
        });
        resumeSimulationBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (isRunning() && isPaused()) {
                    resumeSimulation();
                }
            }
        });
        pauseSimulationBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (isRunning() && !isPaused()) {
                    pauseSimulation();
                }
            }
        });
        resumeSensorsBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (isRunning() && !isPaused() && isSensorsPaused()) {
                    resumeSensors();
                }
            }
        });
        pauseSensorsBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (isRunning() && !isPaused() && !isSensorsPaused()) {
                    pauseSensors();
                }
            }
        });
        disconnectBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                setInternetConnection(false);
                for (Adapter adapter : adapters) {
                    adapter.setInternetConnection(false);
                }
                switchButtonsLook(connectBtn, disconnectBtn);
            }
        });
        connectBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                setInternetConnection(true);
                for (Adapter adapter : adapters) {
                    adapter.setInternetConnection(isInternetConnection());
                }
                switchButtonsLook(disconnectBtn, connectBtn);
            }
        });
    }
    /**
     * Switches given {@link javax.swing.JButton}s background to
     * indicate active and inactive button
     *
     * @param active   to be active button
     * @param inactive to be inactive button
     */
    private void switchButtonsLook(JButton active, JButton inactive) {
        setActiveBtnBackground(active);
        setInactiveBtnBackground(inactive);
    }
    /**
     * Sets active button background
     *
     * @param button to be active
     */
    private void setActiveBtnBackground(JButton button){
        button.setBackground(new Color(-5841665));
        button.setForeground(new Color(-12303292));

    }
    /**
     * Sets inactive button background
     *
     * @param button to be inactive
     */
    private void setInactiveBtnBackground(JButton button){
        button.setBackground(new Color(-3750202));
        button.setForeground(new Color(-1));
    }
    /**
     * Creates new timer for chart updating data
     */
    private void setResponseChartTimer(){
        responseChartTimer  = new Timer(REFRESH_TIME, new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                refreshChart();
            }
        });
    }
    /**
     * Clears server response time chart data
     */
    private void resetChartDataset(){
        chartSeries.clear();
    }
    /**
     * Creates new {@link org.jfree.chart.ChartPanel} from {@link org.jfree.chart.JFreeChart} and
     * adds this panel to main panel
     */
    private void createResponseChart(){
        serverResponsePanel.setVisible(false);
        chart = createChart(createChartDataset(), CHART_COLOR);
        ChartPanel panel = new ChartPanel(chart);
        panel.setMouseWheelEnabled(true);
        panel.setLayout(new GridLayoutManager(1, 1, new Insets(5, 2, 2, 2), -1, -1));
        panel.setBackground(new Color(-1));
        panel.setMaximumSize(new Dimension(245, -1));
        panel.setMinimumSize(new Dimension(245, 65));
        panel.setPreferredSize(new Dimension(245, 65));
        mainPanel.add(panel, new GridConstraints(8, 0, 1, 2, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_VERTICAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(360, 280), new Dimension(360, 280), new Dimension(360, 280), 0, false));
        setResponseChartTimer();
    }
    /**
     * Creates new chart from given data set. Sets all needed information for
     * correct chart visualization
     *
     * @param dataset    set of data for chart
     * @param chartColor color of line generated by chart
     * @return newly crated chart
     */
    private static JFreeChart createChart(XYDataset dataset,Color chartColor){
        JFreeChart chart = ChartFactory.createTimeSeriesChart(
                null,  // title
                "HH:mm:ss",             // x-axis label
                "ms",   // y-axis label
                 dataset,            // data
                 false,               // create legend?
                 false,               // generate tooltips?
                 false               // generate URLs?
        );
        chart.setBackgroundPaint(Color.white);
        chart.setBorderPaint(LABEL_COLOR);
        XYPlot plot = (XYPlot) chart.getPlot();
        plot.setBackgroundPaint(Color.WHITE);
        plot.setDomainGridlinePaint(Color.lightGray);
        plot.setRangeGridlinePaint(Color.lightGray);
        plot.setDomainCrosshairVisible(true);
        plot.setRangeCrosshairVisible(true);
        XYItemRenderer r = plot.getRenderer();
        if (r instanceof XYLineAndShapeRenderer) {
            XYLineAndShapeRenderer renderer = (XYLineAndShapeRenderer) r;
            renderer.setBaseShapesVisible(false);
            renderer.setBaseShapesFilled(false);
            renderer.setDrawSeriesLineAsPath(true);
            renderer.setSeriesPaint(0,chartColor);
            renderer.setBaseFillPaint(LABEL_COLOR);
        }
        ValueAxis YAxis = (ValueAxis) plot.getRangeAxis();
        YAxis.setLabelPaint(LABEL_COLOR);
        YAxis.setAxisLinePaint(LABEL_COLOR);
        DateAxis XAxis = (DateAxis) plot.getDomainAxis();
        XAxis.setDateFormatOverride(new SimpleDateFormat("HH:mm:ss"));
        XAxis.setAutoRange(true);
        XAxis.setFixedAutoRange(CHART_MOVING_BASE);
        XAxis.setLabelPaint(LABEL_COLOR);
        XAxis.setAxisLinePaint(LABEL_COLOR);
        return chart;
    }

    /**
     * Creates new dataset for server response cha
     *
     * @return
     */
    private XYDataset createChartDataset(){
        chartSeries = new TimeSeries("Response");
        refreshChart();
        dataset = new TimeSeriesCollection();
        dataset.addSeries(chartSeries);
        return dataset;
    }
    /**
     * Adds(updates) data in server response chart. This data are taken from
     * {@link com.smarthome.emulator.simulations.basis.Logger} where they are stored
     */
    private void refreshChart(){
        if(chartSeries == null)
            return;
        ArrayList<Response> responses = logger.getNewResponses();
        if(responses.size()!=0){
            SimpleDateFormat ft =new SimpleDateFormat("hh:mm:ss");
            for(Response response : responses){
                if(response != null){
                    //System.out.println("ADDING RESPONSE TO CHART: " + response.getTime() + ":" + response.getValue());
                    chartSeries.addOrUpdate(new Second(new Date(response.getTime())),response.getValue());
                }
            }
        }
    }

    /**
     * Modifications to GUI before painting the frame
     */
    private void UIModifications() {
        logTabPanel.setUI(new JCustomTabbedPaneUI());
        for (int i = 0; i < logTabPanel.getTabCount(); i++) {
            logTabPanel.setForegroundAt(i, new Color(68, 68, 68));
        }
        configurationScroller.setViewportView(configurationPanel);
        configurationScroller.validate();
        startFromIdTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        adaptersNumberTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        sensorsPerAdapterFromTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        sensorsPerAdapterToTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        refreshRateFromTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        refreshRateToTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        adapterSensorProtocolTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        adapterServerProtocolTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));

        temperatureMaxTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        temperatureMinTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        temperatureDevTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        temperatureAvgTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));

        humidityMaxTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        humidityMinTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        humidityAvgTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        humidityDevTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));

        pressureMaxTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        pressureMinTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        pressureDevTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        pressureAvgTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));

        lightIntensityMaxTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        lightIntensityMinTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        lightIntensityDevTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        lightIntensityAvgTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));

        noiseIntensityMaxTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        noiseIntensityMinTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        noiseIntensityDevTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        noiseIntensityAvgTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));

        emissionsMaxTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        emissionsMinTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        emissionsDevTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        emissionsAvgTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));

        portTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        addressTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));

        simulationLogScroller.getVerticalScrollBar().setUI(new JIOTScrollBarUI());
        toBeSentScroller.getVerticalScrollBar().setUI(new JIOTScrollBarUI());
        errorLogScroller.getVerticalScrollBar().setUI(new JIOTScrollBarUI());
        configurationScroller.getVerticalScrollBar().setUI(new JIOTScrollBarUI());
    }
    /**
     * Creating own components before Intellij Designer generates the rest of components
     */
    private void createUIComponents() {
        // TODO: place custom component creation code here
        startFromIdTextField = new JNumberTextField(4, JNumberTextField.INTEGER_NUMBER,false);
        adaptersNumberTextField = new JNumberTextField(3, JNumberTextField.INTEGER_NUMBER,false);
        sensorsPerAdapterFromTextField = new JNumberTextField(2, JNumberTextField.INTEGER_NUMBER,false);
        sensorsPerAdapterToTextField = new JNumberTextField(2, JNumberTextField.INTEGER_NUMBER,false);
        refreshRateFromTextField = new JNumberTextField(2, JNumberTextField.INTEGER_NUMBER,false);
        refreshRateToTextField = new JNumberTextField(2, JNumberTextField.INTEGER_NUMBER,false);
        adapterSensorProtocolTextField = new JNumberTextField(3, JNumberTextField.INTEGER_NUMBER,false);
        adapterServerProtocolTextField = new JNumberTextField(3, JNumberTextField.INTEGER_NUMBER,false);

        portTextField = new JNumberTextField(5, JNumberTextField.INTEGER_NUMBER,false);

        temperatureMaxTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);
        temperatureMinTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);
        temperatureDevTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);
        temperatureAvgTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);

        humidityMaxTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);
        humidityMinTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);
        humidityDevTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);
        humidityAvgTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);

        pressureMaxTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);
        pressureMinTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);
        pressureDevTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);
        pressureAvgTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);

        lightIntensityMaxTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);
        lightIntensityMinTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);
        lightIntensityDevTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);
        lightIntensityAvgTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);

        noiseIntensityMaxTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);
        noiseIntensityMinTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);
        noiseIntensityDevTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);
        noiseIntensityAvgTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);

        emissionsMaxTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);
        emissionsMinTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);
        emissionsDevTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);
        emissionsAvgTextField = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER,true);
    }

    /**
     * Method generated by IntelliJ IDEA GUI Designer
     * >>> IMPORTANT!! <<<
     * DO NOT edit this method OR call it in your code!
     *
     * @noinspection ALL
     */
    private void $$$setupUI$$$() {
        createUIComponents();
        mainPanel = new JPanel();
        mainPanel.setLayout(new GridLayoutManager(10, 8, new Insets(15, 15, 15, 15), -1, -1));
        mainPanel.setBackground(new Color(-1));
        menuPanel = new JPanel();
        menuPanel.setLayout(new FlowLayout(FlowLayout.TRAILING, 5, 5));
        menuPanel.setBackground(new Color(-1));
        menuPanel.setEnabled(true);
        mainPanel.add(menuPanel, new GridConstraints(0, 5, 1, 3, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_VERTICAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        btnMainSave = new JButton();
        btnMainSave.setBackground(new Color(-2368549));
        btnMainSave.setBorderPainted(false);
        btnMainSave.setFocusPainted(false);
        btnMainSave.setFont(new Font("Segoe UI", Font.BOLD, 14));
        btnMainSave.setForeground(new Color(-8224126));
        btnMainSave.setMargin(new Insets(0, 14, 0, 14));
        btnMainSave.setText("Save");
        menuPanel.add(btnMainSave);
        btnMainLoad = new JButton();
        btnMainLoad.setBackground(new Color(-2368549));
        btnMainLoad.setBorderPainted(false);
        btnMainLoad.setFocusPainted(false);
        btnMainLoad.setFont(new Font("Segoe UI", Font.BOLD, 14));
        btnMainLoad.setForeground(new Color(-8224126));
        btnMainLoad.setMargin(new Insets(0, 14, 0, 14));
        btnMainLoad.setText("Load");
        menuPanel.add(btnMainLoad);
        btnMainPrintLogs = new JButton();
        btnMainPrintLogs.setBackground(new Color(-2368549));
        btnMainPrintLogs.setBorderPainted(false);
        btnMainPrintLogs.setFocusPainted(false);
        btnMainPrintLogs.setFont(new Font("Segoe UI", Font.BOLD, 14));
        btnMainPrintLogs.setForeground(new Color(-8224126));
        btnMainPrintLogs.setMargin(new Insets(0, 14, 0, 14));
        btnMainPrintLogs.setText("Print logs");
        menuPanel.add(btnMainPrintLogs);
        configurationLbl = new JLabel();
        configurationLbl.setBackground(new Color(-1));
        configurationLbl.setFont(new Font("Segoe UI", Font.BOLD, 18));
        configurationLbl.setForeground(new Color(-8224126));
        configurationLbl.setOpaque(true);
        configurationLbl.setText("Configuration");
        mainPanel.add(configurationLbl, new GridConstraints(1, 0, 1, 2, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        performanceSimulationLabel = new JLabel();
        performanceSimulationLabel.setFont(new Font("Segoe UI", Font.PLAIN, 28));
        performanceSimulationLabel.setForeground(new Color(-13127169));
        performanceSimulationLabel.setText("Performance Simulation");
        mainPanel.add(performanceSimulationLabel, new GridConstraints(0, 0, 1, 2, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        configurationScroller = new JScrollPane();
        configurationScroller.setAutoscrolls(false);
        configurationScroller.setBackground(new Color(-1052689));
        configurationScroller.setDoubleBuffered(true);
        configurationScroller.setHorizontalScrollBarPolicy(31);
        configurationScroller.setOpaque(false);
        configurationScroller.setVerticalScrollBarPolicy(20);
        mainPanel.add(configurationScroller, new GridConstraints(2, 0, 5, 2, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_VERTICAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_WANT_GROW, new Dimension(360, 300), new Dimension(360, 300), null, 0, false));
        configurationScroller.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEmptyBorder(), null));
        configurationPanel = new JPanel();
        configurationPanel.setLayout(new FlowLayout(FlowLayout.CENTER, 5, 5));
        configurationPanel.setAutoscrolls(false);
        configurationPanel.setBackground(new Color(-1));
        configurationPanel.setFocusCycleRoot(false);
        configurationPanel.setMaximumSize(new Dimension(360, -1));
        configurationPanel.setMinimumSize(new Dimension(360, -1));
        configurationPanel.setPreferredSize(new Dimension(360, -1));
        configurationPanel.setRequestFocusEnabled(true);
        configurationScroller.setViewportView(configurationPanel);
        configurationPanel.setBorder(BorderFactory.createTitledBorder(BorderFactory.createLineBorder(new Color(-1052689)), ""));
        startSimulationBtn = new JButton();
        startSimulationBtn.setBackground(new Color(-5841665));
        startSimulationBtn.setBorderPainted(false);
        startSimulationBtn.setFocusPainted(false);
        startSimulationBtn.setForeground(new Color(-12303292));
        startSimulationBtn.setHorizontalAlignment(0);
        startSimulationBtn.setMargin(new Insets(2, 0, 2, 0));
        startSimulationBtn.setMaximumSize(new Dimension(130, 24));
        startSimulationBtn.setMinimumSize(new Dimension(130, 24));
        startSimulationBtn.setPreferredSize(new Dimension(130, 24));
        startSimulationBtn.setText("Start Simulation");
        configurationPanel.add(startSimulationBtn);
        stopSimulationBtn = new JButton();
        stopSimulationBtn.setAutoscrolls(false);
        stopSimulationBtn.setBackground(new Color(-3750202));
        stopSimulationBtn.setBorderPainted(false);
        stopSimulationBtn.setContentAreaFilled(true);
        stopSimulationBtn.setFocusPainted(false);
        stopSimulationBtn.setForeground(new Color(-1));
        stopSimulationBtn.setHorizontalAlignment(0);
        stopSimulationBtn.setMargin(new Insets(2, 0, 2, 0));
        stopSimulationBtn.setMaximumSize(new Dimension(130, 24));
        stopSimulationBtn.setMinimumSize(new Dimension(130, 24));
        stopSimulationBtn.setPreferredSize(new Dimension(130, 24));
        stopSimulationBtn.setText("Stop Simulation");
        configurationPanel.add(stopSimulationBtn);
        resumeSimulationBtn = new JButton();
        resumeSimulationBtn.setAutoscrolls(false);
        resumeSimulationBtn.setBackground(new Color(-3750202));
        resumeSimulationBtn.setBorderPainted(false);
        resumeSimulationBtn.setContentAreaFilled(true);
        resumeSimulationBtn.setFocusPainted(false);
        resumeSimulationBtn.setForeground(new Color(-1));
        resumeSimulationBtn.setHorizontalAlignment(0);
        resumeSimulationBtn.setLabel("Resume Simulation");
        resumeSimulationBtn.setMargin(new Insets(2, 0, 2, 0));
        resumeSimulationBtn.setMaximumSize(new Dimension(130, 24));
        resumeSimulationBtn.setMinimumSize(new Dimension(130, 24));
        resumeSimulationBtn.setPreferredSize(new Dimension(130, 24));
        resumeSimulationBtn.setText("Resume Simulation");
        configurationPanel.add(resumeSimulationBtn);
        pauseSimulationBtn = new JButton();
        pauseSimulationBtn.setAutoscrolls(false);
        pauseSimulationBtn.setBackground(new Color(-3750202));
        pauseSimulationBtn.setBorderPainted(false);
        pauseSimulationBtn.setContentAreaFilled(true);
        pauseSimulationBtn.setFocusPainted(false);
        pauseSimulationBtn.setForeground(new Color(-1));
        pauseSimulationBtn.setHorizontalAlignment(0);
        pauseSimulationBtn.setMargin(new Insets(2, 0, 2, 0));
        pauseSimulationBtn.setMaximumSize(new Dimension(130, 24));
        pauseSimulationBtn.setMinimumSize(new Dimension(130, 24));
        pauseSimulationBtn.setPreferredSize(new Dimension(130, 24));
        pauseSimulationBtn.setText("Pause Simulation");
        configurationPanel.add(pauseSimulationBtn);
        resumeSensorsBtn = new JButton();
        resumeSensorsBtn.setAutoscrolls(false);
        resumeSensorsBtn.setBackground(new Color(-3750202));
        resumeSensorsBtn.setBorderPainted(false);
        resumeSensorsBtn.setContentAreaFilled(true);
        resumeSensorsBtn.setFocusPainted(false);
        resumeSensorsBtn.setForeground(new Color(-1));
        resumeSensorsBtn.setHorizontalAlignment(0);
        resumeSensorsBtn.setLabel("Resume Sensors");
        resumeSensorsBtn.setMargin(new Insets(2, 0, 2, 0));
        resumeSensorsBtn.setMaximumSize(new Dimension(130, 24));
        resumeSensorsBtn.setMinimumSize(new Dimension(130, 24));
        resumeSensorsBtn.setPreferredSize(new Dimension(130, 24));
        resumeSensorsBtn.setText("Resume Sensors");
        configurationPanel.add(resumeSensorsBtn);
        pauseSensorsBtn = new JButton();
        pauseSensorsBtn.setAutoscrolls(false);
        pauseSensorsBtn.setBackground(new Color(-3750202));
        pauseSensorsBtn.setBorderPainted(false);
        pauseSensorsBtn.setContentAreaFilled(true);
        pauseSensorsBtn.setFocusPainted(false);
        pauseSensorsBtn.setForeground(new Color(-1));
        pauseSensorsBtn.setHorizontalAlignment(0);
        pauseSensorsBtn.setLabel("Pause Sensors");
        pauseSensorsBtn.setMargin(new Insets(2, 0, 2, 0));
        pauseSensorsBtn.setMaximumSize(new Dimension(130, 24));
        pauseSensorsBtn.setMinimumSize(new Dimension(130, 24));
        pauseSensorsBtn.setPreferredSize(new Dimension(130, 24));
        pauseSensorsBtn.setText("Pause Sensors");
        configurationPanel.add(pauseSensorsBtn);
        warningPanel = new JPanel();
        warningPanel.setLayout(new GridLayoutManager(1, 1, new Insets(0, 0, 0, 0), -1, -1));
        warningPanel.setBackground(new Color(-1));
        warningPanel.setMinimumSize(new Dimension(342, 48));
        warningPanel.setPreferredSize(new Dimension(342, 48));
        warningPanel.setVisible(false);
        configurationPanel.add(warningPanel);
        warningLbl = new JLabel();
        warningLbl.setBackground(new Color(-1));
        warningLbl.setFont(new Font("Segoe UI", Font.BOLD, 11));
        warningLbl.setForeground(new Color(-52429));
        warningLbl.setText("");
        warningPanel.add(warningLbl, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        confPanel = new JPanel();
        confPanel.setLayout(new GridLayoutManager(6, 7, new Insets(0, 0, 0, 0), -1, -1));
        confPanel.setBackground(new Color(-1));
        configurationPanel.add(confPanel);
        adaptersLbl = new JLabel();
        adaptersLbl.setBackground(new Color(-1));
        adaptersLbl.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        adaptersLbl.setForeground(new Color(-8224126));
        adaptersLbl.setText("Adapters count:");
        adaptersLbl.setVerticalAlignment(1);
        confPanel.add(adaptersLbl, new GridConstraints(0, 1, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        adaptersNumberTextField.setBackground(new Color(-1));
        adaptersNumberTextField.setCaretColor(new Color(-8224126));
        adaptersNumberTextField.setDisabledTextColor(new Color(-8224126));
        adaptersNumberTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        adaptersNumberTextField.setForeground(new Color(-8224126));
        adaptersNumberTextField.setSelectionColor(new Color(-5841665));
        confPanel.add(adaptersNumberTextField, new GridConstraints(0, 2, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        final JLabel label1 = new JLabel();
        label1.setBackground(new Color(-1));
        label1.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label1.setForeground(new Color(-8224126));
        label1.setText("Adapter->Server protocol:");
        label1.setToolTipText("");
        label1.setVerticalAlignment(1);
        confPanel.add(label1, new GridConstraints(2, 1, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        adapterServerProtocolTextField.setBackground(new Color(-1));
        adapterServerProtocolTextField.setCaretColor(new Color(-8224126));
        adapterServerProtocolTextField.setDisabledTextColor(new Color(-8224126));
        adapterServerProtocolTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        adapterServerProtocolTextField.setForeground(new Color(-8224126));
        adapterServerProtocolTextField.setSelectionColor(new Color(-5841665));
        adapterServerProtocolTextField.setText("");
        adapterServerProtocolTextField.setToolTipText("Only one or double digit number.");
        confPanel.add(adapterServerProtocolTextField, new GridConstraints(2, 2, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        final JLabel label2 = new JLabel();
        label2.setBackground(new Color(-1));
        label2.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label2.setForeground(new Color(-8224126));
        label2.setText("Adapter->Sensor protocol:");
        label2.setVerticalAlignment(1);
        confPanel.add(label2, new GridConstraints(1, 1, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        adapterSensorProtocolTextField.setBackground(new Color(-1));
        adapterSensorProtocolTextField.setCaretColor(new Color(-8224126));
        adapterSensorProtocolTextField.setDisabledTextColor(new Color(-8224126));
        adapterSensorProtocolTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        adapterSensorProtocolTextField.setForeground(new Color(-8224126));
        adapterSensorProtocolTextField.setSelectionColor(new Color(-5841665));
        adapterSensorProtocolTextField.setText("");
        adapterSensorProtocolTextField.setToolTipText("Only one or double digit number.");
        confPanel.add(adapterSensorProtocolTextField, new GridConstraints(1, 2, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        senPerAdaLbl = new JLabel();
        senPerAdaLbl.setBackground(new Color(-1));
        senPerAdaLbl.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        senPerAdaLbl.setForeground(new Color(-8224126));
        senPerAdaLbl.setText("Sensors per adapter:");
        senPerAdaLbl.setVerticalAlignment(1);
        confPanel.add(senPerAdaLbl, new GridConstraints(4, 1, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JLabel label3 = new JLabel();
        label3.setBackground(new Color(-1));
        label3.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label3.setForeground(new Color(-8224126));
        label3.setText("from");
        label3.setVerticalAlignment(1);
        confPanel.add(label3, new GridConstraints(4, 2, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        sensorsPerAdapterFromTextField.setBackground(new Color(-1));
        sensorsPerAdapterFromTextField.setCaretColor(new Color(-8224126));
        sensorsPerAdapterFromTextField.setDisabledTextColor(new Color(-8224126));
        sensorsPerAdapterFromTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        sensorsPerAdapterFromTextField.setForeground(new Color(-8224126));
        sensorsPerAdapterFromTextField.setSelectionColor(new Color(-5841665));
        sensorsPerAdapterFromTextField.setText("");
        sensorsPerAdapterFromTextField.setToolTipText("Only one or double digit number.");
        confPanel.add(sensorsPerAdapterFromTextField, new GridConstraints(4, 3, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        final JLabel label4 = new JLabel();
        label4.setBackground(new Color(-1));
        label4.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label4.setForeground(new Color(-8224126));
        label4.setText("to");
        label4.setVerticalAlignment(1);
        confPanel.add(label4, new GridConstraints(4, 4, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        sensorsPerAdapterToTextField.setBackground(new Color(-1));
        sensorsPerAdapterToTextField.setCaretColor(new Color(-8224126));
        sensorsPerAdapterToTextField.setDisabledTextColor(new Color(-8224126));
        sensorsPerAdapterToTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        sensorsPerAdapterToTextField.setForeground(new Color(-8224126));
        sensorsPerAdapterToTextField.setSelectionColor(new Color(-5841665));
        sensorsPerAdapterToTextField.setText("");
        confPanel.add(sensorsPerAdapterToTextField, new GridConstraints(4, 5, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        final Spacer spacer1 = new Spacer();
        confPanel.add(spacer1, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_WANT_GROW, 1, null, null, null, 0, false));
        final Spacer spacer2 = new Spacer();
        confPanel.add(spacer2, new GridConstraints(4, 6, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_WANT_GROW, 1, null, null, null, 0, false));
        final JLabel label5 = new JLabel();
        label5.setBackground(new Color(-1));
        label5.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label5.setForeground(new Color(-8224126));
        label5.setText("Refresh rate:");
        label5.setVerticalAlignment(1);
        confPanel.add(label5, new GridConstraints(5, 1, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JLabel label6 = new JLabel();
        label6.setBackground(new Color(-1));
        label6.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label6.setForeground(new Color(-8224126));
        label6.setText("from");
        label6.setVerticalAlignment(1);
        confPanel.add(label6, new GridConstraints(5, 2, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        refreshRateFromTextField.setBackground(new Color(-1));
        refreshRateFromTextField.setCaretColor(new Color(-8224126));
        refreshRateFromTextField.setDisabledTextColor(new Color(-8224126));
        refreshRateFromTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        refreshRateFromTextField.setForeground(new Color(-8224126));
        refreshRateFromTextField.setSelectionColor(new Color(-5841665));
        refreshRateFromTextField.setText("");
        refreshRateFromTextField.setToolTipText("Only one or double digit number.");
        confPanel.add(refreshRateFromTextField, new GridConstraints(5, 3, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        final JLabel label7 = new JLabel();
        label7.setBackground(new Color(-1));
        label7.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label7.setForeground(new Color(-8224126));
        label7.setText("to");
        label7.setVerticalAlignment(1);
        confPanel.add(label7, new GridConstraints(5, 4, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        refreshRateToTextField.setBackground(new Color(-1));
        refreshRateToTextField.setCaretColor(new Color(-8224126));
        refreshRateToTextField.setDisabledTextColor(new Color(-8224126));
        refreshRateToTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        refreshRateToTextField.setForeground(new Color(-8224126));
        refreshRateToTextField.setSelectionColor(new Color(-5841665));
        refreshRateToTextField.setText("");
        confPanel.add(refreshRateToTextField, new GridConstraints(5, 5, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        startFromIdTextField.setBackground(new Color(-1));
        startFromIdTextField.setCaretColor(new Color(-8224126));
        startFromIdTextField.setDisabledTextColor(new Color(-8224126));
        startFromIdTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        startFromIdTextField.setForeground(new Color(-8224126));
        startFromIdTextField.setSelectionColor(new Color(-5841665));
        startFromIdTextField.setText("");
        confPanel.add(startFromIdTextField, new GridConstraints(3, 2, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(40, -1), new Dimension(40, -1), new Dimension(40, -1), 0, false));
        final JLabel label8 = new JLabel();
        label8.setBackground(new Color(-1));
        label8.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label8.setForeground(new Color(-8224126));
        label8.setText("Start from Id:");
        label8.setVerticalAlignment(1);
        confPanel.add(label8, new GridConstraints(3, 1, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JLabel label9 = new JLabel();
        label9.setBackground(new Color(-1));
        label9.setFont(new Font("Segoe UI", Font.BOLD, 14));
        label9.setForeground(new Color(-8224126));
        label9.setOpaque(true);
        label9.setText("Server information");
        configurationPanel.add(label9);
        final JPanel panel1 = new JPanel();
        panel1.setLayout(new GridLayoutManager(3, 5, new Insets(0, 0, 0, 0), -1, -1));
        panel1.setBackground(new Color(-1));
        panel1.setMinimumSize(new Dimension(346, 70));
        panel1.setPreferredSize(new Dimension(346, 70));
        configurationPanel.add(panel1);
        final JLabel label10 = new JLabel();
        label10.setBackground(new Color(-1));
        label10.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label10.setForeground(new Color(-8224126));
        label10.setText("Address:");
        label10.setVerticalAlignment(1);
        panel1.add(label10, new GridConstraints(0, 1, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        hostRadBtn = new JRadioButton();
        hostRadBtn.setBackground(new Color(-1));
        hostRadBtn.setForeground(new Color(-8224126));
        hostRadBtn.setIcon(new ImageIcon(getClass().getResource("/img/radioButtonNotSelected.png")));
        hostRadBtn.setPressedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        hostRadBtn.setSelected(true);
        hostRadBtn.setSelectedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        hostRadBtn.setText("Host");
        panel1.add(hostRadBtn, new GridConstraints(0, 2, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(-1, 18), new Dimension(-1, 18), new Dimension(-1, 18), 0, false));
        ipRadBtn = new JRadioButton();
        ipRadBtn.setBackground(new Color(-1));
        ipRadBtn.setForeground(new Color(-8224126));
        ipRadBtn.setIcon(new ImageIcon(getClass().getResource("/img/radioButtonNotSelected.png")));
        ipRadBtn.setPressedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        ipRadBtn.setSelectedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        ipRadBtn.setText("IP");
        panel1.add(ipRadBtn, new GridConstraints(0, 3, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_FIXED, new Dimension(-1, 18), new Dimension(-1, 18), new Dimension(-1, 18), 0, false));
        final Spacer spacer3 = new Spacer();
        panel1.add(spacer3, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_WANT_GROW, 1, null, null, null, 0, false));
        addressTextField = new JTextField();
        addressTextField.setBackground(new Color(-1));
        addressTextField.setCaretColor(new Color(-8224126));
        addressTextField.setForeground(new Color(-8224126));
        addressTextField.setSelectionColor(new Color(-5841665));
        panel1.add(addressTextField, new GridConstraints(1, 2, 1, 2, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(150, -1), null, 0, false));
        final JLabel label11 = new JLabel();
        label11.setBackground(new Color(-1));
        label11.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label11.setForeground(new Color(-8224126));
        label11.setText("Port:");
        label11.setVerticalAlignment(1);
        panel1.add(label11, new GridConstraints(2, 1, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        portTextField.setBackground(new Color(-1));
        portTextField.setCaretColor(new Color(-8224126));
        portTextField.setDisabledTextColor(new Color(-8224126));
        portTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        portTextField.setForeground(new Color(-8224126));
        portTextField.setSelectionColor(new Color(-5841665));
        portTextField.setText("");
        panel1.add(portTextField, new GridConstraints(2, 2, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(40, -1), new Dimension(40, -1), new Dimension(40, -1), 0, false));
        final Spacer spacer4 = new Spacer();
        panel1.add(spacer4, new GridConstraints(1, 4, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_WANT_GROW, 1, null, null, null, 0, false));
        sensorValuesLbl = new JLabel();
        sensorValuesLbl.setBackground(new Color(-1));
        sensorValuesLbl.setFont(new Font("Segoe UI", Font.BOLD, 14));
        sensorValuesLbl.setForeground(new Color(-8224126));
        sensorValuesLbl.setOpaque(true);
        sensorValuesLbl.setText("Sensor's values");
        configurationPanel.add(sensorValuesLbl);
        sensorValuesPanel = new JPanel();
        sensorValuesPanel.setLayout(new GridLayoutManager(13, 8, new Insets(0, 0, 0, 0), -1, -1));
        sensorValuesPanel.setBackground(new Color(-1));
        configurationPanel.add(sensorValuesPanel);
        final JLabel label12 = new JLabel();
        label12.setBackground(new Color(-1));
        label12.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label12.setForeground(new Color(-8224126));
        label12.setText("Temperature value:");
        label12.setVerticalAlignment(1);
        sensorValuesPanel.add(label12, new GridConstraints(0, 1, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JLabel label13 = new JLabel();
        label13.setBackground(new Color(-1));
        label13.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label13.setForeground(new Color(-8224126));
        label13.setText("dev");
        label13.setToolTipText("Deviation");
        label13.setVerticalAlignment(1);
        sensorValuesPanel.add(label13, new GridConstraints(1, 3, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JLabel label14 = new JLabel();
        label14.setBackground(new Color(-1));
        label14.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label14.setForeground(new Color(-8224126));
        label14.setText("avg");
        label14.setToolTipText("Average");
        label14.setVerticalAlignment(1);
        sensorValuesPanel.add(label14, new GridConstraints(1, 5, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        temperatureDevTextField.setBackground(new Color(-1));
        temperatureDevTextField.setCaretColor(new Color(-8224126));
        temperatureDevTextField.setDisabledTextColor(new Color(-8224126));
        temperatureDevTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        temperatureDevTextField.setForeground(new Color(-8224126));
        temperatureDevTextField.setSelectionColor(new Color(-5841665));
        temperatureDevTextField.setText("");
        temperatureDevTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(temperatureDevTextField, new GridConstraints(1, 4, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        temperatureAvgTextField.setBackground(new Color(-1));
        temperatureAvgTextField.setCaretColor(new Color(-8224126));
        temperatureAvgTextField.setDisabledTextColor(new Color(-8224126));
        temperatureAvgTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        temperatureAvgTextField.setForeground(new Color(-8224126));
        temperatureAvgTextField.setSelectionColor(new Color(-5841665));
        temperatureAvgTextField.setText("");
        temperatureAvgTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(temperatureAvgTextField, new GridConstraints(1, 6, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        final JLabel label15 = new JLabel();
        label15.setBackground(new Color(-1));
        label15.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label15.setForeground(new Color(-8224126));
        label15.setText("max");
        label15.setToolTipText("Maximum value");
        label15.setVerticalAlignment(1);
        sensorValuesPanel.add(label15, new GridConstraints(0, 3, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        temperatureMaxTextField.setBackground(new Color(-1));
        temperatureMaxTextField.setCaretColor(new Color(-8224126));
        temperatureMaxTextField.setDisabledTextColor(new Color(-8224126));
        temperatureMaxTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        temperatureMaxTextField.setForeground(new Color(-8224126));
        temperatureMaxTextField.setSelectionColor(new Color(-5841665));
        temperatureMaxTextField.setText("");
        temperatureMaxTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(temperatureMaxTextField, new GridConstraints(0, 4, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        final JLabel label16 = new JLabel();
        label16.setBackground(new Color(-1));
        label16.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label16.setForeground(new Color(-8224126));
        label16.setText("min");
        label16.setToolTipText("Minimum value");
        label16.setVerticalAlignment(1);
        sensorValuesPanel.add(label16, new GridConstraints(0, 5, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        temperatureMinTextField.setBackground(new Color(-1));
        temperatureMinTextField.setCaretColor(new Color(-8224126));
        temperatureMinTextField.setDisabledTextColor(new Color(-8224126));
        temperatureMinTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        temperatureMinTextField.setForeground(new Color(-8224126));
        temperatureMinTextField.setSelectionColor(new Color(-5841665));
        temperatureMinTextField.setText("");
        temperatureMinTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(temperatureMinTextField, new GridConstraints(0, 6, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        final JLabel label17 = new JLabel();
        label17.setBackground(new Color(-1));
        label17.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label17.setForeground(new Color(-8224126));
        label17.setText("Humidity value:");
        label17.setVerticalAlignment(1);
        sensorValuesPanel.add(label17, new GridConstraints(2, 1, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JLabel label18 = new JLabel();
        label18.setBackground(new Color(-1));
        label18.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label18.setForeground(new Color(-8224126));
        label18.setText("max");
        label18.setToolTipText("Maximum value");
        label18.setVerticalAlignment(1);
        sensorValuesPanel.add(label18, new GridConstraints(2, 3, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JLabel label19 = new JLabel();
        label19.setBackground(new Color(-1));
        label19.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label19.setForeground(new Color(-8224126));
        label19.setText("min");
        label19.setToolTipText("Minimum value");
        label19.setVerticalAlignment(1);
        sensorValuesPanel.add(label19, new GridConstraints(2, 5, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JLabel label20 = new JLabel();
        label20.setBackground(new Color(-1));
        label20.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label20.setForeground(new Color(-8224126));
        label20.setText("dev");
        label20.setToolTipText("Deviation");
        label20.setVerticalAlignment(1);
        sensorValuesPanel.add(label20, new GridConstraints(3, 3, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JLabel label21 = new JLabel();
        label21.setBackground(new Color(-1));
        label21.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label21.setForeground(new Color(-8224126));
        label21.setText("avg");
        label21.setToolTipText("Average");
        label21.setVerticalAlignment(1);
        sensorValuesPanel.add(label21, new GridConstraints(3, 5, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        humidityMaxTextField.setBackground(new Color(-1));
        humidityMaxTextField.setCaretColor(new Color(-8224126));
        humidityMaxTextField.setDisabledTextColor(new Color(-8224126));
        humidityMaxTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        humidityMaxTextField.setForeground(new Color(-8224126));
        humidityMaxTextField.setSelectionColor(new Color(-5841665));
        humidityMaxTextField.setText("");
        humidityMaxTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(humidityMaxTextField, new GridConstraints(2, 4, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        humidityMinTextField.setBackground(new Color(-1));
        humidityMinTextField.setCaretColor(new Color(-8224126));
        humidityMinTextField.setDisabledTextColor(new Color(-8224126));
        humidityMinTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        humidityMinTextField.setForeground(new Color(-8224126));
        humidityMinTextField.setSelectionColor(new Color(-5841665));
        humidityMinTextField.setText("");
        humidityMinTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(humidityMinTextField, new GridConstraints(2, 6, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        humidityDevTextField.setBackground(new Color(-1));
        humidityDevTextField.setCaretColor(new Color(-8224126));
        humidityDevTextField.setDisabledTextColor(new Color(-8224126));
        humidityDevTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        humidityDevTextField.setForeground(new Color(-8224126));
        humidityDevTextField.setSelectionColor(new Color(-5841665));
        humidityDevTextField.setText("");
        humidityDevTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(humidityDevTextField, new GridConstraints(3, 4, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        humidityAvgTextField.setBackground(new Color(-1));
        humidityAvgTextField.setCaretColor(new Color(-8224126));
        humidityAvgTextField.setDisabledTextColor(new Color(-8224126));
        humidityAvgTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        humidityAvgTextField.setForeground(new Color(-8224126));
        humidityAvgTextField.setSelectionColor(new Color(-5841665));
        humidityAvgTextField.setText("");
        humidityAvgTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(humidityAvgTextField, new GridConstraints(3, 6, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        final JLabel label22 = new JLabel();
        label22.setBackground(new Color(-1));
        label22.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label22.setForeground(new Color(-8224126));
        label22.setRequestFocusEnabled(false);
        label22.setText("Pressure value:");
        label22.setVerticalAlignment(1);
        sensorValuesPanel.add(label22, new GridConstraints(4, 1, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JLabel label23 = new JLabel();
        label23.setBackground(new Color(-1));
        label23.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label23.setForeground(new Color(-8224126));
        label23.setText("max");
        label23.setToolTipText("Maximum value");
        label23.setVerticalAlignment(1);
        sensorValuesPanel.add(label23, new GridConstraints(4, 3, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JLabel label24 = new JLabel();
        label24.setBackground(new Color(-1));
        label24.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label24.setForeground(new Color(-8224126));
        label24.setText("min");
        label24.setToolTipText("Minimum value");
        label24.setVerticalAlignment(1);
        sensorValuesPanel.add(label24, new GridConstraints(4, 5, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JLabel label25 = new JLabel();
        label25.setBackground(new Color(-1));
        label25.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label25.setForeground(new Color(-8224126));
        label25.setText("dev");
        label25.setToolTipText("Deviation");
        label25.setVerticalAlignment(1);
        sensorValuesPanel.add(label25, new GridConstraints(5, 3, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JLabel label26 = new JLabel();
        label26.setBackground(new Color(-1));
        label26.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label26.setForeground(new Color(-8224126));
        label26.setText("avg");
        label26.setToolTipText("Average");
        label26.setVerticalAlignment(1);
        sensorValuesPanel.add(label26, new GridConstraints(5, 5, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        pressureMaxTextField.setBackground(new Color(-1));
        pressureMaxTextField.setCaretColor(new Color(-8224126));
        pressureMaxTextField.setDisabledTextColor(new Color(-8224126));
        pressureMaxTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        pressureMaxTextField.setForeground(new Color(-8224126));
        pressureMaxTextField.setSelectionColor(new Color(-5841665));
        pressureMaxTextField.setText("");
        pressureMaxTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(pressureMaxTextField, new GridConstraints(4, 4, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        pressureMinTextField.setBackground(new Color(-1));
        pressureMinTextField.setCaretColor(new Color(-8224126));
        pressureMinTextField.setDisabledTextColor(new Color(-8224126));
        pressureMinTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        pressureMinTextField.setForeground(new Color(-8224126));
        pressureMinTextField.setSelectionColor(new Color(-5841665));
        pressureMinTextField.setText("");
        pressureMinTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(pressureMinTextField, new GridConstraints(4, 6, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        pressureDevTextField.setBackground(new Color(-1));
        pressureDevTextField.setCaretColor(new Color(-8224126));
        pressureDevTextField.setDisabledTextColor(new Color(-8224126));
        pressureDevTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        pressureDevTextField.setForeground(new Color(-8224126));
        pressureDevTextField.setSelectionColor(new Color(-5841665));
        pressureDevTextField.setText("");
        pressureDevTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(pressureDevTextField, new GridConstraints(5, 4, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        pressureAvgTextField.setBackground(new Color(-1));
        pressureAvgTextField.setCaretColor(new Color(-8224126));
        pressureAvgTextField.setDisabledTextColor(new Color(-8224126));
        pressureAvgTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        pressureAvgTextField.setForeground(new Color(-8224126));
        pressureAvgTextField.setSelectionColor(new Color(-5841665));
        pressureAvgTextField.setText("");
        pressureAvgTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(pressureAvgTextField, new GridConstraints(5, 6, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        temperatureCheckBox = new JCheckBox();
        temperatureCheckBox.setBackground(new Color(-1));
        temperatureCheckBox.setForeground(new Color(-8224126));
        temperatureCheckBox.setIcon(new ImageIcon(getClass().getResource("/img/checkboxNotChecked.png")));
        temperatureCheckBox.setLabel("");
        temperatureCheckBox.setPressedIcon(new ImageIcon(getClass().getResource("/img/checkboxChecked.png")));
        temperatureCheckBox.setSelected(true);
        temperatureCheckBox.setSelectedIcon(new ImageIcon(getClass().getResource("/img/checkboxChecked.png")));
        temperatureCheckBox.setText("");
        sensorValuesPanel.add(temperatureCheckBox, new GridConstraints(0, 2, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        humidityCheckBox = new JCheckBox();
        humidityCheckBox.setBackground(new Color(-1));
        humidityCheckBox.setForeground(new Color(-8224126));
        humidityCheckBox.setIcon(new ImageIcon(getClass().getResource("/img/checkboxNotChecked.png")));
        humidityCheckBox.setLabel("");
        humidityCheckBox.setPressedIcon(new ImageIcon(getClass().getResource("/img/checkboxChecked.png")));
        humidityCheckBox.setSelected(true);
        humidityCheckBox.setSelectedIcon(new ImageIcon(getClass().getResource("/img/checkboxChecked.png")));
        humidityCheckBox.setText("");
        sensorValuesPanel.add(humidityCheckBox, new GridConstraints(2, 2, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        pressureCheckBox = new JCheckBox();
        pressureCheckBox.setBackground(new Color(-1));
        pressureCheckBox.setForeground(new Color(-8224126));
        pressureCheckBox.setIcon(new ImageIcon(getClass().getResource("/img/checkboxNotChecked.png")));
        pressureCheckBox.setLabel("");
        pressureCheckBox.setPressedIcon(new ImageIcon(getClass().getResource("/img/checkboxChecked.png")));
        pressureCheckBox.setSelected(false);
        pressureCheckBox.setSelectedIcon(new ImageIcon(getClass().getResource("/img/checkboxChecked.png")));
        pressureCheckBox.setText("");
        sensorValuesPanel.add(pressureCheckBox, new GridConstraints(4, 2, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JLabel label27 = new JLabel();
        label27.setBackground(new Color(-1));
        label27.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label27.setForeground(new Color(-8224126));
        label27.setRequestFocusEnabled(false);
        label27.setText("Light intesity value:");
        label27.setVerticalAlignment(1);
        sensorValuesPanel.add(label27, new GridConstraints(7, 1, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        lightIntensityCheckBox = new JCheckBox();
        lightIntensityCheckBox.setBackground(new Color(-1));
        lightIntensityCheckBox.setForeground(new Color(-8224126));
        lightIntensityCheckBox.setIcon(new ImageIcon(getClass().getResource("/img/checkboxNotChecked.png")));
        lightIntensityCheckBox.setLabel("");
        lightIntensityCheckBox.setPressedIcon(new ImageIcon(getClass().getResource("/img/checkboxChecked.png")));
        lightIntensityCheckBox.setSelected(false);
        lightIntensityCheckBox.setSelectedIcon(new ImageIcon(getClass().getResource("/img/checkboxChecked.png")));
        lightIntensityCheckBox.setText("");
        sensorValuesPanel.add(lightIntensityCheckBox, new GridConstraints(7, 2, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JLabel label28 = new JLabel();
        label28.setBackground(new Color(-1));
        label28.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label28.setForeground(new Color(-8224126));
        label28.setText("max");
        label28.setToolTipText("Maximum value");
        label28.setVerticalAlignment(1);
        sensorValuesPanel.add(label28, new GridConstraints(7, 3, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JLabel label29 = new JLabel();
        label29.setBackground(new Color(-1));
        label29.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label29.setForeground(new Color(-8224126));
        label29.setText("min");
        label29.setToolTipText("Minimum value");
        label29.setVerticalAlignment(1);
        sensorValuesPanel.add(label29, new GridConstraints(7, 5, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        lightIntensityMaxTextField.setBackground(new Color(-1));
        lightIntensityMaxTextField.setCaretColor(new Color(-8224126));
        lightIntensityMaxTextField.setDisabledTextColor(new Color(-8224126));
        lightIntensityMaxTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        lightIntensityMaxTextField.setForeground(new Color(-8224126));
        lightIntensityMaxTextField.setSelectionColor(new Color(-5841665));
        lightIntensityMaxTextField.setText("");
        lightIntensityMaxTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(lightIntensityMaxTextField, new GridConstraints(7, 4, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        lightIntensityMinTextField.setBackground(new Color(-1));
        lightIntensityMinTextField.setCaretColor(new Color(-8224126));
        lightIntensityMinTextField.setDisabledTextColor(new Color(-8224126));
        lightIntensityMinTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        lightIntensityMinTextField.setForeground(new Color(-8224126));
        lightIntensityMinTextField.setSelectionColor(new Color(-5841665));
        lightIntensityMinTextField.setText("");
        lightIntensityMinTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(lightIntensityMinTextField, new GridConstraints(7, 6, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        final JLabel label30 = new JLabel();
        label30.setBackground(new Color(-1));
        label30.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label30.setForeground(new Color(-8224126));
        label30.setText("dev");
        label30.setToolTipText("Deviation");
        label30.setVerticalAlignment(1);
        sensorValuesPanel.add(label30, new GridConstraints(8, 3, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        lightIntensityDevTextField.setBackground(new Color(-1));
        lightIntensityDevTextField.setCaretColor(new Color(-8224126));
        lightIntensityDevTextField.setDisabledTextColor(new Color(-8224126));
        lightIntensityDevTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        lightIntensityDevTextField.setForeground(new Color(-8224126));
        lightIntensityDevTextField.setSelectionColor(new Color(-5841665));
        lightIntensityDevTextField.setText("");
        lightIntensityDevTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(lightIntensityDevTextField, new GridConstraints(8, 4, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        final JLabel label31 = new JLabel();
        label31.setBackground(new Color(-1));
        label31.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label31.setForeground(new Color(-8224126));
        label31.setText("avg");
        label31.setToolTipText("Average");
        label31.setVerticalAlignment(1);
        sensorValuesPanel.add(label31, new GridConstraints(8, 5, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        lightIntensityAvgTextField.setBackground(new Color(-1));
        lightIntensityAvgTextField.setCaretColor(new Color(-8224126));
        lightIntensityAvgTextField.setDisabledTextColor(new Color(-8224126));
        lightIntensityAvgTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        lightIntensityAvgTextField.setForeground(new Color(-8224126));
        lightIntensityAvgTextField.setSelectionColor(new Color(-5841665));
        lightIntensityAvgTextField.setText("");
        lightIntensityAvgTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(lightIntensityAvgTextField, new GridConstraints(8, 6, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        final JLabel label32 = new JLabel();
        label32.setBackground(new Color(-1));
        label32.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label32.setForeground(new Color(-8224126));
        label32.setRequestFocusEnabled(false);
        label32.setText("Noise intesity value:");
        label32.setVerticalAlignment(1);
        sensorValuesPanel.add(label32, new GridConstraints(9, 1, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        noiseIntensityCheckBox = new JCheckBox();
        noiseIntensityCheckBox.setBackground(new Color(-1));
        noiseIntensityCheckBox.setForeground(new Color(-8224126));
        noiseIntensityCheckBox.setIcon(new ImageIcon(getClass().getResource("/img/checkboxNotChecked.png")));
        noiseIntensityCheckBox.setLabel("");
        noiseIntensityCheckBox.setPressedIcon(new ImageIcon(getClass().getResource("/img/checkboxChecked.png")));
        noiseIntensityCheckBox.setSelected(false);
        noiseIntensityCheckBox.setSelectedIcon(new ImageIcon(getClass().getResource("/img/checkboxChecked.png")));
        noiseIntensityCheckBox.setText("");
        sensorValuesPanel.add(noiseIntensityCheckBox, new GridConstraints(9, 2, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JLabel label33 = new JLabel();
        label33.setBackground(new Color(-1));
        label33.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label33.setForeground(new Color(-8224126));
        label33.setText("max");
        label33.setToolTipText("Maximum value");
        label33.setVerticalAlignment(1);
        sensorValuesPanel.add(label33, new GridConstraints(9, 3, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JLabel label34 = new JLabel();
        label34.setBackground(new Color(-1));
        label34.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label34.setForeground(new Color(-8224126));
        label34.setText("min");
        label34.setToolTipText("Minimum value");
        label34.setVerticalAlignment(1);
        sensorValuesPanel.add(label34, new GridConstraints(9, 5, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        noiseIntensityMaxTextField.setBackground(new Color(-1));
        noiseIntensityMaxTextField.setCaretColor(new Color(-8224126));
        noiseIntensityMaxTextField.setDisabledTextColor(new Color(-8224126));
        noiseIntensityMaxTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        noiseIntensityMaxTextField.setForeground(new Color(-8224126));
        noiseIntensityMaxTextField.setSelectionColor(new Color(-5841665));
        noiseIntensityMaxTextField.setText("");
        noiseIntensityMaxTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(noiseIntensityMaxTextField, new GridConstraints(9, 4, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        noiseIntensityMinTextField.setBackground(new Color(-1));
        noiseIntensityMinTextField.setCaretColor(new Color(-8224126));
        noiseIntensityMinTextField.setDisabledTextColor(new Color(-8224126));
        noiseIntensityMinTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        noiseIntensityMinTextField.setForeground(new Color(-8224126));
        noiseIntensityMinTextField.setSelectionColor(new Color(-5841665));
        noiseIntensityMinTextField.setText("");
        noiseIntensityMinTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(noiseIntensityMinTextField, new GridConstraints(9, 6, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        final JLabel label35 = new JLabel();
        label35.setBackground(new Color(-1));
        label35.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label35.setForeground(new Color(-8224126));
        label35.setText("dev");
        label35.setToolTipText("Deviation");
        label35.setVerticalAlignment(1);
        sensorValuesPanel.add(label35, new GridConstraints(10, 3, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        noiseIntensityDevTextField.setBackground(new Color(-1));
        noiseIntensityDevTextField.setCaretColor(new Color(-8224126));
        noiseIntensityDevTextField.setDisabledTextColor(new Color(-8224126));
        noiseIntensityDevTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        noiseIntensityDevTextField.setForeground(new Color(-8224126));
        noiseIntensityDevTextField.setSelectionColor(new Color(-5841665));
        noiseIntensityDevTextField.setText("");
        noiseIntensityDevTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(noiseIntensityDevTextField, new GridConstraints(10, 4, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        final JLabel label36 = new JLabel();
        label36.setBackground(new Color(-1));
        label36.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label36.setForeground(new Color(-8224126));
        label36.setText("avg");
        label36.setToolTipText("Average");
        label36.setVerticalAlignment(1);
        sensorValuesPanel.add(label36, new GridConstraints(10, 5, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        noiseIntensityAvgTextField.setBackground(new Color(-1));
        noiseIntensityAvgTextField.setCaretColor(new Color(-8224126));
        noiseIntensityAvgTextField.setDisabledTextColor(new Color(-8224126));
        noiseIntensityAvgTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        noiseIntensityAvgTextField.setForeground(new Color(-8224126));
        noiseIntensityAvgTextField.setSelectionColor(new Color(-5841665));
        noiseIntensityAvgTextField.setText("");
        noiseIntensityAvgTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(noiseIntensityAvgTextField, new GridConstraints(10, 6, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        final JLabel label37 = new JLabel();
        label37.setBackground(new Color(-1));
        label37.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label37.setForeground(new Color(-8224126));
        label37.setRequestFocusEnabled(false);
        label37.setText("Emissions:");
        label37.setVerticalAlignment(1);
        sensorValuesPanel.add(label37, new GridConstraints(11, 1, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        emissionsCheckBox = new JCheckBox();
        emissionsCheckBox.setBackground(new Color(-1));
        emissionsCheckBox.setForeground(new Color(-8224126));
        emissionsCheckBox.setIcon(new ImageIcon(getClass().getResource("/img/checkboxNotChecked.png")));
        emissionsCheckBox.setLabel("");
        emissionsCheckBox.setPressedIcon(new ImageIcon(getClass().getResource("/img/checkboxChecked.png")));
        emissionsCheckBox.setSelected(false);
        emissionsCheckBox.setSelectedIcon(new ImageIcon(getClass().getResource("/img/checkboxChecked.png")));
        emissionsCheckBox.setText("");
        sensorValuesPanel.add(emissionsCheckBox, new GridConstraints(11, 2, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JLabel label38 = new JLabel();
        label38.setBackground(new Color(-1));
        label38.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label38.setForeground(new Color(-8224126));
        label38.setText("max");
        label38.setToolTipText("Maximum value");
        label38.setVerticalAlignment(1);
        sensorValuesPanel.add(label38, new GridConstraints(11, 3, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JLabel label39 = new JLabel();
        label39.setBackground(new Color(-1));
        label39.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label39.setForeground(new Color(-8224126));
        label39.setText("min");
        label39.setToolTipText("Minimum value");
        label39.setVerticalAlignment(1);
        sensorValuesPanel.add(label39, new GridConstraints(11, 5, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        emissionsMaxTextField.setBackground(new Color(-1));
        emissionsMaxTextField.setCaretColor(new Color(-8224126));
        emissionsMaxTextField.setDisabledTextColor(new Color(-8224126));
        emissionsMaxTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        emissionsMaxTextField.setForeground(new Color(-8224126));
        emissionsMaxTextField.setSelectionColor(new Color(-5841665));
        emissionsMaxTextField.setText("");
        emissionsMaxTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(emissionsMaxTextField, new GridConstraints(11, 4, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        emissionsMinTextField.setBackground(new Color(-1));
        emissionsMinTextField.setCaretColor(new Color(-8224126));
        emissionsMinTextField.setDisabledTextColor(new Color(-8224126));
        emissionsMinTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        emissionsMinTextField.setForeground(new Color(-8224126));
        emissionsMinTextField.setSelectionColor(new Color(-5841665));
        emissionsMinTextField.setText("");
        emissionsMinTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(emissionsMinTextField, new GridConstraints(11, 6, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        final JLabel label40 = new JLabel();
        label40.setBackground(new Color(-1));
        label40.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label40.setForeground(new Color(-8224126));
        label40.setText("dev");
        label40.setToolTipText("Deviation");
        label40.setVerticalAlignment(1);
        sensorValuesPanel.add(label40, new GridConstraints(12, 3, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        emissionsDevTextField.setBackground(new Color(-1));
        emissionsDevTextField.setCaretColor(new Color(-8224126));
        emissionsDevTextField.setDisabledTextColor(new Color(-8224126));
        emissionsDevTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        emissionsDevTextField.setForeground(new Color(-8224126));
        emissionsDevTextField.setSelectionColor(new Color(-5841665));
        emissionsDevTextField.setText("");
        emissionsDevTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(emissionsDevTextField, new GridConstraints(12, 4, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        final JLabel label41 = new JLabel();
        label41.setBackground(new Color(-1));
        label41.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label41.setForeground(new Color(-8224126));
        label41.setText("avg");
        label41.setToolTipText("Average");
        label41.setVerticalAlignment(1);
        sensorValuesPanel.add(label41, new GridConstraints(12, 5, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        emissionsAvgTextField.setBackground(new Color(-1));
        emissionsAvgTextField.setCaretColor(new Color(-8224126));
        emissionsAvgTextField.setDisabledTextColor(new Color(-8224126));
        emissionsAvgTextField.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        emissionsAvgTextField.setForeground(new Color(-8224126));
        emissionsAvgTextField.setSelectionColor(new Color(-5841665));
        emissionsAvgTextField.setText("");
        emissionsAvgTextField.setToolTipText("Only one or double digit number.");
        sensorValuesPanel.add(emissionsAvgTextField, new GridConstraints(12, 6, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        final Spacer spacer5 = new Spacer();
        sensorValuesPanel.add(spacer5, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_WANT_GROW, 1, null, null, null, 0, false));
        final Spacer spacer6 = new Spacer();
        sensorValuesPanel.add(spacer6, new GridConstraints(0, 7, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_WANT_GROW, 1, null, null, null, 0, false));
        final JLabel label42 = new JLabel();
        label42.setBackground(new Color(-1));
        label42.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        label42.setForeground(new Color(-8224126));
        label42.setRequestFocusEnabled(false);
        label42.setText("Sensor of actuator:");
        label42.setVerticalAlignment(1);
        sensorValuesPanel.add(label42, new GridConstraints(6, 1, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        sensorOfActuatorCheckBox = new JCheckBox();
        sensorOfActuatorCheckBox.setBackground(new Color(-1));
        sensorOfActuatorCheckBox.setForeground(new Color(-8224126));
        sensorOfActuatorCheckBox.setIcon(new ImageIcon(getClass().getResource("/img/checkboxNotChecked.png")));
        sensorOfActuatorCheckBox.setLabel("");
        sensorOfActuatorCheckBox.setPressedIcon(new ImageIcon(getClass().getResource("/img/checkboxChecked.png")));
        sensorOfActuatorCheckBox.setSelected(false);
        sensorOfActuatorCheckBox.setSelectedIcon(new ImageIcon(getClass().getResource("/img/checkboxChecked.png")));
        sensorOfActuatorCheckBox.setText("");
        sensorValuesPanel.add(sensorOfActuatorCheckBox, new GridConstraints(6, 2, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        sensorOfActuatorTrueRadBtn = new JRadioButton();
        sensorOfActuatorTrueRadBtn.setBackground(new Color(-1));
        sensorOfActuatorTrueRadBtn.setForeground(new Color(-8224126));
        sensorOfActuatorTrueRadBtn.setIcon(new ImageIcon(getClass().getResource("/img/radioButtonNotSelected.png")));
        sensorOfActuatorTrueRadBtn.setPressedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        sensorOfActuatorTrueRadBtn.setSelected(true);
        sensorOfActuatorTrueRadBtn.setSelectedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        sensorOfActuatorTrueRadBtn.setText("True");
        sensorValuesPanel.add(sensorOfActuatorTrueRadBtn, new GridConstraints(6, 3, 1, 2, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(-1, 18), new Dimension(-1, 18), new Dimension(-1, 18), 0, false));
        sensorOfActuatorFalseRadBtn = new JRadioButton();
        sensorOfActuatorFalseRadBtn.setBackground(new Color(-1));
        sensorOfActuatorFalseRadBtn.setForeground(new Color(-8224126));
        sensorOfActuatorFalseRadBtn.setIcon(new ImageIcon(getClass().getResource("/img/radioButtonNotSelected.png")));
        sensorOfActuatorFalseRadBtn.setPressedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        sensorOfActuatorFalseRadBtn.setSelectedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        sensorOfActuatorFalseRadBtn.setText("False");
        sensorValuesPanel.add(sensorOfActuatorFalseRadBtn, new GridConstraints(6, 5, 1, 2, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_FIXED, new Dimension(-1, 18), new Dimension(-1, 18), new Dimension(-1, 18), 0, false));
        serverResponseLbl = new JLabel();
        serverResponseLbl.setBackground(new Color(-1));
        serverResponseLbl.setFont(new Font("Segoe UI", Font.BOLD, 18));
        serverResponseLbl.setForeground(new Color(-8224126));
        serverResponseLbl.setOpaque(true);
        serverResponseLbl.setText("Server Response");
        mainPanel.add(serverResponseLbl, new GridConstraints(7, 0, 1, 2, GridConstraints.ANCHOR_NORTHWEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        serverResponsePanel = new JPanel();
        serverResponsePanel.setLayout(new GridLayoutManager(1, 1, new Insets(5, 2, 2, 2), -1, -1));
        serverResponsePanel.setAutoscrolls(false);
        serverResponsePanel.setBackground(new Color(-1));
        serverResponsePanel.setFocusCycleRoot(false);
        serverResponsePanel.setMaximumSize(new Dimension(245, -1));
        serverResponsePanel.setMinimumSize(new Dimension(245, 65));
        serverResponsePanel.setPreferredSize(new Dimension(245, 65));
        serverResponsePanel.setRequestFocusEnabled(true);
        mainPanel.add(serverResponsePanel, new GridConstraints(8, 0, 1, 2, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_VERTICAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(360, 280), new Dimension(360, 280), new Dimension(360, 280), 0, false));
        serverResponsePanel.setBorder(BorderFactory.createTitledBorder(BorderFactory.createLineBorder(new Color(-1052689)), ""));
        logTabPanel = new JTabbedPane();
        logTabPanel.setMinimumSize(new Dimension(200, 200));
        logTabPanel.setOpaque(false);
        logTabPanel.setPreferredSize(new Dimension(-1, 200));
        logTabPanel.setTabLayoutPolicy(0);
        logTabPanel.setTabPlacement(1);
        logTabPanel.setToolTipText("Right click to see full log");
        mainPanel.add(logTabPanel, new GridConstraints(2, 3, 7, 3, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, new Dimension(560, -1), null, null, 0, false));
        logTabPanel.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEmptyBorder(), null));
        simulationLogScroller = new JScrollPane();
        logTabPanel.addTab("Simulation Log", simulationLogScroller);
        simulationLogScroller.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEmptyBorder(), null));
        simulationLogText = new JTextPane();
        simulationLogText.setBackground(new Color(-1));
        simulationLogText.setEditable(false);
        simulationLogText.setForeground(new Color(-12303292));
        simulationLogText.setMinimumSize(new Dimension(580, 456));
        simulationLogText.setText("");
        simulationLogText.setVisible(true);
        simulationLogText.putClientProperty("charset", "UTF8");
        simulationLogScroller.setViewportView(simulationLogText);
        toBeSentScroller = new JScrollPane();
        logTabPanel.addTab("To be sent", toBeSentScroller);
        toBeSentScroller.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEmptyBorder(), null));
        toBeSentText = new JTextPane();
        toBeSentText.setBackground(new Color(-1));
        toBeSentText.setEditable(false);
        toBeSentText.setForeground(new Color(-12303292));
        toBeSentText.setText("");
        toBeSentText.putClientProperty("charset", "UTF8");
        toBeSentScroller.setViewportView(toBeSentText);
        errorLogScroller = new JScrollPane();
        logTabPanel.addTab("Error Log", errorLogScroller);
        errorLogScroller.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEmptyBorder(), null));
        errorLogText = new JTextPane();
        errorLogText.setBackground(new Color(-1));
        errorLogText.setEditable(false);
        errorLogText.setForeground(new Color(-12303292));
        errorLogScroller.setViewportView(errorLogText);
        currentMem = new JLabel();
        currentMem.setText("Label");
        mainPanel.add(currentMem, new GridConstraints(9, 0, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        memProgressBar = new JProgressBar();
        memProgressBar.setStringPainted(true);
        mainPanel.add(memProgressBar, new GridConstraints(9, 1, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(240, -1), new Dimension(240, -1), 0, false));
        totalMem = new JLabel();
        totalMem.setText("Label");
        mainPanel.add(totalMem, new GridConstraints(9, 3, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        memoryCheckBtn = new JButton();
        memoryCheckBtn.setBackground(new Color(-5841665));
        memoryCheckBtn.setBorderPainted(false);
        memoryCheckBtn.setForeground(new Color(-12303292));
        memoryCheckBtn.setText("Start memory check");
        mainPanel.add(memoryCheckBtn, new GridConstraints(9, 4, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(-1, 20), new Dimension(-1, 20), 0, false));
        final JPanel panel2 = new JPanel();
        panel2.setLayout(new GridLayoutManager(1, 1, new Insets(5, 2, 2, 2), -1, -1));
        mainPanel.add(panel2, new GridConstraints(5, 7, 1, 1, GridConstraints.ANCHOR_NORTH, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_CAN_GROW, null, new Dimension(240, 73), new Dimension(240, 80), 0, false));
        panel2.setBorder(BorderFactory.createTitledBorder(BorderFactory.createLineBorder(new Color(-1052689)), null));
        serverTable = new JDetailsTable();
        serverTable.setBackground(new Color(-1));
        serverTable.setCellSelectionEnabled(false);
        serverTable.setColumnSelectionAllowed(false);
        serverTable.setFillsViewportHeight(false);
        serverTable.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        serverTable.setForeground(new Color(-8224126));
        serverTable.setGridColor(new Color(-1));
        serverTable.setRowMargin(1);
        serverTable.setRowSelectionAllowed(false);
        serverTable.setSelectionBackground(new Color(-1));
        serverTable.setSelectionForeground(new Color(-5841665));
        serverTable.setShowHorizontalLines(false);
        serverTable.setShowVerticalLines(false);
        serverTable.setToolTipText("");
        serverTable.setVisible(true);
        serverTable.putClientProperty("JTable.autoStartsEdit", Boolean.FALSE);
        serverTable.putClientProperty("Table.isFileList", Boolean.FALSE);
        serverTable.putClientProperty("terminateEditOnFocusLost", Boolean.TRUE);
        panel2.add(serverTable, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(-1, 50), new Dimension(-1, 80), 0, false));
        serverDetailsLbl = new JLabel();
        serverDetailsLbl.setBackground(new Color(-1));
        serverDetailsLbl.setFont(new Font("Segoe UI", Font.BOLD, 18));
        serverDetailsLbl.setForeground(new Color(-8224126));
        serverDetailsLbl.setOpaque(true);
        serverDetailsLbl.setText("Server details");
        mainPanel.add(serverDetailsLbl, new GridConstraints(4, 7, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        internetConnectionLbl = new JLabel();
        internetConnectionLbl.setBackground(new Color(-1));
        internetConnectionLbl.setFont(new Font("Segoe UI", Font.BOLD, 18));
        internetConnectionLbl.setForeground(new Color(-8224126));
        internetConnectionLbl.setOpaque(true);
        internetConnectionLbl.setText("Internet connection");
        mainPanel.add(internetConnectionLbl, new GridConstraints(2, 7, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JPanel panel3 = new JPanel();
        panel3.setLayout(new FlowLayout(FlowLayout.CENTER, 5, 0));
        panel3.setBackground(new Color(-1));
        mainPanel.add(panel3, new GridConstraints(3, 7, 1, 1, GridConstraints.ANCHOR_NORTH, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        panel3.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEmptyBorder(), null));
        connectBtn = new JButton();
        connectBtn.setAutoscrolls(false);
        connectBtn.setBackground(new Color(-3750202));
        connectBtn.setBorderPainted(false);
        connectBtn.setContentAreaFilled(true);
        connectBtn.setFocusPainted(false);
        connectBtn.setForeground(new Color(-1));
        connectBtn.setHorizontalAlignment(0);
        connectBtn.setMargin(new Insets(2, 0, 2, 0));
        connectBtn.setMaximumSize(new Dimension(115, 24));
        connectBtn.setMinimumSize(new Dimension(115, 24));
        connectBtn.setPreferredSize(new Dimension(115, 24));
        connectBtn.setText("Connect");
        panel3.add(connectBtn);
        disconnectBtn = new JButton();
        disconnectBtn.setBackground(new Color(-5841665));
        disconnectBtn.setBorderPainted(false);
        disconnectBtn.setFocusPainted(false);
        disconnectBtn.setForeground(new Color(-12303292));
        disconnectBtn.setHorizontalAlignment(0);
        disconnectBtn.setMargin(new Insets(2, 0, 2, 0));
        disconnectBtn.setMaximumSize(new Dimension(115, 24));
        disconnectBtn.setMinimumSize(new Dimension(115, 24));
        disconnectBtn.setPreferredSize(new Dimension(115, 24));
        disconnectBtn.setText("Disconnect");
        panel3.add(disconnectBtn);
        final Spacer spacer7 = new Spacer();
        mainPanel.add(spacer7, new GridConstraints(0, 3, 1, 2, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_WANT_GROW, 1, null, null, null, 0, false));
        ButtonGroup buttonGroup;
        buttonGroup = new ButtonGroup();
        buttonGroup.add(hostRadBtn);
        buttonGroup.add(ipRadBtn);
        buttonGroup = new ButtonGroup();
        buttonGroup.add(sensorOfActuatorTrueRadBtn);
        buttonGroup.add(sensorOfActuatorFalseRadBtn);
    }

    /**
     * @noinspection ALL
     */
    public JComponent $$$getRootComponent$$$() {
        return mainPanel;
    }

    class JIOTLogPopUpMenu extends JPopupMenu{

        private Color activeBackground = new Color(166,220,255);
        private Color border = new Color(54,177,255);
        private Color font = new Color(87,87,87);
        private JMenuItem seeLog = null;

        public JIOTLogPopUpMenu() {
            seeLog = setUpItem("See full log");
            seeLog.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    pauseSimulation();
                    LogFileViewer viewer = new LogFileViewer(logger.getFile());
                    viewer.setVisible(true);
                }
            });
            /*seeLog.addMouseListener(new MouseAdapter() {
                @Override
                public void mouseClicked(MouseEvent e) {
                    LogFileViewer viewer = new LogFileViewer(file);
                        viewer.setVisible(true);
                }
            });*/
            add(seeLog);
            setBorder(new LineBorder(border,1));
            setBackground(activeBackground);
        }

        protected JMenuItem setUpItem(String title){
            JMenuItem item = new JMenuItem(title);
            item.setBackground(activeBackground);
            item.setFont(new Font("Segoe UI", Font.BOLD, 12));
            item.setForeground(font);
            item.setOpaque(false);
            item.setFocusPainted(false);
            item.setBorderPainted(false);
            return item;
        }
    }

    private class LogPopClickListener extends MouseAdapter {
        public void mousePressed(MouseEvent e){
            if (SwingUtilities.isRightMouseButton(e))
                doPop(e);
        }

        public void mouseReleased(MouseEvent e){
            if (SwingUtilities.isRightMouseButton(e))
                doPop(e);
        }

        private void doPop(MouseEvent e){
            JIOTLogPopUpMenu menu = new JIOTLogPopUpMenu();
            menu.show(e.getComponent(), e.getX(), e.getY());
        }
    }
}
