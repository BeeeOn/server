package com.smarthome.emulator.simulations.forms;

import com.intellij.uiDesigner.core.GridConstraints;
import com.intellij.uiDesigner.core.GridLayoutManager;
import com.intellij.uiDesigner.core.Spacer;
import com.smarthome.emulator.design.JLimitedTextField;
import com.smarthome.emulator.design.JNumberTextField;
import com.smarthome.emulator.simulations.DetailedSimulation;
import com.smarthome.emulator.simulations.panels.AdapterPanel;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.net.InetAddress;
import java.net.UnknownHostException;

/**
 * Dialog for adding new {@link com.smarthome.emulator.simulations.panels.AdapterPanel} to {@link com.smarthome.emulator.simulations.DetailedSimulation}.
 * Contains {@link com.smarthome.emulator.simulations.basis.Adapter} and {@link com.smarthome.emulator.simulations.basis.Server} information, which are after
 * inserting validated.
 */
public class AddNewAdapter extends JDialog {
    /**
     * default port
     */
    private static final String DEFAULT_PORT = "7080";
    /**
     * default protocol
     */
    private static final String DEFAULT_PROTOCOL = "0.1";
    /**
     * default address
     */
    private static final String DEFAULT_ADDRESS = "10.1.0.1";
    /**
     * default adapter id
     */
    private static final String DEFAULT_ID = "51914";
    /**
     * default adapter name
     */
    private static final String DEFAULT_NAME = "Emulator #1";
    /**
     * GUI variables
     */
    private JPanel contentPane;
    private JButton buttonOK;
    private JButton buttonCancel;
    private JLabel addNewAdapterLbl;
    private JPanel mainPanel;
    private JPanel mainBtnsPanel;
    private JPanel mainBtnsWrapPanel;
    private JLabel adapterInfoLbl;
    private JLabel serverInfoLbl;
    private JLabel idLbl;
    private JLimitedTextField idTextField;
    private JLabel nameLbl;
    private JTextField nameTextField;
    private JLabel protocolLbl;
    private JTextField protocolTextField;
    private JLabel addressLbl;
    private JTextField addressTextField;
    private JLabel portLbl;
    private JNumberTextField portTextField;
    private JRadioButton ipRadBtn;
    private JRadioButton hostRadBtn;
    private JLabel validateLbl;
    private JButton validateBtn;
    private JLabel warningLbl;
    private JRadioButton registeredYesRadBtn;
    private JRadioButton registeredNoRadBtn;
    /**
     * instance of detailed simulation
     */
    private DetailedSimulation simulation;
    /**
     * number of so far created adapters
     */
    private int adaptersCount;

    /**
     * Initialises GUI components, assigns button listeners.
     *
     * @param simulation    simulation, that called this dialog
     * @param adaptersCount number of so far created adapters
     */
    public AddNewAdapter(DetailedSimulation simulation, int adaptersCount) {
        $$$setupUI$$$();
        UImodifications();
        initModifications();
        this.simulation = simulation;
        this.adaptersCount = adaptersCount;
        setContentPane(contentPane);
        setModal(true);
        getRootPane().setDefaultButton(buttonOK);
        //TODO dynamic bounds set
        setBounds(345, 100, 558, 255);
        setResizable(false);
        buttonOK.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                onOK();
            }
        });

        buttonCancel.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                onCancel();
            }
        });
        // call onCancel() when cross is clicked
        setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                onCancel();
            }
        });
        validateBtn.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (!addressTextField.getText().trim().equals("") && !(addressTextField.getText() == null)) {
                    if (validateAddress()) {
                        validateLbl.setForeground(Color.GREEN);
                        validateLbl.setText("Valid address!");
                    } else {
                        validateLbl.setForeground(Color.RED);
                        validateLbl.setText("Invalid address!");
                    }
                } else {
                    warningLbl.setText("Insert host name or IP address!");
                }
            }
        });
        this.setVisible(true);
    }

    /**
     * Validates, if inserted address is valid
     *
     * @return <code>true</code> if valid, <code>false</code> if not
     */
    private boolean validateAddress() {
        try {
            InetAddress.getByName(addressTextField.getText());
            return true;
        } catch (UnknownHostException ex) {
            return false;
        } catch (SecurityException ex) {
            warningLbl.setText("Security exception raised!");
            return false;
        }
    }

    /**
     * Checks correctness of inserted information. If everything is correct,
     * adds new {@link com.smarthome.emulator.simulations.panels.AdapterPanel} to <code>simulation</code>
     */
    private void onOK() {
        String id, name, protocol, address, port;
        if (!idTextField.getText().trim().equals("") && !(idTextField.getText() == null)) {
            id = idTextField.getText();
        } else {
            warningLbl.setText("Id field is empty!");
            return;
        }
        if (!(id.length() >= 4)) {
            warningLbl.setText("Id must have at least 4 characters!");
            return;
        }
        if (!nameTextField.getText().trim().equals("") && !(nameTextField.getText() == null)) {
            name = nameTextField.getText();
        } else {
            warningLbl.setText("Name field is empty!");
            return;
        }
        if (!protocolTextField.getText().trim().equals("") && !(protocolTextField.getText() == null)) {
            protocol = protocolTextField.getText();
        } else {
            warningLbl.setText("Protocol field is empty!");
            return;
        }
        //TODO add validateAddress() to next if
        if (!addressTextField.getText().trim().equals("") && !(addressTextField.getText() == null)) {
            address = addressTextField.getText();
        } else {
            warningLbl.setText("Address field is empty or address is not valid!");
            return;
        }
        if (!portTextField.getText().trim().equals("") && !(portTextField.getText() == null)) {
            port = portTextField.getText();
        } else {
            warningLbl.setText("Port field is empty!");
            return;
        }
        if (!simulation.isAdapterIdFree(Long.parseLong(id))) {
            warningLbl.setText("This id is already taken!");
            return;
        }
        //adding new adapter panel
        simulation.getAdapters().add(new AdapterPanel(
                simulation,
                adaptersCount,
                hostRadBtn.isSelected() ? address : null,
                ipRadBtn.isSelected() ? address : null,
                Integer.valueOf(port),
                Long.parseLong(id),
                name,
                protocol,
                registeredYesRadBtn.isSelected() ? true : false));
        simulation.adaptersCount++;
        dispose();
    }

    /**
     * Checks if given string is number
     *
     * @param str string to be checked
     * @return <code>true</code> if is number, <code>false</code> otherwise
     */
    private static boolean isNumeric(String str) {
        try {
            int d = Integer.parseInt(str);
        } catch (NumberFormatException nfe) {
            return false;
        }
        return true;
    }

    /**
     * Disposes frame
     */
    private void onCancel() {
        // add your code here if necessary
        dispose();
    }

    /**
     * Inserts default fields values
     */
    private void initModifications() {
        portTextField.setText(DEFAULT_PORT);
        protocolTextField.setText(DEFAULT_PROTOCOL);
        //TODO erase
        addressTextField.setText(DEFAULT_ADDRESS);
        idTextField.setText(DEFAULT_ID);
        nameTextField.setText(DEFAULT_NAME);
    }

    /**
     * Modifications to GUI before painting the frame
     */
    private void UImodifications() {
        idTextField.setBackground(new Color(-1));
        idTextField.setCaretColor(new Color(-8224126));
        idTextField.setForeground(new Color(-8224126));
        idTextField.setMargin(new Insets(0, 5, 0, 10));
        idTextField.setSelectionColor(new Color(-5841665));
        portTextField.setBackground(new Color(-1));
        portTextField.setCaretColor(new Color(-8224126));
        portTextField.setForeground(new Color(-8224126));
        portTextField.setMargin(new Insets(0, 5, 0, 10));
        portTextField.setSelectionColor(new Color(-5841665));
        idTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        portTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        nameTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        protocolTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        addressTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
    }

    /**
     * Components not included in Intellij Designer must be initialised here
     */
    private void createUIComponents() {
        idTextField = new JLimitedTextField(5);
        portTextField = new JNumberTextField(5);
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
        contentPane.setLayout(new GridLayoutManager(2, 1, new Insets(10, 10, 10, 10), -1, -1));
        contentPane.setBackground(new Color(-1));
        mainBtnsWrapPanel = new JPanel();
        mainBtnsWrapPanel.setLayout(new GridLayoutManager(1, 3, new Insets(0, 0, 0, 0), -1, -1));
        mainBtnsWrapPanel.setBackground(new Color(-1));
        contentPane.add(mainBtnsWrapPanel, new GridConstraints(1, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, 1, null, null, null, 0, false));
        mainBtnsPanel = new JPanel();
        mainBtnsPanel.setLayout(new GridLayoutManager(1, 2, new Insets(0, 0, 0, 0), -1, -1, true, false));
        mainBtnsPanel.setBackground(new Color(-1));
        mainBtnsWrapPanel.add(mainBtnsPanel, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, null, null, null, 0, false));
        buttonOK = new JButton();
        buttonOK.setBackground(new Color(-5841665));
        buttonOK.setBorderPainted(false);
        buttonOK.setFocusPainted(false);
        buttonOK.setForeground(new Color(-12303292));
        buttonOK.setMargin(new Insets(2, 0, 2, 0));
        buttonOK.setText("Add");
        mainBtnsPanel.add(buttonOK, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(115, 24), new Dimension(115, 24), new Dimension(115, 24), 0, false));
        buttonCancel = new JButton();
        buttonCancel.setBackground(new Color(-3750202));
        buttonCancel.setBorderPainted(false);
        buttonCancel.setFocusPainted(false);
        buttonCancel.setForeground(new Color(-1));
        buttonCancel.setMargin(new Insets(2, 0, 2, 0));
        buttonCancel.setText("Cancel");
        mainBtnsPanel.add(buttonCancel, new GridConstraints(0, 1, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(115, 24), new Dimension(115, 24), new Dimension(115, 24), 0, false));
        warningLbl = new JLabel();
        warningLbl.setBackground(new Color(-1));
        warningLbl.setForeground(new Color(-52429));
        warningLbl.setText("");
        mainBtnsWrapPanel.add(warningLbl, new GridConstraints(0, 1, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final Spacer spacer1 = new Spacer();
        mainBtnsWrapPanel.add(spacer1, new GridConstraints(0, 2, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_WANT_GROW, 1, null, null, null, 0, false));
        mainPanel = new JPanel();
        mainPanel.setLayout(new GridLayoutManager(3, 3, new Insets(0, 0, 0, 0), -1, -1));
        mainPanel.setBackground(new Color(-1));
        mainPanel.setForeground(new Color(-12303292));
        contentPane.add(mainPanel, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, null, null, null, 0, false));
        addNewAdapterLbl = new JLabel();
        addNewAdapterLbl.setFont(new Font("Segoe UI", Font.PLAIN, 28));
        addNewAdapterLbl.setForeground(new Color(-13127169));
        addNewAdapterLbl.setText("Add new adapter");
        addNewAdapterLbl.setVerticalAlignment(1);
        mainPanel.add(addNewAdapterLbl, new GridConstraints(0, 0, 1, 3, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(211, 40), null, 0, false));
        adapterInfoLbl = new JLabel();
        adapterInfoLbl.setBackground(new Color(-1));
        adapterInfoLbl.setFont(new Font("Segoe UI", Font.BOLD, 14));
        adapterInfoLbl.setForeground(new Color(-8224126));
        adapterInfoLbl.setOpaque(true);
        adapterInfoLbl.setText("Adapter information:");
        adapterInfoLbl.setVerticalAlignment(1);
        mainPanel.add(adapterInfoLbl, new GridConstraints(1, 0, 1, 2, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        serverInfoLbl = new JLabel();
        serverInfoLbl.setBackground(new Color(-1));
        serverInfoLbl.setFont(new Font("Segoe UI", Font.BOLD, 14));
        serverInfoLbl.setForeground(new Color(-8224126));
        serverInfoLbl.setOpaque(true);
        serverInfoLbl.setText("Server information:");
        serverInfoLbl.setVerticalAlignment(1);
        mainPanel.add(serverInfoLbl, new GridConstraints(1, 2, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final JPanel panel1 = new JPanel();
        panel1.setLayout(new GridLayoutManager(4, 3, new Insets(5, 2, 2, 2), -1, -1));
        panel1.setBackground(new Color(-1));
        mainPanel.add(panel1, new GridConstraints(2, 0, 1, 2, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_CAN_GROW, null, null, null, 0, true));
        panel1.setBorder(BorderFactory.createTitledBorder(BorderFactory.createLineBorder(new Color(-1052689)), null));
        idLbl = new JLabel();
        idLbl.setBackground(new Color(-1));
        idLbl.setForeground(new Color(-8224126));
        idLbl.setText("Id:");
        idLbl.setVerticalAlignment(1);
        panel1.add(idLbl, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_NORTHEAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        panel1.add(idTextField, new GridConstraints(0, 1, 1, 2, GridConstraints.ANCHOR_NORTH, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(150, -1), null, 0, false));
        nameLbl = new JLabel();
        nameLbl.setBackground(new Color(-1));
        nameLbl.setForeground(new Color(-8224126));
        nameLbl.setText("Name:");
        panel1.add(nameLbl, new GridConstraints(1, 0, 1, 1, GridConstraints.ANCHOR_NORTHEAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        nameTextField = new JTextField();
        nameTextField.setBackground(new Color(-1));
        nameTextField.setCaretColor(new Color(-8224126));
        nameTextField.setForeground(new Color(-8224126));
        nameTextField.setMargin(new Insets(0, 5, 0, 10));
        nameTextField.setSelectionColor(new Color(-5841665));
        panel1.add(nameTextField, new GridConstraints(1, 1, 1, 2, GridConstraints.ANCHOR_NORTH, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(150, -1), null, 0, false));
        protocolLbl = new JLabel();
        protocolLbl.setBackground(new Color(-1));
        protocolLbl.setForeground(new Color(-8224126));
        protocolLbl.setHorizontalAlignment(4);
        protocolLbl.setHorizontalTextPosition(4);
        protocolLbl.setText("Comm.protocol:");
        panel1.add(protocolLbl, new GridConstraints(2, 0, 1, 1, GridConstraints.ANCHOR_NORTHEAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(100, -1), new Dimension(100, -1), null, 0, false));
        protocolTextField = new JTextField();
        protocolTextField.setBackground(new Color(-1));
        protocolTextField.setCaretColor(new Color(-8224126));
        protocolTextField.setForeground(new Color(-8224126));
        protocolTextField.setMargin(new Insets(0, 5, 0, 10));
        protocolTextField.setSelectionColor(new Color(-5841665));
        protocolTextField.setText("");
        panel1.add(protocolTextField, new GridConstraints(2, 1, 1, 2, GridConstraints.ANCHOR_NORTH, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(150, -1), null, 0, false));
        final JLabel label1 = new JLabel();
        label1.setBackground(new Color(-1));
        label1.setForeground(new Color(-8224126));
        label1.setHorizontalAlignment(4);
        label1.setHorizontalTextPosition(4);
        label1.setText("Registered:");
        panel1.add(label1, new GridConstraints(3, 0, 1, 1, GridConstraints.ANCHOR_NORTHEAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(100, -1), new Dimension(100, -1), null, 0, false));
        registeredYesRadBtn = new JRadioButton();
        registeredYesRadBtn.setBackground(new Color(-1));
        registeredYesRadBtn.setForeground(new Color(-8224126));
        registeredYesRadBtn.setIcon(new ImageIcon(getClass().getResource("/img/radioButtonNotSelected.png")));
        registeredYesRadBtn.setPressedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        registeredYesRadBtn.setSelected(true);
        registeredYesRadBtn.setSelectedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        registeredYesRadBtn.setText("Yes");
        panel1.add(registeredYesRadBtn, new GridConstraints(3, 1, 1, 1, GridConstraints.ANCHOR_NORTHWEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(-1, 18), new Dimension(-1, 18), new Dimension(-1, 18), 0, false));
        registeredNoRadBtn = new JRadioButton();
        registeredNoRadBtn.setBackground(new Color(-1));
        registeredNoRadBtn.setForeground(new Color(-8224126));
        registeredNoRadBtn.setIcon(new ImageIcon(getClass().getResource("/img/radioButtonNotSelected.png")));
        registeredNoRadBtn.setPressedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        registeredNoRadBtn.setSelectedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        registeredNoRadBtn.setText("No");
        panel1.add(registeredNoRadBtn, new GridConstraints(3, 2, 1, 1, GridConstraints.ANCHOR_NORTHWEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_FIXED, new Dimension(-1, 18), new Dimension(-1, 18), new Dimension(-1, 18), 0, false));
        final JPanel panel2 = new JPanel();
        panel2.setLayout(new GridLayoutManager(4, 3, new Insets(5, 2, 2, 2), -1, -1));
        panel2.setBackground(new Color(-1));
        mainPanel.add(panel2, new GridConstraints(2, 2, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, null, null, null, 0, true));
        panel2.setBorder(BorderFactory.createTitledBorder(BorderFactory.createLineBorder(new Color(-1052689)), null));
        addressLbl = new JLabel();
        addressLbl.setBackground(new Color(-1));
        addressLbl.setForeground(new Color(-8224126));
        addressLbl.setHorizontalAlignment(4);
        addressLbl.setHorizontalTextPosition(4);
        addressLbl.setRequestFocusEnabled(true);
        addressLbl.setText("Address:");
        panel2.add(addressLbl, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_NORTHEAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(80, -1), new Dimension(80, -1), null, 0, false));
        hostRadBtn = new JRadioButton();
        hostRadBtn.setBackground(new Color(-1));
        hostRadBtn.setForeground(new Color(-8224126));
        hostRadBtn.setIcon(new ImageIcon(getClass().getResource("/img/radioButtonNotSelected.png")));
        hostRadBtn.setPressedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        hostRadBtn.setSelected(true);
        hostRadBtn.setSelectedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        hostRadBtn.setText("Host");
        panel2.add(hostRadBtn, new GridConstraints(0, 1, 1, 1, GridConstraints.ANCHOR_NORTHWEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(-1, 18), new Dimension(-1, 18), new Dimension(-1, 18), 0, false));
        ipRadBtn = new JRadioButton();
        ipRadBtn.setBackground(new Color(-1));
        ipRadBtn.setForeground(new Color(-8224126));
        ipRadBtn.setIcon(new ImageIcon(getClass().getResource("/img/radioButtonNotSelected.png")));
        ipRadBtn.setPressedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        ipRadBtn.setSelectedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        ipRadBtn.setText("IP");
        panel2.add(ipRadBtn, new GridConstraints(0, 2, 1, 1, GridConstraints.ANCHOR_NORTHWEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_FIXED, new Dimension(-1, 18), new Dimension(-1, 18), new Dimension(-1, 18), 0, false));
        addressTextField = new JTextField();
        addressTextField.setBackground(new Color(-1));
        addressTextField.setCaretColor(new Color(-8224126));
        addressTextField.setForeground(new Color(-8224126));
        addressTextField.setSelectionColor(new Color(-5841665));
        panel2.add(addressTextField, new GridConstraints(1, 1, 1, 2, GridConstraints.ANCHOR_NORTH, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(150, -1), null, 0, false));
        portLbl = new JLabel();
        portLbl.setBackground(new Color(-1));
        portLbl.setForeground(new Color(-8224126));
        portLbl.setRequestFocusEnabled(true);
        portLbl.setText("Port:");
        panel2.add(portLbl, new GridConstraints(2, 0, 1, 1, GridConstraints.ANCHOR_NORTHEAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        panel2.add(portTextField, new GridConstraints(2, 1, 1, 2, GridConstraints.ANCHOR_NORTH, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(150, -1), null, 0, false));
        validateLbl = new JLabel();
        validateLbl.setBackground(new Color(-1));
        validateLbl.setForeground(new Color(-7631989));
        validateLbl.setText("");
        panel2.add(validateLbl, new GridConstraints(3, 0, 1, 2, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(100, -1), new Dimension(100, -1), new Dimension(100, -1), 0, false));
        validateBtn = new JButton();
        validateBtn.setBackground(new Color(-5841665));
        validateBtn.setBorderPainted(false);
        validateBtn.setFocusPainted(false);
        validateBtn.setForeground(new Color(-12303292));
        validateBtn.setMargin(new Insets(5, 0, 5, 0));
        validateBtn.setText("Validate");
        validateBtn.setVerticalAlignment(0);
        validateBtn.setVerticalTextPosition(0);
        panel2.add(validateBtn, new GridConstraints(3, 2, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(100, 24), new Dimension(100, 24), new Dimension(100, 24), 0, false));
        ButtonGroup buttonGroup;
        buttonGroup = new ButtonGroup();
        buttonGroup.add(hostRadBtn);
        buttonGroup.add(ipRadBtn);
        buttonGroup = new ButtonGroup();
        buttonGroup.add(registeredYesRadBtn);
        buttonGroup.add(registeredNoRadBtn);
    }

    /**
     * @noinspection ALL
     */
    public JComponent $$$getRootComponent$$$() {
        return contentPane;
    }
}
