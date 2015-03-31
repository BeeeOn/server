/* 
 * File:   MsgInGetNewDevices.cpp
 * Author: pavel
 * 
 * Created on 6. říjen 2014, 13:27
 */

#include "MsgInGetNewDevices.h"
#include "../DAO/DAODevices.h"
const string MsgInGetNewDevices::state = "getnewdevs";

MsgInGetNewDevices::MsgInGetNewDevices(char* msg, pugi::xml_document* doc) : IMsgInLoginAndAdapterAccessRequired(msg, doc){
}


MsgInGetNewDevices::~MsgInGetNewDevices() {
}

int MsgInGetNewDevices::getMsgAuthorization() {
    return ADMIN;
}

string MsgInGetNewDevices::createResponseMsgOut() {
    
     return envelopeResponseWithAdapterId(R_DEVICES, DAODevices::getInstance().getXMLNewDevices(_adapterId) , _adapterId);
}

