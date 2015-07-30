/* 
 * File:   MsgInDelRoom.cpp
 * Author: pavel
 * 
 * Created on 24. červen 2014, 14:48
 */

#include "LocationDelete.h"
#include "../DAO/DAORooms.h"


const std::string LocationDelete::state = "delroom";

LocationDelete::LocationDelete(pugi::xml_document* doc): IMsgInLoginRequired(doc){
}


LocationDelete::~LocationDelete() {
}

int LocationDelete::getMsgAuthorization() {
    return ADMIN;
}

string LocationDelete::createResponseMsgOut()
{
    pugi::xml_node roomNode = _doc->child(P_COMMUNICATION);
    
    string roomId = roomNode.attribute(P_ROOM_ID).value();
    
    //if(roomId == "0")
     //   return envelopeResponse(R_TRUE);
    DAORooms::getInstance().deleteRoom(_gatewayId, roomId);
    
    return getXMLreply(R_TRUE);
}

