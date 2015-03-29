/* 
 * File:   MsgInGetRooms.cpp
 * Author: pavel
 * 
 * Created on 23. červen 2014, 11:23
 */

#include "MsgInGetRooms.h"
#include "pugixml.hpp"
#include "../DAO/DAORooms.h"

using namespace std;

const string MsgInGetRooms::state = "getrooms";

MsgInGetRooms::MsgInGetRooms(char* msg, pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(msg, doc) {
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