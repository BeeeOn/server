/* 
 * File:   MsgInPartial.cpp
 * Author: pavel
 * 
 * Created on 3. červen 2014, 8:03
 */

#include "DevicesUpdate.h"
#include "ServerException.h"
#include "../DAO/DAODevices.h"
using namespace std;
const string DevicesUpdate::state = "updatedevice";

DevicesUpdate::DevicesUpdate(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc){
}


DevicesUpdate::~DevicesUpdate() {
}

int DevicesUpdate::getMsgAuthorization() {
    return permissions::admin;
}


string DevicesUpdate::createResponseMsgOut()
{    
    
    pugi::xml_node deviceNode =  _doc->child(proto::communicationNode).child(proto::deviceNode);
    
    string id = deviceNode.attribute(proto::deviceIdAttr).value();
    string init = deviceNode.attribute(proto::deviceInitAttr).value();
    string locationId = deviceNode.attribute(proto::deviceLocationIdAttr).value();
    string refresh = deviceNode.attribute(P_DEVICE_REFRESH_DEPRECATED).value();
    string name = deviceNode.attribute(proto::deviceNameAttr).value();

    if(init != "" || locationId != "" || refresh != "" || name != "")
        DAODevices::getInstance().updateFacility(_gatewayId, id, init, locationId, refresh, name);
    
    return envelopeResponse(proto::replyTrue);
}