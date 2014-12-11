#include "ControlNotification.h"

using namespace std;



ControlNotification::ControlNotification(string email, int notificationId,
        vector<string> gcmIds, long time, string action)
: BaseGcmNotification(email, notificationId, gcmIds, time, action),
TYPE_CONTROL("control") {

}

string ControlNotification::getType() {
    return TYPE_CONTROL;
}
