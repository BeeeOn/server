package com.iha.emulator.ui.panels.sensor;

import com.iha.emulator.models.value.implemented.OnOffActuatorValue;
import com.iha.emulator.models.value.Value;
import javafx.beans.binding.BooleanBinding;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TableCell;
import javafx.scene.layout.HBox;

/**
 * Created by Shu on 2.12.2014.
 */
public class SensorValueCellFactory extends TableCell<Value, Value> {
    @Override
    protected void updateItem(Value item, boolean empty) {
        super.updateItem(item, empty);
        if (item != null) {
            switch (item.getValueType()) {
                case SENSOR_TEMPERATURE:
                case SENSOR_HUMIDITY:
                    Label lbl = new Label();
                    lbl.textProperty().bind(item.stringValueProperty());
                    setGraphic(lbl);
                    break;
                case ACTUATOR_ON_OFF:
                    HBox hBox = new HBox();
                    OnOffActuatorValue actuator = (OnOffActuatorValue) item;
                    Button onBtn = new Button("ON");
                    onBtn.getStyleClass().add("actuatorControlBtn");
                    Button offBtn = new Button("OFF");
                    offBtn.getStyleClass().add("actuatorControlBtn");
                    onBtn.disableProperty().bind(new BooleanBinding() {
                        {
                            bind(actuator.valueProperty());
                        }
                        @Override
                        protected boolean computeValue() {
                            if(actuator.getValue()){
                                return true;
                            }else{
                                return false;
                            }
                        }
                    });
                    onBtn.setOnAction(event -> actuator.setValue(true));
                    offBtn.disableProperty().bind(new BooleanBinding() {
                        {
                            bind(actuator.valueProperty());
                        }
                        @Override
                        protected boolean computeValue() {
                            if(actuator.getValue()){
                                return false;
                            }else{
                                return true;
                            }
                        }
                    });
                    offBtn.setOnAction(event -> actuator.setValue(false));
                    hBox.getChildren().addAll(onBtn,offBtn);
                    setGraphic(hBox);
                    break;
            }
        } else {
            setText(null);
            setGraphic(null);
        }
    }
}
