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
  string stringIds = getGcmIds(ids);
  string msg = getGcmMsg(stringIds);

  Notificator::sendGcm(msg);

  //TODO
  vector<string> ahoj;
  return ahoj;
}

string BaseNotification::getUserId() {
  return Utils::intToString(mUserId);
  /*
  string number;
  stringstream ss;
  ss << mUserId;
  ss >> number;
  return number;
  */
}

string BaseNotification::getId() {
  return Utils::intToString(mNotificationId);
  /*
  string number;
  stringstream ss;
  ss << mNotificationId;
  ss >> number;
  return number;
  */
}

string BaseNotification::getGcmIds(vector<string>* ids) {
    stringstream ss;
    
    ss << "[";
    
    if (ids->size() > 0) {
        ss << "\"" << (*ids)[0] << "\"";
    }

    for (unsigned int i = 1; i < ids->size(); i++) {
        ss << ",\"" << (*ids)[i] << "\"";
    }

    ss << "]";
    
    return ss.str();
}

string BaseNotification::getTime() {
  return Utils::longToString(mTime);
  /*
  string number;
    stringstream ss;
    ss << mTime;
    ss >> number;
    return number;
*/
}

string BaseNotification::getName() {
  return mName;
}
