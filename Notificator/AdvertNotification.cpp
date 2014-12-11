/* 
 * File:   AdvertNotification.cpp
 * Author: Martin
 * 
 * Created on 2. prosinec 2014, 0:17
 */

#include "AdvertNotification.h"

AdvertNotification::AdvertNotification(string email, int notificationId,
            vector<string> gcmIds, long time, string action, string message) 
: VisibleNotification(email, notificationId, gcmIds, time, action, message),
        TYPE_CONTROL("advert") {
}

string AdvertNotification::getType() {
    return TYPE_CONTROL;
}

AdvertNotification::~AdvertNotification() {
}

