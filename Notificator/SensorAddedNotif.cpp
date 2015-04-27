/* 
 * File:   SensorAddedNotif.cpp
 * Author: Martin
 * 
 */

#include "SensorAddedNotif.h"
#include "JsonNotificationBuilder.h"
#include "Constants.h"    
#include <iostream>

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

string SensorAddedNotif::getDbXml() {
    // FIXME
    string ahoj = "a";
    return ahoj;
}

SensorAddedNotif::~SensorAddedNotif() {
}

