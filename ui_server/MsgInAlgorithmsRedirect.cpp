/* 
 * File:   MsgInAlgorithmsRedirect.cpp
 * Author: pavel
 * 
 * Created on 16. březen 2015, 14:50
 */

#include "MsgInAlgorithmsRedirect.h"
#include "Config.h"
#include "SocketClient.h"
#include "save_custom_writer.h"

const std::string MsgInAlgorithmsRedirect::state = "algo";

MsgInAlgorithmsRedirect::MsgInAlgorithmsRedirect(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc)  {
}

MsgInAlgorithmsRedirect::~MsgInAlgorithmsRedirect() {
}

string MsgInAlgorithmsRedirect::createResponseMsgOut() {


    string r;
        try{
        SocketClient sc(Config::getInstance().getAlgorithmPort());    
                        
        Logger::getInstance(Logger::DEBUG3)<<"Algo communication"<<endl; 
       pugi::xml_node node = _doc->child(P_COMMUNICATION);
        node.append_attribute("userid")=_userId;
        string algoStr = node_to_string(node);
        Logger::debug3() << "toAlgo: " << algoStr << endl;
        sc.write(algoStr.c_str());
        r = sc.readUntilendTag("</com>");
    }catch(...){
        throw ServerException(ServerException::UI2ALGO);
    }
    Logger::getInstance(Logger::DEBUG3)<<"S2S communication: "<< r<<"|"<<endl; 
        
    return r;
}

int MsgInAlgorithmsRedirect::getMsgAuthorization() {
    return GUEST;
}

