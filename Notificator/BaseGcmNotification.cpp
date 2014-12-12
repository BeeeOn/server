#include "BaseGcmNotification.h"
#include <sstream>

using namespace std;

BaseGcmNotification::BaseGcmNotification(string email, int notificationId,
        vector<string> gcmIds, long time, string action)
: Notification(), mEmail(email), mGcmIds(gcmIds), mNotificationId(notificationId),
mTime(time), mAction(action) {

}

string BaseGcmNotification::getEmail() {
    return mEmail;
}

string BaseGcmNotification::getNotificationId() {
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

string BaseGcmNotification::getAction() {
    return mAction;
}



