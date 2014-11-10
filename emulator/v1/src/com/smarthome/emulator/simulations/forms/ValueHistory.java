package com.smarthome.emulator.simulations.forms;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;

import com.intellij.uiDesigner.core.GridConstraints;
import com.intellij.uiDesigner.core.GridLayoutManager;
import com.intellij.uiDesigner.core.Spacer;
import com.smarthome.emulator.simulations.basis.Sensor;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.data.category.CategoryDataset;
import org.jfree.data.category.DefaultCategoryDataset;
import org.jfree.data.statistics.HistogramDataset;
import org.jfree.data.statistics.HistogramType;
import org.jfree.data.xy.IntervalXYDataset;

/**
 * Displays history of sensor's values in graphs. Uses {@link org.jfree.chart.JFreeChart} library.
 */
public class ValueHistory extends JDialog {
    /**
     * GUI variables
     */
    private JPanel contentPane;
    private JButton buttonOK;
    private JLabel valuesHistoryLbl;
    private JPanel mainPanel;
    private Sensor sensor;
    /**
     * height of one graph
     */
    private static final int GRAPH_HEIGHT = 170;
    /**
     * number of displaying values, Boolean values do not display
     */
    private int sensorsValuesCount = 0;

    /**
     * Gets values from {@link com.smarthome.emulator.simulations.basis.Sensor} given as parameter.
     * For valid ones (not boolean) creates graph. Also creates dialog button listeners.
     *
     * @param sensor containing values to be displayed
     */
    public ValueHistory(Sensor sensor) {
        setContentPane(contentPane);
        setModal(false);
        getRootPane().setDefaultButton(buttonOK);
        this.sensor = sensor;
        //get properties name
        String[] origNames = this.sensor.getPanel().getPropertyNamesString().split(",");
        String[] names = getPropertyNames(origNames);
        //get valid values and display them
        for (int i = 0; i < this.sensor.getValues().size(); i++) {
            if (!(this.sensor.getValues().get(i).getValue() instanceof Boolean)) {
                createHistogramPanel(i, names[i]);
                sensorsValuesCount++;
            } /*else {
                createCategoryPanel(i, names[i]);
            }*/

        }
        //sets frame height according to values count
        int height = 136 + (sensorsValuesCount * GRAPH_HEIGHT);
        setBounds(345, 100, 500, height);
        buttonOK.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                onOK();
            }
        });
        setVisible(true);
    }

    /**
     * Gets names of displaying values from all properties names of {@link com.smarthome.emulator.simulations.panels.SensorPanel}
     *
     * @param origNames
     * @return values names
     */
    private String[] getPropertyNames(String[] origNames) {
        String[] names = new String[5];
        for (int i = 0; i < 5; i++) {
            if (i < origNames.length) {
                names[i] = origNames[i];
            } else {
                names[i] = "-";
            }
        }
        return names;
    }

    private void createCategoryPanel(int index, String name) {
        JFreeChart chart = createCategoryChart(createCategoryDataset(index), index, name);
        ChartPanel panel = new ChartPanel(chart);
        panel.setLayout(new GridLayoutManager(1, 1, new Insets(0, 0, 0, 0), -1, -1));
        panel.setBackground(new Color(-5841665));
        mainPanel.add(panel, new GridConstraints(index + 1, 0, 1, 2, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_CAN_GROW, new Dimension(450, 150), new Dimension(450, 150), new Dimension(450, 150), 0, false));
    }

    /**
     * Creates {@link org.jfree.chart.ChartPanel} with {@link org.jfree.chart.JFreeChart} with
     * with given name and adds this panel to main dialog panel.
     *
     * @param index index of value in {@link com.smarthome.emulator.simulations.basis.Sensor#values} list
     * @param name  title of chart.
     */
    private void createHistogramPanel(int index, String name) {
        JFreeChart chart = createHistogramChart(createHistogramDataset(index), index, name);
        ChartPanel panel = new ChartPanel(chart);
        panel.setLayout(new GridLayoutManager(1, 1, new Insets(0, 0, 0, 0), -1, -1));
        panel.setBackground(new Color(-5841665));
        mainPanel.add(panel, new GridConstraints(index + 1, 0, 1, 2, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_CAN_GROW, new Dimension(450, 150), new Dimension(450, 150), new Dimension(450, 150), 0, false));
    }

    /**
     * Creates and returns {@link org.jfree.chart.JFreeChart} with <code>name</code> given as parameter
     *
     * @param dataset values for chart
     * @param index   index of value in {@link com.smarthome.emulator.simulations.basis.Sensor#values} list
     * @param name    title of chart
     * @return new chart
     */
    private static JFreeChart createHistogramChart(IntervalXYDataset dataset, int index, String name) {
        JFreeChart chart = ChartFactory.createHistogram(
                name,
                null,
                "x100 = percentage",
                dataset,
                PlotOrientation.VERTICAL,
                false,
                true,
                false);
        return chart;
    }

    private static JFreeChart createCategoryChart(CategoryDataset dataset, int index, String name) {
        JFreeChart chart = ChartFactory.createBarChart(
                "",
                "Count",
                "State",
                dataset,
                PlotOrientation.VERTICAL,
                false,
                false,
                false
        );
        return chart;
    }

    /**
     * Creates new dataset for {@link org.jfree.chart.JFreeChart} from {@link com.smarthome.emulator.simulations.basis.Sensor#values}'s
     * {@link com.smarthome.emulator.simulations.basis.Value} identified by index.
     *
     * @param index index of value in {@link com.smarthome.emulator.simulations.basis.Sensor#values} list
     * @return set of data filled by value's history
     */
    private IntervalXYDataset createHistogramDataset(int index) {
        HistogramDataset dataset = new HistogramDataset();
        dataset.setType(HistogramType.RELATIVE_FREQUENCY);
        ArrayList<Object> values = sensor.getValues().get(index).getHistory();
        double[] valuesArray = new double[values.size()];
        for (int i = 0; i < valuesArray.length; i++) {
            valuesArray[i] = Double.valueOf(String.valueOf(values.get(i)));
        }
        dataset.addSeries("H1", valuesArray, 40, sensor.getValues().get(index).getMin(), sensor.getValues().get(index).getMax());
        return dataset;
    }

    private CategoryDataset createCategoryDataset(int index) {
        String categoryTrue = "True";
        String categoryFalse = "False";
        DefaultCategoryDataset dataset = new DefaultCategoryDataset();
        ArrayList<Object> values = sensor.getValues().get(index).getHistory();
        for (Object value : values) {
            if ((Boolean) value) {
                dataset.addValue(1, "value", categoryTrue);
            } else {
                dataset.addValue(0, "value", categoryFalse);
            }
        }
        return dataset;
    }

    /**
     * Dispose frame
     */
    private void onOK() {
        dispose();
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
        panel1.setLayout(new GridLayoutManager(1, 2, new Insets(0, 0, 0, 0), -1, -1));
        panel1.setBackground(new Color(-1));
        contentPane.add(panel1, new GridConstraints(1, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, 1, null, null, null, 0, false));
        final JPanel panel2 = new JPanel();
        panel2.setLayout(new GridLayoutManager(1, 1, new Insets(0, 0, 0, 0), -1, -1));
        panel2.setBackground(new Color(-1));
        panel1.add(panel2, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, null, null, null, 0, false));
        buttonOK = new JButton();
        buttonOK.setBackground(new Color(-5841665));
        buttonOK.setBorderPainted(false);
        buttonOK.setForeground(new Color(-12303292));
        buttonOK.setText("OK");
        panel2.add(buttonOK, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_FIXED, new Dimension(115, 24), new Dimension(115, 24), new Dimension(115, 24), 0, false));
        final Spacer spacer1 = new Spacer();
        panel1.add(spacer1, new GridConstraints(0, 1, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_WANT_GROW, 1, null, null, null, 0, false));
        mainPanel = new JPanel();
        mainPanel.setLayout(new GridLayoutManager(5, 2, new Insets(0, 0, 0, 0), -1, -1));
        mainPanel.setBackground(new Color(-1));
        contentPane.add(mainPanel, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_BOTH, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, GridConstraints.SIZEPOLICY_CAN_SHRINK | GridConstraints.SIZEPOLICY_CAN_GROW, null, null, null, 0, false));
        valuesHistoryLbl = new JLabel();
        valuesHistoryLbl.setBackground(new Color(-1));
        valuesHistoryLbl.setFont(new Font("Segoe UI", Font.PLAIN, 28));
        valuesHistoryLbl.setForeground(new Color(-13127169));
        valuesHistoryLbl.setText("Values history");
        mainPanel.add(valuesHistoryLbl, new GridConstraints(0, 0, 1, 1, GridConstraints.ANCHOR_WEST, GridConstraints.FILL_NONE, GridConstraints.SIZEPOLICY_FIXED, GridConstraints.SIZEPOLICY_FIXED, null, null, null, 0, false));
        final Spacer spacer2 = new Spacer();
        mainPanel.add(spacer2, new GridConstraints(0, 1, 1, 1, GridConstraints.ANCHOR_CENTER, GridConstraints.FILL_HORIZONTAL, GridConstraints.SIZEPOLICY_WANT_GROW, 1, null, null, null, 0, false));
    }

    /**
     * @noinspection ALL
     */
    public JComponent $$$getRootComponent$$$() {
        return contentPane;
    }
}
