/* 
 * File:   WatchdogNotif.cpp
 * Author: Martin
 * 
 */

#include "WatchdogNotif.h"
#include "JsonNotificationBuilder.h"
#include "Constants.h"    
#include <iostream>
#include "XmlHelper.h"

WatchdogNotif::WatchdogNotif(int userId, int notificationId,
            long time, string message, int adapterId, 
            string deviceId, int deviceType, int algId)
:InfoNotification(NAME_WATCHDOG ,userId, notificationId, time), 
  mAdapterId(adapterId), mDeviceType(deviceType), mDeviceId(deviceId), mMsg(message), mAlgId(algId)
{
}

void WatchdogNotif::addGcmData(JsonNotificationBuilder *builder) {
  builder->addData(JSON_DATA_MESSAGE, mMsg);
  builder->addData(JSON_DATA_ADAPTER_ID, Utils::intToString(mAdapterId));
  builder->addData(JSON_DATA_DEVICE_ID, mDeviceId);
  builder->addData(JSON_DATA_DEVICE_TYPE, Utils::intToString(mDeviceType));
  builder->addData(JSON_DATA_ALG_ID, Utils::intToString(mAlgId));
}

void WatchdogNotif::addDbXmlData(stringstream *ss) {
    XmlHelper::tagWithValue(ss, DATA_MESSAGE, mMsg);
    XmlHelper::tagWithValue(ss, DATA_ADAPTER_ID, Utils::intToString(mAdapterId));
    XmlHelper::tagWithValue(ss, DATA_DEVICE_ID, mDeviceId);
    XmlHelper::tagWithValue(ss, DATA_DEVICE_TYPE, Utils::intToString(mDeviceType));
    XmlHelper::tagWithValue(ss, DATA_ALG_ID, Utils::intToString(mAlgId));

}


WatchdogNotif::~WatchdogNotif() {
}

