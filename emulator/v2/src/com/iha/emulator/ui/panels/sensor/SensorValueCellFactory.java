package com.iha.emulator.ui.panels.sensor;

import com.iha.emulator.models.value.Value;
import com.iha.emulator.models.value.implemented.OnOffActuatorValue;
import javafx.beans.binding.BooleanBinding;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TableCell;
import javafx.scene.layout.HBox;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

/**
 * Created by Shu on 2.12.2014.
 */
public class SensorValueCellFactory extends TableCell<Value, Value> {
    private static final Logger logger = LogManager.getLogger(SensorValueCellFactory.class);
    @Override
    protected void updateItem(Value item, boolean empty) {
        super.updateItem(item, empty);
        if (item != null) {
            switch (item.getValueType()) {
                case SENSOR_EMISSIONS:
                case SENSOR_PRESSURE:
                case SENSOR_TEMPERATURE:
                case SENSOR_NOISE:
                case SENSOR_LIGHT:
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
                            return actuator.getValue();
                        }
                    });
                    onBtn.setOnAction(event -> actuator.setValue(true));
                    offBtn.disableProperty().bind(new BooleanBinding() {
                        {
                            bind(actuator.valueProperty());
                        }
                        @Override
                        protected boolean computeValue() {
                            return !actuator.getValue();
                        }
                    });
                    offBtn.setOnAction(event -> actuator.setValue(false));
                    hBox.getChildren().addAll(onBtn,offBtn);
                    setGraphic(hBox);
                    break;
                default:
                    logger.trace("Unknown sensor type, cannot show in table");
                    break;
            }
        } else {
            setText(null);
            setGraphic(null);
        }
    }
}
