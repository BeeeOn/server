/* 
 * File:   MsgInDelDevice.cpp
 * Author: pavel
 * 
 * Created on 5. srpen 2014, 10:27
 */

#include "DeviceDelete.h"
#include "Config.h"
using namespace std;
const string DeviceDelete::state = "deletedevice";
DeviceDelete::DeviceDelete(pugi::xml_document* doc) : IMsgInLoginAndAdapterAccessRequired(doc)  {
}

DeviceDelete::~DeviceDelete(){
    
}

int DeviceDelete::getMsgAuthorization() {
    return permissions::superuser;
}

string DeviceDelete::createResponseMsgOut()
{
    
    pugi::xml_node deviceNode =  _doc->child(proto::communicationNode).child(proto::deviceNode);
    string deviceMac = deviceNode.attribute(proto::deviceIdAttr).value();
    
    string r ;
    try{
        SocketClient sc(Config::getInstance().getActivityPort());    

        sc.write("<request type=\"delete\">"
                            "<sensor id=\""+deviceMac+"\" onAdapter=\""+_adapterId+"\" />"
                        "</request>");
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