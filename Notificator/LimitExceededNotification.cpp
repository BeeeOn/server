/* 
 * File:   LimitExceededNotification.cpp
 * Author: Martin
 * 
 */

#include "LimitExceededNotification.h"
#include "JsonNotificationBuilder.h"
#include "JsonUtil.h"
    

LimitExceededNotification::LimitExceededNotification(string email, int notificationId,
            vector<string> gcmIds, long time, string message)
:InfoNotification(email, notificationId, gcmIds, time, "device", message){
}

string LimitExceededNotification::getJson() {
    
    JsonNotificationBuilder builder;
    return builder
        .registrationIds(getGcmIds())
        .addData(DATA_EMAIL, getEmail())
        .addData(DATA_ACTION, getAction())
        .addData(DATA_TIME, getTime())
        .addData(DATA_TYPE, getType())
        .addData(DATA_MSGID, getNotificationId())
        .addData(DATA_MESSAGE, getMessage())
        .build();

}

LimitExceededNotification::~LimitExceededNotification() {
}

