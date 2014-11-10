package com.smarthome.emulator.simulations;

import com.intellij.uiDesigner.core.GridConstraints;
import com.intellij.uiDesigner.core.GridLayoutManager;
import com.intellij.uiDesigner.core.Spacer;
import com.smarthome.emulator.Emulator;
import com.smarthome.emulator.design.*;
import com.smarthome.emulator.simulations.forms.AddNewSensor;
import com.smarthome.emulator.simulations.forms.ServerOptions;
import com.smarthome.emulator.simulations.panels.AdapterPanel;
import com.smarthome.emulator.simulations.basis.*;
import com.smarthome.emulator.simulations.forms.AddNewAdapter;

import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.swing.plaf.basic.BasicSplitPaneDivider;
import javax.swing.plaf.basic.BasicSplitPaneUI;
import java.awt.*;
import java.awt.event.*;
import java.io.File;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.NotYetBoundException;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Properties;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;
import sun.security.util.Debug;

/**
 * Creates environment for server detailed testing. Provides detailed creation, setting and managing
 * of adapters and their sensors. Also provides possibility to save and load adapters,
 * print adapter's logs and others.
 */
public class DetailedSimulation extends JFrame {
    /**
     * GUI variables
     */
    private JPanel mainPanel;
    private JLabel detailedSimulationLabel;
    private JButton btnMainSave;
    private JPanel menuPanel;
    private JButton btnMainSaveAll;
    private JButton btnMainLoad;
    private JButton btnMainDelete;
    private JScrollPane adaptersScroller;
    private JButton activeAdapterBtn;
    private JButton inactiveAdapterBtn;
    private JPanel adaptersPanel;
    private JButton addNewAdapterBtn;
    private JScrollPane sensorScroller;
    private JScrollPane adapterDetailsScroller;
    private JLabel adaptersLbl;
    private JLabel senActLbl;
    private JLabel adapDetLbl;
    private JPanel sensorsDefaultPanel;
    private JDetailsTable adapterDetailsTable;
    private JDetailsTable serverTable;
    private JLabel serverDetailsLbl;
    private JButton connectBtn;
    private JButton disconnectBtn;
    private JLabel internetConnectionLbl;
    private JTabbedPane logTabPanel;
    private JSplitPane centerSplitPanel;
    private JScrollPane adapterLogScroller;
    private JTextPane adapterLogText;
    private JScrollPane toBeSentScroller;
    private JTextPane toBeSentText;
    private JScrollPane errorLogScroller;
    private JTextPane errorLogText;
    private JProgressBar memProgressBar;
    private JLabel currentMem;
    private JLabel totalMem;
    private JIOTButton memoryCheckBtn;
    private JPanel adapterDetailsPanel;
    private JPanel adapterActivityBtnsPanel;
    private JButton btnMainPrintLogs;
    private JIOTButton btnChangeServer;
    public static final Color simColorMenuItem = new Color(219, 219, 219);
    public static final Color simColorMenuItemFont = new Color(130, 130, 130);
    /**
     * height of adapter's button
     */
    private static final int ADAPTER_BUTTON_HEIGHT = 65;
    /**
     * list of adapters panels
     */
    private ArrayList<AdapterPanel> adapters = new ArrayList<AdapterPanel>();
    /**
     * currently active adapter panel
     */
    private AdapterPanel activeAdapterPanel = null;
    /**
     * number of created adapters
     */
    public int adaptersCount = 0;
    /**
     * number of created sensors, also used as new sensor id
     */
    private static int sensorsCount = 0;
    /**
     * adapter saving indicator
     */
    private boolean saved = false;

    private ActuatorListener server = null;

    /**
     * Creates and initializes all visual components needed to perform detailed simulation
     */
    public DetailedSimulation() {
        $$$setupUI$$$();
        this.initComponents();
        startMemoryCheck();
        setLocationRelativeTo(null);
        setVisible(true);
    }

    /**
     * Show dialog with options to save and print logs of currently active adapter panel,
     * save all creates adapter panels or cancel dialog.
     * If "Save and print current" action is selected, save and print dialogs are shown.
     * If "Save all adapters" action is selected, save dialog is shown.
     */
    public void showSavePrintDialog() {
        Object[] options = {"Save and print current", "Save all adapters", "Do nothing"};
        int dialogResult = JOptionPane.showOptionDialog(null, "Would you like to save simulation and print dialog?", "Warning",
                JOptionPane.DEFAULT_OPTION, JOptionPane.WARNING_MESSAGE,
                null, options, options[0]);
        if (dialogResult == 0) {
            if (activeAdapterPanel != null) {
                if (activeAdapterPanel.getAdapter().isActive())
                    activeAdapterPanel.disableAdapter();
                activeAdapterPanel.getLogger().printLogs("Detailed Simulation -> Adapter:" + activeAdapterPanel.getAdapter().getName() + "/" + activeAdapterPanel.getAdapter().getAdapterId());
                saveAdapterToXML(activeAdapterPanel.getAdapter());
            }
        } else if (dialogResult == 1) {
            saveAdapterToXML(null);
        } else if (dialogResult == 2) {
            dispose();
        }
    }

    /**
     * Checks if <code>id</code> given as parameter exist in adapters list
     *
     * @param id id to be checked
     * @return <code>true</code> if <code>id</code> is free to take, <code>false</code> otherwise
     */
    public boolean isAdapterIdFree(long id) {
        for (int i = 0; i < adapters.size(); i++) {
            if (adapters.get(i).getAdapter().getAdapterId() == id)
                return false;
        }
        return true;
    }

    /**
     * Increase number of sensors
     */
    public static void incrementSensorsCount() {
        if (getSensorsCount() < Integer.MAX_VALUE)
            sensorsCount++;
    }

    /**
     * Decrease number of sensors
     */
    public static void decrementSensorsCount() {
        if (getSensorsCount() > 1)
            sensorsCount--;
    }

    /**
     * Opens load dialog. From given XML file loads adapter information and it's
     * sensors properties sets, which are used to add new adapter panel and all
     * other needed components to simulation.
     * <p>
     * Shows errors:
     * <ul>
     * <li>ADAPTER_ID_TAKEN if loaded adapter id already exists</li>
     * <li>LOAD_DOC_CREATE if cannot create document from given file</li>
     * <li>LOAD_DOC_READ if cannot read given file</li>
     * <li>LOAD_DOC_PARSE if cannot parse given file</li>
     * <li>FILE_NOT_FOUND if given file was not found</li>
     * </ul>
     * </p>
     */
    public void loadAdapterFromXML() {
        try {
            String xmlLoadFile = showDialog("Specify file to load", false);
            if (xmlLoadFile != null) {
                File fXmlFile = new File(xmlLoadFile);
                DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
                DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
                Document doc = dBuilder.parse(fXmlFile);
                //get all adapters
                NodeList adaptersElements = doc.getElementsByTagName("adapter");
                for (int temp = 0; temp < adaptersElements.getLength(); temp++) {
                    Node adapterNode = adaptersElements.item(temp);
                    if (adapterNode.getNodeType() == Node.ELEMENT_NODE) {
                        //get adapter info
                        Element adapterElement = (Element) adapterNode;
                        //get server info
                        Element serverElement = (Element) adapterElement.getElementsByTagName("server").item(0);
                        long id = Long.parseLong(adapterElement.getAttribute("id"));
                        if (!isAdapterIdFree(id)) {
                            Emulator.showError(Error.ADAPTER_ID_TAKEN.getMessage());
                            return;
                        }
                        //create new adapter panel
                        AdapterPanel newAdapter = new AdapterPanel(
                                this,
                                adaptersCount,
                                serverElement.getElementsByTagName("hostname").item(0).getTextContent(),
                                serverElement.getElementsByTagName("ip").item(0).getTextContent(),
                                Integer.valueOf(serverElement.getElementsByTagName("port").item(0).getTextContent()),
                                id,
                                adapterElement.getElementsByTagName("name").item(0).getTextContent(),
                                adapterElement.getElementsByTagName("protocol").item(0).getTextContent(),
                                adapterElement.getElementsByTagName("registered").item(0).getTextContent().equals("true") ? true : false
                        );
                        //adding new adapter panel
                        setInactiveAdapterBtns();
                        getAdapters().add(newAdapter);
                        adaptersCount++;
                        validateAfterAdapterInsert();
                        Element sensorsElement = (Element) adapterElement.getElementsByTagName("sensors").item(0);
                        NodeList sensorsElements = sensorsElement.getElementsByTagName("sensor");
                        for (int temp1 = 0; temp1 < sensorsElements.getLength(); temp1++) {
                            Node sensorNode = sensorsElements.item(temp1);
                            if (sensorNode.getNodeType() == Node.ELEMENT_NODE) {
                                Properties properties = new Properties();
                                Element sensorElement = (Element) sensorNode;
                                properties.setProperty("id", sensorElement.getAttribute("id"));
                                properties.setProperty("name", sensorElement.getElementsByTagName("name").item(0).getTextContent());
                                properties.setProperty("refreshRate", sensorElement.getElementsByTagName("refreshRate").item(0).getTextContent());
                                properties.setProperty("signal", sensorElement.getElementsByTagName("signal").item(0).getTextContent());
                                properties.setProperty("battery", sensorElement.getElementsByTagName("battery").item(0).getTextContent());
                                properties.setProperty("protocol", sensorElement.getElementsByTagName("protocol").item(0).getTextContent());
                                properties.setProperty("icon", sensorElement.getElementsByTagName("icon").item(0).getTextContent());
                                properties.setProperty("backgroundColor", sensorElement.getElementsByTagName("backgroundColor").item(0).getTextContent());
                                properties.setProperty("fontColor", sensorElement.getElementsByTagName("fontColor").item(0).getTextContent());
                                properties.setProperty("lblFontColor", sensorElement.getElementsByTagName("lblFontColor").item(0).getTextContent());
                                properties.setProperty("propertyNames", sensorElement.getElementsByTagName("propertyNames").item(0).getTextContent());
                                Element valuesElement = (Element) sensorElement.getElementsByTagName("values").item(0);
                                NodeList valuesElements = valuesElement.getElementsByTagName("value");
                                for (int temp2 = 0; temp2 < valuesElements.getLength(); temp2++) {
                                    Node valueNode = valuesElements.item(temp2);
                                    if (valueNode.getNodeType() == Node.ELEMENT_NODE) {
                                        Element valueElement = (Element) valueNode;
                                        String previous;
                                        if (properties.getProperty("values") == null)
                                            previous = "";
                                        else
                                            previous = properties.getProperty("values") + ",";
                                        properties.setProperty("values", previous + valueElement.getElementsByTagName("initial").item(0).getTextContent());
                                        if (properties.getProperty("valueTypes") == null)
                                            previous = "";
                                        else
                                            previous = properties.getProperty("valueTypes") + ",";
                                        properties.setProperty("valueTypes", previous + valueElement.getElementsByTagName("type").item(0).getTextContent());
                                        if (properties.getProperty("valueClasses") == null)
                                            previous = "";
                                        else
                                            previous = properties.getProperty("valueClasses") + ",";
                                        properties.setProperty("valueClasses", previous + valueElement.getElementsByTagName("class").item(0).getTextContent());
                                        if (properties.getProperty("min") == null)
                                            previous = "";
                                        else
                                            previous = properties.getProperty("min") + ",";
                                        properties.setProperty("min", previous + valueElement.getElementsByTagName("min").item(0).getTextContent());
                                        if (properties.getProperty("max") == null)
                                            previous = "";
                                        else
                                            previous = properties.getProperty("max") + ",";
                                        properties.setProperty("max", previous + valueElement.getElementsByTagName("max").item(0).getTextContent());
                                        if (properties.getProperty("dev") == null)
                                            previous = "";
                                        else
                                            previous = properties.getProperty("dev") + ",";
                                        properties.setProperty("dev", previous + valueElement.getElementsByTagName("dev").item(0).getTextContent());
                                        if (properties.getProperty("avg") == null)
                                            previous = "";
                                        else
                                            previous = properties.getProperty("avg") + ",";
                                        properties.setProperty("avg", previous + valueElement.getElementsByTagName("avg").item(0).getTextContent());
                                        if (properties.getProperty("seed") == null)
                                            previous = "";
                                        else
                                            previous = properties.getProperty("seed") + ",";
                                        properties.setProperty("seed", previous + valueElement.getElementsByTagName("seed").item(0).getTextContent());
                                        if (properties.getProperty("previouslyGenerated") == null)
                                            previous = "";
                                        else
                                            previous = properties.getProperty("previouslyGenerated") + ",";
                                        properties.setProperty("previouslyGenerated", previous + valueElement.getElementsByTagName("previouslyGenerated").item(0).getTextContent());
                                    }
                                }
                                //create new sensor panel
                                newAdapter.addSensorPanel(AddNewSensor.ipToInt(properties.getProperty("id")), properties);

                            }
                        }


                    }
                }
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
     * Opens save dialog. Offers saving of one or all adapters. To given file saves adapter's and it's sensors information
     * needed for later loading of this adapter.
     *
     * @param adapter adapter to be saved, null if safe all adapters in simulation
     *                <p>
     *                Shows errors:
     *                <ul>
     *                <li>SAVE_DOC_CREATE if cannot create document for given file</li>
     *                </ul>
     *                </p>
     */
    public void saveAdapterToXML(Adapter adapter) {
        try {
            String xmlSaveFile = showDialog("Specify file to save", true);
            if (xmlSaveFile != null) {
                DocumentBuilderFactory docFactory = DocumentBuilderFactory.newInstance();
                DocumentBuilder docBuilder = docFactory.newDocumentBuilder();
                // create document
                Document doc = docBuilder.newDocument();
                //if parameter adapter is null insert all existing adapters to XML
                if (adapter == null) {
                    Element adaptersElement = doc.createElement("adapters");
                    doc.appendChild(adaptersElement);
                    for (int i = 0; i < getAdapters().size(); i++) {
                        adapterToXML(getAdapters().get(i).getAdapter(), doc, adaptersElement);
                    }
                } else {
                    adapterToXML(adapter, doc, null);
                }
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
     * Inserts information from <code>adapter</code> to <code>doc</code> in XML format attaching
     * adapter's element to <code>rootElement</code>. If <code>rootElement</code> is null, makes
     * adapter's element <code>doc</code>'s root element.
     *
     * @param adapter     adapter with information to be saved, not null
     * @param doc         document to save XML elements to, not null
     * @param rootElement element to attach adapter's element to, possible null
     */
    private void adapterToXML(Adapter adapter, Document doc, Element rootElement) {
        Element adapterElement = doc.createElement("adapter");
        adapterElement.setAttribute("id", String.valueOf(adapter.getAdapterId()));
        if (rootElement == null) {
            doc.appendChild(adapterElement);
        } else {
            rootElement.appendChild(adapterElement);
        }
        //adapter name
        Element name = doc.createElement("name");
        String adapterName = adapter.getAdapterName();
        name.appendChild(doc.createTextNode(adapterName != null ? adapterName : ""));
        adapterElement.appendChild(name);
        //adapter protocol
        Element protocol = doc.createElement("protocol");
        protocol.appendChild(doc.createTextNode(Float.valueOf(adapter.getServerProtocol()).toString()));
        adapterElement.appendChild(protocol);
        //adapter protocol
        Element registered = doc.createElement("registered");
        registered.appendChild(doc.createTextNode(adapter.isRegistered() ? "true" : "false"));
        adapterElement.appendChild(registered);

        //server element
        Element server = doc.createElement("server");
        adapterElement.appendChild(server);
        //server hostname
        Element hostname = doc.createElement("hostname");
        hostname.appendChild(doc.createTextNode(adapter.getServer().getName()));
        server.appendChild(hostname);
        //server ip
        Element ip = doc.createElement("ip");
        ip.appendChild(doc.createTextNode(adapter.getServer().getIp()));
        server.appendChild(ip);
        //server port
        Element port = doc.createElement("port");
        port.appendChild(doc.createTextNode(Integer.valueOf(adapter.getServer().getPort()).toString()));
        server.appendChild(port);
        //sensors
        saveSensorsToXML(adapter, doc, adapterElement);
    }

    /**
     * Inserts information from <code>adapter</code>'s sensor list to <code>doc</code> in XML format attaching
     * sensor's element to <code>rootElement</code>.
     *
     * @param adapter     adapter with sensors list to be saved, not null
     * @param doc         document to save XML elements to,not null
     * @param rootElement element to attach sensors element to,not null
     */
    private void saveSensorsToXML(Adapter adapter, Document doc, Element rootElement) {
        //add sensors tag
        Element sensors = doc.createElement("sensors");
        rootElement.appendChild(sensors);
        //get sensors list from adapter
        ArrayList<Sensor> sensorsList = adapter.getSensors();
        for (int i = 0; i < sensorsList.size(); i++) {
            //get sensor
            Sensor sensor = sensorsList.get(i);
            //create sensor root tag
            Element sensorElement = doc.createElement("sensor");
            //assign id as attribute
            sensorElement.setAttribute("id", String.valueOf(sensor.getSensorId()));
            sensors.appendChild(sensorElement);
            //sensor info
            //sensor name
            Element nameElement = doc.createElement("name");
            nameElement.appendChild(doc.createTextNode(sensor.getName()));
            sensorElement.appendChild(nameElement);
            //sensor refresh rate
            Element refreshRateElement = doc.createElement("refreshRate");
            refreshRateElement.appendChild(doc.createTextNode(String.valueOf(sensor.getRefreshRate())));
            sensorElement.appendChild(refreshRateElement);
            //sensor signal
            Element signalElement = doc.createElement("signal");
            signalElement.appendChild(doc.createTextNode(String.valueOf(sensor.getSignal())));
            sensorElement.appendChild(signalElement);
            //sensor battery
            Element batteryElement = doc.createElement("battery");
            batteryElement.appendChild(doc.createTextNode(String.valueOf(sensor.getBattery())));
            sensorElement.appendChild(batteryElement);
            //sensor protocol
            Element protocolElement = doc.createElement("protocol");
            protocolElement.appendChild(doc.createTextNode(String.valueOf(sensor.getProtocol())));
            sensorElement.appendChild(protocolElement);
            //sensor icon
            Element iconElement = doc.createElement("icon");
            iconElement.appendChild(doc.createTextNode(String.valueOf(sensor.getIconPath())));
            sensorElement.appendChild(iconElement);
            //sensor background color
            Element backgroundColorElement = doc.createElement("backgroundColor");
            backgroundColorElement.appendChild(doc.createTextNode(String.valueOf(sensor.getBackgroundColorString())));
            sensorElement.appendChild(backgroundColorElement);
            //sensor font color
            Element fontColorElement = doc.createElement("fontColor");
            fontColorElement.appendChild(doc.createTextNode(String.valueOf(sensor.getFontColorString())));
            sensorElement.appendChild(fontColorElement);
            //sensor label font color
            Element lblFontColorElement = doc.createElement("lblFontColor");
            lblFontColorElement.appendChild(doc.createTextNode(String.valueOf(sensor.getLblFontColorString())));
            sensorElement.appendChild(lblFontColorElement);
            //sensor property names
            Element propertyNamesElement = doc.createElement("propertyNames");
            propertyNamesElement.appendChild(doc.createTextNode(String.valueOf(sensor.getPropertyNames())));
            sensorElement.appendChild(propertyNamesElement);
            //sensor values
            saveSensorValursToXML(sensor, doc, sensorElement);
        }
    }

    /**
     * Inserts information about <code>sensor</code>'s values to <code>doc</code> in XML format
     * attaching values element to given <code>rootElement</code>.
     *
     * @param sensor      sensor with values to be saved, not null
     * @param doc         document to save XML elements to
     * @param rootElement element to attach values element to, not null
     */
    private void saveSensorValursToXML(Sensor sensor, Document doc, Element rootElement) {
        //sensor values
        Element valuesElement = doc.createElement("values");
        rootElement.appendChild(valuesElement);
        ArrayList<Value> valuesList = sensor.getValues();
        for (int i = 0; i < valuesList.size(); i++) {
            Value value = valuesList.get(i);
            //value element
            Element valueElement = doc.createElement("value");
            valuesElement.appendChild(valueElement);
            //value initial value
            Element initialValueElement = doc.createElement("initial");
            initialValueElement.appendChild(doc.createTextNode(String.valueOf(value.getInitialValue())));
            valueElement.appendChild(initialValueElement);
            //value  value
            Element valueValueElement = doc.createElement("valueValue");
            valueValueElement.appendChild(doc.createTextNode(String.valueOf(value.getValue())));
            valueElement.appendChild(valueValueElement);
            //value initial value
            Element typeElement = doc.createElement("type");
            typeElement.appendChild(doc.createTextNode(String.valueOf(value.getType())));
            valueElement.appendChild(typeElement);
            //value initial value
            Element classElement = doc.createElement("class");
            classElement.appendChild(doc.createTextNode(value.getValueClass()));
            /*if (value.getValue() instanceof Float)
                classElement.appendChild(doc.createTextNode("Float"));
            else if (value.getValue() instanceof Boolean)
                classElement.appendChild(doc.createTextNode("Boolean"));
            else if (value.getValue() instanceof Short)
                classElement.appendChild(doc.createTextNode("Short"));
            else if (value.getValue() instanceof Integer)
                classElement.appendChild(doc.createTextNode("Integer"));
            else if (value.getValue() instanceof Double)
                classElement.appendChild(doc.createTextNode("Double"));
            else if (value.getValue() instanceof Long)
                classElement.appendChild(doc.createTextNode("Long"));*/
            valueElement.appendChild(classElement);
            //value max,min,dev,avg for generator
            Element maxElement = doc.createElement("max");
            Element minElement = doc.createElement("min");
            Element devElement = doc.createElement("dev");
            Element avgElement = doc.createElement("avg");
            if (value.getValue() instanceof Boolean) {
                maxElement.appendChild(doc.createTextNode("none"));
                minElement.appendChild(doc.createTextNode("none"));
                devElement.appendChild(doc.createTextNode("none"));
                avgElement.appendChild(doc.createTextNode("none"));
            } else {
                maxElement.appendChild(doc.createTextNode(String.valueOf(value.getMax())));
                minElement.appendChild(doc.createTextNode(String.valueOf(value.getMin())));
                devElement.appendChild(doc.createTextNode(String.valueOf(value.getDev())));
                avgElement.appendChild(doc.createTextNode(String.valueOf(value.getAvg())));
            }
            valueElement.appendChild(maxElement);
            valueElement.appendChild(minElement);
            valueElement.appendChild(devElement);
            valueElement.appendChild(avgElement);
            //value initial value
            Element seedElement = doc.createElement("seed");
            seedElement.appendChild(doc.createTextNode(String.valueOf(value.getGeneratorSeed())));
            valueElement.appendChild(seedElement);
            //number of previously generated values for value history
            Element previouslyGeneratedElement = doc.createElement("previouslyGenerated");
            previouslyGeneratedElement.appendChild(doc.createTextNode(String.valueOf(value.getHistory().size())));
            valueElement.appendChild(previouslyGeneratedElement);
        }
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
        if (save) {
            returnVal = fileChooser.showSaveDialog(this);
        } else {
            returnVal = fileChooser.showOpenDialog(this);
        }
        if (returnVal == fileChooser.APPROVE_OPTION) {
            return fileChooser.getSelectedFile().getAbsolutePath();
        }
        return null;
    }

    /**
     * Sets all existing adapters panels buttons to inactive state and change their background
     */
    public void setInactiveAdapterBtns() {
        for (AdapterPanel adapter : adapters) {
            adapter.setActive(false);
            adapter.getAdapterButton().setBackground(Emulator.simColorAdapterInactive);
            adapter.activityBtnsVisibility(false);
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
        memoryCheckBtn.setActivity(false);
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
        memoryCheckBtn.setActivity(true);
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
    //region Getters and Setters

    /**
     * Returns server information table
     *
     * @return server information table
     */
    public JDetailsTable getServerTable() {
        return serverTable;
    }

    /**
     * Returns panel which stores all adapters panels buttons
     *
     * @return panel which stores all adapters panels buttons
     */
    public JPanel getAdaptersPanel() {
        return adaptersPanel;
    }

    /**
     * Returns <code>JScrollPane</code> for adapter's details panel
     *
     * @return <code>JScrollPane</code> for adapter's details panel
     */
    public JScrollPane getAdapterDetailsScroller() {
        return adapterDetailsScroller;
    }

    /**
     * Returns <code>JScrollPane</code> for adapter's log
     *
     * @return <code>JScrollPane</code> for adapter's log
     */
    public JScrollPane getAdapterLogScroller() {
        return adapterLogScroller;
    }

    /**
     * Returns <code>JScrollPane</code> for adapter's to be sent log
     *
     * @return <code>JScrollPane</code> for adapter's to be sent log
     */
    public JScrollPane getToBeSentScroller() {
        return toBeSentScroller;
    }

    /**
     * Returns <code>JScrollPane</code> for adapter's error log
     *
     * @return <code>JScrollPane</code> for adapter's error log
     */
    public JScrollPane getErrorLogScroller() {
        return errorLogScroller;
    }

    /**
     * Returns <code>JScrollPane</code> for panel containing sensors panels
     *
     * @return <code>JScrollPane</code> for panel containing sensors panels
     */
    public JScrollPane getSensorScroller() {
        return sensorScroller;
    }

    /**
     * Returns whether active adapter's panel is saved
     *
     * @return <code>true</code> if is saved, <code>false</code> otherwise
     */
    public boolean isSaved() {
        return this.saved;
    }

    /**
     * Sets whether active adapter's panel is saved
     *
     * @param b <code>true</code> if is saved, <code>false</code> otherwise
     */
    public void setSaved(Boolean b) {
        this.saved = b;
    }

    /**
     * Returns adapter's details information table
     *
     * @return adapter's details information table
     */
    public JDetailsTable getAdapterDetailsTable() {
        return adapterDetailsTable;
    }

    /**
     * Returns number of sensors created so far
     *
     * @return number of sensors created so far
     */
    public static int getSensorsCount() {
        return sensorsCount;
    }

    /**
     * Returns panel fot adapter's activity buttons
     *
     * @return panel fot adapter's activity buttons
     */
    public JPanel getAdapterActivityBtnsPanel() {
        return adapterActivityBtnsPanel;
    }

    /**
     * Returns list of existing adapters panels
     *
     * @return list of existing adapters panels
     */
    public ArrayList<AdapterPanel> getAdapters() {
        return this.adapters;
    }

    /**
     * Sets currently active adapter panel
     *
     * @param panel adapter panel to be set as active
     */
    public void setActiveAdapterPanel(AdapterPanel panel) {
        this.activeAdapterPanel = panel;
    }
    //endregion
    //region private methods

    /**
     * Removes all visual components and data of currently active adapter panel
     */
    private void deleteCurrentAdapter() {
        //remove instances of class SensorPanel
        activeAdapterPanel.delete();
        //remove instance of class AdapterPanel
        this.remove(activeAdapterPanel);
        adapters.remove(activeAdapterPanel);
        //trim list of AdapterPanels
        adapters.trimToSize();
        //set new active adapter
        if (!adapters.isEmpty())
            setActiveAdapterPanel(adapters.get(adapters.size() - 1));
    }

    /**
     * Called on window closing, if currently active adapter panel is not saved,
     * shows save dialog.
     */
    private void onCancel() {
        if (server != null) {
            server.setTerminate(true);
        }
        if (!isSaved()) {
            showSavePrintDialog();
        }
    }

    /**
     * Initializing all visual components needed for simulation, assigning mouse listeners
     * for all simulation buttons
     */
    private void initComponents() {
        setTitle("Detailed Simulation - SmartHome Emulator");
        setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                onCancel();
            }
        });
        UIModifications();
        setContentPane(mainPanel);

        setVisible(true);
        setBounds(100, 100, 1316, 680);

        addMainBtnMouseListeners();
        mouseListeners();
        startServer();
        pack();
    }

    private void startServer() {
        server = new ActuatorListener();
        server.start();
    }

    /**
     * Create and assign mouse listeners for main simulation buttons
     */
    private void addMainBtnMouseListeners() {
        btnMainSave.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (activeAdapterPanel != null) {
                    saveAdapterToXML(activeAdapterPanel.getAdapter());
                }
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
        btnMainSaveAll.addMouseListener(new MouseAdapter() {
            public void mouseClicked(MouseEvent e) {
                if (adapters.size() != 0) {
                    saveAdapterToXML(null);
                }
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
                loadAdapterFromXML();
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

        btnMainDelete.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (activeAdapterPanel != null) {
                    int dialogResult = JOptionPane.showConfirmDialog(null, "Would you like to delete currently selected adapter?", "Warning", JOptionPane.YES_NO_OPTION);
                    if (dialogResult == JOptionPane.YES_OPTION) {
                        deleteCurrentAdapter();
                    }
                }
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
                if (activeAdapterPanel != null) {
                    if (activeAdapterPanel.getAdapter().isActive())
                        activeAdapterPanel.disableAdapter();
                    activeAdapterPanel.getLogger().printLogs("Detailed Simulation -> Adapter:" + activeAdapterPanel.getAdapter().getName() + "/" + activeAdapterPanel.getAdapter().getAdapterId());
                }
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
     * Create and assign mouse listeners for simulation buttons
     */
    private void mouseListeners() {
        disconnectBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (activeAdapterPanel != null) {
                    activeAdapterPanel.getAdapter().setInternetConnection(false);
                    switchConnectionButtonsLook(connectBtn, disconnectBtn);
                }
            }
        });
        connectBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                //JIOTNotification not = new JIOTNotification(DetailedSimulation.this, "Notification", new Dimension(150, 30));
                if (activeAdapterPanel != null) {
                    activeAdapterPanel.getAdapter().setInternetConnection(true);
                    switchConnectionButtonsLook(disconnectBtn, connectBtn);
                }
            }
        });
        memoryCheckBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (isMemoryChecked)
                    stopMemoryCheck();
                else
                    startMemoryCheck();

            }
        });
        addNewAdapterBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                addAdapterButton();
            }

            @Override
            public void mouseEntered(MouseEvent e) {
                e.getComponent().setBackground(Emulator.simColorAdapterHover);
            }

            @Override
            public void mouseExited(MouseEvent e) {
                e.getComponent().setBackground(Emulator.simColorAdapterAdd);
            }
        });
        btnChangeServer.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (activeAdapterPanel != null) {
                    if (activeAdapterPanel.getAdapter().isActive())
                        activeAdapterPanel.disableAdapter();

                    ServerOptions options = new ServerOptions(activeAdapterPanel.getAdapter().getServer());
                    options.setVisible(true);
                }
            }
        });
    }

    /**
     * Opens {@link com.smarthome.emulator.simulations.forms.AddNewAdapter} dialog.
     */
    public void addAdapterButton() {
        setInactiveAdapterBtns();
        AddNewAdapter addForm = new AddNewAdapter(this, adaptersCount);
        validateAfterAdapterInsert();
    }

    /**
     * Resize panel storing adapters buttons after adding new adapter panel
     */
    private void validateAfterAdapterInsert() {
        if (adapters.size() > 0 && (adaptersPanel.getPreferredSize().getHeight() - (adapters.size() * ADAPTER_BUTTON_HEIGHT + ADAPTER_BUTTON_HEIGHT)) < 0) {
            adaptersPanel.setPreferredSize(new Dimension(adaptersPanel.getPreferredSize().width, adaptersPanel.getPreferredSize().height + ADAPTER_BUTTON_HEIGHT));
        }
        adaptersPanel.validate();
        adaptersScroller.validate();
    }

    /**
     * Switches given {@link javax.swing.JButton}s background to
     * indicate active and inactive button
     *
     * @param active   to be active button
     * @param inactive to be inactive button
     */
    private void switchConnectionButtonsLook(JButton active, JButton inactive) {
        active.setBackground(new Color(-5841665));
        active.setForeground(new Color(-12303292));
        inactive.setBackground(new Color(-3750202));
        inactive.setForeground(new Color(-1));
    }

    /**
     * Modifications to GUI before painting the frame
     */
    private void UIModifications() {
        /* flatten split panel divider */
        centerSplitPanel.setUI(new BasicSplitPaneUI() {
            public BasicSplitPaneDivider createDefaultDivider() {
                return new BasicSplitPaneDivider(this) {
                    public void setBorder(Border b) {
                    }

                    @Override
                    public void paint(Graphics g) {
                        g.setColor(Color.WHITE);
                        g.fillRect(0, 0, getSize().width, getSize().height);
                        super.paint(g);
                    }
                };
            }
        });
        centerSplitPanel.setBorder(null);
        /* change tabbedPane UI */
        logTabPanel.setUI(new JCustomTabbedPaneUI());
        for (int i = 0; i < logTabPanel.getTabCount(); i++) {
            logTabPanel.setForegroundAt(i, new Color(68, 68, 68));
        }
        mainPanel.addComponentListener(new ComponentAdapter() {
            public void componentResized(ComponentEvent e) {
                if (activeAdapterPanel != null) {
                    activeAdapterPanel.resizePanel();
                }
            }
        });

        adaptersScroller.getVerticalScrollBar().setUI(new JIOTScrollBarUI());
        sensorScroller.getVerticalScrollBar().setUI(new JIOTScrollBarUI());
        adapterLogScroller.getVerticalScrollBar().setUI(new JIOTScrollBarUI());
        toBeSentScroller.getVerticalScrollBar().setUI(new JIOTScrollBarUI());
        errorLogScroller.getVerticalScrollBar().setUI(new JIOTScrollBarUI());

    }

    private void createUIComponents() {
        // TODO: place custom component creation code here
        memoryCheckBtn = new JIOTButton("Start memory check", true);

        btnChangeServer = new JIOTButton("Change details", 115, 24, true);
    }

    private Sensor findSensor(long adapterId, int sensorsId) {
        for (AdapterPanel panel : adapters) {
            if (panel.getAdapter().equalsId(adapterId)) {
                return panel.getAdapter().findSensor(sensorsId);
            }
        }
        return null;
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
        mainPanel.setLayout(new GridLayoutManager(10, 7, new Insets(15, 15, 15, 15), -1, -1));
        mainPanel.setBackground(new Color(-1));
        mainPanel.setMinimumSize(new Dimension(1300, 680));
        mainPanel.setOpaque(true);
        mainPanel.setPreferredSize(new Dimension(1300, 680));
        detailedSimulationLabel = new JLabel();
        detailedSimulationLabel.setFont(new Font("Segoe UI", Font.PLAIN, 28));
        detailedSimulationLabel.setForeground(new Color(-13127169));
        detailedSimulationLabel.setText("Detailed Simulation");
        mainPanel.add(detailedSimulationLabel, new GridConstraints(0, 0, 1, 3, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final Spacer spacer1 = new Spacer();
        mainPanel.add(spacer1, new GridConstraints(2, 2, 7, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_VERTICAL, 1, GridConstraints.SIZEPOLICY_WANT_GROW, null, null, null, 0, false));
        adaptersLbl = new JLabel();
        adaptersLbl.setBackground(new Color(-1));
        adaptersLbl.setFont(new Font("Segoe UI", Font.BOLD, 18));
        adaptersLbl.setForeground(new Color(-8224126));
        adaptersLbl.setOpaque(true);
        adaptersLbl.setText("Adapters");
        mainPanel.add(adaptersLbl, new GridConstraints(1, 0, 1, 2, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        senActLbl = new JLabel();
        senActLbl.setBackground(new Color(-1));
        senActLbl.setFont(new Font("Segoe UI", Font.BOLD, 18));
        senActLbl.setForeground(new Color(-8224126));
        senActLbl.setOpaque(true);
        senActLbl.setText("Sensors / Actuators");
        mainPanel.add(senActLbl, new GridConstraints(1, 3, 1, 2, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(529, 25), null, 0, false));
        adapDetLbl = new JLabel();
        adapDetLbl.setBackground(new Color(-1));
        adapDetLbl.setFont(new Font("Segoe UI", Font.BOLD, 18));
        adapDetLbl.setForeground(new Color(-8224126));
        adapDetLbl.setOpaque(true);
        adapDetLbl.setText("Adapter details");
        mainPanel.add(adapDetLbl, new GridConstraints(1, 6, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final Spacer spacer2 = new Spacer();
        mainPanel.add(spacer2, new GridConstraints(2, 5, 7, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_VERTICAL, 1, GridConstraints.SIZEPOLICY_WANT_GROW, null, new Dimension(32, 14), null, 0, false));
        menuPanel = new JPanel();
        menuPanel.setLayout(new FlowLayout(FlowLayout.TRAILING, 5, 5));
        menuPanel.setBackground(new Color(-1));
        menuPanel.setEnabled(true);
        mainPanel.add(menuPanel, new GridConstraints(0, 4, 1, 3, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_VERTICAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        btnMainSave = new JButton();
        btnMainSave.setBackground(new Color(-2368549));
        btnMainSave.setBorderPainted(false);
        btnMainSave.setEnabled(true);
        btnMainSave.setFocusPainted(false);
        btnMainSave.setFont(new Font("Segoe UI", Font.BOLD, 14));
        btnMainSave.setForeground(new Color(-8224126));
        btnMainSave.setMargin(new Insets(0, 14, 0, 14));
        btnMainSave.setText("Save");
        menuPanel.add(btnMainSave);
        btnMainSaveAll = new JButton();
        btnMainSaveAll.setBackground(new Color(-2368549));
        btnMainSaveAll.setBorderPainted(false);
        btnMainSaveAll.setEnabled(true);
        btnMainSaveAll.setFocusPainted(false);
        btnMainSaveAll.setFont(new Font("Segoe UI", Font.BOLD, 14));
        btnMainSaveAll.setForeground(new Color(-8224126));
        btnMainSaveAll.setMargin(new Insets(0, 14, 0, 14));
        btnMainSaveAll.setText("Save All");
        menuPanel.add(btnMainSaveAll);
        btnMainLoad = new JButton();
        btnMainLoad.setBackground(new Color(-2368549));
        btnMainLoad.setBorderPainted(false);
        btnMainLoad.setEnabled(true);
        btnMainLoad.setFocusPainted(false);
        btnMainLoad.setFont(new Font("Segoe UI", Font.BOLD, 14));
        btnMainLoad.setForeground(new Color(-8224126));
        btnMainLoad.setMargin(new Insets(0, 14, 0, 14));
        btnMainLoad.setText("Load");
        menuPanel.add(btnMainLoad);
        btnMainDelete = new JButton();
        btnMainDelete.setBackground(new Color(-2368549));
        btnMainDelete.setBorderPainted(false);
        btnMainDelete.setFocusPainted(false);
        btnMainDelete.setFont(new Font("Segoe UI", Font.BOLD, 14));
        btnMainDelete.setForeground(new Color(-8224126));
        btnMainDelete.setMargin(new Insets(0, 14, 0, 14));
        btnMainDelete.setText("Delete");
        menuPanel.add(btnMainDelete);
        btnMainPrintLogs = new JButton();
        btnMainPrintLogs.setBackground(new Color(-2368549));
        btnMainPrintLogs.setBorderPainted(false);
        btnMainPrintLogs.setFocusPainted(false);
        btnMainPrintLogs.setFont(new Font("Segoe UI", Font.BOLD, 14));
        btnMainPrintLogs.setForeground(new Color(-8224126));
        btnMainPrintLogs.setMargin(new Insets(0, 14, 0, 14));
        btnMainPrintLogs.setText("Print Logs");
        menuPanel.add(btnMainPrintLogs);
        centerSplitPanel = new JSplitPane();
        centerSplitPanel.setBackground(new Color(-1));
        centerSplitPanel.setContinuousLayout(true);
        centerSplitPanel.setDividerLocation(310);
        centerSplitPanel.setDividerSize(15);
        centerSplitPanel.setOrientation(0);
        mainPanel.add(centerSplitPanel, new GridConstraints(2, 3, 7, 2, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_WANT_GROW, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, null, new Dimension(200, 200), null, 0, false));
        centerSplitPanel.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEmptyBorder(), null));
        logTabPanel = new JTabbedPane();
        logTabPanel.setMinimumSize(new Dimension(200, 200));
        logTabPanel.setOpaque(false);
        logTabPanel.setPreferredSize(new Dimension(-1, 200));
        logTabPanel.setTabLayoutPolicy(0);
        logTabPanel.setTabPlacement(1);
        centerSplitPanel.setRightComponent(logTabPanel);
        logTabPanel.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEmptyBorder(), null));
        adapterLogScroller = new JScrollPane();
        logTabPanel.addTab("Adapter Log", adapterLogScroller);
        adapterLogScroller.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEmptyBorder(), null));
        adapterLogText = new JTextPane();
        adapterLogText.setBackground(new Color(-1));
        adapterLogText.setEditable(false);
        adapterLogText.setForeground(new Color(-12303292));
        adapterLogText.setText("");
        adapterLogText.setVisible(true);
        adapterLogText.putClientProperty("charset", "UTF8");
        adapterLogScroller.setViewportView(adapterLogText);
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
        sensorScroller = new JScrollPane();
        sensorScroller.setBackground(new Color(-1052689));
        sensorScroller.setHorizontalScrollBarPolicy(30);
        sensorScroller.setMinimumSize(new Dimension(412, 18));
        sensorScroller.setPreferredSize(new Dimension(625, 420));
        centerSplitPanel.setLeftComponent(sensorScroller);
        sensorScroller.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEmptyBorder(), null));
        sensorsDefaultPanel = new JPanel();
        sensorsDefaultPanel.setLayout(new FlowLayout(FlowLayout.LEFT, 1, 4));
        sensorsDefaultPanel.setBackground(new Color(-1052689));
        sensorsDefaultPanel.setMinimumSize(new Dimension(220, 210));
        sensorsDefaultPanel.setName("sensorPanel");
        sensorsDefaultPanel.setPreferredSize(new Dimension(620, 420));
        sensorScroller.setViewportView(sensorsDefaultPanel);
        sensorsDefaultPanel.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEmptyBorder(), null));
        adaptersScroller = new JScrollPane();
        adaptersScroller.setAutoscrolls(false);
        adaptersScroller.setBackground(new Color(-1052689));
        adaptersScroller.setDoubleBuffered(true);
        adaptersScroller.setHorizontalScrollBarPolicy(31);
        adaptersScroller.setOpaque(false);
        adaptersScroller.setVerticalScrollBarPolicy(20);
        mainPanel.add(adaptersScroller, new GridConstraints(2, 0, 2, 2, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_VERTICAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_WANT_GROW, new Dimension(260, -1), new Dimension(260, -1), null, 0, false));
        adaptersScroller.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEmptyBorder(), null));
        adaptersPanel = new JPanel();
        adaptersPanel.setLayout(new FlowLayout(FlowLayout.CENTER, 5, 5));
        adaptersPanel.setAutoscrolls(false);
        adaptersPanel.setBackground(new Color(-1052689));
        adaptersPanel.setFocusCycleRoot(false);
        adaptersPanel.setMaximumSize(new Dimension(245, -1));
        adaptersPanel.setMinimumSize(new Dimension(245, 65));
        adaptersPanel.setPreferredSize(new Dimension(245, 65));
        adaptersPanel.setRequestFocusEnabled(true);
        adaptersScroller.setViewportView(adaptersPanel);
        adaptersPanel.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEmptyBorder(), ""));
        activeAdapterBtn = new JButton();
        activeAdapterBtn.setBackground(new Color(-13192705));
        activeAdapterBtn.setBorderPainted(false);
        activeAdapterBtn.setFocusPainted(false);
        activeAdapterBtn.setFont(new Font("Segoe UI", Font.BOLD, 14));
        activeAdapterBtn.setForeground(new Color(-1));
        activeAdapterBtn.setHorizontalAlignment(2);
        activeAdapterBtn.setIcon(new ImageIcon(getClass().getResource("/img/active_adapter.png")));
        activeAdapterBtn.setIconTextGap(10);
        activeAdapterBtn.setMargin(new Insets(2, 0, 2, 0));
        activeAdapterBtn.setMaximumSize(new Dimension(240, 60));
        activeAdapterBtn.setMinimumSize(new Dimension(240, 60));
        activeAdapterBtn.setPreferredSize(new Dimension(240, 60));
        activeAdapterBtn.setText("Adapter Name / ID");
        activeAdapterBtn.setVisible(false);
        adaptersPanel.add(activeAdapterBtn);
        inactiveAdapterBtn = new JButton();
        inactiveAdapterBtn.setBackground(new Color(-5841665));
        inactiveAdapterBtn.setBorderPainted(false);
        inactiveAdapterBtn.setFocusPainted(false);
        inactiveAdapterBtn.setFont(new Font("Segoe UI", Font.BOLD, 14));
        inactiveAdapterBtn.setForeground(new Color(-1));
        inactiveAdapterBtn.setHorizontalAlignment(2);
        inactiveAdapterBtn.setIcon(new ImageIcon(getClass().getResource("/img/inactive_adapter.png")));
        inactiveAdapterBtn.setIconTextGap(10);
        inactiveAdapterBtn.setMargin(new Insets(2, 0, 2, 0));
        inactiveAdapterBtn.setMaximumSize(new Dimension(240, 60));
        inactiveAdapterBtn.setMinimumSize(new Dimension(240, 60));
        inactiveAdapterBtn.setPreferredSize(new Dimension(240, 60));
        inactiveAdapterBtn.setText("Adapter Name / ID");
        inactiveAdapterBtn.setVisible(false);
        adaptersPanel.add(inactiveAdapterBtn);
        addNewAdapterBtn = new JButton();
        addNewAdapterBtn.setBackground(new Color(-3750202));
        addNewAdapterBtn.setBorderPainted(false);
        addNewAdapterBtn.setFocusPainted(false);
        addNewAdapterBtn.setFont(new Font("Segoe UI", Font.BOLD, 14));
        addNewAdapterBtn.setForeground(new Color(-1));
        addNewAdapterBtn.setHorizontalAlignment(2);
        addNewAdapterBtn.setIcon(new ImageIcon(getClass().getResource("/img/add_adapter.png")));
        addNewAdapterBtn.setIconTextGap(10);
        addNewAdapterBtn.setMargin(new Insets(2, 0, 2, 0));
        addNewAdapterBtn.setMaximumSize(new Dimension(240, 60));
        addNewAdapterBtn.setMinimumSize(new Dimension(240, 60));
        addNewAdapterBtn.setPreferredSize(new Dimension(240, 60));
        addNewAdapterBtn.setText("Add new adapter");
        adaptersPanel.add(addNewAdapterBtn);
        serverDetailsLbl = new JLabel();
        serverDetailsLbl.setBackground(new Color(-1));
        serverDetailsLbl.setFont(new Font("Segoe UI", Font.BOLD, 18));
        serverDetailsLbl.setForeground(new Color(-8224126));
        serverDetailsLbl.setOpaque(true);
        serverDetailsLbl.setText("Server details");
        mainPanel.add(serverDetailsLbl, new GridConstraints(4, 0, 1, 2, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        internetConnectionLbl = new JLabel();
        internetConnectionLbl.setBackground(new Color(-1));
        internetConnectionLbl.setFont(new Font("Segoe UI", Font.BOLD, 18));
        internetConnectionLbl.setForeground(new Color(-8224126));
        internetConnectionLbl.setOpaque(true);
        internetConnectionLbl.setText("Internet connection");
        mainPanel.add(internetConnectionLbl, new GridConstraints(7, 0, 1, 2, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JPanel panel1 = new JPanel();
        panel1.setLayout(new FlowLayout(FlowLayout.LEFT, 5, 5));
        panel1.setBackground(new Color(-1));
        mainPanel.add(panel1, new GridConstraints(8, 0, 1, 2, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, null, null, null, 0, false));
        panel1.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEmptyBorder(), null));
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
        panel1.add(connectBtn);
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
        panel1.add(disconnectBtn);
        currentMem = new JLabel();
        currentMem.setText("Label");
        mainPanel.add(currentMem, new GridConstraints(9, 0, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        memProgressBar = new JProgressBar();
        memProgressBar.setStringPainted(true);
        mainPanel.add(memProgressBar, new GridConstraints(9, 1, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(115, -1), new Dimension(115, -1), 0, false));
        totalMem = new JLabel();
        totalMem.setText("Label");
        mainPanel.add(totalMem, new GridConstraints(9, 3, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        memoryCheckBtn.setText("Start memory check");
        mainPanel.add(memoryCheckBtn, new GridConstraints(9, 4, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(-1, 20), new Dimension(-1, 20), 0, false));
        adapterDetailsPanel = new JPanel();
        adapterDetailsPanel.setLayout(new GridLayoutManager(1, 1, new Insets(6, 2, 2, 2), -1, -1));
        mainPanel.add(adapterDetailsPanel, new GridConstraints(2, 6, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_CAN_GROW, new Dimension(260, -1), new Dimension(260, -1), null, 0, false));
        adapterDetailsPanel.setBorder(BorderFactory.createTitledBorder(BorderFactory.createLineBorder(new Color(-1052689)), null));
        adapterDetailsTable = new JDetailsTable();
        adapterDetailsTable.setBackground(new Color(-1));
        adapterDetailsTable.setFillsViewportHeight(true);
        adapterDetailsTable.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        adapterDetailsTable.setForeground(new Color(-8224126));
        adapterDetailsTable.setGridColor(new Color(-1));
        adapterDetailsPanel.add(adapterDetailsTable, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, null, null, null, 0, false));
        final JPanel panel2 = new JPanel();
        panel2.setLayout(new GridLayoutManager(1, 1, new Insets(5, 2, 2, 2), -1, -1));
        mainPanel.add(panel2, new GridConstraints(5, 0, 1, 2, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_VERTICAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_CAN_GROW, null, new Dimension(240, -1), new Dimension(240, -1), 0, false));
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
        btnChangeServer.setText("Change details");
        mainPanel.add(btnChangeServer, new GridConstraints(6, 1, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        adapterActivityBtnsPanel = new JPanel();
        adapterActivityBtnsPanel.setLayout(new FlowLayout(FlowLayout.LEFT, 5, 5));
        adapterActivityBtnsPanel.setBackground(new Color(-1));
        mainPanel.add(adapterActivityBtnsPanel, new GridConstraints(4, 6, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_VERTICAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        adapterActivityBtnsPanel.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEmptyBorder(), null));
        detailedSimulationLabel.setLabelFor(sensorScroller);
        adaptersLbl.setLabelFor(adaptersScroller);
        senActLbl.setLabelFor(adaptersScroller);
        adapDetLbl.setLabelFor(adaptersScroller);
        serverDetailsLbl.setLabelFor(adaptersScroller);
        internetConnectionLbl.setLabelFor(adaptersScroller);
    }

    /**
     * @noinspection ALL
     */
    public JComponent $$$getRootComponent$$$() {
        return mainPanel;
    }

    //endregion

    private class ActuatorListener extends Thread {
        private boolean terminate = false;
        private final static int PORT = 7978;
        private boolean proceed = true;
        private ServerSocketChannel serverSocket;

        public ActuatorListener() {
            try {
                serverSocket = ServerSocketChannel.open();
                serverSocket.socket().bind(new InetSocketAddress(PORT));
                serverSocket.configureBlocking(false);
                System.out.println("Started to listen on port: " + PORT);
            } catch (NotYetBoundException e) {
                System.out.println("ERROR: Port is busy");
            } catch (IOException e) {
                System.out.println("ERROR: Cannot bind to port");
            }
        }

        public void run() {
            while (!terminate) {
                if (proceed) {
                    try {
                        SocketChannel incomingSocket = serverSocket.accept();
                        if (incomingSocket != null) {
                            System.out.println("Received new message from server");
                            ByteBuffer in = ByteBuffer.allocate(5000);
                            int bytesRead = incomingSocket.read(in);
                            //if socket suddenly closes
                            if (bytesRead != -1) {
                                parseMessage(in);
                            } else {
                                System.out.println("ERROR: Cannot read data from server");
                            }
                            incomingSocket.close();
                        }
                    } catch (IOException e) {
                        System.out.println("ERROR: Cannot accept server");
                    }
                }
            }
            try {
                serverSocket.close();
                System.out.println("Server terminated");
            } catch (IOException e) {
                System.out.println("ERROR: Cannot close serverSocket");
            }
        }

        private void parseMessage(ByteBuffer message) {
            System.out.println("Parsing incoming message from server");
            message.flip();
            message.position(0);
            /*Short protocol = message.getShort();
            String sensorId = String.valueOf(message.getInt());
            String adapterId = String.valueOf(message.getInt());
            Short refreshRate = message.getShort();
            if (Sensor.DEBUG) {
                System.out.println("Protocol: " + protocol);
                System.out.println("Sensor ID: " + sensorId);
                System.out.println("Adapter ID: " + adapterId);
                System.out.println("Refresh rate: " + refreshRate);
            }*/

            /*String fromBytes = new String(message.array());
            org.dom4j.Document document = null;
            try {
                document = DocumentHelper.parseText(fromBytes);
            } catch (DocumentException e) {
                System.err.println("Cannot create XML document from server initiated message!");
            }

            if (document == null) {
                System.err.println("XML document from server-initiated message is null");
                return;
            }
            org.dom4j.Element rootElement = document.getRootElement();
            String protocol = rootElement.attribute("protocol_version").getValue();
            if (protocol.equals(String.valueOf(activeAdapterPanel.getAdapter()))) {
                String state = rootElement.attribute("state").getValue();
                String id = null;
                switch (state) {
                    case "listen":
                        id = rootElement.attribute("id").getValue();
                        break;
                }
                if (id != null) {
                    for (AdapterPanel adapterPanel:getAdapters()){
                        if(adapterPanel.getAdapter().getAdapterId() == Long.valueOf(id)){
                        }
                    }
                }
            }*/
        }

        public void setTerminate(boolean b) {
            this.terminate = b;
        }
    }

}