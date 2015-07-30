/* 
 * File:   MsgInSetAdapter.cpp
 * Author: Pavel
 * 
 * Created on 9. července 2015, 13:21
 */

#include "GateUpdate.h"
#include "IMsgIn.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"
using namespace std;

const string GateUpdate::state = "setgate";

GateUpdate::GateUpdate(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc)  {
}


GateUpdate::~GateUpdate() {
}

int GateUpdate::getMsgAuthorization() {
    return ADMIN;
}

string GateUpdate::createResponseMsgOut()
{                     
    string newTimeZone = _doc->child(P_COMMUNICATION).attribute(P_TIME_UTC).value();
    if(newTimeZone != ""){
        int tz;
        try{
            tz = stoi(newTimeZone);
        }catch(...){
            _outputMainNode.append_attribute(P_ERRCODE) = ServerException::TIMEZONE_UTC;
            return getXMLreply(R_FALSE);
        }
        if(tz <= P_TIME_MIN || tz >= P_TIME_MAX)
        {
            _outputMainNode.append_attribute(P_ERRCODE) = ServerException::TIMEZONE_UTC;
            return getXMLreply(R_FALSE);
        }
    }
    string newName = _doc->child(P_COMMUNICATION).attribute(P_ADAPTER_NAME).value();
    
    DAOAdapters::getInstance().updateAdapter(_gatewayId, newName, newTimeZone);
    
    return getXMLreply(R_TRUE);
}
