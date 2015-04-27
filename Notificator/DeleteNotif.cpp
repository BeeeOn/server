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

DeleteNotif::DeleteNotif(int userId, int notificationId,
            long time, int deleteNotifId)
:ControlNotification(NAME_DELETE_NOTIF ,userId, notificationId, time), 
mDeleteNotifId(deleteNotifId)
{
}

void DeleteNotif::addGcmData(JsonNotificationBuilder *builder) {
  builder->addData(JSON_DATA_MSGID_DELETE, Utils::intToString(mDeleteNotifId));
}

string DeleteNotif::getDbXml() {
  // it should not be called  
  return "";
}

DeleteNotif::~DeleteNotif() {
}

