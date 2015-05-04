/* 
 * File:   DeleteNotif.cpp
 * Author: Martin
 * 
 */

#include "DeleteNotif.h"
#include "JsonNotificationBuilder.h"
#include "Constants.h"    
#include <iostream>
#include "Utils.h"
#include "XmlHelper.h"

DeleteNotif::DeleteNotif(int userId, int notificationId,
            long time, int deleteNotifId)
:ControlNotification(NAME_DELETE_NOTIF ,userId, notificationId, time), 
mDeleteNotifId(deleteNotifId)
{
}

void DeleteNotif::addGcmData(JsonNotificationBuilder *builder) {
  builder->addData(JSON_DATA_MSGID_DELETE, Utils::intToString(mDeleteNotifId));
}

void DeleteNotif::addDbXmlData(stringstream *ss) {
    XmlHelper::tagWithValue(ss, DATA_MSGID_DELETE, Utils::intToString(mDeleteNotifId));
}


DeleteNotif::~DeleteNotif() {
}

