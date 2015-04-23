/* 
 * File:   MsgInNotificationRead.cpp
 * Author: Pavel
 * 
 * Created on 23. dubna 2015, 13:08
 */

#include "MsgInGetNotifications.h"
#include "IMsgInLoginRequired.h"
#include "../DAO/DAONotification.h"

const std::string MsgInGetNotifications::state = "getnotifs";

MsgInGetNotifications::MsgInGetNotifications(char* msg, pugi::xml_document* doc): IMsgInLoginRequired(msg, doc) {
}


MsgInGetNotifications::~MsgInGetNotifications() {
}

int MsgInGetNotifications::getMsgAuthorization() {
    return GUEST;
}

std::string MsgInGetNotifications::createResponseMsgOut() {
    return envelopeResponseWithAdapterId(R_NOTIFICATIONS, DAONotification::getInstance().getXMLNotifications(_userId));
}
