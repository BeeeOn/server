/* 
 * File:   WatchdogNotif.cpp
 * Author: Martin
 * 
 */

#include "WatchdogNotif.h"
#include "JsonNotificationBuilder.h"
#include "Constants.h"    

WatchdogNotif::WatchdogNotif(int userId, int notificationId,
            long time, string message, int adapterId, 
            string deviceId, int sensorType)
:InfoNotification(NAME_WATCHDOG ,userId, notificationId, time, message), 
  mAdapterId(adapterId), mSensorType(sensorType), mDeviceId(deviceId)
{
}

string WatchdogNotif::getGcmMsg(string ids) {
    
    JsonNotificationBuilder builder;
    return builder
        .registrationIds(ids)
        .addData(JSON_DATA_NAME, getName())
        .addData(JSON_DATA_USER_ID, getUserId())
        .addData(JSON_DATA_TIME, getTime())
        .addData(JSON_DATA_TYPE, getType())
        .addData(JSON_DATA_MSGID, getId())
        .addData(JSON_DATA_MESSAGE, getMessage())
        .build();
}

string WatchdogNotif::getDbXml() {
    // FIXME
    string ahoj = "a";
    return ahoj;
}

WatchdogNotif::~WatchdogNotif() {
}

