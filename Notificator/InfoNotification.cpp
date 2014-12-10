/* 
 * File:   InfoNotification.cpp
 * Author: Martin
 * 
 * Created on 2. prosinec 2014, 0:13
 */

#include "InfoNotification.h"

InfoNotification::InfoNotification(string email, int notificationId,
        vector<string> gcmIds, long time, string action, string message)
: VisibleNotification(email, notificationId, gcmIds, time, action, message),
TYPE_CONTROL("info") {
}

string InfoNotification::getType() {
    return TYPE_CONTROL;
}

string InfoNotification::getMessage() {
    return VisibleNotification::getMessage();
}
string InfoNotification::getEmail() {
    return VisibleNotification::getEmail();
}
string InfoNotification::getNotificationId() {
    return VisibleNotification::getNotificationId();
}
string InfoNotification::getGcmIds() {
    return VisibleNotification::getGcmIds();
}
string InfoNotification::getTime() {
    return VisibleNotification::getTime();
}
string InfoNotification::getAction() {
    return VisibleNotification::getAction();
}

InfoNotification::~InfoNotification() {
}

