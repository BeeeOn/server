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

const string GateUpdate::state = "updategate";

GateUpdate::GateUpdate(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc)  {
}


GateUpdate::~GateUpdate() {
}

int GateUpdate::getMsgAuthorization() {
    return permissions::admin;
}

string GateUpdate::createResponseMsgOut()
{                    
    pugi::xml_node gateNode =  _doc->child(proto::communicationNode).child(proto::adapterNode);
    
    
    string newTimeZone = gateNode.attribute(proto::gatewayUtcAttr).value();
    if(newTimeZone != ""){
        int tz;
        try{
            tz = stoi(newTimeZone);
        }catch(...){
            _outputMainNode.append_attribute(proto::errorCodeAttr) = ServerException::TIMEZONE_UTC;
            return getXMLreply(proto::replyFalse);
        }
        if(tz <= P_TIME_MIN || tz >= P_TIME_MAX)
        {
            _outputMainNode.append_attribute(proto::errorCodeAttr) = ServerException::TIMEZONE_UTC;
            return getXMLreply(proto::replyFalse);
        }
    }
    string newName = gateNode.attribute(proto::gatewayNameAttr).value();
    
    DAOAdapters::getInstance().updateAdapter(_gatewayId, newName, newTimeZone);
    
    return getXMLreply(proto::replyTrue);
}
