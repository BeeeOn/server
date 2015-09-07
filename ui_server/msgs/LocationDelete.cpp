/* 
 * File:   MsgInDelRoom.cpp
 * Author: pavel
 * 
 * Created on 24. červen 2014, 14:48
 */

#include "LocationDelete.h"
#include "../DAO/DAORooms.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"


const std::string LocationDelete::state = "deletelocation";

LocationDelete::LocationDelete(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc){
}


LocationDelete::~LocationDelete() {
}

int LocationDelete::getMsgAuthorization() {
    return permissions::admin;
}

string LocationDelete::createResponseMsgOut()
{
    pugi::xml_node locationNode = _doc->child(proto::communicationNode).child(proto::locationNode);
    
    string locationId = locationNode.attribute(proto::locationIdAttr).value();
    
    //if(roomId == "0")
     //   return envelopeResponse(proto::replyTrue);
    DAORooms::getInstance().deleteRoom(_gatewayId, locationId);
    
    return getXMLreply(proto::replyTrue);
}

