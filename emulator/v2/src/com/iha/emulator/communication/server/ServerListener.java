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
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;

/**
 * Created by Shu on 6.2.2015.
 */
public class ServerListener extends Thread {

    private static final Logger logger = LogManager.getLogger(ServerListener.class);

    private boolean terminate = false;
    private int port;
    private ServerSocketChannel serverSocketChannel;
    private ListProperty<AdapterController> adapterControllersList;

    public ServerListener(int port,ListProperty<AdapterController> adapterControllersList) {
        this.port = port;
        this.adapterControllersList = adapterControllersList;
    }

    public void connect() throws IOException {
        serverSocketChannel = ServerSocketChannel.open();
        serverSocketChannel.socket().bind(new InetSocketAddress(port));
        //serverSocketChannel.configureBlocking(false);
        logger.info("Server listener started on port: " + port);
    }

    public void run(){
        while(!terminate){
            SocketChannel socketChannel = null;
            try{
                logger.debug("Listening for incoming connections...");
                socketChannel = serverSocketChannel.accept();
            }catch (IOException e) {
                if(terminate){
                    logger.info("Server listener stopped.");
                    return;
                }
                Platform.runLater(() -> DetailedSimulationPresenter.showException(logger, "Server listener cannot accept new connection!", e, false, null));
            }
            logger.debug("Accepted message from server.");
            if(socketChannel != null){
                new Thread(new ServerListenerWorker(socketChannel,adapterControllersList)).start();
            }else{
                Platform.runLater(() -> DetailedSimulationPresenter.showException(logger, "Unknown error on server listener. Accepted socket is null", null, false, null));
            }

        }
    }

    public void terminateServerListener(){
        logger.info("Terminating server listener");
        this.terminate = true;
    }
}
