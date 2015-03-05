package com.iha.emulator.ui.panels.sensor;

import com.iha.emulator.control.SensorController;
import com.iha.emulator.models.value.Value;
import com.iha.emulator.resources.images.sensor_types.SensorIcon;
import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.dialogs.sensor.SensorDetailsDialogPresenter;
import com.iha.emulator.ui.panels.PanelPresenter;
import com.iha.emulator.utilities.Utilities;
import javafx.application.Platform;
import javafx.beans.binding.StringBinding;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.FlowPane;
import javafx.stage.Stage;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.io.InputStream;

/**
 * Created by Shu on 28.11.2014.
 */
public class SensorPanelPresenter implements Presenter,PanelPresenter{

    private static final Logger logger = LogManager.getLogger(SensorPanelPresenter.class);
    private static final String FXML_PATH = "SensorPanel.fxml";
    private static final String CSS_PATH = "/com/iha/emulator/resources/css/theme-light.css";

    public interface Display {
        public Node getView();
        public void setPresenter(SensorPanelPresenter presenter);
        public Label getNameLbl();
        public Label getBatteryLbl();
        public Label getSignalLbl();
        public Label getRefreshTimeLbl();
        public Label getStatusLbl();
        public Label getIdLbl();
        public Button getSettingsBtn();
        public Button getConnectionBtn();
        public TableView getValueTable();
    }

    private Display view;
    private SensorController controller;
    private Node container;

    private SensorIcon iconType;
    private String hexHeaderColor;

    public SensorPanelPresenter(){

    }

    public SensorPanelPresenter(Node container){
        this.container = container;
    }

    public void connection(){
        controller.getModel().setStatus(!controller.getModel().getStatus());
    }

    public void settings(){
        getController().disable();
        SensorDetailsDialogPresenter sensorDetailsDialogPresenter;
        try{
            if(controller == null) throw new NullPointerException("Controller is null");
            Stage stage = new Stage();
            sensorDetailsDialogPresenter = new SensorDetailsDialogPresenter(stage,controller);
            stage.setTitle("Add new adapter");
            Scene scene = new Scene((Parent) sensorDetailsDialogPresenter.loadView());
            // set css for view
            logger.trace("Loading CSS from: " + CSS_PATH);
            scene.getStylesheets().add(getClass().getResource(CSS_PATH).toExternalForm());
            stage.setScene(scene);
            stage.setResizable(false);
            stage.show();
        } catch (IOException e) {
            Platform.runLater(()->Utilities.showException(logger,"Cannot load sensor details dialog",e,false,null));
        } catch (NullPointerException e){
            Platform.runLater(()->Utilities.showException(logger,"Cannot load sensor controller",e,false,null));
        }
    }

    public void deletePanel(){
        if(this.container != null){
            ((FlowPane)this.container).getChildren().remove(getView());
        }
    }

    @Override
    public Node loadView() throws IOException {
        logger.trace("Loading SensorPanelView from: " + FXML_PATH);
        InputStream fxmlStream = null;
        try
        {
            //load resource FXML
            fxmlStream = getClass().getResourceAsStream(FXML_PATH);
            FXMLLoader loader = new FXMLLoader();
            loader.load(fxmlStream);
            //remember view
            view = loader.getController();
            // bind view and presenter
            bind();
            // set css for view
            logger.trace("OK");
            return view.getView();
        }
        finally
        {
            if (fxmlStream != null) fxmlStream.close();
        }
    }

    @Override
    @SuppressWarnings("unchecked")
    public void addModel(Object newModel) {
        logger.trace("Assigning new model to SensorPanelPresenter");
        if(this.controller != null){
            logger.trace("Model already exists -> unbinding labels");
            unbindLbl(view.getStatusLbl());
            unbindLbl(view.getIdLbl());
            unbindLbl(view.getNameLbl());
            unbindLbl(view.getBatteryLbl());
            unbindLbl(view.getSignalLbl());
            unbindLbl(view.getRefreshTimeLbl());
        }else{
            logger.trace("No model = no need to unbind");
        }
        this.controller = (SensorController) newModel;
        logger.trace("New sensor model:");
        if (newModel != null) {
            logger.trace("      Status: " + this.controller.getModel().getStatus());
            logger.trace("      Name: " + this.controller.getModel().getName());
            logger.trace("      Id: " + this.controller.getModel().getId());
            logger.trace("      Battery: " + this.controller.getModel().getBattery());
            logger.trace("      Signal: " + this.controller.getModel().getSignal());
            logger.trace("      Refresh time: " + this.controller.getModel().getRefreshTime());
            logger.trace("      Protocol: " + this.controller.getModel().getProtocol().getVersion());
        }else{
            logger.trace("      Model is null");
        }

        //bind GUI labels
        view.getStatusLbl().textProperty().bind(new StringBinding() {
            {
                bind(controller.getModel().statusProperty());
            }
            @Override
            protected String computeValue() {
                if(controller.getModel().statusProperty().get()){
                    return "Connected";
                }else{
                    return "Disconnected";
                }
            }
        });
        view.getIdLbl().textProperty().bind(new StringBinding() {
            {
                bind(controller.getModel().idProperty());
            }
            @Override
            protected String computeValue() {
                return Utilities.intToIpString(controller.getModel().getId());
            }
        });
        view.getNameLbl().textProperty().bind(this.controller.getModel().nameProperty());
        view.getBatteryLbl().textProperty().bind(new StringBinding() {
            {
                bind(controller.getModel().batteryProperty());
            }
            @Override
            protected String computeValue() {
                return controller.getModel().getBattery() + "%";
            }
        });
        view.getSignalLbl().textProperty().bind(new StringBinding() {
            {
                bind(controller.getModel().signalProperty());
            }
            @Override
            protected String computeValue() {
                return controller.getModel().getSignal() + "%";
            }
        });
        view.getRefreshTimeLbl().textProperty().bind(new StringBinding() {
            {
                bind(controller.getModel().refreshTimeProperty());
            }
            @Override
            protected String computeValue() {
                return Utilities.formatSeconds(controller.getModel().getRefreshTime());
            }
        });
        //bind GUI buttons
        view.getConnectionBtn().textProperty().bind(new StringBinding() {
            {
                bind(controller.getModel().statusProperty());
            }
            @Override
            protected String computeValue() {
                if(controller.getModel().getStatus()){
                    return "Disconnect";
                }else{
                    return "Connect";
                }
            }
        });
        logger.trace("OK");
        //show values in table and bind them
        if(controller.getModel().getValues().size() == 0) return;
        // values names column, setting to bind to value name
        ((TableColumn<Value,String>)view.getValueTable().getColumns().get(0)).setCellValueFactory(new PropertyValueFactory<>("name"));
        // values column, setting to show UI item according to value's type
        ((TableColumn<Value,Object>)view.getValueTable().getColumns().get(1)).setCellValueFactory(new SensorValueFactory());
        ((TableColumn<Value,Value>)view.getValueTable().getColumns().get(1)).setCellFactory(param -> new SensorValueCellFactory());
        view.getValueTable().setItems(controller.getModel().getValues());
    }

    public void setHeaderColor(String hexColor){
        view.getNameLbl().setStyle("-fx-background-color: " + hexColor + ";");
        view.getConnectionBtn().setStyle("-fx-background-color: " + hexColor + ";");
        view.getSettingsBtn().setStyle("-fx-background-color: " + hexColor + ";");
        setHexHeaderColor(hexColor);
    }

    public void setIcon(SensorIcon icon){
        view.getNameLbl().setGraphic(
                new ImageView(
                        new Image("/com/iha/emulator/resources/images/sensor_types/" + icon.getFile())
                )
        );
        setIconType(icon);
    }

    public SensorIcon getIconType() {
        return iconType;
    }

    public void setIconType(SensorIcon iconType) {
        this.iconType = iconType;
    }

    public String getHexHeaderColor() {
        return hexHeaderColor;
    }

    public void setHexHeaderColor(String hexHeaderColor) {
        this.hexHeaderColor = hexHeaderColor;
    }

    private void unbindLbl(Label lbl){
        lbl.textProperty().unbind();
    }

    @Override
    public Object getModel() {
        return controller.getModel();
    }

    public SensorController getController(){
        return controller;
    }

    @Override
    public Node getView() {
        return view.getView();
    }

    @Override
    public void clear() {

    }

    @Override
    public void bind() {
        view.setPresenter(this);
    }

}
