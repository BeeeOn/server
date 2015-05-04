/* 
 * File:   SensorAddedNotif.cpp
 * Author: Martin
 * 
 */

#include "SensorAddedNotif.h"
#include "JsonNotificationBuilder.h"
#include "Constants.h"    
#include <iostream>
#include "XmlHelper.h"

SensorAddedNotif::SensorAddedNotif(int userId, int notificationId,
            long time, int adapterId, 
            string deviceId)
:InfoNotification(NAME_SENSOR_ADDED ,userId, notificationId, time), 
  mAdapterId(adapterId), mDeviceId(deviceId)
{
}

void SensorAddedNotif::addGcmData(JsonNotificationBuilder *builder) {
  builder->addData(JSON_DATA_ADAPTER_ID, Utils::intToString(mAdapterId));
  builder->addData(JSON_DATA_DEVICE_ID, mDeviceId);
}

void SensorAddedNotif::addDbXmlData(stringstream *ss) {
    XmlHelper::tagWithValue(ss, DATA_ADAPTER_ID, Utils::intToString(mAdapterId));
    XmlHelper::tagWithValue(ss, DATA_DEVICE_ID, mDeviceId);
}

SensorAddedNotif::~SensorAddedNotif() {
}

