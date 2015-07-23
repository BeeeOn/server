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

const std::string DevicesGetAll::state = "getalldevs";

DevicesGetAll::DevicesGetAll(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc) {
}

DevicesGetAll::~DevicesGetAll() {
}

int DevicesGetAll::getMsgAuthorization() {
    return GUEST;
}

string DevicesGetAll::createResponseMsgOut()
{          
        return envelopeResponseWithAdapterId(R_DEVICES_ALL, DAODevices::getInstance().getXMLAllDevs(_adapterId), _adapterId);
}
