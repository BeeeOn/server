/* 
 * File:   UriNotif.cpp
 * Author: Martin
 * 
 */

#include "UriNotif.h"
#include "JsonNotificationBuilder.h"
#include "Constants.h"    
#include <iostream>
UriNotif::UriNotif(int userId, int notificationId,
            long time, string message, string uri)
:AdvertNotification(NAME_URI, userId, notificationId, time), 
mMsg(message), mUri(uri)
{
}

void UriNotif::addGcmData(JsonNotificationBuilder *builder) {
  builder->addData(JSON_DATA_MESSAGE, mMsg);
  builder->addData(JSON_DATA_URI, mUri);
}

string UriNotif::getDbXml() {
    // FIXME
    string ahoj = "a";
    return ahoj;
}

UriNotif::~UriNotif() {
}

