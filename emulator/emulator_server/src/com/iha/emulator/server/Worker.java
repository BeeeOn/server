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
 * Created by Shu on 5.12.2014.
 */
public class Worker implements Runnable{

    private static final Logger logger = LogManager.getLogger(Server.class);
    private static DocumentFactory xmlFactory = DocumentFactory.getInstance();

    private Socket clientSocket = null;
    private String serverText = null;
    private Database database = null;
    private DatabaseInfo databaseInfo;

    public Worker(Socket clientSocket,String serverText,DatabaseInfo dbInfo) {
        this.clientSocket = clientSocket;
        this.serverText = serverText;
        this.database = new Database();
        this.databaseInfo = dbInfo;
    }

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

    private String parseMessage(String clientMessageString){
        logger.trace("Parsing message");
        try{
            return parseXml(clientMessageString);
        }catch (DocumentException e ){
            logger.error("Cannot parse message from client",e);
            return serverErrorMessage("Cannot parse message from client");
        }
    }

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
     * <server_emulator>
     *     <result state="OK/ERROR">
     *         Result or error message
     *     </result>
     * </server_emulator>
     * @param result error message
     * @return XML message as string
     */
    private String buildMessage(Element result){
        Document doc = xmlFactory.createDocument();
        doc.addElement("server_emulator")
                .add(result);
        return doc.asXML();
    }

    /**
     * <server_emulator>
     *     <result state="ERROR">
     *         Error message
     *     </result>
     * </server_emulator>
     * @param error error message
     * @return XML message as string
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
