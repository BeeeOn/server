/* 
 * File:   WatchdogNotif.cpp
 * Author: Martin
 * 
 */

#include "WatchdogNotif.h"
#include "JsonNotificationBuilder.h"
#include "Constants.h"    
#include <iostream>
WatchdogNotif::WatchdogNotif(int userId, int notificationId,
            long time, string message, int adapterId, 
            string deviceId, int deviceType)
:InfoNotification(NAME_WATCHDOG ,userId, notificationId, time), 
  mAdapterId(adapterId), mDeviceType(deviceType), mDeviceId(deviceId), mMsg(message)
{
}

void WatchdogNotif::addGcmData(JsonNotificationBuilder *builder) {
  (*builder)
      .addData(JSON_DATA_MESSAGE, mMsg);
  (*builder)
      .addData(JSON_DATA_ADAPTER_ID, Utils::intToString(mAdapterId));
  (*builder)
      .addData(JSON_DATA_DEVICE_ID, mDeviceId);
  (*builder)
      .addData(JSON_DATA_DEVICE_TYPE, Utils::intToString(mDeviceType));
}

string WatchdogNotif::getDbXml() {
    // FIXME
    string ahoj = "a";
    return ahoj;
}

WatchdogNotif::~WatchdogNotif() {
}

