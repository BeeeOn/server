/* 
 * File:   MsgInUnknown.cpp
 * Author: pavel
 * 
 * Created on 18. duben 2014, 15:55
 */

#include "MsgInUnknown.h"
#include "ServerException.h"

using namespace std;

const std::string MsgInUnknown::state = "";

MsgInUnknown::MsgInUnknown(pugi::xml_document* doc): IMsgInFreeAccess(doc){
}


MsgInUnknown::~MsgInUnknown() {
}

int MsgInUnknown::getMsgAuthorization() {
    return permissions::everyone;
}

string MsgInUnknown::createResponseMsgOut()
{
        return envelopeResponse(proto::replyFalse, "not supported by server", ServerException::MSG_UNKNOWN);
}