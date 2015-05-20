package com.iha.emulator.ui.panels.chart;

import com.iha.emulator.utilities.watchers.Response;
import javafx.application.Platform;
import javafx.beans.property.BooleanProperty;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.scene.Node;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;
import javafx.scene.layout.StackPane;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.util.Date;

/**
 * Graphical component class providing graph, which displays {@link com.iha.emulator.utilities.watchers.Response}s
 * gathered by {@link com.iha.emulator.utilities.watchers.ResponseTracker}.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class ResponseChart {
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(ResponseChart.class);
    /** chart enabled property */
    private BooleanProperty enabled;
    /** list of chart chart data */
    ObservableList<XYChart.Data<Date,Number>> responseSeriesData = FXCollections.observableArrayList();
    /** chart series created from list of data */
    private XYChart.Series<Date,Number> series = new XYChart.Series(responseSeriesData);
    /** date-time axis */
    private DateAxis dateAxis;
    /** response time axis */
    private NumberAxis responseAxis;
    /** chart */
    private LineChart<Date,Number> linechart;
    /** chart's container component */
    private Node container;

    /**
     * Creates new chart, which is added to given container component
     * @param container container component for chart
     */
    public ResponseChart(Node container) {
        this.enabled = new SimpleBooleanProperty(false);
        this.container = container;
        createChart();
    }

    /**
     * Creates {@link javafx.scene.chart.LineChart} with date and response time axis. Binds {@link #series} as data
     * source.
     */
    private void createChart(){
        responseAxis = new NumberAxis();
        responseAxis.setAutoRanging(true);
        responseAxis.setAnimated(false);
        responseAxis.setLabel("Response time (ms)");
        dateAxis = new DateAxis();
        dateAxis.setAutoRanging(true);
        dateAxis.setAnimated(false);
        dateAxis.setLabel("Time");
        linechart = new LineChart<>(dateAxis,responseAxis);
        linechart.getData().add(series);
        linechart.setAnimated(false);
        linechart.setLegendVisible(false);
        linechart.setCreateSymbols(false);
        ((StackPane) container).getChildren().clear();
        ((StackPane) container).getChildren().add(linechart);
    }

    /**
     * Adds new response to charts data source
     * @param response new response to be added to chart
     */
    public synchronized void addResponse(Response response){
        Platform.runLater(()->responseSeriesData.add(new XYChart.Data<>(new Date(response.getTime()), response.getValue())));
    }

    /**
     * Clear chart's data
     */
    public synchronized void clearChart(){
        Platform.runLater(responseSeriesData::clear);
    }

    /**
     * Gets whether chart is enabled
     * @return <code>true</code> it is enabled, <code>false</code> otherwise
     */
    public boolean getEnabled() {
        return enabled.get();
    }

    /**
     * Chart enabled property, that can be bound
     * @return chart enabled property
     */
    public BooleanProperty enabledProperty() {
        return enabled;
    }

    /**
     * Sets whether chart is enabled
     * @param enabled <code>true</code> enables chart, <code>false</code> disables chart
     */
    public void setEnabled(boolean enabled) {
        this.enabled.set(enabled);
    }
}
