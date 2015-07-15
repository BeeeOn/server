/* 
 * File:   MsgInAddRoom.cpp
 * Author: pavel
 * 
 * Created on 24. červen 2014, 13:57
 */

#include "LocationAdd.h"
#include "../DAO/DAORooms.h"
#include "ServerException.h"
#include "IMsgInLoginRequired.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"

using namespace std;

const std::string LocationAdd::state = "addroom";

LocationAdd::LocationAdd(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc){
}

LocationAdd::~LocationAdd() {
}

int LocationAdd::getMsgAuthorization() {
    return ADMIN;
}

string LocationAdd::createResponseMsgOut()
{
    pugi::xml_node roomNode = _doc->child(P_COMMUNICATION);

    string roomType = roomNode.attribute(P_ROOM_TYPE).value();
    int rt;
    try{
        rt = stoi(roomType);
    }catch(...){
        return getNegativeXMLReply(ServerException::ROOM_TYPE);
    }

    if(rt < P_ROOM_MIN_TYPE || rt > P_ROOM_MAX_TYPE)
        return getNegativeXMLReply(ServerException::ROOM_TYPE);

    string roomName = roomNode.attribute(P_ROOM_NAME).value();

    int newRoomId = DAORooms::getInstance().insertNewRoom(_adapterId, roomType, roomName);

    _outputMainNode.append_attribute(P_ROOM_ID) = newRoomId;
    return getXMLreply(R_ROOM_CREATED);
}
