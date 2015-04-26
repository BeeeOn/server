/* 
 * File:   AdvertNotification.cpp
 * Author: Martin
 * 
 * Created on 2. prosinec 2014, 0:17
 */

#include "AdvertNotification.h"
#include "Constants.h"

AdvertNotification::AdvertNotification(string name, int userId, 
    int notificationId, long time) 
: BaseNotification(name, userId, notificationId, time)
{
}

int AdvertNotification::getLevel() {
    return LEVEL_ADVERT;
}

string AdvertNotification::getType() {
    return TYPE_ADVERT;
}

AdvertNotification::~AdvertNotification() {
}

