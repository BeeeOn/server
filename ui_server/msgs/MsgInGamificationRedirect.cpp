/* 
 * File:   MsgInGamificationRedirect.cpp
 * Author: Pavel
 * 
 * Created on 22. dubna 2015, 10:12
 */

#include "MsgInGamificationRedirect.h"
#include "Config.h"
#include "../SocketClient.h"
#include "save_custom_writer.h"
using namespace std;
MsgInGamificationRedirect::MsgInGamificationRedirect(pugi::xml_document* doc): IMsgInLoginRequired(doc){
}

MsgInGamificationRedirect::~MsgInGamificationRedirect() {
}

string MsgInGamificationRedirect::createResponseMsgOut() {


    string r;
    try{
        SocketClient sc(Config::getInstance().getGamificationPort());    
                        
        Logger::getInstance(Logger::DEBUG3)<<"Gami communication"<<endl; 
        pugi::xml_node node = _doc->child(proto::communicationNode);
        node.append_attribute("userid")=_userId;
        string algoStr = node_to_string(node);
        Logger::debug3() << "toGami: " << algoStr << endl;
        sc.write(algoStr.c_str());
        r = sc.readUntilendTag("</com>");
    }catch(...){
        throw ServerException(ServerException::UI2ALGO);
    }
    Logger::getInstance(Logger::DEBUG3)<<"S2S communication: "<< r<<"|"<<endl; 
        
    return r;
}

int MsgInGamificationRedirect::getMsgAuthorization() {
    return permissions::guest;
}