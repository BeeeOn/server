/* 
 * File:   LimitExceededNotification.cpp
 * Author: Martin
 * 
 */

#include "LimitExceededNotification.h"
#include "JsonNotificationBuilder.h"
#include "Constants.h"    

LimitExceededNotification::LimitExceededNotification(string email, int notificationId,
            vector<string> gcmIds, long time, string message, int adapterId, 
            string deviceId, int type, int offset)
:InfoNotification(email, notificationId, gcmIds, time, ACTION_DEVICE, message), 
  mAdapterId(adapterId), mType(type), mOffset(offset), mDeviceId(deviceId)
{
}

string LimitExceededNotification::getJson() {
    
    JsonNotificationBuilder builder;
    return builder
        .registrationIds(getGcmIds())
        .addData(JSON_DATA_EMAIL, getEmail())
        .addData(JSON_DATA_ACTION, getAction())
        .addData(JSON_DATA_TIME, getTime())
        .addData(JSON_DATA_TYPE, getType())
        .addData(JSON_DATA_MSGID, getNotificationId())
        .addData(JSON_DATA_MESSAGE, getMessage())
        //.addData(JSON)
        .build();

}

LimitExceededNotification::~LimitExceededNotification() {
}

