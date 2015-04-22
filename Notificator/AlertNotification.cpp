/* 
 * File:   AlertNotification.cpp
 * Author: Martin
 * 
 * Created on 1. prosinec 2014, 23:56
 */

#include "AlertNotification.h"

AlertNotification::AlertNotification(string name, int userId, 
    int notificationId, vector<string> gcmIds, long time, 
    string message)
: VisibleNotification(name, userId, notificationId, gcmIds, 
    time, message), TYPE_ALERT("alert"), LEVEL_ALERT(400)
{

}

int AlertNotification::getLevel() {
    return LEVEL_ALERT;
}

string AlertNotification::getType() {
    return TYPE_ALERT;
}

AlertNotification::~AlertNotification() {

}

