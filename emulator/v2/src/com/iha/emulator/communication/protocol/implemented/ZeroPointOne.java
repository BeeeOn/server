package com.iha.emulator.communication.protocol.implemented;

import com.iha.emulator.communication.protocol.AbstractProtocol;
import com.iha.emulator.communication.protocol.SetNewValue;
import com.iha.emulator.control.AdapterController;
import com.iha.emulator.control.SensorController;
import com.iha.emulator.models.Adapter;
import com.iha.emulator.models.Sensor;
import com.iha.emulator.models.value.Value;
import javafx.application.Platform;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;

import java.util.ArrayList;
import java.util.Iterator;

/**
 * Implementation of protocol version 0.1 (see <a href="https://ant-2.fit.vutbr.cz/projects/adapter/wiki/Adapt%C3%A9r_-_server_XML_komunikace">Wiki</a> )
 *
 * @see com.iha.emulator.communication.protocol.Protocol
 * @see com.iha.emulator.communication.protocol.AbstractProtocol
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class ZeroPointOne extends AbstractProtocol {
    /**
     * Creates instance of ZeroPointOne protocol
     */
    public ZeroPointOne() {
        super(Version.ZERO_POINT_ONE.getVersion());
    }
    /**
     * <p>{@inheritDoc}</p>
     * <p>
     *     ZeroPointOne (0.1): <br>
     *     Message contains protocol version, state(data), adapter ID, firmware version, time ( when was message created )
     * </p>
     */
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
    /**
     * <p>{@inheritDoc}</p>
     * <p>
     *     ZeroPointOne (0.1): <br>
     *     Message contains sensor/actuator ID, battery value, signal value, values information (type, offset, value).
     * </p>
     */
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
    /**
     * <p>{@inheritDoc}</p>
     * <p>
     *     ZeroPointOne (0.1): <br>
     *     Message contains protocol version, state(register), adapter ID, firmware version, time ( when was message created )
     * </p>
     */
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
    /**
     * <p>{@inheritDoc}</p>
     */
    @Override
    public Document convertInMessageToXML(String inMessage) throws DocumentException, NullPointerException {
        Document document;
        try{
            document = DocumentHelper.parseText(inMessage);
        }catch (DocumentException e){
            throw new DocumentException("Unparsable message from server: \n" + (inMessage.isEmpty() ? "message is empty": inMessage));
        }
        if(document == null) throw new NullPointerException("ZeroPointOne protocol -> parseInMessage -> document=null");
        return document;
    }
    /**
     * <p>{@inheritDoc}</p>
     * <p>
     *     ZeroPointOne (0.1): <br>
     *     Checks, if protocol version in message equals 0.1
     * </p>
     */
    @Override
    public Document checkProtocolVersion(Document inDocument) throws IllegalArgumentException, NullPointerException{
        Element rootElement = inDocument.getRootElement();
        //get protocol version
        String protocol = rootElement.attribute("protocol_version").getValue();
        //check protocol version
        if(!protocol.equals(String.valueOf(getVersion()))){
            throw new IllegalArgumentException(" --> Protocols doesn't match! (in/current): " + protocol + " / " + getVersion());
        }
        getLogger().trace("Protocol version match");
        return inDocument;
    }
    /**
     * <p>{@inheritDoc}</p>
     * <p>
     *     ZeroPointOne(0.1):
     * </p>
     * <ul>
     *    <li>Processes states:</li>
     *    <li>update - changes sensor's refresh time, if server sends different from currently set</li>
     * </ul>
     *
     * @param inDocument Dom4j XML document containing response message from server
     * @param senderController controller of sensor, for which was message sent
     * @param adapterController controller of adapter, which own sensor
     * @throws IllegalArgumentException if sensor ID or time XML element doesn't exist in received message
     * @throws NullPointerException if trying to parse XML element that should be in message, but is not.
     */
    @Override
    public void parseInSensorMessage(Document inDocument, SensorController senderController, AdapterController adapterController) throws IllegalArgumentException,NullPointerException{
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
                    if(senderController == null || senderController.getModel() == null || adapterController == null){
                        getLogger().error("Null where should not be null");
                        return;
                    }
                    if(senderController.getModel().getRefreshTime() != timeInt && timeInt != 0 && adapterController != null && !senderController.getIgnoreRefreshChange()){
                        senderController.setNewRefreshTime(timeInt);
                        if(senderController.isFullMessage()){
                            Platform.runLater(() -> adapterController.getLog().log("Adapter/" + adapterController.getAdapter().getId() + " -> Sensor/" + senderController.getModel().getId() + " new Refresh time --> " + timeInt));
                        }else {
                            Platform.runLater(() -> adapterController.getLog().log("Sensor " + senderController.getModel().getName() + "/" + senderController.getModel().getId() + " new Refresh time --> " + time));
                        }
                    }
                }else{
                    throw new IllegalArgumentException(" --> Id or time in server response doesn't exist");
                }
                break;
        }
    }

    /**
     * <p>{@inheritDoc}</p>
     * <p>
     *     ZeroPointOne(0.1):
     * </p>
     *<ul>
     *     <li>Processes states:</li>
     *     <li>set - sets new value on sensor with given ID</li>
     *     <li>register - response to register message, true if adapter was successfully registered, false otherwise</li>
     *     <li>clean - delete sensor with given ID</li>
     *     <li>listen - sensor pairing ( only prints message to log )</li>
     * </ul>
     *
     * @param inDocument Dom4j XML document containing message from server
     * @param adapterController controller of adapter, which received message
     * @throws NullPointerException if trying to parse XML element that should be in message, but is not.
     * @throws IllegalArgumentException if message state is unknown or one of the message arguments is wrong
     * (example: sensor ID doesn't exist). More info in exception message
     */
    @Override
    public void parseInAdapterMessage(Document inDocument, AdapterController adapterController) throws NullPointerException,IllegalArgumentException{
        getLogger().debug("Parsing incoming message -> getting state");
        Element rootElement = inDocument.getRootElement();
        String state = rootElement.attribute("state").getValue();
        getLogger().trace("State = " + state);
        int sensorId;
        switch (state){
            case "set":
                sensorId = Integer.valueOf(rootElement.attribute("id").getValue());
                getLogger().debug("Sensor Ip: " + sensorId);
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
            case "register":
                Boolean response = Boolean.valueOf(rootElement.attributeValue("response"));
                if(response == null) throw new IllegalArgumentException("Cannot parse \"response\" attribute in incoming message");
                if(response){
                    Platform.runLater(()->adapterController.getAdapter().setRegistered(true));
                }else{
                    throw new IllegalArgumentException("Register response is FALSE");
                }
                return;
            case "clean":
                sensorId = Integer.valueOf(rootElement.attribute("id").getValue());
                adapterController.cleanSensor(sensorId);
                return;
            case "listen":
                Platform.runLater(()->adapterController.sendMessage("Server requested \"listen\" (sensor pairing)"));
                return;
            default:
                getLogger().warn("Unknown state");
                throw new IllegalArgumentException("Unknown message state: " + state);
        }
    }
}
