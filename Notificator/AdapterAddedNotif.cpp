/* 
 * File:   AdapterAddedNotif.cpp
 * Author: Martin
 * 
 */

#include "AdapterAddedNotif.h"
#include "JsonNotificationBuilder.h"
#include "Constants.h"    
#include <iostream>
#include "XmlHelper.h"

AdapterAddedNotif::AdapterAddedNotif(int userId, int notificationId,
            long time, int adapterId)
:InfoNotification(NAME_ADAPTER_ADDED, userId, notificationId, time), 
  mAdapterId(adapterId)
{
}

void AdapterAddedNotif::addGcmData(JsonNotificationBuilder *builder) {
  builder->addData(JSON_DATA_ADAPTER_ID, Utils::intToString(mAdapterId));
}

void AdapterAddedNotif::addDbXmlData(stringstream *ss) {
  XmlHelper::tagWithValue(ss, DATA_ADAPTER_ID, Utils::intToString(mAdapterId));
}

AdapterAddedNotif::~AdapterAddedNotif() {
}

