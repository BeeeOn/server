/* 
 * File:   MsgInNotificationRead.cpp
 * Author: Pavel
 * 
 * Created on 23. dubna 2015, 13:08
 */

#include "MsgInNotificationRead.h"
#include "IMsgInLoginRequired.h"

const std::string MsgInNotificationRead::state = "notifread";

MsgInNotificationRead::MsgInNotificationRead(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc) {
}


MsgInNotificationRead::~MsgInNotificationRead() {
}

int MsgInNotificationRead::getMsgAuthorization() {
    return GUEST;
}

std::string MsgInNotificationRead::createResponseMsgOut() {
    
    return envelopeResponse(R_TRUE);
}
