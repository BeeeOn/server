#include "BaseGcmNotification.h"
#include <sstream>

using namespace std;

BaseGcmNotification::BaseGcmNotification(string name, int userId, int notificationId, vector<string> gcmIds, long time)
: Notification(), 
  mName(name), 
  mUserId(userId), 
  mGcmIds(gcmIds), 
  mNotificationId(notificationId), 
  mTime(time) {
}

vector<string> BaseGcmNotification::send() {
  // TODO
  vector<string> ahoj;
  return ahoj;
}

string BaseGcmNotification::getUserId() {
    string number;
    stringstream ss;
    ss << mUserId;
    ss >> number;
    return number;
}

string BaseGcmNotification::getId() {
    string number;
    stringstream ss;
    ss << mNotificationId;
    ss >> number;
    return number;
}

string BaseGcmNotification::getGcmIds() {
    stringstream ss;
    ss << "[";
    if (mGcmIds.size() > 0) {
        ss << "\"" << mGcmIds[0] << "\"";
    }
    for (unsigned int i = 1; i < mGcmIds.size(); i++) {
        ss << ",\"" << mGcmIds[i] << "\"";
    }
    ss << "]";
    return ss.str();
}

string BaseGcmNotification::getTime() {
    string number;
    stringstream ss;
    ss << mTime;
    ss >> number;
    return number;
}


string BaseGcmNotification::getName() {
  return mName;
}

