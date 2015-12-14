/* 
 * File:   SensorLowSignalNotif.cpp
 * Author: Martin
 * 
 */

#include "SensorLowSignalNotif.h"
#include "JsonNotificationBuilder.h"
#include "Constants.h"    
#include <iostream>
#include "XmlHelper.h"

SensorLowSignalNotif::SensorLowSignalNotif(int userId, int notificationId,
            long time, int adapterId, 
            string deviceId, int signal)
:AlertNotification(NAME_SENSOR_LOW_SIGNAL, userId, notificationId, time), 
  mAdapterId(adapterId), mDeviceId(deviceId), mSignal(signal)
{
cout <<"int"<<endl;
}

SensorLowSignalNotif::SensorLowSignalNotif(int userId, int notificationId,
            long time, long long int adapterId, 
            string deviceId, int signal)
:AlertNotification(NAME_SENSOR_LOW_SIGNAL, userId, notificationId, time), 
  mAdapterId(adapterId), mDeviceId(deviceId), mSignal(signal)
{
cout <<"long"<<endl;
}

void SensorLowSignalNotif::addGcmData(JsonNotificationBuilder *builder) {
  builder->addData(JSON_DATA_ADAPTER_ID, Utils::longToString(mAdapterId));
  builder->addData(JSON_DATA_DEVICE_ID, mDeviceId);
  builder->addData(JSON_DATA_SIGNAL, Utils::intToString(mSignal));
}

void SensorLowSignalNotif::addDbXmlData(stringstream *ss) {
    XmlHelper::tagWithValue(ss, DATA_ADAPTER_ID, Utils::longToString(mAdapterId));
    XmlHelper::tagWithValue(ss, DATA_DEVICE_ID, mDeviceId);
}

SensorLowSignalNotif::~SensorLowSignalNotif() {

}

