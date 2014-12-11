/* 
 * File:   AlertNotification.cpp
 * Author: Martin
 * 
 * Created on 1. prosinec 2014, 23:56
 */

#include "AlertNotification.h"

AlertNotification::AlertNotification(string email, int notificationId,
        vector<string> gcmIds, long time, string action, string message)
: VisibleNotification(email, notificationId, gcmIds, time, action, message),
        TYPE_CONTROL("alert")
{

}

string AlertNotification::getType() {
    return TYPE_CONTROL;
}

AlertNotification::~AlertNotification() {
}

