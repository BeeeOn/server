#include "BaseNotification.h"
#include "Notificator.h"
#include "Utils.h"
#include <sstream>
#include <iostream>
using namespace std;

BaseNotification::BaseNotification(string name, int userId, int notificationId, long time)
: Notification(), 
  mName(name), 
  mUserId(userId), 
  mNotificationId(notificationId), 
  mTime(time) {
}

vector<string> BaseNotification::sendGcm(vector<string> *ids) {
  JsonNotificationBuilder builder;
  builder =  builder.registrationIds(ids)
         .addData(JSON_DATA_NAME, getName())
         .addData(JSON_DATA_USER_ID, getUserId())
         .addData(JSON_DATA_TIME, getTime())
         .addData(JSON_DATA_TYPE, getType())
         .addData(JSON_DATA_MSGID, getId());

  addGcmData(&builder);

  Notificator::sendGcm(builder.build());

  //TODO
  vector<string> gcmDelete;
  gcmDelete.clear();
  return gcmDelete;
}

string BaseNotification::getUserId() {
  return Utils::intToString(mUserId);
}

string BaseNotification::getId() {
  return Utils::intToString(mNotificationId);
}

string BaseNotification::getTime() {
  return Utils::longToString(mTime);
}

string BaseNotification::getName() {
  return mName;
}
