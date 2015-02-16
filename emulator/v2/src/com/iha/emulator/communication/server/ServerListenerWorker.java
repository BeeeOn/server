package com.iha.emulator.communication.server;

import com.iha.emulator.communication.protocol.Protocol;
import com.iha.emulator.communication.protocol.ProtocolFactory;
import com.iha.emulator.control.AdapterController;
import com.iha.emulator.ui.simulations.detailed.DetailedSimulationPresenter;
import javafx.application.Platform;
import javafx.beans.property.ListProperty;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

/**
 * Created by Shu on 12.2.2015.
 */
public class ServerListenerWorker implements Runnable {

    private static final Logger logger = LogManager.getLogger(ServerListenerWorker.class);

    private ListProperty<AdapterController> adapterControllersList;
    private SocketChannel socketChannel;
    private ByteBuffer reusableBuffer = ByteBuffer.allocate(2048);

    public ServerListenerWorker(SocketChannel socketChannel,ListProperty<AdapterController> adapterControllers) {
        this.socketChannel = socketChannel;
        this.adapterControllersList = adapterControllers;
    }

    public void run(){
        int bytesRead = 0;
        try{
            if (socketChannel != null) {
                reusableBuffer.clear();
                logger.debug("Received new message from server");
                bytesRead = socketChannel.read(reusableBuffer);
                //if socket suddenly closes
                if (bytesRead == -1) {
                    throw new IllegalStateException("ByteBuffer equals to -1, none or corrupted data received");
                }
                processMessage(DocumentHelper.parseText(new String(reusableBuffer.array()).substring(0, bytesRead)));
                //incomingSocket.close();
            }
        }catch ( IllegalStateException | NullPointerException | IllegalArgumentException e){
            final int finalBytesRead = bytesRead;
            Platform.runLater(() -> DetailedSimulationPresenter.showException(logger,"Cannot parse message or error on socket reading!\n" + new String(reusableBuffer.array()).substring(0, finalBytesRead),e,false,null));
        }catch (IOException e) {
            Platform.runLater(() -> DetailedSimulationPresenter.showException(logger, "Cannot read message from socket!", e, false, null));
        }catch (DocumentException e) {
            Platform.runLater(() -> DetailedSimulationPresenter.showException(logger,"Cannot parse incoming message from server",e,false,null));
        } finally {
            if(socketChannel != null) {
                try {
                    socketChannel.close();
                } catch (IOException e) {
                    Platform.runLater(() -> DetailedSimulationPresenter.showException(logger, "Cannot close socket on incoming connection", e, false, null));
                }
            }
        }
    }

    public void processMessage(Document inDocument) throws IllegalArgumentException, NullPointerException{
        logger.debug("Processing incoming message: \n" + inDocument.asXML());
        logger.trace("Building protocol");
        Protocol protocol = getMessageProtocolVersion(inDocument);
        protocol.checkProtocolVersion(inDocument);
        logger.trace("Getting adapter id");
        int adapterId = protocol.parseAdapterId(inDocument);
        logger.trace("Getting adapter controller for adapter id: " + adapterId);
        AdapterController adapterController;
        if(adapterId != 0) {
            adapterController = getAdapterControllerById(adapterId);
        }else {
            logger.debug("Cannot retrieve adapter id from incoming message. Cancel message processing.");
            return;
        }
        if(adapterController != null){
            protocol.parseInAdapterMessage(inDocument,adapterController);
        }else {
            logger.error("AdapterController for adapter " + adapterId + " NOT found");
            Platform.runLater(() -> DetailedSimulationPresenter.showException(logger, "Incoming message error. Cannot find adapter with id: " + adapterId, null, false, null)
            );
        }

    }

    public AdapterController getAdapterControllerById(int id){
        if (adapterControllersList == null) return null;
        for(AdapterController adapterController : adapterControllersList){
            if(adapterController.getAdapter().getId() == id){
                logger.trace("Adapter Controller FOUND");
                return adapterController;
            }
        }
        return null;
    }

    public Protocol getMessageProtocolVersion(Document inDocument) throws IllegalArgumentException{
        Element rootElement = inDocument.getRootElement();
        String protocolVersion = rootElement.attribute("protocol_version").getValue();
        logger.trace("Incoming message protocol: " + protocolVersion);
        return ProtocolFactory.buildProtocol(Double.valueOf(protocolVersion));
    }
}
