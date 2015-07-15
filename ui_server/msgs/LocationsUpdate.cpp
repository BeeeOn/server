/* 
 * File:   MsgInUpdateRooms.cpp
 * Author: pavel
 * 
 * Created on 23. červen 2014, 11:03
 */

#include "LocationsUpdate.h"
#include "../lib/pugixml.hpp"
#include "../DAO/DAORooms.h"
#include "IMsgIn.h"
using namespace std;

const string LocationsUpdate::state = "setrooms";

LocationsUpdate::LocationsUpdate(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc) {
}

LocationsUpdate::~LocationsUpdate() {
}

int LocationsUpdate::getMsgAuthorization() {
    return ADMIN;
}

string LocationsUpdate::createResponseMsgOut()
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
            return getNegativeXMLReply(ServerException::MISSING_ENTITY);
        return getXMLreply(R_TRUE);
}

