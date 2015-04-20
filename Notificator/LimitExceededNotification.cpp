/* 
 * File:   LimitExceededNotification.cpp
 * Author: Martin
 * 
 */

#include "LimitExceededNotification.h"
#include "JsonNotificationBuilder.h"
#include "Constants.h"    

LimitExceededNotification::LimitExceededNotification(int userId, int notificationId,
            vector<string> gcmIds, long time, string message, int adapterId, 
            string deviceId, int type)
:InfoNotification("warchdog" ,userId, notificationId, gcmIds, time, message), 
  mAdapterId(adapterId), mType(type),  mDeviceId(deviceId)
{
}

string LimitExceededNotification::getJson() {
    
    JsonNotificationBuilder builder;
    return builder
        .registrationIds(getGcmIds())
        .addData(JSON_DATA_USER_ID, getUserId())
        .addData(JSON_DATA_TIME, getTime())
        .addData(JSON_DATA_TYPE, getType())
        .addData(JSON_DATA_MSGID, getId())
        .addData(JSON_DATA_MESSAGE, getMessage())
        //.addData(JSON)
        .build();

}

string LimitExceededNotification::getDbXml() {
    // FIXME
    string ahoj = "a";
    return ahoj;
}

LimitExceededNotification::~LimitExceededNotification() {
}

