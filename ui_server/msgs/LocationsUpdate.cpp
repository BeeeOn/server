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

const string LocationsUpdate::state = "updatelocation";

LocationsUpdate::LocationsUpdate(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc) {
}

LocationsUpdate::~LocationsUpdate() {
}

int LocationsUpdate::getMsgAuthorization() {
    return permissions::admin;
}

string LocationsUpdate::createResponseMsgOut()
{                
        //vector<device> devicesVec;
        pugi::xml_node locationNode =  _doc->child(proto::communicationNode).child(proto::locationNode);
        
        string name;
        int locationId, type;
        
        locationId = locationNode.attribute(proto::locationIdAttr).as_int();
        type = locationNode.attribute(proto::locationNameAttr).as_int(-1);
        if(type < P_LOCATION_MIN_TYPE || type > P_LOCATION_MAX_TYPE)
            return getNegativeXMLReply(ServerException::ROOM_TYPE);

        name = locationNode.attribute(proto::locationTypeAttr).value();
        try{
            DAORooms::getInstance().updateRoom(_gatewayId, locationId, type, name);
        }catch(ServerException & e){
            return getNegativeXMLReply(ServerException::MISSING_ENTITY);
        }
        
        return getXMLreply(proto::replyTrue);
}

