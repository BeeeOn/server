/* 
 * File:   MsgInUnknown.cpp
 * Author: pavel
 * 
 * Created on 18. duben 2014, 15:55
 */

#include "MsgInUnknown.h"
#include "ServerException.h"

const std::string MsgInUnknown::state = "";

MsgInUnknown::MsgInUnknown(char* msg, pugi::xml_document* doc): IMsgInLoginUnwanted(msg, doc){
}


MsgInUnknown::~MsgInUnknown() {
}

int MsgInUnknown::getMsgAuthorization() {
    return EVERYONE;
}

string MsgInUnknown::createResponseMsgOut()
{
        return envelopeResponse(R_FALSE, "not supported by server", ServerException::MSG_UNKNOWN);
}