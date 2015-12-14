package com.smarthome.emulator;

import com.intellij.uiDesigner.core.GridConstraints;
import com.intellij.uiDesigner.core.GridLayoutManager;
import com.intellij.uiDesigner.core.Spacer;
import com.smarthome.emulator.simulations.*;
import com.smarthome.emulator.simulations.Error;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.*;

/**
 * Main application class, which extends class {@link javax.swing.JFrame}. Opens frame in which
 * user chooses {@link com.smarthome.emulator.simulations.DetailedSimulation} or
 * {@link com.smarthome.emulator.simulations.PerformanceSimulation}
 */
public class Emulator extends JFrame {
    /* GUI variables */
    /**
     * Main frame panel
     */
    private JPanel mainPanel;
    /**
     * Main panel label
     */
    private JLabel mainLabel;
    /**
     * button for "Detailed Simulation"
     */
    private JButton btnDetailedSimulation;
    /**
     * button for "Performance Simulation"
     */
    private JButton btnPerformanceSimulation;
    /*Class variables*/
    /**
     * Help variable allowing to write to console
     */
    public static boolean writeConsole = true;
    public static boolean FILE_OUTPUT = true;
    /**
     * Application color's
     */
    public static final Color simColorBlue = new Color(54, 177, 255);
    public static final Color simColorBlueHover = new Color(116, 200, 254);
    public static final Color simColorAdapterHover = new Color(94, 192, 254);
    public static final Color simColorAdapterInactive = new Color(166, 220, 255);
    public static final Color simColorAdapterAdd = new Color(198, 198, 198);
    public static final Color simColorConnectionGrey = new Color(198, 198, 198);
    public static final Color simColorConnectionBlue = new Color(166, 220, 255);

    /**
     * Main application method. Creates new instance of this class
     *
     * @param args - application arguments, not used
     */
    public static void main(String[] args) {
        Emulator emulator = new Emulator();
        emulator.setVisible(true);
        if (FILE_OUTPUT) {
            try {
                System.setOut(new PrintStream(new File("emu_out.txt")));
                System.setErr(new PrintStream(new File("emu_out.txt")));
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * Sets Look&Feel to "Nimbus", load fonts from library. Paints buttons for "Detailed" and
     * "Performance Simulation". Also assigns mouse click listeners for these buttons.
     */
    public Emulator() {
        super("SmartHome Emulator");
        //set look&feel
        try {
            for (UIManager.LookAndFeelInfo info : UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    UIManager.setLookAndFeel(info.getClassName());
                    break;
                } else if ("Windows".equals(info.getName())) {
                    UIManager.setLookAndFeel(info.getClassName());
                    break;
                } else if ("Metal".equals(info.getName())) {
                    UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (Exception e) {
            showError(Error.SET_LOOK_AND_FEEL.getMessage());
        }
        //load fonts
        try {
            GraphicsEnvironment ge =
                    GraphicsEnvironment.getLocalGraphicsEnvironment();
            ge.registerFont(Font.createFont(Font.TRUETYPE_FONT, getClass().getResourceAsStream("/fonts/SEGOEUI.TTF")));
            ge.registerFont(Font.createFont(Font.TRUETYPE_FONT, getClass().getResourceAsStream("/fonts/SEGOEUIB.TTF")));
            ge.registerFont(Font.createFont(Font.TRUETYPE_FONT, getClass().getResourceAsStream("/fonts/SEGOEUII.TTF")));
            ge.registerFont(Font.createFont(Font.TRUETYPE_FONT, getClass().getResourceAsStream("/fonts/SEGOEUIL.TTF")));
            ge.registerFont(Font.createFont(Font.TRUETYPE_FONT, getClass().getResourceAsStream("/fonts/SEGOEUIZ.TTF")));
            ge.registerFont(Font.createFont(Font.TRUETYPE_FONT, getClass().getResourceAsStream("/fonts/SEGUISB.TTF")));
        } catch (IOException e) {
            showError(Error.FONT_OPEN.getMessage());
        } catch (FontFormatException e) {
            showError(Error.FONT_REGISTER.getMessage());
        }
        setContentPane(mainPanel);
        pack();
        //add action listeners
        btnDetailedSimulation.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                EventQueue.invokeLater(new Runnable() {
                    public void run() {
                        try {
                            DetailedSimulation detSim = new DetailedSimulation();
                            detSim.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                        } catch (Exception e) {
                            showError(Error.OPEN_DETAILED_SIMULATION.getMessage());
                        }
                    }
                });
                SwingUtilities.getWindowAncestor(e.getComponent()).dispose();
            }
        });
        btnPerformanceSimulation.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
               /* EventQueue.invokeLater(new Runnable() {
                    public void run() {
                        try {
                            PerformanceSimulation perSim = new PerformanceSimulation();
                            perSim.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                        } catch (Exception e) {
                            showError(Error.OPEN_PERFORMANCE_SIMULATION.getMessage());
                        }
                    }
                });
                SwingUtilities.getWindowAncestor(e.getComponent()).dispose();*/
            }
        });
        setLocationRelativeTo(null);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        UIManager.put("Tree.collapsedIcon", new ImageIcon(getClass().getResource("/img/tree/collapsed.png")));
        UIManager.put("Tree.expandedIcon", new ImageIcon(getClass().getResource("/img/tree/expanded.png")));
    }

    /**
     * Shows error message dialog with message from enum {@link com.smarthome.emulator.simulations.Error} given as parameter.
     *
     * @param message error message from enum {@link com.smarthome.emulator.simulations.Error}
     */
    public static void showError(String message) {
        JOptionPane.showMessageDialog(null, message + "!", "Error!", JOptionPane.ERROR_MESSAGE);
    }

    /**
     * Shows message given as parameter in dialog frame.
     *
     * @param message
     */
    public static void showMessage(String message) {
        JOptionPane.showMessageDialog(null, message, "Message", JOptionPane.OK_OPTION);
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
        mainPanel = new JPanel();
        mainPanel.setLayout(new GridLayoutManager(2, 3, new Insets(15, 15, 15, 15), -1, -1));
        mainPanel.setBackground(new Color(-1));
        mainLabel = new JLabel();
        mainLabel.setFont(new Font("Segoe UI", Font.PLAIN, 24));
        mainLabel.setForeground(new Color(-13127169));
        mainLabel.setText("SmartHome Emulator");
        mainPanel.add(mainLabel, new GridConstraints(0, 0, 1, 3, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final Spacer spacer1 = new Spacer();
        mainPanel.add(spacer1, new GridConstraints(1, 1, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_VERTICAL, 1, GridConstraints.SIZEPOLICY_WANT_GROW, null, null, null, 0, false));
        btnDetailedSimulation = new JButton();
        btnDetailedSimulation.setBackground(new Color(-13127169));
        btnDetailedSimulation.setBorderPainted(false);
        btnDetailedSimulation.setFocusPainted(false);
        btnDetailedSimulation.setFont(new Font("Segoe UI", Font.PLAIN, 24));
        btnDetailedSimulation.setForeground(new Color(-1));
        btnDetailedSimulation.setHideActionText(false);
        btnDetailedSimulation.setHorizontalTextPosition(0);
        btnDetailedSimulation.setMargin(new Insets(8, 14, 8, 14));
        btnDetailedSimulation.setOpaque(true);
        btnDetailedSimulation.setRequestFocusEnabled(true);
        btnDetailedSimulation.setText("<html><center>Detailed Simulation</center></html>");
        mainPanel.add(btnDetailedSimulation, new GridConstraints(1, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(200, 147), new Dimension(200, 147), new Dimension(200, 147), 0, false));
        btnPerformanceSimulation = new JButton();
        btnPerformanceSimulation.setActionCommand("<html><center>Performance Simulation</center></html>");
        btnPerformanceSimulation.setBackground(new Color(-13127169));
        btnPerformanceSimulation.setBorderPainted(false);
        btnPerformanceSimulation.setEnabled(false);
        btnPerformanceSimulation.setFocusPainted(false);
        btnPerformanceSimulation.setFont(new Font("Segoe UI", Font.PLAIN, 24));
        btnPerformanceSimulation.setForeground(new Color(-1));
        btnPerformanceSimulation.setLabel("<html><center>Performance Simulation</center></html>");
        btnPerformanceSimulation.setMargin(new Insets(8, 14, 8, 14));
        btnPerformanceSimulation.setText("<html><center>Performance Simulation</center></html>");
        mainPanel.add(btnPerformanceSimulation, new GridConstraints(1, 2, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, new Dimension(200, 147), new Dimension(200, 147), new Dimension(200, 147), 0, false));
    }

    /**
     * @noinspection ALL
     */
    public JComponent $$$getRootComponent$$$() {
        return mainPanel;
    }
}
