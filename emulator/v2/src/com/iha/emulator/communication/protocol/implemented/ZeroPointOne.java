package com.iha.emulator.communication.protocol.implemented;

import com.iha.emulator.communication.protocol.AbstractProtocol;
import com.iha.emulator.communication.protocol.SetNewValue;
import com.iha.emulator.control.AdapterController;
import com.iha.emulator.control.SensorController;
import com.iha.emulator.models.Adapter;
import com.iha.emulator.models.Sensor;
import com.iha.emulator.models.value.Value;
import com.iha.emulator.utilities.Utilities;
import javafx.application.Platform;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;

import java.util.ArrayList;
import java.util.Iterator;

/**
 * Created by Shu on 26.11.2014.
 */
public class ZeroPointOne extends AbstractProtocol {

    public ZeroPointOne() {
        super(Version.ZERO_POINT_ONE.getVersion());
    }

    @Override
    public Element buildAdapterMessage(Adapter adapter) {
        Document document = DocumentHelper.createDocument();
        Element root = document.addElement("adapter_server")
                .addAttribute("protocol_version",String.valueOf(adapter.getProtocolVersion()))
                .addAttribute("state","data")
                .addAttribute("adapter_id",getHexAdapterId(adapter))
                .addAttribute("fw_version",String.valueOf(adapter.getFirmware()))
                .addAttribute("time",String.valueOf((System.currentTimeMillis()/1000L)));
        return root;
    }

    @Override
    public Document buildSensorMessage(Element rootElement,Sensor sensor) {
        //save device tag
        Element deviceElement = rootElement.addElement("device")
                .addAttribute("id", getHexSensorId(sensor));
        //save battery tag
        deviceElement.addElement("battery")
                .addAttribute("value",String.valueOf(sensor.getBattery()));
        //save signal tag
        deviceElement.addElement("rssi")
                .addAttribute("value",String.valueOf(sensor.getSignal()));
        //save values tag

        Element valuesElement = deviceElement.addElement("values")
                .addAttribute("count",String.valueOf(sensor.getValues().size()));
        //save individual values
        for(Value value : sensor.getValues()){
            valuesElement.addElement("value")
                    .addAttribute("type",value.getValueType().getType())
                    .addAttribute("offset",getOffsetAsHexString(value.getOffset()))
                    .addText(value.asMessageString());
        }
        return rootElement.getDocument();
    }

    @Override
    public Document buildRegisterMessage(AdapterController adapterController){
        Document document = DocumentHelper.createDocument();
        Element root = document.addElement("adapter_server")
                .addAttribute("protocol_version",String.valueOf(adapterController.getAdapter().getProtocolVersion()))
                .addAttribute("state","register")
                .addAttribute("adapter_id",getHexAdapterId(adapterController.getAdapter()))
                .addAttribute("fw_version",String.valueOf(adapterController.getAdapter().getFirmware()))
                .addAttribute("time",String.valueOf((System.currentTimeMillis()/1000L)));
        root.addElement("tmp");
        return document;
    }

    @Override
    public Document convertInMessageToXML(String inMessage) throws DocumentException, NullPointerException {
        Document document;
        try{
            document = DocumentHelper.parseText(inMessage);
        }catch (DocumentException e){
            getLogger().error("Unparsable message from server: " + inMessage,e);
            throw new DocumentException("Unparsable message from server: " + inMessage);
        }
        if(document == null) throw new NullPointerException("ZeroPointOne protocol -> parseInMessage -> document=null");
        return document;
    }

    @Override
    public Document checkProtocolVersion(Document inDocument) throws IllegalArgumentException, NullPointerException{
        Element rootElement = inDocument.getRootElement();
        //get protocol version
        String protocol = rootElement.attribute("protocol_version").getValue();
        //check protocol version
        if(!protocol.equals(String.valueOf(getVersion()))){
            throw new IllegalArgumentException(" --> Protocols doesn't match! (in/current): " + protocol + " / " + getVersion());
        }
        return inDocument;
    }

    @Override
    public void parseInSensorMessage(Document inDocument, SensorController senderController, AdapterController adapterController) throws NullPointerException{
        Element rootElement = inDocument.getRootElement();
        //check state
        String state = rootElement.attribute("state").getValue();
        //get attributes same for all messages
        String id = rootElement.attribute("id").getValue();
        String time = rootElement.attribute("time").getValue();
        switch (state){
            case "update":
                if(id!= null && time != null){
                    int timeInt = Integer.valueOf(time);
                    if(timeInt < 0) throw new IllegalArgumentException(" --> server trying to set refresh rate " + timeInt);
                    if(senderController.getModel().getRefreshTime() != timeInt && timeInt != 0 && adapterController != null && !senderController.getIgnoreRefreshChange()){
                        senderController.setNewRefreshTime(timeInt);
                        if(senderController.isFullMessage()){
                            Platform.runLater(() -> adapterController.getLog().log("Adapter/" + adapterController.getAdapter().getId() + " -> Sensor/" + senderController.getSensorIdAsIp() + " new Refresh time --> " + timeInt));
                        }else {
                            Platform.runLater(() -> adapterController.getLog().log("Sensor " + senderController.getModel().getName() + "/" + senderController.getSensorIdAsIp() + " new Refresh time --> " + time));
                        }
                    }
                }else{
                    throw new IllegalArgumentException(" --> Id or time in server response doesn't exist");
                }
                break;
        }
    }

    @Override
    public int parseAdapterId(Document inDocument) throws NullPointerException,NumberFormatException{
        Element rootElement = inDocument.getRootElement();
        String state = rootElement.attribute("state").getValue();
        getLogger().trace("Incoming message state: " + state);
        switch (state){
            case "set":
                return Integer.valueOf(rootElement.attributeValue("debug_adapter_id"));
            case "listen":
                return Integer.valueOf(rootElement.attribute("adapter_id").getValue());
        }
        return 0;
    }

    @Override
    public void parseInAdapterMessage(Document inDocument, AdapterController adapterController) throws NullPointerException,IllegalArgumentException{
        getLogger().debug("Parsing incoming message");
        Element rootElement = inDocument.getRootElement();
        String state = rootElement.attribute("state").getValue();
        getLogger().trace("State = " + state);
        switch (state){
            case "set":
                int sensorId = Integer.valueOf(rootElement.attribute("id").getValue());
                getLogger().debug("Sensor Ip: " + Utilities.intToIpString(sensorId));
                ArrayList<SetNewValue> newValues = new ArrayList<>();
                for(Iterator i = rootElement.elementIterator();i.hasNext();){
                    Element current = (Element) i.next();
                    if(current.getName().equalsIgnoreCase("value")){
                        newValues.add(new SetNewValue(current.attribute("type").getValue(),current.attribute("offset").getValue(),current.getText()));
                    }
                }
                getLogger().trace("Number of values, to be changed: " + newValues.size());
                adapterController.changeValueOnSensor(sensorId,newValues);

                return;
        }
        getLogger().trace("Unknown state");
    }



}
