/* 
 * File:   MsgInNotificationRead.cpp
 * Author: Pavel
 * 
 * Created on 23. dubna 2015, 13:08
 */

#include "NotificationsGet.h"
#include "IMsgInLoginRequired.h"
#include "../DAO/DAONotification.h"

const std::string NotificationsGet::state = "getnotifications";

NotificationsGet::NotificationsGet(pugi::xml_document* doc): IMsgInLoginRequired(doc) {
}


NotificationsGet::~NotificationsGet() {
}

int NotificationsGet::getMsgAuthorization() {
    return permissions::guest;
}

std::string NotificationsGet::createResponseMsgOut() {
    return envelopeResponse(proto::replyNotifications, DAONotification::getInstance().getXMLNotifications(_userId));
}
