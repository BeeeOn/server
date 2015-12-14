package com.iha.emulator.server;

import com.iha.emulator.server.task.Task;
import com.iha.emulator.server.task.TaskParser;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.dom4j.*;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

/**
 * Class serving request from one client. Ran in separate thread, so server can accept more connections while this is
 * processed.
 *
 * @author <a href="mailto:xsutov00@stud.fit.vutbr.cz">Filip Sutovsky</a>
 */
public class Worker implements Runnable{
    /** Log4j2 logger field */
    private static final Logger logger = LogManager.getLogger(Server.class);
    /** xml document factory */
    private static DocumentFactory xmlFactory = DocumentFactory.getInstance();
    /** client socket */
    private Socket clientSocket = null;
    /** information about database from configuration file */
    private DatabaseInfo databaseInfo;

    /**
     * Creates new worker responsible for receiving and processing client request.
     *
     * @param clientSocket connected client socket
     * @param dbInfo information about database from configuration file
     */
    public Worker(Socket clientSocket,DatabaseInfo dbInfo) {
        this.clientSocket = clientSocket;
        this.databaseInfo = dbInfo;
    }

    /**
     * Method reads message on client socket, processes this message and sends server's response. Afterwards is connection
     * closed.
     */
    @Override
    public void run() {
        try {
            logger.info("Serving request from ip-> " + clientSocket.getInetAddress().getHostAddress());
            BufferedReader stdIn = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            PrintWriter out = new PrintWriter(clientSocket.getOutputStream());
            String clientInput,message = "";
            logger.debug("Receiving message from client:");
            while ((clientInput = stdIn.readLine()) != null) {
                message = message + clientInput;
                if(message.endsWith("</emulator_server>"))
                    break;
            }
            logger.debug(message);
            message  = parseMessage(message);
            logger.debug("Sending message:");
            logger.debug(message);
            out.println(message);
            out.flush();
            logger.debug("Message sent successfully");
            logger.info("Request from -> " + clientSocket.getInetAddress().getHostAddress() + " -> served successfully");
            stdIn.close();
            out.close();
        } catch (IOException e) {
            //report exception somewhere.
            logger.error("Cannot get communication streams",e);
        }finally {
            try {
                clientSocket.close();
            } catch (IOException e) {
                logger.error("Cannot close socket",e);
            }
        }
    }

    /**
     * Parse XML message given as string.
     *
     * @param clientMessageString XML message content
     * @return server's response in XML format as string
     */
    private String parseMessage(String clientMessageString){
        logger.trace("Parsing message");
        try{
            return parseXml(clientMessageString);
        }catch (DocumentException e ){
            logger.error("Cannot parse message from client",e);
            return serverErrorMessage("Cannot parse message from client");
        }
    }

    /**
     * Parse given XML content, creates new {@link com.iha.emulator.server.DatabaseInfo} from one created from configuration
     * file and inserts database name parsed from message. Creates and resolves EmulatorServer {@link com.iha.emulator.server.task.Task}.
     *
     * @param clientMessageString message from client in XML format
     * @return server's response in XML format as string
     * @throws DocumentException
     */
    private String parseXml(String clientMessageString) throws DocumentException {
        //create XML document from received string
        Document doc = DocumentHelper.parseText(clientMessageString);
        //extract db name stored as root element attribute
        String databaseName = doc.getRootElement().attributeValue("db");
        //extract task element
        Element taskElement =   doc.getRootElement().element("task");
        DatabaseInfo tmpDbInfo = new DatabaseInfo(databaseInfo.getHost(),databaseInfo.getPort(),databaseInfo.getUser(),databaseInfo.getPass());
        tmpDbInfo.setDbName(databaseName);
        //determine task
        Task task = TaskParser.parseTask(tmpDbInfo,taskElement);
        //determine task and resolve it
        logger.trace("Resolving task");
        if(task != null){
            return buildMessage(task.resolveTask());
        }else{
            logger.error("Cannot resolve task");
            return serverErrorMessage("Cannot resolve task");
        }

    }

    /**
     * Wraps task result in default element
     * @param result task's result
     * @return response ready to be sent to client
     */
    private String buildMessage(Element result){
        Document doc = xmlFactory.createDocument();
        doc.addElement("server_emulator")
                .add(result);
        return doc.asXML();
    }

    /**
     * When error on server occurs, error message is created. This message is
     * ready to be sent to client. This is global server error, not specific
     * task error.
     *
     * @param error error message for client
     * @return error message ready to be sent to client
     */
    private String serverErrorMessage(String error){
        Document doc = xmlFactory.createDocument();
        Element root = doc.addElement("server_emulator")
                .addElement("result")
                .addAttribute("state","ERROR")
                .addText(error);
        return doc.asXML();
    }


}
