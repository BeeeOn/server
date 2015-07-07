/* 
 * File:   MsgInDelRoom.cpp
 * Author: pavel
 * 
 * Created on 24. červen 2014, 14:48
 */

#include "MsgInDelRoom.h"
#include "../DAO/DAORooms.h"


const std::string MsgInDelRoom::state = "delroom";

MsgInDelRoom::MsgInDelRoom(pugi::xml_document* doc): IMsgInLoginRequired(doc){
}


MsgInDelRoom::~MsgInDelRoom() {
}

int MsgInDelRoom::getMsgAuthorization() {
    return ADMIN;
}

string MsgInDelRoom::createResponseMsgOut()
{
    pugi::xml_node roomNode = _doc->child(P_COMMUNICATION);
    
    string roomId = roomNode.attribute(P_ROOM_ID).value();
    
    //if(roomId == "0")
     //   return envelopeResponse(R_TRUE);
    DAORooms::getInstance().deleteRoom(_adapterId, roomId);
    
    return envelopeResponse(R_TRUE);
}

