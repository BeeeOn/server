/* 
 * File:   InfoNotification.cpp
 * Author: Martin
 * 
 * Created on 2. prosinec 2014, 0:13
 */

#include "InfoNotification.h"

InfoNotification::InfoNotification(string name, int userId, int notificationId,
        vector<string> gcmIds, long time, string message)
: VisibleNotification(name, userId, notificationId, gcmIds, time, message),
TYPE_INFO("info"), LEVEL_INFO(300) {
}

string InfoNotification::getType() {
    return TYPE_INFO;
}

int InfoNotification::getLevel() {
    return LEVEL_INFO;
}

string InfoNotification::getMessage() {
    return VisibleNotification::getMessage();
}
string InfoNotification::getUserId() {
    return VisibleNotification::getUserId();
}
string InfoNotification::getId() {
    return VisibleNotification::getId();
}
string InfoNotification::getGcmIds() {
    return VisibleNotification::getGcmIds();
}
string InfoNotification::getTime() {
    return VisibleNotification::getTime();
}

InfoNotification::~InfoNotification() {
}

