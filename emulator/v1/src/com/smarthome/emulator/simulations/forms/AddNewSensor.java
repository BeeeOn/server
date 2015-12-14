package com.smarthome.emulator.simulations.forms;

import com.intellij.uiDesigner.core.GridConstraints;
import com.intellij.uiDesigner.core.GridLayoutManager;
import com.intellij.uiDesigner.core.Spacer;
import com.smarthome.emulator.Emulator;
import com.smarthome.emulator.simulations.Error;
import com.smarthome.emulator.simulations.panels.AdapterPanel;
import com.smarthome.emulator.simulations.utilities.IPAddressValidator;

import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.border.EmptyBorder;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Properties;

/**
 * Dialog for adding new {@link com.smarthome.emulator.simulations.panels.SensorPanel} to {@link com.smarthome.emulator.simulations.panels.AdapterPanel}.
 * Loads from predefined property files or inserted by user. Also verifies properties correctness and afterwards adds new
 * {@link com.smarthome.emulator.simulations.panels.SensorPanel}
 */
public class AddNewSensor extends JDialog {
    /**
     * GUI variables
     */
    private JPanel contentPane;
    private JButton buttonOK;
    private JButton buttonCancel;
    private JRadioButton existingRadBtn;
    private JRadioButton loadNewRadBtn;
    private JLabel warningLbl;
    private JLabel chooseLbl;
    private JLabel existingTemplatesLbl;
    private JComboBox existingTemplatesComboBox;
    private JTextField loadTemplateTextField;
    private JLabel loadTemplateLbl;
    private JButton loadTemplateBtn;
    private JLabel newIconLbl;
    private JComboBox newIconComboBox;
    private JTextField sensorIdTextField;
    /**
     * property file with names of predefined properties files
     */
    private Properties types;
    /**
     * property file with predefined icons paths
     */
    private Properties icons;
    /**
     * resource folder with predefined properties files
     */
    private String typesFolder;
    /**
     * resource folder with predefined icons paths
     */
    private String iconsFolder;
    /**
     * array of predefined types
     */
    private String[] typesList;
    /**
     * array of predefined icons paths
     */
    protected String[] iconsTextList;
    /**
     * array of icon created from predefined icons paths
     */
    protected ImageIcon[] imageIcons;
    /**
     * adapter panel that will own new sensor panel
     */
    private AdapterPanel adapterPanel;

    /**
     * Initialises GUI components, loads predefined types properties files and predefined icons.
     *
     * @param adapter {@link com.smarthome.emulator.simulations.panels.AdapterPanel} that will own new {@link com.smarthome.emulator.simulations.panels.SensorPanel}
     */
    public AddNewSensor(AdapterPanel adapter) {
        setContentPane(contentPane);
        setModal(true);
        getRootPane().setDefaultButton(buttonOK);
        setBounds(345, 100, 411, 280);
        setResizable(false);
        this.adapterPanel = adapter;
        UImodifications();
        listeners();
        loadExistingTemplates();
        loadIcons();
        //loadTemplateTextField.setText("C:\\Users\\Shu\\Desktop\\new.properties");
        this.setVisible(true);

    }

    /**
     * Loads predefined types properties files to {@link javax.swing.JComboBox}
     */
    public void loadExistingTemplates() {
        types = new Properties();
        InputStream typesInput;
        try {
            typesInput = getClass().getResourceAsStream("/sensor_types/Types.properties");
            types.load(typesInput);
            typesInput.close();
            typesFolder = types.getProperty("folder");
            if (typesFolder == null) {
                Emulator.showError(Error.PROPERTIES_TYPES_INVALID_FOLDER.getMessage());
                return;
            }
            typesList = types.getProperty("files").split(",");
            if (typesList == null || typesList.length == 0) {
                Emulator.showError(Error.PROPERTIES_TYPES_INVALID_FILES.getMessage());
                return;
            }
            for (int i = 0; i < typesList.length; i++) {
                existingTemplatesComboBox.addItem(typesList[i]);
            }
        } catch (FileNotFoundException e) {
            Emulator.showError(Error.PROPERTIES_TYPES_NOT_FOUND.getMessage());
        } catch (IOException e) {
            Emulator.showError(Error.PROPERTIES_TYPES_LOAD.getMessage());
        }
    }

    /**
     * Loads predefined icons to {@link javax.swing.JComboBox}
     */
    public void loadIcons() {
        icons = new Properties();
        InputStream iconsInput;
        try {
            iconsInput = getClass().getResourceAsStream("/img/images.properties");
            icons.load(iconsInput);
            iconsInput.close();
            iconsFolder = icons.getProperty("folder");
            if (iconsFolder == null) {
                Emulator.showError(Error.PROPERTIES_ICON_INVALID_FOLDER.getMessage());
                return;
            }
            iconsTextList = icons.getProperty("files").split(",");
            if (iconsTextList == null || iconsTextList.length == 0) {
                Emulator.showError(Error.PROPERTIES_ICON_INVALID_FILES.getMessage());
                return;
            }
            imageIcons = new ImageIcon[iconsTextList.length];
            for (int i = 0; i < iconsTextList.length; i++) {
                imageIcons[i] = new ImageIcon(getClass().getResource("/" + iconsFolder + "/" + iconsTextList[i] + ".png"));
                newIconComboBox.addItem(iconsTextList[i]);
            }
        } catch (FileNotFoundException e) {
            Emulator.showError(Error.PROPERTIES_ICON_NOT_FOUND.getMessage());
        } catch (IOException e) {
            Emulator.showError(Error.PROPERTIES_ICON_LOAD.getMessage());
        }
    }

    /**
     * Opens dialog for new properties file to be loaded.
     *
     * @return filename or string <code>none</code> if couldn't load filename
     */
    public String openLoadDialog() {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setDialogTitle("Specify property file to load");
        fileChooser.setApproveButtonText("Load");
        FileNameExtensionFilter xml = new FileNameExtensionFilter("Properties Files", "properties");
        fileChooser.addChoosableFileFilter(xml);
        fileChooser.setFileFilter(xml);
        int returnVal = fileChooser.showOpenDialog(this);
        if (returnVal == fileChooser.APPROVE_OPTION) {
            File loadFile = fileChooser.getSelectedFile();
            return loadFile.getAbsolutePath();
        }
        return null;
    }

    /**
     * Assigns listeners to dialog buttons.
     */
    private void listeners() {
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
        loadTemplateBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (loadTemplateBtn.isEnabled()) {
                    String fileName = openLoadDialog();
                    if (fileName != null) {
                        loadTemplateTextField.setText(fileName);
                    }
                }
            }
        });
        existingRadBtn.addItemListener(new ItemListener() {
            @Override
            public void itemStateChanged(ItemEvent e) {
                existingTemplatesComboBox.setEnabled(true);
                loadTemplateTextField.setEnabled(false);
                loadTemplateBtn.setEnabled(false);
                newIconComboBox.setEnabled(false);
            }
        });
        loadNewRadBtn.addItemListener(new ItemListener() {
            @Override
            public void itemStateChanged(ItemEvent e) {
                existingTemplatesComboBox.setEnabled(false);
                loadTemplateTextField.setEnabled(true);
                loadTemplateBtn.setEnabled(true);
                newIconComboBox.setEnabled(true);
            }
        });
        /*existingRadBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                existingTemplatesComboBox.setEnabled(true);
                loadTemplateTextField.setEnabled(false);
                loadTemplateBtn.setEnabled(false);
                newIconComboBox.setEnabled(false);
            }
        });
        loadNewRadBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                existingTemplatesComboBox.setEnabled(false);
                loadTemplateTextField.setEnabled(true);
                loadTemplateBtn.setEnabled(true);
                newIconComboBox.setEnabled(true);
            }
        });*/
        setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                onCancel();
            }
        });
    }

    /**
     * Checks correctness of inserted information. If everything is correct,
     * adds new {@link com.smarthome.emulator.simulations.panels.SensorPanel} to {@link com.smarthome.emulator.simulations.panels.AdapterPanel}
     * given as constructor parameter.
     * <p>
     * Shows errors:
     * <ul>
     * <li>PROPERTIES_NOT_FOUND if given properties file cannot be found</li>
     * </ul>
     * </p>
     */
    private void onOK() {
        String fileName;
        InputStream input;
        try {
            if (existingRadBtn.isSelected()) {
                fileName = (String) existingTemplatesComboBox.getSelectedItem();
                input = getClass().getResourceAsStream("/" + typesFolder + "/" + fileName + ".properties");
            } else {
                fileName = loadTemplateTextField.getText();
                input = new FileInputStream(fileName);
            }
            Properties properties = checkPropertiesFile(input, fileName);
            if (properties != null) {
                String id = sensorIdTextField.getText();
                int res = checkId(id);
                if (res == 0) {
                    adapterPanel.addSensorPanel(Integer.valueOf(id), properties);
                    this.dispose();
                } else if (res == 1) {
                    adapterPanel.addSensorPanel(ipToInt(id), properties);
                    this.dispose();
                } else {
                    Emulator.showError(Error.SENSOR_ID_FORMAT.getMessage());
                }
            }
        } catch (FileNotFoundException e) {
            Emulator.showError(Error.PROPERTIES_NOT_FOUND.getMessage());
        } catch (UnknownHostException e) {
            Emulator.showError(Error.SENSOR_ID_IP_CONVERT.getMessage());
        }

    }

    public static int ipToInt(String id) throws UnknownHostException {
        int result = 0;
        for (byte b : InetAddress.getByName(id).getAddress()) {
            result = result << 8 | (b & 0xFF);
        }
        return result;
    }

    public static int checkId(String idString) {
        IPAddressValidator validator = new IPAddressValidator();
        if (isNumeric(idString)) {
            return 0;
        } else if (validator.validate(idString)) {
            return 1;
        } else {
            return 2;
        }
    }

    private static boolean isNumeric(String str) {
        try {
            int d = Integer.parseInt(str);
        } catch (NumberFormatException nfe) {
            return false;
        }
        return true;
    }

    /**
     * Checks if loaded properties file is correct.
     * <p>
     * Shows errors:
     * <ul>
     * <li>PROPERTIES_NAME - name property incorrect</li>
     * <li>PROPERTIES_BCOLOR - background color property incorrect</li>
     * <li>PROPERTIES_FCOLOR - text color property is incorrect</li>
     * <li>PROPERTIES_LCOLOR - lable text color property is incorrect</li>
     * <li>PROPERTIES_REFRESH_RATE - refresh rate property is incorrect</li>
     * <li>PROPERTIES_SIGNAL - signal property is incorrect</li>
     * <li>PROPERTIES_BATTERY - battery property is incorrect </li>
     * <li>PROPERTIES_PROTOCOL - protocol property is incorrect</li>
     * <li>PROPERTIES_MISSING - one or more properties missing</li>
     * <li>PROPERTIES_MISSING_WRONG - multiset properties incorrect</li>
     * <li>PROPERTIES_INTEGER - some properties are not in correct number format</li>
     * <li>PROPERTIES_LOAD - cannot load properties file</li>
     * <li>PROPERTIES_NOT_FOUND - cannot find properties file</li>
     * </ul>
     * </p>
     *
     * @param input    input stream with properties file
     * @param fileName file name
     * @return properties set created from file
     */
    private Properties checkPropertiesFile(InputStream input, String fileName) {
        Properties properties = new Properties();
        try {
            properties.load(input);
            input.close();
            if ((properties.getProperty("name")) == null) {
                Emulator.showError(Error.PROPERTIES_NAME.getMessage());
                return null;
            }
            if ((properties.getProperty("backgroundColor")) == null || properties.getProperty("backgroundColor").split(",").length != 3) {
                Emulator.showError(Error.PROPERTIES_BCOLOR.getMessage());
                return null;
            }
            if ((properties.getProperty("fontColor")) == null || properties.getProperty("fontColor").split(",").length != 3) {
                Emulator.showError(Error.PROPERTIES_FCOLOR.getMessage());
                return null;
            }
            if ((properties.getProperty("lblFontColor")) == null || properties.getProperty("lblFontColor").split(",").length != 3) {
                Emulator.showError(Error.PROPERTIES_LCOLOR.getMessage());
                return null;
            }
            if ((properties.getProperty("refreshRate")) == null) {
                Emulator.showError(Error.PROPERTIES_REFRESH_RATE.getMessage());
                return null;
            } else {
                Integer.valueOf(properties.getProperty("refreshRate"));
            }
            if ((properties.getProperty("signal")) == null) {
                Emulator.showError(Error.PROPERTIES_SIGNAL.getMessage());
                return null;
            } else {
                Integer.valueOf(properties.getProperty("signal"));
            }
            if ((properties.getProperty("battery")) == null) {
                Emulator.showError(Error.PROPERTIES_BATTERY.getMessage());
                return null;
            } else {
                Integer.valueOf(properties.getProperty("battery"));
            }
            if ((properties.getProperty("protocol")) == null) {
                Emulator.showError(Error.PROPERTIES_PROTOCOL.getMessage());
                return null;
            } else {
                Float.valueOf(properties.getProperty("protocol"));
            }
            String valueTypes = properties.getProperty("valueTypes");
            String valueClasses = properties.getProperty("valueClasses");
            String values = properties.getProperty("values");
            String max = properties.getProperty("max");
            String min = properties.getProperty("min");
            String avg = properties.getProperty("avg");
            String dev = properties.getProperty("dev");

            if (valueTypes == null || valueClasses == null || values == null || max == null || min == null || avg == null || dev == null) {
                Emulator.showError(Error.PROPERTIES_MISSING.getMessage());
                return null;
            }
            if ((valueTypes.split(",").length != valueClasses.split(",").length) ||
                    (valueClasses.split(",").length != values.split(",").length) ||
                    (values.split(",").length != max.split(",").length) ||
                    (max.split(",").length != min.split(",").length) ||
                    (min.split(",").length != avg.split(",").length) ||
                    (avg.split(",").length != dev.split(",").length)
                    ) {
                Emulator.showError(Error.PROPERTIES_MISSING_WRONG.getMessage());
                return null;
            }
            if (loadNewRadBtn.isSelected()) {
                OutputStream output = new FileOutputStream(fileName);
                properties.setProperty("icon", "/" + iconsFolder + "/" + iconsTextList[newIconComboBox.getSelectedIndex()] + ".png");
                properties.store(output, "sensor properties");
                output.close();
            }
            return properties;
        } catch (NumberFormatException e) {
            Emulator.showError(Error.PROPERTIES_INTEGER.getMessage());
            return null;
        } catch (FileNotFoundException e) {
            Emulator.showError(Error.PROPERTIES_LOAD.getMessage());
            return null;
        } catch (IOException e) {
            Emulator.showError(Error.PROPERTIES_NOT_FOUND.getMessage());
            return null;
        }
    }

    /**
     * Disposes frame
     */
    private void onCancel() {
        dispose();
    }

    /**
     * Modifications to GUI before painting the frame
     */
    private void UImodifications() {
        ComboBoxRenderer renderer = new ComboBoxRenderer();
        renderer.setPreferredSize(new Dimension(150, 38));
        newIconComboBox.setRenderer(renderer);
    }

    {
// GUI initializer generated by IntelliJ IDEA GUI Designer
// >>> IMPORTANT!! <<<
// DO NOT EDIT OR ADD ANY CODE HERE!
        $$$setupUI$$$();
    }

    /**
     * Method generated by IntelliJ IDEA GUI Designer
     * >>> IMPORTANT!! <<<
     * DO NOT edit this method OR call it in your code!
     *
     * @noinspection ALL
     */
    private void $$$setupUI$$$() {
        contentPane = new JPanel();
        contentPane.setLayout(new GridLayoutManager(2, 1, new Insets(10, 10, 10, 10), -1, -1));
        contentPane.setBackground(new Color(-1));
        final JPanel panel1 = new JPanel();
        panel1.setLayout(new GridLayoutManager(1, 3, new Insets(0, 0, 0, 0), -1, -1));
        panel1.setBackground(new Color(-1));
        contentPane.add(panel1, new GridConstraints(1, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, 1, null, null, null, 0, false));
        final JPanel panel2 = new JPanel();
        panel2.setLayout(new GridLayoutManager(1, 2, new Insets(0, 0, 0, 0), -1, -1, true, false));
        panel2.setBackground(new Color(-1));
        panel1.add(panel2, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, null, null, null, 0, false));
        buttonCancel = new JButton();
        buttonCancel.setBackground(new Color(-3750202));
        buttonCancel.setBorderPainted(false);
        buttonCancel.setForeground(new Color(-1));
        buttonCancel.setMargin(new Insets(2, 0, 2, 0));
        buttonCancel.setText("Cancel");
        panel2.add(buttonCancel, new GridConstraints(0, 1, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_FIXED, new Dimension(115, 24), new Dimension(115, 24), new Dimension(115, 24), 0, false));
        buttonOK = new JButton();
        buttonOK.setBackground(new Color(-5841665));
        buttonOK.setBorderPainted(false);
        buttonOK.setForeground(new Color(-12303292));
        buttonOK.setMargin(new Insets(2, 0, 2, 0));
        buttonOK.setText("Add");
        panel2.add(buttonOK, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_FIXED, new Dimension(115, 24), new Dimension(115, 24), new Dimension(115, 24), 0, false));
        final Spacer spacer1 = new Spacer();
        panel1.add(spacer1, new GridConstraints(0, 2, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_WANT_GROW, 1, null, null, null, 0, false));
        warningLbl = new JLabel();
        warningLbl.setBackground(new Color(-1));
        warningLbl.setForeground(new Color(-52429));
        warningLbl.setText("");
        panel1.add(warningLbl, new GridConstraints(0, 1, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JPanel panel3 = new JPanel();
        panel3.setLayout(new GridLayoutManager(7, 5, new Insets(0, 0, 0, 0), -1, -1));
        panel3.setBackground(new Color(-1));
        contentPane.add(panel3, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, null, null, null, 0, false));
        final JLabel label1 = new JLabel();
        label1.setBackground(new Color(-1));
        label1.setFont(new Font("Segoe UI", Font.PLAIN, 28));
        label1.setForeground(new Color(-13127169));
        label1.setText("Add new sensor/actuator");
        panel3.add(label1, new GridConstraints(0, 0, 1, 5, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final Spacer spacer2 = new Spacer();
        panel3.add(spacer2, new GridConstraints(6, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_VERTICAL, 1, GridConstraints.SIZEPOLICY_WANT_GROW, null, null, null, 0, false));
        chooseLbl = new JLabel();
        chooseLbl.setBackground(new Color(-1));
        chooseLbl.setFont(new Font("Segoe UI", Font.BOLD, 14));
        chooseLbl.setForeground(new Color(-8224126));
        chooseLbl.setText("Choose template:");
        panel3.add(chooseLbl, new GridConstraints(2, 0, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        existingRadBtn = new JRadioButton();
        existingRadBtn.setBackground(new Color(-1));
        existingRadBtn.setForeground(new Color(-8224126));
        existingRadBtn.setIcon(new ImageIcon(getClass().getResource("/img/radioButtonNotSelected.png")));
        existingRadBtn.setPressedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        existingRadBtn.setSelected(true);
        existingRadBtn.setSelectedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        existingRadBtn.setText("Existing");
        panel3.add(existingRadBtn, new GridConstraints(2, 1, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_FIXED, new Dimension(-1, 18), new Dimension(-1, 18), new Dimension(-1, 18), 0, false));
        loadNewRadBtn = new JRadioButton();
        loadNewRadBtn.setBackground(new Color(-1));
        loadNewRadBtn.setForeground(new Color(-8224126));
        loadNewRadBtn.setIcon(new ImageIcon(getClass().getResource("/img/radioButtonNotSelected.png")));
        loadNewRadBtn.setPressedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        loadNewRadBtn.setSelectedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        loadNewRadBtn.setText("Load new");
        panel3.add(loadNewRadBtn, new GridConstraints(2, 2, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_FIXED, new Dimension(-1, 18), new Dimension(-1, 18), new Dimension(-1, 18), 0, false));
        existingTemplatesLbl = new JLabel();
        existingTemplatesLbl.setBackground(new Color(-1));
        existingTemplatesLbl.setForeground(new Color(-8224126));
        existingTemplatesLbl.setText("Existing templates:");
        panel3.add(existingTemplatesLbl, new GridConstraints(3, 0, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        existingTemplatesComboBox = new JComboBox();
        existingTemplatesComboBox.setBackground(new Color(-1));
        existingTemplatesComboBox.setForeground(new Color(-8224126));
        existingTemplatesComboBox.setMaximumRowCount(15);
        panel3.add(existingTemplatesComboBox, new GridConstraints(3, 1, 1, 3, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        loadTemplateLbl = new JLabel();
        loadTemplateLbl.setBackground(new Color(-1));
        loadTemplateLbl.setEnabled(true);
        loadTemplateLbl.setForeground(new Color(-8224126));
        loadTemplateLbl.setText("Load template:");
        panel3.add(loadTemplateLbl, new GridConstraints(4, 0, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        loadTemplateTextField = new JTextField();
        loadTemplateTextField.setBackground(new Color(-1));
        loadTemplateTextField.setCaretColor(new Color(-8224126));
        loadTemplateTextField.setDisabledTextColor(new Color(-8224126));
        loadTemplateTextField.setEditable(true);
        loadTemplateTextField.setEnabled(false);
        loadTemplateTextField.setForeground(new Color(-8224126));
        loadTemplateTextField.setSelectionColor(new Color(-5841665));
        loadTemplateTextField.setText("");
        panel3.add(loadTemplateTextField, new GridConstraints(4, 1, 1, 3, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_WANT_GROW, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(150, -1), null, 0, false));
        loadTemplateBtn = new JButton();
        loadTemplateBtn.setBackground(new Color(-5841665));
        loadTemplateBtn.setBorderPainted(false);
        loadTemplateBtn.setEnabled(false);
        loadTemplateBtn.setForeground(new Color(-12303292));
        loadTemplateBtn.setMargin(new Insets(2, 0, 2, 0));
        loadTemplateBtn.setText("Load");
        panel3.add(loadTemplateBtn, new GridConstraints(4, 4, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_FIXED, new Dimension(60, 24), new Dimension(60, 24), new Dimension(60, 24), 0, false));
        newIconLbl = new JLabel();
        newIconLbl.setBackground(new Color(-1));
        newIconLbl.setEnabled(true);
        newIconLbl.setForeground(new Color(-8224126));
        newIconLbl.setText("New template icon:");
        panel3.add(newIconLbl, new GridConstraints(5, 0, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        newIconComboBox = new JComboBox();
        newIconComboBox.setBackground(new Color(-1));
        newIconComboBox.setEnabled(false);
        newIconComboBox.setForeground(new Color(-8224126));
        newIconComboBox.setMaximumRowCount(15);
        panel3.add(newIconComboBox, new GridConstraints(5, 1, 1, 2, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JLabel label2 = new JLabel();
        label2.setBackground(new Color(-1));
        label2.setForeground(new Color(-8224126));
        label2.setText("Sensor ID:");
        panel3.add(label2, new GridConstraints(1, 0, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        sensorIdTextField = new JTextField();
        sensorIdTextField.setBackground(new Color(-1));
        sensorIdTextField.setForeground(new Color(-8224126));
        panel3.add(sensorIdTextField, new GridConstraints(1, 1, 1, 2, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_WANT_GROW, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(150, -1), null, 0, false));
        ButtonGroup buttonGroup;
        buttonGroup = new ButtonGroup();
        buttonGroup.add(existingRadBtn);
        buttonGroup.add(loadNewRadBtn);
    }

    /**
     * @noinspection ALL
     */
    public JComponent $$$getRootComponent$$$() {
        return contentPane;
    }

    /**
     * {@link javax.swing.JComboBox} renderer extending {@link javax.swing.JLabel} for ability to show {@link javax.swing.ImageIcon}s
     * as {@link javax.swing.JComboBox} items.
     */
    class ComboBoxRenderer extends JLabel
            implements ListCellRenderer {
        /**
         * no image text font
         */
        private Font uhOhFont;
        /**
         * item background
         */
        private Color selectedBackground = new Color(-5841665);
        /**
         * not selected item background
         */
        private Color notSelectedBackground = new Color(-1);
        /**
         * item border
         */
        private Border border = new EmptyBorder(0, 10, 0, 10);

        /**
         * Sets new renderer's opaque and placement
         */
        public ComboBoxRenderer() {
            setOpaque(true);
            setHorizontalAlignment(LEFT);
            setVerticalAlignment(CENTER);
        }

        /*
         * This method finds the image and text corresponding
         * to the selected value and returns the label, set up
         * to display the text and image.
         */

        /**
         * Finding images and there corresponding text to be displayed.
         * Return {@link javax.swing.JLabel} as {@link javax.swing.JComboBox} item.
         *
         * @param list
         * @param value
         * @param index
         * @param isSelected
         * @param cellHasFocus
         * @return
         */
        public Component getListCellRendererComponent(
                JList list,
                Object value,
                int index,
                boolean isSelected,
                boolean cellHasFocus) {
            //gets selected item
            int selectedIndex = 0;
            for (int i = 0; i < iconsTextList.length; i++) {
                if (iconsTextList[i].equals((String) value)) {
                    selectedIndex = i;
                }
            }
            //change background state
            if (isSelected) {
                setBackground(selectedBackground);
                setForeground(list.getSelectionForeground());
            } else {
                setBackground(notSelectedBackground);
                setForeground(list.getForeground());
            }
            setBorder(border);
            //Set the icon and text.  If icon was null, say so.
            ImageIcon icon = imageIcons[selectedIndex];
            String iconText = iconsTextList[selectedIndex];
            setIcon(icon);
            if (icon != null) {
                setText("  " + iconText);
                setFont(list.getFont());
            } else {
                setUhOhText(iconText + " (no image available)",
                        list.getFont());
            }
            return this;
        }

        /**
         * Sets text and text font for no image found
         *
         * @param uhOhText
         * @param normalFont
         */
        protected void setUhOhText(String uhOhText, Font normalFont) {
            if (uhOhFont == null) { //lazily create this font
                uhOhFont = normalFont.deriveFont(Font.ITALIC);
            }
            setFont(uhOhFont);
            setText(uhOhText);
        }
    }
}
