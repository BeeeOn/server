/* 
 * File:   MsgInDelDevice.cpp
 * Author: pavel
 * 
 * Created on 5. srpen 2014, 10:27
 */

#include "DeviceDelete.h"
#include "Config.h"
using namespace std;
const string DeviceDelete::state = "deldev";
DeviceDelete::DeviceDelete(pugi::xml_document* doc) : IMsgInLoginAndAdapterAccessRequired(doc)  {
}

DeviceDelete::~DeviceDelete(){
    
}

int DeviceDelete::getMsgAuthorization() {
    return SUPERUSER;
}

string DeviceDelete::createResponseMsgOut()
{
    
    pugi::xml_node deviceNode =  _doc->child(P_COMMUNICATION);
    string deviceId = deviceNode.attribute(P_DEVICE_ID).value();
    
    string r ;
    try{
        SocketClient sc(Config::getInstance().getActivityPort());    

        sc.write("<request type=\"delete\">"
                            "<sensor id=\""+deviceId+"\" onAdapter=\""+_adapterId+"\" />"
                        "</request>");
        r = sc.readUntilendTag("</reply>");
    }catch(...){
        throw ServerException(ServerException::SERVER2SERVER);
    }
    Logger::getInstance(Logger::DEBUG3)<<"S2S communication: "<< r<<endl; 
    
    if(r == "<reply>true</reply>")
        return envelopeResponse(R_TRUE);
    else
         throw ServerException(ServerException::SWITCH_FAIL);
        
}