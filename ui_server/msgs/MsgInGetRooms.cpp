/* 
 * File:   MsgInGetRooms.cpp
 * Author: pavel
 * 
 * Created on 23. červen 2014, 11:23
 */

#include "MsgInGetRooms.h"
#include "../lib/pugixml.hpp"
#include "../DAO/DAORooms.h"

using namespace std;

const string MsgInGetRooms::state = "getrooms";

MsgInGetRooms::MsgInGetRooms(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc) {
}


MsgInGetRooms::~MsgInGetRooms() {
}

int MsgInGetRooms::getMsgAuthorization() {
    return GUEST;
}

string MsgInGetRooms::createResponseMsgOut()
{                        
    return envelopeResponseWithAdapterId(R_ROOMS, DAORooms::getInstance().getXMLrooms(_adapterId));
}