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
 * Created by Shu on 23/03/15.
 */
public class ResponseChart {

    private static final Logger logger = LogManager.getLogger(ResponseChart.class);

    private BooleanProperty enabled;
    ObservableList<XYChart.Data<Date,Number>> responseSeriesData = FXCollections.observableArrayList();
    private XYChart.Series<Date,Number> series = new XYChart.Series(responseSeriesData);
    private DateAxis dateAxis;
    private NumberAxis responseAxis;
    private LineChart<Date,Number> linechart;
    private Node container;

    public ResponseChart(Node container) {
        this.enabled = new SimpleBooleanProperty(false);
        this.container = container;
        createChart();
    }

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

    public synchronized void addResponse(Response response){
        Platform.runLater(()->responseSeriesData.add(new XYChart.Data<>(new Date(response.getTime()), response.getValue())));
    }

    public synchronized void clearChart(){
        Platform.runLater(responseSeriesData::clear);
    }

    public boolean getEnabled() {
        return enabled.get();
    }

    public BooleanProperty enabledProperty() {
        return enabled;
    }

    public void setEnabled(boolean enabled) {
        this.enabled.set(enabled);
    }
}
