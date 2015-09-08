/* 
 * File:   MsgInGetXml.cpp
 * Author: pavel
 * 
 * Created on 3. červenec 2014, 13:03
 */

#include "DevicesGetAll.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"

#include "../DAO/DAODevices.h"

using namespace std;

const std::string DevicesGetAll::state = "getalldevices";

DevicesGetAll::DevicesGetAll(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc) {
}

DevicesGetAll::~DevicesGetAll() {
}

int DevicesGetAll::getMsgAuthorization() {
    return permissions::guest;
}

string DevicesGetAll::createResponseMsgOut()
{          
        //return envelopeResponseWithAdapterId(proto::replyAllDevices, DAODevices::getInstance().getXMLAllDevs(_gatewayId), _adapterId);
    return envelopeResponseWithAdapterId(proto::replyAllDevices, DAODevices::getInstance().getXMLAllDevs(_gatewayId), "");
    
}
