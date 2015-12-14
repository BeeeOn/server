package com.smarthome.emulator.simulations.panels;

import com.smarthome.emulator.Emulator;
import com.smarthome.emulator.simulations.DetailedSimulation;
import com.smarthome.emulator.simulations.basis.Adapter;
import com.smarthome.emulator.simulations.basis.Logger;
import com.smarthome.emulator.simulations.forms.AddNewSensor;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.Properties;

/**
 * Creates and stores every component needed to display emulation of adapter including {@link com.smarthome.emulator.simulations.panels.SensorPanel}s,
 *  {@link com.smarthome.emulator.simulations.panels.AdapterDetailsPanel}, {@link com.smarthome.emulator.simulations.basis.Logger},
 *  etc. needed to show and switch between displayed representation of adapters.
 */
public class AdapterPanel extends JPanel {
    /** default sensor panel width */
    public static final int SENSOR_PANEL_WIDTH = 205;
    /** default sensor panel height */
    public static final int SENSOR_PANEL_HEIGHT = 300;
    /** adapter's activity indicator */
    private boolean active;
    /** adapter's active icon */
    private ImageIcon inactiveIcon = new ImageIcon(getClass().getResource("/img/inactive_adapter.png"));
    /** adapter's inactive icon */
    private ImageIcon activeIcon = new ImageIcon(getClass().getResource("/img/active_adapter.png"));
    /** adapter's server panel */
    private ServerPanel serverPanel;
    /** GUI variables */
    private JButton adapterButton;
    private JButton addSensorBtn;
    private JButton disableAdapterBtn;
    private JButton enableAdapterBtn;
    private JScrollPane adapterPanelScroller;
    /** this instance */
    private AdapterPanel adapterPanel;
    /** simulation that owns adapter panel */
    private DetailedSimulation simulation;
    /** adapter panel number */
    public int panelNumber;
    /** number of sensors on adapter */
    private int sensorCount = 0;
    /** adapter's logger */
    private Logger logger;
    /** adapter assign for panel */
    private Adapter adapter = null;
    /** detaiels table assign for adapter */
    private AdapterDetailsPanel detailsPanel;
    /** list of sensor panels */
    private ArrayList<SensorPanel> sensorsPanels = new ArrayList<SensorPanel>();

    /**
     * Creates new panel for showing {@link com.smarthome.emulator.simulations.panels.SensorPanel}s, creates and initializes <code>adapter's</code>
     * {@link com.smarthome.emulator.simulations.basis.Logger}, {@link com.smarthome.emulator.simulations.panels.ServerPanel},
     * {@link com.smarthome.emulator.simulations.panels.AdapterDetailsPanel} and {@link com.smarthome.emulator.simulations.basis.Adapter}.
     * @param sim simulation creating new adapter panel, not null
     * @param i number of new adapter panel, not null
     * @param host server hostname, null id <code>ip</code> is not null
     * @param ip server ip, null if <code>host</code> is not null
     * @param port server port
     * @param id new adapter's id
     * @param name new adapter's name
     * @param protocol communication protocol version
     */
    public AdapterPanel(DetailedSimulation sim, int i,String host,String ip,int port,long id,String name,String protocol,boolean registered) {
        super(new FlowLayout(FlowLayout.LEFT,1,6));
        //assign class variables
        panelNumber = i;
        adapterPanel = this;
        simulation = sim;
        //set layout properties
        FlowLayout layout = new FlowLayout(FlowLayout.LEFT, 1, 4);
        layout.setAlignOnBaseline(true);
        this.setLayout(layout);
        this.setBackground(new Color(-1052689));
        this.setMinimumSize(new Dimension(210, 200));
        this.setPreferredSize(new Dimension(625, 450));
        this.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEmptyBorder(), null));
        this.setVisible(true);
        //assign this panel parent scroller and set it's viewport
        this.adapterPanelScroller = simulation.getSensorScroller();
        this.adapterPanelScroller.setViewportView(this);
        //add additional buttons
        addBtnToAdaptersPanel(simulation.getAdaptersPanel());
        //add enable and disable buttons for adapter
        addActivityBtns(simulation.getAdapterActivityBtnsPanel());
        addSensorAddBtn();
        addBtnListeners();
        //set this panel as active
        setActive(true);
        //create adapter log
        logger = new Logger(simulation.getAdapterLogScroller(),simulation.getToBeSentScroller(),simulation.getErrorLogScroller(),false);
        logger.setTrackResponse(true);
        logger.setDumpResponses(true);
        logger.setScrollLog(false);
        //create server table
        serverPanel = new ServerPanel(simulation.getServerTable(),null);
        //create adapter details table
        detailsPanel = new AdapterDetailsPanel(simulation.getAdapterDetailsTable());
        //create and start Adapter class
        this.adapter = new Adapter(this, getLogger());
        this.adapter.setRegistered(registered);
        this.adapter.setServerParameters(host,ip,port);
        this.adapter.setAdapterName(name);
        //name shown in adapter button in side panel with all adapters
        this.setAdapterName(id + " / " + name);
        this.adapter.setAdapterId(id);
        this.adapter.setServerProtocol(Float.valueOf(protocol));
        //initialize scheduler
        this.adapter.initScheduler();
        this.adapter.setActivity(false);
        //display server information
        serverPanel.refreshTable(adapter.getServer());
    }
    /**
     * Adds new {@link com.smarthome.emulator.simulations.panels.SensorPanel} and {@link com.smarthome.emulator.simulations.basis.Sensor}.
     * @param properties properties including information for new sensor
     */
    public void addSensorPanel(int id,Properties properties){
        SensorPanel p = new SensorPanel(this, DetailedSimulation.getSensorsCount(),properties);
        sensorsPanels.add(p);
        adapter.addSensor(id,p);
        DetailedSimulation.incrementSensorsCount();
    }
    /**
     * Opens {@link com.smarthome.emulator.simulations.forms.AddNewSensor} dialog.
     */
    public void addNewSensor(){
        AddNewSensor newSensorDialog = new AddNewSensor(this);
    }
    /**
     * Sets adapter to active state
     */
    public void enableAdapter(){
        getAdapter().setActivity(true);
        setActive(true);
        changeActivityIcon();
        switchActivityBtnsLook(disableAdapterBtn,enableAdapterBtn);
    }
    /**
     * Sets adapter to inactive state
     */
    public void disableAdapter(){
        getAdapter().setActivity(false);
        setActive(false);
        changeActivityIcon();
        switchActivityBtnsLook(enableAdapterBtn,disableAdapterBtn);
        getAdapter().stopSensorsTimers();
    }
    /**
     * Removes all visual adapter's visual components from simulation, removes all
     * instances of classes needed to run adapter's emulation
     */
    public void delete(){
        simulation.getAdaptersPanel().remove(adapterButton);
        simulation.getAdapterActivityBtnsPanel().remove(enableAdapterBtn);
        simulation.getAdapterActivityBtnsPanel().remove(disableAdapterBtn);
        logger.delete();
        adapter.delete();
        serverPanel.delete();
        detailsPanel.delete();
        this.remove(addSensorBtn);
        for(int i = 0; i < sensorsPanels.size(); i++){
            sensorsPanels.get(i).delete();
        }
        adapterPanelScroller.remove(this);
        adapterPanelScroller.setViewportView(null);
        sensorsPanels.clear();
    }
    /**
     * Resize panel holding {@link com.smarthome.emulator.simulations.panels.SensorPanel}s to fit newly added panel
     */
    public void resizePanel(){
        int panelsPerLine = adapterPanel.getSize().width / SENSOR_PANEL_WIDTH;
        int panelsPerColumn = adapterPanel.getPreferredSize().height / SENSOR_PANEL_HEIGHT;
        int newLineCount = (sensorsPanels.size()+1)-(panelsPerLine*panelsPerColumn);
        if(newLineCount>0)
            adapterPanel.setPreferredSize(new Dimension(adapterPanel.getPreferredSize().width, adapterPanel.getPreferredSize().height + SENSOR_PANEL_WIDTH));
        adapterPanel.validate();
    }
    /**
     * Switches {@link com.smarthome.emulator.simulations.panels.AdapterPanel#adapterButton}'s activity icon
     */
    public void changeActivityIcon(){
        if(this.getAdapter().isActive()){
            adapterButton.setIcon(activeIcon);
        }else{
            adapterButton.setIcon(inactiveIcon);
        }
    }
    /**
     * Sets adapter's activity buttons to be visible or not visible according to value given as parameter.
     * @param b <code>true</code> if adapter's activity buttons should be visible, <code>false</code> otherwise
     */
    public void activityBtnsVisibility(Boolean b){
        enableAdapterBtn.setVisible(b);
        disableAdapterBtn.setVisible(b);
    }
    /**
     * Returns adapter's assigned logger
     * @return adapter's assigned logger
     */
    public Logger getLogger() {
        return logger;
    }
    /**
     * Returns adapter's assigned server panel
     * @return adapter's assigned server panel
     */
    public ServerPanel getServerPanel(){
        return this.serverPanel;
    }
    /**
     * Returns adapter instance with information about adapter
     * @return adapter instance with information about adapter
     */
    public Adapter getAdapter(){
        return this.adapter;
    }
    /**
     * Returns adapter's assigned details panel
     * @return adapter's assigned details panel
     */
    public AdapterDetailsPanel getDetailsPanel() {
        return detailsPanel;
    }
    /**
     * Returns adapter's panel number
     * @return adapter's panel number
     */
    public int getPanelNumber(){
        return this.panelNumber;
    }
    /**
     * Sets name of adapter to {@link com.smarthome.emulator.simulations.panels.AdapterPanel#adapterButton}
     * @param s adapter's name
     */
    public void setAdapterName(String s){
        adapterButton.setText(s);
        adapterButton.setToolTipText(s);
    }
    /**
     * Returns whether this adapter's panel is active
     * @return <code>true</code> if adapter's panel is active, <code>false</code> otherwise
     */
    public boolean isActive() {
        return active;
    }
    /**
     * Sets whether this adapter's panel is active
     */
    public void setActive(boolean active) {
        this.active = active;
    }
    /**
     * Returns adapter's button in simulation panel containing all adapter's panels buttons
     * @return adapter's button in simulation panel containing all adapter's panels buttons
     */
    public JButton getAdapterButton() {
        return adapterButton;
    }
    /**
     * Creates {@link javax.swing.JButton} listeners for buttons needed by adapter's panel
     */
    private void addBtnListeners(){
        enableAdapterBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                enableAdapter();
            }
        });
        disableAdapterBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                disableAdapter();
            }
        });
        addSensorBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                addNewSensor();
                resizePanel();
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
        adapterButton.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                adapterPanelScroller.setViewportView(adapterPanel);
                logger.setActiveViewPorts();
                serverPanel.assignModelToTable();
                detailsPanel.assignModelToTable();
                adapterPanelScroller.getVerticalScrollBar().setValue(0);
                simulation.setInactiveAdapterBtns();
                simulation.setActiveAdapterPanel(adapterPanel);
                adapterPanel.setActive(true);
                activityBtnsVisibility(true);
                adapterButton.setBackground(Emulator.simColorBlue);
            }

            @Override
            public void mouseEntered(MouseEvent e) {
                e.getComponent().setBackground(Emulator.simColorAdapterHover);
            }

            @Override
            public void mouseExited(MouseEvent e) {
                if(isActive())
                    e.getComponent().setBackground(Emulator.simColorBlue);
                else
                    e.getComponent().setBackground(Emulator.simColorAdapterInactive);
            }
        });
    }
    /**
     * Creates {@link javax.swing.JButton} for adding new {@link com.smarthome.emulator.simulations.panels.SensorPanel}
     */
    private void addSensorAddBtn(){
        addSensorBtn = new JButton();
        addSensorBtn.setBackground(new Color(-3750202));
        addSensorBtn.setBorderPainted(false);
        addSensorBtn.setFocusPainted(false);
        addSensorBtn.setFont(new Font("Segoe UI", Font.PLAIN, 14));
        addSensorBtn.setForeground(new Color(-12303292));
        addSensorBtn.setHorizontalAlignment(0);
        addSensorBtn.setHorizontalTextPosition(4);
        addSensorBtn.setIcon(new ImageIcon(getClass().getResource("/img/active_sensor.png")));
        addSensorBtn.setIconTextGap(15);
        addSensorBtn.setMaximumSize(new Dimension(SENSOR_PANEL_WIDTH, SENSOR_PANEL_HEIGHT));
        addSensorBtn.setMinimumSize(new Dimension(SENSOR_PANEL_WIDTH, SENSOR_PANEL_HEIGHT));
        addSensorBtn.setOpaque(true);
        addSensorBtn.setPreferredSize(new Dimension(SENSOR_PANEL_WIDTH, SENSOR_PANEL_HEIGHT));
        addSensorBtn.setRequestFocusEnabled(true);
        addSensorBtn.setText("Add Sensor / Actuator");
        addSensorBtn.setVerticalAlignment(0);
        addSensorBtn.setVerticalTextPosition(0);
        this.add(addSensorBtn);
    }
    /**
     * Creates adapter's activity {@link javax.swing.JButton}s and adds them to {@link javax.swing.JPanel} given as parameter
     * @param btnsPanel panel to which are new buttons added to.
     */
    private void addActivityBtns(JPanel btnsPanel){
        enableAdapterBtn = new JButton();
        enableAdapterBtn.setAutoscrolls(false);
        enableAdapterBtn.setBackground(new Color(-5841665));
        enableAdapterBtn.setBorderPainted(false);
        enableAdapterBtn.setContentAreaFilled(true);
        enableAdapterBtn.setFocusPainted(false);
        enableAdapterBtn.setForeground(new Color(-12303292));
        enableAdapterBtn.setHorizontalAlignment(0);
        enableAdapterBtn.setMargin(new Insets(2, 0, 2, 0));
        enableAdapterBtn.setMaximumSize(new Dimension(115, 24));
        enableAdapterBtn.setMinimumSize(new Dimension(115, 24));
        enableAdapterBtn.setPreferredSize(new Dimension(115, 24));
        enableAdapterBtn.setText("Enable");
        btnsPanel.add(enableAdapterBtn);
        disableAdapterBtn = new JButton();
        disableAdapterBtn.setBackground(new Color(-3750202));
        disableAdapterBtn.setBorderPainted(false);
        disableAdapterBtn.setFocusPainted(false);
        disableAdapterBtn.setForeground(new Color(-1));
        disableAdapterBtn.setHorizontalAlignment(0);
        disableAdapterBtn.setMargin(new Insets(2, 0, 2, 0));
        disableAdapterBtn.setMaximumSize(new Dimension(115, 24));
        disableAdapterBtn.setMinimumSize(new Dimension(115, 24));
        disableAdapterBtn.setPreferredSize(new Dimension(115, 24));
        disableAdapterBtn.setText("Disable");
        btnsPanel.add(disableAdapterBtn);
    }
    /**
     * Creates new {@link javax.swing.JButton} and adds it to {@link javax.swing.JPanel} containing all other adapters panels buttons
     * @param panel
     */
    private void addBtnToAdaptersPanel(JPanel panel){
        adapterButton = new JButton();
        adapterButton.setBackground(Emulator.simColorBlue);
        adapterButton.setBorderPainted(false);
        adapterButton.setFocusPainted(false);
        adapterButton.setFont(new Font("Segoe UI", Font.BOLD, 14));
        adapterButton.setForeground(new Color(-1));
        adapterButton.setHorizontalAlignment(2);
        adapterButton.setIcon(inactiveIcon);
        adapterButton.setIconTextGap(10);
        adapterButton.setMargin(new Insets(2, 0, 2, 0));
        adapterButton.setMaximumSize(new Dimension(240, 60));
        adapterButton.setMinimumSize(new Dimension(240, 60));
        adapterButton.setPreferredSize(new Dimension(240, 60));
        adapterButton.setText("Adapter Name / ID");
        adapterButton.setVisible(true);
        panel.add(adapterButton,0);
        simulation.setActiveAdapterPanel(this);
    }
    /**
     * Switches background color between two {@link javax.swing.JButton}s given as parameter
     * @param active to be active button
     * @param inactive to be inactive button
     */
    private void switchActivityBtnsLook(JButton active, JButton inactive) {
        active.setBackground(new Color(-5841665));
        active.setForeground(new Color(-12303292));
        inactive.setBackground(new Color(-3750202));
        inactive.setForeground(new Color(-1));
    }

}
