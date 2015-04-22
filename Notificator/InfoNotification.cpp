/* 
 * File:   InfoNotification.cpp
 * Author: Martin
 * 
 * Created on 2. prosinec 2014, 0:13
 */

#include "InfoNotification.h"
#include "Constants.h"

InfoNotification::InfoNotification(string name, int userId, int notificationId,
        long time, string message)
: VisibleNotification(name, userId, notificationId, time, message)
{
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

string InfoNotification::getTime() {
    return VisibleNotification::getTime();
}

InfoNotification::~InfoNotification() {
}

