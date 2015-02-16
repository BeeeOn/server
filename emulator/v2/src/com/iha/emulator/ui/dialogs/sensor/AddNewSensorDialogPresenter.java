package com.iha.emulator.ui.dialogs.sensor;

import com.iha.emulator.control.AdapterController;
import com.iha.emulator.control.SensorController;
import com.iha.emulator.models.value.HasLinearDistribution;
import com.iha.emulator.models.value.HasNormalDistribution;
import com.iha.emulator.models.value.Value;
import com.iha.emulator.models.value.ValueFactory;
import com.iha.emulator.models.value.implemented.EmptyValue;
import com.iha.emulator.models.value.implemented.HasGenerator;
import com.iha.emulator.resources.images.sensor_types.SensorIcon;
import com.iha.emulator.ui.Presenter;
import com.iha.emulator.ui.panels.PanelPresenter;
import com.iha.emulator.ui.simulations.detailed.DetailedSimulationPresenter;
import com.iha.emulator.utilities.Utilities;
import com.iha.emulator.communication.eserver.EmulatorServerClient;
import com.iha.emulator.communication.eserver.task.ServerTask;
import com.iha.emulator.communication.eserver.task.TaskParser;
import com.iha.emulator.communication.eserver.task.implemented.CheckSensorIdTask;
import javafx.application.Platform;
import javafx.beans.binding.BooleanBinding;
import javafx.beans.binding.StringBinding;
import javafx.beans.property.BooleanProperty;
import javafx.beans.property.ListProperty;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.beans.property.SimpleListProperty;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.concurrent.Task;
import javafx.fxml.FXMLLoader;
import javafx.fxml.LoadException;
import javafx.scene.Node;
import javafx.scene.control.*;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.StackPane;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.stage.FileChooser;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.util.Callback;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.controlsfx.validation.ValidationMessage;
import org.controlsfx.validation.ValidationResult;
import org.controlsfx.validation.ValidationSupport;
import org.controlsfx.validation.Validator;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;

import java.io.*;
import java.text.DecimalFormat;

import static com.iha.emulator.models.value.Value.Generator;
import static com.iha.emulator.models.value.Value.Type;

/**
 * Created by Shu on 10.12.2014.
 */
public class AddNewSensorDialogPresenter implements Presenter,PanelPresenter{

    private static final Logger logger = LogManager.getLogger(AddNewSensorDialogPresenter.class);
    private static final String FXML_PATH = "AddNewSensorDialog.fxml";
    private static final String TEMPLATES_DEFAULT_DIR = "sensor_templates";
    private static final double REFRESH_SLIDER_VALUE_COUNT = 16;
    private static final String[] DEFAULT_COLORS = {
            "0cdf56", "65c2ff", "ffe037", "fe9e49", "ffff81", "db8fff"
    };
    private Display view;
    private Stage window;
    private Node sensorContainer;

    private BooleanProperty sensorInfoSet;
    private BooleanProperty valueInfoSet;
    private AdapterController adapterController;
    private ValidationSupport sensorGeneralValidator = new ValidationSupport();
    private String valueValidationMessage = "";
    private ValidationSupport valueValidator;
    private final DecimalFormat signalBatterySliderFormatter = new DecimalFormat( "##0" );
    private ListProperty<Value> values = new SimpleListProperty<>(FXCollections.observableArrayList());

    public interface Display{
        public Node getView();
        public void setPresenter(AddNewSensorDialogPresenter presenter);
        public TextField getSensorIdLbl();
        public TextField getSensorNameLbl();
        public ComboBox getSensorIconComboBox();
        public ColorPicker getSensorColorPicker();
        public Slider getSensorBatterySlider();
        public Slider getSensorSignalSlider();
        public Slider getSensorRefreshSlider();
        public Label getSensorBatteryValueLbl();
        public Label getSensorSignalValueLbl();
        public Label getSensorRefreshValueLbl();
        public Label getSensorHeaderSampleLbl();
        public MenuItem getCheckIdMenuItem();
        public TreeView getValuesTree();
        public VBox getValuesPanelContainer();
        public Label getNoValueSelectedLbl();
        public TextField getValueTypeTextField();
        public TextField getValueNameTextField();
        public TextField getValueTextField();
        public RadioButton getValueYesStoreHistoryRadBtn();
        public RadioButton getValueNoStoreHistoryRadBtn();
        public RadioButton getValueYesGenerateValueRadBtn();
        public RadioButton getValueNoGenerateValueRadBtn();
        public StackPane getGeneratorsContainer();
        public Label getGeneratorTypeLbl();
        public RadioButton getNormalDistributionRadBtn();
        public GridPane getNormalDistributionContainer();
        public TextField getMinNormalTextField();
        public TextField getMaxNormalTextField();
        public TextField getDevNormalTextField();
        public TextField getAvgNormalTextField();
        public RadioButton getLinearDistributionRadBtn();
        public GridPane getLinearDistributionContainer();
        public TextField getMinLinearTextField();
        public TextField getMaxLinearTextField();
        public TextField getStepLinearTextField();
        public Button getSaveValueBtn();
    }

    // SUGGESTION change valueTextField to show combo box when value is of type boolean
    public AddNewSensorDialogPresenter(Stage window,Node sensorContainer,AdapterController adapterController) {
        this.window = window;
        this.adapterController = adapterController;
        this.sensorInfoSet = new SimpleBooleanProperty(false);
        this.valueInfoSet = new SimpleBooleanProperty(false);
        this.sensorContainer = sensorContainer;
    }

    public void loadTemplate() {
        logger.trace("Trying to load from XML file");
        FileChooser fileChooser = new FileChooser();
        //Set extension filter
        FileChooser.ExtensionFilter extFilter = new FileChooser.ExtensionFilter("XML files (*.xml)", "*.xml");
        fileChooser.getExtensionFilters().add(extFilter);
        //directory
        File dir = new File(TEMPLATES_DEFAULT_DIR);
        if(!dir.exists()) dir.mkdirs();
        fileChooser.setInitialDirectory(dir);
        //Show save file dialog
        File file = fileChooser.showOpenDialog(window);
        try{
            if(file != null){
                parseAndShowXml(readFile(file));
            }
        }catch (IOException e){
            DetailedSimulationPresenter.showException(logger,"Cannot load file",e,false,null);
        } catch (DocumentException e) {
            DetailedSimulationPresenter.showException(logger, "Cannot parse loaded file", e, false, null);
        } catch (NullPointerException ex) {
            DetailedSimulationPresenter.showException(logger, "Cannot parse loaded file. Incorrect or damaged file content format.", ex, false, null);
        }
    }

    private String readFile(File file) throws IOException {
        logger.trace("reading XML file");
        FileReader fileReader = new FileReader(file);
        BufferedReader bufferedReader = new BufferedReader(fileReader);
        StringBuilder stringBuffer = new StringBuilder();
        String line;
        while ((line = bufferedReader.readLine()) != null) {
            stringBuffer.append(line);
        }
        fileReader.close();
        return stringBuffer.toString();
    }

    private void parseAndShowXml(String content) throws DocumentException ,NullPointerException {
        logger.trace("Parsing XML file");
        Document doc = DocumentHelper.parseText(content);
        Element rootElement = doc.getRootElement();
        //load general info
        String name = rootElement.element("name").getText();
        String battery = rootElement.element("battery").getText();
        String signal = rootElement.element("signal").getText();
        String refresh = rootElement.element("refresh").getText();
        String color = rootElement.element("color").getText();
        //load values
        ObservableList<Value> tmpValues = FXCollections.observableArrayList();
        for(Element valueElement : rootElement.element("values").elements()){
            //extract value type name
            String valueType = valueElement.element("type").getText();
            //create new value from this type
            Value tmpValue = ValueFactory.buildValue(valueType);
            if(tmpValue == null) throw new NullPointerException("Unknown value type -> " + valueType);
            tmpValue.setName(valueElement.element("name").getText());
            tmpValue.setValue(tmpValue.fromStringToValueType(valueElement.element("initial").getText()));
            tmpValue.setStoreHistory(valueElement.element("store_history").getText().equals("true"));
            tmpValue.setGenerateValue(valueElement.element("generate_value").getText().equals("true"));
            Element generatorElement = null;
            try{
                generatorElement = valueElement.element("generator");
            }catch (NullPointerException e){
                //it is OK, when generator element doesn't exist
            }
            if(generatorElement != null){
                String generatorType = generatorElement.element("type").getText();
                Generator generator = ValueFactory.generatorByName(generatorType);
                if(generator == null) throw new NullPointerException("Unknown generator type -> " + generatorType);
                ((HasGenerator)tmpValue).setGeneratorType(generator);
                switch (generator){
                    case NORMAL_DISTRIBUTION:
                        ((HasNormalDistribution)tmpValue).setMax(Double.valueOf(generatorElement.element("generator_values").attribute("min").getValue()));
                        ((HasNormalDistribution)tmpValue).setMin(Double.valueOf(generatorElement.element("generator_values").attribute("max").getValue()));
                        ((HasNormalDistribution)tmpValue).setDev(Double.valueOf(generatorElement.element("generator_values").attribute("dev").getValue()));
                        ((HasNormalDistribution)tmpValue).setAvg(Double.valueOf(generatorElement.element("generator_values").attribute("avg").getValue()));
                        break;
                    case LINEAR_DISTRIBUTION:
                        ((HasLinearDistribution)tmpValue).setMin(Double.valueOf(generatorElement.element("generator_values").attribute("min").getValue()));
                        ((HasLinearDistribution)tmpValue).setMax(Double.valueOf(generatorElement.element("generator_values").attribute("max").getValue()));
                        ((HasLinearDistribution)tmpValue).setStep(Double.valueOf(generatorElement.element("generator_values").attribute("step").getValue()));
                        break;
                }
            }
            tmpValues.add(tmpValue);
        }
        logger.trace("Setting data from XML to dialog");
        //show loaded data in form
        view.getSensorNameLbl().setText(name);
        view.getSensorBatterySlider().setValue(Double.valueOf(battery));
        view.getSensorSignalSlider().setValue(Double.valueOf(signal));
        view.getSensorRefreshSlider().setValue(
                Utilities.secondsToRefershSliderScale(
                        new Double(refresh).intValue()
                )
        );
        view.getSensorColorPicker().setValue(Color.web(color));
        values.clear();
        for(Value newValue : tmpValues){
            addValueToTree(newValue);
        }
        view.getValuesTree().getSelectionModel().selectLast();
    }

    public void saveTemplate() {
        if(getValueInfoSet() && values.size() > 0){
            logger.trace("Trying to save to XML file");
            FileChooser fileChooser = new FileChooser();
            //Set extension filter
            FileChooser.ExtensionFilter extFilter = new FileChooser.ExtensionFilter("XML files (*.xml)", "*.xml");
            fileChooser.getExtensionFilters().add(extFilter);
            //directory
            File dir = new File(TEMPLATES_DEFAULT_DIR);
            if(!dir.exists())
                dir.mkdirs();
            fileChooser.setInitialDirectory(dir);
            //Show save file dialog
            File file = fileChooser.showSaveDialog(window);
            try {
                if(file != null){
                    if (!file.getPath().endsWith(".xml")) {
                        file = new File(file.getPath() + ".xml");
                    }
                    writeFile(buildSensorTemplateXml().asXML(), file);
                    logger.trace("OK");
                    showInformation("File saved", "Sensor template successfully saved", "Saved to file \"" + file.getName() + "\"");
                }
            }catch (IOException e){
                DetailedSimulationPresenter.showException(logger,"Cannot save file",e,false,null);
            }
        }else{
            showMissingInfoWarning(true);
        }
    }

    private void writeFile(String content, File file) throws IOException {
        FileWriter fileWriter;
        fileWriter = new FileWriter(file);
        fileWriter.write(content);
        fileWriter.close();
    }

    private Document buildSensorTemplateXml(){
        Document doc = DocumentHelper.createDocument();
        //add general info
        Element rootElement = doc.addElement("sensor_template");
        String name;
        if(view.getSensorNameLbl().getText().equals("") && values.size() ==1)
            name = "Sensor";
        else if(view.getSensorNameLbl().getText().equals("") && values.size() ==1){
            name = "Multisensor";
        }else{
            name = view.getSensorNameLbl().getText();
        }
        rootElement.addElement("name")
                    .addText(name);
        rootElement.addElement("battery")
                    .addText(String.valueOf(new Double(view.getSensorBatterySlider().getValue()).intValue()));
        rootElement.addElement("signal")
                    .addText(String.valueOf(new Double(view.getSensorSignalSlider().getValue()).intValue()));
        rootElement.addElement("refresh")
                    .addText(String.valueOf(Utilities.refreshSliderScaleToSeconds(view.getSensorRefreshSlider().getValue())));
        rootElement.addElement("color")
                    .addText(Utilities.toRGBCode(view.getSensorColorPicker().getValue()));
        //add values info
        Element valuesElement = rootElement.addElement("values");
        for (Value value : values){
            Element valueElement = valuesElement.addElement("value");
            valueElement.addElement("type")
                    .addText(value.getValueType().getName());
            valueElement.addElement("name")
                    .addText(value.getName());
            valueElement.addElement("initial")
                    .addText(String.valueOf(value.getValue()));
            valueElement.addElement("store_history")
                    .addText(String.valueOf(value.isStoreHistory()));
            valueElement.addElement("generate_value")
                    .addText(String.valueOf(value.isGenerateValue()));
            if(((HasGenerator)value).getGeneratorType() != null){
                Element generatorElement = valueElement.addElement("generator");
                if(((HasGenerator)value).getGeneratorType() == Generator.NORMAL_DISTRIBUTION){
                    generatorElement.addElement("type")
                            .addText(Generator.NORMAL_DISTRIBUTION.getName());
                    generatorElement.addElement("generator_values")
                            .addAttribute("min", String.valueOf(((HasNormalDistribution) value).getMin()))
                            .addAttribute("max", String.valueOf(((HasNormalDistribution) value).getMax()))
                            .addAttribute("dev", String.valueOf(((HasNormalDistribution) value).getDev()))
                            .addAttribute("avg", String.valueOf(((HasNormalDistribution) value).getAvg()));
                } else if(((HasGenerator)value).getGeneratorType() == Generator.LINEAR_DISTRIBUTION){
                    generatorElement.addElement("type")
                            .addText(Generator.LINEAR_DISTRIBUTION.getName());
                    generatorElement.addElement("generator_values")
                            .addAttribute("min", String.valueOf(((HasLinearDistribution) value).getMin()))
                            .addAttribute("max",String.valueOf(((HasLinearDistribution) value).getMax()))
                            .addAttribute("step",String.valueOf(((HasLinearDistribution) value).getStep()));
                }
            }
        }
        return doc;
    }

    public void checkId() {
        Task<Object> worker = new Task<Object>() {
            @Override
            protected Object call() throws Exception {
                EmulatorServerClient server = new EmulatorServerClient();
                try{
                    server.connect();
                }catch (IOException e){
                    Platform.runLater(() -> DetailedSimulationPresenter.showException(logger, "Cannot connect to server", e, false, null));
                }
                try{
                    //composite message for server
                    ServerTask task = new CheckSensorIdTask(adapterController.getServerController().getModel().getDatabaseName(),view.getSensorIdLbl().getText());
                    //send message and wait for response
                    String messageFromServer = server.sendMessage(task.buildMessage());
                    //determine result state (OK/ERROR)
                    TaskParser.parseTaskResult(messageFromServer);
                    //if ok, parse response
                    task.parseResponse(messageFromServer);
                    //show response in table
                    Platform.runLater(()->showAdapterFromServer(((CheckSensorIdTask)task).getResult()));
                }catch (IOException e){
                    Platform.runLater(()-> DetailedSimulationPresenter.showException(logger,"Cannot read from socket",e,false,null));
                }catch (DocumentException de){
                    Platform.runLater(()-> DetailedSimulationPresenter.showException(logger,"Cannot parse server message",de,false,null));
                }catch (IllegalStateException ie){
                    Platform.runLater(()-> DetailedSimulationPresenter.showException(logger,"Error on server",ie,false,null));
                }
                return null;
            }
        };
        //create thread for background process
        Thread th = new Thread(worker);
        th.setDaemon(true);
        //run background process
        th.start();
    }

    private void showAdapterFromServer(String adapterId){
        //TODO show dialog, that id was found and if user want to set data from server
        if(adapterId == null){
            Alert dlg = new Alert(Alert.AlertType.INFORMATION,"");
            dlg.initModality(Modality.WINDOW_MODAL);
            dlg.initOwner(window);
            dlg.setTitle("Check sensor ID");
            dlg.setHeaderText("ID was not found in database");
            //dlg.getDialogPane().setContentText("Setting registered to: \"No\"");
            dlg.show();
        } else {
            showInformation(
                    "Check sensor ID",
                    "ID was found in database",
                    "ID registered to adapter \"" + adapterId + "\"");
        }
    }


    public void add() {
        if(getSensorInfoSet() && getValueInfoSet() && values.size() > 0){
            if(Utilities.isSensorIdTaken(adapterController,view.getSensorIdLbl().getText())){
                showWarning("Sensor information","This sensor ID is already taken"," Please choose another ID");
                return;
            }
            logger.debug("All info OK -> creating sensor");
            try{
                if(sensorContainer != null && adapterController != null){
                    SensorController newSensor = adapterController.createSensor(
                            sensorContainer, // sensor panel container
                            Utilities.toRGBCode(view.getSensorColorPicker().getValue()), //sensor panel header color
                            (SensorIcon)view.getSensorIconComboBox().getSelectionModel().getSelectedItem(), //sensor icon
                            values.get(),
                            false, //sensor status
                            Utilities.ipStringToInt(view.getSensorIdLbl().getText()), //ip as int
                            view.getSensorNameLbl().getText(), //sensor name
                            Double.valueOf(view.getSensorBatterySlider().getValue()).intValue(), //battery
                            Double.valueOf(view.getSensorSignalSlider().getValue()).intValue(), //signal
                            Utilities.refreshSliderScaleToSeconds(view.getSensorRefreshSlider().getValue()), //refresh time in seconds
                            adapterController.getAdapter().getProtocol() //comm.protocol
                    );
                    logger.debug("Sensor \"" + Utilities.intToIpString(newSensor.getModel().getId()) + "\" created successfully");
                    //newSensor.getValues().addAll(values.get());

                }else{
                    DetailedSimulationPresenter.showException(logger,"Cannot create new sensor, missing container or controller",null,true,null);
                }
                window.hide();
            }catch (LoadException e){
                DetailedSimulationPresenter.showException(logger,"Cannot create new sensor",e,false,null);
            }
        }else {
            showMissingInfoWarning(false);
        }
    }

    public void cancel() {
        window.hide();
    }

    public void addValue(){
        ChoiceDialog<Value.Type> dlg = new ChoiceDialog<>(Type.SENSOR_TEMPERATURE, Type.values());
        dlg.setTitle("Choose");
        dlg.getDialogPane().setContentText("Choose value type");
        dlg.show();
        dlg.resultProperty().addListener(new ChangeListener<Value.Type>() {
            @Override
            public void changed(ObservableValue<? extends Value.Type> observable, Type oldValue, Type newValue) {
                addValueToTree(newValue);
            }
        });
    }

    public void deleteValue(){
        logger.trace("Deleting values");
        TreeItem item = (TreeItem) view.getValuesTree().getSelectionModel().getSelectedItem();
        if(item != null){
            values.remove(item.getValue());
            view.getValuesTree().getRoot().getChildren().remove(item);
        }
        updateTree();
        logger.trace("Values after");
        for(Value v : values){
            logger.trace(" Value -> " + v.getName());
        }
    }

    @SuppressWarnings("unchecked")
    protected void addValueToTree(Type type) {
        logger.debug("Chosen value: " + type.getName());
        Value newValue = ValueFactory.buildValue(type);
        view.getValuesTree().getRoot().getChildren().add(new TreeItem<>(newValue));
        view.getValuesTree().getSelectionModel().selectLast();
        values.add(newValue);
    }

    protected void addValueToTree(Value newValue) {
        logger.debug("Adding new value to tree: " + newValue.getName());
        view.getValuesTree().getRoot().getChildren().add(new TreeItem<>(newValue));
        view.getValuesTree().getSelectionModel().selectLast();
        values.add(newValue);
    }

    protected void prepareValueInfoContainer(Value value){
        logger.trace("Preparing value information container for new value");
        clearValueInformation();
        valueValidator = new ValidationSupport();
        //hide generator radio buttons
        view.getNormalDistributionRadBtn().setVisible(false);
        view.getLinearDistributionRadBtn().setVisible(false);
        //show needed radio buttons
        if(value.getValueType().getGenerators() != null){
            for(Generator generator: value.getValueType().getGenerators()){
                switch (generator){
                    case NORMAL_DISTRIBUTION:
                        view.getNormalDistributionRadBtn().setVisible(true);
                        view.getMaxNormalTextField().setText(String.valueOf(((HasNormalDistribution)value).getMax()));
                        view.getMinNormalTextField().setText(String.valueOf(((HasNormalDistribution)value).getMin()));
                        view.getDevNormalTextField().setText(String.valueOf(((HasNormalDistribution)value).getDev()));
                        view.getAvgNormalTextField().setText(String.valueOf(((HasNormalDistribution)value).getAvg()));
                        break;
                    case LINEAR_DISTRIBUTION:
                        view.getLinearDistributionRadBtn().setVisible(true);
                        view.getMaxLinearTextField().setText(String.valueOf(((HasLinearDistribution)value).getMax()));
                        view.getMinLinearTextField().setText(String.valueOf(((HasLinearDistribution)value).getMin()));
                        view.getStepLinearTextField().setText(String.valueOf(((HasLinearDistribution)value).getStep()));
                        break;
                    default:
                        break;
                }
            }
            view.getValueYesGenerateValueRadBtn().setDisable(false);
        }else {
            view.getValueYesGenerateValueRadBtn().setDisable(true);
        }
        try{
            Generator generator =((HasGenerator)value).getGeneratorType();
            if(generator != null){
                logger.trace("Setting generator");
                switch (generator){
                    case NORMAL_DISTRIBUTION:
                        logger.trace("setting NORMAL_DISTRIBUTION");
                        view.getNormalDistributionRadBtn().setSelected(true);
                        view.getMaxNormalTextField().setText(String.valueOf(((HasNormalDistribution)value).getMax()));
                        view.getMinNormalTextField().setText(String.valueOf(((HasNormalDistribution)value).getMin()));
                        view.getDevNormalTextField().setText(String.valueOf(((HasNormalDistribution)value).getDev()));
                        view.getAvgNormalTextField().setText(String.valueOf(((HasNormalDistribution)value).getAvg()));
                        break;
                    case LINEAR_DISTRIBUTION:
                        logger.trace("setting LINEAR_DISTRIBUTION");
                        view.getLinearDistributionRadBtn().setSelected(true);
                        view.getMaxLinearTextField().setText(String.valueOf(((HasLinearDistribution)value).getMax()));
                        view.getMinLinearTextField().setText(String.valueOf(((HasLinearDistribution)value).getMin()));
                        view.getStepLinearTextField().setText(String.valueOf(((HasLinearDistribution)value).getStep()));
                        break;
                    default:
                        break;
                }
            }else {
                logger.trace("Value doesn't have generator");
            }
        }catch (Exception e){
            logger.trace("Doesn't have generator",e);
        }
        view.getValueTypeTextField().setText(value.getValueType().getName());
        view.getValueNameTextField().setText(value.getName());
        view.getValueTextField().setText(String.valueOf(value.getValue()));
        view.getValueYesStoreHistoryRadBtn().setSelected(value.isStoreHistory());
        view.getValueNoStoreHistoryRadBtn().setSelected(!value.isStoreHistory());
        view.getValueYesGenerateValueRadBtn().setSelected(value.isGenerateValue());
        view.getValueNoGenerateValueRadBtn().setSelected(!value.isGenerateValue());
        view.getSaveValueBtn().setOnAction(null);
        view.getSaveValueBtn().setOnAction(event -> {
            validateSaveValue();
        });
        setValueInfoSet(true);
    }

    private void clearValueInformation(){
        view.getValueTypeTextField().clear();
        view.getValueNameTextField().clear();
        view.getValueTextField().clear();
        view.getMinNormalTextField().clear();
        view.getMaxNormalTextField().clear();
        view.getDevNormalTextField().clear();
        view.getAvgNormalTextField().clear();
        view.getMinLinearTextField().clear();
        view.getMaxLinearTextField().clear();
        view.getStepLinearTextField().clear();
    }

    private void validateSaveValue(){
        valueValidationMessage = "";
        if(view.getValueNameTextField().getText().equals(""))
            valueValidationMessage = "Name is required" + "\n";
        if(view.getValueTextField().getText().equals(""))
            valueValidationMessage = valueValidationMessage +  "Value is required" + "\n";
        if(view.getNormalDistributionRadBtn().isVisible() && !view.getNormalDistributionRadBtn().isDisabled() && view.getNormalDistributionRadBtn().isSelected()){
            if(view.getMinNormalTextField().getText().equals(""))
                valueValidationMessage = valueValidationMessage +  "Normal distribution minimum is required" + "\n";
            if(view.getMaxNormalTextField().getText().equals(""))
                valueValidationMessage = valueValidationMessage +  "Normal distribution maximum is required" + "\n";
            if(view.getAvgNormalTextField().getText().equals(""))
                valueValidationMessage = valueValidationMessage +  "Normal distribution average is required" + "\n";
            if(view.getDevNormalTextField().getText().equals(""))
                valueValidationMessage = valueValidationMessage +  "Normal distribution deviation is required" + "\n";
        }
        if(view.getLinearDistributionRadBtn().isVisible() && !view.getLinearDistributionRadBtn().isDisabled() && view.getLinearDistributionRadBtn().isSelected()){
            if(view.getMinLinearTextField().getText().equals(""))
                valueValidationMessage = valueValidationMessage +  "Linear distribution minimum is required" + "\n";
            if(view.getMaxLinearTextField().getText().equals(""))
                valueValidationMessage = valueValidationMessage +  "Linear distribution maximum is required" + "\n";
            if(view.getStepLinearTextField().getText().equals(""))
                valueValidationMessage = valueValidationMessage +  "Linear distribution step is required" + "\n";
        }
        if(valueValidationMessage.equals("")){
            saveValue();
        }else{
            showWarning("Value information","Please fill all necessary information",valueValidationMessage);
        }
    }

    private void saveValue(){
        logger.trace("Saving information to value");
        Value value = (Value) ((TreeItem)view.getValuesTree().getSelectionModel().getSelectedItem()).getValue();
        try{
            value.setValue(value.fromStringToValueType(view.getValueTextField().getText()));
        }catch (NumberFormatException e){
            showWarning("Value information","Cannot parse value. Is it correct datatype?",null);
            return;
        }
        try{
            if(view.getNormalDistributionRadBtn().isVisible() && !view.getNormalDistributionRadBtn().isDisabled() && view.getNormalDistributionRadBtn().isSelected()){
                logger.trace("Saving NORMAL generator");
                ((HasNormalDistribution)value).setAvg(Double.valueOf(view.getAvgNormalTextField().getText()));
                ((HasNormalDistribution)value).setDev(Double.valueOf(view.getDevNormalTextField().getText()));
                ((HasNormalDistribution)value).setMax(Double.valueOf(view.getMaxNormalTextField().getText()));
                ((HasNormalDistribution)value).setMin(Double.valueOf(view.getMinNormalTextField().getText()));
                ((HasGenerator)value).setGeneratorType(Generator.NORMAL_DISTRIBUTION);
            }else if(view.getLinearDistributionRadBtn().isVisible() && !view.getLinearDistributionRadBtn().isDisabled() && view.getLinearDistributionRadBtn().isSelected()){
                logger.trace("Saving LINEAR generator");
                ((HasLinearDistribution)value).setStep(Double.valueOf(view.getStepLinearTextField().getText()));
                ((HasLinearDistribution)value).setMax(Double.valueOf(view.getMaxLinearTextField().getText()));
                ((HasLinearDistribution)value).setMin(Double.valueOf(view.getMinLinearTextField().getText()));
                ((HasGenerator)value).setGeneratorType(Generator.LINEAR_DISTRIBUTION);
            }
        }catch (NumberFormatException e){
            showWarning("Value information","Cannot parse distribution values.",null);
            return;
        }
        value.setName(view.getValueNameTextField().getText());
        value.setStoreHistory(view.getValueYesStoreHistoryRadBtn().isSelected());
        value.setGenerateValue(view.getValueYesGenerateValueRadBtn().isSelected());
        logger.trace("Updating tree");
        updateTree();
        setValueInfoSet(true);
        logger.trace("OK");
    }

    private void listenToValueChanges(){
        logger.trace("Adding value information change listeners");
        ChangeListener<Boolean> radBtnChangeListener = (observable, oldValue, newValue) ->
                setValueInfoSet(false);
        ChangeListener<String> textFieldChangeListener = (observable, oldValue, newValue) ->
                setValueInfoSet(oldValue.equals(newValue));
        //general info
        view.getValueNameTextField().textProperty().addListener(textFieldChangeListener);
        view.getValueTextField().textProperty().addListener(textFieldChangeListener);
        // store history
        view.getValueYesStoreHistoryRadBtn().selectedProperty().addListener(radBtnChangeListener);
        view.getValueNoStoreHistoryRadBtn().selectedProperty().addListener(radBtnChangeListener);
        //generator
        view.getValueYesGenerateValueRadBtn().selectedProperty().addListener(radBtnChangeListener);
        view.getValueNoGenerateValueRadBtn().selectedProperty().addListener(radBtnChangeListener);
        view.getNormalDistributionRadBtn().selectedProperty().addListener(radBtnChangeListener);
        view.getLinearDistributionRadBtn().selectedProperty().addListener(radBtnChangeListener);
        //generator text fields
        view.getMinNormalTextField().textProperty().addListener(textFieldChangeListener);
        view.getMaxNormalTextField().textProperty().addListener(textFieldChangeListener);
        view.getDevNormalTextField().textProperty().addListener(textFieldChangeListener);
        view.getAvgNormalTextField().textProperty().addListener(textFieldChangeListener);

        view.getMaxLinearTextField().textProperty().addListener(textFieldChangeListener);
        view.getMinLinearTextField().textProperty().addListener(textFieldChangeListener);
        view.getStepLinearTextField().textProperty().addListener(textFieldChangeListener);
    }

    private void showWarning(String title,String headerMessage,String message){
        logger.trace("Showing warning");
        Alert dlg = new Alert(Alert.AlertType.WARNING, "");
        dlg.initModality(Modality.WINDOW_MODAL);
        dlg.initOwner(this.window);
        dlg.setTitle(title);
        dlg.getDialogPane().setContentText(message);
        dlg.getDialogPane().setHeaderText(headerMessage);
        dlg.show();
    }

    private void showInformation(String title,String headerMessage,String message){
        logger.trace("Showing information");
        Alert dlg = new Alert(Alert.AlertType.INFORMATION, "");
        dlg.initModality(Modality.WINDOW_MODAL);
        dlg.initOwner(this.window);
        dlg.setTitle(title);
        dlg.getDialogPane().setContentText(message);
        dlg.getDialogPane().setHeaderText(headerMessage);
        dlg.show();
    }

    private void showMissingInfoWarning(boolean valueInfoOnly){
        logger.trace("Adapter info not filled. Cannot \"Finish\" dialog");
        String message = null;
        if(!valueInfoOnly){
            for(ValidationMessage msg : sensorGeneralValidator.getValidationResult().getErrors()){
                if(message == null){
                    message = msg.getText();
                }else {
                    message = message + "\n"  + msg.getText();
                }
            }
        }
        if(valueValidationMessage == null || valueValidationMessage.equals("")){
            valueValidationMessage = "\nMake sure at least 1 value is created and value information are saved.";
        }
        if(message == null) message = "";
        showWarning("Sensor information","Please fill all necessary information",message + valueValidationMessage);
    }

    @Override
    public Node loadView() throws IOException {
        logger.trace("Loading AddNewSensorView from: " + FXML_PATH);
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
            initialize();
            logger.trace("OK");
            return view.getView();
        }
        finally
        {
            if (fxmlStream != null) fxmlStream.close();
        }
    }

    @SuppressWarnings("unchecked")
    private void initialize(){
        //populate sensor icon combo box
        view.getSensorIconComboBox().setItems(getSensorIcons());
        view.getSensorIconComboBox().getSelectionModel().selectFirst();
        view.getSensorIconComboBox().setCellFactory(new Callback<ListView<SensorIcon>, ListCell<SensorIcon>>() {
            @Override
            public ListCell call(ListView<SensorIcon> icon) {
                return new ListCell<SensorIcon>() {
                    @Override
                    protected void updateItem(SensorIcon s, boolean bln) {
                        super.updateItem(s, bln);
                        if (s != null) {
                            ImageView image = new ImageView("/com/iha/emulator/resources/images/sensor_types/" + s.getFile());
                            setGraphic(image);
                            setText(String.valueOf(s.getName()));
                        } else {
                            setText(null);
                        }
                    }
                };
            }
        });
        view.getSensorIconComboBox().setButtonCell(new ListCell<SensorIcon>(){
            @Override
            protected void updateItem(SensorIcon s,boolean bln){
                super.updateItem(s,bln);
                if( s != null ){
                    ImageView image = new ImageView("/com/iha/emulator/resources/images/sensor_types/" + s.getFile());
                    image.setFitWidth(22);
                    image.setFitHeight(22);
                    setGraphic(image);
                }else{
                    setText(null);
                }
            }
        });
        //validation support bind with add button
        sensorGeneralValidator.validationResultProperty().addListener((observable, oldValue, newValue) -> {
            setSensorInfoSet(!sensorGeneralValidator.isInvalid());
        });
        //populate value tree
        if(view.getValuesTree().getRoot() == null){
            view.getValuesTree().setRoot(new TreeItem<Value>(new EmptyValue("Values")));
            view.getValuesTree().getRoot().setExpanded(true);
            view.getValuesTree().setShowRoot(false);
        }
        view.getValuesTree().setCellFactory(new Callback<TreeView<Value>, TreeCell<Value>>() {
            @Override
            public TreeCell call(TreeView<Value> param) {
                TreeCell<Value> treeItem = new TreeCell<Value>() {
                    @Override
                    protected void updateItem(Value v, boolean bln) {
                        super.updateItem(v, bln);
                        if (v != null) {
                            setText(v.getName());
                        } else {
                            setText(null);
                        }
                    }
                };

                return treeItem;
            }
        });
        view.getValuesTree().getSelectionModel().selectedItemProperty().addListener(new ChangeListener() {
            @Override
            public void changed(ObservableValue observable, Object oldValue, Object newValue) {
                if(oldValue != null){
                }
                if (newValue != null && newValue instanceof TreeItem) {
                    prepareValueInfoContainer((Value) ((TreeItem) newValue).getValue());
                }
            }
        });
    }

    private void updateTree(){
        TreeItem newItem = new TreeItem();
        view.getValuesTree().getRoot().getChildren().add(newItem);
        view.getValuesTree().getRoot().getChildren().remove(newItem);
    }

    private ObservableList<SensorIcon> getSensorIcons(){
        return FXCollections.observableArrayList(SensorIcon.values());
    }

    public boolean getSensorInfoSet() {
        return sensorInfoSet.get();
    }

    public BooleanProperty sensorInfoSetProperty() {
        return sensorInfoSet;
    }

    public void setSensorInfoSet(boolean sensorInfoSet) {
        this.sensorInfoSet.set(sensorInfoSet);
    }

    public boolean getValueInfoSet() {
        return valueInfoSet.get();
    }

    public BooleanProperty valueInfoSetProperty() {
        return valueInfoSet;
    }

    public void setValueInfoSet(boolean valueInfoSet) {
        this.valueInfoSet.set(valueInfoSet);
    }

    @Override
    public void addModel(Object model) {

    }

    @Override
    public Object getModel() {
        return null;
    }

    @Override
    public Node getView() {
        return view.getView();
    }

    @Override
    public void clear() {

    }

    @SuppressWarnings("unchecked")
    @Override
    public void bind() {
        view.setPresenter(this);
        //normal distribution
        view.getNormalDistributionContainer().setVisible(false);
        view.getNormalDistributionContainer().visibleProperty().bind(new BooleanBinding() {
            {
                bind(view.getNormalDistributionRadBtn().selectedProperty());
            }
            @Override
            protected boolean computeValue() {
                return view.getNormalDistributionRadBtn().isSelected();
            }
        });
        view.getLinearDistributionContainer().setVisible(false);
        view.getLinearDistributionContainer().visibleProperty().bind(new BooleanBinding() {
            {
                bind(view.getLinearDistributionRadBtn().selectedProperty());
            }
            @Override
            protected boolean computeValue() {
                return view.getLinearDistributionRadBtn().isSelected();
            }
        });
        //generators
        view.getGeneratorsContainer().visibleProperty().bind(view.getValueYesGenerateValueRadBtn().selectedProperty());
        view.getGeneratorTypeLbl().disableProperty().bind(view.getValueNoGenerateValueRadBtn().selectedProperty());
        view.getNormalDistributionRadBtn().disableProperty().bind(view.getValueNoGenerateValueRadBtn().selectedProperty());
        view.getLinearDistributionRadBtn().disableProperty().bind(view.getValueNoGenerateValueRadBtn().selectedProperty());
        //general information
        sensorGeneralValidator.registerValidator(view.getSensorNameLbl(), false, Validator.createEmptyValidator("Name is required"));
        sensorGeneralValidator.registerValidator(view.getSensorIdLbl(), false, Validator.createEmptyValidator("Id is required"));
        sensorGeneralValidator.registerValidator(view.getSensorIdLbl(), false, (Control c, String newValue) ->
                ValidationResult.fromErrorIf(view.getSensorIdLbl(), "Id must be in IP4 address format (xxx.xxx.xxx.xxx)", !Utilities.isIp(newValue)));
        sensorGeneralValidator.registerValidator(view.getSensorIconComboBox(), false, (Control c, SensorIcon icon) ->
                ValidationResult.fromErrorIf(view.getSensorIconComboBox(), "Icon is required", (icon == null)));
        // --sliders
        // -- -- battery slider
        view.getSensorBatteryValueLbl().textProperty().bind(new StringBinding() {
            {
                bind(view.getSensorBatterySlider().valueProperty());
            }
            @Override
            protected String computeValue() {
                return signalBatterySliderFormatter.format(view.getSensorBatterySlider().getValue()) + " %";
            }
        });
        // -- -- signal slider
        view.getSensorSignalValueLbl().textProperty().bind(new StringBinding() {
            {
                bind(view.getSensorSignalSlider().valueProperty());
            }
            @Override
            protected String computeValue() {
                return signalBatterySliderFormatter.format(view.getSensorSignalSlider().getValue()) + " %";
            }
        });
        // -- -- refresh slider
        view.getSensorRefreshSlider().setMax(REFRESH_SLIDER_VALUE_COUNT);
        view.getSensorRefreshValueLbl().textProperty().bind(new StringBinding() {
            {
                bind(view.getSensorRefreshSlider().valueProperty());
            }
            @Override
            protected String computeValue() {
                return Utilities.formatSeconds(Utilities.refreshSliderScaleToSeconds(view.getSensorRefreshSlider().getValue()));
            }
        });
        // -- color picker
        addCustomColorsToColorPicker(view.getSensorColorPicker());
        // -- sensor header sample
        view.getSensorHeaderSampleLbl().textProperty().bind(view.getSensorNameLbl().textProperty());
        view.getSensorIconComboBox().valueProperty().addListener((observable, oldValue, newValue) ->
                view.getSensorHeaderSampleLbl().setGraphic(
                    new ImageView(
                            new Image("/com/iha/emulator/resources/images/sensor_types/" + ((SensorIcon)newValue).getFile())
                    )
                ));
        view.getSensorColorPicker().valueProperty().addListener(new ChangeListener<Color>() {
            @Override
            public void changed(ObservableValue<? extends Color> observable, Color oldValue, Color newValue) {
                if(view.getSensorColorPicker().getValue() != null)
                    view.getSensorHeaderSampleLbl().setStyle("-fx-background-color: " + Utilities.toRGBCode(newValue) + ";");
            }
        });
        view.getSensorColorPicker().setValue(view.getSensorColorPicker().getCustomColors().get(0));
        // VALUES
        // -- values count bound to visibility of values information panel
        view.getValuesPanelContainer().visibleProperty().bind(new BooleanBinding() {
            {
                bind(values.sizeProperty());
            }
            @Override
            protected boolean computeValue() {
                return (values.size() > 0);
            }
        });
        view.getNoValueSelectedLbl().visibleProperty().bind(new BooleanBinding() {
            {
                bind(values.sizeProperty());
            }
            @Override
            protected boolean computeValue() {
                return !(values.size() > 0);
            }
        });
        // -- values tree selection action


        view.getSaveValueBtn().visibleProperty().bind(new BooleanBinding() {
            {
                bind(valueInfoSet);
            }

            @Override
            protected boolean computeValue() {
                return !valueInfoSet.get();
            }
        });
        view.getValuesTree().disableProperty().bind(new BooleanBinding() {
            {
                bind(valueInfoSet);
            }
            @Override
            protected boolean computeValue() {
                return !getValueInfoSet();

            }
        });

        //--checkId button
        view.getCheckIdMenuItem().disableProperty().bind(new BooleanBinding() {
            {
                bind(view.getSensorIdLbl().textProperty());
            }
            @Override
            protected boolean computeValue() {
                if(view.getSensorIdLbl().getText().equals("")){
                    return true;
                }else{
                    return false;
                }
            }
        });

        view.getSensorIdLbl().setPromptText("Example: 100.100.100.100");
        view.getSensorNameLbl().setPromptText("Example: Multisensor");
        listenToValueChanges();
    }

    private void addCustomColorsToColorPicker(ColorPicker picker){
        // clear default picker colors
        picker.getCustomColors().clear();
        for(String color : DEFAULT_COLORS){
            picker.getCustomColors().add(Color.web(color));
        }

    }
}