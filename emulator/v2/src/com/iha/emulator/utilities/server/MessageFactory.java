package com.iha.emulator.utilities.server;

import org.dom4j.*;

/**
 * Created by Shu on 6.12.2014.
 */
public class MessageFactory {

    private static MessageFactory instance;
    private static DocumentFactory factory = DocumentFactory.getInstance();

    public static synchronized MessageFactory getInstance(){
        if(instance == null) instance = new MessageFactory();
        return instance;
    }

    public static Document createDocument(){
        return factory.createDocument();
    }

    /**
     * <emulator_server db="database name" task="getAdapters">
     *     <attributes>id,name</attributes>
     * </emulator_server>
     */
    public static String buildGetAdaptersMessage(String databaseName,String attributes){
        Document doc = createDocument();
        Element root = doc.addElement("emulator_server")
                .addAttribute("db",databaseName)
                .addAttribute("task","getAdapters");
        root.addElement("attributes").setText(attributes);
        return doc.asXML();
    }

    public static void parseServerResponse(String response) throws DocumentException {
        Document document = DocumentHelper.parseText(response);

    }
}
