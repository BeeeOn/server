package com.smarthome.emulator.simulations.forms;

import com.intellij.uiDesigner.core.GridConstraints;
import com.intellij.uiDesigner.core.GridLayoutManager;
import com.intellij.uiDesigner.core.Spacer;
import com.smarthome.emulator.design.JIOTButton;
import com.smarthome.emulator.design.JNumberTextField;
import com.smarthome.emulator.simulations.basis.Server;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.net.InetAddress;
import java.net.UnknownHostException;

public class ServerOptions extends JDialog {
    private JPanel contentPane;
    private JIOTButton buttonOK;
    private JIOTButton buttonCancel;
    private JLabel LblServer;
    private JLabel addressLbl;
    private JRadioButton hostRadBtn;
    private JRadioButton ipRadBtn;
    private JTextField addressTextField;
    private JLabel portLbl;
    private JNumberTextField portTextField;
    private JLabel validateLbl;
    private JButton validateBtn;

    private Server server;

    public ServerOptions(Server serve) {
        this.server = serve;
        $$$setupUI$$$();
        setContentPane(contentPane);
        setModal(true);
        setBounds(345, 100, 550, 360);
        getRootPane().setDefaultButton(buttonOK);
        UIModifications();
        if (server != null) {
            initFields();
        }
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

// call onCancel() when cross is clicked
        setDefaultCloseOperation(DO_NOTHING_ON_CLOSE);
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                onCancel();
            }
        });

// call onCancel() on ESCAPE
        contentPane.registerKeyboardAction(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                onCancel();
            }
        }, KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE, 0), JComponent.WHEN_ANCESTOR_OF_FOCUSED_COMPONENT);

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
                    validateLbl.setText("Insert host name or IP address!");
                }
            }
        });
        hostRadBtn.addItemListener(new ItemListener() {
            @Override
            public void itemStateChanged(ItemEvent e) {
                if (e.getStateChange() == ItemEvent.SELECTED) {
                    if (server != null) {
                        addressTextField.setText(server.getName());
                    }
                }
            }
        });
        ipRadBtn.addItemListener(new ItemListener() {
            @Override
            public void itemStateChanged(ItemEvent e) {
                if (e.getStateChange() == ItemEvent.SELECTED) {
                    if (server != null) {
                        addressTextField.setText(server.getIp());
                    }
                }
            }
        });
        pack();
    }


    private void onOK() {
        String address, port;
        if (!addressTextField.getText().trim().equals("") && !(addressTextField.getText() == null)) {
            address = addressTextField.getText();
        } else {
            validateLbl.setText("Address field is empty or address is not valid!");
            return;
        }
        if (!portTextField.getText().trim().equals("") && !(portTextField.getText() == null)) {
            port = portTextField.getText();
        } else {
            validateLbl.setText("Port field is empty!");
            return;
        }
        if (!validateAddress()) {
            validateLbl.setText("Invalid address!");
            return;
        }
        validateLbl.setText("");
        if (!server.update(hostRadBtn.isSelected() ? address : null, ipRadBtn.isSelected() ? address : null, Integer.valueOf(port))) {
            validateLbl.setText("Can not update server!");
            return;
        }
        dispose();
    }

    private void onCancel() {
// add your code here if necessary
        dispose();
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
            validateLbl.setText("Security exception raised!");
            return false;
        }
    }

    private void initFields() {
        addressTextField.setText(server.getName());
        portTextField.setText(String.valueOf(server.getPort()));
        hostRadBtn.setSelected(true);
    }

    private void UIModifications() {
        portTextField.setBackground(new Color(-1));
        portTextField.setCaretColor(new Color(-8224126));
        portTextField.setForeground(new Color(-8224126));
        portTextField.setMargin(new Insets(0, 5, 0, 10));
        portTextField.setSelectionColor(new Color(-5841665));
        portTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
        addressTextField.setBorder(BorderFactory.createLineBorder(new Color(220, 220, 220), 1));
    }

    private void createUIComponents() {
        // TODO: place custom component creation code here
        portTextField = new JNumberTextField(5);
        buttonOK = new JIOTButton("OK", true);
        buttonCancel = new JIOTButton("Cancel", false);
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
        final JPanel panel1 = new JPanel();
        panel1.setLayout(new GridLayoutManager(1, 1, new Insets(0, 0, 0, 0), -1, -1));
        panel1.setBackground(new Color(-1));
        contentPane.add(panel1, new GridConstraints(1, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, 1, null, null, null, 0, false));
        final JPanel panel2 = new JPanel();
        panel2.setLayout(new GridLayoutManager(1, 3, new Insets(0, 0, 0, 0), -1, -1));
        panel2.setBackground(new Color(-1));
        panel1.add(panel2, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, null, null, null, 0, false));
        buttonOK.setText("OK");
        panel2.add(buttonOK, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        buttonCancel.setText("Cancel");
        panel2.add(buttonCancel, new GridConstraints(0, 1, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final Spacer spacer1 = new Spacer();
        panel2.add(spacer1, new GridConstraints(0, 2, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_WANT_GROW, 1, null, null, null, 0, false));
        final JPanel panel3 = new JPanel();
        panel3.setLayout(new GridLayoutManager(3, 2, new Insets(0, 0, 0, 0), -1, -1));
        panel3.setBackground(new Color(-1));
        contentPane.add(panel3, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, null, null, null, 0, false));
        LblServer = new JLabel();
        LblServer.setBackground(new Color(-1));
        LblServer.setFont(new Font("Segoe UI", Font.PLAIN, 28));
        LblServer.setForeground(new Color(-13127169));
        LblServer.setText("Server Details");
        panel3.add(LblServer, new GridConstraints(0, 0, 1, 2, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final Spacer spacer2 = new Spacer();
        panel3.add(spacer2, new GridConstraints(2, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_VERTICAL, 1, GridConstraints.SIZEPOLICY_WANT_GROW, null, null, null, 0, false));
        final JPanel panel4 = new JPanel();
        panel4.setLayout(new GridLayoutManager(4, 3, new Insets(5, 2, 2, 2), -1, -1));
        panel4.setBackground(new Color(-1));
        panel3.add(panel4, new GridConstraints(1, 0, 1, 2, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, null, null, null, 0, true));
        panel4.setBorder(BorderFactory.createTitledBorder(BorderFactory.createLineBorder(new Color(-1052689)), null));
        addressLbl = new JLabel();
        addressLbl.setBackground(new Color(-1));
        addressLbl.setForeground(new Color(-8224126));
        addressLbl.setHorizontalAlignment(4);
        addressLbl.setHorizontalTextPosition(4);
        addressLbl.setRequestFocusEnabled(true);
        addressLbl.setText("Address:");
        panel4.add(addressLbl, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_NORTHEAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(80, -1), new Dimension(80, -1), null, 0, false));
        hostRadBtn = new JRadioButton();
        hostRadBtn.setBackground(new Color(-1));
        hostRadBtn.setForeground(new Color(-8224126));
        hostRadBtn.setIcon(new ImageIcon(getClass().getResource("/img/radioButtonNotSelected.png")));
        hostRadBtn.setPressedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        hostRadBtn.setSelected(true);
        hostRadBtn.setSelectedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        hostRadBtn.setText("Host");
        panel4.add(hostRadBtn, new GridConstraints(0, 1, 1, 1, GridConstraints.ANCHOR_NORTHWEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(-1, 18), new Dimension(-1, 18), new Dimension(-1, 18), 0, false));
        ipRadBtn = new JRadioButton();
        ipRadBtn.setBackground(new Color(-1));
        ipRadBtn.setForeground(new Color(-8224126));
        ipRadBtn.setIcon(new ImageIcon(getClass().getResource("/img/radioButtonNotSelected.png")));
        ipRadBtn.setPressedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        ipRadBtn.setSelectedIcon(new ImageIcon(getClass().getResource("/img/radioButtonSelected.png")));
        ipRadBtn.setText("IP");
        panel4.add(ipRadBtn, new GridConstraints(0, 2, 1, 1, GridConstraints.ANCHOR_NORTHWEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_FIXED, new Dimension(-1, 18), new Dimension(-1, 18), new Dimension(-1, 18), 0, false));
        addressTextField = new JTextField();
        addressTextField.setBackground(new Color(-1));
        addressTextField.setCaretColor(new Color(-8224126));
        addressTextField.setForeground(new Color(-8224126));
        addressTextField.setSelectionColor(new Color(-5841665));
        panel4.add(addressTextField, new GridConstraints(1, 1, 1, 2, GridConstraints.ANCHOR_NORTH, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(150, -1), null, 0, false));
        portLbl = new JLabel();
        portLbl.setBackground(new Color(-1));
        portLbl.setForeground(new Color(-8224126));
        portLbl.setRequestFocusEnabled(true);
        portLbl.setText("Port:");
        panel4.add(portLbl, new GridConstraints(2, 0, 1, 1, GridConstraints.ANCHOR_NORTHEAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        panel4.add(portTextField, new GridConstraints(2, 1, 1, 2, GridConstraints.ANCHOR_NORTH, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, new Dimension(150, -1), null, 0, false));
        validateLbl = new JLabel();
        validateLbl.setBackground(new Color(-1));
        validateLbl.setForeground(new Color(-7631989));
        validateLbl.setText("");
        panel4.add(validateLbl, new GridConstraints(3, 0, 1, 2, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(100, -1), new Dimension(100, -1), new Dimension(100, -1), 0, false));
        validateBtn = new JButton();
        validateBtn.setBackground(new Color(-5841665));
        validateBtn.setBorderPainted(false);
        validateBtn.setFocusPainted(false);
        validateBtn.setForeground(new Color(-12303292));
        validateBtn.setMargin(new Insets(5, 0, 5, 0));
        validateBtn.setText("Validate");
        validateBtn.setVerticalAlignment(0);
        validateBtn.setVerticalTextPosition(0);
        panel4.add(validateBtn, new GridConstraints(3, 2, 1, 1, GridConstraints.ANCHOR_EAST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(100, 24), new Dimension(100, 24), new Dimension(100, 24), 0, false));
        ButtonGroup buttonGroup;
        buttonGroup = new ButtonGroup();
        buttonGroup.add(hostRadBtn);
        buttonGroup.add(ipRadBtn);
    }

    /**
     * @noinspection ALL
     */
    public JComponent $$$getRootComponent$$$() {
        return contentPane;
    }
}
