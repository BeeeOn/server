/* 
 * File:   MsgInGetRooms.cpp
 * Author: pavel
 * 
 * Created on 23. červen 2014, 11:23
 */

#include "LocationsGet.h"
#include "../DAO/DAORooms.h"

using namespace std;

const string LocationsGet::state = "getrooms";

LocationsGet::LocationsGet(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc) {
}


LocationsGet::~LocationsGet() {
}

int LocationsGet::getMsgAuthorization() {
    return GUEST;
}

string LocationsGet::createResponseMsgOut()
{                        
    return envelopeResponseWithAdapterId(R_ROOMS, DAORooms::getInstance().getXMLrooms(_gatewayId));
}