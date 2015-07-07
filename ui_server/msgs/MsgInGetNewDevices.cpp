/* 
 * File:   MsgInGetNewDevices.cpp
 * Author: pavel
 * 
 * Created on 6. říjen 2014, 13:27
 */

#include "MsgInGetNewDevices.h"
#include "../DAO/DAODevices.h"

using namespace std;

const string MsgInGetNewDevices::state = "getnewdevs";

MsgInGetNewDevices::MsgInGetNewDevices(pugi::xml_document* doc) : IMsgInLoginAndAdapterAccessRequired(doc){
}


MsgInGetNewDevices::~MsgInGetNewDevices() {
}

int MsgInGetNewDevices::getMsgAuthorization() {
    return ADMIN;
}

string MsgInGetNewDevices::createResponseMsgOut() {
    
     return envelopeResponseWithAdapterId(R_DEVICES, DAODevices::getInstance().getXMLNewDevices(_adapterId) , _adapterId);
}

