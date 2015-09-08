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

const std::string LocationAdd::state = "addlocation";

LocationAdd::LocationAdd(pugi::xml_document* doc): IMsgInLoginAndAdapterAccessRequired(doc){
}

LocationAdd::~LocationAdd() {
}

int LocationAdd::getMsgAuthorization() {
    return permissions::admin;
}

string LocationAdd::createResponseMsgOut()
{
    pugi::xml_node locationNode = _doc->child(proto::communicationNode).child(proto::locationNode);

    string roomType = locationNode.attribute(proto::locationTypeAttr).value();
    int rt;
    try{
        rt = stoi(roomType);
    }catch(...){
        return getNegativeXMLReply(ServerException::ROOM_TYPE);
    }

    if(rt < P_LOCATION_MIN_TYPE || rt > P_LOCATION_MAX_TYPE)
        return getNegativeXMLReply(ServerException::ROOM_TYPE);

    string roomName = locationNode.attribute(proto::locationNameAttr).value();

    int newRoomId = DAORooms::getInstance().insertNewRoom(_gatewayId, roomType, roomName);

    
    pugi::xml_node newLocationNode = _outputMainNode.append_child();
    newLocationNode.set_name(proto::locationNode);
    newLocationNode.append_attribute(proto::locationIdAttr) = newRoomId;
    return getXMLreply(proto::replyLocationCreated);
}
