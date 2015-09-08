/* 
 * File:   MsgInGetRooms.cpp
 * Author: pavel
 * 
 * Created on 23. červen 2014, 11:23
 */

#include "LocationsGet.h"
#include "../DAO/DAORooms.h"

using namespace std;

const string LocationsGet::state = "getlocations";

LocationsGet::LocationsGet(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc) {
}


LocationsGet::~LocationsGet() {
}

int LocationsGet::getMsgAuthorization() {
    return permissions::guest;
}

string LocationsGet::createResponseMsgOut()
{                        
    return envelopeResponseWithAdapterId(proto::replyLocations, DAORooms::getInstance().getXMLrooms(_gatewayId));
}