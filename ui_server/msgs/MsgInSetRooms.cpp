/* 
 * File:   MsgInUpdateRooms.cpp
 * Author: pavel
 * 
 * Created on 23. červen 2014, 11:03
 */

#include "MsgInSetRooms.h"
#include "../lib/pugixml.hpp"
#include "../DAO/DAORooms.h"
using namespace std;

const string MsgInSetRooms::state = "setrooms";

MsgInSetRooms::MsgInSetRooms(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc) {
}

MsgInSetRooms::~MsgInSetRooms() {
}

int MsgInSetRooms::getMsgAuthorization() {
    return ADMIN;
}

string MsgInSetRooms::createResponseMsgOut()
{                
        //vector<device> devicesVec;
        pugi::xml_node locationNode =  _doc->child(P_COMMUNICATION).child(P_ROOM);
        
        string id,type,name;
        bool failed = false;
        
        for (; locationNode; locationNode = locationNode.next_sibling(P_ROOM))
        {
            id = locationNode.attribute(P_IN_ROOM_ID).value();
            type = locationNode.attribute(P_IN_ROOM_TYPE).value();
            //TODO check type
            name = locationNode.attribute(P_IN_ROOM_NAME).value();
            try{
                DAORooms::getInstance().updateRoom(_adapterId, id, type, name);
            }catch(ServerException & e){
                failed = true;
            }
        }
        if(failed)
            throw ServerException(ServerException::MISSING_ENTITY);
        return envelopeResponse(R_TRUE);
}

