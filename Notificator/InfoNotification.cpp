/* 
 * File:   InfoNotification.cpp
 * Author: Martin
 * 
 * Created on 2. prosinec 2014, 0:13
 */

#include "InfoNotification.h"
#include "Constants.h"

InfoNotification::InfoNotification(string name, int userId, int notificationId,
        long time)
: BaseNotification(name, userId, notificationId, time)
{
}

string InfoNotification::getType() {
    return TYPE_INFO;
}

int InfoNotification::getLevel() {
    return LEVEL_INFO;
}

InfoNotification::~InfoNotification() {
}

