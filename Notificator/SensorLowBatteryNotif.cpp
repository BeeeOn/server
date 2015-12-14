/* 
 * File:   SensorLowBatteryNotif.cpp
 * Author: Martin
 * 
 */

#include "SensorLowBatteryNotif.h"
#include "JsonNotificationBuilder.h"
#include "Constants.h"    
#include <iostream>
#include "XmlHelper.h"

SensorLowBatteryNotif::SensorLowBatteryNotif(int userId, int notificationId,
            long time, long long int adapterId, 
            string deviceId, int battery)
:AlertNotification(NAME_SENSOR_LOW_BATTERY, userId, notificationId, time), 
  mAdapterId(adapterId), mDeviceId(deviceId), mBattery(battery)
{
}

void SensorLowBatteryNotif::addGcmData(JsonNotificationBuilder *builder) {
  builder->addData(JSON_DATA_ADAPTER_ID, Utils::longToString(mAdapterId));
  builder->addData(JSON_DATA_DEVICE_ID, mDeviceId);
  builder->addData(JSON_DATA_BATTERY, Utils::intToString(mBattery));
}

void SensorLowBatteryNotif::addDbXmlData(stringstream *ss) {
    XmlHelper::tagWithValue(ss, DATA_ADAPTER_ID, Utils::longToString(mAdapterId));
    XmlHelper::tagWithValue(ss, DATA_DEVICE_ID, mDeviceId);
}

SensorLowBatteryNotif::~SensorLowBatteryNotif() {

}

