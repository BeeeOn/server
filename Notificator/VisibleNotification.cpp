/* 
 * File:   VissibleNotification.cpp
 * Author: Martin
 * 
 * Created on 1. prosinec 2014, 23:22
 */

#include "VisibleNotification.h"

VisibleNotification::VisibleNotification(string email, int notificationId,
        vector<string> gcmIds, long time, string action, string message)
: BaseGcmNotification(email, notificationId, gcmIds, time, action),
mMessage(message) {
}

string VisibleNotification::getMessage() {
    return mMessage;
}

string VisibleNotification::getEmail() {
    return BaseGcmNotification::getEmail();
}

string VisibleNotification::getNotificationId() {
    return BaseGcmNotification::getNotificationId();
}

string VisibleNotification::getGcmIds() {
    return BaseGcmNotification::getGcmIds();
}

string VisibleNotification::getTime() {
    return BaseGcmNotification::getTime();
}

string VisibleNotification::getAction() {
    return BaseGcmNotification::getAction();
}

VisibleNotification::~VisibleNotification() {
}

