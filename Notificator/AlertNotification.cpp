/* 
 * File:   AlertNotification.cpp
 * Author: Martin
 * 
 * Created on 1. prosinec 2014, 23:56
 */

#include "AlertNotification.h"
#include "Constants.h"

AlertNotification::AlertNotification(string name, int userId, 
    int notificationId, long time)
: BaseNotification(name, userId, notificationId, time)
{

}

bool AlertNotification::saveToDb() {
  return true;
}

int AlertNotification::getLevel() {
    return LEVEL_ALERT;
}

string AlertNotification::getType() {
    return TYPE_ALERT;
}

AlertNotification::~AlertNotification() {

}

