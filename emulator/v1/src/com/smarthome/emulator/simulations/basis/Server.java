package com.smarthome.emulator.simulations.basis;

import com.smarthome.emulator.Emulator;
import com.smarthome.emulator.simulations.Error;
import com.smarthome.emulator.simulations.panels.ServerPanel;
import org.dom4j.Document;

import java.io.IOException;
import java.net.*;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.util.Set;

/**
 * Stores information about server, sends messages created by {@link Sensor} to server
 */
public class Server {
    /** GUI server information panel */
    private ServerPanel panel = null;
    /** server connection indicator */
    private boolean connection;
    /** server hostname */
    private String name;
    /** server ip */
    private String ip;
    /** server address */
    private InetAddress address;
    /** server port */
    private int port;
    /** socket for sending afterwards receiving messages */
    private SocketChannel socketChannel;
    /** wrong server response indicator */
    private boolean wrongResponse = false;
    /** adapter logger */
    private Logger logger = null;
    /** number of connection warnings before writing to error log */
    private int beforeConnectWarning = 10;
    private long responseStart = 0L;
    /** timeout for socket */
    private static final int SOCKET_TIMEOUT = 5000;
    /**
     * Checks, if ether one of <code>givenName</code> or <code>givenIp</code> is set and validates them.
     * Sets connection to <code>false</code>
     * @param panel GUI panel for showing server information, not null
     * @param logger adapter logger, not null
     * @param givenName server hostname, null if <code>givenIp</code> is not null
     * @param givenIp server ip, null if <code>givenName</code> is not null
     * @param givenPort server port, not null
     */
    public Server(ServerPanel panel,Logger logger,String givenName,String givenIp,int givenPort) {
        this.panel = panel;
        this.logger = logger;
        update(givenName,givenIp,givenPort);
        setConnection(false);
    }

    public boolean update(String host,String ip,int port){
        if(host!=null && port != 0){
            try {
                setAddress(InetAddress.getByName(host));
                setName(host);
                setIp(getIpAsString(this.address));
            } catch (UnknownHostException e) {
                Emulator.showError(Error.SERVER_IP_FROM_HOST.getMessage());
                return false;
            }
        }else if (ip != null && port != 0) {
            try {
                setAddress(InetAddress.getByName(ip));
                setName(address.getHostName());
                setIp(getIpAsString(this.address));
            } catch (UnknownHostException e) {
                Emulator.showError(Error.SERVER_HOST_FROM_IP.getMessage());
                return false;
            }
        }
        if(port != 0){
            setPort(port);
        }else{
            Emulator.showError(Error.SERVER_NO_PORT.getMessage());
            return false;
        }
        return true;
    }
    /**
     * Tries to send message created by {@link Sensor} to server.
     * If successful, returns response from server.
     * <p>
     *     Shows errors:
     *     <ul>
     *         <li>SERVER_NOT_RESPONDING if server not responding over then {@link com.smarthome.emulator.simulations.basis.Server#SOCKET_TIMEOUT}</li>
     *         <li>SERVER_CONNECT if cannot connect to server</li>
     *     </ul>
     * </p>
     * @param message message according to server <-> adapter communication protocol
     * @return server response or null if an error occurred
     */
    public ByteBuffer sendMessage(Document message,boolean waitForInMessage){
        try {
            socketChannel = SocketChannel.open();
            socketChannel.socket().setSoTimeout(SOCKET_TIMEOUT);
            //tries to connect
            socketChannel.connect(new InetSocketAddress(getName(),getPort()));
            if(logger.isTrackResponse()) responseStart = System.currentTimeMillis();
            //set server connection
            this.setConnection(true);
            //System.out.println(message.asXML());
            //send message
            ByteBuffer out = ByteBuffer.wrap(message.asXML().getBytes());
            while(out.hasRemaining()){
                socketChannel.write(out);
            }
            if(waitForInMessage){
                ByteBuffer in = ByteBuffer.allocate(5024);
                int bytesRead = socketChannel.read(in);
                //if socket suddenly closes
                if(bytesRead == -1){
                    socketChannel.close();
                    setWrongResponse(true);
                    return null;
                }
                //receive response
                socketChannel.close();
                if(logger.isTrackResponse()) logger.addResponse(responseStart,System.currentTimeMillis());
                //set ByteBuffer limit to actual data inside
                in.flip();
                return in;
            }else{
                System.out.println("Not waiting for incoming message");
                socketChannel.close();
                return null;
            }
        }catch (SocketTimeoutException e) {
            logger.writeErrorMessage(Error.SERVER_NOT_RESPONDING.getMessage());
            this.setConnection(false);
            return null;
        }catch (IOException e) {
            //becouse scheduler tries to send message every 300 miliseconds,server writes an error every 10th time
            logger.writeErrorMessage(Error.SERVER_CONNECT.getMessage());
            this.setConnection(false);
            return null;
        }
        /*try {
            socketChannel = SocketChannel.open();
            socketChannel.socket().setSoTimeout(SOCKET_TIMEOUT);
            //tries to connect
            socketChannel.connect(new InetSocketAddress(getName(),getPort()));
            if(logger.isTrackResponse()) responseStart = System.currentTimeMillis();
            //set server connection
            this.setConnection(true);
            //send message
            while(message.hasRemaining()){
                socketChannel.write(message);
            }
            //receive response
            ByteBuffer in = ByteBuffer.allocate(message.limit());
            int bytesRead = socketChannel.read(in);
            //if socket suddenly closes
            if(bytesRead == -1){
                message.position(0);
                socketChannel.close();
                setWrongResponse(true);
                return null;
            }
            socketChannel.close();
            if(logger.isTrackResponse()) logger.addResponse(responseStart,System.currentTimeMillis());
            //set ByteBuffer limit to actual data inside
            in.flip();
            return in;
        }catch (SocketTimeoutException e) {
            logger.writeErrorMessage(Error.SERVER_NOT_RESPONDING.getMessage());
            this.setConnection(false);
            return null;
        }catch (IOException e) {
            //becouse scheduler tries to send message every 300 miliseconds,server writes an error every 10th time
            if(beforeConnectWarning == 0){
                logger.writeErrorMessage(Error.SERVER_CONNECT.getMessage());
                beforeConnectWarning = 10;
            }else{
                beforeConnectWarning--;
            }
            this.setConnection(false);
            return null;
        }*/
    }
    /**
     * Gets ip address to string format from {@link java.net.InetAddress} given as parameter
     * @param address
     * @return ip address formatted as string
     */
    public static String getIpAsString(InetAddress address) {
        byte[] ipAddress = address.getAddress();
        StringBuffer str = new StringBuffer();
        for(int i=0; i<ipAddress.length; i++) {
            if(i > 0) str.append('.');
            str.append(ipAddress[i] & 0xFF);
        }
        return str.toString();
    }
    /**
     * Updates server information in GUI panel
     */
    public void updatePanel(){
        if(panel != null)
            panel.refreshTable(this);
    }
    //region Getters and Setters
    /**
     * Returns last connection to server was successful
     * @return <code>true</code> if is connected, <code>false</code> otherwise
     */
    public boolean isConnected() {
        return this.connection;
    }
    /**
     * Returns server ip address
     * @return server ip address
     */
    public String getIp() {
        return ip;
    }
    /**
     * Sets server ip address and updates GUI panel
     * @param ip server ip
     */
    public void setIp(String ip) {
        this.ip = ip;
        updatePanel();
    }

    /**
     * Sets if connection to server was successful and updates GUI panel
     * @param b <code>true</code> if success, <code>false</code> otherwise
     */
    public void setConnection(boolean b) {
        this.connection = b;
        updatePanel();
    }
    /**
     * Returns server hostname
     * @return server hostname
     */
    public String getName() {
        return name;
    }
    /**
     * Sets server hostname and updates GUI panel
     * @param name
     */
    public void setName(String name) {
        this.name = name;
        updatePanel();
    }
    /**
     * Returns server address as {@link java.net.InetAddress}
     * @return server address
     */
    public InetAddress getAddress() {
        return address;
    }
    /**
     * Sets ip, hostname and address from address given as parameter, laso updates GUI panel
     * @param address address to be set
     */
    public void setAddress(InetAddress address) {
        this.address = address;
        this.ip = getIpAsString(address);
        this.name = address.getHostName();
        updatePanel();
    }
    /**
     * Returns server port
     * @return server port
     */
    public int getPort() {
        return port;
    }
    /**
     * Sets server port and updates GUI panel
     * @param port server port
     */
    public void setPort(int port) {
        this.port = port;
        updatePanel();
    }

    /**
     * Returns if last server response was in wrong format
     * @return <code>true</code> if last message's format was wrong, <code>false</code> otherwise
     */
    public boolean isWrongResponse(){
        return this.wrongResponse;
    }

    /**
     * Sets if last server response was in wrong format
     * @param b <code>true</code> if last message's format was wrong, <code>false</code> otherwise
     */
    public void setWrongResponse(boolean b){
        this.wrongResponse = b;
    }
    //endregion
}
