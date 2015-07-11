/* 
 * File:   MsgInNotificationRead.cpp
 * Author: Pavel
 * 
 * Created on 23. dubna 2015, 13:08
 */

#include "MsgInNotificationRead.h"
#include "IMsgInLoginRequired.h"
#include "../DAO/DAONotification.h"

using namespace std;

const std::string MsgInNotificationRead::state = "notifread";

MsgInNotificationRead::MsgInNotificationRead(pugi::xml_document* doc): IMsgInLoginRequired(doc) {
}


MsgInNotificationRead::~MsgInNotificationRead() {
}

int MsgInNotificationRead::getMsgAuthorization() {
    return GUEST;
}

std::string MsgInNotificationRead::createResponseMsgOut() {
    int notif_id = _doc->child(P_COMMUNICATION).child(P_NOTIFICATION).attribute(P_NOTIFICATION_ID).as_int(-1);
    
    if ( DAONotification::getInstance().setReaded(notif_id) > 0 )
    {
        return getXMLreply(R_TRUE);
    }
    else
    {
        _outputMainNode.append_attribute(P_ERRCODE) = ServerException::NOTIFICATION_ID;
        return getXMLreply(R_FALSE);
    }
}
