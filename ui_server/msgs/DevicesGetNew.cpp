/* 
 * File:   MsgInGetNewDevices.cpp
 * Author: pavel
 * 
 * Created on 6. říjen 2014, 13:27
 */

#include "DevicesGetNew.h"
#include "../DAO/DAODevices.h"

using namespace std;

const string DevicesGetNew::state = "getnewdevs";

DevicesGetNew::DevicesGetNew(pugi::xml_document* doc) : IMsgInLoginAndAdapterAccessRequired(doc){
}


DevicesGetNew::~DevicesGetNew() {
}

int DevicesGetNew::getMsgAuthorization() {
    return ADMIN;
}

string DevicesGetNew::createResponseMsgOut() {
    
     return envelopeResponseWithAdapterId(R_DEVICES, DAODevices::getInstance().getXMLNewDevices(_gatewayId) , _adapterId);
}

