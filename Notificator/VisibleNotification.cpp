/* 
 * File:   VissibleNotification.cpp
 * Author: Martin
 * 
 * Created on 1. prosinec 2014, 23:22
 */

#include "VisibleNotification.h"

VisibleNotification::VisibleNotification(string name, int userId, 
    int notificationId, vector<string> gcmIds, long time, 
    string message)
: BaseGcmNotification(name, userId, notificationId, gcmIds, time),
mMessage(message) {
}

string VisibleNotification::getMessage() {
    return mMessage;
}

string VisibleNotification::getUserId() {
    return BaseGcmNotification::getUserId();
}

string VisibleNotification::getId() {
    return BaseGcmNotification::getId();
}

string VisibleNotification::getGcmIds() {
    return BaseGcmNotification::getGcmIds();
}

string VisibleNotification::getTime() {
    return BaseGcmNotification::getTime();
}

VisibleNotification::~VisibleNotification() {
}

