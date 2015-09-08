/* 
 * File:   MsgInNotificationRead.cpp
 * Author: Pavel
 * 
 * Created on 23. dubna 2015, 13:08
 */

#include "NotificationReaded.h"
#include "IMsgInLoginRequired.h"
#include "../DAO/DAONotification.h"

using namespace std;

const std::string NotificationReaded::state = "notificationreaded";

NotificationReaded::NotificationReaded(pugi::xml_document* doc): IMsgInLoginRequired(doc) {
}


NotificationReaded::~NotificationReaded() {
}

int NotificationReaded::getMsgAuthorization() {
    return permissions::guest;
}

std::string NotificationReaded::createResponseMsgOut() {
    int notif_id = _doc->child(proto::communicationNode).child(proto::notificationNode).attribute(proto::locationTypeAttr).as_int(-1);
    
    if ( DAONotification::getInstance().setReaded(notif_id) > 0 )
    {
        return getXMLreply(proto::replyTrue);
    }
    else
    {
        _outputMainNode.append_attribute(proto::errorCodeAttr) = ServerException::NOTIFICATION_ID;
        return getXMLreply(proto::replyFalse);
    }
}
