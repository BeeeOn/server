/* 
 * File:   MsgInUpdateRooms.cpp
 * Author: pavel
 * 
 * Created on 23. červen 2014, 11:03
 */

#include "MsgInUpdateRooms.h"
#include "pugixml.hpp"
#include "DBConnector.h"
using namespace std;

const string MsgInUpdateRooms::state = "setrooms";

MsgInUpdateRooms::MsgInUpdateRooms(char* msg, pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(msg, doc) {
}

MsgInUpdateRooms::~MsgInUpdateRooms() {
}

int MsgInUpdateRooms::getMsgAuthorization() {
    return ADMIN;
}

string MsgInUpdateRooms::createResponseMsgOut()
{                
        vector<device> devicesVec;
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
                DBConnector::getInstance().updateRoom(_adapterId, id, type, name);
            }catch(ServerException & e){
                failed = true;
            }
        }
        if(failed)
            throw ServerException(ServerException::MISSING_ENTITY);
        return envelopeResponse(R_TRUE);
}