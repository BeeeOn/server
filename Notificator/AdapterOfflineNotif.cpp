/* 
 * File:   AdapterOfflineNotif.cpp
 * Author: Martin
 * 
 */

#include "AdapterOfflineNotif.h"
#include "JsonNotificationBuilder.h"
#include "Constants.h"    
#include <iostream>
#include "XmlHelper.h"

AdapterOfflineNotif::AdapterOfflineNotif(int userId, int notificationId,
            long time, int adapterId)
:AlertNotification(NAME_ADAPTER_OFFLINE, userId, notificationId, time), 
  mAdapterId(adapterId)
{
}

void AdapterOfflineNotif::addGcmData(JsonNotificationBuilder *builder) {
  builder->addData(JSON_DATA_ADAPTER_ID, Utils::intToString(mAdapterId));
}

void AdapterOfflineNotif::addDbXmlData(stringstream *ss) {
  XmlHelper::tagWithValue(ss, DATA_ADAPTER_ID, Utils::intToString(mAdapterId));
}

AdapterOfflineNotif::~AdapterOfflineNotif() {
}

