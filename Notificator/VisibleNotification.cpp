/* 
 * File:   VissibleNotification.cpp
 * Author: Martin
 * 
 * Created on 1. prosinec 2014, 23:22
 */

#include "VisibleNotification.h"

VisibleNotification::VisibleNotification(string name, int userId, 
    int notificationId, long time, string message)
: BaseNotification(name, userId, notificationId, time),
mMessage(message) {
}

string VisibleNotification::getMessage() {
    return mMessage;
}

string VisibleNotification::getUserId() {
    return BaseNotification::getUserId();
}

string VisibleNotification::getId() {
    return BaseNotification::getId();
}

string VisibleNotification::getTime() {
    return BaseNotification::getTime();
}

VisibleNotification::~VisibleNotification() {
}

