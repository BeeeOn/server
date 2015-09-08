/* 
 * File:   MsgInAdapterListen.cpp
 * Author: pavel
 * 
 * Created on 22. červenec 2014, 10:14
 */


#include "GateScanMode.h"
#include "Config.h"

using namespace std;
const std::string GateScanMode::state = "scanmode";

GateScanMode::GateScanMode(pugi::xml_document* doc) : IMsgInLoginAndAdapterAccessRequired(doc) {
}

GateScanMode::~GateScanMode() {
}

int GateScanMode::getMsgAuthorization() {
    return permissions::superuser;
}

string GateScanMode::createResponseMsgOut()
{       
    string r ;
    try{
        SocketClient sc(Config::getInstance().getActivityPort());    

        sc.write("<request type=\"listen\">"
                            "<adapter id=\""+_adapterId+"\"/>"
                        "</request>");
        r= sc.read();
    }catch(...){
        _outputMainNode.append_attribute(proto::errorCodeAttr) = ServerException::SERVER2SERVER;
        return getXMLreply(proto::replyFalse);
    }
    
    Logger::getInstance(Logger::DEBUG3)<<"S2S communication: "<< r<<endl; 
    
    if(r == "<reply>true</reply>")
    {
        return envelopeResponse(proto::replyTrue);
    }
    else
    {
        _outputMainNode.append_attribute(proto::errorCodeAttr) = ServerException::ADAPTER_LISTEN_FAIL;
        return getXMLreply(proto::replyFalse);
    }
}
