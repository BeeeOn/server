/* 
 * File:   MsgInSwitch.cpp
 * Author: pavel
 * 
 * Created on 22. červenec 2014, 10:15
 */

#include "ModuleSwitchState.h"
#include "Config.h"

using namespace std;

const string ModuleSwitchState::state = "switchstate";
ModuleSwitchState::ModuleSwitchState(pugi::xml_document* doc) : IMsgInLoginAndAdapterAccessRequired(doc) {
}

ModuleSwitchState::~ModuleSwitchState() {
}

int ModuleSwitchState::getMsgAuthorization() {
    return permissions::user;
}

string ModuleSwitchState::createResponseMsgOut()
{    
    pugi::xml_node comNode =  _doc->child(proto::communicationNode);
    string deviceId = comNode.attribute("deviceid").value();
    string deviceType = comNode.attribute("moduleid").value();
    string newValue = comNode.attribute(proto::moduleValueIdAttr).value();
    
    string r ;
    try{
        SocketClient sc(Config::getInstance().getActivityPort());    
        string request = "<request type=\"switch\">"
                            "<sensor id=\""+deviceId+"\" type=\""+deviceType+"\" onAdapter=\""+_adapterId+"\">"
                                    "<value>"+newValue+"</value>"
                            "</sensor>"
                        "</request>";
                        
        Logger::getInstance(Logger::DEBUG3)<<"S2S communication: "<< request<<endl; 
        sc.write(request);
        r = sc.readUntilendTag("</reply>");
    }catch(...){
        throw ServerException(ServerException::SERVER2SERVER);
    }
    Logger::getInstance(Logger::DEBUG3)<<"S2S communication: "<< r<<endl; 
    
    if(r == "<reply>true</reply>")
    return envelopeResponse(proto::replyTrue);
    else
         throw ServerException(ServerException::SWITCH_FAIL);
        
}