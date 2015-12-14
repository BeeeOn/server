/* 
 * File:   UriNotif.cpp
 * Author: Martin
 * 
 */

#include "UriNotif.h"
#include "JsonNotificationBuilder.h"
#include "Constants.h"    
#include <iostream>
#include "XmlHelper.h"

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

void UriNotif::addDbXmlData(stringstream *ss) {
  XmlHelper::tagWithValue(ss, DATA_MESSAGE, mMsg);
  XmlHelper::tagWithValue(ss, DATA_URI, mUri);
}

UriNotif::~UriNotif() {
}

