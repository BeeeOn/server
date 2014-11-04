/* 
 * File:   MsgInGetXml.cpp
 * Author: pavel
 * 
 * Created on 3. červenec 2014, 13:03
 */

#include "MsgInGetAllDevices.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"

using namespace std;

const std::string MsgInGetAllDevices::state = "getalldevs";

MsgInGetAllDevices::MsgInGetAllDevices(char* msg, pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(msg, doc) {
}

MsgInGetAllDevices::~MsgInGetAllDevices() {
}

int MsgInGetAllDevices::getMsgAuthorization() {
    return GUEST;
}

string MsgInGetAllDevices::createResponseMsgOut()
{          
        return envelopeResponseWithAdapterId(R_DEVICES_ALL, DBConnector::getInstance().getXMLAllDevs(_adapterId), _adapterId);
}
