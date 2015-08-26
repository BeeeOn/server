/* 
 * File:   MsgInGetNewDevices.cpp
 * Author: pavel
 * 
 * Created on 6. říjen 2014, 13:27
 */

#include "DevicesGetNew.h"
#include "../DAO/DAODevices.h"

using namespace std;

const string DevicesGetNew::state = "getnewdevices";

DevicesGetNew::DevicesGetNew(pugi::xml_document* doc) : IMsgInLoginAndAdapterAccessRequired(doc){
}


DevicesGetNew::~DevicesGetNew() {
}

int DevicesGetNew::getMsgAuthorization() {
    return permissions::admin;
}

string DevicesGetNew::createResponseMsgOut() {
    
     return envelopeResponseWithAdapterId(proto::replyDevices, DAODevices::getInstance().getXMLNewDevices(_gatewayId) , _adapterId);
}

