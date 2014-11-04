/* 
 * File:   MsgInDelRoom.cpp
 * Author: pavel
 * 
 * Created on 24. červen 2014, 14:48
 */

#include "MsgInDelRoom.h"


const std::string MsgInDelRoom::state = "delroom";

MsgInDelRoom::MsgInDelRoom(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc){
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
    DBConnector::getInstance().deleteRoom(_adapterId, roomId);
    
    return envelopeResponse(R_TRUE);
}

