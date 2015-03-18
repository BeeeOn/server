package com.iha.emulator.ui.panels.server.response;

import com.iha.emulator.control.task.SimulationTask;
import javafx.animation.Timeline;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;
import javafx.scene.layout.StackPane;


/**
 * Created by Shu on 9.3.2015.
 */
public class ResponseChart {

    private StackPane container;
    private SimulationTask task;
    private XYChart.Series<Number,Number>  responseSeries;
    private NumberAxis timeAxis;
    private NumberAxis responseAxis;
    private Timeline timer;


    public ResponseChart(StackPane container,SimulationTask task) {
        this.container = container;
        this.task = task;
    }

    public void createChart(){

    }



}
