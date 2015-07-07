/* 
 * File:   MsgInAddRoom.cpp
 * Author: pavel
 * 
 * Created on 24. červen 2014, 13:57
 */

#include "MsgInAddRoom.h"
#include "../DAO/DAORooms.h"
#include "ServerException.h"
#include "IMsgInLoginRequired.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"

using namespace std;

const std::string MsgInAddRoom::state = "addroom";

MsgInAddRoom::MsgInAddRoom(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc){
}

MsgInAddRoom::~MsgInAddRoom() {
}

int MsgInAddRoom::getMsgAuthorization() {
    return ADMIN;
}

string MsgInAddRoom::createResponseMsgOut()
{
        pugi::xml_node roomNode = _doc->child(P_COMMUNICATION);
       
        string roomType = roomNode.attribute(P_ROOM_TYPE).value();
        int rt;
        try{
            rt = stoi(roomType);
        }catch(...){
            throw ServerException(ServerException::ROOM_TYPE);
        }
        
        if(rt < P_ROOM_MIN_TYPE || rt > P_ROOM_MAX_TYPE)
            throw ServerException(ServerException::ROOM_TYPE);
        
        string roomName = roomNode.attribute(P_ROOM_NAME).value();
        
        int newRoomId = DAORooms::getInstance().insertNewRoom(_adapterId, roomType, roomName);
        
        stringstream ss;
        ss << newRoomId;
        string newRoomIdStr = ss.str();
        
    string attr = makeXMLattribute("lid", newRoomIdStr);
    return envelopeResponseWithAttributes(R_ROOM_CREATED,attr);
}
