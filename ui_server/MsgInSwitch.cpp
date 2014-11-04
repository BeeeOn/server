/* 
 * File:   MsgInSwitch.cpp
 * Author: pavel
 * 
 * Created on 22. červenec 2014, 10:15
 */

#include "MsgInSwitch.h"
#include "Config.h"

const string MsgInSwitch::state = "switch";
MsgInSwitch::MsgInSwitch(char* msg, pugi::xml_document* doc) : IMsgInLoginAndAdapterAccessRequired(msg, doc) {
}

MsgInSwitch::~MsgInSwitch() {
}

int MsgInSwitch::getMsgAuthorization() {
    return USER;
}

string MsgInSwitch::createResponseMsgOut()
{    
    pugi::xml_node comNode =  _doc->child(P_COMMUNICATION);
    string deviceId = comNode.attribute(P_DEVICE_ID).value();
    string deviceType = comNode.attribute(P_DEVICE_TYPE).value();
    string newValue = comNode.child(P_DEVICE_VALUE).child_value();
    
    string r ;
    try{
        SocketClient sc(Config::getInstance().getActivityPort());    

        sc.write("<request type=\"switch\">"
                            "<sensor id=\""+deviceId+"\" type=\""+deviceType+"\" onAdapter=\""+_adapterId+"\">"
                                    "<value>"+newValue+"</value>"
                            "</sensor>"
                        "</request>");
        r = sc.read();
    }catch(...){
        throw ServerException(ServerException::SERVER2SERVER);
    }
    Logger::getInstance(Logger::DEBUG3)<<"S2S communication: "<< r<<endl; 
    
    if(r == "<reply>true</reply>")
    return envelopeResponse(R_TRUE);
    else
         throw ServerException(ServerException::SWITCH_FAIL);
        
}