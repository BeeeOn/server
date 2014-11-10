package com.smarthome.emulator.simulations.forms;

import com.intellij.uiDesigner.core.GridConstraints;
import com.intellij.uiDesigner.core.GridLayoutManager;
import com.intellij.uiDesigner.core.Spacer;
import com.smarthome.emulator.design.ColorSwatch;
import com.smarthome.emulator.design.JIOTButton;
import com.smarthome.emulator.design.JIOTScrollBarUI;
import com.smarthome.emulator.design.JNumberTextField;
import com.smarthome.emulator.design.tree.JIOTTree;
import com.smarthome.emulator.design.tree.JIOTTreeCellRenderer;
import com.smarthome.emulator.design.tree.JIOTTreeNode;
import com.smarthome.emulator.design.tree.TreeIcon;
import com.smarthome.emulator.simulations.basis.Sensor;
import com.smarthome.emulator.simulations.basis.Value;
import com.smarthome.emulator.simulations.panels.SensorPanel;

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreePath;
import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;

public class SensorOptions extends JDialog {
    private JPanel contentPane;
    private JIOTButton buttonOK;
    private JIOTButton buttonCancel;
    private JIOTButton buttonApply;
    private JIOTTree sensorTree;
    private JLabel LblSensor;
    private JScrollPane sensorTreeScroller;
    private JPanel panelContent;
    private JTextField txtFieldName;
    private JNumberTextField txtFieldProtocol;
    private JNumberTextField txtFieldSignal;
    private JNumberTextField txtFieldBattery;
    private JNumberTextField txtFieldRefreshRate;
    private JIOTButton btnBackgroundColor;
    private JIOTButton btnLabelTextColor;
    private JIOTButton btnTextColor;
    private JPanel panelGeneralSettings;
    private JLabel LblGeneralSettings;
    private JLabel lblName;
    private JLabel lblProtocol;
    private JLabel lblSignal;
    private JLabel lblBattery;
    private JLabel lblRefreshRate;
    private JLabel lblSeparator;
    private JLabel lblBackgroundColor;
    private JLabel lblLabelTextColor;
    private JLabel lblTextColor;
    private JLabel lblValueSettings;
    private JLabel lblValue;
    private JLabel lblValueClass;
    private JLabel lblValueType;
    private JTextField txtFieldValue;
    private JTextField txtFieldValueClass;
    private JNumberTextField txtFieldValueType;
    private JLabel lblGenerateValue;
    private JRadioButton radBtnGenerateYes;
    private JRadioButton radBtnGenerateNo;
    private JLabel lblValueMax;
    private JNumberTextField txtFieldValueMax;
    private JLabel lblValueAvg;
    private JLabel lblValueMin;
    private JLabel lblValueDev;
    private JNumberTextField txtFieldValueMin;
    private JNumberTextField txtFieldValueAvg;
    private JNumberTextField txtFieldValueDev;
    private JPanel panelValueSettings;
    private JLabel lblWarning;
    private SensorPanel sensorPanel;
    private JIOTTreeNode rootTreeNode;
    private DefaultTreeModel sensorTreeModel;
    private Color backgroundColor;
    private Color labelTextColor;
    private Color textColor;
    private ColorSwatch swatchBackgroundColor;
    private ColorSwatch swatchLabelTextColor;
    private ColorSwatch swatchTextColor;

    private ArrayList<JTextField> txtFields = new ArrayList<>();

    public boolean isChanged() {
        return changed;
    }

    public void setChanged(boolean changed) {
        this.changed = changed;
    }

    private boolean changed = false;

    private boolean fillingFields = false;

    private static final int COLORS_SWATCH_SIZE = 16;
    private static final int GENERAL_SETTINGS = 1;
    private static final int VALUE_SETTINGS = 2;

    private static final int SHOW_GENERAL_SETTINGS = 10;
    private static final int SHOW_VALUE_SETTINGS = 11;

    //TODO export sensor properties file
    public SensorOptions(SensorPanel sensorPanel) {
        $$$setupUI$$$();
        if (sensorPanel != null && sensorPanel.getSensor() != null)
            setTitle(sensorPanel.getSensor().getName() + " - Sensor Details");
        else
            setTitle("Sensor Details");
        this.sensorPanel = sensorPanel;
        setContentPane(contentPane);
        setModal(false);
        setBounds(345, 100, 550, 360);
        getRootPane().setDefaultButton(buttonOK);

        UIModifications();
        fillGeneralSettings();
        initializeTree();
        createListeners();
    }


    private void enableGenerateValueFields() {
        txtFieldValueMax.setEnabled(true);
        txtFieldValueMin.setEnabled(true);
        txtFieldValueAvg.setEnabled(true);
        txtFieldValueDev.setEnabled(true);
    }

    private void disableGenerateValueFields() {
        txtFieldValueMax.setEnabled(false);
        txtFieldValueMin.setEnabled(false);
        txtFieldValueAvg.setEnabled(false);
        txtFieldValueDev.setEnabled(false);
    }

    private void initializeTree() {
        //add general sensor settings
        JIOTTreeNode generalNode = new JIOTTreeNode(TreeIcon.SETTINGS, "General Settings", false);
        rootTreeNode.add(generalNode);
        //add values folder
        JIOTTreeNode valuesNode = new JIOTTreeNode(TreeIcon.FOLDER, "Values", true);
        rootTreeNode.add(valuesNode);
        //add values from sensor
        if (sensorPanel.getSensor() != null && sensorPanel.getSensor().getValues() != null) {
            JIOTTreeNode valueNode = null;
            //TODO add all value types
            for (Value value : sensorPanel.getSensor().getValues()) {
                switch (value.getType().shortValue()) {
                    case (short) 0:
                        valueNode = new JIOTTreeNode(TreeIcon.TEMPERATURE, value, false);
                        break;
                    case (short) 1:
                        valueNode = new JIOTTreeNode(TreeIcon.HUMIDITY, value, false);
                        break;
                    case (short) 2:
                        valueNode = new JIOTTreeNode(TreeIcon.PRESSURE, value, false);
                        break;
                    case (short) 3:
                        valueNode = new JIOTTreeNode(TreeIcon.ACTUATOR, value, false);
                        break;
                    case (short) 4:
                        valueNode = new JIOTTreeNode(TreeIcon.ACTUATOR, value, false);
                        break;
                    default:
                        valueNode = new JIOTTreeNode(TreeIcon.UNKNOWN, value, false);
                        break;
                }
                valuesNode.add(valueNode);
            }
        }
        sensorTreeModel.reload();
        sensorTree.setSelectionPath(new TreePath((((JIOTTreeNode) sensorTreeModel.getRoot()).getFirstLeaf()).getPath()));
    }

    private void fillGeneralSettings() {
        fillingFields = true;
        Sensor sensor = sensorPanel.getSensor();
        //fill fields
        txtFieldName.setText(sensor.getName());
        txtFieldProtocol.setText(String.valueOf(sensor.getProtocol()));
        txtFieldSignal.setText(String.valueOf(sensor.getSignal()));
        txtFieldBattery.setText(String.valueOf(sensor.getBattery()));
        txtFieldRefreshRate.setText(String.valueOf(sensor.getRefreshRate()));
        //background color string
        String[] bcString = sensor.getBackgroundColorString().split(",");
        backgroundColor = new Color(Integer.valueOf(bcString[0]), Integer.valueOf(bcString[1]), Integer.valueOf(bcString[2]));
        swatchBackgroundColor = new ColorSwatch(COLORS_SWATCH_SIZE, backgroundColor);
        btnBackgroundColor.setIcon(swatchBackgroundColor);
        btnBackgroundColor.setBorder(new EmptyBorder(0, 0, 0, 0));
        //label text color string
        String[] ltString = sensor.getLblFontColorString().split(",");
        labelTextColor = new Color(Integer.valueOf(ltString[0]), Integer.valueOf(ltString[1]), Integer.valueOf(ltString[2]));
        swatchLabelTextColor = new ColorSwatch(COLORS_SWATCH_SIZE, labelTextColor);
        btnLabelTextColor.setIcon(swatchLabelTextColor);
        btnLabelTextColor.setBorder(new EmptyBorder(0, 0, 0, 0));
        //text color string
        String[] tcString = sensor.getFontColorString().split(",");
        textColor = new Color(Integer.valueOf(tcString[0]), Integer.valueOf(tcString[1]), Integer.valueOf(tcString[2]));
        swatchTextColor = new ColorSwatch(COLORS_SWATCH_SIZE, textColor);
        btnTextColor.setIcon(swatchTextColor);
        btnTextColor.setBorder(new EmptyBorder(0, 0, 0, 0));
        //TODO add ability to change icon
        fillingFields = false;
    }

    private void fireWarning(String warning) {
        lblWarning.setVisible(true);
        lblWarning.setText(warning);
    }

    private boolean validateChanges(int validate_option) {
        lblWarning.setText("");
        lblWarning.setVisible(false);
        if (validate_option == GENERAL_SETTINGS) {
            if (emptyOrNull(txtFieldName.getText())) {
                fireWarning("Sensor name field is empty!");
                return false;
            }
            if (emptyOrNull(txtFieldProtocol.getText())) {
                fireWarning("Protocol field is empty!");
                return false;
            }
            if (emptyOrNull(txtFieldSignal.getText())) {
                fireWarning("Signal field is empty!");
                return false;
            }
            if (emptyOrNull(txtFieldBattery.getText())) {
                fireWarning("Battery field is empty!");
                return false;
            }
            if (emptyOrNull(txtFieldRefreshRate.getText())) {
                fireWarning("Refresh rate field is empty!");
                return false;
            }
        } else if (validate_option == VALUE_SETTINGS) {
            if (emptyOrNull(txtFieldValue.getText())) {
                fireWarning("Value field is empty!");
                return false;
            }
            if (emptyOrNull(txtFieldValueClass.getText())) {
                fireWarning("Value class field is empty!");
                return false;
            }
            if (emptyOrNull(txtFieldValueType.getText())) {
                fireWarning("Value type field is empty!");
                return false;
            }
            if (radBtnGenerateYes.isSelected() && (!txtFieldValueType.getText().trim().equals("4") && !txtFieldValueType.getText().trim().equals("3"))) {
                if (emptyOrNull(txtFieldValueMax.getText())) {
                    fireWarning("Value max field is empty!");
                    return false;
                }
                if (emptyOrNull(txtFieldValueMin.getText())) {
                    fireWarning("Value max field is empty!");
                    return false;
                }
                if (emptyOrNull(txtFieldValueAvg.getText())) {
                    fireWarning("Value max field is empty!");
                    return false;
                }
                if (emptyOrNull(txtFieldValueDev.getText())) {
                    fireWarning("Value max field is empty!");
                    return false;
                }
            }
        }
        return true;
    }

    private boolean emptyOrNull(String text) {
        if (text == null || text.trim().equals(""))
            return true;
        return false;
    }

    private boolean onApply() {
        if (panelGeneralSettings.isVisible()) {
            if (!validateChanges(GENERAL_SETTINGS)) {
                return false;
            }
            if (!applyChanges(GENERAL_SETTINGS)) {
                return false;
            }
        } else if (panelValueSettings.isVisible()) {
            if (!validateChanges(VALUE_SETTINGS)) {
                return false;
            }
            if (!applyChanges(VALUE_SETTINGS)) {
                return false;
            }
        }
        finalizeApplying();
        return true;
    }

    private boolean applyChanges(int setting_no) {
        if (setting_no == GENERAL_SETTINGS) {
            //update Sensor object variables
            Sensor sensor = sensorPanel.getSensor();
            sensor.setName(txtFieldName.getText());
            sensor.setProtocol(Short.valueOf(txtFieldProtocol.getText()));
            Short signal = Short.valueOf(txtFieldSignal.getText());
            if (signal > 100) {
                fireWarning("Signal not in range 0-100");
                return false;
            }
            sensor.setSignal(signal);
            Short battery = Short.valueOf(txtFieldBattery.getText());
            if (battery > 100) {
                fireWarning("Battery not in range 0-100");
                return false;
            }
            sensor.setBattery(battery);
            int refreshRate = Integer.valueOf(txtFieldRefreshRate.getText());
            if (refreshRate == 0) {
                fireWarning("Refresh rate cannot be set to 0");
                return false;
            }
            sensor.setRefreshRate(refreshRate);
            sensor.setBackgroundColorString(String.valueOf(backgroundColor.getRed()) + "," + String.valueOf(backgroundColor.getGreen()) + "," + String.valueOf(backgroundColor.getBlue()));
            sensor.setLblFontColorString(String.valueOf(labelTextColor.getRed()) + "," + String.valueOf(labelTextColor.getGreen()) + "," + String.valueOf(labelTextColor.getBlue()));
            sensor.setFontColorString(String.valueOf(textColor.getRed()) + "," + String.valueOf(textColor.getGreen()) + "," + String.valueOf(textColor.getBlue()));
            //update SensorPanel object variables

            sensorPanel.setBackgroundColor(new Color(backgroundColor.getRed(), backgroundColor.getGreen(), backgroundColor.getBlue()));
            sensorPanel.setLblFontColor(new Color(labelTextColor.getRed(), labelTextColor.getGreen(), labelTextColor.getBlue()));
            sensorPanel.setFontColor(new Color(textColor.getRed(), textColor.getGreen(), textColor.getBlue()));
        } else if (setting_no == VALUE_SETTINGS) {
            JIOTTreeNode node = (JIOTTreeNode) sensorTree.getLastSelectedPathComponent();
                /* if nothing is selected */
            if (node == null) {
                fireWarning("Can not retrieve selected value!");
                return false;
            }
            Value value = (Value) node.getUserObject();
            /*//nothing to change on actuators sensor
            if (value.getType().shortValue() == (short) 3) {
                fireWarning("Internal error- trying to update actuator's sensor");
                return false;
            }*/
            //if actuator
            if (value.getType().shortValue() == (short) 4 || value.getType().shortValue() == (short) 3) {
                //check value
                if (txtFieldValue.getText().trim().equalsIgnoreCase("true") || txtFieldValue.getText().trim().equalsIgnoreCase("false")) {
                    value.nextValue(Boolean.valueOf(txtFieldValue.getText()));
                } else {
                    fireWarning("Wrong current value format");
                    return false;
                }
                if (radBtnGenerateYes.isSelected()) {
                    value.setGenerateValue(true);
                } else {
                    value.setGenerateValue(false);
                }
            } else { //if other sensor
                //check value
                try {
                    Float v = Float.valueOf(txtFieldValue.getText());
                    value.nextValue(v);
                } catch (NumberFormatException e) {
                    fireWarning("Wrong current value format");
                    return false;
                }
                if (radBtnGenerateYes.isSelected()) {
                    try {
                        Float max = Float.valueOf(txtFieldValueMax.getText());
                        Float min = Float.valueOf(txtFieldValueMin.getText());
                        Float avg = Float.valueOf(txtFieldValueAvg.getText());
                        Float dev = Float.valueOf(txtFieldValueDev.getText());
                        value.setMax(max);
                        value.setMax(min);
                        value.setAvg(avg);
                        value.setDev(dev);
                    } catch (NumberFormatException e) {
                        fireWarning("Wrong generator values format!");
                        return false;
                    }
                    value.setGenerateValue(true);
                } else {
                    value.setGenerateValue(false);
                }
            }
            sensorPanel.getSensor().updateDetails();
        }
        return true;
    }

    private void finalizeApplying() {
        setChanged(false);
        buttonApply.setActivity(false);
        lblWarning.setText("");
        lblWarning.setVisible(false);
    }

    private void onOK() {
        if (isChanged())
            onApply();
        dispose();
    }

    private int settingsChangesDialog() {
        Object[] options = {"Apply", "No"};
        int dialogResult = JOptionPane.showOptionDialog(null, "Would you like to apply changes?", "Warning",
                JOptionPane.DEFAULT_OPTION, JOptionPane.WARNING_MESSAGE,
                null, options, options[0]);
        return dialogResult;
    }

    private void onCancel() {
        if (isChanged()) {
            settingsChangesDialog();
        }
        dispose();
    }

    private void UIModifications() {
        rootTreeNode = new JIOTTreeNode(TreeIcon.SENSOR, "Sensor", true);
        sensorTreeModel = new DefaultTreeModel(rootTreeNode);
        sensorTree.setModel(sensorTreeModel);
        sensorTree.setCellRenderer(new JIOTTreeCellRenderer());
        sensorTree.putClientProperty("JTree.lineStyle", "None");
        sensorTree.setRootVisible(true);
        sensorTree.setBorder(new EmptyBorder(4, 4, 4, 4));
        sensorTree.setRowHeight(20);
        sensorTreeScroller.setViewportView(sensorTree);

        txtFields.add(txtFieldName);
        txtFields.add(txtFieldProtocol);
        txtFields.add(txtFieldSignal);
        txtFields.add(txtFieldBattery);
        txtFields.add(txtFieldRefreshRate);
        txtFields.add(txtFieldValue);
        txtFields.add(txtFieldValueClass);
        txtFields.add(txtFieldValueType);
        txtFields.add(txtFieldValueMax);
        txtFields.add(txtFieldValueMin);
        txtFields.add(txtFieldValueAvg);
        txtFields.add(txtFieldValueDev);

        Color borderColor = new Color(220, 220, 220);
        for (JTextField txtField : txtFields) {
            txtField.setBorder(BorderFactory.createLineBorder(borderColor, 1));
        }
        //set panels to be invisible
        //panelGeneralSettings.setVisible(false);
        panelValueSettings.setVisible(false);

        txtFieldValueType.setEnabled(false);
        txtFieldValueClass.setEnabled(false);

        sensorTreeScroller.getVerticalScrollBar().setUI(new JIOTScrollBarUI());
    }

    private void createListeners() {
        btnBackgroundColor.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                Color newColor = JColorChooser.showDialog(SensorOptions.this, "Choose sensor background color", backgroundColor);
                if (newColor != null) {
                    backgroundColor = newColor;
                    swatchBackgroundColor.setColor(backgroundColor);
                    settingsChanged();
                    validate();
                }
            }
        });
        btnLabelTextColor.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                Color newColor = JColorChooser.showDialog(SensorOptions.this, "Choose sensor background color", labelTextColor);
                if (newColor != null) {
                    labelTextColor = newColor;
                    swatchLabelTextColor.setColor(labelTextColor);
                    settingsChanged();
                    validate();
                }
            }
        });
        btnTextColor.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                Color newColor = JColorChooser.showDialog(SensorOptions.this, "Choose sensor background color", textColor);
                if (newColor != null) {
                    textColor = newColor;
                    swatchTextColor.setColor(textColor);
                    settingsChanged();
                    validate();
                }
            }
        });
        //tree selection change listener
        sensorTree.addTreeSelectionListener(new TreeSelectionListener() {
            public void valueChanged(TreeSelectionEvent e) {
                JIOTTreeNode node = (JIOTTreeNode)
                        sensorTree.getLastSelectedPathComponent();
                /* if nothing is selected */
                if (node == null) return;
                if (node.isLeaf()) {
                    if (isChanged()) {
                        int dialogResult = settingsChangesDialog();
                        if (dialogResult == 0) {
                            if (!onApply())
                                return;
                        } else if (dialogResult == 1) {
                            finalizeApplying();
                        }
                    }
                    if (node.getUserObject() instanceof Value) {
                        showSettings(SHOW_VALUE_SETTINGS);
                        fillValuePanel((Value) node.getUserObject());
                        /*if (((Value) node.getUserObject()).getType().shortValue() == (short) 3) {
                            actuatorsSensorSetEnabled(false);
                        } else {
                            actuatorsSensorSetEnabled(true);
                        }*/
                    } else {
                        showSettings(SHOW_GENERAL_SETTINGS);
                        fillGeneralSettings();
                    }
                }
            }
        });
        buttonOK.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                onOK();
            }
        });
        buttonCancel.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                onCancel();
            }
        });
        buttonApply.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (buttonApply.isActive())
                    onApply();
            }
        });

        setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                onCancel();
            }
        });

        contentPane.registerKeyboardAction(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                onCancel();
            }
        }, KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE, 0), JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT);
        //show that something has changed after clicking on radio button
        MouseAdapter radBtnsChangeListener = new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                settingsChanged();
            }
        };
        radBtnGenerateYes.addMouseListener(radBtnsChangeListener);
        radBtnGenerateNo.addMouseListener(radBtnsChangeListener);
        //when radio buttons change, enable or disable generator text fields
        radBtnGenerateYes.addItemListener(new ItemListener() {
            @Override
            public void itemStateChanged(ItemEvent e) {
                if (e.getStateChange() == ItemEvent.SELECTED) {
                    enableGenerateValueFields();
                }
            }
        });
        radBtnGenerateNo.addItemListener(new ItemListener() {
            @Override
            public void itemStateChanged(ItemEvent e) {
                if (e.getStateChange() == ItemEvent.SELECTED) {
                    disableGenerateValueFields();
                }
            }
        });
        //text fields change
        DocumentListener txtFieldsChangeListener = new DocumentListener() {
            public void changedUpdate(DocumentEvent documentEvent) {
                if (!fillingFields)
                    settingsChanged();
            }

            public void insertUpdate(DocumentEvent documentEvent) {
                if (!fillingFields)
                    settingsChanged();
            }

            public void removeUpdate(DocumentEvent documentEvent) {
                if (!fillingFields)
                    settingsChanged();
            }
        };
        //add change listeners for all text fields
        for (JTextField txtField : txtFields) {
            txtField.getDocument().addDocumentListener(txtFieldsChangeListener);
        }
    }

    private void actuatorsSensorSetEnabled(boolean b) {
        txtFieldValue.setEnabled(b);
        radBtnGenerateNo.setEnabled(b);
        radBtnGenerateYes.setEnabled(b);
    }

    private void settingsChanged() {
        setChanged(true);
        buttonApply.setActivity(true);
    }

    private void fillValuePanel(Value value) {
        fillingFields = true;
        /*if (value.getType().shortValue() == (short) 3)
            txtFieldValue.setEnabled(false);*/
        txtFieldValue.setText(String.valueOf(value.getValue()));
        txtFieldValueClass.setText(value.getValueClass());
        txtFieldValueType.setText(String.valueOf(value.getType()));
        if (value.isGenerateValue()) {

            radBtnGenerateYes.setSelected(true);
            radBtnGenerateNo.setSelected(false);
            enableGenerateValueFields();
        } else {
            radBtnGenerateYes.setSelected(false);
            radBtnGenerateNo.setSelected(true);
            disableGenerateValueFields();
        }
        if (value.getMax() != null)
            txtFieldValueMax.setText(String.valueOf(value.getMax()));
        else
            txtFieldValueMax.setText("");
        if (value.getMin() != null)
            txtFieldValueMin.setText(String.valueOf(value.getMin()));
        else
            txtFieldValueMin.setText("");
        if (value.getAvg() != null)
            txtFieldValueAvg.setText(String.valueOf(value.getAvg()));
        else
            txtFieldValueAvg.setText("");
        if (value.getDev() != null)
            txtFieldValueDev.setText(String.valueOf(value.getDev()));
        else
            txtFieldValueDev.setText("");
        fillingFields = false;
    }

    private void showSettings(int settings_no) {
        disableAllSettingsPanel();
        if (settings_no == SHOW_GENERAL_SETTINGS)
            panelGeneralSettings.setVisible(true);
        else if (settings_no == SHOW_VALUE_SETTINGS) {
            panelValueSettings.setVisible(true);

        }

    }

    private void disableAllSettingsPanel() {
        Component[] components = panelContent.getComponents();
        for (Component component : components) {
            component.setVisible(false);
        }
    }

    private void clearAllValueFields() {
        txtFieldValue.setText("");
        txtFieldValueClass.setText("");
        txtFieldValueType.setText("");
        txtFieldValueMax.setText("");
        txtFieldValueMin.setText("");
        txtFieldValueAvg.setText("");
        txtFieldValueDev.setText("");
    }

    private void createUIComponents() {
        //main dialog buttons
        buttonOK = new JIOTButton("OK", 115, 24, true);
        buttonApply = new JIOTButton("Apply", 115, 24, false);
        buttonCancel = new JIOTButton("Cancel", 115, 24, false);
        //sensor tree
        sensorTree = new JIOTTree();
        //other fields
        txtFieldProtocol = new JNumberTextField(4, JNumberTextField.INTEGER_NUMBER, false);
        txtFieldSignal = new JNumberTextField(4, JNumberTextField.INTEGER_NUMBER, false);
        txtFieldBattery = new JNumberTextField(4, JNumberTextField.INTEGER_NUMBER, false);
        txtFieldRefreshRate = new JNumberTextField(4, JNumberTextField.INTEGER_NUMBER, false);

        btnBackgroundColor = new JIOTButton("", 16, 16, true);
        btnBackgroundColor.setBackground(new Color(-1));
        btnLabelTextColor = new JIOTButton("", 16, 16, true);
        btnLabelTextColor.setBackground(new Color(-1));
        btnTextColor = new JIOTButton("", 16, 16, true);
        btnTextColor.setBackground(new Color(-1));

        txtFieldValueType = new JNumberTextField(4, JNumberTextField.INTEGER_NUMBER, false);

        txtFieldValueMax = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER, true);
        txtFieldValueMin = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER, true);
        txtFieldValueAvg = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER, true);
        txtFieldValueDev = new JNumberTextField(6, JNumberTextField.DECIMAL_NUMBER, true);

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
        contentPane = new JPanel();
        contentPane.setLayout(new GridLayoutManager(3, 1, new Insets(10, 10, 10, 10), -1, -1));
        contentPane.setBackground(new Color(-1));
        final JPanel panel1 = new JPanel();
        panel1.setLayout(new GridLayoutManager(1, 1, new Insets(0, 0, 0, 0), -1, -1));
        panel1.setBackground(new Color(-1));
        contentPane.add(panel1, new GridConstraints(2, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, 1, null, null, null, 0, false));
        final JPanel panel2 = new JPanel();
        panel2.setLayout(new GridLayoutManager(1, 4, new Insets(0, 0, 0, 0), -1, -1));
        panel2.setBackground(new Color(-1));
        panel1.add(panel2, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, null, null, null, 0, false));
        buttonOK.setText("OK");
        panel2.add(buttonOK, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        buttonCancel.setText("Cancel");
        panel2.add(buttonCancel, new GridConstraints(0, 2, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final Spacer spacer1 = new Spacer();
        panel2.add(spacer1, new GridConstraints(0, 3, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_WANT_GROW, 1, null, null, null, 0, false));
        buttonApply.setText("Apply");
        panel2.add(buttonApply, new GridConstraints(0, 1, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JPanel panel3 = new JPanel();
        panel3.setLayout(new GridLayoutManager(2, 3, new Insets(0, 0, 0, 0), -1, -1));
        panel3.setBackground(new Color(-1));
        contentPane.add(panel3, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, null, null, null, 0, false));
        LblSensor = new JLabel();
        LblSensor.setBackground(new Color(-1));
        LblSensor.setFont(new Font("Segoe UI", Font.PLAIN, 28));
        LblSensor.setForeground(new Color(-13127169));
        LblSensor.setText("Sensor Details");
        panel3.add(LblSensor, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        sensorTreeScroller = new JScrollPane();
        sensorTreeScroller.setBackground(new Color(-1));
        sensorTreeScroller.setHorizontalScrollBarPolicy(31);
        panel3.add(sensorTreeScroller, new GridConstraints(1, 0, 1, 2, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_VERTICAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_WANT_GROW, new Dimension(200, -1), new Dimension(200, -1), new Dimension(200, -1), 0, false));
        sensorTreeScroller.setBorder(BorderFactory.createTitledBorder(BorderFactory.createLineBorder(new Color(-1052689)), null));
        sensorTreeScroller.setViewportView(sensorTree);
        final Spacer spacer2 = new Spacer();
        panel3.add(spacer2, new GridConstraints(0, 1, 1, 2, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_WANT_GROW, 1, null, null, null, 0, false));
        panelContent = new JPanel();
        panelContent.setLayout(new GridLayoutManager(3, 2, new Insets(0, 0, 0, 0), -1, -1));
        panelContent.setBackground(new Color(-1));
        panel3.add(panelContent, new GridConstraints(1, 2, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, null, null, null, 0, false));
        panelGeneralSettings = new JPanel();
        panelGeneralSettings.setLayout(new GridLayoutManager(11, 3, new Insets(0, 10, 0, 10), -1, -1));
        panelGeneralSettings.setBackground(new Color(-1));
        panelContent.add(panelGeneralSettings, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_NORTHWEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        panelGeneralSettings.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEmptyBorder(), null));
        LblGeneralSettings = new JLabel();
        LblGeneralSettings.setBackground(new Color(-1));
        LblGeneralSettings.setFont(new Font("Segoe UI", Font.BOLD, 18));
        LblGeneralSettings.setForeground(new Color(-8224126));
        LblGeneralSettings.setText("General Settings");
        panelGeneralSettings.add(LblGeneralSettings, new GridConstraints(0, 0, 1, 2, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final Spacer spacer3 = new Spacer();
        panelGeneralSettings.add(spacer3, new GridConstraints(10, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_VERTICAL, 1, GridConstraints.SIZEPOLICY_WANT_GROW, null, null, null, 0, false));
        lblName = new JLabel();
        lblName.setBackground(new Color(-1));
        lblName.setForeground(new Color(-8224126));
        lblName.setText("Name:");
        panelGeneralSettings.add(lblName, new GridConstraints(2, 0, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        txtFieldName = new JTextField();
        txtFieldName.setBackground(new Color(-1));
        txtFieldName.setCaretColor(new Color(-8224126));
        txtFieldName.setForeground(new Color(-8224126));
        txtFieldName.setMargin(new Insets(0, 5, 0, 10));
        txtFieldName.setSelectionColor(new Color(-5841665));
        panelGeneralSettings.add(txtFieldName, new GridConstraints(2, 1, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(150, -1), null, 0, false));
        final Spacer spacer4 = new Spacer();
        panelGeneralSettings.add(spacer4, new GridConstraints(0, 2, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_WANT_GROW, 1, null, null, null, 0, false));
        lblProtocol = new JLabel();
        lblProtocol.setBackground(new Color(-1));
        lblProtocol.setForeground(new Color(-8224126));
        lblProtocol.setText("Protocol:");
        panelGeneralSettings.add(lblProtocol, new GridConstraints(3, 0, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        lblSignal = new JLabel();
        lblSignal.setBackground(new Color(-1));
        lblSignal.setForeground(new Color(-8224126));
        lblSignal.setText("Signal:");
        panelGeneralSettings.add(lblSignal, new GridConstraints(4, 0, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        lblBattery = new JLabel();
        lblBattery.setBackground(new Color(-1));
        lblBattery.setForeground(new Color(-8224126));
        lblBattery.setText("Battery:");
        panelGeneralSettings.add(lblBattery, new GridConstraints(5, 0, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        txtFieldProtocol.setBackground(new Color(-1));
        txtFieldProtocol.setCaretColor(new Color(-8224126));
        txtFieldProtocol.setDisabledTextColor(new Color(-8224126));
        txtFieldProtocol.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        txtFieldProtocol.setForeground(new Color(-8224126));
        txtFieldProtocol.setSelectionColor(new Color(-5841665));
        txtFieldProtocol.setText("");
        txtFieldProtocol.setToolTipText("Integer number");
        panelGeneralSettings.add(txtFieldProtocol, new GridConstraints(3, 1, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(150, -1), null, 0, false));
        txtFieldSignal.setBackground(new Color(-1));
        txtFieldSignal.setCaretColor(new Color(-8224126));
        txtFieldSignal.setDisabledTextColor(new Color(-8224126));
        txtFieldSignal.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        txtFieldSignal.setForeground(new Color(-8224126));
        txtFieldSignal.setSelectionColor(new Color(-5841665));
        txtFieldSignal.setText("");
        txtFieldSignal.setToolTipText("Only one or double digit number.");
        panelGeneralSettings.add(txtFieldSignal, new GridConstraints(4, 1, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(150, -1), null, 0, false));
        txtFieldBattery.setBackground(new Color(-1));
        txtFieldBattery.setCaretColor(new Color(-8224126));
        txtFieldBattery.setDisabledTextColor(new Color(-8224126));
        txtFieldBattery.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        txtFieldBattery.setForeground(new Color(-8224126));
        txtFieldBattery.setSelectionColor(new Color(-5841665));
        txtFieldBattery.setText("");
        txtFieldBattery.setToolTipText("Only one or double digit number.");
        panelGeneralSettings.add(txtFieldBattery, new GridConstraints(5, 1, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(150, -1), null, 0, false));
        lblRefreshRate = new JLabel();
        lblRefreshRate.setBackground(new Color(-1));
        lblRefreshRate.setForeground(new Color(-8224126));
        lblRefreshRate.setText("Refresh Rate:");
        panelGeneralSettings.add(lblRefreshRate, new GridConstraints(6, 0, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        txtFieldRefreshRate.setBackground(new Color(-1));
        txtFieldRefreshRate.setCaretColor(new Color(-8224126));
        txtFieldRefreshRate.setDisabledTextColor(new Color(-8224126));
        txtFieldRefreshRate.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        txtFieldRefreshRate.setForeground(new Color(-8224126));
        txtFieldRefreshRate.setSelectionColor(new Color(-5841665));
        txtFieldRefreshRate.setText("");
        txtFieldRefreshRate.setToolTipText("Integer number.");
        panelGeneralSettings.add(txtFieldRefreshRate, new GridConstraints(6, 1, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(150, -1), null, 0, false));
        lblSeparator = new JLabel();
        lblSeparator.setBackground(new Color(-1));
        lblSeparator.setText("");
        panelGeneralSettings.add(lblSeparator, new GridConstraints(1, 0, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        lblBackgroundColor = new JLabel();
        lblBackgroundColor.setBackground(new Color(-1));
        lblBackgroundColor.setForeground(new Color(-8224126));
        lblBackgroundColor.setText("Background color:");
        panelGeneralSettings.add(lblBackgroundColor, new GridConstraints(7, 0, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        btnBackgroundColor.setText("");
        panelGeneralSettings.add(btnBackgroundColor, new GridConstraints(7, 1, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        lblLabelTextColor = new JLabel();
        lblLabelTextColor.setBackground(new Color(-1));
        lblLabelTextColor.setForeground(new Color(-8224126));
        lblLabelTextColor.setText("Label text color:");
        panelGeneralSettings.add(lblLabelTextColor, new GridConstraints(8, 0, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        lblTextColor = new JLabel();
        lblTextColor.setBackground(new Color(-1));
        lblTextColor.setForeground(new Color(-8224126));
        lblTextColor.setText("Text color:");
        panelGeneralSettings.add(lblTextColor, new GridConstraints(9, 0, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        btnLabelTextColor.setText("");
        panelGeneralSettings.add(btnLabelTextColor, new GridConstraints(8, 1, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        btnTextColor.setText("");
        panelGeneralSettings.add(btnTextColor, new GridConstraints(9, 1, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final Spacer spacer5 = new Spacer();
        panelContent.add(spacer5, new GridConstraints(0, 1, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_WANT_GROW, 1, null, null, null, 0, false));
        final Spacer spacer6 = new Spacer();
        panelContent.add(spacer6, new GridConstraints(2, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_VERTICAL, 1, GridConstraints.SIZEPOLICY_WANT_GROW, null, null, null, 0, false));
        panelValueSettings = new JPanel();
        panelValueSettings.setLayout(new GridLayoutManager(9, 5, new Insets(0, 10, 0, 10), -1, -1));
        panelValueSettings.setBackground(new Color(-1));
        panelContent.add(panelValueSettings, new GridConstraints(1, 0, 1, 1, GridConstraints.ANCHOR_NORTHWEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, null, null, null, 0, false));
        panelValueSettings.setBorder(BorderFactory.createTitledBorder(BorderFactory.createEmptyBorder(), null));
        lblValueSettings = new JLabel();
        lblValueSettings.setBackground(new Color(-1));
        lblValueSettings.setFont(new Font("Segoe UI", Font.BOLD, 18));
        lblValueSettings.setForeground(new Color(-8224126));
        lblValueSettings.setText("Value Settings");
        panelValueSettings.add(lblValueSettings, new GridConstraints(0, 0, 1, 2, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final Spacer spacer7 = new Spacer();
        panelValueSettings.add(spacer7, new GridConstraints(0, 4, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_WANT_GROW, 1, null, null, null, 0, false));
        final Spacer spacer8 = new Spacer();
        panelValueSettings.add(spacer8, new GridConstraints(8, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_VERTICAL, 1, GridConstraints.SIZEPOLICY_WANT_GROW, null, new Dimension(82, 14), null, 0, false));
        final JLabel label1 = new JLabel();
        label1.setBackground(new Color(-1));
        label1.setText("");
        panelValueSettings.add(label1, new GridConstraints(1, 0, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        lblValue = new JLabel();
        lblValue.setBackground(new Color(-1));
        lblValue.setFont(new Font("Segoe UI", Font.BOLD, 14));
        lblValue.setForeground(new Color(-8224126));
        lblValue.setText("Current Value:");
        panelValueSettings.add(lblValue, new GridConstraints(2, 0, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(82, 20), null, 0, false));
        lblValueClass = new JLabel();
        lblValueClass.setBackground(new Color(-1));
        lblValueClass.setForeground(new Color(-8224126));
        lblValueClass.setText("Class:");
        panelValueSettings.add(lblValueClass, new GridConstraints(3, 0, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(82, 16), null, 0, false));
        lblValueType = new JLabel();
        lblValueType.setBackground(new Color(-1));
        lblValueType.setForeground(new Color(-8224126));
        lblValueType.setText("Type:");
        panelValueSettings.add(lblValueType, new GridConstraints(4, 0, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(82, 16), null, 0, false));
        txtFieldValue = new JTextField();
        txtFieldValue.setBackground(new Color(-1));
        txtFieldValue.setCaretColor(new Color(-8224126));
        txtFieldValue.setForeground(new Color(-8224126));
        txtFieldValue.setMargin(new Insets(0, 5, 0, 10));
        txtFieldValue.setSelectionColor(new Color(-5841665));
        panelValueSettings.add(txtFieldValue, new GridConstraints(2, 1, 1, 3, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(150, -1), null, 0, false));
        txtFieldValueClass = new JTextField();
        txtFieldValueClass.setBackground(new Color(-1));
        txtFieldValueClass.setCaretColor(new Color(-8224126));
        txtFieldValueClass.setForeground(new Color(-8224126));
        txtFieldValueClass.setMargin(new Insets(0, 5, 0, 10));
        txtFieldValueClass.setSelectionColor(new Color(-5841665));
        panelValueSettings.add(txtFieldValueClass, new GridConstraints(3, 1, 1, 3, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(150, -1), null, 0, false));
        txtFieldValueType.setBackground(new Color(-1));
        txtFieldValueType.setCaretColor(new Color(-8224126));
        txtFieldValueType.setDisabledTextColor(new Color(-5841665));
        txtFieldValueType.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        txtFieldValueType.setForeground(new Color(-8224126));
        txtFieldValueType.setSelectionColor(new Color(-5841665));
        txtFieldValueType.setText("");
        txtFieldValueType.setToolTipText("Only one or double digit number.");
        panelValueSettings.add(txtFieldValueType, new GridConstraints(4, 1, 1, 3, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(150, -1), null, 0, false));
        lblGenerateValue = new JLabel();
        lblGenerateValue.setBackground(new Color(-1));
        lblGenerateValue.setForeground(new Color(-8224126));
        lblGenerateValue.setText("Generate value:");
        panelValueSettings.add(lblGenerateValue, new GridConstraints(5, 0, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        radBtnGenerateYes = new JRadioButton();
        radBtnGenerateYes.setBackground(new Color(-1));
        radBtnGenerateYes.setForeground(new Color(-8224126));
        radBtnGenerateYes.setIcon(new ImageIcon(getClass().getResource("/img/radioButtonNotSelected.png")));
        radBtnGenerateYes.setPressedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        radBtnGenerateYes.setSelected(true);
        radBtnGenerateYes.setSelectedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        radBtnGenerateYes.setText("Yes");
        panelValueSettings.add(radBtnGenerateYes, new GridConstraints(5, 1, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(-1, 18), new Dimension(-1, 18), new Dimension(-1, 18), 0, false));
        radBtnGenerateNo = new JRadioButton();
        radBtnGenerateNo.setBackground(new Color(-1));
        radBtnGenerateNo.setForeground(new Color(-8224126));
        radBtnGenerateNo.setIcon(new ImageIcon(getClass().getResource("/img/radioButtonNotSelected.png")));
        radBtnGenerateNo.setPressedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        radBtnGenerateNo.setSelected(false);
        radBtnGenerateNo.setSelectedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        radBtnGenerateNo.setText("No");
        panelValueSettings.add(radBtnGenerateNo, new GridConstraints(5, 2, 1, 2, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(-1, 18), new Dimension(-1, 18), new Dimension(-1, 18), 0, false));
        lblValueMax = new JLabel();
        lblValueMax.setBackground(new Color(-1));
        lblValueMax.setForeground(new Color(-8224126));
        lblValueMax.setText("Max:");
        panelValueSettings.add(lblValueMax, new GridConstraints(6, 0, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        txtFieldValueMax.setBackground(new Color(-1));
        txtFieldValueMax.setCaretColor(new Color(-8224126));
        txtFieldValueMax.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        txtFieldValueMax.setForeground(new Color(-8224126));
        txtFieldValueMax.setSelectionColor(new Color(-5841665));
        txtFieldValueMax.setText("");
        txtFieldValueMax.setToolTipText("Only one or double digit number.");
        panelValueSettings.add(txtFieldValueMax, new GridConstraints(6, 1, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        lblValueMin = new JLabel();
        lblValueMin.setBackground(new Color(-1));
        lblValueMin.setForeground(new Color(-8224126));
        lblValueMin.setText("Min:");
        panelValueSettings.add(lblValueMin, new GridConstraints(6, 2, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        txtFieldValueMin.setBackground(new Color(-1));
        txtFieldValueMin.setCaretColor(new Color(-8224126));
        txtFieldValueMin.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        txtFieldValueMin.setForeground(new Color(-8224126));
        txtFieldValueMin.setSelectionColor(new Color(-5841665));
        txtFieldValueMin.setText("");
        txtFieldValueMin.setToolTipText("Only one or double digit number.");
        panelValueSettings.add(txtFieldValueMin, new GridConstraints(6, 3, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        lblValueAvg = new JLabel();
        lblValueAvg.setBackground(new Color(-1));
        lblValueAvg.setForeground(new Color(-8224126));
        lblValueAvg.setText("Avg:");
        panelValueSettings.add(lblValueAvg, new GridConstraints(7, 0, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        txtFieldValueAvg.setBackground(new Color(-1));
        txtFieldValueAvg.setCaretColor(new Color(-8224126));
        txtFieldValueAvg.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        txtFieldValueAvg.setForeground(new Color(-8224126));
        txtFieldValueAvg.setSelectionColor(new Color(-5841665));
        txtFieldValueAvg.setText("");
        txtFieldValueAvg.setToolTipText("Only one or double digit number.");
        panelValueSettings.add(txtFieldValueAvg, new GridConstraints(7, 1, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        lblValueDev = new JLabel();
        lblValueDev.setBackground(new Color(-1));
        lblValueDev.setForeground(new Color(-8224126));
        lblValueDev.setText("Dev:");
        panelValueSettings.add(lblValueDev, new GridConstraints(7, 2, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        txtFieldValueDev.setBackground(new Color(-1));
        txtFieldValueDev.setCaretColor(new Color(-8224126));
        txtFieldValueDev.setFont(new Font("Segoe UI", Font.PLAIN, 11));
        txtFieldValueDev.setForeground(new Color(-8224126));
        txtFieldValueDev.setSelectionColor(new Color(-5841665));
        txtFieldValueDev.setText("");
        txtFieldValueDev.setToolTipText("Only one or double digit number.");
        panelValueSettings.add(txtFieldValueDev, new GridConstraints(7, 3, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(30, -1), new Dimension(30, -1), new Dimension(30, -1), 0, false));
        lblWarning = new JLabel();
        lblWarning.setBackground(new Color(-1));
        lblWarning.setForeground(new Color(-52429));
        lblWarning.setText("");
        lblWarning.setVisible(false);
        contentPane.add(lblWarning, new GridConstraints(1, 0, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        ButtonGroup buttonGroup;
        buttonGroup = new ButtonGroup();
        buttonGroup.add(radBtnGenerateYes);
        buttonGroup.add(radBtnGenerateNo);
    }

    /**
     * @noinspection ALL
     */
    public JComponent $$$getRootComponent$$$() {
        return contentPane;
    }
}
