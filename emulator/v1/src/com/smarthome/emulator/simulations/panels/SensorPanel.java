package com.smarthome.emulator.simulations.panels;

import com.intellij.uiDesigner.core.GridConstraints;
import com.intellij.uiDesigner.core.GridLayoutManager;
import com.smarthome.emulator.design.JDetailsTableCellRenderer;
import com.smarthome.emulator.design.JDetailsTable;
import com.smarthome.emulator.simulations.basis.Sensor;
import com.smarthome.emulator.simulations.basis.Sensor;
import com.smarthome.emulator.simulations.forms.SensorOptions;
import com.smarthome.emulator.simulations.forms.ValueHistory;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.math.BigInteger;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Properties;

/**
 * Creates and stores every component needed to display sensor emulation. Displays information from {@link com.smarthome.emulator.simulations.basis.Sensor}
 * and provides {@link javax.swing.JButton}s to change or overview <code>sensor</code>'s state.
 */
public class SensorPanel extends JPanel{
    //region UI variables
    /** this instance */
    private SensorPanel panel;
    /** sensor containing data */
    private Sensor sensor = null;
    /** adapter's panel that creates this panel */
    private AdapterPanel adapterPanel;
    private JButton sensorLnlBtn;
    /** table showing sensor's information */
    private JDetailsTable propertyTbl;
    /** names of properties in table */
    private String[] property_names = new String[11];
    /** names of properties in table in string format */
    private String propertyNamesString;
    private String[] prop_col_names = {"Property","Value"};
    /** number of rows in sensor's table */
    private static final int PROPERTY_ROW_COUNT = 11;
    /** data model for table */
    private DefaultTableModel propertyModel;
    private JPanel btnPanel;
    /** color of sensor's label background */
    private Color backgroundColor;
    /** color of text showing information about sensor */
    private Color fontColor;
    /** color of sensor's label text */
    private Color lblFontColor;
    /** icon assigned to sensor's label */
    private ImageIcon lblIcon;
    //region btn panel variables
    /** buttons for sensor */
    private JButton valueHistoryBtn = null;
    private JButton switchBtn = null;
    private JButton changeDetailsBtn = null;
    private JButton disconnectBtn = null;
    //endregion
    //endregion
    /** sensor's panel number */
    private SensorPanel me = null;
    private int panelNumber;
    /** sensor's name shown in sensors's label */
    private String name;
    /** properties set holding information about sensor */
    private Properties defaultProperties;
    /**
     * Creates new panel for showing {@link com.smarthome.emulator.simulations.basis.Sensor}'s information, also creates {@link javax.swing.JButton}s
     * for change or overview these information
     * @param aPanel adapter panel that creates this panel
     * @param panelNumber sensor's panel number
     * @param properties properties set containing information about sensor
     */
    public SensorPanel(AdapterPanel aPanel,int panelNumber,Properties properties) {
        //creates layout
        super(new GridLayoutManager(3, 1, new Insets(0, 0, 5, 0), -1, -1));
        this.me = this;
        this.defaultProperties = properties;
        this.name = defaultProperties.getProperty("name");
        //assign icon
        this.lblIcon = new ImageIcon(getClass().getResource(defaultProperties.getProperty("icon")));
        //sets colors from properties set
        setColorsFromProperties();
        this.panel = this;
        this.panelNumber = panelNumber;
        //sets GUI variables
        this.setBackground(Color.WHITE);
        this.setFocusable(true);
        this.setFont(new Font("Segoe UI", Font.PLAIN, 12));
        this.setForeground(new Color(-8224126));
        this.setMaximumSize(new Dimension(AdapterPanel.SENSOR_PANEL_WIDTH, 2147483647));
        this.setMinimumSize(new Dimension(AdapterPanel.SENSOR_PANEL_WIDTH, -1));
        this.setOpaque(true);
        this.setPreferredSize(new Dimension(AdapterPanel.SENSOR_PANEL_WIDTH, AdapterPanel.SENSOR_PANEL_HEIGHT));
        this.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEmptyBorder(), null));
        this.setVisible(true);
        this.adapterPanel = aPanel;
        //adds sensor panel to adapter panel
        adapterPanel.add(this,0);
        addLblBtn();
        addPropertyTbl();
        addBtnPanel();
        createPropertyTable();
        adapterPanel.validate();
    }
    /**
     * Removes all visual components for displaying sensor panel
     */
    public void delete(){
        panel.remove(sensorLnlBtn);
        propertyModel.setRowCount(0);
        propertyTbl.validate();
        panel.remove(propertyTbl);
        panel.remove(btnPanel);
    }
    /**
     * Updates model's data with information from {@link com.smarthome.emulator.simulations.basis.Sensor}
     */
    public void refreshPropertyTbl(){
        if(this.sensor != null){
            if(this.sensor.isActive()){
                this.setStatus("Active");
                disconnectBtn.setText("Disconnect");
            }else{
                this.setStatus("Inactive");
                disconnectBtn.setText("Connect");
            }
            this.setSensorName(this.sensor.getName());
            this.setSensorId(this.sensor.getSensorId());
            this.setRefreshRate(this.sensor.getRefreshRate());
            this.setBattery(this.sensor.getBattery());
            this.setSignal(this.sensor.getSignal());
            this.setLastValues(this.sensor.getLastValues());
        }
    }
    /**
     * Sets table's model information about <code>sensor</code>'s activity status
     * @param s <code>sensor</code>'s activity status ( <code>Active</code> or <code>Inactive</code> )
     */
    public void setStatus(String s){
        propertyModel.setValueAt(s,0,1);
    }
    /**
     * Sets table's model information about <code>sensor</code>'s id
     * @param ip <code>sensor</code>'s id
     */
    public void setSensorId(int ip){
        propertyModel.setValueAt(intToIp(ip),1,1);
    }

    public static String intToIp(int i){
        return ((i >> 24 ) & 0xFF) + "." +

                ((i >> 16 ) & 0xFF) + "." +

                ((i >> 8 ) & 0xFF) + "." +

                ( i & 0xFF);
    }
    /**
     * Sets table's model information about <code>sensor</code>'s refresh rate
     * @param i <code>sensor</code>'s refresh rate
     */
    public void setRefreshRate(int i){
        propertyModel.setValueAt(i,2,1);
    }
    /**
     * Sets table's model information about <code>sensor</code>'s battery
     * @param s <code>sensor</code>'s battery
     */
    public void setBattery(short s){
        propertyModel.setValueAt(s,3,1);
    }
    public void setSensorName(String name){
        this.name = name;
        sensorLnlBtn.setText(this.name);
    }
    /**
     * Sets table's model information about <code>sensor</code>'s signal
     * @param s <code>sensor</code>'s signal
     */
    public void setSignal(short s){
        propertyModel.setValueAt(s,4,1);
    }
    public Sensor getSensor(){
        return this.sensor;
    }
    /**
     * Returns string with sensor's property names separated by ","
     * @return string with sensor's property names separated by ","
     */
    public String getPropertyNamesString(){
        return this.propertyNamesString;
    }
    /**
     * Set's table's model information about current sensor's values
     * @param values list of current values
     */
    public void setLastValues(ArrayList<Object> values){
        if(values == null)
            return;
        for (int i = 0; i < values.size(); i++){
            if(values.get(i) instanceof Boolean){
                propertyModel.setValueAt(((Boolean) values.get(i)).booleanValue() ? "ON" : "OFF", i + 6, 1);
            }else{
                propertyModel.setValueAt(values.get(i),i + 6,1);
            }
        }
    }
    /**
     * Returns sensor's activity button
     * @return sensor's activity button
     */
    public JButton getDisconnectBtn(){
        return this.disconnectBtn;
    }
    /**
     * Assign {@link com.smarthome.emulator.simulations.basis.Sensor} to panel
     * @param s sensor to be assigned to panel
     */
    public void setSensor(Sensor s){
        this.sensor = s;
    }
    /**
     * Returns default properties set for sensor's panel and sensor
     * @return default properties set for sensor's panel and sensor
     */
    public Properties getDefaultProperties(){
        return defaultProperties;
    }
    /**
     * Sets GUI colors from default properties set
     */
    private void setColorsFromProperties(){
        String[] bColor = defaultProperties.getProperty("backgroundColor").split(",");
        String[] fColor = defaultProperties.getProperty("fontColor").split(",");
        String[] lfColor = defaultProperties.getProperty("lblFontColor").split(",");
        try{
            backgroundColor = new Color(Integer.parseInt(bColor[0]),Integer.parseInt(bColor[1]),Integer.parseInt(bColor[2]));
            fontColor = new Color(Integer.parseInt(fColor[0]),Integer.parseInt(fColor[1]),Integer.parseInt(fColor[2]));
            lblFontColor = new Color(Integer.parseInt(lfColor[0]),Integer.parseInt(lfColor[1]),Integer.parseInt(lfColor[2]));
        } catch (NumberFormatException e){
            backgroundColor = new Color(166, 220, 255);
            fontColor = new Color(198,198,198);
            lblFontColor = new Color(68,68,68);
        }
    }
    /**
     * Creates new {@link javax.swing.table.DefaultTableModel} for {@link com.smarthome.emulator.design.JDetailsTable} table.
     * Also assigns new renderer {@link com.smarthome.emulator.design.JDetailsTableCellRenderer} fot this table.
     */
    private void createPropertyTable(){
        //if property name is not set in properties file, set it to "-"
        propertyNamesString = defaultProperties.getProperty("propertyNames");
        defaultProperties.setProperty("propertyNames", "Status:,Id:,Refresh Rate:,Battery:,Signal:,LastValues," + defaultProperties.getProperty("propertyNames"));
        String[] tmp_names = defaultProperties.getProperty("propertyNames").split(",");
        for(int i = 0;i< PROPERTY_ROW_COUNT;i++){
            if(i<tmp_names.length){
                property_names[i] = tmp_names[i];
            }else{
                property_names[i] = "-";
            }
        }
        propertyModel = new DefaultTableModel(prop_col_names, PROPERTY_ROW_COUNT) {
            public Object getValueAt(int row, int col) {
                if (col == 0)
                    return property_names[row];
                return super.getValueAt(row, col);
            }

            public boolean isCellEditable(int row, int col) {
                return false;
            }
        };
        propertyTbl.setModel(this.propertyModel);
        propertyTbl.setRowSelectionAllowed(false);
        propertyTbl.setColumnSelectionAllowed(false);
        propertyTbl.setDefaultRenderer(Object.class, new JDetailsTableCellRenderer());
        ((JDetailsTableCellRenderer)propertyTbl.getDefaultRenderer(Object.class)).setTextColor(fontColor);
    }
    /**
     * Inserts {@link javax.swing.JButton} to sensor's label.
     * Currently unavailable
     */
    private void addLblBtn(){
        sensorLnlBtn = new JButton();
        sensorLnlBtn.setBackground(backgroundColor);
        sensorLnlBtn.setBorderPainted(false);
        sensorLnlBtn.setFocusPainted(false);
        sensorLnlBtn.setFont(new Font("Segoe UI", Font.PLAIN, 14));
        sensorLnlBtn.setForeground(lblFontColor);
        sensorLnlBtn.setHorizontalAlignment(0);
        sensorLnlBtn.setHorizontalTextPosition(4);
        if(this.lblIcon != null) sensorLnlBtn.setIcon(lblIcon);
        sensorLnlBtn.setIconTextGap(15);
        sensorLnlBtn.setOpaque(true);
        sensorLnlBtn.setRequestFocusEnabled(true);
        sensorLnlBtn.setText(name);
        sensorLnlBtn.setVisible(true);
        panel.add(sensorLnlBtn,
                new GridConstraints(0, 0, 1, 1,
                        GridConstraints.ANCHOR_NORTH,
                        GridConstraints.FILL_HORIZONTAL,
                        GridConstraints.SIZEPOLICY_CAN_GROW,
                        GridConstraints.SIZEPOLICY_FIXED,
                        new Dimension(-1, 55),
                        new Dimension(-1, 55),
                        new Dimension(-1, 55), 0, false)
        );
    }
    /**
     * Creates {@link com.smarthome.emulator.design.JDetailsTable} table for displaying sensor's information
     */
    private void addPropertyTbl(){
        propertyTbl = new JDetailsTable();
        propertyTbl.setBackground(new Color(-1));
        propertyTbl.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        propertyTbl.setForeground(new Color(-8224126));
        propertyTbl.setGridColor(new Color(-1));
        propertyTbl.setShowHorizontalLines(false);
        propertyTbl.setShowVerticalLines(false);
        panel.add(propertyTbl,
                new GridConstraints(1, 0, 1, 1,
                        GridConstraints.ANCHOR_CENTER,
                        GridConstraints.FILL_BOTH,
                        GridConstraints.SIZEPOLICY_WANT_GROW,
                        GridConstraints.SIZEPOLICY_WANT_GROW,
                        null,
                        new Dimension(150, 50),
                        null,
                        0,
                        false
                )
        );
    }
    /**
     * Creates new {@link javax.swing.JPanel} containing new {@link javax.swing.JButton}s providing
     * possibility to change or overview sensor's information
     */
    private void addBtnPanel(){
        btnPanel = new JPanel();
        btnPanel.setLayout(new GridLayoutManager(2, 2, new Insets(3, 0, 3, 0), -1, -1));
        btnPanel.setBackground(new Color(-1));
        btnPanel.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEmptyBorder(), null));
        panel.add(btnPanel,
                new GridConstraints(
                        2, 0, 1, 1,
                        GridConstraints.ANCHOR_CENTER,
                        GridConstraints.FILL_NONE,
                        GridConstraints.SIZEPOLICY_FIXED,
                        GridConstraints.SIZEPOLICY_FIXED,
                        null,
                        null,
                        null,
                        0,
                        false
                )
        );
        //add sensor buttons
        //valueHistoryBtn = addBtn("Value history",0,0);
        changeDetailsBtn = addBtn("Change details",0,0);
        disconnectBtn = addBtn("Connect",0,1);
        //if sensor has actuator, add switch button
        if(defaultProperties != null){
            String t = defaultProperties.getProperty("valueTypes",null);
            if(t!= null){
                String[] types = t.split(",");
                for(String type : types){
                    //TODO set global actuator number
                    if(type.trim().equals("4") || type.trim().equals("3")){
                        switchBtn = addBtn("Switch",1,1);
                    }
                }
            }
        }
        //add buttons listeners
        /*valueHistoryBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if(sensor!=null){
                    ValueHistory valueHistoryDialog = new ValueHistory(sensor);
                }
            }
        });*/
        disconnectBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                Object o = e.getSource();
                JButton b = null;
                if(o instanceof JButton)
                    b = (JButton)o;
                if(b!= null)
                    if(sensor.isActive()){
                        sensor.disconnect();
                    }else{
                        sensor.connect();
                    }
            }
        });
        changeDetailsBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if(sensor.isActive())
                    sensor.disconnect();
                SensorOptions a = new SensorOptions(me);
                a.setVisible(true);
            }
        });
        if(switchBtn != null){
            switchBtn.addMouseListener(new MouseAdapter() {
                @Override
                public void mouseClicked(MouseEvent e) {
                    if(sensor!=null)
                        sensor.switchActuator();
                }
            });
        }
    }
    /**
     * Help method for creating new buttons for sensor's panel
     * @param text button's text
     * @param row row in button's panel
     * @param col col in button's panel
     * @return new button
     */
    private JButton addBtn(String text,int row,int col){
        JButton newBtn = new JButton();
        newBtn.setBackground(backgroundColor);
        newBtn.setBorderPainted(false);
        newBtn.setFocusPainted(false);
        newBtn.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        newBtn.setForeground(lblFontColor);
        newBtn.setMargin(new Insets(0, 0, 0, 0));
        newBtn.setMaximumSize(new Dimension(93, 21));
        newBtn.setMinimumSize(new Dimension(93, 21));
        newBtn.setPreferredSize(new Dimension(93, 21));
        newBtn.setText(text);
        btnPanel.add(newBtn,
                new GridConstraints(
                        row, col,
                        1, 1,
                        GridConstraints.ANCHOR_CENTER,
                        GridConstraints.FILL_NONE,
                        GridConstraints.SIZEPOLICY_FIXED,
                        GridConstraints.SIZEPOLICY_FIXED,
                        new Dimension(93, 21),
                        new Dimension(39, 21),
                        new Dimension(93, 21),
                        0,
                        false
                )
        );
        return newBtn;
    }

    public void setBackgroundColor(Color color){
        this.backgroundColor = color;
        colorsChanged();
        validate();
    }
    public void setFontColor(Color color){
        this.fontColor = color;
        colorsChanged();
        validate();
    }
    public void setLblFontColor(Color color){
        this.lblFontColor = color;
        colorsChanged();
        validate();
    }



    private void colorsChanged(){
        valueHistoryBtn.setBackground(backgroundColor);
        valueHistoryBtn.setForeground(lblFontColor);

        changeDetailsBtn.setBackground(backgroundColor);
        changeDetailsBtn.setForeground(lblFontColor);

        disconnectBtn.setBackground(backgroundColor);
        disconnectBtn.setForeground(lblFontColor);

        ((JDetailsTableCellRenderer)propertyTbl.getDefaultRenderer(Object.class)).setTextColor(fontColor);
        sensorLnlBtn.setBackground(backgroundColor);
        sensorLnlBtn.setForeground(lblFontColor);

        /*switchBtn.setBackground(backgroundColor);
        switchBtn.setForeground(lblFontColor);*/
    }
}
